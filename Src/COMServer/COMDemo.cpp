// COMDemo.cpp : CCOMDemo ��ʵ��

#include "stdafx.h"
#include "COMDemo.h"


// CCOMDemo



STDMETHODIMP CCOMDemo::Greeting(BSTR name, BSTR* message)
{
	// TODO: �ڴ����ʵ�ִ���
	CComBSTR temp("Welcome, ");
	temp.Append(name);
	*message = temp;

	//Fire_Completed();
	return S_OK;
}
STDMETHODIMP CCOMDemo::Add(long val1, long val2, long* result)
{
	*result = val1 + val2;
	Fire_Completed();
	return S_OK;

}
STDMETHODIMP CCOMDemo::Sub(long val1, long val2, long* result)
{

	*result = val1 - val2;
	Fire_Completed();
	return S_OK;
}