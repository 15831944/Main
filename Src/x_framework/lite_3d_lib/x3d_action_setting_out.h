#pragma once
#include "x3d_data_base.h"


class x3d_polygon;

// ��ʾ��ά�ռ��еķ�������
class X_FRAMEWORK_API x3d_action_setting_out : public x3d_action_base
{
	DECLARE_SERIAL(x3d_action_setting_out)

public:
	x3d_action_setting_out();
	virtual ~x3d_action_setting_out();

public:	
	void do_action(x3d_graph& result);

public:
	// 
	std::vector<std::pair<size_t, x3d_action_section*>>  m_path_section;  // vector<pair<·���ϵ�������� �˵��϶�Ӧ�Ľ���>>
	std::vector<x3d_point> m_path; // ������·��
};

