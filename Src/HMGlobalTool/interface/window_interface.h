#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_WINDOW_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_WINDOW_INTERFACE_H__	

#include <set>
#include "app_ui_interface.h"

class HM_GlobalTool_EXT window_interface
{
public:
	window_interface();
	virtual ~window_interface();

public:
	// �������ݣ��������ݽڵ��tag,���û�д����ַ���
	virtual bool load_data_to_ui(const CString& data_tag);

	// �������ݣ��������ݽڵ��tag���������ʱ�����һ����Ҳ�����Լ���¼���ݼ���ʱ��tag��ֻ��Ϊ��������ʡ�£��Ž�tag�ٴδ���
	virtual bool save_data_from_ui();
	
	// ���ش��ڱ��⣬���ӿڻ��ڼ������ݺ���ã��Ա㴰���ܸ����������ɾ�������
	virtual CString get_title();
	
	//���ڹر�֮ǰ��Ҫ����ʱ �����ش˺���
	virtual void pre_close(const CString& node_tag);

	//���ڹر�֮ǰ��Ҫ����ô˽ӿڣ��������false���򲻻�رղ���ʾdetail_info��Ϣ���˽ӿڻ���PreClose֮ǰ������
	virtual bool can_be_close(CString& detail_info);

	// ��ǰ�����Ƿ������ݸĶ���Ҫ���棨����ʵ��Ĭ�Ϸ���false����ʾ�����棩
	virtual bool need_save_ui_data();

	// ��Ҫ�󴰿�ǿ��post��ʽˢ���ػ����Ϣʱ����
	virtual void invalidate(bool will_erase_background = false);

	// ϵͳ����ı�ʱ���ã�����Ǵ��ڴ���ʱϵͳ������ô˽ӿڣ�Ҫ�Լ�����������ӿڻ�ȡ��ǰ��ʾ�����Ⲣ�ֶ����ô˽ӿ�
	virtual void app_theme_changed(theme_style_settings& settings);
	
	// �����Լ��������ı�ǩ������ribbon.xml������Ϊ�ֹ�����ʱ�ᱻ���ã��Զ�ʱ�ɿ�ܴ���
	virtual void active_context_ribbon_tab();
	
public:
	static std::set<window_interface*> get_all_window_instances();
};
#endif //__HMGLOBALTOOL_INTERFACE_WINDOW_INTERFACE_H__
