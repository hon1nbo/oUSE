/***************************************************************************
                          spanish_stem.h  -  description
                             -------------------
    begin                : Sat May 19 2004
    copyright            : (C) 2004 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the BSD License.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __SPANISH_STEM_H__
#define __SPANISH_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    Letters in Spanish include the following accented forms,<br /> 
        -&#225; &#233; &#237; &#243; &#250; &#252; &#241;<br /><br /> 
    
    The following letters are vowels:<br /> 
        -a e i o u &#225; &#233; &#237; &#243; &#250; &#252;<br /><br />
    
    R2 is defined in the usual way - see the note on R1 and R2.<br /><br />

    RV is defined as follows (and this is not the same as the French stemmer definition):<br /> 

    If the second letter is a consonant, RV is the region after the next following vowel,
    or if the first two letters are vowels, RV is the region after the next consonant,
    and otherwise (consonant-vowel case) RV is the region after the third letter.
    But RV is the end of the word if these positions cannot be found.<br /><br /> 

    For example,<br /> 

        m a c h o     o l i v a     t r a b a j o     &#225; u r e o<br />
        &nbsp;&nbsp;&nbsp;&nbsp;|...|&nbsp;&nbsp;&nbsp;&nbsp;|...|&nbsp;&nbsp;&nbsp;&nbsp;|.......|&nbsp;&nbsp;&nbsp;&nbsp;|...|
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class spanish_stem : public stem<string_typeT>
        {
    public:
        //---------------------------------------------
        /**@param text string to stem*/
        void operator()(string_typeT& text)
            {
            if (text.length() < 3)
                {
                remove_spanish_acutes(text);
                return;
                }

            //reset internal data
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);

            find_r1(text, SPANISH_VOWELS);
            find_r2(text, SPANISH_VOWELS);
            find_spanish_rv(text, SPANISH_VOWELS);

            step_0(text);
            step_1(text);
            ///steps 2a and 2b and only called from step1
            step_3(text);

            remove_spanish_acutes(text);
            }
    private:
        ///Always do steps 0 and 1.
        /**\htmlonly
        Search for the longest among the following suffixes<br /><br />

            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-me se sela selo selas selos la le lo las les los nos<br /><br />

            and delete it, if comes after one of<br /><br /> 

            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-i&#233;ndo &#225;ndo &#225;r &#233;r &#237;r<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-ando iendo ar er ir<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-yendo following u<br /><br />

        in RV. In the case of (c), yendo must lie in RV, but the preceding u can be outside it.<br /><br />

        In the case of (a), deletion is followed by removing the acute accent
        (for example, haci&#233;ndola -> haciendo).
        \endhtmlonly*/
        //---------------------------------------------
        void step_0(string_typeT& text)
            {
            if (is_suffix_in_rv(text,/*selos*/LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 5))
					{ return; }
                if (step_0b(text, 5))
					{ return; }
                if (step_0c(text, 5))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*selas*/LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_A, UPPER_A, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 5))
					{ return; }
                if (step_0b(text, 5))
					{ return; }
                if (step_0c(text, 5))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*sela*/LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_A, UPPER_A))
                {
                if (step_0a(text, 4))
					{ return; }
                if (step_0b(text, 4))
					{ return; }
                if (step_0c(text, 4))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*selo*/LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_O, UPPER_O))
                {
                if (step_0a(text, 4))
					{ return; }
                if (step_0b(text, 4))
					{ return; }
                if (step_0c(text, 4))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*las*/LOWER_L, UPPER_L, LOWER_A, UPPER_A, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 3))
					{ return; }
                if (step_0b(text, 3))
					{ return; }
                if (step_0c(text, 3))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*les*/LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 3))
					{ return; }
                if (step_0b(text, 3))
					{ return; }
                if (step_0c(text, 3))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*los*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 3))
					{ return; }
                if (step_0b(text, 3))
					{ return; }
                if (step_0c(text, 3))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*nos*/LOWER_N, UPPER_N, LOWER_O, UPPER_O, LOWER_S, UPPER_S))
                {
                if (step_0a(text, 3))
					{ return; }
                if (step_0b(text, 3))
					{ return; }
                if (step_0c(text, 3))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*la*/LOWER_L, UPPER_L, LOWER_A, UPPER_A))
                {
                if (step_0a(text, 2))
					{ return; }
                if (step_0b(text, 2))
					{ return; }
                if (step_0c(text, 2))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*le*/LOWER_L, UPPER_L, LOWER_E, UPPER_E))
                {
                if (step_0a(text, 2))
					{ return; }
                if (step_0b(text, 2))
					{ return; }
                if (step_0c(text, 2))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*lo*/LOWER_L, UPPER_L, LOWER_O, UPPER_O))
                {
                if (step_0a(text, 2))
					{ return; }
                if (step_0b(text, 2))
					{ return; }
                if (step_0c(text, 2))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*me*/LOWER_M, UPPER_M, LOWER_E, UPPER_E))
                {
                if (step_0a(text, 2))
					{ return; }
                if (step_0b(text, 2))
					{ return; }
                if (step_0c(text, 2))
					{ return; }
                return;
                }
            else if (is_suffix_in_rv(text,/*se*/LOWER_S, UPPER_S, LOWER_E, UPPER_E))
                {
                if (step_0a(text, 2))
					{ return; }
                if (step_0b(text, 2))
					{ return; }
                if (step_0c(text, 2))
					{ return; }
                return;
                }
            }
        //---------------------------------------------
        bool step_0a(string_typeT& text, size_t suffix_length)
            {
            if ((text.length() >= suffix_length+5) &&
                stem<string_typeT>::get_rv() <= (text.length()-(5+suffix_length)) &&
                /*i&#233;ndo*/
                (is_either<wchar_t>(text[text.length()-(5+suffix_length)], LOWER_I, UPPER_I) &&
                    is_either<wchar_t>(text[text.length()-(4+suffix_length)], LOWER_E_ACUTE, UPPER_E_ACUTE) &&
                    is_either<wchar_t>(text[text.length()-(3+suffix_length)], LOWER_N, UPPER_N) &&
                    is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_D, UPPER_D) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_O, UPPER_O) ) )
                {
                text.erase(text.length()-suffix_length);
                text[text.length()-4] = LOWER_E;
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+4) &&
                stem<string_typeT>::get_rv() <= (text.length()-(4+suffix_length)) &&
                /*&#225;ndo*/
                (is_either<wchar_t>(text[text.length()-(4+suffix_length)], LOWER_A_ACUTE, UPPER_A_ACUTE) &&
                    is_either<wchar_t>(text[text.length()-(3+suffix_length)], LOWER_N, UPPER_N) &&
                    is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_D, UPPER_D) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_O, UPPER_O) ) )
                {
                text.erase(text.length()-suffix_length);
                text[text.length()-4] = LOWER_A;
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*&#225;r*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_A_ACUTE, UPPER_A_ACUTE) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                text[text.length()-2] = LOWER_A;
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*&#233;r*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_E_ACUTE, UPPER_E_ACUTE) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                text[text.length()-2] = LOWER_E;
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length + 2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*&#237;r*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_I_ACUTE, UPPER_I_ACUTE) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                text[text.length()-2] = LOWER_I;
                update_r_sections(text);
				return true;
                }
			return false;
            }

        //---------------------------------------------
        bool step_0b(string_typeT& text, size_t suffix_length)
            {
            if ((text.length() >= suffix_length+5) &&
                stem<string_typeT>::get_rv() <= (text.length()-(5+suffix_length)) &&
                /*iendo*/
                (is_either<wchar_t>(text[text.length()-(5+suffix_length)], LOWER_I, UPPER_I) &&
                    is_either<wchar_t>(text[text.length()-(4+suffix_length)], LOWER_E, UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-(3+suffix_length)], LOWER_N, UPPER_N) &&
                    is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_D, UPPER_D) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_O, UPPER_O) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+4) &&
                stem<string_typeT>::get_rv() <= (text.length()-(4+suffix_length)) &&
                /*ando*/
                (is_either<wchar_t>(text[text.length()-(4+suffix_length)], LOWER_A, UPPER_A) &&
                    is_either<wchar_t>(text[text.length()-(3+suffix_length)], LOWER_N, UPPER_N) &&
                    is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_D, UPPER_D) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_O, UPPER_O) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*ar*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_A, UPPER_A) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*er*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_E, UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
            else if ((text.length() >= suffix_length+2) &&
                stem<string_typeT>::get_rv() <= (text.length()-(2+suffix_length)) &&
                /*ir*/
                (is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_I, UPPER_I) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_R, UPPER_R) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
			return false;
            }

        //---------------------------------------------
        bool step_0c(string_typeT& text, size_t suffix_length)
            {
            if ((text.length() >= suffix_length+6) &&
                stem<string_typeT>::get_rv() <= text.length()-(suffix_length+5) &&
                /*uyendo*/
                (is_either<wchar_t>(text[text.length()-(6+suffix_length)], LOWER_U, UPPER_U) &&
                    is_either<wchar_t>(text[text.length()-(5+suffix_length)], LOWER_Y, UPPER_Y) &&
                    is_either<wchar_t>(text[text.length()-(4+suffix_length)], LOWER_E, UPPER_E) &&
                    is_either<wchar_t>(text[text.length()-(3+suffix_length)], LOWER_N, UPPER_N) &&
                    is_either<wchar_t>(text[text.length()-(2+suffix_length)], LOWER_D, UPPER_D) &&
                    is_either<wchar_t>(text[text.length()-(1+suffix_length)], LOWER_O, UPPER_O) ) )
                {
                text.erase(text.length()-suffix_length);
                update_r_sections(text);
				return true;
                }
			return false;
            }
        /**\htmlonly
        Search for the longest among the following suffixes, and perform the action indicated.<br /><br />

            -anza anzas ico ica icos icas ismo ismos able ables ible ibles ista istas oso osa osos osas amiento amientos imiento imientos<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br /><br />

            -adora ador aci&#243;n adoras adores aciones ante antes ancia ancias<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ic, delete if in R2<br /><br />

            -log&#237;a log&#237;as<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with log if in R2<br /><br />

            -uci&#243;n uciones<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with u if in R2<br /><br />

            -encia encias<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-replace with ente if in R2<br /><br />

            -amente<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R1<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by iv, delete if in R2 (and if further preceded by at, delete if in R2), otherwise,<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by os, ic or ad, delete if in R2<br />

            -mente<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by ante, able or ible, delete if in R2<br /><br />

            -idad idades<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by abil, ic or iv, delete if in R2<br /><br />

            -iva ivo ivas ivos<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in R2<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-if preceded by at, delete if in R2
        \endhtmlonly*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            size_t original_length = text.length();
            if (delete_if_is_in_r2(text,/*imientos*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*amientos*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (is_suffix_in_r2(text,/*uciones*/LOWER_U, UPPER_U, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                text.erase(text.length()-6);
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r2(text,/*amiento*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*imiento*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*aciones*/LOWER_A, UPPER_A, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O, UPPER_O, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C);
                    return;
                    }
                step_2a(text);
                }
            else if (is_suffix_in_r2(text,/*log&#237;as*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                text.erase(text.length()-3);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r2(text,/*encias*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                text.erase(text.length()-2);
                text[text.length()-2] = LOWER_T;
                text[text.length()-1] = LOWER_E;
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r2(text,/*idades*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    if (delete_if_is_in_r2(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) ||
                        delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                        delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                        {
                        return;
                        }
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r1(text,/*amente*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (original_length > text.length() )
                    {
                    if (delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                        {
                        delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                        return;
                        }
                    else
                        {
                        if (delete_if_is_in_r2(text,/*os*/LOWER_O, UPPER_O, LOWER_S, UPPER_S) ||
                            delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                            delete_if_is_in_r2(text,/*ad*/LOWER_A, UPPER_A, LOWER_D, UPPER_D) )
                            {
                            return;
                            }
                        }
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*adores*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S) ||
                     delete_if_is_in_r2(text,/*adoras*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S) ||
                     delete_if_is_in_r2(text,/*ancias*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A, LOWER_S, UPPER_S))
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*adora*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R, LOWER_A, UPPER_A) ||
                     delete_if_is_in_r2(text,/*aci&#243;n*/LOWER_A, UPPER_A, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O_ACUTE, UPPER_O_ACUTE, LOWER_N, UPPER_N) ||
                     delete_if_is_in_r2(text,/*antes*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_S, UPPER_S) ||
                     delete_if_is_in_r2(text,/*ancia*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ibles*/LOWER_I, UPPER_I, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*istas*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ables*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ismos*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*anzas*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_Z, UPPER_Z, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (is_suffix_in_r2(text,/*log&#237;a*/LOWER_L, UPPER_L, LOWER_O, UPPER_O, LOWER_G, UPPER_G, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A) )
                {
                text.erase(text.length()-2);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r2(text,/*uci&#243;n*/LOWER_U, UPPER_U, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_O_ACUTE, UPPER_O_ACUTE, LOWER_N, UPPER_N) )
                {
                text.erase(text.length()-4);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r2(text,/*encia*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_C, UPPER_C, LOWER_I, UPPER_I, LOWER_A, UPPER_A) )
                {
                text.erase(text.length()-1);
                text[text.length()-2] = LOWER_T;
                text[text.length()-1] = LOWER_E;
                update_r_sections(text);
                return;
                }
            else if (delete_if_is_in_r2(text,/*mente*/LOWER_M, UPPER_M, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (original_length > text.length() )
                    {
                    if (delete_if_is_in_r2(text,/*ante*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) ||
                        delete_if_is_in_r2(text,/*able*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E) ||
                        delete_if_is_in_r2(text,/*ible*/LOWER_I, UPPER_I, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E) )
                        {
                        return;
                        }
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*anza*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_Z, UPPER_Z, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*icos*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*icas*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ismo*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_M, UPPER_M, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*able*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ible*/LOWER_I, UPPER_I, LOWER_B, UPPER_B, LOWER_L, UPPER_L, LOWER_E, UPPER_E) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ista*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*osos*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*osas*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ivas*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_A, UPPER_A, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ivos*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ador*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_R, UPPER_R) ||
                    delete_if_is_in_r2(text,/*ante*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_T, UPPER_T, LOWER_E, UPPER_E) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*idad*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_D, UPPER_D) )
                {
                if (original_length > text.length() )
                    {
                    if (delete_if_is_in_r2(text,/*abil*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_I, UPPER_I, LOWER_L, UPPER_L) ||
                        delete_if_is_in_r2(text,/*ic*/LOWER_I, UPPER_I, LOWER_C, UPPER_C) ||
                        delete_if_is_in_r2(text,/*iv*/LOWER_I, UPPER_I, LOWER_V, UPPER_V) )
                        {
                        return;
                        }
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ico*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ica*/LOWER_I, UPPER_I, LOWER_C, UPPER_C, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*oso*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*osa*/LOWER_O, UPPER_O, LOWER_S, UPPER_S, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*iva*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_A, UPPER_A) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    return;
                    }
                step_2a(text);
                }
            else if (delete_if_is_in_r2(text,/*ivo*/LOWER_I, UPPER_I, LOWER_V, UPPER_V, LOWER_O, UPPER_O) )
                {
                if (original_length > text.length() )
                    {
                    delete_if_is_in_r2(text,/*at*/LOWER_A, UPPER_A, LOWER_T, UPPER_T);
                    return;
                    }
                step_2a(text);
                }    
            //this should only be called from here if nothing was removed in step 1
            if (text.length() == original_length)
                {
                step_2a(text);
                }
            }
        ///Do step 2a if no ending was removed by step 1.
        /**\htmlonly
        Search for the longest among the following suffixes in RV, and if found, delete if preceded by u.<br /><br />

        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-ya ye yan yen yeron yendo yo y&#243; yas yes yais yamos<br /><br />

        (Note that the preceding u need not be in RV.)
        \endhtmlonly*/
        //---------------------------------------------
        void step_2a(string_typeT& text)
            {
            size_t original_length = text.length();
            if (is_suffix_in_rv(text,/*yeron*/LOWER_Y, UPPER_Y, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N))
                {
                if (is_either<wchar_t>(text[text.length()-6], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yendo*/LOWER_Y, UPPER_Y, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O))
                {
                if (text.length() >= 6 &&
                    is_either<wchar_t>(text[text.length()-6], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yamos*/LOWER_Y, UPPER_Y, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S))
                {
                if (text.length() >= 6 &&
                    is_either<wchar_t>(text[text.length()-6], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-5);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yais*/LOWER_Y, UPPER_Y, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S))
                {
                if (text.length() >= 5 &&
                    is_either<wchar_t>(text[text.length()-5], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-4);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yan*/LOWER_Y, UPPER_Y, LOWER_A, UPPER_A, LOWER_N, UPPER_N))
                {
                if (text.length() >= 4 &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yen*/LOWER_Y, UPPER_Y, LOWER_E, UPPER_E, LOWER_N, UPPER_N))
                {
                if (text.length() >= 4 &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yas*/LOWER_Y, UPPER_Y, LOWER_A, UPPER_A, LOWER_S, UPPER_S))
                {
                if (text.length() >= 4 &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yes*/LOWER_Y, UPPER_Y, LOWER_E, UPPER_E, LOWER_S, UPPER_S))
                {
                if (text.length() >= 4 &&
                    is_either<wchar_t>(text[text.length()-4], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-3);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*ya*/LOWER_Y, UPPER_Y, LOWER_A, UPPER_A))
                {
                if (text.length() >= 3 &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*ye*/LOWER_Y, UPPER_Y, LOWER_E, UPPER_E))
                {
                if (text.length() >= 3 &&
                    is_either<wchar_t>(text[text.length()-3],LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*yo*/LOWER_Y, UPPER_Y, LOWER_O, UPPER_O))
                {
                if (text.length() >= 3 &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            else if (is_suffix_in_rv(text,/*y&#243;*/LOWER_Y, UPPER_Y, LOWER_O_ACUTE, UPPER_O_ACUTE))
                {
                if (text.length() >= 3 &&
                    is_either<wchar_t>(text[text.length()-3], LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-2);
                    update_r_sections(text);
                    return;
                    }
                step_2b(text);
                }
            //only called if 2a fails to remove a suffix
            if (text.length() == original_length)
                {
                step_2b(text);
                }
            }
        ///Do Step 2b if step 2a was done, but failed to remove a suffix.
        /**\htmlonly
        Search for the longest among the following suffixes in RV, and perform the action indicated.<br /><br />

            -en es &#233;is emos<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete, and if preceded by gu delete the u (the gu need not be in RV)<br /><br />

            -ar&#237;an ar&#237;as ar&#225;n ar&#225;s ar&#237;ais ar&#237;a ar&#233;is ar&#237;amos aremos ar&#225; ar&#233; er&#237;an er&#237;as
            er&#225;n er&#225;s er&#237;ais er&#237;a er&#233;is er&#237;amos eremos er&#225; er&#233; ir&#237;an ir&#237;as ir&#225;n ir&#225;s ir&#237;ais
            ir&#237;a ir&#233;is ir&#237;amos iremos ir&#225; ir&#233; aba ada ida &#237;a ara iera ad ed id ase iese aste
            iste an aban &#237;an aran ieran asen iesen aron ieron ado ido ando iendo i&#243; ar er ir
            as abas adas idas &#237;as aras ieras ases ieses &#237;s &#225;is abais &#237;ais arais ierais
            aseis ieseis asteis isteis ados idos amos &#225;bamos &#237;amos imos &#225;ramos i&#233;ramos i&#233;semos &#225;semos<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete
        \endhtmlonly*/
        //---------------------------------------------
        void step_2b(string_typeT& text) 
            {
            if (delete_if_is_in_rv(text,/*ar&#237;amos*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#237;amos*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;amos*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*i&#233;ramos*/LOWER_I, UPPER_I, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*i&#233;semos*/LOWER_I, UPPER_I, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#237;ais*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aremos*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#237;ais*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*eremos*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;ais*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iremos*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ierais*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ieseis*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*asteis*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*isteis*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#225;bamos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#225;ramos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#225;semos*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#237;an*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#237;as*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#233;is*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#237;an*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#237;as*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#233;is*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;an*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;as*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#233;is*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ieran*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iesen*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ieron*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iendo*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ieras*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ieses*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*abais*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*arais*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aseis*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;amos*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*emos*/LOWER_E, UPPER_E, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                if (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#225;n*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#225;s*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#237;a*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#225;n*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#225;s*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#237;a*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#225;n*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#225;s*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#237;a*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iera*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iese*/LOWER_I, UPPER_I, LOWER_E, UPPER_E, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aste*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*iste*/LOWER_I, UPPER_I, LOWER_S, UPPER_S, LOWER_T, UPPER_T, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aban*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aran*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*asen*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aron*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_O, UPPER_O, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ando*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*abas*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*adas*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*idas*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aras*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ases*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;ais*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ados*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*idos*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*amos*/LOWER_A, UPPER_A, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*imos*/LOWER_I, UPPER_I, LOWER_M, UPPER_M, LOWER_O, UPPER_O, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#225;*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar&#233;*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#225;*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er&#233;*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#225;*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_A_ACUTE, UPPER_A_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir&#233;*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, LOWER_E_ACUTE, UPPER_E_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*aba*/LOWER_A, UPPER_A, LOWER_B, UPPER_B, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ada*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ida*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ara*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ase*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;an*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ado*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ido*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, LOWER_O, UPPER_O, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;as*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#225;is*/LOWER_A_ACUTE, UPPER_A_ACUTE, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#233;is*/LOWER_E_ACUTE, UPPER_E_ACUTE, LOWER_I, UPPER_I, LOWER_S, UPPER_S, false) )
                {
                if (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;a*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_A, UPPER_A, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ad*/LOWER_A, UPPER_A, LOWER_D, UPPER_D, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ed*/LOWER_E, UPPER_E, LOWER_D, UPPER_D, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*id*/LOWER_I, UPPER_I, LOWER_D, UPPER_D, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*an*/LOWER_A, UPPER_A, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*i&#243;*/LOWER_I, UPPER_I, LOWER_O_ACUTE, UPPER_O_ACUTE, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ar*/LOWER_A, UPPER_A, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*ir*/LOWER_I, UPPER_I, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*as*/LOWER_A, UPPER_A, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;s*/LOWER_I_ACUTE, UPPER_I_ACUTE, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                if (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            else if (delete_if_is_in_rv(text,/*es*/LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                if (is_suffix(text,/*gu*/LOWER_G, UPPER_G, LOWER_U, UPPER_U) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            }
        ///Always do step 3.
        /**\htmlonly
        Search for the longest among the following suffixes in RV, and perform the action indicated.<br /><br />

            -os a o &#225; &#237; &#243;<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in RV<br /><br />

            -e &#233;<br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-delete if in RV, and if preceded by gu with the u in RV delete the u
        \endhtmlonly*/
        //---------------------------------------------
        void step_3(string_typeT& text) 
            {
            if (delete_if_is_in_rv(text,/*os*/LOWER_O, UPPER_O, LOWER_S, UPPER_S) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*a*/LOWER_A, UPPER_A) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*o*/LOWER_O, UPPER_O) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#225;*/LOWER_A_ACUTE, UPPER_A_ACUTE) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#237;*/LOWER_I_ACUTE, UPPER_I_ACUTE) )
                {
                return;
                }
            else if (delete_if_is_in_rv(text,/*&#243;*/LOWER_O_ACUTE, UPPER_O_ACUTE) )
                {
                return;
                }

            else if (delete_if_is_in_rv(text,/*&#233;*/LOWER_E_ACUTE, UPPER_E_ACUTE) ||
                    delete_if_is_in_rv(text,/*e*/LOWER_E, UPPER_E) )
                {
                if (is_suffix_in_rv(text,/*u*/LOWER_U, UPPER_U))
                    {
                    if (text.length() >= 2 &&
                        is_either<wchar_t>(text[text.length()-2], LOWER_G, UPPER_G) )
                        {
                        text.erase(text.length()-1);
                        }
                    }
                return;
                }
            }
        };
    }

#endif //__SPANISH_STEM_H__
