#pragma once
#include "HMGridControl/GridCtrl.h"

// CGridDlg �Ի���

class CGridDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGridDlg)

public:
	CGridDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGridDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_GridControl };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_Grid;
};
