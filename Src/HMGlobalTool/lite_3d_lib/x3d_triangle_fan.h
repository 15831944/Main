#pragma once
#include "../interface/serialize_interface.h"
#include "x3d_data_base.h"


// ��ʾһ����������ɵ����Σ���һ����Ϊ�������㣬ÿһ����ǰһ���㼰�����������һ�������Σ������������㣬���˳����ʱ�룩
// �˽ṹ��Ϊ�˿�������ģ�ͱ������������Ҫ��ͼ�νṹ��������Ϊ�˼���ͼ�εİ�װ����˻��಻�Ǽ��ζ���
class HM_GlobalTool_EXT x3d_triangle_fan : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_triangle_fan)

public:
	x3d_triangle_fan();
	virtual ~x3d_triangle_fan();
};


