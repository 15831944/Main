#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_AUTO_LOAD_MODULE_H__
#define __HMGLOBALTOOL_TOOLS_AUTO_LOAD_MODULE_H__

// �Զ�����ģ�飬�������������ʱ���ص�ģ������˹��߼��أ���Ҫ���ڽ��������ķ�ģ��ע��ʱģ��δ�����޷���̬����������
class X_FRAMEWORK_API auto_load_module
{
private:
	auto_load_module();
	~auto_load_module();

public:
	static void load_model_by_xml();

private:
	static std::vector<CString> m_models;
};

#endif //__HMGLOBALTOOL_TOOLS_AUTO_LOAD_MODULE_H__
