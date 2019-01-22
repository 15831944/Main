#pragma once
#include <vector>
#include <map>
#include <memory>

#include "../windows_helper/multi_grid_view.h"
#include "../interface/data_center_interface.h"
#include "../interface/window_interface.h"
#include "x_observer_base.h"



// ������ֻ�����ڹ������༭�����ڵĸ������ɣ�����λ�õĴ��ڲ�֧��
class X_FRAMEWORK_API x_mvc_editor_base : public CWnd,
	public window_interface, public data_node_notify_interface
{
	DECLARE_DYNCREATE(x_mvc_editor_base)
	DECLARE_MESSAGE_MAP()

public:
	x_mvc_editor_base();
	virtual ~x_mvc_editor_base();

public:
	virtual bool load_data_to_ui(const CString& data_tag);
	virtual bool save_data_from_ui();
	virtual CString get_title();
	virtual void app_theme_changed(theme_style_settings& settings);
	virtual void on_project_data_notify(const CString& node_tag, notifer_action ac);
	virtual bool need_save_ui_data();

public:
	int get_self_index_in_working_space() const; // �ò�������-1
	void set_data_node_tag(const CString& tag); // �����༭�����ڲ�observer������
	void set_ui_observer(x_observer_base* observer); // �����༭���ڲ���observer����ȷ���ø����໥������ϵ
	CString get_data_node_tag() const;
	x_observer_base* get_ui_observer() const;

private:
	CString						m_data_tag;
	x_observer_base*			m_ui_observer;
	CString						m_custom_title;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT type, int cx, int cy);
};

// ����һ���༭�����ڲ���ӵ����������䣨��Ҫ�Ǽ��ֹ�����Ĺ��̣�
// ���ݺʹ��ڹ۲������Ϊ�գ���ʾ�˴��ڲ���Ҫ�ⲿ���ݺ����̽���������ʼҳ
// ע�⣺ʹ����x_mvc_editor_base���Ƶı༭��������ʹ�ô˽ӿ�������ϼ���
X_FRAMEWORK_API void active_workspace_mvc_editor(const CString& data_node_tag,
	const CString& ui_observer_class_name, const CString& mvc_editor_class_name);
