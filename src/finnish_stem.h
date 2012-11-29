/***************************************************************************
                          finnish_stem.h  -  description
                             -------------------
    begin                : Sat May 11 2004
    copyright            : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __FINNISH_STEM_H__
#define __FINNISH_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    Finnish is not an Indo-European language, but belongs to the Finno-Ugric group,
    which again belongs to the Uralic group. Distinctions between a-, i- and d-suffixes
    can be made in Finnish, but they are much less sharply separated than in an
    Indo-European language. The system of endings is extremely elaborate, but strictly
    defined, and applies equally to all nominals, that is, to nouns, adjectives and pronouns.
    Verb endings have a close similarity to nominal endings, which again makes Finnish
    very different from any Indo-European language.<br /><br />

    More problematical than the endings themselves is the change that can be effected in a
    stem as a result of taking a particular ending. A stem typically has two forms
    ,strong and weak, where one class of ending follows the strong form and the complementary
    class the weak. Normalising strong and weak forms after ending removal is not generally possible,
    although the common case where strong and weak forms only differ in the single or double form of
    a final consonant can be dealt with.<br /><br />

    Finnish includes the following accented forms:<br />
        -&#228;   &#246;<br /><br />
    The following letters are vowels:<br />
        -a   e   i   o   u   y   &#228;   &#246;<br /><br />

    R1 and R2 are then defined in the usual way.
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class finnish_stem : public stem<string_typeT>
        {
    public:
        finnish_stem() : m_step_3_successful(false) {}
        //---------------------------------------------
        /**@param text string to stem*/
        void operator()(string_typeT& text)
            {
            if (text.length() < 2)
                {
                return;
                }

            //reset internal data
            m_step_3_successful = false;
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);

            find_r1(text, FINNISH_VOWELS);
            find_r2(text, FINNISH_VOWELS);

            step_1(text);
            step_2(text);
            step_3(text);
            step_4(text);
            step_5(text);
            step_6(text);
            }
    private:
        /**\htmlonly
        Step 1: particles etc.<br /> 
        Search for the longest among the following suffixes in R1, and perform the action indicated:<br /><br />

        - kin   kaan   k&#228;&#228;n   ko   k&#246;   han   h&#228;n   pa   p&#228;<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if preceded by n, t or a vowel<br /><br />
        - sti<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if in R2<br /><br />
            (Of course, the n, t or vowel of 1(a) need not be in R1:
            only the suffix removed must be in R1. And similarly below
        \endhtmlonly*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            if (is_suffix_in_r1(text,/*kaan*/LOWER_K, UPPER_K, LOWER_A, UPPER_A, LOWER_A, UPPER_A, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*k&#228;&#228;n*/LOWER_K, UPPER_K, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, LOWER_N, UPPER_N) )
                {
                if (text.length() >= 5 &&
                    string_util::is_one_of(text[text.length()-5], FINNISH_STEP_1_SUFFIX) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix_in_r1(text,/*kin*/LOWER_K, UPPER_K, LOWER_I, UPPER_I, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*han*/LOWER_H, UPPER_H, LOWER_A, UPPER_A, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*h&#228;n*/LOWER_H, UPPER_H, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, LOWER_N, UPPER_N) )
                {
                if (text.length() >= 4 &&
                    string_util::is_one_of(text[text.length()-4], FINNISH_STEP_1_SUFFIX) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix_in_r1(text,/*sti*/LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_I, UPPER_I) )
                {
                delete_if_is_in_r2(text,/*sti*/LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_I, UPPER_I);
                return;
                }
            else if (is_suffix_in_r1(text,/*ko*/LOWER_K, UPPER_K, LOWER_O, UPPER_O) ||
                is_suffix_in_r1(text,/*k&#246;*/LOWER_K, UPPER_K, LOWER_O_UMLAUTS, UPPER_O_UMLAUTS) ||
                is_suffix_in_r1(text,/*pa*/LOWER_P, UPPER_P, LOWER_A, UPPER_A) ||
                is_suffix_in_r1(text,/*p&#228;*/LOWER_P, UPPER_P, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) )
                {
                if (text.length() >= 3 &&
                    string_util::is_one_of(text[text.length()-3], FINNISH_STEP_1_SUFFIX) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            }
        /**\htmlonly
        Step 2: possessives<br />
        Search for the longest among the following suffixes in R1, and perform the action indicated:<br />

            - si<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if not preceded by k<br /><br />
            - ni<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete<br /><br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- if preceded by kse, replace with ksi<br /><br />
            - nsa   ns&#228;   mme   nne<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete<br /><br />
            - an<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if preceded by one of   ta   ssa   sta   lla   lta   na<br /><br />
            - &#228;n<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if preceded by one of   t&#228;   ss&#228;   st&#228;   ll&#228;   lt&#228;   n&#228;<br /><br />
            - en<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if preceded by one of   lle   ine<br />
        \endhtmlonly*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*nsa*/LOWER_N, UPPER_N, LOWER_S, UPPER_S, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*ns&#228;*/LOWER_N, UPPER_N, LOWER_S, UPPER_S, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r1(text,/*mme*/LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_E, UPPER_E, false) ||
                delete_if_is_in_r1(text,/*nne*/LOWER_N, UPPER_N, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (is_suffix_in_r1(text,/*si*/LOWER_S, UPPER_S, LOWER_I, UPPER_I) )
                {
                if (text.length() >= 3 &&
                    !(text[text.length()-3] == LOWER_K || text[text.length()-3] == UPPER_K))
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_r1(text,/*ni*/LOWER_N, UPPER_N, LOWER_I, UPPER_I, false) )
                {
                if (is_suffix(text, /*kse*/LOWER_K, UPPER_K, LOWER_S, UPPER_S, LOWER_E, UPPER_E) )
                    {
                    text[text.length()-1] = LOWER_I;
                    }
                return;
                }
            else if (is_suffix_in_r1(text,/*an*/LOWER_A, UPPER_A, LOWER_N, UPPER_N) )
                {
                if ((text.length() >= 4 &&
                    (is_partial_suffix(text, (text.length()-4), LOWER_T, UPPER_T, LOWER_A, UPPER_A) ||
                    is_partial_suffix(text, (text.length()-4), LOWER_N, UPPER_N, LOWER_A, UPPER_A)) ) ||
                    (text.length() >= 5 &&
                    (is_partial_suffix(text, (text.length()-5), LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_A, UPPER_A) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_L, UPPER_L, LOWER_T, UPPER_T, LOWER_A, UPPER_A) ) ) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix_in_r1(text,/*&#228;n*/LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, LOWER_N, UPPER_N) )
                {
                if ((text.length() >= 4 &&
                    (is_partial_suffix(text, (text.length()-4), LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ||
                    is_partial_suffix(text, (text.length()-4), LOWER_N, UPPER_N, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS)) ) ||
                    (text.length() >= 5 &&
                    (is_partial_suffix(text, (text.length()-5), LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_L, UPPER_L, LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ) ) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            else if (is_suffix_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N) )
                {
                if (text.length() >= 5 &&
                    (is_partial_suffix(text, (text.length()-5), LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_E, UPPER_E) ||
                    is_partial_suffix(text, (text.length()-5), LOWER_I, UPPER_I, LOWER_N, UPPER_N, LOWER_E, UPPER_E) ) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    }
                return;
                }
            }
        /**\htmlonly
        //////////////////////////////////////////////////////////////////////////////////////////////<br />
        Define a v (vowel) as one of   a   e   i   o   u   y   &#228;   &#246;.<br />                  
        Define a V (restricted vowel) as one of   a   e   i   o   u   &#228;   &#246;.<br />           
        So Vi means a V followed by letter i.<br />                                                    
        Define LV (long vowel) as one of   aa   ee   ii   oo   uu   &#228;&#228;   &#246;&#246;.<br />
        Define a c (consonant) as a character other than a v.<br />                                    
        So cv means a c followed by a v.<br />                                                        
        //////////////////////////////////////////////////////////////////////////////////////////////<br /><br />

        Step 3: cases<br />
        Search for the longest among the following suffixes in R1, and perform the action indicated:<br /><br />

            - hXn   preceded by X, where X is a V other than u (a/han, e/hen etc) <br />
            - siin   den   tten   preceded by Vi<br />
            - seen   preceded by LV<br />
            - a   &#228;   preceded by cv<br />
            - tta   tt&#228;   preceded by e<br />
            - ta   t&#228;   ssa   ss&#228;   sta   st&#228;   lla   ll&#228;   lta   lt&#228;   lle   na   n&#228;   ksi   ine<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete<br /><br />
            - n<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete, and if preceded by LV or ie, delete the last vowel
        \endhtmlonly*/
        //---------------------------------------------
        void step_3(string_typeT& text)
            {
            //seen followed by LV
            if (text.length() >= 6 &&
                is_suffix_in_r1(text,/*seen*/LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_E, UPPER_E, LOWER_N, UPPER_N) &&
                string_util::is_one_of(text[text.length()-5], FINNISH_VOWELS_NO_Y) &&
                string_util::tolower_western(text[text.length()-5]) == string_util::tolower_western(text[text.length()-6]) )
                {
                text.erase(text.length()-4);
                update_r_sections(text);
                m_step_3_successful = true;
                return;
                }
            //suffix followed by Vi
            else if (text.length() >= 6 &&
                    is_either<wchar_t>(text[text.length()-5], LOWER_I, UPPER_I) &&
                    (is_suffix_in_r1(text,/*siin*/LOWER_S, UPPER_S, LOWER_I, UPPER_I, LOWER_I, UPPER_I, LOWER_N, UPPER_N) ||
                    is_suffix_in_r1(text,/*tten*/LOWER_T, UPPER_T, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_N, UPPER_N) ) &&
                    string_util::is_one_of(text[text.length()-6], FINNISH_VOWELS_NO_Y) )
                {
                text.erase(text.length()-4);
                update_r_sections(text);
                m_step_3_successful = true;
                return;
                }
            //suffix followed by Vi
            else if (text.length() >= 5 &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_I, UPPER_I) &&
                    is_suffix_in_r1(text,/*den*/LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_N, UPPER_N) &&
                    string_util::is_one_of(text[text.length()-5], FINNISH_VOWELS_NO_Y) )
                {
                text.erase(text.length()-3);
                update_r_sections(text);
                m_step_3_successful = true;
                return;
                }
            else if ((is_suffix_in_r1(text,/*tta*/LOWER_T, UPPER_T, LOWER_T, UPPER_T, LOWER_A, UPPER_A) ||
                is_suffix_in_r1(text,/*tt&#228;*/LOWER_T, UPPER_T, LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS)) &&
                is_either<wchar_t>(text[text.length()-4], LOWER_E, UPPER_E) )
                {
                text.erase(text.length()-3);
                update_r_sections(text);
                m_step_3_successful = true;
                return;
                }
            //ends if VHVN
            else if (
                (is_suffix_in_r1(text,/*han*/LOWER_H, UPPER_H, LOWER_A, UPPER_A, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*hen*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*hin*/LOWER_H, UPPER_H, LOWER_I, UPPER_I, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*hon*/LOWER_H, UPPER_H, LOWER_O, UPPER_O, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*h&#228;n*/LOWER_H, UPPER_H, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, LOWER_N, UPPER_N) ||
                is_suffix_in_r1(text,/*h&#246;n*/LOWER_H, UPPER_H, LOWER_O_UMLAUTS, UPPER_O_UMLAUTS, LOWER_N, UPPER_N) ) )
                {
                if (string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-4]) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    m_step_3_successful = true;
                    }
                return;
                }
            else if (delete_if_is_in_r1(text,/*ssa*/LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*ss&#228;*/LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r1(text,/*sta*/LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*st&#228;*/LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r1(text,/*lla*/LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*ll&#228;*/LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r1(text,/*lta*/LOWER_L, UPPER_L, LOWER_T, UPPER_T, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*lt&#228;*/LOWER_L, UPPER_L, LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r1(text,/*lle*/LOWER_L, UPPER_L, LOWER_L, UPPER_L, LOWER_E, UPPER_E, false) ||
                delete_if_is_in_r1(text,/*ksi*/LOWER_K, UPPER_K, LOWER_S, UPPER_S, LOWER_I, UPPER_I, false) ||
                delete_if_is_in_r1(text,/*ine*/LOWER_I, UPPER_I, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) ||
                delete_if_is_in_r1(text,/*na*/LOWER_N, UPPER_N, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*n&#228;*/LOWER_N, UPPER_N, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) )
                {
                m_step_3_successful = true;
                return;
                }
            else if (delete_if_is_in_r1(text,/*ta*/LOWER_T, UPPER_T, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r1(text,/*t&#228;*/LOWER_T, UPPER_T, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) )
                {
                m_step_3_successful = true;
                return;
                }
            //suffix followed by cv
            else if (text.length() >= 3 &&
                    (is_suffix_in_r1(text, LOWER_A, UPPER_A) || is_suffix_in_r1(text, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) )&&
                    !string_util::is_one_of(text[text.length()-3], FINNISH_VOWELS) &&
                    string_util::is_one_of(text[text.length()-2], FINNISH_VOWELS) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                m_step_3_successful = true;
                return;
                }
            //suffix followed by LV or ie
            else if (is_suffix_in_r1(text, LOWER_N, UPPER_N) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                if (text.length() >= 2 &&
                    (string_util::is_one_of(text[text.length()-1], FINNISH_VOWELS_NO_Y) &&
                    string_util::tolower_western(text[text.length()-1]) == string_util::tolower_western(text[text.length()-2])) ||
                    is_suffix_in_r1(text,/*ie*/LOWER_I, UPPER_I, LOWER_E, UPPER_E) ) 
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                m_step_3_successful = true;
                return;
                }
            }
        /**\htmlonly
        Step 4: other endings<br />
        Search for the longest among the following suffixes in R2, and perform the action indicated<br /><br />

            - mpi   mpa   mp&#228;   mmi   mma   mm&#228;<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete if not preceded by po<br />
            - impi   impa   imp&#228;   immi   imma   imm&#228;   eja   ej&#228;<br /> 
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- delete
        \endhtmlonly*/
        //---------------------------------------------
        void step_4(string_typeT& text)
            {
            if (delete_if_is_in_r2(text,/*impi*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_I, UPPER_I, false) ||
                delete_if_is_in_r2(text,/*impa*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r2(text,/*imp&#228;*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r2(text,/*immi*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_I, UPPER_I, false) ||
                delete_if_is_in_r2(text,/*imma*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r2(text,/*imm&#228;*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) ||
                delete_if_is_in_r2(text,/*eja*/LOWER_E, UPPER_E, LOWER_J, UPPER_J, LOWER_A, UPPER_A, false) ||
                delete_if_is_in_r2(text,/*ej&#228;*/LOWER_E, UPPER_E, LOWER_J, UPPER_J, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS, false) )
                {
                return;
                }
            else if (text.length() >= 5 &&
                    (is_suffix_in_r2(text,/*mpi*/LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_I, UPPER_I) ||
                    is_suffix_in_r2(text,/*mpa*/LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_A, UPPER_A) ||
                    is_suffix_in_r2(text,/*mp&#228;*/LOWER_M, UPPER_M, LOWER_P, UPPER_P, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ||
                    is_suffix_in_r2(text,/*mmi*/LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_I, UPPER_I) ||
                    is_suffix_in_r2(text,/*mma*/LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A, UPPER_A) ||
                    is_suffix_in_r2(text,/*mm&#228;*/LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A_UMLAUTS, UPPER_A_UMLAUTS) ) )
                {
                if (!(is_either<wchar_t>(text[text.length()-5], LOWER_P, UPPER_P) &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_O, UPPER_O) ) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    }
                }
            return;
            }
        /**\htmlonly
        Step 5: plurals 
        If an ending was removed in step 3, delete a final i or j if in R1;<br />
        otherwise, if an ending was not removed in step 3,<br />
        then delete the final t in R1 if it follows a vowel, and, if a t is removed,<br />
        then delete a final mma or imma in R2, unless the mma is preceded by po.
        \endhtmlonly*/
        //---------------------------------------------
        void step_5(string_typeT& text)
            {
            //if step 3 was successful in removing a suffix
            if (m_step_3_successful)
                {
                if (delete_if_is_in_r1(text, LOWER_I, UPPER_I) ||
                    delete_if_is_in_r1(text, LOWER_J, UPPER_J) )
                    {
                    //NOOP
                    }
                }
            else
                {
                if (text.length() >= 2 &&
                    string_util::is_one_of(text[text.length()-2], FINNISH_VOWELS) )
                    {
                    if (delete_if_is_in_r1(text, LOWER_T, UPPER_T) )
                        {
                        if (!delete_if_is_in_r2(text,/*imma*/LOWER_I, UPPER_I,LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A, UPPER_A) )
                            {
                            if (text.length() >= 5 &&
                                is_suffix_in_r2(text,/*mma*/LOWER_M, UPPER_M, LOWER_M, UPPER_M, LOWER_A, UPPER_A) &&
                                //isn't proceeded by "po"
                                !(is_either<wchar_t>(text[text.length()-5], LOWER_P, UPPER_P) &&
                                is_either<wchar_t>(text[text.length()-4], LOWER_O, UPPER_O) ) )
                                {
                                text.erase(text.length()-3);
                                update_r_sections(text);
                                }
                            }
                        }
                    }
                }
            }
        /**Step 6: tidying up 
        Perform turn steps (a), (b), (c), (d), restricting all tests to the R1 region.*/
        //---------------------------------------------
        void step_6(string_typeT& text)
            {
            step_6a(text);
            step_6b(text);
            step_6c(text);
            step_6d(text);
            step_6e(text);
            }
        /**Step a) If R1 ends with LV then delete the last letter.*/
        //---------------------------------------------
        void step_6a(string_typeT& text)
            {
            if (text.length() >= 2 &&
                stem<string_typeT>::get_r1() <= text.length()-2 &&
                string_util::is_one_of(text[text.length()-1], FINNISH_VOWELS_NO_Y) &&
                string_util::tolower_western(text[text.length()-1]) == string_util::tolower_western(text[text.length()-2]))
                {
                text.erase(text.end()-1);
                update_r_sections(text);
                }
            }
        /**\htmlonly
        Step b) If R1 ends with cX, c a consonant and X one of:<br />
        a   &#228;   e   i<br /><br />
        then delete the last letter.
        \endhtmlonly*/
        //---------------------------------------------
        void step_6b(string_typeT& text)
            {
            if (text.length() >= 2 &&
                stem<string_typeT>::get_r1() <= text.length()-2 &&
                !string_util::is_one_of(text[text.length()-2], FINNISH_VOWELS) &&
                string_util::is_one_of(text[text.length()-1], FINNISH_VOWELS_SIMPLE) )
                {
                text.erase(text.end()-1);
                update_r_sections(text);
                }
            }
        /**Step c) If R1 ends with oj or uj then delete the last letter.*/
        //---------------------------------------------
        void step_6c(string_typeT& text)
            {
            if (is_suffix_in_r1(text,/*oj*/LOWER_O, UPPER_O, LOWER_J, UPPER_J) ||
                is_suffix_in_r1(text,/*uj*/LOWER_U, UPPER_U, LOWER_J, UPPER_J) )
                {
                text.erase(text.end()-1);
                update_r_sections(text);
                }
            }
        /**Step d) If R1 ends with jo then delete the last letter.*/
        //---------------------------------------------
        void step_6d(string_typeT& text)
            {
            if (is_suffix_in_r1(text,/*jo*/LOWER_J, UPPER_J, LOWER_O, UPPER_O) )
                {
                text.erase(text.end()-1);
                update_r_sections(text);
                }
            }
        /**\htmlonly
        Do step (e), which is not restricted to R1.<br />
        Step e) If the word ends with a double consonant followed by zero or more vowels,<br />
        then remove the last consonant (so el&#228;kk -&gt; el&#228;k, aatonaatto -> aatonaato)
        \endhtmlonly*/
        //---------------------------------------------
        void step_6e(string_typeT& text)
            {
            //find the last consonant
            size_t index = text.find_last_not_of(FINNISH_VOWELS);
            if (index == string_typeT::npos ||
                index < 1)
                {
                return;
                }
            if (string_util::tolower_western(text[index]) == string_util::tolower_western(text[index-1]))
                {
                text.erase(text.begin()+(index) );
                update_r_sections(text);
                }
            }
        //internal data specific to Finnish stemmer
        bool m_step_3_successful;
        };
    }

#endif //__FINNISH_STEM_H__
