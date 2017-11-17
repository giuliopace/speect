/*
 * Copyright (c) 2014-2017, Giulio Paci <giuliopaci@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
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

#ifndef _STUFFD_TEMPLATE_PRIVATE_H
#define _STUFFD_TEMPLATE_PRIVATE_H


#define DECLARE_ARRAY_ADD(functionname,datatype)			\
	void functionname(datatype** array, size_t* array_length, size_t* array_size, datatype element, int* error) { \
		if(*array == NULL) {					\
			datatype* tmp = (datatype*) malloc(sizeof(datatype)*16); \
			if(tmp == NULL) {				\
				*error = MEMORY_ERROR;			\
				return;					\
			}						\
			*array = tmp;					\
			*array_size = 16;				\
			*array_length = 0;                              \
		}							\
		if(*array_size < *array_length + 1 ) {			\
			size_t tmp_len = *array_size * 2;		\
			datatype *tmp = (datatype*) realloc((*array), tmp_len*sizeof(datatype)); \
			if(tmp == NULL) {				\
				*error = MEMORY_ERROR;			\
				return;					\
			}						\
			*array = tmp;					\
			*array_size = tmp_len;                          \
		}							\
		(*array)[*array_length] = element;                      \
		*array_length = *array_length + 1;                      \
	}

#define DECLARE_ARRAY_APPEND(functionname,datatype)			\
	void functionname(datatype** array, size_t* array_length, size_t* array_size, const datatype* element_array, size_t element_array_length, int* error) { \
		if(*array == NULL) {					\
			size_t tmp_len = 16;				\
			while(tmp_len < *array_length + element_array_length ) { \
				tmp_len *= 2;				\
			}						\
			datatype* tmp = (datatype*) malloc(tmp_len*sizeof(datatype)); \
			if(tmp == NULL) {				\
				*error = MEMORY_ERROR;			\
				return;					\
			}						\
			*array = tmp;					\
			*array_size = tmp_len;                          \
			*array_length = 0;                              \
		}							\
		if(*array_size < *array_length + element_array_length ) { \
			size_t tmp_len = *array_size;			\
			while(tmp_len < *array_length + element_array_length ) { \
				tmp_len *= 2;				\
			}						\
			datatype *tmp = (datatype*) realloc((*array), tmp_len*sizeof(datatype)); \
			if(tmp == NULL) {				\
				*error = MEMORY_ERROR;			\
				return;					\
			}						\
			*array = tmp;					\
			*array_size = tmp_len;                          \
		}							\
		memcpy((*array) + *array_length, element_array, element_array_length*sizeof(datatype)); \
		*array_length = *array_length + element_array_length;   \
	}

#define DECLARE_ARRAY_DELETE(functionname,datatype)			\
	void functionname(datatype** array, size_t* array_length, size_t* array_size, size_t start, size_t end, int* error) { \
		if(start >= end) {					\
			return;						\
		}							\
		if(end >= *array_length) {                              \
			*array_length = start;                          \
			return;						\
		}							\
		memmove(array+start, array, (end-start)*sizeof(datatype)); \
	}

#define DECLARE_ARRAY_TRIM(functionname,datatype)			\
	void functionname(datatype** array, size_t* array_length, size_t* array_size, int* error) { \
                if(*array == NULL) {                                    \
                        return;                                         \
                }                                                       \
                if(*array_size > *array_length)                         \
                {                                                       \
			datatype *tmp = (datatype*) realloc((*array), (*array_length)*sizeof(datatype)); \
			if(tmp == NULL) {				\
				*error = MEMORY_ERROR;			\
			} else {                                        \
                                *array_size = *array_length;            \
                        }                                               \
                        return;                                         \
                }                                                       \
	}


#endif
