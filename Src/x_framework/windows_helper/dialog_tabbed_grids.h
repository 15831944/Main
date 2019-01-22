#pragma once

#include "../resource.h"
#include "afxwin.h"

#include "multi_grid_view.h"
#include "THGridControl/THGridControlInc.h"
#include "app_dialog_base.h"


class CTHExcel;
class CTHDataBase;

// �������ǩ�Ի�����࣬���Ի�����ͼ��ʱ�Զ�����Ϊ�������ͼ��
// �������ǩ�Ի����ɴ�������ֻ����дget_data_from_ui��set_data_to_ui
class X_FRAMEWORK_API dialog_tabbed_grids : public app_dialog_base
{
	DECLARE_SERIAL(dialog_tabbed_grids)
	DECLARE_MESSAGE_MAP()

public:
	dialog_tabbed_grids(CWnd* parent = nullptr);
	virtual ~dialog_tabbed_grids();

public:
	CString m_text;
	int m_default_left_pos; // Ϊ0���Զ�����
	int m_default_top_pos;  // Ϊ0���Զ�����
	int m_default_height;
	int m_default_width;

protected:
	enum { IDD = IDD_DIALOG_TABBED_GRID };

protected:
	virtual void set_template_names(std::vector<std::pair<CString, CString>>& temp_names);
	virtual void get_data_from_ui(std::map<CString, CTHDataBase*>& all_grids);
	virtual void set_data_to_ui(std::map<CString, CTHDataBase*>& all_grids);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void adjust_window_layout();

	// �ṩ�麯�������������������Ϣ��Ӧ�Ĵ��룬��Ϊ����IDû�зų���
	virtual bool grid_remove_data_in_grid(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_add_data_in_grid(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_row_col_size_changed(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_value_changed(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_button_clicked(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_focus_changed(CTHDataBase* db, NM_THGRID* notify_struct, LRESULT* result);

protected:
	multi_grid_tab_control m_tab_control;
	CButton m_button;
	CButton m_cancel;
	std::vector<std::tuple<CString, CTHExcel*, CTHDataBase*>> m_all_grid_gages;
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nType, LPRECT rect);
	afx_msg void on_button_click_ok();

	afx_msg BOOL on_remove_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_add_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_row_col_size_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_value_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_button_clicked(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_focus_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
};

