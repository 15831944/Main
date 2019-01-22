#pragma once

#include "x3d_definition.h"
#include "x3d_data_base.h"


//////////////////////////////////////////////////////////////////////////
typedef x3d_vector x3d_line_loop;
//////////////////////////////////////////////////////////////////////////

class x3d_line;

// �ɶ�����Ԫ�����ɵĲ���ϵĶ����
class X_FRAMEWORK_API x3d_polygon : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_polygon)

public:
	x3d_polygon();
	virtual ~x3d_polygon();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	// �Ƿ�Ϊ�գ��������С��2��Ϊ��
	bool is_empty() const;

	// ����ߵ��ܳ�
	double get_length() const;

	// ��ָ�����ȴ��ضϣ�����ʧ��ʱʲôҲ����
	// reserve_frontΪtrue��ʾɾ�����沿�֣�����ǰ�沿�֣�Ϊfalse��ʾɾ��ǰ�沿�֣��������沿��
	void trim_at_pos(double length, bool reserve_front = true);

	// �ӳ�ָ���ĳ��ȣ�����ʧ��ʱʲôҲ����
	// ext_frontΪtrue��ʾ�ӳ�ͷ����Ϊfalse��ʾ�ӳ�β��
	// �ӳ�ʱ����˲��㴦�ĵ��ߣ����ص�����ֱ������
	void extend_length(double ext_len, bool ext_front = false);

	// ������е�
	void clear_points();

	// �õ�����ֵΪindex�ĵ������(��0��ʼ)
	x3d_point get_point(size_t index) const;

	// ���һ���µ㣬to_frontΪtrue��ʾ��ͷ�����
	void add_point(x3d_point new_point, bool to_front = false);

	// ����µ㼯��ext_frontΪtrue��ʾ��ͷ�����
	void add_points(std::vector<x3d_point>& points, bool ext_front = false);

	// ��ָ�����ȴ������
	void insert_point(double length);

	// ���ֱ�߶Σ�ֱ��������vec�ķ�������length�ĳ��ȣ�����ʧ��ʱʲôҲ����
	void add_line(x3d_vector vec, double length, bool to_front = false);

	// �������һ��polygon
	void connect_polygon(x3d_polygon& another, bool to_front = false);	

	// ��ָ������Ԫ�󽻵㣬�����ɹ�����true�����򷵻�false
	// ��ָ���ӳ���ʽʱ�������˵㴦�ĵ��߷����ӳ�
	// ������ƽ����ͶӰ���󽻣���װ�㡢ֱ�ߡ�������������ݽṹ���������Ϊ�����ʾֱ������ά�ռ��еĽ�
	// �㣬��������ж������ֻ�᷵���ҵ��ĵ�һ����
	// precisionΪ�����жϾ��ȣ�����������ϵľ���С�ڵ��ڴ�ֵ����Ϊ���ཻ��һ�㣩
	bool get_all_intersection(std::vector<std::pair<x3d_point, x3d_intersection_type>>& points,
		x3d_line& l, x3d_extend_type et = x3d_extend_type::none, double precision = 1e-6);

	// ����ֱ�ߵ���һ�����㣬�������ڷ���false
	bool get_first_intersection(x3d_point& p, x3d_line& l, x3d_extend_type et = x3d_extend_type::none, double precision = 1e-6);

	// ����ֱ�����һ�����㣬�������ڷ���false
	bool get_last_intersection(x3d_point& p, x3d_line& l, x3d_extend_type et = x3d_extend_type::none, double precision = 1e-6);

	// ����ֱ�������һ�����㣬�������ڷ���false
	// �����߶��н��㣬��õ���һ������
	bool get_nearest_intersection(x3d_point& p, x3d_line& l, x3d_extend_type et = x3d_extend_type::none, double precision = 1e-6);

	// �ж�һ�����Ƿ�������
	// ������������ƽ����ͶӰ���󽻣���װ�㡢ֱ�ߡ�������������ݽṹ���������Ϊ�����ʾֱ������ά�ռ��еĽ�
	// �㣬��������ж������ֻ�᷵���ҵ��ĵ�һ����
	// precisionΪ�����жϾ��ȣ�����������ϵľ���С�ڵ��ڴ�ֵ����Ϊ���ཻ��һ�㣩
	bool is_in_line(x3d_point& p, double precision = 1e-6);

	// ���������ָ�����봦�ĵ����꣬�����߻��޷����ʱ����false�����򷵻�true
	bool get_point_at_pos(x3d_point& pt, double dist);

	// �õ�����ƫ�ƵĵĶ����(ƫ�ƾ���Ϊ����ʾ�ض���߷�������ƫ�ƣ�ƫ�Ʊ�֤z���겻��;�������ⲿ�ӹ�)
	x3d_polygon* offset(double dist) const;
	x3d_polygon& self_offset(double dist);

	// ��x3d_graphת����polygon
	void transfer_from(x3d_graph& src);

	// ��polygonת��section
	void transfer_to_section(x3d_action_section& sec);

public:
	// ��Ԫ������Ҫ�������ӳ������̣�ʹ��deque�ۺ�����Ҫ����vector
	std::deque<x3d_point> m_all_points;
};
