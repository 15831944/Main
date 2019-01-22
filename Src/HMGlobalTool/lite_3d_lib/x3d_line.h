#pragma once
#include "x3d_data_base.h"


// ��ʾһ����ά�ռ��е�ֱ�ߣ�ֱ�߶Σ�
class HM_GlobalTool_EXT x3d_line : public x3d_geometry_base
{
	DECLARE_SERIAL(x3d_line)

	// ��֤�߶ε���㡢�е㡢�յ㲻��
	enum keeppoint
	{
		start = 0,
		mid = 1,
		end = 2
	};

public:
	x3d_line();
	x3d_line(const x3d_point& p1, const x3d_point& p2); // �����յ㹹�ɵ��߶�
	x3d_line(const x3d_point& p, const x3d_point& p1, const x3d_point& p2);
	x3d_line(double x1, double y1, double z1, double x2, double y2, double z2);
	x3d_line(const x3d_point& p, const x3d_vector& v, double len);
	virtual ~x3d_line();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false) override;
	virtual void regenerate_vertext(x3d_graph& graph) override;

public:
	// ָ����㡢�յ㹹��һ��ֱ�߶�
	void set_data(const x3d_point& p1, const x3d_point& p2);

	// ָ�����㡢��㡢�յ㣬�Ի���Ϊԭ�㣬�����յ�����ֵ����һ��ֱ�߶Σ������յ���ϻ����������ƫ�ƣ�
	void set_data(const x3d_point& b, const x3d_point& p1, const x3d_point& p2);

	// ָ����㡢�������������ȹ���һ��ֱ�߶�
	void set_data(const x3d_point& p, const x3d_vector& v, double len);

	// ����߶εĳ���
	double get_length() const;
	double get_length_square()const;
	
	// �ж��Ƿ񹹳�ֱ�߶Σ���β���Ƿ��غϣ�
	bool is_zero_length();

	// �õ���������
	x3d_vector get_vector() const;

	// �õ�ֱ�ߵķ�����(ƽ����ָ��ƽ��,��������������ȡ���ģ�,normalizeΪfalse��ʾ�������ת��Ϊ��λ������Ч�ʸ��ߣ�
	x3d_vector get_normal_vector(x3d_plane_type py = x3d_plane_type::oxy, bool normalize = false) const;
	 
	// �ж�һ�����Ƿ��ڴ�ֱ���ϣ�need_extendΪtrue��ʾ���ӳ�����Ҳ��
	bool is_on_line(const x3d_point& p, bool need_extend = false) const;

	// �ж�һ������ֱ�ߵĹ�ϵ
	// ����ֱ���ϣ�����none�������߶��ϣ����������յ㣩����self��
	// ��������������ϣ�����start
	// �����յ��ӳ����Ϸ���end
	x3d_intersection_type relation_with_point(const x3d_point& p) const;

	// �ж�����ֱ���Ƿ���
	bool is_plane(const x3d_line& l)const;

	// �ж�ֱ�߶�(���ӳ�)����һֱ�߶�(��ָ���ӳ���ʽ���Ƿ��ཻ,���ཻ���������p������λ��
	bool is_cross(const x3d_line& l, std::pair<x3d_point, x3d_intersection_type>& pt,
		x3d_extend_type et = x3d_extend_type::none, double precision = 1e-6) const;

	// ������㼰���򲻱䣬��ֱ�߶�����n��
	void self_scale(double n);
	x3d_point scale(double n) const;
	x3d_point mid_point() const;

	// ���߶α�Ϊָ���ĳ��ȣ�type����ָ���Ǳ�����㣬�е㣬�յ㲻��
	x3d_line& self_assign_length(double len, keeppoint tp = keeppoint::start);
	x3d_line assign_length(double len, keeppoint tp = keeppoint::start) const;

	// �õ�����ֱ���ϵĴ��㣨ͶӰ�㣩����
	x3d_point get_foot_point(const x3d_point& p) const;

	// �õ�����ڴ�ֱ�ߵĶԳƵ�
	x3d_point get_symmetry_point(const x3d_point& p) const;

	// �õ�����һ��ֱ�����������꣨���ڱ�ֱ���ϣ�
	x3d_point get_closest_point(const x3d_line& l) const;

	// ������һ��ֱ�߽��㣬û�н��㷵��false���ж��������ȡ����һ��������true
	// �ռ��������������д���max_dist��ʾ�����󽻵Ŀռ������루С�ڴ˾���ʱ��Ϊ�н��㣩�����
	// ֵ���ڹ����������壨�ڲ�mm��λ����������һ�㾫����1mm���ڹ��ˣ����������ۼ���Ĭ��Ϊ0.001)
	bool get_cross_point(const x3d_line& l, x3d_point& p, double max_dist = 0.001) const;

	// �õ�һ��ֱ�߹��ڴ�ֱ�ߵĶԳ�ֱ��
	x3d_line get_symmetry_line(const x3d_line& l) const;

	// �õ�����һ��ֱ�ߵļнǣ����ȣ�
	double get_angle(const x3d_line& line) const;

	// ������ƽ��ֱ�߶�
	x3d_line move(const x3d_vector& vec) const;
	x3d_line& self_move(const x3d_vector& vec);

	// �õ�ƫ���߶�(ƫ�ƾ���Ϊ����ʾ�ض���߷�������ƫ�ƣ�ƫ�Ʊ�֤z���겻��)
	// ����ָ�����ĸ�ƽ���Ͻ���ƫ�ƣ���ѡoxy��oxz��oyz��Ĭ��Ϊoxy��
	x3d_line offset(double dist, x3d_plane_type py = x3d_plane_type::oxy) const;
	x3d_line& self_offset(double dist, x3d_plane_type py = x3d_plane_type::oxy);

public:
	x3d_point       m_s;             // �߶ε�������ά����
	x3d_point       m_e;             // �߶ε��յ����ά����
};

