#pragma once
#include <vector>
#include <map>

#include "tab_multi_grid_observer.h"
#include "../windows_helper/multi_grid_view.h"
#include "x_mvc_editor_base.h"

class tabed_multi_grid_editor;

class X_FRAMEWORK_API tabed_multi_grid_control :public CWnd
{
	DECLARE_MESSAGE_MAP()

public:
	tabed_multi_grid_control(tabed_multi_grid_editor* editor);
	virtual ~tabed_multi_grid_control();

public:
	// ���ݶ����ڴ����ⲿ�༭��ͳһ�����ڲ����ݴ�ָ��
	bool load_data_to_ui(tab_multi_grid_observer* observer);
	bool save_data_from_ui(); // �����ݱ��������true��û���κ�������Ҫ���淵��false
	void set_window_backcolor(COLORREF bk_color);
	void add_mode_navigate_button(CXTPTabManagerNavigateButton* button);
	multi_grid_tab_control* get_grid_tab_control();
	bool any_databases_be_modified() const;

private:
	void clear_ui_data();
	void make_all_children_window_back_color_same_as_this();

private:
	multi_grid_tab_control				m_grid_tabs;
	tab_multi_grid_observer*			m_grid_ui_observer;
	COLORREF							m_bk_color;

	// ���洴���Ķ����������
	std::vector<CWnd*>					m_all_tab_page;
	std::map<CString, CTHDataBase*>		m_all_db_grids;

	// ��¼�������Ķ���ڣ�ʵ�����ǿ��Բ��üǵģ������´������ȡ��tab�ж�Ӧ�Ķ����
	// CXTPTabManagerItem* cur_tab_item = m_grid_tabs.GetSelectedItem();
	// CWnd* cur_wnd = (cur_tab_item ? CWnd::FromHandle(cur_tab_item->GetHandle()) : nullptr);
	// multi_grid_view* cur_view = (cur_wnd ? dynamic_cast<multi_grid_view*>(cur_wnd) : nullptr);
	// ��Ϊ��Ч�ʻ���Ҫ����һ�±ȽϺ�
	std::vector<multi_grid_view*>		m_created_multi_grid;

	tabed_multi_grid_editor*			m_editor; // �ݴ���󣬲������ڴ�

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg BOOL on_cell_value_changed(UINT id, NMHDR * notify_struct, LRESULT * result);
	afx_msg BOOL on_cell_button_clicked(UINT id, NMHDR * notify_struct, LRESULT * result);
	afx_msg BOOL on_cell_focus_changed(UINT id, NMHDR * notify_struct, LRESULT * result);
};


class X_FRAMEWORK_API tabed_multi_grid_editor : public x_mvc_editor_base
{
	DECLARE_SERIAL(tabed_multi_grid_editor)
	DECLARE_MESSAGE_MAP()

public:
	tabed_multi_grid_editor();
	virtual ~tabed_multi_grid_editor();

public:
	virtual bool load_data_to_ui(const CString& data_tag);
	virtual bool save_data_from_ui();
	virtual CString get_title();
	virtual void app_theme_changed(theme_style_settings& settings);
	virtual void on_project_data_notify(const CString& node_tag, notifer_action ac);
	virtual bool need_save_ui_data();

private:
	tabed_multi_grid_control*			m_tabbed_grid_wnd;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT type, int cx, int cy);
};

