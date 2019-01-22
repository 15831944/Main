#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_APP_UI_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_APP_UI_INTERFACE_H__

#include <map>
#include <vector>
#include <utility>

#include "XTToolkit\XTToolkitPro.h"


class window_interface;


enum class app_theme_type
{
	theme_unknown,
	theme_office_2007_blue,
	theme_office_2007_silver,
	theme_office_2007_black,
	theme_office_2010_blue,
	theme_office_2010_silver,
	theme_office_2010_black,
	theme_office_2013_white,
	theme_office_2013_dark
};


// Ƥ����������ز���
struct X_FRAMEWORK_API theme_style_settings
{
	int							m_style_id;							// ��ǰ�Ľ�����ʾ���˵���id
	app_theme_type				m_style_type;						// ������ʽ
	XTPToolTipStyle				m_tooltip_style;					// ��ʾ���
	XTPDockingPanePaintTheme	m_docking_pane_theme;				// ͣ�������
	XTPTabAppearanceStyle		m_tab_appearance;					// ѡ��Լ�ͣ�����ͣ����ı�ǩ���
	XTPPaintTheme				m_paint_theme;						// �����������ط��
	CString						m_theme_dll_ini_config_file_name;	// �����Ӧ��dll����Ƕ��ini�����ļ��������޴�Сд
	XTPButtonStyle				m_ribbon_system_button_style;		// ribbonϵͳ��ť���
	bool						m_enable_aero;						// �Ƿ����ñ���acroЧ��
	CString						m_skin_file_name;					// �����ļ�������
	CString						m_skin_file_ini_section_name;		// ��ǰ�����Ӧ��Ƥ���ļ������õ�����ģʽ���������޴�Сд
	XTPTabColorStyle			m_tab_color;						// ѡ���ɫ���

	//////////////////////////////////////////////////////////////////////////

	COLORREF					m_window_back_color;				// �Ƽ��Ĵ��ڱ���ɫ
};


// ������Ҫͨ��app_ui_interface����Ϣ�����Ŀɵ��ö������Ӵ�������
class X_FRAMEWORK_API functor_by_message
{
public:
	functor_by_message();
	virtual ~functor_by_message();

public:
	virtual void call() = 0;
};


// ����������ṩ�Ŀɵ��ýӿڣ����ṩ����Ĳ������ܣ����漰�κε�ҵ����
class X_FRAMEWORK_API app_ui_interface
{
public:
	app_ui_interface();
	virtual ~app_ui_interface();

public:
	// �жϹ������Ƿ��Ѿ�����ָ���Ĵ��ڣ���������һ��������tag��һ������Ϊƥ�䣬������������Win32��ע�ᴰ���࣬����CWnd��������������
	bool is_working_space_view_exist(const CString& class_name, const CString& tag);

	// �л���ָ���Ĺ��������ڣ���������ڣ���ֱ�ӷ���
	void select_working_space_view(const CString& class_name, const CString& tag);

	// �õ����������ڵ�ѡ�����ֵ����0��ʼ�����������򷵻�-1
	int get_working_space_view_index(const CString& class_name, const CString& tag);
	int get_working_space_view_index(const window_interface* wnd);

public:
	// ���һ����Ŀ���ؼ��У����¼ԭ���ڵ�ָ�룩����������Ӻ��tab���ڵı�ǩ���
	// �����Ͳ�����һ���Ĵ����Ѿ����ڵ�������ǲ������������ڵ�
	// ע�⣺
	//   �������ΪCView�������࣬��CView���ڴ�������֮��OnPostDestroy����delete this�����޷��ÿգ����¿����
	//   ָ�����գ����ٴ���ʱ��������˶�CView����������Ҫ���⴦������PostNcDestroy�麯������ֹ����CView�İ汾
	//   ���ɣ�Ҳ�����ٵ���delete����������ؼ�����ȷ�������Ⲣ��������ڴ�й©
	virtual int add_working_space_view(const CString window_class, const CString node_tag, int index = -1, int image_index = -1);
	
	// ���һ���ⲿʵ�����õĴ��ڵ�����������������Ӻ��tab���ڵı�ǩ��ţ��ڴ��ɹ��������ܣ�ע�⣺������ڱ������ڶ��Ϸ����
	// �ⲿֻҪ�����ô��ڼ��ɣ����ݵļ��ص������Խӿں����������ɿ���Զ����
	// �������������󣬷����Ƽ�ʹ�ã�����ʹ�ô���������ذ汾���������Ĵ�������������tag������ͬ����ӣ�
	// ע�⣺
	//   �������ΪCView�������࣬��CView���ڴ�������֮��OnPostDestroy����delete this�����޷��ÿգ����¿����
	//   ָ�����գ����ٴ���ʱ��������˶�CView����������Ҫ���⴦������PostNcDestroy�麯������ֹ����CView�İ汾
	//   ���ɣ�Ҳ�����ٵ���delete����������ؼ�����ȷ�������Ⲣ��������ڴ�й©
	virtual int add_working_space_view(window_interface* wnd, const CString node_tag, int index = -1, int image_index = -1);
	
	// ��ù������е����д���
	virtual void get_all_working_space_windows(std::vector<window_interface*>& wnds);

	// ����һ�����й��������ڵ�ѡ�����
	virtual void update_all_working_space_window_captions();
	
	// ��ù������е�ǰ����Ĵ�����Ϣ,firstΪ�����࣬secondΪ�������tag,���û�л��ȡʧ�ܣ���first��secon������_T("")
	virtual void get_current_working_space_active_window(std::pair<CString, CString>& info);

	// ��ù������еĵ�ǰ����
	virtual window_interface* get_current_working_space_windows();

	// ��ù������е����д���,firstΪ�����࣬secondΪ���ݽڵ��tag
	virtual void get_all_working_space_windows_info(std::vector<std::pair<CString, CString>>& info);

	// ��ȡ��������������
	virtual int get_working_space_view_count();

	// ��ȡָ�������Ĵ�����Ϣ0������������ 1���������tag��2������ָ�룬�ɹ�����true�����򷵻�false
	virtual bool get_tab_window_and_info(std::tuple<CString, CString, window_interface*>& info, int index) const;

	// �ر�ָ������������
	virtual bool close_working_space_view_by_class(const CString& class_name, bool force_close = false); // ������ͬ��ȫ���ر�
	virtual bool close_working_space_view_by_tag(const CString& tag, bool force_close = false); // tag��ͬ��ȫ���ر�(һ�������ò�ͬ���͵Ĵ�����ʾ)
	virtual bool close_working_space_view_by_index(int index, bool force_close = false);

	// ���湤����������н������ݵ�ģ������
	virtual void save_all_workspace_window_ui_data();

	// ���ݹ��������ڵ���Ӧ�ӿ����¸��¶�Ӧ��ѡ�����
	virtual void update_working_space_tab_caption(int index);

	// �������ͣ�����й���Ĵ���
	virtual void get_all_docking_pane_window(std::vector<window_interface*>& wnds);

	// ��ȡ����ָ���������ͣ�����й���Ĵ��ڣ��������Ϊ�ձ�ʾ��ȡ���д�window_interface�����Ķ���
	virtual void get_all_docking_pane_window_by_class(std::vector<window_interface*>& wnds, const CString& class_name);

	// ��ȡָ�������ʶ��ͣ�����й���Ĵ���
	virtual window_interface* get_docking_pane_window_by_identify(const CString& app_identify);

	// ��ȡͣ�����й����ָ��id���ڣ�id����ͣ���̵�id�������ڲ�������Ӵ���id��
	virtual window_interface* get_docking_pane_window_by_id(int id);

	// ��������������ͣ�����(����Ѿ��������ˣ��Ͳ�Ҫ�ٴ�����)
	virtual void destroy_all_docking_panes();
	virtual void create_all_docking_panes();

	// ��ȡͣ�����й���Ĵ���id,�Ҳ�������0
	virtual int get_docking_pane_window_id(window_interface* wnd);

	// ��ʾ������ָ��id�Ĵ���(id����ͣ���̵�id�������ڲ�������Ӵ���id��������ͬ�Ĵ��ڿ����ж��)
	virtual void hide_docking_pane_by_id(int id);
	virtual void show_docking_pane_by_id(int id);

	// ��ʾ������ָ�������Ĵ���
	virtual void hide_docking_pane_by_class(const CString& class_name);
	virtual void show_docking_pane_by_class(const CString& class_name);

	// ��ʾ������ָ�������ʶ�Ĵ���
	virtual void hide_docking_pane_by_identify(const CString& identify);
	virtual void show_docking_pane_by_identify(const CString& identify);
	
	// ���¼���ָ��id��ͣ������еĽ�������
	virtual void reload_docking_pane_by_id(int id, const CString& node_tag);
	virtual void reload_docking_pane_data_by_identify(const CString& identify, const CString& node_tag);
	virtual void reload_docking_pane_data_by_class(const CString& class_name, const CString& node_tag);

	// ��������ͣ����崰���е�����
	virtual void save_all_docking_pane_ui_data();

	// �رչ�����������д��ڣ����Զ����ñ���������ݣ�
	virtual bool close_all_workspace_window(bool force_close = false);

	// ��ȡ�����ó�����⣬�˽ӿ��봰�ڵı������ò�̫һ�����˽ӿ�ʵ��ʱ���������ı������ݴ����߼���������ϳ�������
	// �ղ�����ʾĬ�ϱ���
	virtual void set_title(const CString& title);
	virtual CString get_title() const;

	// �õ�ribbon�ؼ���tab�����еĿؼ�����
	// ע�⣺
	//   1.�������еĿؼ����������Ӧ��û������Ŀؼ���ʾ���߱���Ϊ������ؼ�û�����塣
	//   2.��һ��ؼ���Ϊ���ƣ�ʹ����ͬ�����ָ����ͬ�Ĳ�����ɶ����Ĺ���������������Ҫ������ҵĲ�����
	//     ��compare_param��Ϊtrue�����ܻ��ڲ��ҿؼ�ʱ��ͬ����Ҳ���бȽϣ�����������ıȽϲ����ִ�Сд��
	//   3.��Щ�ؼ��ǲ�����ָ�������ģ�ָ����Ҳ��Ч����combo�ؼ��������ɿ�ܾ����������û����ã�����μ�ribbon��
	//     xml�����ļ����ݣ������ֿؼ�����ʹ�ò�ͬ�����������ʹ�ò�����֮����
	virtual CXTPControl* get_ribbon_control(const CString& domain, const CString& cmd, const CString& param = _T(""), bool compare_param = false);

	// ѡ��Ribbon��ǩ(�����Ͽ������ı�ǩ����ʾ)
	virtual void select_ribbon_tab(const CString& tab_name);

	// ��ʾָ���������ı�ǩ��δָ���������ı�ǩ�ᱻ���أ����xml�����õ������ı�ǩ��auto_activeΪfalseҲ��ǿ����ʾ
	virtual void show_ribbon_context_tabs(const std::vector<CString>& tab_names);

	// �����ļ�ȫ·������ʷ�б�
	virtual void add_recent_file_list(const CString& file_path_name);

	// ��ȡ��ʷ�б��е��ļ�·��
	virtual void get_recent_file_list(std::vector<CString>& file_list);

	// ͣ�������Ƿ��Ѿ����������
	virtual bool docking_panes_be_created();

	// ͨ��PostMessage����Ϣ����ִ���������水��ִ��ָ������
	// �ӷ��������߳��е��ô˺���ʱ��ȷ�������ڶ��Ϸ���ģ�������ɺ��Զ�ɾ������Ĳ���
	// ����Ͷ�ݵ���������Ϣ�����з���true�����򷵻�false
	virtual bool do_function_by_post_message(functor_by_message* fun);

	// ͨ��SendMessage����Ϣ����ִ���������水��ִ��ָ������
	// �ӷ��������߳��е��ô˺���ʱ��ȷ�������ڶ��Ϸ���ģ�������ɺ��Զ�ɾ������Ĳ���
	// �˽ӿ����������ǽ������̲߳���ת���������߳���ִ�У��ر������߳��е������߳��еĽӿ�
	// ���߳�����ִ�з���true�����򷵻�false
	virtual bool do_function_by_send_message(functor_by_message* fun);

	// ��ȡ��ǰ�������Ƥ����Ϣ
	virtual theme_style_settings get_style_and_settings() const;

	// ��鵱ǰ�����Ƿ�Ϊ����ģʽ���������κ���Ϣ����Ҫ���ڲ��Գ�����Զ���ִ�У���������Ϣ��Ҫ������
	// ʱ���Ϊд�뵽��־�����ѳ���ѡ���ѡ��Ҳд����־����Ĭ��ʵ��Ϊfalse
	virtual bool is_silent_model() const;

	// �Ƿ������л����������(�˽ӿھ߱����̰߳�ȫ)
	virtual bool is_style_changing() const;

	// ���ذ���ģʽ��������������ʱ���������Զ�ȡֵ��keyΪ�������ڴ���λ�ñ�ʶ, valueΪĬ��ȡֵ
	// ֵ��������Ҫ���úͶ�ȡ�ĵط���ʽ����һ�£��������ݵĽ��ͻ����л���һ�������˽������ݸ���ʱ��json��
	// ���û��ָ�������������Ϊ��ȷ�����������ʵ�֣�
	virtual void get_silent_model_config(std::map<CString, CString>& configs);

public:
	// ͨ��postmessage��ʽ��ӡ��رչ���������
	void post_add_working_space_view_by_class(const CString window_class, const CString node_tag, int index = -1, int image_index = -1);
	void post_add_working_space_view_by_wnd(window_interface* wnd_ix, const CString node_tag, int index = -1, int image_index = -1);
	void post_close_working_space_view_by_class(const CString& class_name, bool force_close = false);
	void post_close_working_space_view_by_tag(const CString& tag, bool force_close = false); // tag��ͬ��ȫ���ر�(һ�������ò�ͬ���͵Ĵ�����ʾ)
	void post_close_working_space_view_by_index(int index, bool force_close = false);
	void post_close_all_workspace_window(bool force_close = false);
};

X_FRAMEWORK_API app_ui_interface* get_app_ui_interface();
X_FRAMEWORK_API void set_app_ui_interface(app_ui_interface* app);
#endif //__HMGLOBALTOOL_INTERFACE_APP_UI_INTERFACE_H__
