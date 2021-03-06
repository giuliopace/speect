/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* WIN32 threads.                                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "base/utils/alloc.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/threads/platform/win32/win32_threads.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/*
 * _S_THREAD_ERR_PRINT(ERR, FUNC, MSG)
 *
 * Print a thread error to stderr.
 * ERR The error, of type s_erc.
 * FUNC String function name in which the error occured.
 * MSG String error message.
 * FN String file name.
 * LN Interger line number.
 *
 * Aborts after error.
 *
 */

#define _S_THREAD_ERR_PRINT(ERR, FUNC, MSG, FN, LN)						\
	do {																\
		char *err_str = s_error_str(ERR);								\
		fprintf(stderr, "[FATAL ERROR (%s) %lu] %s (in function '%s', %s, %d)\n", \
				err_str, s_win32_thread_self(), MSG, FUNC, FN, LN);		\
		S_FREE(err_str);												\
		abort();														\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void s_win32_mutex_init(s_mutex_t *m, const char *file_name, int line_number)
{
	HANDLE _mutex;


	_mutex = CreateMutex(NULL, FALSE, NULL);

	if (!_mutex)
	{
		_S_THREAD_ERR_PRINT(S_FAILURE,
							"s_win32_mutex_init",
							"Call to \"CreateMutex\" failed",
							file_name, line_number);
	}

	m->mutex = _mutex;
	m->destroyed = 0;
	m->init = 1;
	m->locked = 0;
}


S_API void s_win32_mutex_destroy(s_mutex_t *m, const char *file_name, int line_number)
{
	if (m->locked == 1)
	{
		_S_THREAD_ERR_PRINT(S_FAILURE,
							"s_win32_mutex_destroy",
							"Failed to destroy mutex, mutex is locked",
							file_name, line_number);
	}

	if (CloseHandle((HANDLE)m->mutex) != 0)
	{
		_S_THREAD_ERR_PRINT(S_FAILURE,
							"s_win32_mutex_destroy",
							"Call to \"CloseHandle\" failed",
							file_name, line_number);
	}

	m->destroyed = 1;
}


S_API void s_win32_mutex_lock(s_mutex_t *m, const char *file_name, int line_number)
{
	DWORD ret;


	ret = WaitForSingleObject(m->mutex, INFINITE);

	if (ret == WAIT_OBJECT_0)
	{
		_S_THREAD_ERR_PRINT(S_FAILURE,
							"s_win32_mutex_lock",
							"Call to \"WaitForSingleObject\" failed",
							file_name, line_number);
	}

	m->locked = 1;
}


S_API void s_win32_mutex_unlock(s_mutex_t *m, const char *file_name, int line_number)
{
	DWORD ret;


	ret = ReleaseMutex(m->mutex);

	if (ret == 0)
	{
		_S_THREAD_ERR_PRINT(S_FAILURE, "s_win32_mutex_unlock",
							"Call to \"ReleaseMutex\" failed",
							file_name, line_number);
	}

	m->locked = 0;
}


S_API unsigned long s_win32_thread_self(void)
{
	return (unsigned long)GetCurrentThreadId();
}
