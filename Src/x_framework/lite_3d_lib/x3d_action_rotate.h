#pragma once
#include "x3d_data_base.h"


class x3d_line;

// ��ʾ��ά�ռ��е���ת����
class X_FRAMEWORK_API x3d_action_rotate : public x3d_action_base
{
	DECLARE_SERIAL(x3d_action_rotate)

public:
	x3d_action_rotate();
	x3d_action_rotate(x3d_action_section* sec, double ang = M_PI, double sp = M_PI_4 / 4);
	x3d_action_rotate(x3d_action_section* sec, x3d_line* ln, double ang = 2 * M_PI, int n = 16);
	virtual ~x3d_action_rotate();

public:
	virtual void do_action(x3d_graph& result);

public:
	void set_data(x3d_action_section* sec, double ang = 2 * M_PI, double sp = M_PI_4 / 4);
	void set_data(x3d_action_section* sec, x3d_line* ln, double ang = 2 * M_PI, int n = 16);

	void rotate_along_zaxis(x3d_graph& result);

public:
	x3d_action_section*        m_section;           // ����ĵ㼯
	x3d_line*                  m_line;              // ������ת
	double                     m_angle;             // ��ת�ĽǶȣ����ȣ�
	int                        m_count;             // ��ת��ϸ�ֶ���
	double                     m_step;              // ��ת�ǶȽ���������)
};

