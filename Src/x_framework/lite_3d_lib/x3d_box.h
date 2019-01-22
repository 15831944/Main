#pragma once
#include "../interface/serialize_interface.h"
#include "x3d_data_base.h"


// ��ʾ��ά�ռ��е�һ��������(ԭ��λ�ڳ���������)
class X_FRAMEWORK_API x3d_box : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_box)

public:
	x3d_box();
	x3d_box(double l, double w, double h);
	x3d_box(const x3d_point& p, double l, double w, double h);
	virtual ~x3d_box();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double l, double w, double h);
	void set_data(const x3d_point& p, double l, double w, double h);

public:
	double	m_l;		// ��  ����x�᷽��
	double	m_w;		// ��  ����y�᷽��
	double	m_h;		// ��  ����z�᷽��
};

