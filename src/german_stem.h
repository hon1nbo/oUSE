/***************************************************************************
                          german_stem.h  -  description
                             -------------------
    begin                : Sat May 18 2004
    copyright            : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __GERMAN_STEM_H__
#define __GERMAN_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    German includes the following accented forms,<br/>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#228; &#246; &#252; and a special letter, &#223;, equivalent to double s.<br/><br/>

    The following letters are vowels:<br/>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-a e i o u y &#228; &#246; &#252;
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class german_stem : public stem<string_typeT>
        {
    public:
        //---------------------------------------------
        /**@param text string to stem.
           @param contract_transliterated_umlauts use variant algorithm.
           that expands \htmlonly "&#228;" \endhtmlonly to "ae", ect...*/
        void operator()(string_typeT& text,
                        bool contract_transliterated_umlauts = false)
            {
            if (text.length() < 2)
                {
                remove_german_umlauts(text);
                return;
                }

            //reset internal data
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);

            hash_german_yu(text, GERMAN_VOWELS);
            //change '&#223;' to "ss"
            string_util::replace_all<string_typeT>(text, string_typeT(1, ESZETT), L"ss");
            //German variant addition
            if (contract_transliterated_umlauts)
                {
                string_util::replace_all<string_typeT>(text, L"ae", string_typeT(1, LOWER_A_UMLAUTS));
                string_util::replace_all<string_typeT>(text, L"oe", string_typeT(1, LOWER_O_UMLAUTS));
                //ue to &#252;, if not in front of 'q'
                size_t start = 1;
                while (start != string_typeT::npos)
                    {
                    start = text.find(L"ue", start);
                    if (start == string_typeT::npos ||
                        is_either<wchar_t>(text[start-1], LOWER_Q, UPPER_Q) )
                        {
                        break;
                        }
                    text.replace(start, 2, string_typeT(1, LOWER_U_UMLAUTS));
                    }
                }

            find_r1(text, GERMAN_VOWELS);
            if (stem<string_typeT>::get_r1() == text.length() )
                {
                remove_german_umlauts(text);
                unhash_german_yu(text);
                return;
                }
            find_r2(text, GERMAN_VOWELS);
            //R1 must have at least 3 characters in front of it
            if (stem<string_typeT>::get_r1() < 3)
                {
                stem<string_typeT>::set_r1(3);    
                }

            step_1(text);
            step_2(text);
            step_3(text);

            //unhash special 'u' and 'y' back, and remove the umlaut accent from a, o and u. 
            remove_german_umlauts(text);
            unhash_german_yu(text);
            }
    private:
        /**\htmlonly
        Search for the longest among the following suffixes:<br />
            -e em en ern er es<br />
            -s (preceded by a valid s-ending)<br /><br />
        and delete if in R1. (Of course the letter of the valid s-ending is not necessarily in R1)<br /><br /> 

        (For example, &#228;ckern -> &#228;ck, ackers -> acker, armes -> arm)
        \endhtmlonly*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*ern*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_N, UPPER_N) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*es*/LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*em*/LOWER_E, UPPER_E, LOWER_M, UPPER_M) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text, LOWER_E, UPPER_E) )
                {
                return;
                }
            ///Define a valid s-ending as one of b, d, f, g, h, k, l, m, n, r or t.
            else if (is_suffix_in_r1(text, LOWER_S, UPPER_S) )
                {
                if (string_util::is_one_of(text[text.length()-2], L"bdfghklmnrtBDFGHKLMNRT") )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            }
        /**Search for the longest among the following suffixes:<br />
            -en er est<br />
            -st (preceded by a valid st-ending, itself preceded by at least 3 letters)<br /><br />
        and delete if in R1.<br /><br />

        (For example, derbsten -> derbst by step 1, and derbst -> derb by step 2,
        because b is a valid st-ending, and is preceded by just 3 letters)*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*est*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_T, UPPER_T) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                {
                return;
                }
            ///Define a valid st-ending as the same list, excluding letter r.
            else if (text.length() >= 6 &&
                    is_suffix_in_r1(text,/*st*/LOWER_S, UPPER_S, LOWER_T, UPPER_T) )
                {
                if (string_util::is_one_of(text[text.length()-3], L"bdfghklmntBDFGHKLMNT") )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            }
        /**@htmlonly
        Search for the longest among the following suffixes, and perform the action indicated.<br />
    
            -end ung<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ig, delete if in R2 and not preceded by e<br /><br />

            -ig ik isch<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2 and not preceded by e<br /><br />

            -lich heit<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by er or en, delete if in R1<br /><br />

            -keit<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by lich or ig, delete if in R2
            @endhtmlonly*/
        //---------------------------------------------
        void step_3(string_typeT& text) 
            {
            if (delete_if_is_in_r2(text,/*heit*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_T, UPPER_T) ||
                delete_if_is_in_r2(text,/*lich*/LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_H, UPPER_H) )
                {
                if (delete_if_is_in_r1(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R) ||
                    delete_if_is_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                    {
                    return;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*keit*/LOWER_K, UPPER_K, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_T, UPPER_T) )
                {
                if (delete_if_is_in_r2(text,/*lich*/LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_H, UPPER_H) ||
                    delete_if_is_in_r2(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G) )
                    {
                    return;
                    }
                return;
                }
            else if (is_suffix(text,/*isch*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_C, UPPER_C, LOWER_H, UPPER_H) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_r2() <= (text.length()-4) &&
                    is_neither<wchar_t>(text[text.length()-5], LOWER_E, UPPER_E) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    }
                }
            else if (delete_if_is_in_r2(text,/*end*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D) )
                {
                if (text.length() >= 3 &&
                    is_suffix_in_r2(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G)  &&
                    is_neither<wchar_t>(text[text.length()-3], LOWER_E, UPPER_E) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*ung*/LOWER_U, UPPER_U, LOWER_N, UPPER_N, LOWER_G, UPPER_G) )
                {
                if (text.length() >= 3 &&
                    is_suffix_in_r2(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G)  &&
                    is_neither<wchar_t>(text[text.length()-3], LOWER_E, UPPER_E) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_r2() <= text.length()-2 &&
                    is_neither<wchar_t>(text[text.length()-3], LOWER_E, UPPER_E) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                }
            else if (is_suffix(text,/*ik*/LOWER_I, UPPER_I, LOWER_K, UPPER_K) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_r2() <= text.length()-2 &&
                    is_neither<wchar_t>(text[text.length()-3], LOWER_E, UPPER_E) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                }
            }
        };
    }

#endif //__GERMAN_STEM_H__
