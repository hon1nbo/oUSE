/***************************************************************************
                          french_stem.h  -  description
                             -------------------
    begin                : Sat May 25 2004
    copyright            : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __FRENCH_STEM_H__
#define __FRENCH_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    Letters in French include the following accented forms:<br />

    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#226; &#224; &#231; &#235; &#233; &#234; &#232; &#239; &#238; &#244; &#251; &#249; 

    The following letters are vowels:<br />

    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-a e i o u y &#226; &#224; &#235; &#233; &#234; &#232; &#239; &#238; &#244; &#251; &#249;
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class french_stem : public stem<string_typeT>
        {
    public:
        french_stem() : m_step_1_successful(false) {}
        //---------------------------------------------
        /**@param text string to stem*/
        void operator()(string_typeT& text)
            {
            if (text.length() < 2)
                {
                return;
                }

            //reset internal data
            m_step_1_successful = false;
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);
            hash_french_yui(text, FRENCH_VOWELS);

            find_r1(text, FRENCH_VOWELS);
            find_r2(text, FRENCH_VOWELS);
            find_french_rv(text, FRENCH_VOWELS);

            size_t length = text.length();
            step_1(text);
            if (!m_step_1_successful)
                {
                step_2a(text);
                }
            if (length != text.length() )
                {
                step_3(text);
                }
            else
                {
                step_4(text);
                }
            step_5(text);
            step_6(text);

            unhash_french_yui(text);
            }
    private:
        bool ic_to_iqu(string_typeT& text)
            {
            if (is_suffix(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) )
                {
                if (stem<string_typeT>::get_r2() <= text.length()-2)
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return true;
                    }
                else
                    {
                    text.erase(text.length()-2);
                    text += L"iq";
                    text += LOWER_U_HASH;
                    //need to search for r2 again because the 'iq' added here may change that
                    find_r2(text, FRENCH_VOWELS);
                    find_french_rv(text, FRENCH_VOWELS);
                    return true;
                    }
                }
            return false;
            }
        /**\htmlonly
        Search for the longest among the following suffixes, and perform the action indicated.<br /><br />

            -ance iqUe isme able iste eux ances iqUes ismes ables istes<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br /><br />

            -atrice ateur ation atrices ateurs ations<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ic, delete if in R2, else replace by iqU<br /><br />

            -logie logies<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with log if in R2<br /><br />

            -usion ution usions utions<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with u if in R2<br /><br />

            -ence ences<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ent if in R2<br /><br />

            -ement ements<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in RV<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by iv, delete if in R2 (and if further preceded by at, delete if in R2), otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by eus, delete if in R2, else replace by eux if in R1, otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by abl or iqU, delete if in R2, otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by i&#232;r or I&#232;r, replace by i if in RV<br /><br />

            -it&#233; it&#233;s<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by abil, delete if in R2, else replace by abl, otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ic, delete if in R2, else replace by iqU, otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by iv, delete if in R2<br />

            -if ive ifs ives<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by at, delete if in R2 (and if further preceded by ic, delete if in R2, else replace by iqU)<br /><br />

            -eaux<br />
             &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with eau<br /><br />

            -aux<br />
             &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with al if in R1<br /><br />

            -euse euses<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2, else replace by eux if in R1<br /><br />

            -issement issements<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R1 and preceded by a non-vowel<br />

            -amment<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ant if in RV<br />

            -emment<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ent if in RV<br /><br />

            -ment ments<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if preceded by a vowel in RV
        \endhtmlonly*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            size_t length = text.length();
            if (is_suffix(text,/*issements*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 10 &&
                    stem<string_typeT>::get_r1() <= (text.length()-9) &&
                    !string_util::is_one_of(text[text.length()-10], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-9);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (is_suffix(text,/*issement*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 9 &&
                    stem<string_typeT>::get_r1() <= (text.length()-8) &&
                    !string_util::is_one_of(text[text.length()-9], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-8);
                    m_step_1_successful = true;
                    }
                return;
                }
            //7
            else if (delete_if_is_in_r2(text,/*atrices*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                if (length != text.length() )
                    {
                    ic_to_iqu(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            //6
            else if (is_suffix(text,/*amment*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (stem<string_typeT>::get_rv() <= (text.length()-6) )
                    {
                    text.replace(text.end()-5, text.end(), L"nt");
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix(text,/*emment*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (stem<string_typeT>::get_rv() <= (text.length()-6) )
                    {
                    text.replace(text.end()-5, text.end(), L"nt");
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix(text,/*logies*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-6) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*atrice*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_E, UPPER_E, false) ||
                    delete_if_is_in_r2(text,/*ateurs*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_R, UPPER_R, LOWER_S, UPPER_S, false) ||
                    delete_if_is_in_r2(text,/*ations*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                if (length != text.length() )
                    {
                    ic_to_iqu(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (is_suffix(text,/*usions*/LOWER_U, UPPER_U, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) ||
                    is_suffix(text,/*utions*/LOWER_U, UPPER_U, LOWER_T, UPPER_T, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-6) )
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*ements*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_S, UPPER_S, false) )
                {
                if (delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    }
                else if (is_suffix(text,/*eus*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_S, UPPER_S) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-3)
                        {
                        text.erase(text.length()-3);
                        update_r_sections(text);
                        }
                    else if (stem<string_typeT>::get_r1() <= text.length()-3)
                        {
                        text[text.length()-1] = LOWER_X;
                        }
                    }
                else if (delete_if_is_in_r2(text,/*abl*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L) )
                    {
                    //NOOP
                    }
                else if (text.length() >= 3 &&
                    (text[text.length()-3] == LOWER_I || text[text.length()-3] == UPPER_I) &&
                    (text[text.length()-2] == LOWER_Q || text[text.length()-2] == UPPER_Q) &&
                    is_either<wchar_t>(text[text.length()-1], LOWER_U_HASH, UPPER_U_HASH) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-3)
                        {
                        text.erase(text.length()-3);
                        update_r_sections(text);
                        }
                    }
                else if (is_suffix_in_rv(text,/*i&#232;r*/LOWER_I, UPPER_I, LOWER_E_GRAVE, UPPER_E_GRAVE, LOWER_R, UPPER_R) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                else if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= (text.length()-3) &&
                    (text[text.length()-2] == LOWER_E_GRAVE || text[text.length()-2] == UPPER_E_GRAVE) &&
                    (text[text.length()-1] == LOWER_R || text[text.length()-1] == UPPER_R) &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_I_HASH, UPPER_I_HASH) )
                    {
                    text.replace(text.end()-3, text.end(), L"i");
                    update_r_sections(text);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            //5
            else if (delete_if_is_in_r2(text,/*ateur*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_R, UPPER_R, false) ||
                    delete_if_is_in_r2(text,/*ation*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, false) )
                {
                if (length != text.length() )
                    {
                    ic_to_iqu(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (is_suffix(text,/*usion*/LOWER_U, UPPER_U, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N) ||
                    is_suffix(text,/*ution*/LOWER_U, UPPER_U, LOWER_T, UPPER_T, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-5) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (is_suffix(text,/*ences*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-5) )
                    {
                    text.replace(text.end()-3, text.end(), L"t");
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*ables*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) ||
                    delete_if_is_in_r2(text,/*istes*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) ||
                    delete_if_is_in_r2(text,/*ismes*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) ||
                    delete_if_is_in_r2(text,/*ances*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (text.length() >= 5 &&
                    (text[text.length()-5] == LOWER_I || text[text.length()-5] == UPPER_I) &&
                    (text[text.length()-4] == LOWER_Q || text[text.length()-4] == UPPER_Q) &&
                    (text[text.length()-2] == LOWER_E || text[text.length()-2] == UPPER_E) &&
                    (text[text.length()-1] == LOWER_S || text[text.length()-1] == UPPER_S) &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_U_HASH, UPPER_U_HASH) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-5)
                        {
                        text.erase(text.length()-5);
                        update_r_sections(text);
                        }
                    if (length != text.length() )
                        {
                        m_step_1_successful = true;
                        }
                    return;
                    }
            else if (is_suffix(text,/*logie*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I, UPPER_I, LOWER_E, UPPER_E) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-5) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*ement*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, false) )
                {
                if (delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, false) )
                    {
                    delete_if_is_in_r2(text, /*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    }
                else if (is_suffix(text,/*eus*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_S, UPPER_S) )
                    {
                    if (text.length() >= 3 &&
                        stem<string_typeT>::get_r2() <= text.length()-3)
                        {
                        text.erase(text.length()-3);
                        update_r_sections(text);
                        }
                    else if (text.length() >= 3 &&
                        stem<string_typeT>::get_r1() <= text.length()-3)
                        {
                        text[text.length()-1] = LOWER_X;
                        }
                    }
                else if (delete_if_is_in_r2(text,/*abl*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L) )
                    {
                    //NOOP
                    }
                else if (text.length() >= 3 &&
                    (text[text.length()-3] == LOWER_I || text[text.length()-3] == UPPER_I) &&
                    (text[text.length()-2] == LOWER_Q || text[text.length()-2] == UPPER_Q) &&
                    is_either<wchar_t>(text[text.length()-1], LOWER_U_HASH, UPPER_U_HASH) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-3)
                        {
                        text.erase(text.length()-3);
                        update_r_sections(text);
                        }
                    }
                else if (is_suffix_in_rv(text,/*i&#232;r*/LOWER_I, UPPER_I, LOWER_E_GRAVE, UPPER_E_GRAVE, LOWER_R, UPPER_R) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                else if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= (text.length()-3) &&
                    (text[text.length()-2] == LOWER_E_GRAVE || text[text.length()-2] == UPPER_E_GRAVE) &&
                    (text[text.length()-1] == LOWER_R || text[text.length()-1] == UPPER_R) &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_I_HASH, UPPER_I_HASH) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                }
            else if (is_suffix(text,/*ments*/LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_S, UPPER_S) )
                {
                //the proceeding vowel must also be n RV
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= text.length()-6 &&
                    string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);    
                    }
                return;
                }
            else if (is_suffix(text,/*euses*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_r2() <= text.length()-5)
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);
                    }
                else if (stem<string_typeT>::get_r1() <= text.length()-5)
                    {
                    text.replace(text.end()-3, text.end(), L"x");
                    update_r_sections(text);
                    }
                m_step_1_successful = true;
                }
            //4
            else if (is_suffix(text,/*euse*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_S, UPPER_S, LOWER_E, UPPER_E) )
                {
                if (stem<string_typeT>::get_r2() <= text.length()-4)
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    }
                else if (text.length() >= 4 &&
                    stem<string_typeT>::get_r1() <= text.length()-4)
                    {
                    text.replace(text.end()-2, text.end(), L"x");
                    update_r_sections(text);
                    }
                m_step_1_successful = true;
                }
            else if (is_suffix(text,/*ment*/LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                //the proceeding vowel must also be n RV
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= text.length()-5 &&
                    string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);    
                    }
                return;
                }
            else if (is_suffix(text,/*ence*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_E, UPPER_E) )
                {
                if (stem<string_typeT>::get_r2() <= (text.length()-4) )
                    {
                    text.replace(text.end()-2, text.end(), L"t");
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*ance*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_E, UPPER_E, false) ||
                    delete_if_is_in_r2(text,/*isme*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_E, UPPER_E, false) ||
                    delete_if_is_in_r2(text,/*able*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E, false) ||
                    delete_if_is_in_r2(text,/*iste*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (text.length() >= 4 &&
                    (text[text.length()-4] == LOWER_I || text[text.length()-4] == UPPER_I) &&
                    (text[text.length()-3] == LOWER_Q || text[text.length()-3] == UPPER_Q) &&
                    (text[text.length()-1] == LOWER_E || text[text.length()-1] == UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-2], LOWER_U_HASH, UPPER_U_HASH) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-4)
                        {
                        text.erase(text.length()-4);
                        update_r_sections(text);
                        }
                    if (length != text.length() )
                        {
                        m_step_1_successful = true;
                        }
                    return;
                    }
            else if (is_suffix(text,/*eaux*/LOWER_E, UPPER_E, LOWER_A, UPPER_A, LOWER_U, UPPER_U, LOWER_X, UPPER_X) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                m_step_1_successful = true;
                return;
                }
            else if (delete_if_is_in_r2(text,/*it&#233;s*/LOWER_I, UPPER_I, LOWER_T, UPPER_T, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_S, UPPER_S, false) )
                {
                if (is_suffix(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-4)
                        {
                        text.erase(text.length()-4);
                        update_r_sections(text);
                        }
                    else
                        {
                        text.replace(text.end()-2, text.end(), L"l");
                        }
                    }
                else if (is_suffix(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-2)
                        {
                        text.erase(text.length()-2);
                        update_r_sections(text);
                        }
                    else
                        {
                        text.erase(text.length()-2);
                        text += L"iq";
                        text += LOWER_U_HASH;
                        //need to search for r2 again because the 'iq' added here may change that
                        find_r2(text, FRENCH_VOWELS);
                        find_french_rv(text, FRENCH_VOWELS);
                        }
                    }
                else
                    {
                    delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*ives*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T) )
                    {
                    ic_to_iqu(text);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            //3
            else if (delete_if_is_in_r2(text,/*it&#233;*/LOWER_I, UPPER_I, LOWER_T, UPPER_T, LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                if (is_suffix(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-4)
                        {
                        text.erase(text.length()-4);
                        update_r_sections(text);
                        }
                    else
                        {
                        text.replace(text.end()-2, text.end(), L"l");
                        }
                    }
                else if (is_suffix(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) )
                    {
                    if (stem<string_typeT>::get_r2() <= text.length()-2)
                        {
                        text.erase(text.length()-2);
                        update_r_sections(text);
                        }
                    else
                        {
                        text.erase(text.length()-2);
                        text += L"iq";
                        text += LOWER_U_HASH;
                        //need to search for r2 again because the 'iq' added here may change that
                        find_r2(text, FRENCH_VOWELS);
                        find_french_rv(text, FRENCH_VOWELS);
                        }
                    }
                else
                    {
                    delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*eux*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, LOWER_X, UPPER_X, false) )
                {
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (is_suffix(text,/*aux*/LOWER_A, UPPER_A, LOWER_U, UPPER_U, LOWER_X, UPPER_X) )
                {
                if (stem<string_typeT>::get_r1() <= (text.length()-3) )
                    {
                    text.replace(text.end()-2, text.end(), L"l");
                    update_r_sections(text);
                    m_step_1_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r2(text,/*ive*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_E, UPPER_E, false) ||
                    delete_if_is_in_r2(text,/*ifs*/LOWER_I, UPPER_I, LOWER_F, UPPER_F, LOWER_S, UPPER_S, false) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, false) )
                    {
                    ic_to_iqu(text);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            //2
            else if (delete_if_is_in_r2(text,/*if*/LOWER_I, UPPER_I, LOWER_F, UPPER_F, false) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, false) )
                    {
                    ic_to_iqu(text);
                    }
                if (length != text.length() )
                    {
                    m_step_1_successful = true;
                    }
                return;
                }
            }
        /**\htmlonly
        In steps 2a and 2b all tests are confined to the RV region.<br /><br />

        Do step 2a if either no ending was removed by step 1, or if one of endings amment, emment, ment, ments was found.<br /><br />

        Search for the longest among the following suffixes and if found, delete if preceded by a non-vowel:<br />

            -&#238;mes &#238;t &#238;tes i ie ies ir ira irai iraIent irais irait iras irent irez iriez irions irons iront is issaIent
            issais issait issant issante issantes issants isse issent isses issez issiez issions issons it<br /><br />

        (Note that the non-vowel itself must also be in RV.)
        \endhtmlonly*/
        //---------------------------------------------
        void step_2a(string_typeT& text)
            {
            if (text.length() >= 9 &&
                stem<string_typeT>::get_rv() <= (text.length()-8) &&
                (text[text.length()-8] == LOWER_I || text[text.length()-8] == UPPER_I) &&
                (text[text.length()-7] == LOWER_S || text[text.length()-7] == UPPER_S) &&
                (text[text.length()-6] == LOWER_S || text[text.length()-6] == UPPER_S) &&
                (text[text.length()-5] == LOWER_A || text[text.length()-5] == UPPER_A) &&
                (text[text.length()-3] == LOWER_E || text[text.length()-3] == UPPER_E) &&
                (text[text.length()-2] == LOWER_N || text[text.length()-2] == UPPER_N) &&
                (text[text.length()-1] == LOWER_T || text[text.length()-1] == UPPER_T) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_I_HASH, UPPER_I_HASH) )
                {
                if (stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-9], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-8);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issantes*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 9 &&
                    stem<string_typeT>::get_rv() <= (text.length()-9) &&
                    !string_util::is_one_of(text[text.length()-9], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-8);
                    return;
                    }
                }
            else if (text.length() >= 8 &&
                stem<string_typeT>::get_rv() <= (text.length()-7) &&
                (text[text.length()-7] == LOWER_I || text[text.length()-7] == UPPER_I) &&
                (text[text.length()-6] == LOWER_R || text[text.length()-6] == UPPER_R) &&
                (text[text.length()-5] == LOWER_A || text[text.length()-5] == UPPER_A) &&
                (text[text.length()-3] == LOWER_E || text[text.length()-3] == UPPER_E) &&
                (text[text.length()-2] == LOWER_N || text[text.length()-2] == UPPER_N) &&
                (text[text.length()-1] == LOWER_T || text[text.length()-1] == UPPER_T) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_I_HASH, UPPER_I_HASH) )
                {
                if (stem<string_typeT>::get_rv() <= (text.length()-8) &&
                    !string_util::is_one_of(text[text.length()-8], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-7);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issante*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (text.length() >= 8 &&
                    stem<string_typeT>::get_rv() <= (text.length()-8) &&
                    !string_util::is_one_of(text[text.length()-8], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-7);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issants*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 8 &&
                    stem<string_typeT>::get_rv() <= (text.length()-8) &&
                    !string_util::is_one_of(text[text.length()-8], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-7);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issions*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 8 &&
                    stem<string_typeT>::get_rv() <= (text.length()-8) &&
                    !string_util::is_one_of(text[text.length()-8], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-7);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irions*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issais*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issait*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issant*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issent*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issiez*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issons*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 7 &&
                    stem<string_typeT>::get_rv() <= (text.length()-7) &&
                    !string_util::is_one_of(text[text.length()-7], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-6);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irais*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irait*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irent*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*iriez*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irons*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*iront*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*isses*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6) &&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*issez*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z) )
                {
                if (text.length() >= 6 &&
                    stem<string_typeT>::get_rv() <= (text.length()-6)&&
                    !string_util::is_one_of(text[text.length()-6], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-5);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*&#238;mes*/LOWER_I_CIRCUMFLEX, UPPER_I_CIRCUMFLEX, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*&#238;tes*/LOWER_I_CIRCUMFLEX, UPPER_I_CIRCUMFLEX, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irai*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*iras*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*irez*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*isse*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E) )
                {
                if (text.length() >= 5 &&
                    stem<string_typeT>::get_rv() <= (text.length()-5) &&
                    !string_util::is_one_of(text[text.length()-5], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-4);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*ies*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 4 &&
                    stem<string_typeT>::get_rv() <= (text.length()-4) &&
                    !string_util::is_one_of(text[text.length()-4], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-3);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*ira*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A) )
                {
                if (text.length() >= 4 &&
                    stem<string_typeT>::get_rv() <= (text.length()-4) &&
                    !string_util::is_one_of(text[text.length()-4], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-3);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text, LOWER_I_CIRCUMFLEX, UPPER_I_CIRCUMFLEX, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= text.length()-3 &&
                    !string_util::is_one_of(text[text.length()-3], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-2);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*ie*/LOWER_I, UPPER_I, LOWER_E, UPPER_E) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= text.length()-3 &&
                    !string_util::is_one_of(text[text.length()-3], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-2);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*ir*/LOWER_I, UPPER_I, LOWER_R, UPPER_R) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= text.length()-3 &&
                    !string_util::is_one_of(text[text.length()-3], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-2);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*is*/LOWER_I, UPPER_I, LOWER_S, UPPER_S) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= (text.length()-3) &&
                    !string_util::is_one_of(text[text.length()-3], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-2);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text,/*it*/LOWER_I, UPPER_I, LOWER_T, UPPER_T) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_rv() <= (text.length()-3) &&
                    !string_util::is_one_of(text[text.length()-3], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-2);
                    return;
                    }
                }
            else if (is_suffix_in_rv(text, LOWER_I, UPPER_I) )
                {
                if (text.length() >= 2 &&
                    stem<string_typeT>::get_rv() <= (text.length()-2) &&
                    !string_util::is_one_of(text[text.length()-2], FRENCH_VOWELS) )
                    {
                    text.erase(text.length()-1);
                    return;
                    }
                }
            //only called if 2a fails to remove a suffix
            step_2b(text);
            }
        /**\htmlonly
        Do step 2b if step 2a was done, but failed to remove a suffix.<br />
        Search for the longest among the following suffixes, and perform the action indicated.<br /><br /> 

            -ions<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br /><br />

            -&#233; &#233;e &#233;es &#233;s &#232;rent er era erai eraIent erais erait eras erez eriez erions erons eront ez iez<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete<br /><br />

            -&#226;mes &#226;t &#226;tes a ai aIent ais ait ant ante antes ants as asse assent asses assiez assions<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by e, delete<br /><br />

        (Note that the e that may be deleted in this last step must also be in RV.)
        \endhtmlonly*/
        //---------------------------------------------
        void step_2b(string_typeT& text)
            {
            if (delete_if_is_in_rv(text,/*assions*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*assent*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*assiez*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (text.length() >= 7 &&
                stem<string_typeT>::get_rv() <= (text.length()-7) &&
                (text[text.length()-7] == LOWER_E || text[text.length()-7] == UPPER_E) &&
                (text[text.length()-6] == LOWER_R || text[text.length()-6] == UPPER_R) &&
                (text[text.length()-5] == LOWER_A || text[text.length()-5] == UPPER_A) &&
                (text[text.length()-3] == LOWER_E || text[text.length()-3] == UPPER_E) &&
                (text[text.length()-2] == LOWER_N || text[text.length()-2] == UPPER_N) &&
                (text[text.length()-1] == LOWER_T || text[text.length()-1] == UPPER_T) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_I_HASH, UPPER_I_HASH) )
                {
                text.erase(text.length()-7);
                return;
                }
            else if (delete_if_is_in_rv(text,/*erions*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (text.length() >= 5 &&
                stem<string_typeT>::get_rv() <= (text.length()-5) &&
                (text[text.length()-5] == LOWER_A || text[text.length()-5] == UPPER_A) &&
                (text[text.length()-3] == LOWER_E || text[text.length()-3] == UPPER_E) &&
                (text[text.length()-2] == LOWER_N || text[text.length()-2] == UPPER_N) &&
                (text[text.length()-1] == LOWER_T || text[text.length()-1] == UPPER_T) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_I_HASH, UPPER_I_HASH) )
                {
                text.erase(text.length()-5);
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*antes*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*asses*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#232;rent*/LOWER_E_GRAVE, UPPER_E_GRAVE, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*erais*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*erait*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*eriez*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*erons*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*eront*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (is_suffix_in_r1(text,/*ions*/LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) &&
                delete_if_is_in_r2(text,/*ions*/LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_S, UPPER_S) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#226;mes*/LOWER_A_CIRCUMFLEX, UPPER_A_CIRCUMFLEX, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#226;tes*/LOWER_A_CIRCUMFLEX, UPPER_A_CIRCUMFLEX, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*ante*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*ants*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*asse*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*erai*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*eras*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*erez*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ais*/LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*ait*/LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_T, UPPER_T, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*ant*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;es*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*era*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iez*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#226;t*/LOWER_A_CIRCUMFLEX, UPPER_A_CIRCUMFLEX, LOWER_T, UPPER_T, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*ai*/LOWER_A, UPPER_A, LOWER_I, UPPER_I, false) )
                {
                delete_if_is_in_rv(text,LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*as*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;e*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;s*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ez*/LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_A, UPPER_A, false) )
                {
                delete_if_is_in_rv(text, LOWER_E, UPPER_E);
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;*/LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                return;
                }
            }
        /**\htmlonly
        If the last step to be obeyed - either step 1, 2a or 2b - altered the word, do step 3 
        Replace final Y with i or final &#231; with c
        \endhtmlonly*/
        //---------------------------------------------
        void step_3(string_typeT& text)
            {
            if (text.length() == 0)
                { return; }
            if (text[text.length()-1] == LOWER_Y_HASH)
                {
                text[text.length()-1] = LOWER_I;
                }
            else if (text[text.length()-1] == UPPER_Y_HASH)
                {
                text[text.length()-1] = UPPER_I;
                }
            else if (text[text.length()-1] == UPPER_C_CEDILLA)
                {
                text[text.length()-1] = UPPER_C;
                }
            else if (text[text.length()-1] == LOWER_C_CEDILLA)
                {
                text[text.length()-1] = LOWER_C;
                }
            }
        /**\htmlonly
        Alternatively, if the last step to be obeyed did not alter the word, do step 4<br />

        If the word ends s, not preceded by a, i, o, u, &#232; or s, delete it.<br /> 

        In the rest of step 4, all tests are confined to the RV region.<br /><br />

        Search for the longest among the following suffixes, and perform the action indicated:<br />

            -ion<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2 and preceded by s or t<br /><br />

            -ier i&#232;re Ier I&#232;re<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with i<br /><br />

            -e<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete<br /><br />

            -&#235;<br /> 
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by gu, delete<br /><br />

        (So note that ion is removed only when it is in R2 - as well as being in RV -
        and preceded by s or t which must be in RV.)
        \endhtmlonly*/
        //---------------------------------------------
        void step_4(string_typeT& text)
            {
            if (text.length() >= 2 &&
                (text[text.length()-1] == LOWER_S || text[text.length()-1] == UPPER_S) &&
                !string_util::is_one_of(text[text.length()-2], FRENCH_AIOUES) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                }

            if (is_suffix_in_rv(text,/*i&#232;re*/LOWER_I, UPPER_I, LOWER_E_GRAVE, UPPER_E_GRAVE, LOWER_R, UPPER_R, LOWER_E, UPPER_E) )
                {
                text.replace(text.end()-4, text.end(), L"i");
                update_r_sections(text);
                return;
                }
            else if (text.length() >= 4 &&
                stem<string_typeT>::get_rv() <= (text.length()-4) &&
                (text[text.length()-3] == LOWER_E_GRAVE || text[text.length()-3] == UPPER_E_GRAVE) &&
                (text[text.length()-2] == LOWER_R || text[text.length()-2] == UPPER_R) &&
                (text[text.length()-1] == LOWER_E || text[text.length()-1] == UPPER_E) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_I_HASH, UPPER_I_HASH) )
                {
                text.replace(text.end()-4, text.end(), L"i");
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_rv(text,/*ier*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R) )
                {
                text.replace(text.end()-3, text.end(), L"i");
                update_r_sections(text);
                return;
                }
            else if (text.length() >= 3 &&
                stem<string_typeT>::get_rv() <= (text.length()-3) &&
                (text[text.length()-2] == LOWER_E || text[text.length()-2] == UPPER_E) &&
                (text[text.length()-1] == LOWER_R || text[text.length()-1] == UPPER_R) &&
                is_either<wchar_t>(text[text.length()-3], LOWER_I_HASH, UPPER_I_HASH) )
                {
                text.replace(text.end()-3, text.end(), L"i");
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_rv(text,/*sion*/LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N) ||
                is_suffix_in_rv(text,/*tion*/LOWER_T, UPPER_T, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N) )
                {
                if (text.length() >= 3 &&
                    stem<string_typeT>::get_r2() <= text.length()-3)
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix_in_rv(text,/*&#235;*/LOWER_E_UMLAUTS, UPPER_E_UMLAUTS) )
                {
                if (text.length() >= 3 &&
                    (is_either<wchar_t>(text[text.length()-3], LOWER_G, UPPER_G) &&
                    is_either<wchar_t>(text[text.length()-2], LOWER_U, UPPER_U) ) )
                    {
                    text.erase(text.length()-1);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_E, UPPER_E) )
                {
                return;
                }
            }
        /**If the word ends enn, onn, ett, ell or eill, delete the last letter.*/
        //---------------------------------------------
        void step_5(string_typeT& text)
            {
            if (is_suffix(text,/*enn*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_N, UPPER_N) ||
                is_suffix(text,/*onn*/LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_N, UPPER_N) ||
                is_suffix(text,/*ett*/LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_T, UPPER_T) ||
                is_suffix(text,/*ell*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_L, UPPER_L) ||
                is_suffix(text,/*eill*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_L, UPPER_L, LOWER_L, UPPER_L) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                }
            }
        /**@htmlonly
        If the words ends &#233; or &#232; followed by at least one non-vowel, remove the accent from the e.
        @endhtmlonly*/
        //---------------------------------------------
        void step_6(string_typeT& text)
            {
            size_t last_vowel = text.find_last_of(FRENCH_VOWELS);
            if (last_vowel == string_typeT::npos ||
                last_vowel == text.length()-1)
                {
                return;
                }
            else if (is_either<wchar_t>(text[last_vowel], 0xE9, 0xE8) ||
                    is_either<wchar_t>(text[last_vowel], 0xC9, 0xC8) )
                {
                text[last_vowel] = LOWER_E;
                }
            }

        //internal data specific to French stemmer
        bool m_step_1_successful;
        };
    }

#endif //__FRENCH_STEM_H__
