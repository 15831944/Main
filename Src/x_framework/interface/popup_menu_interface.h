#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_POPUP_MENU_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_POPUP_MENU_INTERFACE_H__	

#include <functional>
#include <vector>
#include <map>


// ���ú�ϵͳ���������ť���˵�����������ť�Ȼᴥ��WM_COMMAND��Ϣ��Ԫ�ص�id���ص�
// ע��ribbon��20000-30100�����ټ���10000-19999���μ�������ģ���stdafx.h��
#define APP_POPUP_MENU_ID_START  40000
#define APP_POPUP_MENU_ID_END    49999


class popup_menu_interface;

class X_FRAMEWORK_API popup_menu_item
{
public:
	// ����һ���˵��update_status���������������Աm_update_status
	popup_menu_item(const CString& text,
		std::function<void()> on_command = nullptr,
		std::function<void(bool&, bool&, bool&)> update_status = nullptr);

    ~popup_menu_item();

public:
	// ���һ���Ӳ˵��update_status���������������Աm_update_status
	popup_menu_item* add_sub_item(const CString& text,
		std::function<void()> on_command = nullptr,
		std::function<void(bool&, bool&, bool&)> update_status = nullptr);

	// sub_mu�����Ƕ���new�����Ķ���
	popup_menu_item* add_sub_item(popup_menu_item* sub_mu);

	void clear_items();

private:
	static unsigned make_menu_item_id();

private:
	void make_menu_implement(CMenu& parent_menu);

private:
    CString										m_text;						// ��ʾ���֣�Ϊ"-"��ʾ�ָ�����
    unsigned									m_id = -1;					// �˵�id
	std::function<void(bool&, bool&, bool&)>	m_update_status = nullptr;	// ���ò˵�״̬��ȡ������is_enabled, is_hidden, is_checked��
	std::function<void()>						m_do_command = nullptr;		// �˵���Ӧ�����������������Ӧ������δ����״̬��ȡ��������Ĭ���Զ����˵���enable״̬����Ϊtrue��
    std::vector<popup_menu_item*>				m_sub_menus;
	std::vector<CMenu*>							m_created_menus;

private:
	friend class popup_menu_interface;
};


class X_FRAMEWORK_API popup_menu_interface
{
public:
	popup_menu_interface();
	virtual ~popup_menu_interface();

public:
	// �����˵������ڴ������Ժ����
	void popup_menu(const CString& tag);

	// ���һ�������˵�����ָ����ʶ
	popup_menu_item* add_popup_menu(const CString& tag);

	// ��ON_RANGE_COMMAND(APP_POPUP_MENU_ID_START, APP_POPUP_MENU_ID_END)����Ӧ�����е��ñ������Խӹܲ˵���Ӧ
	void on_transform_popup_menu_command(unsigned id);
   
	// ��ղ˵���Դ
	void clear_all_menu_items();

protected:
	// ��������Ӳ˵����ʵ�ֺ����������е��ã�������ڹ��캯���򴴽�����������
	virtual void initialize_menus() = 0;

private:
	bool on_transform_popup_menu_command_implement(popup_menu_item *item, unsigned id);

private:
	// �洢���Ӳ˵��ĸ��˵�������ʱ����ʾ������ʾ�Ӳ˵��
	// ����Ϊֻ��ͨ���ӿ���Ӳ˵��������ֱ�Ӳ���������
	std::map<CString, popup_menu_item*> m_all_menu_roots;
};



#endif //__HMGLOBALTOOL_INTERFACE_POPUP_MENU_INTERFACE_H__
