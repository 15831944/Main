#pragma once
#include "x3d_data_base.h"


// ��ʾ��ά�ռ����������������׶(Բ��λ�ڵ������ԲԲ��)
class X_FRAMEWORK_API x3d_pyramid : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_pyramid)

public:
	x3d_pyramid();
    x3d_pyramid(int s, double r, double h);
    x3d_pyramid(const x3d_point& p, int s, double r, double h);
	virtual ~x3d_pyramid();

public:
    virtual void copy(const data_node_base& node, bool copy_tag = false) override;
    virtual void regenerate_vertext(x3d_graph& graph) override;

public:
    void set_data(int s, double r, double h);
    void set_data(const x3d_point& p,int s, double r, double h);
    void set_normal_type(x3d_normal_type type);

public:
    int      m_sides;             // ������α���������ԭ��λ�ڵײ�����������ģ�
    double   m_radius;            // ����������Բ�뾶  
    double   m_height;            // �߶�
    x3d_normal_type m_normal_type;//���߷�ʽ

};

