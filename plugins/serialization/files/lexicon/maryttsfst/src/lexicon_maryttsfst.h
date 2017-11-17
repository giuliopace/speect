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
/* A lexicon class implementation with the lexicon entries in a SMap                */
/* structure read from a JSON format file. Inherits from SLexicon.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_LEXICON_MARYTTSFST_H__
#define _SPCT_PLUGIN_LEXICON_MARYTTSFST_H__


/**
 * @file lexicon_maryttsfst.h
 * A lexicon class implementation with the lexicon entries in a SMap
 * structure read from a JSON format file.
 */


/**
 * @ingroup SLexicon
 * @defgroup SLexiconmaryttsfst JSON Lexicon
 * A lexicon class implementation with the lexicon entries in a SMap
 * structure read from a JSON format file. Inherits from SLexicon.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "lexicon.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SLexiconmaryttsfst child/parent class object as a
 * #SLexiconmaryttsfst object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SLexiconmaryttsfst* type.
 * @note This casting is not safety checked.
 */
#define S_LEXICON_MARYTTSFST(SELF)    ((SLexiconmaryttsfst *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SLexiconmaryttsfst definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SLexiconmaryttsfst structure.
 * @extends SLexicon
 */
typedef struct
{
	/**
	 * @protected Inherit from #SLexicon.
	 */
	SLexicon      obj;

	/**
	 * @protected Lexicon entries.
	 */
	SMap         *entries;
} SLexiconmaryttsfst;


/************************************************************************************/
/*                                                                                  */
/* SLexiconmaryttsfstClass definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of the SLexiconmaryttsfst class. This class adds no class methods to the
 * #SLexiconClass and is therefore exactly the same.
 */
typedef SLexiconClass SLexiconmaryttsfstClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SLexiconmaryttsfst plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_lexicon_maryttsfst_class_reg(s_erc *error);

/**
 * Free the #SLexiconmaryttsfst plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_lexicon_maryttsfst_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_LEXICON_MARYTTSFST_H__ */
