#pragma once
#include "../interface/serialize_interface.h"
#include "x3d_data_base.h"


// ��ʾһ����ά�ռ��еĵ����ı�
class X_FRAMEWORK_API x3d_text : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_text)

public:
	x3d_text();
	virtual ~x3d_text();
};

