#pragma once
#include <vector>
#include <array>
#include <map>
#include "../interface/data_center_interface.h"
#include "x3d_definition.h"


class x3d_node_base;
class x3d_node_group;
class x3d_graph;
class x3d_geometry_base;
class x3d_trans_matrix;
class x3d_vector;
class x3d_color;
class x3d_vertex;
class x3d_aabb_box;


//////////////////////////////////////////////////////////////////////////

// ��ά�����൱��һ����ά�㣬��Ҫ���¶���һ��������ݽṹ
// ����ĵ�ֻ������ά�ռ�Ķ�λʹ�ã����Ǳ�ʾһ�����Ƴ����ĵ����
typedef x3d_vector x3d_point;

//////////////////////////////////////////////////////////////////////////



// ��ʾ��ά�ռ��еı任����4*4��
// �˹�������Ϊ�任�����ڲ�һ�нӿڶ���Ϊ���ܱ���ģ�Ϳռ�任ʹ�ã�û����������������ṩ�������Ĳ����ӿڣ���
// ����Ҫ��ʹ�����������������Դ��
class X_FRAMEWORK_API x3d_trans_matrix
{
public:
	explicit x3d_trans_matrix(bool is_identity = true); // is_identityΪtrue��ʾ��ʼ��Ϊ��λ���󣬷����ʼ��ȫ0�ľ���
	x3d_trans_matrix(const x3d_trans_matrix& src);
	~x3d_trans_matrix();

public:
	void normalize(); // ��׼������ʽ������ʼ��Ϊ��λ����
	void initialize(bool is_identity = true); // is_identityΪtrue��ʾ��ʼ��Ϊ��λ���󣬷����ʼ��ȫ0�ľ���

	double get_value(int row_index, int column_index) const;
	void set_value(int row_index, int column_index, double val);
	int get_row_count() const;
	int get_column_count() const;

	void get_values(std::vector<std::vector<double>>& vals) const;
	void set_values(std::vector<std::vector<double>>& vals);
	void get_values(std::array<std::array<double, 4>, 4>& vals) const;
	void set_values(std::array<std::array<double, 4>, 4>& vals);

public:
	// ���¾���任�Զ�α任�߱�����Ч�������еĉ任��ʹ���ҳ˾���ķ�ʽ���ó˷����㣩
	void rotate(const x3d_vector& vec, double a);	                   // �ڵ�ǰ״̬����ָ������������ʱ����תָ���Ķ��������ȣ�
	void rotate(const x3d_point& p1, const x3d_point& p2, double a);   // �ڵ�ǰ״̬����ָ��ֱ�ߣ�p1,p2����ʱ����תָ���Ķ��������ȣ�
	void move(double dx, double dy, double dz);		                   // �ڵ�ǰ״̬����x,y,z�᷽���ƶ�ָ���ľ���
	void scale(double sx, double sy, double sz);	                   // �ڵ�ǰ״̬���������᷽�������ָ���ı���
	void scale(const x3d_point& p, double sx, double sy, double sz);   // ����ڲο����������᷽�������ָ���ı���
	void scale(const x3d_vector& vec, double s);	                   // �ڵ�ǰ״̬����ָ���������������ָ���ı���
	x3d_vector get_rotation() const;				                   // ���������任��ɵ���ת��
	x3d_vector get_move() const;					                   // ���������任��ɵ�λ����
	x3d_vector get_scale() const;					                   // ���������任��ɵ�������
	void no_move();									                   // ȥ��λ�Ʊ任��Ϣ

	// ��һ��������б任
	x3d_point& transform_vertex_self(x3d_point& pt) const; // ���ز������������Լ�ʹ���ߴ���
	x3d_point transform_vertex(const x3d_point& pt) const;

	// �Ե�ǰ�ı任�������ҳ�һ���任�����൱���ٵ���һ�ֱ任��ע��˴����ҳˣ����Ҫ��ˣ�����˾����˳��һ�£�
	x3d_trans_matrix& operator*(const x3d_trans_matrix& right_mx);

public:
	double		m_00, m_01, m_02, m_03;
	double		m_10, m_11, m_12, m_13;
	double		m_20, m_21, m_22, m_23;
	double		m_30, m_31, m_32, m_33;
};



// ��ʾһ����ά��������ά��
class X_FRAMEWORK_API x3d_vector
{
public:
	x3d_vector();
	x3d_vector(const x3d_vector& src);
	x3d_vector(double x, double y, double z);
	x3d_vector(const x3d_point& s, const x3d_point& e);
	~x3d_vector();

public:
	x3d_vector& operator +=(const x3d_vector& vec);
	x3d_vector& operator -=(const x3d_vector& vec);
	x3d_vector& operator*(double scale);

	double operator*(const x3d_vector& vec) const; // ���������ĵ��������һ����ֵ
	bool operator==(const x3d_vector& vec) const; // ��compare_tools��Ĭ�Ͼ��ȣ����Ҫ���ƾ��ȣ���ʹ��is_equal����

	x3d_vector operator+(const x3d_vector& vec) const;
	x3d_vector operator-(const x3d_vector& vec) const;
	x3d_vector operator-() const;
	x3d_vector operator*(double scale) const;    // ���ı䱾����������˰汾

public:
	// �ڵ�ǰ״̬����ָ������������ʱ����תָ���Ķ��������ȣ�
	x3d_vector& rotate(const x3d_vector& vec, double a);

	// ������ĳ�����ƶ�һ�ξ���
	x3d_vector& move(const x3d_vector& v, double l);

	// ����ά������ֵ
	x3d_vector& set_value(double x, double y, double z);

	// ����������õ�����
	x3d_vector& set_value(const x3d_point& s, const x3d_point& e);

	// ���ƶ����ȱȽ��Ƿ�Ϊ�غϵ�
	bool is_equal(const x3d_point& pt, double pre = 1.0e-8) const;

	// ȡģ
	double get_module() const;
	double get_module_square() const;

	// �õ���λ����
	x3d_vector get_unit_vector() const;

	// ��������Ϊ��λ���������ģΪ0����Ϊ0������
	x3d_vector& normalize();

	// ������һ�����������ı�ֵ�����������򷵻��㣩��������������ֱ�ӷ���0��
	double get_ratio_to_another_vector(const x3d_vector& v) const;

	// ������һ����ά��ľ��루��ʾ��ʱ�����壩
	double get_distance(const x3d_vector& p) const;
	double get_distance_square(const x3d_vector& p) const;

	// ����һ���������
	x3d_vector get_cross_vector(const x3d_vector& v) const;

	// ��ĳ�����ƶ�һ�ξ���ĵ�����
	x3d_vector get_point_by_vector(const x3d_vector& v, double l) const;

	// �õ�����һ�����������ƽ���ϵļн�(����)
	double get_angle(const x3d_vector& vec) const;

	// �ж�����һ�������Ƿ���
	bool is_collinear(const x3d_vector& vec) const;

	// �ж�����һ�������Ƿ�ֱ
	bool is_perpendicular(const x3d_vector& vec) const;

	// ���������������Ƿ�ȫΪ0
	bool is_all_zero() const;

public:
	double	m_x;	// ��ʾ��ʱΪx���꣬��ʾ����ʱΪx����ķ���
	double	m_y;	// ��ʾ��ʱΪy���꣬��ʾ����ʱΪy����ķ���
	double	m_z;	// ��ʾ��ʱΪz���꣬��ʾ����ʱΪz����ķ���
};



// ��ʾһ����ɫֵ����������ɫ����Ϊ0-255,����������ɫ����Ϊ0.0f~1.0f
// ԭ������˵�������ı�ʾ�������Ա�ʾ��������ɫ����������ʽֻ�ܱ������޵���ɫ����Ŀǰ��ʾ����ʹ�����ͱ�
// �����ɫ���㹻�ˣ���ʹ�ø�����ʡ�ռ���ˣ����ι���4byte������������16byte��
class X_FRAMEWORK_API x3d_color
{
public:
	x3d_color(); // �൱����ȫ0�ĺ�ɫ��alphaΪ1)
	x3d_color(const x3d_color& src);
	x3d_color(unsigned char r, unsigned char g, unsigned int b, unsigned char a = 255);
	x3d_color(int r, int g, int b, int a = 255);
	x3d_color(float r, float g, float b, float a = 1.0f);
	x3d_color(double r, double g, double b, double a = 1.0);
	x3d_color(COLORREF rgb, unsigned char a = 255);
	virtual ~x3d_color();

public:
	void set_color(unsigned char r, unsigned char g, unsigned int b, unsigned char a = 255);
	void set_color(int r, int g, int b, int a = 255);
	void set_color(float r, float g, float b, float a = 1.0f);
	void set_color(double r, double g, double b, double a = 1.0);
	void set_color(COLORREF rgb, unsigned char a = 255);
	COLORREF get_color() const;
	void get_color(float& r, float& g, float& b) const;
	void get_color(float& r, float& g, float& b, float& a) const;
	void get_color(double& r, double& g, double& b) const;
	void get_color(double& r, double& g, double& b, double& a) const;

	unsigned char ri() const;
	unsigned char gi() const;
	unsigned char bi() const;
	unsigned char ai() const;

	void ri(unsigned char r);
	void gi(unsigned char g);
	void bi(unsigned char b);
	void ai(unsigned char a);

	float rf() const;
	float gf() const;
	float bf() const;
	float af() const;

	void rf(float r);
	void gf(float g);
	void bf(float b);
	void af(float a);

	double rd() const;
	double gd() const;
	double bd() const;
	double ad() const;

	void rd(double r);
	void gd(double g);
	void bd(double b);
	void ad(double a);

public:
	unsigned char	m_r; // red
	unsigned char	m_g; // green
	unsigned char	m_b; // blue
	unsigned char	m_a; // alpha
};



// ��ʾһ����ά����
// ĿǰBIMϵͳ����Ҫ֧�ֹ��պ���ɫ���ü��ɣ����������ݲ���֧�֣���˿��Բ��ð���uv�����������Ϣ,��ʱֻ����������
class X_FRAMEWORK_API x3d_vertex
{
public:
	x3d_vertex();
	x3d_vertex(const x3d_point& pt, const x3d_vector& nm);
	x3d_vertex(const x3d_point& pt, const x3d_color& cl = x3d_color(), const x3d_vector& nm = x3d_vector(0, 0, -1));
	virtual ~x3d_vertex();

public:
	// �������꣬��ɫ��������
	void set(const x3d_point& pt, const x3d_vector& nm);
	void set(const x3d_point& pt, const x3d_color& cl = x3d_color(), const x3d_vector& nm = x3d_vector(0, 0, -1));

public:
	x3d_point		m_point;			// ������
	x3d_color		m_color;			// ��ɫ
	x3d_vector		m_normal_vector;	// �������������ǵ�λ������
};



// ��ʾ��ά�ռ��е�AABB��Χ�У�ԭ���ϴ˽ӿ�ֻ�����ж�ģ���Ƿ���ָ����������������ײ��⣬������Ҫ��ͼ�λ���
// ������������ĳЩ��������Ҫ��ʾҲ��Χ���Ա��ڵ��ԣ������Ҫ���Ƴ�ͼ�Σ����Ծ�����ͼ�λ�������
class X_FRAMEWORK_API x3d_aabb_box
{
public:
	x3d_aabb_box();
	x3d_aabb_box(const x3d_point& min_pt, const x3d_point& max_pt);
	virtual ~x3d_aabb_box();

public:
	void set_min_pt(const x3d_point& pt);
	void set_max_pt(const x3d_point& pt);
	void set_data(const x3d_point& min_pt, const x3d_point& max_pt);
	x3d_point get_min_pt() const;
	x3d_point get_max_pt() const;

	// ����ת��Ϊ��Χ��ʱ�İ뾶
	double calc_radius() const;

	// ������Χ���Ա����ɻ��ų����ض�����ķ�Χ
	void include_point(const x3d_point& pt);
	void include_box(const x3d_aabb_box& box);
	void include_graph(const x3d_graph& gp);

	// һ�����������������֣���m_min_pt��pt�Լ���pt��m_max_pt��reserve_min_partΪtrue��ʾȡǰһ����
	// ���������һ����������һ����Χ�л�ͼ����ȫ�����ڱ���Χ����ʱҲ��ʹ�ô˲���
	void exclude_point(const x3d_point& pt, bool reserve_min_part = true);
	void exclude_box(const x3d_aabb_box& box, bool reserve_min_part = true);
	void exclude_graph(const x3d_graph& gp, bool reserve_min_part = true);

	// �ж�һ�����ڲ��ڰ�Χ���ڣ��ڱ�����Ҳ���ں��ڣ�
	bool is_point_in_box(const x3d_point& pt) const;

private:
	// У�鲢�������ݣ�ȷ��һ������aabb��Χ�е�����Ҫ��
	void check_data();

private:
	x3d_point m_min_pt;
	x3d_point m_max_pt;
};



// ��ʾһ��ͼ�ε�һ����Ԫ������ͼ��x3d_graph��ɱ��һ��������3Dģ�ͣ����ӵ�ģ�ͱ�Ȼ������ͬ���棬�໥���
// ���ɣ���ͬ����ʹ�õĵ���Ʒ�ʽ��ͬ����Բ���Ķ�������ʹ�����εķ�ʽ���������Σ�����ʹ�����Ǵ������ƣ����������
// ���������ͬ�����Ϊ��ͬ��ͼԪ�ӽڵ���̫���ʣ�ͼԪ�ӽڵ��ʺϰ�����Ĳ�λ���ָ������������ʽ�յĸ���������
// �����Ⱦ�����Ϊ��ͼԪ��һ�����ֵĲ�ͬ����������ɵ��߼��ϲ����ʷ�Ϊ��ͼ�ޣ������������£��ʺϽ���ͬ��Ķ����
// Ϊ��ͬ���飬ÿ���Ϊһ����Ԫ��ͬһ��Ԫ�ĵ���Ʒ�ʽ����һ�£�һ��ͼԪ�����ж����Ԫ����Ԫ������Ӧһ��ʵ�壬���
// ���ṩ����任���ܣ�����̫С�������ʣ���ͼԪ���ṩ���ɣ�
class X_FRAMEWORK_API x3d_graph_unit
{
public:
	x3d_graph_unit(x3d_drawing_type dt = x3d_drawing_type::none);
	~x3d_graph_unit();

public:
	x3d_drawing_type m_drawing_type;
	std::vector<x3d_vertex> m_vertexs;

public:
	// ʹ��ָ����ɫ�Ե�Ԫ�ڵ����ж��������ɫ
	void set_color(x3d_color cl);
};



// ����һ��3dģ�͵�ͨ�����ݻ���
class X_FRAMEWORK_API x3d_node_base : public data_node_base
{
	DECLARE_SERIAL(x3d_node_base)

public:
	x3d_node_base();
	virtual ~x3d_node_base();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false);

public:
	void hide(bool is_hidden = false);
	bool is_hidden() const;
	bool is_graph_group() const;

private:
	// �ڵ��Ƿ�Ϊ����״̬������״̬ʱ������ͼ�δ�������Ⱦ������Ĭ��Ϊfalse
	// һ���ڵ��Ƿ���ʾ���˱���Ϊ��ʾ״̬����Ҫ���еĸ��ڵ㶼Ϊ��ʾ״̬����
	bool m_is_hidden;

	// �Ƿ�ΪͼԪ������
	// ʹ�ô˱����ܿ�ͨ���麯����RTTI����ʵ��ͼԪ�������жϣ�������޶����������ж��ٶ�(�����൱�ײ㣬ͼԪ�����ж�Ƶ�����൱�ߵ�)
	// �˱���������ͼԪ�����д������Ĭ�ϳ�ʼ��Ϊfalse
	bool m_is_graph_group;

private:
	friend class x3d_node_group;
};



// ����һ��3dģ�͵�ͼԪ���࣬��Ҫ����ͼ�����ݵĴ�������������Ĳ���ͼ��״̬�������صȣ�����һ�������ǶԴ��ͳ���
// ����ϸ�֣��Ա��Ż��������ṹ�������ȾЧ��
class X_FRAMEWORK_API x3d_node_group : public x3d_node_base
{
	DECLARE_SERIAL(x3d_node_group)

public:
	x3d_node_group();
	virtual ~x3d_node_group();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false);

public:
	// �����ͼԪ����ͼԪ��������������ӿڿ�ʹ��data_node_base����Ľӿڣ�
	// ����ɹ��᷵��ת�����ͼ�ζ�������ⲿ���к������������򷵻�nullptr
	x3d_graph* add_child_graph(x3d_geometry_base& gp);
	x3d_graph* add_child_graph(x3d_graph* gp); // ָ�����ڲ��ӹ�

	// ʹ��ָ����ɫ�����е�Ԫ�ڵĶ��������ɫ
	void set_color(x3d_color cl);

	// ��ǰͼԪ���ù���Ч�����Ա�ɫ��ʾ�൱�ڹرչ��գ�����ǰͼ�μ�������ͼ�ξ���Ӱ��
	void enable_light(bool enable = true);
	bool is_light_enabled() const;

	// ��ǰͼԪ������Ȳ��ԣ���ǰͼ�μ�������ͼ�ξ���Ӱ��
	void enable_depth_test(bool enable = true);
	bool is_depth_test_enabled() const;

	// �����Χ�У�only_selfΪtrue��ʾ��ͼԪ���ڼ��㷶Χ�ڣ�����������İ�Χ�У��޷����㷵��false�����򷵻�true
	bool calc_bound_box(x3d_aabb_box& box, bool only_self = false) const;

public:
	bool m_enable_light = true; // ���ô�ͼԪ�Ĺ���Ч��
	bool m_enable_depth_test = true; // ���ô�ͼԪ����Ȳ���
};



// ��ʾһ�������ⶥ�����ݹ��ɵ��ܱ�ͼ������ӿ�ʶ��Ŀ���ʾ��3dͼ�Σ����еĶ�������ɶ�λ��������
// ��ͼԪһ��λ�����κ���״Լ��ԭ���ϣ��κεļ���ͼԪ����תΪ��ͼԪ����
// ע�����ּ���ͼԪ������ͼԪ�Ķ���һ���ǲ����޸ĵģ����Ҫ�޸ģ�����Ҫ�޸ļ���ͼԪ����ļ��β����������ɶ�
// �㣬��ȷ�������Ƿ��ϼ���ͼԪ�ļ������ʵģ�������ͼԪ���뼸���޹أ��������κεļ��β���
// ע�⣺�ṩ�Ľӿ�������ò�ʹ�ô�3d��������û�Ҳ�ܽ��루��ʹ��occ�ȣ�
class X_FRAMEWORK_API x3d_graph : public x3d_node_base
{
	DECLARE_SERIAL(x3d_graph)

public:
	x3d_graph();
	x3d_graph(x3d_graph& src, bool copy_tag = false);
	x3d_graph(x3d_geometry_base& src); // ֧�ִӼ���ģ��תΪ����ģ��
	virtual ~x3d_graph();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false);

public:
	// ʹ��ָ����ɫ�����е�Ԫ�ڵĶ��������ɫ��only_selfΪtrue��ʾ��ͼԪ������ɫ�����޸������ͼԪ��ɫ
	void set_color(x3d_color cl, bool only_self = false);

	// ��ǰͼԪ���ù���Ч�����Ա�ɫ��ʾ�൱�ڹرչ��գ�������ǰͼ����Ч����Ӱ����ͼ�νڵ�
	void enable_light(bool enable = true);
	bool is_light_enabled() const;

	// ��ǰͼԪ������Ȳ��ԣ�����ǰͼ����Ч����Ӱ����ͼ�νڵ�
	void enable_depth_test(bool enable = true);
	bool is_depth_test_enabled() const;

	// �����Χ�У�only_selfΪtrue��ʾ��ͼԪ���ڼ��㷶Χ�ڣ�����������İ�Χ�У��޷����㷵��false�����򷵻�true
	bool calc_bound_box(x3d_aabb_box& box, bool only_self = false) const;

	// ��һ��ͼԪת��Ϊ����ͼԪ
	// һ�����ڸ����κ�ͼԪ���ɵ�ͼԪתΪ�����ͣ���������Ѿ�������ͼԪ����ת����������Ӷ������Ӷ���Ҳ��ת
	// ��Ϊ����ͼԪ������֤����νṹ����
	// ����Ķ��󱾶�������ƶ������ݣ�����������ڴ�
	void convert_from(x3d_geometry_base& gp);

	// �����ͼԪ����ͼԪ��������������ӿڿ�ʹ��data_node_base����Ľӿڣ�
	// ����ɹ��᷵��ת�����ͼ�ζ�������ⲿ���к������������򷵻�nullptr
	x3d_graph* add_child_graph(x3d_geometry_base& gp);
	x3d_graph* add_child_graph(x3d_graph* gp); // ָ�����ڲ��ӹ�

	// �任�ӿ�
	void apply_trans_matrix(const x3d_trans_matrix& mat);	// Ӧ���ⲿ����
	void rotate(const x3d_vector& vec, double a);			// ��ָ������������ʱ����תָ���Ķ��������ȣ�
	void rotate(const x3d_point& p1, const x3d_point& p2, double a); // ��ָ��ֱ�ߣ�p1,p2����ʱ����תָ���Ķ��������ȣ�
	void move(double dx, double dy, double dz);				// ��x,y,z�᷽���ƶ�ָ���ľ���
	void scale(double sx, double sy, double sz);			// �������᷽�������ָ���ı���
	void scale(const x3d_vector& vec, double s);			// ��ָ���������������ָ���ı���

public:
	//////////////////////////////////////////////////////////////////////////
	// ���ڶԵ�����ģ�͸�ʽ��֧�֣�������Ŀǰ��֧��ͨ�õ����ָ�ʽ��3ds��stl
	// �Ժ��������չ����ѭ����Ҫ��
	// 1.����ǹ���ͨ�õĸ�ʽ���Ҷ����ʽ�Ľ�������Ҫ����������⣨���п�Դ���룬��С������Ƕ����ģ�飩�����
	//   �ڱ���ӿ��м�����ӣ�������������ģ�����Լ�����Ϊ��ɢ�Ķ����������Ϊ����Ķ����Ա���ʹ�ã�����
	//   �ĸ�ʽ��osg��ģ�ͣ���ģ����Ҫ�����Ӵ��osgģ�����ʵ�ֵ��룬���˷��ڴ�ģ�飬���ⲿʹ��osg��ģ�鵼��
	//   ģ�ͺ���ȡ�������ɱ��������������ڹ��������Ǻ�С������
	// 2.����С�ڵ�3Dģ�͸�ʽ���������ڴ������ӣ�ǿ���Ƽ�������ģ�鴦�����ȡ���㲢����Ϊ����Ķ���
	//////////////////////////////////////////////////////////////////////////

	// ���롢����3dsģ��
	void import_3ds(const CString& path_name);
	void export_3ds(const CString& path_name);

	// ���롢����stlģ��
	void import_stl(const CString& path_anme);
	void export_stl(const CString& path_anme);


public:
	// �洢����ĵ�Ԫ���ϣ�firstΪ��Ԫ����secondΪ��Ԫ����
	// һ��3dģ�������ɶ�����ֻ�������ɣ���ͬ�Ĳ��ֻ��Ʒ�ʽ��ϸ�ֵ������ȸ�����ͬ��������ͼ�ε�Ԫ�����ݽṹ��
	// Ҫ��ͼ�εĶ����Ե�Ԫ����С���ȵĹ���ͼԪ���Ǵ����ȵĹ���ʽ���ӽڵ�һ����ڵ�Ԫ��ͼԪ���м䣩
	// ͼԪ��һ�����ڳ����Ĵ����ȹ����ӽڵ�һ�����ڵ���ģ�͵Ĳ�ͬ��λ������Ԫһ������ģ��ĳ��λ������Ƶ�Ԫ�Ĺ���
	std::map<CString, x3d_graph_unit> m_all_units;

	bool m_enable_light = true; // ���ô�ͼԪ�Ĺ���Ч��
	bool m_enable_depth_test = true; // ���ô�ͼԪ����Ȳ���
};


// ��ʾһ������ͼ����
// ����ͼԪ�Ķ���һ���ǲ����޸ĵģ����ҪҪ�޸ģ�����Ҫ�޸ļ���ͼԪ����ļ��β����������ɶ�
// �㣬��ȷ�������Ƿ��ϼ���ͼԪ�ļ������ʵ�
class X_FRAMEWORK_API x3d_geometry_base : public x3d_node_base
{
	DECLARE_SERIAL(x3d_geometry_base)

public:
	x3d_geometry_base();
	virtual ~x3d_geometry_base();

public:
	virtual void copy(const data_node_base& node, bool copy_tag = false);

	// �������ɶ��������Ա�֤�����ı��ȡ���Ķ���������ƥ���
	// ע�⣬�˽ӿ��ڲ�ʵ���ǲ�Ҫ���Ǿ���任�ͻ�׼��λ��ƫ�ƣ���ܻ��ڶ�������ͳһ����
	virtual void regenerate_vertext(x3d_graph& graph);

public:
	void set_span_step(double step);
	void set_span_count(double count);
	void set_base_point(const x3d_point& p);

	double get_span_step() const;
	double get_span_count() const;
	const x3d_point& get_base_point() const;

	// �����ܳ�������ʵ�ϸ�ֲ�������ϸ����Ŀ
	// ��ע����С�ڵ���0ʱ��ʾ�޷����㣬��Ҫֱ��ʹ�ã��Լ�����Ϊ0���ϸ��ʱ��ѭ��������
	double calc_span_step(double total_len) const;
	double calc_real_span_count(double total_len) const; // ���ۼ������ϸ����Ŀ��������С��
	int calc_span_count(double total_len) const; // �Լ������ϸ����Ŀ����ƫ��ȡ���󷵻�


protected:
	// ����ģ��ϸ�ֶ���ʱ�Ĳ�����ϸ�����ò�������Щģ�;�������ֲ����䶯���³ߴ��о޴�Ĳ����Ա仯����·�ߵ�����
	// ��·�ߵĲ������ٺ͸���Ԫ����Ӱ�죬���Ȼᾭ���䣩��Ϊ��֤ģ�ͳߴ���ʱ����ͬ�Ŵ�����ľ�ϸҪ���£�ϸ�ֵ���Ŀ
	// �϶�Ҫ�Ӵ����������ʹ��ϸ����Ŀ�����ж���Ĳ����Ͳ�̫���ʣ������ʺ��ò��������ã������������£�ϸ����Ŀ
	// ��ͨ�ã���ˣ��˴��ṩ����������ʹ��ʱ���˹���
	// 1.���ֵΪ����0����Ӧ��ֵ��Ч��������һ��ֵ��������ֵ��С�ڵ���0,�������ϸ�������㷨�ڲ��������ʵ�ȡֵ����
	// 2.��ֵ������0ʱ����Ҫϸ�ֵ��ܳ��ֱ�ϸ����Ŀ�Ͳ���������õ����������Ķ�����Ŀ��ȡ��Ŀ���һ����Ϊԭ��
	// 3.��ģ���ж����Ԫ��ɣ��������õĲ����Ƕ�ȫ����Ԫ��Ч����ֻ���������͵ĵ�Ԫ��Ч�����߸������͸��ӳ���Բ�ͬ
	//   ��Ԫʹ�ò�ͬ�ݶȵĲ����ʣ��ϸ��һ����Ԫ���������ò�����Ҫ�󼴿ɣ����ɣ��ɾ���ģ�����ž���
	// 4.��һЩ��������ģ�ͣ���ֱ�ߣ���ϸ�Ȳ���û������
	double m_span_step; // ϸ�ֵĳ��Ȳ��������൱����һ�����뵱ǰ��ĳ��ȼ������Ĭ��0.0
	double m_span_count; // ϸ����Ŀ,Ĭ��0.0(������С�������Ҫ������������ʹ�ÿ���е����ݹ���ȡ��)

	// ����ģ�ͽ�ģʱʹ�õĶ�λ�����꣬��λ���ڼ���ģ�͵��ĸ���λ�������������ɾ���
	// ע�⣺����ģ�͵Ķ������겻����ڴ˶�λ����Ϊ�ֲ�����ϵ�������붨λ��ʹ��ͬһ����ϵ
	x3d_point m_base_point;
};



// ��ʾ��ά�ռ��еĽ�ģ�����Ļ���
class X_FRAMEWORK_API x3d_action_base : public CObject, public serialize_interface
{
	DECLARE_SERIAL(x3d_action_base)

public:
	x3d_action_base();
	x3d_action_base(const x3d_action_base& src);
	virtual ~x3d_action_base();

public:
	x3d_action_base& operator=(const x3d_action_base& data);

public:
	virtual void do_action(x3d_graph& result);
};



// ��ʾ��ά�ռ��е�һ��ƽ��
class X_FRAMEWORK_API x3d_plane
{
public:
	x3d_plane();
	x3d_plane(const x3d_point& p1, const x3d_point& p2, const x3d_point& p3);
	virtual ~x3d_plane();

public:
	// ��ƽ��������㸳ֵ
	void set_data(const x3d_point& p1, const x3d_point& p2, const x3d_point& p3);

	// ��ƽ��ķ�������m1m2��m1m3�뷨�����������ֶ��򣬷��ؽ���Ѿ�����Ϊ��λ����
	// �������㲻����һ��ƽ�棬�򷵻�0
	x3d_vector get_normal_vector() const;

	// �õ�����ֱ���ϵĴ��㣨ͶӰ�㣩����
	x3d_point get_footpoint_on_plane(const x3d_point& p) const;

	// �ж����������Ƿ񹹳�һ��ƽ�棨�������Ƿ��ߣ�
	bool is_plane();

	// �ж�����һ�����Ƿ��ڴ�ƽ����
	bool is_in_plane(const x3d_point& p)const;


public:
	x3d_point       m_1;
	x3d_point       m_2;
	x3d_point       m_3;
};


// ��ʾ��ά�ռ�����������/����/��ת�����Ľ���ĵ㼯
// ע���㼯��˳��Ҫ��:
// 1.��������ÿ���������ĵ������γ��棻��Ҫ�γɷ��ͼ�Σ������һ���㼴Ϊ��ʼ��
// 2.����ʼ������ֹ�濴�����˳������˳ʱ�뷽��
class X_FRAMEWORK_API x3d_action_section
{
public:
	struct point
	{
		point();
		point(x3d_point& pt, bool left = true, CString k = _T(""))
			:p(pt), is_left(left), key(k){};

		x3d_point	p;
		bool		is_left;
		CString		key;
	};

public:
	x3d_action_section();
	x3d_action_section(std::vector<x3d_point>& points);
	x3d_action_section(std::vector<x3d_action_section::point>& points);
	x3d_action_section(const x3d_action_section& src);
	~x3d_action_section();

public:
	void set_data(std::vector<x3d_point>& points);
	void set_data(std::vector<x3d_action_section::point>& points);
	void add_point(const x3d_point& point, bool is_left = true, const CString& key = _T(""));
	void clear_points();


public:
	// bool��ʾ�����Ƿ�����ߣ�����㰴��ʱ����β����˳�����������һ������һ��Ϊǰ����������Ҳ�
	std::vector<x3d_action_section::point>	m_all_points;
	x3d_normal_type							m_nm_type;		// ����ķ�������ʽ
	bool                                    m_has_key;      // �Ƿ�ʹ�ùؼ��㣨��
};
