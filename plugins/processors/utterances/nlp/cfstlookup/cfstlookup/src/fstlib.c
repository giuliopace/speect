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

#include "template-private.h"
#include "stuffd/read.h"
#include "cfstlookup/cfstlookup.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static DECLARE_ARRAY_ADD(char_array_add,char);
static DECLARE_ARRAY_APPEND(char_array_append,char);
static DECLARE_ARRAY_DELETE(char_array_delete,char);
static DECLARE_ARRAY_ADD(buf_array_add,cfstlookup_buf_t);
static DECLARE_ARRAY_APPEND(buf_array_append,cfstlookup_buf_t);
static DECLARE_ARRAY_DELETE(buf_array_delete,cfstlookup_buf_t);

static void latin1toutf8(char** buffer, size_t* buffer_len, size_t* buffer_size, size_t* word_offsets, const char* word, size_t word_len, int* error) {
        size_t i, inew;
        for(i = 0, inew = 0; i < word_len; i++) {
                if(word_offsets) {
			word_offsets[i] = inew;
		}
                unsigned char ch = (unsigned char) word[i];
                if(ch < 0x80) {
                        *error = NO_ERROR;
                        char_array_add(buffer, buffer_len, buffer_size, ch, error);
                        if(*error) {
                                return;
                        }
			inew++;
                } else {
                        *error = NO_ERROR;
                        char_array_add(buffer, buffer_len, buffer_size, (0xc0 | (ch & 0xc0) >> 6), error);
                        if(*error) {
                                return;
                        }
                        char_array_add(buffer, buffer_len, buffer_size, (0x80 | (ch & 0x3f)), error);
                        if(*error) {
                                return;
                        }
                        inew += 2;
                }
        }
}

static int str_starts_with(const char* s1, size_t s1_len, const char* prefix, size_t prefix_len, int32_t offset) {
	if( s1_len-offset < prefix_len) {
		return 0;
	}
	return !memcmp(s1+offset, prefix, prefix_len);
}

void cfstlookup_dictionary_lookup(cfstlookup_fst_t* fst, const char* word, int32_t offset1, int32_t arc, int generate,
                                  char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len, int (*callback)(const char* result, size_t result_length, void* callback_data), void* callback_data) {
        do {
		int label = fst->labels[arc];
		if(label==0) {
			if(offset1==strlen(word)) {
				if(callback != NULL) {
					callback(*buffer2, *buffer2_strlen, callback_data);
				}
			}
		} else {
			cfstlookup_buf_t s1;
			if (generate) s1 = fst->strings[2*label+1];
			else s1 = fst->strings[2*label];
			if(str_starts_with(word, strlen(word), s1.data, s1.length, offset1)) {
				cfstlookup_buf_t s2;
				if (generate) s2 = fst->strings[2*label];
				else s2 = fst->strings[2*label+1];
                                int offset2 = *buffer2_strlen;
				int error = NO_ERROR;
				char_array_append(buffer2, buffer2_strlen, buffer2_len, s2.data, s2.length, &error);
				// TODO check errors
				cfstlookup_dictionary_lookup(fst, word, offset1+s1.length, fst->targets[arc], generate, buffer2, buffer2_strlen, buffer2_len, callback, callback_data);
				if (offset2<*buffer2_strlen) {
					error = NO_ERROR;
                                        char_array_delete(buffer2, buffer2_strlen, buffer2_len, offset2, *buffer2_strlen, &error);
					// TODO check errors
				}
			}
		}
        } while (!fst->terminal[arc++]);
}



/*
 lookup_old finisce se fst->terminal[arc++] != 0 e ( (esaminato dopo arc++ precedente) fst->labels[arc] == 0 oppure ! str_starts_with(word, strlen(word), s1.data, s1.length, offset1) )
 */
void lookup_new(cfstlookup_fst_t* fst, const char* word, int32_t offset1, int32_t arc, int generate,
		      char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len,
		      char*** results) {
        // 1) Create an empty stack.
        int32_t arcs[500] = {0};
        int32_t* p = arcs;
        // 2) Initialize current arc to the passed arc
        int32_t current = arc;
        cfstlookup_buf_t in, out;
        int32_t offset2 = 0;
        offset1 = 0;
        do {
                // 3) Push the current arc to stack
                *p = current; p++;
                // and set current = targets[current] until labels[current] is 0
                int label = fst->labels[current];
                current = fst->targets[current];
                while(label !=0) {
                        current--;
                        do {
                                current++;
                                label = fst->labels[current];
                                if (generate) {
                                        in = fst->strings[2*label+1];
                                        out = fst->strings[2*label];
                                } else {
                                        in = fst->strings[2*label];
                                        out = fst->strings[2*label+1];
                                }
                                fwrite(in.data, 1, in.length, stdout);
                                fputc('?', stdout);
                                fputc('\n', stdout);
                        } while(!str_starts_with(word, strlen(word), in.data, in.length, offset1) && !fst->terminal[current]);
                        if(str_starts_with(word, strlen(word), in.data, in.length, offset1) ) {
                                offset1+=in.length;
                                fwrite(in.data, 1, in.length, stdout);
                                fputc('!', stdout);
                                fputc('\n', stdout);
                                //results.add(buffer2.toString());
                                *p = current; p++;
                                current = fst->targets[current];
                                int error = NO_ERROR;
                                char_array_append(buffer2, buffer2_strlen, buffer2_len, out.data, out.length, &error);
                                fputc(':', stdout);
                                fputc(':', stdout);
                                fputc(':', stdout);
                                fwrite(*buffer2, 1, *buffer2_strlen, stdout);
                                fputc('\n', stdout);
                        } else {
                                do {
                                        p--; current = *p;
                                        label = fst->labels[current];
                                        if (generate) {
                                                in = fst->strings[2*label+1];
                                                out = fst->strings[2*label];
                                        } else {
                                                in = fst->strings[2*label];
                                                out = fst->strings[2*label+1];
                                        }
                                        int error = NO_ERROR;
                                        char_array_delete(buffer2, buffer2_strlen, buffer2_len, *buffer2_strlen-out.length, *buffer2_strlen, &error);
                                        offset1 -= in.length;
                                        fputc('-', stdout);
                                        fputc('-', stdout);
                                        fputc('-', stdout);
                                        fwrite(in.data, 1, in.length, stdout);
                                        fputc('-', stdout);
                                        fputc('\n', stdout);
                                } while(fst->terminal[current] && p != arcs);
                                if(p == arcs) {
                                        return;
                                }
                                if(!fst->terminal[current]) {
                                        current++;
                                }
                        }
                }
                fprintf(stdout, "found: %d %zu\n", offset1, strlen(word));
                return;
        } while( p != arcs );
        return;
}

void cfstlookup_dictionary_lookupall(cfstlookup_fst_t* fst, int32_t arc,
                                     char** buffer1, size_t* buffer1_strlen, size_t* buffer1_len,
                                     char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len,
                                     char*** results) {
	int generate = 0;
        // 1) Create an empty stack.
        int32_t arcs[500] = {0};
        int32_t* p = arcs;
        // 2) Initialize current arc to the passed arc
        int32_t current = arc;
        cfstlookup_buf_t in, out;
        int32_t offset1 = 0;
        int32_t offset2 = 0;
        offset1 = 0;
        do {
                // 3) Push the current arc to stack
                *p = current; p++;
                // and set current = targets[current] until labels[current] is 0
                int label = fst->labels[current];
                while(label != 0 /*&& str_starts_with(word, strlen(word), in.data, in.length, offset1)*/){
                        current = fst->targets[current];
                        *p = current; p++;
                        label = fst->labels[current];
                        if (generate) {
                                in = fst->strings[2*label+1];
                                out = fst->strings[2*label];
                        } else {
                                in = fst->strings[2*label];
                                out = fst->strings[2*label+1];
                        }
                        offset1 += in.length;
                        offset2 += out.length;
                        /* fputc('+', stdout); */
                        /* fwrite(in.data, 1, in.length, stdout); */
                        /* fputc('\t', stdout); */
                        /* fwrite(out.data, 1, out.length, stdout); */
                        /* fputc('\n', stdout); */
                        int error = NO_ERROR;
                        char_array_append(buffer2, buffer2_strlen, buffer2_len, out.data, out.length, &error);
                        error = NO_ERROR;
                        char_array_append(buffer1, buffer1_strlen, buffer1_len, in.data, in.length, &error);

                }
			//results.add(buffer2.toString());
			fprintf(stdout, "found: ");
			fwrite(*buffer1, 1, *buffer1_strlen, stdout);
			fwrite(" | ", 1, 3, stdout);
			fwrite(*buffer2, 1, *buffer2_strlen, stdout);
			fputc('\n', stdout);
                while(fst->terminal[current] && p != arcs) {
                        p--; current = *p;
                        label = fst->labels[current];
                        if (generate) {
                                in = fst->strings[2*label+1];
                                out = fst->strings[2*label];
                        } else {
                                in = fst->strings[2*label];
                                out = fst->strings[2*label+1];
                        }
                        offset1 -= in.length;
                        offset2 -= out.length;
                        /* fputc('-', stdout); */
                        /* fwrite(in.data, 1, in.length, stdout); */
                        /* fputc('\t', stdout); */
                        /* fwrite(out.data, 1, out.length, stdout); */
                        /* fputc('\n', stdout); */
                        //fprintf(stderr, "%d\n", offset1);
                        int error = NO_ERROR;
                        char_array_delete(buffer2, buffer2_strlen, buffer2_len, offset2, *buffer2_strlen, &error);
                        error = NO_ERROR;
                        char_array_delete(buffer1, buffer1_strlen, buffer1_len, offset1, *buffer1_strlen, &error);

                }
                if(!fst->terminal[current]) {
                         current = current + 1;
                        label = fst->labels[current];
                        if (generate) {
                                in = fst->strings[2*label+1];
                                out = fst->strings[2*label];
                        } else {
                                in = fst->strings[2*label];
                                out = fst->strings[2*label+1];
                        }
                        offset1 += in.length;
                        offset2 += out.length;
                        /* fputc('+', stdout); */
                        /* fwrite(in.data, 1, in.length, stdout); */
                        /* fputc('\t', stdout); */
                        /* fwrite(out.data, 1, out.length, stdout); */
                        /* fputc('\n', stdout); */
                        int error = NO_ERROR;
                        char_array_append(buffer2, buffer2_strlen, buffer2_len, out.data, out.length, &error);
                        error = NO_ERROR;
                        char_array_append(buffer1, buffer1_strlen, buffer1_len, in.data, in.length, &error);
                }
        } while( p != arcs );
        return;
}

static void lookupall_old(cfstlookup_fst_t* fst, int32_t arc,
                          char** buffer2, size_t* buffer2_strlen, size_t* buffer2_len,
                          char** buffer3, size_t* buffer3_strlen, size_t* buffer3_len,
                          char*** results) {
	do {
		int label = fst->labels[arc];
		if(label==0) {
			//results.add(buffer2.toString());
			fprintf(stdout, "found: ");
			fwrite(*buffer3, 1, *buffer3_strlen, stdout);
			fwrite(" | ", 1, 3, stdout);
			fwrite(*buffer2, 1, *buffer2_strlen, stdout);
			fputc('\n', stdout);
		} else {
			cfstlookup_buf_t s3;
			s3 = fst->strings[2*label];
			cfstlookup_buf_t s2;
			s2 = fst->strings[2*label+1];
			int error = NO_ERROR;
                        int offset2 = *buffer2_strlen;
                        int offset3 = *buffer3_strlen;
			char_array_append(buffer2, buffer2_strlen, buffer2_len, s2.data, s2.length, &error);
			char_array_append(buffer3, buffer3_strlen, buffer3_len, s3.data, s3.length, &error);
			// TODO check errors
			lookupall_old(fst, fst->targets[arc], buffer2, buffer2_strlen, buffer2_len, buffer3, buffer3_strlen, buffer3_len, results);
			if (offset2<*buffer2_strlen) {
				error = NO_ERROR;
				char_array_delete(buffer2, buffer2_strlen, buffer2_len, offset2, *buffer2_strlen, &error);
				// TODO check errors
			}
			if (offset3<*buffer3_strlen) {
				error = NO_ERROR;
				char_array_delete(buffer3, buffer3_strlen, buffer3_len, offset3, *buffer3_strlen, &error);
				// TODO check errors
			}
		}
        } while (!fst->terminal[arc++]);
}

/**
 * The following function is loosely based on MaryTTS FST.java code.
 * The code has been used to understand the MaryTTS FST file format
 * and no verbatim code has been copied into this function.
 *
 * The MaryTTS FST file format uses big-endian byte order to store
 * integers. It starts with an encoding string stored as a 32bit
 * integer followed by a '\0' terminated string. The 32bit integer is
 * supposed to be the length of the string, not counting the '\0'
 * byte, that should also be read from file.
 *
 * After the encoding string, two 32bit integer reports the bit layout
 * of the arc information. The first integer is the overall number of
 * bits needed to represent the arc information. The second integer
 * represents the number of bits required to represent a target arc
 * index.
 *
 * After the bit layout information, an array of arc information is
 * present. The array is stored with a 32bit integer, representing the
 * array length, and the sequence of elements whose size is determined
 * by the overall number of bits defined by the bit layout
 * information. Currently the overall number of bits is supposed to be
 * 32, and the value as stored as 32bit integer (thus with big-endian
 * byte order). It is unknown how different overall number of bits
 * should be handled.
 * The least significant bits, whose number is indicated by the bit
 * layout information, represent the target arc index. Then label ids
 * are represented with the remaining bits, with the exception of the
 * most significant one. The most significant bit of each arc
 * information is supposed to be a boolean flag indicating if the
 * target arc is the last arc going out of the current node or not:
 * arcs going out from the same node are stored one next to the other.
 *
 * After the arc information, another array is present reporting bytes
 * offsets. The array is stored with a 32bit integer, representing
 * half of the length of the array, and the data elements, that are
 * stored as 16bit integer values. The first integer is half of the
 * length, because theoretically the elements of the array should be
 * two 16bit integers, not one. The integer must be greater than, or
 * equal to, the maximum value in label ids array.
 * The byte offsets are relative to the block of bytes that are stored
 * at the end of the file, after the offsets array. The block of bytes
 * at the end of the file is supposed to be the concatenation of
 * several '\0' terminated strings encoded with the encoding defined
 * at the beginning of the file. '\0' bytes are preserved in the block
 * of bytes. Current C implementation do not perform any conversion.
 * The offsets can be used to identify the strings that must be
 * associated to each label id. The offset array is mostly useless, as
 * it is always possible to reorder the strings in the bytes block, so
 * that their position in the block is equal to the labelid (moreover
 * this is already the case with current FSTs from MaryTTS). The
 * offset array could be used to compress the block if many strings
 * are reused, but in practice it does not happen.
 *
 * There is an additional legacy headerless format for MaryTTS FSTs.
 * The legacy format starts immediately with the arc information array
 * and then continues as the new format.
 * With the legacy format, the encoding should be manually provided,
 * the overall bits number is 32, the target arc bits number is 20.
 * However all known files using legacy format use Latin1 encoding,
 * that is automatically supported by this function.
 *
 * SPECIAL NOTES
 *
 * The following euristic is used to detect if current FST format is
 * the legacy headerless format or the new one: if the first 32bit
 * integer number is less than 30, it is assumed to be the length of
 * the encoding string and thus the new format with header is
 * assumed. If the first 32bit integer is greater than 30, than it is
 * assumed to be the arc number. There is currently no known
 * headerless FST file with less than 30 arcs that should be
 * supported.
 *
 * Encoding conversion is not supported in the general case. No
 * conversion is performed for files with headers. Latin1 to UTF-8
 * conversion is automatically performed for legacy headerless files.
 */
cfstlookup_fst_t cfstlookup_read_fst_t(FILE* fp, int* error) {
        /* Read encoding string */
	char *encoding = NULL;
	int32_t encoding_len = 0;
        int32_t overall_bits = 32;
        int32_t targetarc_bits = 20;
	int32_t labelid_bits = 32;
        int32_t narcs = 0;
	cfstlookup_fst_t fst = {0};

        encoding_len = read_int32_t_be(fp, error);
	//TODO CHECK ERRORS

	if(encoding_len < 30) {
                /* assume format with header */
                encoding = (char*) malloc(encoding_len+1);
                if(encoding != NULL) {
                        fgets(encoding, encoding_len+1, fp);
                        //TODO CHECK ERRORS
                        /*
                          printf("encoding: \"%s\"\n", encoding);
                        */
                }
                /* Read arc information bit layout */
                overall_bits = read_int32_t_be(fp, error);
                //TODO CHECK ERRORS
                targetarc_bits = read_int32_t_be(fp, error);
                //TODO CHECK ERRORS
                narcs = read_int32_t_be(fp, error);
                //TODO CHECK ERRORS
                if(encoding != NULL) {
                        free(encoding);
                        encoding = NULL;
                }
        } else {
                /* assume headerless format */
                narcs = encoding_len;
                encoding_len = 0;
                /* *error = IO_FORMAT_ERROR; */
                /* return (cfstlookup_fst_t){0}; */

        }

	labelid_bits = overall_bits - (targetarc_bits +1);
	/*
	  printf("encoding_len: %d\n", (int) encoding_len);
	  printf("overall_bits: %d\n", (int) overall_bits);
	  printf("targetarc_bits: %d\n", (int) targetarc_bits);
	  printf("labelid_bits: %d\n", (int) labelid_bits);
	*/
	if(overall_bits != 32 || labelid_bits > 16) {
		/* The bit layout for arc information in this FST is not supported. */
		*error = CFSTLOOKUP_ERROR_IO_FORMAT;
		return fst;
	}
        int32_t mask_targetarc = (uint32_t)0xFFFFFFFF >> (overall_bits-targetarc_bits);
        int32_t mask_labelid = (uint32_t)0xFFFFFFFF >> (overall_bits-labelid_bits);
        /*
          printf("mask_targetarc: %d\n", (int) mask_targetarc);
          printf("mask_labelid: %d\n", (int) mask_labelid);
          printf("narcs: %d\n", (int) narcs);
        */

        /* Read arc information array (label ids, target arcs, terminal bit) */

	fst.targets = (int32_t*) malloc(sizeof(int32_t)*narcs);
	fst.labels = (int16_t*) malloc(sizeof(int16_t)*narcs);
	fst.terminal = (char*) malloc(narcs);
        for(int i=0; i<narcs; i++) {
	    int32_t arc = read_int32_t_be(fp, error);
	    //TODO CHECK ERRORS

            fst.targets[i]= arc&mask_targetarc;
            fst.labels[i]=(int16_t)((arc>>targetarc_bits) & mask_labelid);
            fst.terminal[i]=((char)(arc >> (overall_bits-1)))!=0;

        }

        /* Read offsets array */
	int32_t bytes_offsets_pairs_len = read_int32_t_be(fp, error);
	//TODO CHECK ERRORS
	/*
	  printf("bytes_offsets_pairs_len: %d\n", (int) bytes_offsets_pairs_len);
	*/
        int16_t *bytes_offsets = NULL;
        bytes_offsets = (int16_t*) malloc(sizeof(int16_t)*2*bytes_offsets_pairs_len);
        for(int i=0; i<2*bytes_offsets_pairs_len; i++) {
	    bytes_offsets[i] = read_int16_t_be(fp, error);
	    //TODO CHECK ERRORS
	}

        /* Read bytes block */
	size_t bytes_length = 0;
	fst.bytes = NULL;
        size_t nbytes = read_bytes_to_array(fp, &fst.bytes, &bytes_length, error);
        /*
          printf("nbytes: %d\n", (int) nbytes);
        */
	//TODO CHECK ERRORS

        /* Store bytes as strings */
        if(encoding_len == 0) {
        	// TODO improve encoding conversions at least for Latin1 legacy format
		char* tmp_bytes = NULL;
		size_t tmp_bytes_length = 0;
		size_t tmp_bytes_size = 0;
		size_t* tmp_offsets = (size_t*) malloc (bytes_length*sizeof(size_t));
		if(tmp_offsets == NULL) {
			//TODO CHECK ERRORS
		}
                int error = NO_ERROR;
		latin1toutf8(&tmp_bytes, &tmp_bytes_length, &tmp_bytes_size, tmp_offsets, fst.bytes, nbytes, &error);
		free(fst.bytes);
		fst.bytes = tmp_bytes;
                nbytes = tmp_bytes_length ;
		for (int i=0;i<2*bytes_offsets_pairs_len;i++) {
			/*
			  fprintf(stderr, "%d %zu\n", bytes_offsets[i], tmp_offsets[bytes_offsets[i]]);
			*/
			bytes_offsets[i] =tmp_offsets[bytes_offsets[i]];
		}
		free(tmp_offsets);
		/*
		  fprintf(stderr, "\n\nConvertit\n\n");
		*/
	}
	size_t strings_len = 0;
	size_t strings_size = 0;
	fst.strings = NULL;
        for (int i=0;i<2*bytes_offsets_pairs_len;i++) {
		size_t offset = bytes_offsets[i];
                if(offset < nbytes) {
                        size_t len = strlen(fst.bytes + offset);
                        int error = NO_ERROR;
                        cfstlookup_buf_t t = { (char*) (fst.bytes + offset), len };
                        buf_array_add(&fst.strings, &strings_len, &strings_size, t, &error );

                } else {
	                /*
	                  fprintf(stderr, "\n\nERROR\n\n");
	                */
	                *error = CFSTLOOKUP_ERROR_IO_FORMAT;
	                return fst;
                }
        }
	if(bytes_offsets != NULL) {
		free(bytes_offsets);
                bytes_offsets = NULL;
	}
	return fst;
}

void cfstlookup_fst_t_cleanup(cfstlookup_fst_t* fst) {
	if(fst->strings != NULL) {
		free(fst->strings);
                fst->strings = NULL;
	}
	if(fst->bytes != NULL) {
		free(fst->bytes);
                fst->bytes = NULL;
	}
	if(fst->targets != NULL) {
		free(fst->targets);
                fst->targets = NULL;
	}
	if(fst->labels != NULL) {
		free(fst->labels);
                fst->labels = NULL;
	}
	if(fst->terminal != NULL) {
		free(fst->terminal);
		fst->terminal = NULL;
	}
}
