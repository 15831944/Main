// HMToSoft.h : HMToSoft DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHMToSoftApp
// �йش���ʵ�ֵ���Ϣ������� HMToSoft.cpp
//

class CHMToSoftApp : public CWinApp
{
public:
	CHMToSoftApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
