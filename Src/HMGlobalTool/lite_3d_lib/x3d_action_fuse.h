#pragma once
#include "x3d_data_base.h"


class x3d_polygon;

// ��ʾ��ά�ռ��е��ںϲ���
class HM_GlobalTool_EXT x3d_action_fuse : public x3d_action_base
{
	DECLARE_SERIAL(x3d_action_fuse)

public:
	x3d_action_fuse();
	virtual ~x3d_action_fuse();

public:
	virtual void do_action(x3d_graph& result);

public:
	// �����ʼ����ֹ��㼯
	// ��ʼ����ֹ��㼯����û����Թؼ��֣����豣֤��һ�������һ�������Ӧ
	void set_data(x3d_action_section* beg, x3d_action_section* end);

	// �����ʼ����ֹ��㼯,�Լ��ؼ����Ӧ������켣
	void set_data(x3d_action_section* beg, x3d_action_section* end, size_t step, std::function<x3d_polygon*(size_t)> func);

private:
	x3d_action_section*        m_section_begin;               // ��ʼ��ĵ㼯
	x3d_action_section*        m_section_end;                 // ��ֹ��ĵ㼯
	size_t                     m_step;                        // �ֳɶ��ٶΣ�����polygon���Ӧ
	std::vector<x3d_polygon*>  m_all_track;                   // �����Ӧ������켣
};

