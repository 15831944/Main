#pragma once
#include <afxdialogex.h>


// �Ų��жԻ�����࣬���Ի�����ͼ��ʱ�Զ�����Ϊ�������ͼ��
class X_FRAMEWORK_API app_dialog_base : public CDialogEx
{
	DECLARE_SERIAL(app_dialog_base)
	DECLARE_MESSAGE_MAP()

public:
	app_dialog_base();
	app_dialog_base(UINT template_id, CWnd* parent = nullptr);
	app_dialog_base(LPCTSTR template_name, CWnd* parent = nullptr);
	virtual ~app_dialog_base();

protected:
	virtual BOOL OnInitDialog();
};

// �����ʸĻ���ĶԻ�������ڴ������ʼ��ʱ���ô˽ӿ�����
X_FRAMEWORK_API void set_window_icon_same_as_application(CWnd* wnd);
