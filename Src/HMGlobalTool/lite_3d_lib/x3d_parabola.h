#pragma once
#include "x3d_data_base.h"


// ��ʾƽ���ϵ�һ�ζ�������ߣ�ֻ����ߴ������Y=aX`n��������ԭ��λ�������߶���
class HM_GlobalTool_EXT x3d_parabola : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_parabola)

	// ��ʾ���������ڵ�ƽ�漰���ڵĳ���xy��ʾ��oxyƽ�棬��һ����ĸ��ʾ���ڳ��򣬴��»��ߵı�ʾ������
	enum direction
	{
		xy = 0,
		_xy = 1,
		yx = 2,
		_yx = 3,
		xz = 4,
		_xz = 5,
		zx = 6,
		_zx = 7,
		yz = 8,
		_yz = 9,
		zy = 10,
		_zy = 11
	};

public:
	x3d_parabola();
	x3d_parabola(double w, double h, double ll, double rl, direction dr = direction::yz, float n = 2);  // ���������߰�ߵĿ�Ⱥ͸߶ȣ��Լ������ڷ�����ߵĿ���ұߵĿ�
	x3d_parabola(const x3d_point& p, double w, double h, double ll, double rl,
		direction dr = direction::yz, float n = 2);
	virtual ~x3d_parabola();
	
public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double w, double h, double ll, double rl, direction dr = direction::yz, float n = 2);
	void set_data(const x3d_point& p, double w, double h, double ll, double rl,
		direction dr = direction::yz, float n = 2);
public:
	double           m_width;                   // �����߰�ߵĿ��
	double           m_height;                  // �����߰�ߵĸ߶�
	double           m_left_length;             // �������߿��ڷ������ߵĳ���
	double           m_right_length;            // �������߿��ڷ����Ұ�ߵĳ���
	direction        m_dir;                     // ����������ƽ�漰���ڷ���
	float            m_n;                       // �����ߵ��ݴ�(Ĭ��Ϊ2)
};
