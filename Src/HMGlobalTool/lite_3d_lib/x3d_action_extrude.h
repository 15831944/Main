#pragma once
#include "x3d_data_base.h"



// ��ʾ��ά�ռ��е��������
class HM_GlobalTool_EXT x3d_action_extrude : public x3d_action_base
{
	DECLARE_SERIAL(x3d_action_extrude)

public:
	x3d_action_extrude();
	virtual ~x3d_action_extrude();

public:
	virtual void do_action(x3d_graph& result);

	// �õ������ĵ㼯
	x3d_action_section* get_extrude_section();

public:	
	// ����㼯�����󣬵㼯����Ϊ��
	void set_data(x3d_action_section* sec, x3d_trans_matrix* mtx = nullptr);

	// ���봹ֱ�Ƚ�������ĳ���, is_reverseΪtrue��ʾ���˳�������췽����������ϵ��Ϊfalse����������ϵ
	void set_data(x3d_action_section* sec, double l, bool is_reverse = false);

	// ����㼯������ת���Ļص�����
	void set_data(x3d_action_section* sec, std::function<void(size_t, x3d_action_section::point&)> func);

	x3d_action_section* get_section() const;

	x3d_trans_matrix* get_matrix() const;

private:

	// ���ݴ���ĵ㼯ͨ���ص����������������Χ����
	void get_surround_graph_by_fuc(x3d_graph& result);
	
private:
	x3d_action_section*    m_section;               // ����ĵ㼯
	x3d_trans_matrix*      m_trans_matrix;	   	    // ������ֹͼ�εı任����	
	double                 m_length;                // ��ֱ����ľ���
	bool                   m_reverse;               // Ϊtrue��ʾ���˳�������췽����������ϵ��Ϊfalse����������ϵ
	std::function<void(size_t, x3d_action_section::point&)> m_trans_fun_callback; // ����ת���Ļص�����
};

