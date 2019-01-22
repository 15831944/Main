#pragma once
#include "x3d_data_base.h"


// ��ʾ��ά�ռ��еİ����壨����ԭ�������ģ�
class X_FRAMEWORK_API x3d_hemisphere : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_hemisphere)

public:
	x3d_hemisphere();
	x3d_hemisphere(double r);
	x3d_hemisphere(const x3d_point& p, double r);
	virtual ~x3d_hemisphere();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double r);
	void set_data(const x3d_point& p, double r);

public:
	double	m_radius;          // ��뾶 
	bool	m_if_has_bottom;   // �Ƿ��е���
};

