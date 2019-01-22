#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_GLOBAL_PARAMETERS_H__
#define __HMGLOBALTOOL_TOOLS_GLOBAL_PARAMETERS_H__

#include <functional>

// ȫ�ֲ�����
class HM_GlobalTool_EXT global_parameters
{
private:
	global_parameters() = delete;
	~global_parameters() = delete;

public:
	// name������Ϊ��������,��ֹ�ⲿ������ַ����������ڱ��̶߳����������µı���
	static void set_parameter(const CString name, const CString& value);	// ���̰߳�ȫ�ģ���ȷ������Ĳ����ڶ��߳����ǿɰ�ȫ���ʵ�
	static CString get_parameter(const CString name);						// ���̰߳�ȫ�ģ���ȷ������Ĳ����ڶ��߳����ǿɰ�ȫ���ʵ�
	static void get_parameter(const CString name, CString& value);			// ע��˽ӿڴ�������ڷ��ؽ����value�����������߳̿�ͬʱ���ʣ����ڲ���������ʱ���ܵ��±�������ע�⴫����̷߳��ʰ�ȫ�Ķ���
	static bool parameter_exist(const CString name);						// ���̰߳�ȫ�ģ���ȷ������Ĳ����ڶ��߳����ǿɰ�ȫ���ʵ�

	// ��������߳���ʹ�ô�ȫ�ֲ����࣬����ʹ��id��ʽ���ַ�����ʽ�漰���ַ�������Ĺ��죬�ڴ��������ص��ڴ��
	// ������֤�ⲿ����Ĳ������̰߳�ȫ�Ǻ��ѵģ����������׺��Ӳ����������ݽ�����ҲҪ���̱߳�������ɱ����͵�Ч��ʵ��
	// valueΪ-1��ʾid�����ڻ��޷������ֵ
	static void set_parameter(int id, int value);							// ���̰߳�ȫ�ģ���ȷ������Ĳ����ڶ��߳����ǿɰ�ȫ���ʵ�
	static void get_parameter(int id, int& value);							// ע��˽ӿڴ�������ڷ��ؽ����value�����������߳̿�ͬʱ���ʣ����ڲ���������ʱ���ܵ��±�������ע�⴫����̷߳��ʰ�ȫ�Ķ���
	static int get_parameter(int id);										// ע��˽ӿڴ�������ڷ��ؽ����value�����������߳̿�ͬʱ���ʣ����ڲ���������ʱ���ܵ��±�������ע�⴫����̷߳��ʰ�ȫ�Ķ���
	static bool parameter_exist(int id);									// ���̰߳�ȫ�ģ���ȷ������Ĳ����ڶ��߳����ǿɰ�ȫ���ʵ�
};

// ��������Χ���Զ����úͻָ�ȫ�ֲ���
class HM_GlobalTool_EXT auto_set_global_parameters
{
public:
	auto_set_global_parameters(const CString& name, const CString& value_begin, const CString& value_end);
	auto_set_global_parameters(int id, int value_begin, int value_end); // idС��1000����ϵͳ����ռ�ã��û������id��ȷ�����ڵ�����1000
	auto_set_global_parameters(std::function<void()> set_fun, std::function<void()> undo_fun);
	~auto_set_global_parameters();
	auto_set_global_parameters(const auto_set_global_parameters&) = delete;
	auto_set_global_parameters(auto_set_global_parameters&&) = delete;

private:
	int		m_type;

	CString m_name;
	CString m_value_begin_string;
	CString m_value_end_string;
	std::function<void()> m_undo_fun;

	int		m_id;
	int		m_int_value_begin;
	int		m_int_value_end;
};
#endif //__HMGLOBALTOOL_TOOLS_GLOBAL_PARAMETERS_H__
