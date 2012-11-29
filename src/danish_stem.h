/***************************************************************************
                          danish_stem.h  -  Danish stemmer
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

#ifndef __DANISH_STEM_H__
#define __DANISH_STEM_H__

#include "stemming.h"

namespace stemming
    {
    /**\htmlonly
    The Danish alphabet includes the following additional letters:<br />
        -&#230;   &#229;   &#248;<br /><br />
    The following letters are vowels:<br />
        -a   e   i   o   u   y   &#230;   &#229;   &#248;<br /><br />
    A consonant is defined as a non-vowel.<br /><br />

    R2 is not used: R1 is defined in the same way as in the German stemmer.
    \endhtmlonly*/
    //------------------------------------------------------
    template <typename string_typeT = std::wstring>
    class danish_stem : public stem<string_typeT>
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

            //reset internal data
            stem<string_typeT>::reset_r_values();

            trim_western_punctuation(text);

            //see where the R1 section begins
            //R1 is the first consonant after the first vowel
            find_r1(text, DANISH_VOWELS);
            if (stem<string_typeT>::get_r1() == text.length() )
                {
                return;
                }
            //R1 must have at least 3 characters in front of it
            if (stem<string_typeT>::get_r1() < 3)
                {
                stem<string_typeT>::set_r1(3);
                }
            //danish does not use R2

            step_1(text);
            step_2(text);
            step_3(text);
            step_4(text);
            }
    private:
        /**Search for the longest among the following suffixes in R1, and perform the action indicated. 

            -#hed   ethed   ered   e   erede   ende   erende   ene   erne   ere   en   heden   eren   er
              heder   erer   heds   es   endes   erendes   enes   ernes   eres   ens   hedens   erens   ers   ets   erets   et   eret 
                -delete

            -#s
                -delete if preceded by a valid s-ending 

        (Of course the letter of the valid s-ending is not necessarily in R1)*/
        //---------------------------------------------
        void step_1(string_typeT& text)
            {
            if (delete_if_is_in_r1(text,/*erendes*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*erende*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*hedens*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ethed*/LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*erede*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*heden*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*heder*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*endes*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ernes*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*erens*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*erets*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_S, UPPER_S, false) )
                {
                return;
                }    
            else if (delete_if_is_in_r1(text,/*eres*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*enes*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*heds*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*erer*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*eren*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*erne*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ende*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_D, UPPER_D, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ered*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_D, UPPER_D, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*eret*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, LOWER_T, UPPER_T, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*hed*/LOWER_H, UPPER_H, LOWER_E, UPPER_E, LOWER_D, UPPER_D, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ets*/LOWER_E, UPPER_E, LOWER_T, UPPER_T, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ere*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ene*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_E, UPPER_E, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ens*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, LOWER_S, UPPER_S, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*ers*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, LOWER_S, UPPER_S, false) )
                {
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
            else if (delete_if_is_in_r1(text,/*er*/LOWER_E, UPPER_E, LOWER_R, UPPER_R, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,/*en*/LOWER_E, UPPER_E, LOWER_N, UPPER_N, false) )
                {
                return;
                }
            else if (delete_if_is_in_r1(text,LOWER_E, UPPER_E, false) )
                {
                return;
                }
            /**\htmlonly
            Define a valid s-ending as one of 
                -a b c d f g h j k l m n o p r t v y z &#229;
            \endhtmlonly*/
            else if (is_suffix_in_r1(text,LOWER_S, UPPER_S) )
                {
                if (text.length() >= 2 &&
                    string_util::is_one_of(text[text.length()-2], DANISH_ALPHABET) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                return;
                }
            }
        /**Search for one of the following suffixes in R1, and if found delete the last letter. 

            -gd   dt   gt   kt 

        (For example, friskt -> frisk)*/
        //---------------------------------------------
        void step_2(string_typeT& text)
            {
            if (is_suffix_in_r1(text,/*gd*/LOWER_G, UPPER_G, LOWER_D, UPPER_D) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r1(text,/*dt*/LOWER_D, UPPER_D, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r1(text,/*gt*/LOWER_G, UPPER_G, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            else if (is_suffix_in_r1(text,/*kt*/LOWER_K, UPPER_K, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                return;
                }
            }
        /**\htmlonly
        If the word ends igst, remove the final st Search for the longest among the following suffixes in R1, and perform the action indicated.<br /><br />

            -#ig   lig   elig   els<br />
                -delete, and then repeat step 2<br /><br />

            -#l&#248;st<br />
                -replace with l&#248;s
        \endhtmlonly*/
        //---------------------------------------------
        void step_3(string_typeT& text)
            {
            //do this check separately
            if (is_suffix(text,/*igst*/LOWER_I, UPPER_I, LOWER_G, UPPER_G, LOWER_S, UPPER_S, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-2);
                update_r_sections(text);
                }
            //now start looking for the longest suffix
            if (delete_if_is_in_r1(text,/*elig*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                step_2(text);
                return;
                }
            else if (is_suffix_in_r1(text,/*l&#248;st*/LOWER_L, UPPER_L, LOWER_O_STROKE, UPPER_O_STROKE, LOWER_S, UPPER_S, LOWER_T, UPPER_T) )
                {
                text.erase(text.length()-1);
                update_r_sections(text);
                }
            else if (delete_if_is_in_r1(text,/*lig*/LOWER_L, UPPER_L, LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                step_2(text);
                return;
                }
            else if (delete_if_is_in_r1(text,/*els*/LOWER_E, UPPER_E, LOWER_L, UPPER_L, LOWER_S, UPPER_S, false) )
                {
                step_2(text);
                return;
                }
            else if (delete_if_is_in_r1(text,/*ig*/LOWER_I, UPPER_I, LOWER_G, UPPER_G, false) )
                {
                step_2(text);
                return;
                }
            }
        /**Step 4: undouble 
        If the word ends with double consonant in R1, remove one of the consonants. 

        (For example, bestemmelse -> bestemmels (step 1) -> bestemm (step 3a) -> bestem in this step.)*/
        //---------------------------------------------
        void step_4(string_typeT& text)
            {
            ///undouble consecutive (same) consonants if either are in R1 section
            if (text.length() >= 2 &&
                stem<string_typeT>::get_r1() <= text.length()-1 &&
                string_util::tolower_western(text[text.length()-2]) == string_util::tolower_western(text[text.length()-1]) )
                {
                if (!string_util::is_one_of(text[text.length()-2], DANISH_VOWELS) )
                    {
                    text.erase(text.length()-1);
                    update_r_sections(text);
                    }
                }
            }
        };
    }

#endif //__DANISH_STEM_H__
