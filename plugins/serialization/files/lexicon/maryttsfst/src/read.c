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
/* Read a lexicon from a MaryTTS FST format file.                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_lex.h"
#include "cfstlookup/cfstlookup.h"
#include "stdio.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SLexiconMaryttsFST *s_read_lexicon_marytts(const char *path, s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

struct _search_logger {
	int count;
	const char* original_string;
};

static int _print_results(const char* buffer, size_t buffer_length, void* data) {
	struct _search_logger* logger = (struct _search_logger*) data;
	fprintf(stdout, "string \"%s\" found: ", logger->original_string);
	fwrite(buffer, 1, buffer_length, stdout);
	fputc('\n', stdout);
	logger->count++;
	return 0;
}

S_LOCAL SLexiconMaryttsFST *s_read_lexicon_maryttsfst(const char *path, s_erc *error)
{
	SLexiconMaryttsFST *lex = NULL;
	//SMap *parsedFile = NULL;
	//const SMap *tmpMap;
	//const SObject *tmp;
	//s_bool is_present;
	//size_t entries_size;
	FILE* fp = NULL;

	S_CLR_ERR(error);

	/* create lexicon */
	lex = S_NEW(SLexiconMaryttsFST, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_maryttsfst",
				  "Failed to create new lexicon object"))
		goto quit_error;

	/* read the JSON file into a SMAP */
	/*parsedFile = s_json_parse_config_file(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"s_json_parse_config_file\" failed"))
		goto quit_error;*/

	/* get "lexicon-definition" key */
/*	tmp = SMapGetObjectDef(parsedFile, "lexicon-definition", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;
*/
/*	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_lexicon_json",
				  "Lexicon file does not have a 'lexicon-definition' key");
		goto quit_error;
	}
*/
	/* cast to make sure it's a map */
/*	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Lexicon file key 'lexicon-definition' must be a map type"))
		goto quit_error;*/

	/*set_lex_info(S_LEXICON(lex), tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"set_lex_info\" failed"))
		goto quit_error;*/

	/* get lexicon features, if any */


	/*if the path is valid open the file*/
	fp = fopen(path, "rb");

	fprintf(stderr, "%s\n", path);
	if (fp != NULL){
		cfstlookup_fst_t temp = {0};
		int err = 0;
		temp = cfstlookup_read_fst_t(fp, &err);
		if (err)
			goto quit_error;
		lex->dictionary = &temp;
		fprintf(stderr, "%s\n", "dentro if");
	}

	/* get lexicon entries */
	/*tmp = SMapGetObjectDef(parsedFile, "lexicon-entries", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_lexicon_json",
				  "Lexicon file does not have a 'lexicon-entries' key");
		goto quit_error;
	}
*/
	/* cast to make sure it's a map */
	/*tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Lexicon file key 'lexicon-entries' must be a map type"))
		goto quit_error;
*/
	/* get the size of the map */
/*	entries_size = SMapSize(tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapSize\" failed"))
		goto quit_error;
*/
	/* the entries map is a SMapList (default for JSON reader), we
	 * want a SMapHashTable. Create a new one with the given size.
	 */
/*	lex->entries = S_MAP(S_NEW(SMapHashTable, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Failed to create new 'SMapHashTable' object"))
		goto quit_error;*/

	/* initialize with the read size */
/*	SMapHashTableResize(S_MAPHASHTABLE(lex->entries), entries_size,
						error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapHashTableResize\" failed"))
	{
		S_DELETE(lex->entries, "s_read_lexicon_json", error);
		goto quit_error;
	}
*/
	/* copy the entries from the read JSON to the lexicon (from
	 * SMapList to SMapHashTable)
	 */
/*	SMapCopy(lex->entries, tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapCopy\" failed"))
		goto quit_error;
*/
	/* done */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (lex != NULL)
		//TODO distruggere dictionary
		S_DELETE(lex, "s_read_lexicon_maryttsfst", error);  /* sets lex = NULL */

	/* normal exit start clean up code here */
quit:
	if (fp!=NULL){
		fclose(fp);
		fp=NULL;
	}
	/*if (parsedFile != NULL)
		S_DELETE(parsedFile, "s_read_lexicon_marytts", error);*/

	return lex;
}
