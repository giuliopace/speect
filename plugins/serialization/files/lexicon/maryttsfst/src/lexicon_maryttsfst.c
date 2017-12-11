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
/* structure read from a MaryTTS FST format file. Inherits from SLexicon.           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "lexicon_maryttsfst.h"
#include "cfstlookup/cfstlookup.h"
#include "stdio.h"

/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SLexiconMaryttsFSTClass LexiconMaryttsFSTClass; /* SLexiconMaryttsFST class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/*const SList *get_matched_word_info(const SMap *singleEntry, s_bool *syllabified,
								   s_erc *error);

static const SList *get_word_info(const SObject *wordEntries, const SMap *features,
								  s_bool *syllabified, s_erc *error);

static s_bool check_if_match(const SMap *singleEntry, const SMap *features,
							 s_erc *error);*/


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_lexicon_maryttsfst_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&LexiconMaryttsFSTClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexicon_maryttsfst_class_reg",
			  "Failed to register SLexiconMaryttsFSTClass");
}


S_LOCAL void _s_lexicon_maryttsfst_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&LexiconMaryttsFSTClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexicon_maryttsfst_class_free",
			  "Failed to free SLexiconMaryttsFSTClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/




/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SLexiconMaryttsFST *self = obj;


	S_CLR_ERR(error);
	self->dictionary = NULL;
}



static void Destroy(void *obj, s_erc *error)
{
	SLexiconMaryttsFST *self = obj;


	S_CLR_ERR(error);
	if (self->dictionary != NULL) {
		cfstlookup_fst_t_cleanup(self->dictionary);
		self->dictionary = NULL;
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetName(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->name;
}


static const char *GetDescription(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->description;
}


static const char *GetLanguage(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->language;
}


static const char *GetLangCode(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->lang_code;
}


static const s_version *SGetVersion(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const s_version*)&(self->info->version);
}

static const SObject *GetFeature(const SLexicon *self, const char *key,
								 s_erc *error)
{
	const SObject *feature;


	S_CLR_ERR(error);
	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetFeature",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (self->features == NULL)
		return NULL;

	feature = SMapGetObjectDef(self->features, key, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetFeature",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return feature;
}

struct _search_logger {
	int count;
	const char* original_string;
	SList *results;
	s_bool is_first_call;
	s_erc *error;
};

static int _results_list_filler(const char* buffer, size_t buffer_length, void* data) {
	fprintf(stderr, "%s\n", "3");

	(void)(buffer_length);
	struct _search_logger* logger = (struct _search_logger*) data;

	fprintf(stderr, "%s\n", "3");
	if (logger->is_first_call==TRUE) {

		SObject *tmp = SObjectSetString(buffer, logger->error);

		fprintf(stderr, "%s\n", "g");
		SListAppend(logger->results, tmp, logger->error);

	    fprintf(stderr, "%s\n", "4");

		logger->count++;
		logger->is_first_call=FALSE;
		return 0;
	}
	else{
		return 1;
	}
}

static SList *GetWord(const SLexicon *self, const char *word,
					  const SMap *features, s_bool *syllabified,
					  s_erc *error)
{
	SList *results = NULL;
	const SLexiconMaryttsFST *lex = S_LEXICON_MARYTTSFST(self);
	(void)(features);

	S_CLR_ERR(error);
	if (word == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetWord",
				  "Argument \"word\" is NULL");
		return NULL;
	}

	if (syllabified == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetWord",
				  "Argument \"syllabified\" is NULL");
		return NULL;
	}

	if (lex->dictionary == NULL)
		return NULL;

	char* buffer = NULL;
	size_t buffer_len = 0;
	size_t buffer_strlen = 0;

	fprintf(stderr, "%s\n", "1");
	struct _search_logger data = {0, word, results, TRUE, error};
	fprintf(stderr, "%s\n", word);
	cfstlookup_dictionary_lookup(lex->dictionary, word, 0, 0, 0, &buffer, &buffer_strlen, &buffer_len, _results_list_filler, (void*)&data);

	fprintf(stderr, "%s\n", "2");
	if(buffer != NULL) {
		free(buffer);
	}

	*syllabified=TRUE;

	return results;
}


/************************************************************************************/
/*                                                                                  */
/* SLexicon class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SLexiconMaryttsFSTClass LexiconMaryttsFSTClass =
{
	/* SObjectClass */
	{
		"SLexicon:SLexiconMaryttsFST",
		sizeof(SLexiconMaryttsFST),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SLexiconClass */
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	SGetVersion,         /* get_version     */
	GetFeature,          /* get_feature     */
	GetWord              /* get_word        */
};
