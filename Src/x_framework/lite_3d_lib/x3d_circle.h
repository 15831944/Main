#pragma once
#include "x3d_data_base.h"


// ��ʾ��ά�ռ��е�ԲȦ (Բ��oxyƽ���ϣ�����ԭ��λ��Բ��Բ�ģ�
class X_FRAMEWORK_API x3d_circle : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_circle)

public:
	x3d_circle();
	x3d_circle(double r, x3d_plane_type tp = x3d_plane_type::oxy);
	x3d_circle(x3d_point& p, double r, x3d_plane_type tp = x3d_plane_type::oxy);
	virtual ~x3d_circle();


public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double r, x3d_plane_type tp = x3d_plane_type::oxy);
	void set_data(x3d_point& p, double r, x3d_plane_type tp = x3d_plane_type::oxy);

public:
	double m_radius;              // ����ԭ����Բ�ģ�z����Ϊ0
	x3d_plane_type   m_type;              // Բ�����ĸ�ƽ���ϣ�oxyƽ�棬oxzƽ�棬oyzƽ��
};

