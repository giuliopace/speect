/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Simone Daminato                                                   */
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
/* Word num syllables feature processor.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "word_num_syls.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWordNumSylsFeatProcClass WordNumSylsFeatProcClass; /* SWordNumSylsFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_word_num_syls_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&WordNumSylsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_num_syls_class_reg",
			  "Failed to register SWordNumSylsFeatProcClass");
}


S_LOCAL void _s_word_num_syls_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&WordNumSylsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_num_syls_class_free",
			  "Failed to free SWordNumSylsFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	const SItem *itemInSylStructRel;
	const SItem *itrItem = NULL;
	sint32 count;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInSylStructRel == NULL)
	{
		extractedFeat = SObjectSetInt(0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;
	}
	else
	{
		itrItem = SItemDaughter(itemInSylStructRel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					"Run",
					"Call to \"SItemDaughter\" failed"))
			goto quit_error;
	}

	count = 0;
	while (itrItem != NULL)
	{
		count++;
		itrItem = SItemNext(itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	extractedFeat = SObjectSetInt(count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetInt\" failed"))
		goto quit_error;

	/* all OK here */
	return extractedFeat;

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SWordNumSylsFeatProc class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static SWordNumSylsFeatProcClass WordNumSylsFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SWordNumSylsFeatProc",
		sizeof(SWordNumSylsFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	NULL,               /* initialize */
	Run                  /* run     */
};
