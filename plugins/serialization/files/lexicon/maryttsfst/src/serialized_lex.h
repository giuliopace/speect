/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedFile implementation for MaryTTS FST format lexicons.                  */
/* Read lexicons from MaryTTS FST formatted files.                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SERIALIZED_MARYTTSFST_LEXICONS__
#define _SPCT_PLUGIN_SERIALIZED_MARYTTSFST_LEXICONS__


/**
 * @file serialized_lex.h
 * SSerializedFile implementation for MaryTTS FST format lexicons.
 */


/**
 * @ingroup SSerializedFile
 * @defgroup SMaryttsFSTLexicon Serialized MaryTTS FST Lexicon
 * SSerializedFile implementation for MaryTTS FST format lexicons.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "lexicon_maryttsfst.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_maryttsfst_lexicon_reg(s_erc *error);


/**
 * Free the plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_maryttsfst_lexicon_free(s_erc *error);


/**
 * Read an MaryTTS FST format lexicon from the given file.
 *
 * @param path The full path and file name of the MaryTTS FST format lexicon
 * to read.
 * @param error Error code.
 *
 * @return Loaded lexicon or @c NULL on error.
 */
S_LOCAL SLexiconMaryttsFST *s_read_lexicon_maryttsfst(const char *path, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_PLUGIN_SERIALIZED_MARYTTSFST_LEXICONS__ */
