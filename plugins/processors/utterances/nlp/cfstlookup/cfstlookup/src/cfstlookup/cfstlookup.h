#ifndef _CFSTLOOKUP_CFSTLOOKUP_HEADER_H
#define _CFSTLOOKUP_CFSTLOOKUP_HEADER_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "cfstlookup/defines.h"

#define CFSTLOOKUP_ERROR_IO_FORMAT 1

CFSTLOOKUP_BEGIN_C_DECLS

struct cfstlookup_buf_s {
	char* data;
	size_t length;
};
typedef struct cfstlookup_buf_s cfstlookup_buf_t;

struct cfstlookup_fst_s {
	int32_t* targets;
	int16_t* labels;
	char* terminal;
	char* bytes;
	cfstlookup_buf_t* strings;
};
typedef struct cfstlookup_fst_s cfstlookup_fst_t;

cfstlookup_fst_t cfstlookup_read_fst_t(FILE* fp, int* error);

void cfstlookup_fst_t_cleanup(cfstlookup_fst_t* fst);

CFSTLOOKUP_API void cfstlookup_dictionary_lookup(cfstlookup_fst_t* fst, const char* word, int32_t offset1, int32_t arc, int generate,
                                                 char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len,
                                                 int (*callback)(const char* result, size_t result_length, void* callback_data), void* callback_data);

CFSTLOOKUP_API void cfstlookup_dictionary_lookupall(cfstlookup_fst_t* fst, int32_t arc,
                                                    char** buffer1, size_t* buffer1_strlen, size_t* buffer1_len,
                                                    char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len,
                                                    char*** results);

CFSTLOOKUP_END_C_DECLS

#endif
