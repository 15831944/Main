#pragma once
#include "x3d_data_base.h"


// ��ʾOXYƽ���ϵ�һ��Բ�������Բ���ϵĵ�ض���ͬһƽ����(����ԭ��λ��Բ����Բ�ģ�
class HM_GlobalTool_EXT x3d_arc : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_arc)

public:
	x3d_arc();
	x3d_arc(double r, double a, double s, x3d_plane_type tp = x3d_plane_type::oxy);  // �뾶.Բ������(0-2PI).Բ�������OX/OY������н�(0-2PI)
	x3d_arc(const x3d_point& p, double r, double a, double s, x3d_plane_type tp = x3d_plane_type::oxy);
	virtual ~x3d_arc();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double r, double a, double s, x3d_plane_type tp = x3d_plane_type::oxy);
	void set_data(const x3d_point& p, double r, double a, double s, x3d_plane_type tp = x3d_plane_type::oxy);

public:
	double           m_radius;            // �뾶 
	double           m_angle;             // Բ���Ķ�����0-2PI��
	double           m_start_angle;       // Բ�������ox/oy������ĽǶȣ�0-2PI��
	x3d_plane_type   m_type;              // Բ�������ĸ�ƽ���ϣ�oxyƽ�棬oxzƽ�棬oyzƽ��
};

// �������ʾһ��Բ��
class HM_GlobalTool_EXT x3d_arc2
{
public:
	x3d_arc2();
	x3d_arc2(const x3d_point& p1, const x3d_point& p2, const x3d_point& p3);
	~x3d_arc2();

public:
	// ��Բ��
	x3d_point get_center() const;

	void set_data(const x3d_point& p1, const x3d_point& p2, const x3d_point& p3);

public:
	x3d_point m_p1;
	x3d_point m_p2;
	x3d_point m_p3;

};