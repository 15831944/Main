#pragma once
#include "x3d_data_base.h"


// ��ʾһ����ά�ռ��е�һ�����ο�(���ο���oxyƽ���ϣ�����ԭ��λ�ھ��ο�����)
class HM_GlobalTool_EXT x3d_rectangle : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_rectangle)

public:
	x3d_rectangle();
    x3d_rectangle(double l, double w);
    x3d_rectangle(const x3d_point& p,double l, double w);
	virtual ~x3d_rectangle();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
    virtual void regenerate_vertext(x3d_graph& graph) override;

public:
    void set_data(double l, double w);
    void set_data(const x3d_point& p, double l, double w);
	

public:
	double	m_l;		// ��
	double	m_w;		// ��
};

