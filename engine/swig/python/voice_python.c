/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SVoice Python wrapper.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/




%extend SVoice
{
	PyObject *uttTypes;
	PyObject *uttProcessors;
	PyObject *dataObjects;
	PyObject *featProcessors;


	void uttType_set(const char *key, PyObject *uttType, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(uttType, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoice::uttType_set",
					  "Call to \"s_pyobject_2_sobject\" failed"))
			return;

		SVoiceSetUttType($self, key, S_LIST(object), error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::uttType_set", error);
	}


	PyObject *uttType_get(const char *key, s_erc *error)
	{
		const SList *uttType;
		PyObject *plist;


		uttType = SVoiceGetUttType($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		plist = s_sobject_2_pyobject(S_OBJECT(uttType), FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return plist;
	}


	void data_set(const char *key, PyObject *pobject, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(pobject, error);
		if (*error != S_SUCCESS)
			return;

		SVoiceSetData($self, key, object, error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::data_set", error);
	}


	PyObject *data_get(const char *key, s_erc *error)
	{
		const SObject *sdata;
		PyObject *pdata;


		sdata = SVoiceGetData($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		pdata = s_sobject_2_pyobject(sdata, FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pdata;
	}


	void uttProcessor_set(const char *key, PyObject *uttProc, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(uttProc, error);
		if (*error != S_SUCCESS)
			return;

		SVoiceSetUttProc($self, key, S_UTTPROCESSOR(uttProc), error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::uttProcessor_set", error);
	}


	PyObject *uttProcessor_get(const char *key, s_erc *error)
	{
		const SUttProcessor *uttProc;
		PyObject *pProc;


		uttProc = SVoiceGetUttProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;


		pProc = s_sobject_2_pyobject(S_OBJECT(uttProc), FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pProc;
	}


	void featProcessor_set(const char *key, PyObject *featProc, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(featProc, error);
		if (*error != S_SUCCESS)
			return;

		SVoiceSetFeatProc($self, key, S_FEATPROCESSOR(featProc), error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::featProcessor_set", error);

	}


	PyObject *featProcessor_get(const char *key, s_erc *error)
	{
		const SFeatProcessor *featProc;
		PyObject *fProc;


		featProc = SVoiceGetFeatProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		fProc = s_sobject_2_pyobject(S_OBJECT(featProc), FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return fProc;
	}
};



%{
	PyObject *SVoice_uttTypes_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttTypes;
		PyObject *pobject;


		S_CLR_ERR(&error);
		uttTypes = SVoiceGetUttTypesKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(uttTypes), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(uttTypes, "SVoice_uttTypes_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_uttTypes_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_uttProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttProcessors;
		PyObject *pobject;


		S_CLR_ERR(&error);
		uttProcessors = SVoiceGetUttProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(uttProcessors), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(uttProcessors, "SVoice_uttProcessors_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_uttProcessors_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_dataObjects_get(const SVoice *voice)
	{
		s_erc error;
		SList *data;
		PyObject *pobject;


		S_CLR_ERR(&error);
		data = SVoiceGetDataKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(data), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(data, "SVoice_dataObjects_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_dataObjects_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_featProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *featProcessors;
		PyObject *pobject;


		S_CLR_ERR(&error);
		featProcessors = SVoiceGetFeatProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(featProcessors), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(featProcessors, "SVoice_featProcessors_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_featProcessors_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}
%}
