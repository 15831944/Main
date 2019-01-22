#pragma once
#include "../interface/serialize_interface.h"

// ��������ڴ洢����֮����乳ָ����Ϣ�����ڲ�ʹ��
class X_FRAMEWORK_API frame_hook_setting
{
public:
	frame_hook_setting();
	~frame_hook_setting();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString		m_hook_name;		// �乳����
	CString		m_ref_line_name;	// �ο�����
	CString		m_is_top_hook;		// �乳�Ƿ����ڶ����㣬���ʾ���ڵײ���

	CString		m_node_index;		// ���Ƶ����(���Ƶ�����������ȫ��ͬ��X������ͬʱ������Ŵ�С�����ţ���
									// ��С����ǰ����Ŵ���ں�)�����Ƶ��ָ����m_ref_line_name�Ŀ��Ƶ㣬��
									// �Ƶ������߿�ʼ����һֱ���ο����ұ����һ��Ϊֹ����һ�����Ϊ1����
									// ��0����ע��ȷ�����Ƶ����ݽṹʶ�����һ��Զʱ��Ӧ�����

	CString		m_node_begin_layer; // �乳��Ϣ�Ƿ�Ӧ�����Ϊm_node_index�Ŀ��Ƶ������㣬���Ϊ�����ʾ������ֹ��
	CString		m_segment_name;		// �乳m_hook_name�������������߶���
};

// ��ָ���乳��Ϣ�ַ���ת��ָ���乳��Ϣ��Ķ��󣬷������������㷨��Ԥ���������
void X_FRAMEWORK_API string_to_hook_settings(const CString& setting_text, std::vector<frame_hook_setting>& setting_data);
void X_FRAMEWORK_API hook_settings_to_string(const std::vector<frame_hook_setting>& setting_data, CString& setting_text);
