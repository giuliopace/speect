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
/* Serialized object reader functions for floating point matrix data types          */
/* in ebml format.                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "matrix_float.h"
#include "ebml.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void read_row_data(SEbmlRead *reader, SMatrixFloat *self, s_erc *error);

static void read_col_data(SEbmlRead *reader, SMatrixFloat *self,
						  uint32 row_counter, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void read_float_matrix(SEbmlRead *reader, SObject *object, s_erc *error)
{
	SMatrixFloat *self = (SMatrixFloat*)object;
	uint32 id;
	s_bool container_exhausted;
	uint32 counter;


	S_CLR_ERR(error);

	/* read S_EBML_ID_OBJECT_DATA container, id is checked in the
	 * 'read_object' method of the SEbmlRead class
	 */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_float_matrix",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_float_matrix",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the matrix object */

		/* peek id for matrix object elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_float_matrix",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_MATRIX_FLOAT_EBML_ROW_COUNT:
		{
			/* read number of rows */
			self->row_count = S_EBMLREAD_CALL(reader, read_uint)(reader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_float_matrix",
						  "Call to SEbmlRead \"read_uint\" failed"))
				return;
			break;
		}
		case S_MATRIX_FLOAT_EBML_COL_COUNT:
		{
			/* read number of rows */
			self->col_count = S_EBMLREAD_CALL(reader, read_uint)(reader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_float_matrix",
						  "Call to SEbmlRead \"read_uint\" failed"))
				return;
			break;
		}
		case S_MATRIX_FLOAT_EBML_ROW_DATA:
		{
			/* allocate matrix memory */
			self->f = S_MALLOC(float*, self->row_count);
			if (self->f == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "read_float_matrix",
						  "Failed to allocate memory for 'float*' object");
				return;
			}

			for (counter = 0; counter < self->row_count; counter++)
			{
				self->f[counter] = S_MALLOC(float, self->col_count);
				if (self->f[counter] == NULL)
				{
					S_FTL_ERR(error, S_MEMERROR,
							  "read_float_matrix",
							  "Failed to allocate memory for 'float' object");
					return;
				}
			}

			read_row_data(reader, self, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_float_matrix",
						  "Call to \"read_row_data\" failed"))
				return;
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_float_matrix",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_float_matrix",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void read_row_data(SEbmlRead *reader, SMatrixFloat *self, s_erc *error)
{
	uint32 id;
	s_bool container_exhausted;
	uint32 row_counter;


	S_CLR_ERR(error);

	/* read S_MATRIX_FLOAT_EBML_ROW_DATA container */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_row_data",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	row_counter = 0;
	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_row_data",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the row data */

		/* peek id for row data elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_row_data",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_MATRIX_FLOAT_EBML_COL_DATA:
		{
			read_col_data(reader, self, row_counter++, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_row_data",
						  "Call to \"read_col_data\" failed"))
				return;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_row_data",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_row_data",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}


static void read_col_data(SEbmlRead *reader, SMatrixFloat *self, uint32 row_counter, s_erc *error)
{
	uint32 id;
	s_bool container_exhausted;
	uint32 col_counter;


	S_CLR_ERR(error);

	/* read S_MATRIX_FLOAT_EBML_COL_DATA container */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_col_data",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	col_counter = 0;
	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_col_data",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the column data */

		/* peek id for column data elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_col_data",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_MATRIX_FLOAT_EBML_DATA_ELEMENT:
		{
			self->f[row_counter][col_counter++] = S_EBMLREAD_CALL(reader,
																  read_float)(reader,
																			  &id,
																			  error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_col_data",
						  "Call to SEbmlRead \"read_float\" failed"))
				return;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_col_data",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_col_data",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}
