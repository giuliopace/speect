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

#ifndef _STUFFD_READ_H
#define _STUFFD_READ_H

#include "stuffd/common.h"
#include <stdint.h>
#include <stdio.h>

S_BEGIN_C_DECLS

int64_t read_int64_t_be(FILE* fp, int* error);

int32_t read_int32_t_be(FILE* fp, int* error);

int16_t read_int16_t_be(FILE* fp, int* error);

uint64_t read_uint64_t_be(FILE* fp, int* error);

uint32_t read_uint32_t_be(FILE* fp, int* error);

uint16_t read_uint16_t_be(FILE* fp, int* error);

double read_double_be(FILE* fp, int* error);

float read_float_be(FILE* fp, int* error);

size_t read_bytes_to_array(FILE* fp, char** buffer, size_t* buffer_len, int* error);

S_END_C_DECLS

#endif /* _STUFFD_READ_H */
