#pragma once
#include "../interface/serialize_interface.h"
#include "x3d_data_base.h"


// ��ʾ��ά�ռ��е�����B��������
class X_FRAMEWORK_API x3d_bspline : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_bspline)

public:
	x3d_bspline();
	virtual ~x3d_bspline();
};

