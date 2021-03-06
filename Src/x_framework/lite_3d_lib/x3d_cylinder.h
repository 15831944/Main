#pragma once
#include "x3d_data_base.h"


// 表示三维空间中的圆柱 (坐标原点位于体心）
class X_FRAMEWORK_API x3d_cylinder : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_cylinder)

public:
	x3d_cylinder();
	x3d_cylinder(double r, double h);
	x3d_cylinder(const x3d_point& p, double r, double h);
	virtual ~x3d_cylinder();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double r, double h);
	void set_data(const x3d_point& p, double r, double h);
	
public:
	double   m_radius;            // 半径
	double   m_height;            // 高度
};

