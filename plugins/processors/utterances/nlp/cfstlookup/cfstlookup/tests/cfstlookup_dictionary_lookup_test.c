/*
 * Copyright (c) 2014-2017, Giulio Paci <giuliopaci@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 *  modification, are permitted #provided that the following conditions
 *  are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of #conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "cfstlookup/cfstlookup.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char**argv) {
	FILE* fp = NULL;
	int error = 0;
	cfstlookup_fst_t fst = {0};

	if(argc <= 1) {
		fprintf(stderr, "Error: %s\n", "Missing FST file");
		return -1;
	}
	if(argc <= 2) {
		fprintf(stderr, "Error: %s\n", "Missing words to lookup");
		return -1;
	}

	fp = fopen(argv[1], "rb");
	if(fp == NULL) {
		fprintf(stderr, "Error: %s %s\n", "Unable to open FST file", argv[1]);
		return -2;
	}

	fst = cfstlookup_read_fst_t(fp, &error);
	fclose(fp);
        if(error) {
	        fprintf(stderr, "Error: %s\n", "FST file read failed");
                cfstlookup_fst_t_cleanup(&fst);
                return -3;
        }

	char* buffer2 = NULL;
	size_t buffer2_len = 0;
	size_t buffer2_strlen = 0;
	for(int i = 2; i < argc; i++) {
		const char* word = argv[i];
		struct _search_logger data = { 	0, word };
		cfstlookup_dictionary_lookup(&fst, word, 0, 0, 0, &buffer2, &buffer2_strlen, &buffer2_len, _print_results, (void*)&data);
		if(data.count < 1) {
			fprintf(stdout, "string \"%s\" not found!\n", word);
		}
	}
	if(buffer2 != NULL) {
		free(buffer2);
	}
        cfstlookup_fst_t_cleanup(&fst);
	return 0;
}
