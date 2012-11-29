/***************************************************************************
                          portuguese_stem.h  -  description
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

#ifndef __PORTUGUESE_STEM_H__
#define __PORTUGUESE_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    Letters in Portuguese include the following accented forms,<br />

    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#225; &#233; &#237; &#243; &#250; &#226; &#234; &#244; &#231; &#227; &#245; &#252; &#241;<br /><br />

    The following letters are vowels:<br />

    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-a e i o u &#225; &#233; &#237; &#243; &#250; &#226; &#234; &#244;<br /><br />

    And the two nasalised vowel forms<br />

    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#227; &#245;<br /><br />

    should be treated as a vowel followed by a consonant. 

    &#227; and &#245; are therefore replaced by a~ and o~ in the word, where ~ is a separate character to be treated as a consonant.
    And then R2 and RV have the same definition as in the Spanish stemmer.
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class portuguese_stem : public stem<string_typeT>
        {
    public:
        portuguese_stem() : m_step1_step2_altered(false), m_altered_suffix_index(0)
            {}
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
            m_altered_suffix_index = 0;
            m_step1_step2_altered = false;
            stem<string_typeT>::reset_r_values();

            string_util::replace_all<string_typeT>(text, string_typeT(1, LOWER_A_TILDE), L"a~");
            string_util::replace_all<string_typeT>(text, string_typeT(1, UPPER_A_TILDE), L"A~");
            string_util::replace_all<string_typeT>(text, string_typeT(1, LOWER_O_TILDE), L"o~");
            string_util::replace_all<string_typeT>(text, string_typeT(1, UPPER_O_TILDE), L"O~");

            find_r1(text, PORTUGUESE_VOWELS);
            find_r2(text, PORTUGUESE_VOWELS);
            find_spanish_rv(text, PORTUGUESE_VOWELS);

            step_1(text);
            //intermediate steps handled by step 1
            if (!m_step1_step2_altered)
                {
                step_4(text);
                }
            step_5(text);

            //Turn a~, o~ back into &#227;, &#245;
            string_util::replace_all<string_typeT>(text, L"a~", string_typeT(1, LOWER_A_TILDE));
            string_util::replace_all<string_typeT>(text, L"A~", string_typeT(1, UPPER_A_TILDE));
            string_util::replace_all<string_typeT>(text, L"o~", string_typeT(1, LOWER_O_TILDE));
            string_util::replace_all<string_typeT>(text, L"O~", string_typeT(1, UPPER_O_TILDE));
            }
    private:
        /**\htmlonly
        Search for the longest among the following suffixes, and perform the action indicated.<br />

            -eza   ezas   ico   ica   icos   icas   ismo   ismos   &#225;vel   &#237;vel   ista   istas
             oso   osa   osos   osas   amento   amentos   imento   imentos   adora   ador   a&#231;a~o
             adoras   adores   a&#231;o~es   ante   antes   &#226;ncia<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br /><br />

            -log&#237;a log&#237;as<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with log if in R2<br /><br />

            -uci&#243;n uciones<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with u if in R2<br /><br />

            -&#234;ncia &#234;ncias<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ente if in R2<br /><br />

            -amente<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R1<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by iv, delete if in R2 (and if further preceded by at, delete if in R2), otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by os, ic or ad, delete if in R2<br /><br />

            -mente<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ante, avel or &#237;vel, delete if in R2<br /><br />

            -idade idades<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by abil, ic or iv, delete if in R2<br /><br />

            -iva ivo ivas ivos<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by at, delete if in R2<br /><br />

            -ira iras<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ir if in RV and preceded by e<br />
        \endhtmlonly*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            size_t original_length = text.length();
            if (is_suffix_in_r2(text,/*uciones*/LOWER_U, UPPER_U, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                text.erase(text.length()-6);
                m_altered_suffix_index = text.length()-1;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*amentos*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*imentos*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*amento*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*imento*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*adoras*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*adores*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*a&#231;o~es*/LOWER_A, UPPER_A, LOWER_C_CEDILLA, UPPER_C_CEDILLA, LOWER_O, UPPER_O, TILDE, TILDE, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (is_suffix_in_r2(text,/*&#234;ncias*/LOWER_E_CIRCUMFLEX, UPPER_E_CIRCUMFLEX, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                text.replace(text.end()-6, text.end(), L"ente");
                m_altered_suffix_index = text.length()-4;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }
            else if (is_suffix_in_r2(text,/*log&#237;as*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                text.erase(text.length()-3);
                m_altered_suffix_index = text.length()-3;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r1(text,/*amente*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, false) )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T, false);
                    }
                else
                    {
                    if (delete_if_is_in_r2(text,/*os*/LOWER_O, UPPER_O, LOWER_S, UPPER_S) ||
                        delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                        delete_if_is_in_r2(text,/*ad*/LOWER_A, UPPER_A, LOWER_D, UPPER_D) )
                        {
                        //NOOP (fall through to branching statement)
                        }
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*idades*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (delete_if_is_in_r2(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) ||
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                    delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }    
            else if (is_suffix_in_r2(text,/*log&#237;a*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A) )
                {
                text.erase(text.length()-2);
                m_altered_suffix_index = text.length()-3;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }    
            else if (is_suffix_in_r2(text,/*uci&#243;n*/LOWER_U, UPPER_U, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O_ACUTE, UPPER_O_ACUTE, LOWER_N, UPPER_N) )
                {
                text.erase(text.length()-4);
                m_altered_suffix_index = text.length()-1;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }
            else if (is_suffix_in_r2(text,/*&#234;ncia*/LOWER_E_CIRCUMFLEX, UPPER_E_CIRCUMFLEX, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A) )
                {
                text.replace(text.end()-5, text.end(), L"ente");
                m_altered_suffix_index = text.length()-4;
                update_r_sections(text);
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*mente*/LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (delete_if_is_in_r2(text,/*ante*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) ||
                    delete_if_is_in_r2(text,/*avel*/LOWER_A, UPPER_A, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_L, UPPER_L) ||
                    delete_if_is_in_r2(text,/*&#237;vel*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_L, UPPER_L) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*idade*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_E, UPPER_E) )
                {
                if (delete_if_is_in_r2(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) ||
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                    delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (is_suffix(text,/*eiras*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (stem<string_typeT>::get_rv() <= text.length()-4)
                    {
                    text.erase(text.length()-2);
                    m_altered_suffix_index = text.length()-3;
                    update_r_sections(text);
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*antes*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*&#226;ncia*/LOWER_A_CIRCUMFLEX, UPPER_A_CIRCUMFLEX, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ismos*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*istas*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*adora*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*a&#231;a~o*/LOWER_A, UPPER_A, LOWER_C_CEDILLA, UPPER_C_CEDILLA, LOWER_A, UPPER_A, TILDE, TILDE, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ante*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ezas*/LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*icos*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*icas*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ismo*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*&#225;vel*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_L, UPPER_L) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*&#237;vel*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_L, UPPER_L) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ista*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*osos*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*osas*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ador*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ivas*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ivos*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (is_suffix(text,/*eira*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A) )
                {
                if (stem<string_typeT>::get_rv() <= text.length()-3)
                    {
                    text.erase(text.length()-1);
                    m_altered_suffix_index = text.length()-3;
                    update_r_sections(text);
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*iva*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_A, UPPER_A) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ivo*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_O, UPPER_O) )
                {
                if (delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T) )
                    {
                    //NOOP (fall through to branching statement)
                    }
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*eza*/LOWER_E, UPPER_E, LOWER_Z, UPPER_Z, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ico*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*ica*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*oso*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_O, UPPER_O) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_r2(text,/*osa*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_A, UPPER_A) )
                {
                //NOOP (fall through to branching statement)
                }
            //branch to the next appropriate step
            if (original_length == text.length() )
                {
                step_2(text);
                }
            else
                {
                m_step1_step2_altered = true;
                step_3(text);
                }
             }
        /**\htmlonly
        Do step 2 if no ending was removed by step 1.<br /><br />
        Search for the longest among the following suffixes in RV, and if found, delete.<br /><br /> 

            -ada ida ia aria eria iria ar&#225; ara er&#225; era ir&#225; ava asse esse isse aste este iste
            ei arei erei irei am iam ariam eriam iriam aram eram iram avam em arem erem irem
            assem essem issem ado ido ando endo indo ara~o era~o ira~o ar er ir as adas idas
            ias arias erias irias ar&#225;s aras er&#225;s eras ir&#225;s avas es ardes erdes irdes ares eres
            ires asses esses isses astes estes istes is ais eis &#237;eis ar&#237;eis er&#237;eis ir&#237;eis &#225;reis
            areis &#233;reis ereis &#237;reis ireis &#225;sseis &#233;sseis &#237;sseis &#225;veis ados idos &#225;mos amos &#237;amos
            ar&#237;amos er&#237;amos ir&#237;amos &#225;ramos &#233;ramos &#237;ramos &#225;vamos emos aremos eremos iremos &#225;ssemos
            &#234;ssemos &#237;ssemos imos armos ermos irmos eu iu ou ira iras<br /><br />

        If the last step to be obeyed - either step 1 or 2 - altered the word, do step 3
        \endhtmlonly*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            size_t original_length = text.length();

            if (delete_if_is_in_rv(text,/*ar&#237;amos*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*er&#237;amos*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;amos*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;ssemos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#234;ssemos*/LOWER_E_CIRCUMFLEX, UPPER_E_CIRCUMFLEX, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;ssemos*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ar&#237;eis*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*er&#237;eis*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;eis*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;sseis*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#233;sseis*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;sseis*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;ramos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#233;ramos*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;ramos*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;vamos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_V, UPPER_V, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*aremos*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eremos*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iremos*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S ,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ariam*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eriam*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iriam*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*assem*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*essem*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*issem*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ara~o*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, TILDE, TILDE, LOWER_O, UPPER_O,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*era~o*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, TILDE, TILDE, LOWER_O, UPPER_O,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ira~o*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, TILDE, TILDE, LOWER_O, UPPER_O,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*arias*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*erias*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*irias*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ardes*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*erdes*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*irdes*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*asses*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*esses*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*isses*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*astes*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*estes*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*istes*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;reis*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*areis*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#233;reis*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ereis*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;reis*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ireis*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;veis*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_V, UPPER_V, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;amos*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*armos*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ermos*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*irmos*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S,false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*aria*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eria*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iria*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I, UPPER_I, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*asse*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*esse*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*isse*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*aste*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*este*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iste*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*arei*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*erei*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*irei*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_I, UPPER_I, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*aram*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eram*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iram*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*avam*/LOWER_A, UPPER_A, LOWER_V, UPPER_V, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*arem*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*erem*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*irem*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ando*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*endo*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*indo*/LOWER_I, UPPER_I, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*adas*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*idas*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ar&#225;s*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*aras*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*er&#225;s*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eras*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ir&#225;s*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*avas*/LOWER_A, UPPER_A, LOWER_V, UPPER_V, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ares*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eres*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ires*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#237;eis*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ados*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*idos*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*&#225;mos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*amos*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*emos*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*imos*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iras*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ada*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ida*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ar&#225;*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ara*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*er&#225;*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*era*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ir&#225;*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ava*/LOWER_A, UPPER_A, LOWER_V, UPPER_V, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iam*/LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ado*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ido*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ias*/LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ais*/LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eis*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ira*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ia*/LOWER_I, UPPER_I, LOWER_A, UPPER_A, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ei*/LOWER_E, UPPER_E, LOWER_I, UPPER_I, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*am*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*em*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ar*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ir*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*as*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*es*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*is*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*eu*/LOWER_E, UPPER_E, LOWER_U, UPPER_U, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*iu*/LOWER_I, UPPER_I, LOWER_U, UPPER_U, false) )
                {
                //NOOP (fall through to branching statement)
                }
            else if (delete_if_is_in_rv(text,/*ou*/LOWER_O, UPPER_O, LOWER_U, UPPER_U, false) )
                {
                //NOOP (fall through to branching statement)
                }

            if (original_length != text.length() )
                {
                m_step1_step2_altered = true;
                step_3(text);
                }
            }
        /**Delete suffix i if in RV and preceded by c*/
        //---------------------------------------------
        void step_3(string_typeT& text) 
            {
            if (is_suffix(text,/*ci*/LOWER_C, UPPER_C, LOWER_I, UPPER_I) &&
                stem<string_typeT>::get_rv() <= text.length()-1)
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                }
            }
        /**\htmlonly
        Alternatively, if neither steps 1 nor 2 altered the word, do step 4.<br /><br />
        If the word ends with one of the suffixes<br /> 

            -os a i o &#225; &#237; &#243;<br /><br /> 

        in RV, delete it
        \endhtmlonly*/
        //---------------------------------------------
        void step_4(string_typeT& text) 
            {
            if (delete_if_is_in_rv(text,/*os*/LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_A, UPPER_A) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_I, UPPER_I) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_O, UPPER_O) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_A_ACUTE, UPPER_A_ACUTE) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_I_ACUTE, UPPER_I_ACUTE) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text, LOWER_O_ACUTE, UPPER_O_ACUTE) )
                {
                return;
                }
            }
        /**\htmlonly
        Always do step 5.<br /><br />
        If the word ends with one of<br />

            -e &#233; &#234;<br /><br /> 

        in RV, delete it, and if preceded by gu (or ci) with the u (or i) in RV, delete the u (or i). 
        Or if the word ends &#231; remove the cedilla
        \endhtmlonly*/
        //---------------------------------------------
        void step_5(string_typeT& text) 
            {
            if (delete_if_is_in_rv(text, LOWER_E, UPPER_E, false) )
                {
                if (text.length() >= 1 &&
                    stem<string_typeT>::get_rv() <= text.length()-1 &&
                    (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) ||
                    is_suffix(text,/*ci*/LOWER_C, UPPER_C, LOWER_I, UPPER_I)) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;*/LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                if (text.length() >= 1 &&
                    stem<string_typeT>::get_rv() <= text.length()-1 &&
                    (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) ||
                    is_suffix(text,/*ci*/LOWER_C, UPPER_C, LOWER_I, UPPER_I)) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#234;*/LOWER_E_CIRCUMFLEX, UPPER_E_CIRCUMFLEX, false) )
                {
                if (text.length() >= 1 &&
                    stem<string_typeT>::get_rv() <= text.length()-1 &&
                    (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) ||
                    is_suffix(text,/*ci*/LOWER_C, UPPER_C, LOWER_I, UPPER_I)) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (text.length() >= 1 &&
                text[text.length()-1] == UPPER_C_CEDILLA)
                {
                text[text.length()-1] = UPPER_C;
                }
            else if (text.length() >= 1 &&
                text[text.length()-1] == LOWER_C_CEDILLA)
                {
                text[text.length()-1] = LOWER_C;
                }
            }
        //internal data specific to Portuguese stemmer
        bool m_step1_step2_altered;
        size_t m_altered_suffix_index;
        };
    }

#endif //__PORTUGUESE_STEM_H__
