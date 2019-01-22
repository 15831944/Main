#pragma once
#include "../resource.h"

#include "app_dialog_base.h"

class X_FRAMEWORK_API app_dialog_input : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(app_dialog_input)
	DECLARE_MESSAGE_MAP()

public:
	app_dialog_input(CWnd* pParent = NULL);
	virtual ~app_dialog_input();

public:
	enum { IDD = IDD_APP_DIALOG_INPUT };

public:
	CString						m_result_name;					// �������������������ʱָ����������ΪĬ��������䵽�����
	bool						m_checking_boot_result_name;	// ���ڼ��ص�ʱ������Ĭ���˽�����Ʊ�֤�����е����Ʋ��ظ���Ĭ��false)
	std::vector<CString>		m_exist_names;					// ��������ֵ�������
	int							m_default_width;				// ����Ĭ�Ͽ��
	int							m_default_left_pos;				// Ĭ��Ϊ0���Զ�����
	int							m_default_top_pos;				// Ĭ��Ϊ0���Զ�����
	CString						m_caption;						// ����Ĭ�ϱ���
	CString						m_label;						// ����Ĭ����ʾ����
	CString						m_repeat_message;				// ��������������Ѿ����ڵ������ظ�ʱ��������Ϣ��Ϊ�յ���Ĭ����Ϣ

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* dx);
	virtual void OnOK();

protected:
	void adjust_window_layout();
};


