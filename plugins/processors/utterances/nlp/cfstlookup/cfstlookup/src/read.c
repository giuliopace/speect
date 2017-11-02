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

#include "ieee754.h"
#include "stuffd/read.h"
#include <stdlib.h>

int64_t read_int64_t_be(FILE* fp, int* error) {
	unsigned char bytes[8];
	int64_t ret = 0;
	if(fread(bytes, 8, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((int64_t) (bytes[0]) << 56) | ((int64_t) (bytes[1]) << 48) |
		((int64_t) (bytes[2]) << 40)  | ((int64_t) (bytes[3]) << 32) |
                ((int64_t) (bytes[4]) << 24) | ((int64_t) (bytes[5]) << 16) |
		((int64_t) (bytes[6]) << 8)  | (int64_t) (bytes[7]);
	return ret;
}

int32_t read_int32_t_be(FILE* fp, int* error) {
	unsigned char bytes[4];
	int32_t ret = 0;
	if(fread(bytes, 4, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((int32_t) (bytes[0]) << 24) | ((int32_t) (bytes[1]) << 16) |
		((int32_t) (bytes[2]) << 8)  | (int32_t) (bytes[3]);
	return ret;
}

int16_t read_int16_t_be(FILE* fp, int* error) {
	unsigned char bytes[2];
	int16_t ret = 0;
	if(fread(bytes, 2, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((int16_t) (bytes[0]) << 8)  |  (int16_t) (bytes[1]);
	return ret;
}

uint64_t read_uint64_t_be(FILE* fp, int* error) {
	unsigned char bytes[8];
	uint64_t ret = 0;
	if(fread(bytes, 8, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((uint64_t) (bytes[0]) << 56) | ((uint64_t) (bytes[1]) << 48) |
		((uint64_t) (bytes[2]) << 40)  | ((uint64_t) (bytes[3]) << 32) |
                ((uint64_t) (bytes[4]) << 24) | ((uint64_t) (bytes[5]) << 16) |
		((uint64_t) (bytes[6]) << 8)  | (uint64_t) (bytes[7]);
	return ret;
}

uint32_t read_uint32_t_be(FILE* fp, int* error) {
	unsigned char bytes[4];
	uint32_t ret = 0;
	if(fread(bytes, 4, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((uint32_t) (bytes[0]) << 24) | ((uint32_t) (bytes[1]) << 16) |
		((uint32_t) (bytes[2]) << 8)  | (uint32_t) (bytes[3]);
	return ret;
}

uint16_t read_uint16_t_be(FILE* fp, int* error) {
	unsigned char bytes[2];
	uint16_t ret = 0;
	if(fread(bytes, 2, 1, fp) == 0) {
		*error = IO_ERROR;
		return ret;
	}
	ret = ((uint16_t) (bytes[0]) << 8)  |  (uint16_t) (bytes[1]);
	return ret;
}

static size_t chunk_size = 8192;
size_t read_bytes_to_array(FILE* fp, char** buffer, size_t* buffer_len, int* error) {
	size_t pos = 0;
	size_t el_read = 0;
	if(*buffer_len < chunk_size) {
		char *tmp = (char*) realloc(*buffer, chunk_size);
		if(tmp == NULL) {
			*error = MEMORY_ERROR;
			return 0;
		}
		*buffer = tmp;
		*buffer_len = chunk_size;
	}
	el_read = fread(*buffer + pos, 1, chunk_size, fp);
	pos += el_read;
	while(el_read == chunk_size) {
		if(*buffer_len < (pos + chunk_size)) {
			char *tmp = (char*) realloc(*buffer, (pos + chunk_size));
			if(tmp == NULL) {
				*error = MEMORY_ERROR;
				return pos;
			}
			*buffer = tmp;
			*buffer_len = pos + chunk_size;
		}
		el_read = fread(*buffer + pos, 1, chunk_size, fp);
		pos += el_read;
	};
	if(ferror(fp)) {
		*error = IO_ERROR;
	}
	return pos;
}

double read_double_be(FILE* fp, int* error) {
        uint64_t dt = read_uint64_t_be(stdin, error);
        return unpack754_64(dt);
        //double *dp = (double*) &dt;
        //return *dp;
}

float read_float_be(FILE* fp, int* error) {
        uint32_t ft = read_uint32_t_be(stdin, error);
        return unpack754_32(ft);
        //float *f = (float*) &ft;
        //return *f;
}
