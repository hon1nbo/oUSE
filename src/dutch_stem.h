/***************************************************************************
                          dutch_stem.h  -  description
                             -------------------
    begin               : Sat May 11 2004
    copyright           : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __DUTCH_STEM_H__
#define __DUTCH_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    Dutch includes the following accented forms 
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#228;   &#235;   &#239;   &#246;   &#252;   &#225;   &#233;   &#237;   &#243;   &#250;   &#232;
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class dutch_stem : public stem<string_typeT>
        {
    public:
        dutch_stem() : m_step_2_succeeded(false) {}
        //---------------------------------------------
        /**@param text string to stem*/
        void operator()(string_typeT& text)
            {
            //First, remove all umlaut and acute accents
            remove_dutch_umlauts(text);
            remove_dutch_acutes(text);

            if (text.length() < 3)
                {
                return;
                }

            //reset internal data
            m_step_2_succeeded = false;
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);

            //Hash initial y, y after a vowel, and i between vowels
            hash_dutch_yi(text, DUTCH_VOWELS);

            find_r1(text, DUTCH_VOWELS);
            find_r2(text, DUTCH_VOWELS);
            //R1 must have at least 3 characters in front of it
            if (stem<string_typeT>::get_r1() < 3)
                {
                stem<string_typeT>::set_r1(3);
                }

            step_1(text);
            step_2(text);
            step_3a(text);
            step_3b(text);
            step_4(text);

            //unhash I and Y back into their original form 
            unhash_dutch_yi(text);
            }
    private:
        /**Step 1:
        Search for the longest among the following suffixes, and perform the action indicated: 

            -# heden 
                - replace with heid if in R1

            -# en   ene 
                - delete if in R1 and preceded by a valid en-ending, and then undouble the ending

            -# s   se 
                - delete if in R1 and preceded by a valid s-ending*/ 
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            if (is_suffix(text,/*heden*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                {
                if (stem<string_typeT>::get_r1() <= text.length()-5)
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    text[text.length()-2] = LOWER_I;
                    text[text.length()-1] = LOWER_D;
                    }
                }
            ///Define a valid en-ending as a non-vowel, and not gem.
            else if (is_suffix(text,/*ene*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E)    )
                {
                if (stem<string_typeT>::get_r1() <= text.length()-3 &&
                    !string_util::is_one_of(text[text.length()-4], DUTCH_VOWELS) &&
                    (text.length() < 6 ||
                    //"gem" in front of "en" ending
                    !(is_either<wchar_t>(text[text.length()-6], LOWER_G, UPPER_G) &&
                    is_either<wchar_t>(text[text.length()-5], LOWER_E, UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_M, UPPER_M) ) ) )
                    {
                    text.erase(text.length()-3);
                    //undouble dd, kk, tt
                    if (string_util::is_one_of(text[text.length()-1], DUTCH_KDT) &&
                        string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]))
                        {
                        text.erase(text.length()-1);
                        }
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                {
                if (stem<string_typeT>::get_r1() <= text.length()-2 &&
                    !string_util::is_one_of(text[text.length()-3], DUTCH_VOWELS) &&
                    (text.length() < 5 ||
                    !(is_either<wchar_t>(text[text.length()-5], LOWER_G, UPPER_G) &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_E, UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_M, UPPER_M) ) ) )
                    {
                    text.erase(text.length()-2);
                    //undouble dd, kk, tt
                    if (string_util::is_one_of(text[text.length()-1], DUTCH_KDT) &&
                        string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]) )
                        {
                        text.erase(text.length()-1);
                        }
                    update_r_sections(text);
                    }
                return;
                }
            else if (text.length() >= 3 &&
                is_suffix(text,/*se*/LOWER_S, UPPER_S, LOWER_E, UPPER_E) &&
                !string_util::is_one_of(text[text.length()-3], DUTCH_S_ENDING))
                {
                if (stem<string_typeT>::get_r1() <= text.length()-2)
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return;
                    }
                }
            ///Define a valid s-ending as a non-vowel other than j
            else if (text.length() >= 2 &&
                is_suffix(text, LOWER_S, UPPER_S) &&
                !string_util::is_one_of(text[text.length()-2], DUTCH_S_ENDING))
                {
                if (stem<string_typeT>::get_r1() <= text.length()-1)
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    return;
                    }
                }
            }

        /**Step 2:
        Delete suffix e if in R1 and preceded by a non-vowel, and then undouble the ending*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            if (is_suffix(text, LOWER_E, UPPER_E) )
                {
                if (text.length() >= 2 &&
                    stem<string_typeT>::get_r1() <= text.length()-1 &&
                    !string_util::is_one_of(text[text.length()-2], DUTCH_VOWELS) )
                    {
                    //watch out for vowel I/Y vowel
                    if (text.length() >= 3 &&
                        string_util::is_one_of(text[text.length()-2], L"iyIY") &&
                        string_util::is_one_of(text[text.length()-3], DUTCH_VOWELS) )
                        {
                        return;
                        }
                    else
                        {
                        text.erase(text.length()-1);
                        //undouble dd, kk, tt
                        if (string_util::is_one_of(text[text.length()-1], DUTCH_KDT) &&
                            string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]))
                            {
                            text.erase(text.length()-1);
                            }
                        update_r_sections(text);
                        m_step_2_succeeded = true;
                        }
                    }
                }
            }
        /**Step 3a: heid 
        delete heid if in R2 and not preceded by c, and treat a preceding en as in step 1(b).*/
        //---------------------------------------------
        void step_3a(string_typeT& text)
            {
            if (is_suffix(text,/*heid*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_D, UPPER_D) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_r2() <= text.length()-4 &&
                    is_neither<wchar_t>(text[text.length()-5], LOWER_C, UPPER_C) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    if (stem<string_typeT>::get_r1() <= text.length()-2 &&
                        is_suffix(text, LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                        {
                        if ((!string_util::is_one_of(text[text.length()-3], DUTCH_VOWELS) ||
                            (string_util::is_one_of(text[text.length()-3], L"iyYI") &&
                            string_util::is_one_of(text[text.length()-4], DUTCH_VOWELS))) &&
                            (text.length() < 5 ||
                            !(is_either<wchar_t>(text[text.length()-5], LOWER_G, UPPER_G) &&
                            is_either<wchar_t>(text[text.length()-4], LOWER_E, UPPER_E) &&
                            is_either<wchar_t>(text[text.length()-3], LOWER_M, UPPER_M) ) ) )
                            {
                            text.erase(text.length()-2);
                            //undouble dd, kk, tt
                            if (string_util::is_one_of(text[text.length()-1], DUTCH_KDT) &&
                                string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]))
                                {
                                text.erase(text.length()-1);
                                }
                            update_r_sections(text);
                            }
                        }
                    return;
                    }
                }
            }
        /**Step 3b: d-suffixes (*) 
        Search for the longest among the following suffixes, and perform the action indicated. 

            - end   ing
                - delete if in R2.
                - if preceded by ig, delete if in R2 and not preceded by e, otherwise undouble the ending.

            - ig
                - delete if in R2 and not preceded by e.

            - lijk
                - delete if in R2, and then repeat step 2.

            - baar
                - delete if in R2.

            - bar
                - delete if in R2 and if step 2 actually removed an e.*/
        //---------------------------------------------
        void step_3b(string_typeT& text)
            {
            if (delete_if_is_in_r2(text,/*end*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D) ||
                delete_if_is_in_r2(text,/*ing*/LOWER_I, UPPER_I, LOWER_N, UPPER_N, LOWER_G, UPPER_G) )
                {
                update_r_sections(text);
                if (text.length() > 3)
                    {
                    if (is_neither<wchar_t>(text[text.length()-3], LOWER_E , UPPER_E) &&
                        delete_if_is_in_r2(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G) )
                        {
                        update_r_sections(text);
                        return;
                        }
                    else
                        {
                        if (string_util::is_one_of(text[text.length()-1], DUTCH_KDT) &&
                            string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]))
                            {
                            text.erase(text.length()-1);
                            update_r_sections(text);
                            return;
                            }
                        }
                    }
                return;
                }
            else if (!(text.length() >= 3 && is_either<wchar_t>(text[text.length()-3], LOWER_E, UPPER_E) ) &&
                    delete_if_is_in_r2(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G) )
                {
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r2(text,/*baar*/LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_A, UPPER_A, LOWER_R, UPPER_R) )
                {
                return;
                }
            else if (delete_if_is_in_r2(text,/*lijk*/LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_J, UPPER_J, LOWER_K, UPPER_K) )
                {
                step_2(text);
                return;
                }
            else if (m_step_2_succeeded &&
                delete_if_is_in_r2(text,/*bar*/LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_R, UPPER_R) )
                {
                return;
                }
            }
        /**Step 4: undouble vowel 
            If the words ends CVD, where C is a non-vowel, D is a non-vowel other than I,
            and V is double a, e, o or u, remove one of the vowels from V
            (for example, maan -> man, brood -> brod).*/
        //------------------------------------------------------
        void step_4(string_typeT& text)
            {
            //undouble consecutive (same) consonants
            if (text.length() >= 4 &&
                !string_util::is_one_of(text[text.length()-4], DUTCH_VOWELS) &&
                !string_util::is_one_of(text[text.length()-1], DUTCH_VOWELS) &&
                text[text.length()-1] != LOWER_I_HASH &&
                text[text.length()-1] != UPPER_I_HASH &&
                string_util::is_one_of(text[text.length()-2], L"aeouAEOU") &&
                string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-3]) )
                {
                text.erase(text.end()-2, text.end()-1);
                update_r_sections(text);
                }
            }
        //internal data specific to Dutch stemmer
        bool m_step_2_succeeded;
        };
    }

#endif //__DUTCH_STEM_H__
