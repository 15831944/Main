#pragma once
#include "../resource.h"
#include "hook_setting_data.h"


class CTHExcel;
class CTHDataBase;


class X_FRAMEWORK_API dialog_node_hook_setting : public CDialog
{
	DECLARE_DYNAMIC(dialog_node_hook_setting)
	DECLARE_MESSAGE_MAP()

public:
	dialog_node_hook_setting(CWnd* parent = nullptr);
	virtual ~dialog_node_hook_setting();

public:
	enum { IDD = IDD_DIALOG_HOOK_SETTING };

public:
	CString		m_setting_info_text;		// ����е�����
	CString		m_ref_lines;				// �ο���,�������õ��ο��ߵ������б���
	CString		m_ref_segment;				// �ο���,�������õ��ο��ε������б���
	CString		m_hook_name_list;			// �乳����,������ʾ�������б���
	CString		m_setment_sketch;			// �ο��ε�ʾ��ͼ���ַ����ĵ��͸�ʽΪ��THBW-PM-001-A01--D#15990

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* dx);
	virtual void OnOK();
	virtual void OnCancel();

private:
	void get_data_from_grid(OUT std::vector<frame_hook_setting>& setting_data);
	void set_data_to_grid(IN const std::vector<frame_hook_setting>& setting_data);
	

private:
	CTHExcel* m_hook_excel;
	CTHDataBase* m_hook_settings_db;

private:
	afx_msg void OnClose();
	afx_msg BOOL on_focus_changed( UINT nID, NMHDR * notify_struct, LRESULT * result );
};
