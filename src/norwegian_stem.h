/***************************************************************************
                          norwegian_stem.h  -  description
                             -------------------
    begin                : Sat May 17 2004
    copyright            : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __NORWEGIAN_STEM_H__
#define __NORWEGIAN_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    The Norwegian alphabet includes the following additional letters:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#230;   &#229;   &#248;<br /><br />
    The following letters are vowels:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-a   e   i   o   u   y   &#230;   &#229;   &#248;<br /><br />
    R2 is not used: R1 is defined in the same way as in the German stemmer.<br /><br />

    Define a valid s-ending as one of<br />
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-b   c   d   f   g   h   j   l   m   n   o   p   r   t   v   y   z,<br />
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-or k not preceded by a vowel.
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class norwegian_stem : public stem<string_typeT>
        {
    public:
        //---------------------------------------------
        /**@param text string to stem*/
        void operator()(string_typeT& text)
            {
            if (text.length() < 3)
                {
                return;
                }
            trim_western_punctuation(text);

            //reset internal data
            stem<string_typeT>::reset_r_values();

            find_r1(text, NORWEGIAN_VOWELS);
            if (stem<string_typeT>::get_r1() == text.length() )
                {
                return;
                }
            //R1 must have at least 3 characters in front of it
            if (stem<string_typeT>::get_r1() < 3)
                {
                stem<string_typeT>::set_r1(3);
                }
            //norwegian does not use R2

            step_1(text);
            step_2(text);
            step_3(text);
            }
    private:
        /**Search for the longest among the following suffixes in R1, and perform the action indicated. 
            -#a e ede ande ende ane ene hetene en heten ar er heter as es edes
              endes enes hetenes ens hetens ers ets et het ast 
                -delete 
            -#s 
                -delete if preceded by a valid s-ending 

            -#erte   ert 
                -replace with er 

        (Of course the letter of the valid s-ending is not necessarily in R1)*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*hetenes*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*hetene*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*hetens*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*heter*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*heten*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*endes*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ande*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ende*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*edes*/LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*enes*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            if (is_suffix_in_r1(text,/*erte*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                text.erase(text.length()-2);
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r1(text,/*ers*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ets*/LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*het*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ast*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ens*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ene*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ane*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ede*/LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (is_suffix_in_r1(text,/*ert*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r1(text,/*et*/LOWER_E, UPPER_E, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*es*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*as*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ar*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (is_suffix_in_r1(text, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_r1() <= text.length()-1)
                    {
                    //only delete if a valid "s" ending
                    if (text.length() >= 2 &&
                        string_util::is_one_of(text[text.length()-2],
                        L"bcdfghjlmnoprtvyzBCDFGHJLMNOPRTVYZ") )
                        {
                        text.erase(text.length()-1);
                        update_r_sections(text);
                        return;
                        }
                    else if (text.length() >= 3 &&
                        is_either<wchar_t>(text[text.length()-2], LOWER_K, UPPER_K) &&
                        !string_util::is_one_of(text[text.length()-3], NORWEGIAN_VOWELS))
                        {
                        text.erase(text.length()-1);
                        update_r_sections(text);
                        return;
                        }
                    }
                }
            }

        /**If the word ends dt or vt in R1, delete the t.
        (For example, meldt -> meld, operativt -> operativ)*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            if (is_suffix_in_r1(text,/*dt*/LOWER_D, UPPER_D, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r1(text,/*vt*/LOWER_V, UPPER_V, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            }
        /**Search for the longest among the following suffixes in R1, and if found, delete. 

            -leg eleg ig eig lig elig els lov elov slov hetslov*/
        //---------------------------------------------
        void step_3(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*hetslov*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_S, UPPER_S, LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_V, UPPER_V, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*slov*/LOWER_S, UPPER_S, LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_V, UPPER_V, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*elov*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_V, UPPER_V, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*elig*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*eleg*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*lov*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_V, UPPER_V, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*els*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*lig*/LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*eig*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*leg*/LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                return;
                }
            }
        };
    }

#endif //__NORWEGIAN_STEM_H__
