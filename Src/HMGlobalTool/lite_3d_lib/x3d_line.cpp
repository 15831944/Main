#include "stdafx.h"

#include "../tools/compare_tools.h"
#include "x3d_line.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


IMPLEMENT_SERIAL(x3d_line, x3d_geometry_base, 1)

x3d_line::x3d_line()
{
	m_s = x3d_point(0, 0, 0);
	m_e = x3d_point(0, 0, 0);
}

x3d_line::x3d_line(const x3d_point& p1, const x3d_point& p2)
{
	set_data(p1, p2);
}

x3d_line::x3d_line(double x1, double y1, double z1, double x2, double y2, double z2)
{
	set_data(x3d_point(x1, y1, z1), x3d_point(x2, y2, z2));
}

x3d_line::x3d_line(const x3d_point& p, const x3d_point& p1, const x3d_point& p2)
{
	set_data(p, p1, p2);
}

x3d_line::x3d_line(const x3d_point& p, const x3d_vector& v, double len)
{
	set_data(p, v, len);
}

x3d_line::~x3d_line()
{

}

void x3d_line::copy(const data_node_base& node, bool copy_tag /*= false*/)
{
	x3d_geometry_base::copy(node, copy_tag);
	const x3d_line* src_obj = dynamic_cast<const x3d_line*>(&node);
	if (!src_obj) return;

	m_s = src_obj->m_s;
	m_e = src_obj->m_e;
}

void x3d_line::regenerate_vertext(x3d_graph& graph)
{
	graph.m_all_units.clear();

	auto it = graph.m_all_units.insert(make_pair(_T("lines"), x3d_graph_unit()));
	(*it.first).second.m_drawing_type = x3d_drawing_type::lines;
	vector<x3d_vertex>& unit = (*it.first).second.m_vertexs;

	x3d_vector nm(0, 0, 1);
	unit.push_back(x3d_vertex(m_s + m_base_point, nm));
	unit.push_back(x3d_vertex(m_e + m_base_point, nm));
}

void x3d_line::set_data(const x3d_point& p1, const x3d_point& p2)
{
	m_base_point = x3d_point(0, 0, 0);
	m_s = p1;
	m_e = p2;
}

void x3d_line::set_data(const x3d_point& p, const x3d_point& p1, const x3d_point& p2)
{
	m_base_point = p;
	m_s = p1;
	m_e = p2;
}

void x3d_line::set_data(const x3d_point& p, const x3d_vector& v, double len)
{
	if (compare_tools::is_less_equal(len, 0.0)) len = 1.0;

	m_s = p;
	m_e = p.get_point_by_vector(v, len);;
}

double x3d_line::get_length() const
{
	return m_s.get_distance(m_e);
}

double x3d_line::get_length_square() const
{
	return m_s.get_distance_square(m_e);
}

x3d_vector x3d_line::get_vector() const
{
	return m_e - m_s;
}


x3d_vector x3d_line::get_normal_vector(x3d_plane_type py /*= x3d_plane_type::oxy*/, bool normalize/* = false*/) const
{
	x3d_vector nm;
	if (x3d_plane_type::oxy == py)
	{
		double dx = m_e.m_x - m_s.m_x;
		double dy = m_e.m_y - m_s.m_y;
		nm.set_value(-dy, dx, 0.0);
	}
	else if (x3d_plane_type::oxz == py)
	{
		double dz = m_e.m_z - m_s.m_z;
		double dx = m_e.m_x - m_s.m_x;
		nm.set_value(-dz, 0, dx);
	}
	else if (x3d_plane_type::oyz == py)
	{
		double dz = m_e.m_z - m_s.m_z;
		double dy = m_e.m_y - m_s.m_y;
		nm.set_value(0, -dz, dy);
	}
	else
	{
		// nothing
	}

	if (normalize) nm.normalize();

	return nm;
}

bool x3d_line::is_zero_length()
{
	return !(m_s == m_e);
}

bool x3d_line::is_on_line(const x3d_point& p, bool need_extend/* = false*/) const
{
	if (need_extend)
		return x3d_intersection_type::none != relation_with_point(p);
	else
		return x3d_intersection_type::self == relation_with_point(p);
}

x3d_intersection_type x3d_line::relation_with_point(const x3d_point& p) const
{
	if (p.is_equal(m_e, 1e-6) || p.is_equal(m_s, 1e-6)) return x3d_intersection_type::self; // ��Ϊֱ�ߵ������յ㣬����self
	
	x3d_vector v1(m_s, p), v2(m_e, p);
	double n = v1.get_ratio_to_another_vector(v2); // �õ�v1��v2�ı�ֵ

	if (compare_tools::is_equal(n, 0.0)) return x3d_intersection_type::none;  // v1��v2�����ߣ�����none
	if (compare_tools::is_less(n, 0.0)) return x3d_intersection_type::self; // ����ֱ���ϣ�����self
	if (n < 1.0) return x3d_intersection_type::start; // ��������������ϣ�����start
		return x3d_intersection_type::end; // �����յ��ӳ����Ϸ���end��n>1)
}

bool x3d_line::is_plane(const x3d_line& l) const
{
	return x3d_plane(m_s, m_e, l.m_s).is_in_plane(l.m_e);
}



bool x3d_line::is_cross(const x3d_line& l, std::pair<x3d_point, x3d_intersection_type>& pt,
	x3d_extend_type et /*= x3d_extend_type::none*/, double precision /*= 1e-6*/) const
{
	if (!get_cross_point(l, pt.first, precision)) return false;           // �����ֱ���޽��㣬����false
	if (relation_with_point(pt.first) != x3d_intersection_type::self) return false; // ������㲻�ڱ���ֱ�߶��ϣ�����false
	pt.second = l.relation_with_point(pt.first);                                   // ��¼��������
	if (et == x3d_extend_type::first && pt.second == x3d_intersection_type::end) return false; // ����߶�Ϊ����ӳ����������յ��ӳ����ϣ�����false
	if (et == x3d_extend_type::second && pt.second == x3d_intersection_type::start) return false;// ����߶�Ϊ�յ��ӳ�������������ӳ����ϣ�����false
	if (et == x3d_extend_type::none && pt.second != x3d_intersection_type::self) return false;   // ����߶β��ӳ������㲻���߶��ϣ�����false
	return true;
}

void x3d_line::self_scale(double n)
{
	m_e -= get_vector() * (n - 1.0);
}
 
x3d_point x3d_line::scale(double n) const
{
	return m_e - get_vector() * (1.0 - n);
}

x3d_point x3d_line::mid_point() const
{
	return scale(0.5);
}

x3d_line& x3d_line::self_assign_length(double len, keeppoint tp /*= keeppoint::start*/)
{
	if (compare_tools::is_less(len, 0.0)) return *this;    // �����볤��С��0���򲻸ı�

	if (keeppoint::start == tp)		
	{
		m_e = m_s.get_point_by_vector(get_vector(), len);
	}
	else if (keeppoint::mid == tp)
	{
		m_e = m_s.get_point_by_vector(get_vector(), len / 2);
		m_s = m_e.get_point_by_vector(-get_vector(), len / 2);
	}
	else if (keeppoint::end == tp)
	{
		m_s = m_e.get_point_by_vector(-get_vector(), len);
	}
	else
	{
		// nothing
	}
	
	return *this;
}

x3d_line x3d_line::assign_length(double len, keeppoint tp /*= keeppoint::start*/) const
{
	x3d_line tmp(*this);
	return tmp.self_assign_length(len, tp);
}

x3d_point x3d_line::get_foot_point(const x3d_point& p) const
{
	double up = (p.m_x*(m_e.m_x - m_s.m_x) + p.m_y*(m_e.m_y - m_s.m_y) + p.m_z*(m_e.m_z - m_s.m_z)
		+ m_s.m_x*m_s.m_x + m_s.m_y*m_s.m_y + m_s.m_z*m_s.m_z
		- m_e.m_x*m_s.m_x - m_e.m_y*m_s.m_y - m_e.m_z*m_s.m_z);
	double down = (m_e.m_x - m_s.m_x)*(m_e.m_x - m_s.m_x) + (m_e.m_y - m_s.m_y)*(m_e.m_y - m_s.m_y)
		+ (m_e.m_z - m_s.m_z)*(m_e.m_z - m_s.m_z);

	if (compare_tools::is_equal(down, 0.0)) return m_s; // ��������յ��غϣ��򷵻����յ�

	double n = up / down;
	return scale(n);

    /*��һ�ַ���*/
    //double dx = m_s.m_x - m_e.m_x;
    //double dy = m_s.m_y - m_e.m_y;
    //double dz = m_s.m_z - m_e.m_z;
    //double dx_ps = p.m_x - m_s.m_x;
    //double dy_ps = p.m_y - m_s.m_y;
    //double dz_ps = p.m_z - m_s.m_z;
    //if ((dx_ps / dx) == (dy_ps / dy) && (dx_ps / dx) == (dz_ps / dz))//����ֱ���ϣ�ֱ�ӷ��ر���
    //    return p;
    //double u = dx_ps*dx + dy_ps*dy + dz_ps*dz;
    //u = u / ((dx*dx) + (dy*dy) + (dz*dz));
    //double p_x = m_s.m_x + u*dx;
    //double p_y = m_s.m_y + u*dy;
    //double p_z = m_s.m_z + u*dz;
    //return x3d_point(p_x, p_y, p_z);
}

x3d_point x3d_line::get_symmetry_point(const x3d_point& p) const
{
	x3d_point fp = get_foot_point(p);
	x3d_line(p, fp).self_scale(2);
	return fp;
}


x3d_point x3d_line::get_closest_point(const x3d_line& l) const
{
	x3d_point p;
	get_cross_point(l, p);
	return p;
}

bool x3d_line::get_cross_point(const x3d_line& l, x3d_point& p, double max_dist/* = 0.001*/) const
{
	if (l.is_on_line(m_s)) { p = m_s; return true; }
	if (l.is_on_line(m_e)) { p = m_e; return true; }
	if (is_on_line(l.m_s)) { p = l.m_s; return true; }
	if (is_on_line(l.m_e)) { p = l.m_e; return true; }

	x3d_vector vec_ln = l.get_vector();
	if (vec_ln.is_all_zero()) { p = m_e; return false; }      // ���ֱ��l���յ��غϣ��򷵻�this���յ�
	x3d_vector vec_this = get_vector();
	if (vec_this.is_all_zero()){ p = l.m_e; return false; }   // ���ֱ��this���յ��غϣ��򷵻�l���յ�

	// �������ߵ�����������������������ӣ���ǰ���Ѿ��ж�����غϣ��˴�������Ϊ������
	x3d_vector vec_connection(l.m_s.m_x - m_s.m_x, l.m_s.m_y - m_s.m_y, l.m_s.m_z - m_s.m_z);

	x3d_vector v1 = vec_connection.get_cross_vector(vec_ln);
	x3d_vector v2 = vec_connection.get_cross_vector(vec_this);
	x3d_vector v3 = vec_this.get_cross_vector(vec_ln);

	double dd = v3.get_module_square();
	if (compare_tools::is_equal(dd, 0.0)) // �����ֱ���غϣ��򷵻ص�һ�����յ㼰�ڶ����������е�����
	{
		p = x3d_line(m_e, l.m_s).mid_point();
		return true;
	}

	double t1 = v1 * v3 / dd;
	double t2 = v2 * v3 / dd;

	//�õ������λ��
	p.m_x = (m_s.m_x + (m_e.m_x - m_s.m_x) * t1);
	p.m_y = (m_s.m_y + (m_e.m_y - m_s.m_y) * t1);
	p.m_z = (m_s.m_z + (m_e.m_z - m_s.m_z) * t1);

	x3d_point pl; // ֱ��l������������
	pl.m_x = (l.m_s.m_x + (l.m_e.m_x - l.m_s.m_x) * t2);
	pl.m_y = (l.m_s.m_y + (l.m_e.m_y - l.m_s.m_y) * t2);
	pl.m_z = (l.m_s.m_z + (l.m_e.m_z - l.m_s.m_z) * t2);
	
	double mx_dist_square = max_dist * max_dist;
	return p.get_distance_square(pl) < mx_dist_square;  // ����С�ڸ����������ཻ������true
}

x3d_line x3d_line::get_symmetry_line(const x3d_line& l) const
{
	x3d_point start = get_symmetry_point(l.m_s);
	x3d_point end = get_symmetry_point(l.m_e);
	return x3d_line(start, end);
}

double x3d_line::get_angle(const x3d_line& line) const
{
	return get_vector().get_angle(line.get_vector());
}

x3d_line x3d_line::move(const x3d_vector& vec) const
{
	x3d_line tmp = *this;
	tmp.self_move(vec);
	return tmp;
}

x3d_line& x3d_line::self_move(const x3d_vector& vec)
{
	m_s += vec;
	m_e += vec;
	return *this;
}

x3d_line x3d_line::offset(double dist, x3d_plane_type py /*= x3d_plane_type::oxy*/) const
{
	x3d_line tmp = *this;
	tmp.self_offset(dist, py);
	return tmp;
}

x3d_line& x3d_line::self_offset(double dist, x3d_plane_type py /*= x3d_plane_type::oxy*/)
{
	x3d_vector mv = get_normal_vector(py, true);

	self_move(mv * dist);
	return *this;
}

