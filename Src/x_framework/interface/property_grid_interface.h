#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_PROPERTY_GRID_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_PROPERTY_GRID_INTERFACE_H__

#include <set>
#include <mutex>
#include <vector>
#include <utility>
#include "data_center_interface.h"



// ���ڽ������Ա�������ݱ䶯ʱ������֪ͨ�Ķ���ӿڣ�������Ҫ����Ӧ���Դ��������ݱ䶯�Ķ��󶼱���Ӵ˽ӿ�����
class X_FRAMEWORK_API property_notification_interface
{
public:
	property_notification_interface(bool enable_notification = true); // ���󴴽�ʱĬ�ϻ��յ����Ա�����ݱ䶯֪ͨ
	virtual ~property_notification_interface();

public:
	// ���Ա��������ݱ��޸�Ҫ����Ӧ����Ӧ���޸�ʱ���ã�����Ķ��󲻵�������ͳһ�ɿ�ܹ���
	virtual void on_property_items_changed(std::vector<CXTPPropertyGridItem*>& prop_items);
	
	// ����ʵ���������Ŀ���Ա���ʾ�����Ա���
	// ������ӵ����Ա��е�������Ŀ������new�����ģ����ڴ潻�ɿ�ܹ���
	virtual void set_property_items(CXTPPropertyGrid& prop_grid);

public:
	// ��ʾ���ӿ�ʵ������������Դ��ڣ��˴���Ϊ�����Զ���װ�ӿڣ�����ֱ��ʹ�����Ա�ӿڵ���ز����ֹ����
	void show_property_window(bool set_focus = false);

	// ������Դ������ݣ��˴���Ϊ�����Զ���װ�ӿڣ�����ֱ��ʹ�����Ա�ӿڵ���ز����ֹ����
	void clear_property_window();

	// Ҫ�����Ա���ֵ�䶯����Ϣ�����˶����Ҫ�����Ա��ڲ��ٽ���ֵ�䶯�Ķ��������˶���
	void enable_notification_to_me(bool enable);
};


//////////////////////////////////////////////////////////////////////////


// ���Դ��ڽӿڣ���������Դ��ڱ���Ӵ˽ӿ��������ܱ���������ܲ�ʵ�ֽ�����һ������ֻ֧��һ�����Ա���������
class X_FRAMEWORK_API property_window_interface
{
public:
	property_window_interface();
	virtual ~property_window_interface();

public:
	// ��ʾ���Դ��ڣ�������Դ����Ѿ���ʾ�������������
	virtual void show_window(property_notification_interface* obj, bool set_focus = false);

	// ��մ�������
	virtual void clear_content();

	// ��ǰ������ʾ���Ƿ�Ϊ������
	virtual bool is_content_empty();

public:
	// ������ע������ݱ䶯֪ͨ�������Դ��ڲ�������ע�������ڴ����
	static void regist_reciver(property_notification_interface* obj);
	static void unregist_reciver(property_notification_interface* obj);
	static bool is_registed(property_notification_interface* obj);
	static void apply_propery_items_to_registered_obj(std::vector<CXTPPropertyGridItem*>& prop_items);
	
private:
	// ���ںܶ�������������̣߳���osg����Ⱦ�̣߳��д�����ע��ģ����Ҫ���̱߳������Է�������߳���ɾ�̬
	static std::mutex m_mutex_reciver_list;
	static std::set<property_notification_interface*> m_registed_prop_reciver;
	static property_notification_interface* m_last_obj;
};


//////////////////////////////////////////////////////////////////////////


// ��ȡ�����ó�����ʵ���������Ա��ڽӿ�ָ��
X_FRAMEWORK_API property_window_interface* get_property_window();
X_FRAMEWORK_API void set_property_window(property_window_interface* obj);

#endif //__HMGLOBALTOOL_INTERFACE_PROPERTY_GRID_INTERFACE_H__
