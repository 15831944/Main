// HMExportTool.h : HMExportTool DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHMExportToolApp
// �йش���ʵ�ֵ���Ϣ������� HMExportTool.cpp
//

class CHMExportToolApp : public CWinApp
{
public:
	CHMExportToolApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
