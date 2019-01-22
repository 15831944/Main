#pragma once
#include "x3d_data_base.h"


// ��ʾ��ά�ռ��е���ԲȦ  (��ԲȦλ��oxyƽ���ϣ�����ԭ��λ����Բ��Բ�ģ�
class HM_GlobalTool_EXT x3d_ellipse : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_ellipse)

public:
	x3d_ellipse();
	x3d_ellipse(double a, double b);
	x3d_ellipse(const x3d_point& p, double a, double b);
	virtual ~x3d_ellipse();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	void set_data(double a, double b);
	void set_data(const x3d_point& p, double a, double b);
	
public:
	double   m_a;                 //  ��Բ�����᳤ ��x�᷽��
	double   m_b;                 //  ��Բ�̰��᳤ ��y�᷽��
};

