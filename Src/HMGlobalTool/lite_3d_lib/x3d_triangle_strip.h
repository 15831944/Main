#pragma once
#include "../interface/serialize_interface.h"
#include "x3d_data_base.h"


// 表示一条三角带（每一个点与前两个点组成一个三角形，至少有三个点，点的顺序按逆时针）
// 此结构是为了快速描述模型表面最基本和重要的图形结构，并不是为了几何图形的包装，因此基类不是几何对象
class HM_GlobalTool_EXT x3d_triangle_strip : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_triangle_strip)

public:
	x3d_triangle_strip();
	virtual ~x3d_triangle_strip();
};
