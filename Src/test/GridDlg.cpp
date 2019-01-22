// GridDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "GridDlg.h"
#include "afxdialogex.h"


// CGridDlg �Ի���

IMPLEMENT_DYNAMIC(CGridDlg, CDialogEx)

CGridDlg::CGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_GridControl, pParent)
{
	//HINSTANCE hInst = AfxGetInstanceHandle();
	//m_Grid.RegisterWindowClass(AfxGetInstanceHandle());//ע��
	double capHeight = ::GetSystemMetrics(SM_CYCAPTION);
	CWnd *pWnd = GetDlgItem(IDC_PIC_ADDRESS);
	CRect rect(0, 0, 0, 0);
	if (pWnd)
		pWnd->GetClientRect(&rect);
	m_Grid.Create(CRect(100, 100, 200, 200), this, IDC_GridControl);
	//m_Grid.Create(CRect(rect.TopLeft(), rect.TopLeft() + CSize(300, 400)), this, IDC_GridControl);
	int rowCount = 5;
	int columnCount = 5;
	m_Grid.SetColumnCount(columnCount);
	m_Grid.SetRowCount(rowCount);
	m_Grid.SetBkColor(RGB(255, 0, 0));
	m_Grid.SetGridColor(RGB(0, 255, 0));
	for (int i=0;i<rowCount;i++)
	{
		for (int j=0;j<columnCount;j++)
		{
			m_Grid.SetItemText(i, j,  _T("Hell0"));
		}
	}
	m_Grid.ShowWindow(SW_SHOW);
}

CGridDlg::~CGridDlg()
{
}

void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM, m_Grid);
}


BEGIN_MESSAGE_MAP(CGridDlg, CDialogEx)
END_MESSAGE_MAP()


// CGridDlg ��Ϣ�������
