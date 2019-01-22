#pragma once
#include <vector>
#include "../macro.h"
#include "../interface/data_center_interface.h"


// �ӱ�����ö��
enum sub_steel_type
{
	ST_NONE, 		// ��Ŀ������
	ST_DESIGN,		// ���˼��
	ST_BASIC,		// ������Ϣ
	ST_LAYOUT,		// ������Ϣ
	ST_EDEG,		// �߾���Ϣ
	ST_DETAIL		// ������Ϣ
};


// �������˼����Ŀ
class X_FRAMEWORK_API steel_design_item
{
public:
	steel_design_item();
	steel_design_item(const CString& id, const CString& value); // ������ʶ, ���˼��ֵ(ע������VAX��ʾ)
	~steel_design_item();

public:
	// ������ʶ, ���˼��ֵ(ע������VAX��ʾ)
	void set(const CString& id, const CString& value);
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_id;					// ������ʶ
	bool			m_using_string_value;	// ���ֽ�����˼��CString��ʽ��ֵʱ�Ƿ�ʹ��ԭ����ֵ�����򸳵�ǰѡ�е�����������Ŀ��������Ϊ��������ʽΪtrue��
	CString			m_value;				// ���˼��ֵ(���������Ϊ��������ʽ����m_bValueByStrΪtrue��ֵΪ��0��ʼ����Ŀ��������������Ϊ���������ԭ���ַ���)
};


// ����������Ϣ��Ŀ
class X_FRAMEWORK_API steel_basic_item
{
public:
	steel_basic_item();
	// ������ʶ, ֱ��, ���ֺ�, �乳����, �˾�, ��������(ע������VAX��ʾ)
	steel_basic_item(const CString& id,
					const CString& diameter,
					const CString& steel_type, 
					const CString& hook_type, 
					const CString& distance,
					const CString& force_type);
	~steel_basic_item();

public:
	// ������ʶ, ֱ��, ���ֺ�, �乳����, �˾�, ��������(ע������VAX��ʾ)
	void set(	const CString& id,
				const CString& diameter, 
				const CString& steel_type, 
				const CString& hook_type, 
				const CString& distance,
				const CString& force_type);
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_id;			// ������ʶ
	CString			m_diameter;		// ֱ��
	CString			m_steel_type;	// ���ֺ�
	CString			m_hook_type;	// �乳���ͣ�����ֻ��Ϊ��Щֵ�е�һ��������"/", "���乳", "��׼�乳/90��", "��׼�乳/135��", "��׼�乳/180��", "�����乳/90��", "�����乳/135��", "�����乳/180��", "�����乳/90��", "�����乳/135��", "�����乳/180��"
	CString			m_distance;		// �˾�
	CString			m_force_type;	// �������ͣ�����ֻ��Ϊ��Щֵ�е����ɸ���"CA", "CB", "CC", "CD"
};


// ����������Ϣ��Ŀ
class X_FRAMEWORK_API steel_layout_item
{
public:
	steel_layout_item();
	// ������ʶ, �ӱ�ʶ, ���ü��, ��������������(ע������VAX��ʾ)
	steel_layout_item(	const CString& id,
						const CString& sub_id,
						const CString& space_distance,
						const CString& space_count);
	~steel_layout_item();

public:
	// ������ʶ, �ӱ�ʶ, ���ü��, ��������������(ע������VAX��ʾ)
	void set(const CString& id,
			const CString& sub_id,
			const CString& space_distance,
			const CString& space_count);
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_id;				// ������ʶ
	CString			m_sub_id;			// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	CString			m_space_distance;	// ���ü��
	CString			m_space_count;		// ���� ����� �� ����
};


// �����߾���Ϣ��Ŀ
class X_FRAMEWORK_API steel_edge_item
{
public:
	steel_edge_item();
	// ������ʶ, �ӱ�ʶ, λ��(��λ��), ��ϵ����(��λ��ʽ), �߾�ֵ, ȡ������, ȡ����ʽ, �����߾�(ע������VAX��ʾ)
	steel_edge_item(const CString& id,
					const CString& sub_id,
					const CString& location,
					const CString& logic_type,
					const CString& edge_distance,
					const CString& round_level,
					const CString& round_type,
					const CString& expecting_edge);
	~steel_edge_item();

public:
	// ������ʶ, �ӱ�ʶ, λ��(��λ��), ��ϵ����(��λ��ʽ), �߾�ֵ, ȡ������, ȡ����ʽ, �����߾�(ע������VAX��ʾ)
	void set(const CString& id,
			const CString& sub_id,
			const CString& location, 
			const CString& logic_type, 
			const CString& edge_distance,
			const CString& round_level, 
			const CString& round_type, 
			const CString& expecting_edge);
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_id;				// ������ʶ
	CString			m_sub_id;			// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	CString			m_location;			// λ�ã������ĵ���Ϊ����λ�㣩����ֻ����Ϊ�����ġ��͡���Ե��������ת�����ݻ����
	CString			m_logic_type;		// ��ϵ���ͣ������ĵ���Ϊ����λ��ʽ������ֻ��Ϊ����С�ڡ��������ڡ�������С��ȡ����
	CString			m_edge_distance;	// �߾�ֵ
	CString			m_round_level;		// ȡ����������ֻ��Ϊ��Щֵ�е����ɸ�����/������һ�������ַ��������ڱ�ʾȡ������
	CString			m_round_type;		// ȡ����ʽ������ֻ��Ϊ��Щֵ�е����ɸ���"/", "ƫ��", "ƫС", "��������"
	CString			m_edge_expect;		// �����߾ࣨ�����ĵ���Ϊ��������ࣩ
};


// ����������Ϣ��Ŀ
class X_FRAMEWORK_API steel_detail_item
{
public:
	steel_detail_item();
	// ������ʶ, �ӱ�ʶ, ֵ(ע������VAX��ʾ)
	steel_detail_item(const CString& id, const CString& sub_id, const CString& value);
	~steel_detail_item();

public:
	// ������ʶ, �ӱ�ʶ, ֵ(ע������VAX��ʾ)
	void set(const CString& id, const CString& sub_id, const CString& value);
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_id;					// ������ʶ
	CString			m_sub_id;				// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	bool			m_using_string_value;	// ���ֽ�Ĵ���CString��ʽ��ֵʱ�Ƿ�ʹ��ԭ����ֵ�����򸳵�ǰѡ�е�����������Ŀ��������Ϊ��������ʽΪtrue��
	CString			m_value;				// ����ֵ(���������Ϊ��������ʽ����m_bValueByStrΪtrue��ֵΪ��0��ʼ����Ŀ��������������Ϊ���������ԭ���ַ���)
};


//////////////////////////////////////////////////////////////////////////


// һ��ͨ�øֽ�������˼�롢������Ϣ��������Ϣ���߾���Ϣ��������Ϣ����ӱ�
class X_FRAMEWORK_API single_steel_data
{
public:
	single_steel_data();
	~single_steel_data();

public:
	void clear_all();	// ��յ�ǰ����ͨ�øֽ�����������
	//************************************
	// ��ֽ�������һ����Ϣ����Ϣ������ö�����г�
	// ��Ҫ�����ʶ����ֵ��û���ӱ�ʶ�򴫿գ�����ֵ���6����ÿһ�����ݴ�����������������б�ǩһ��
	void add_steel_item(sub_steel_type type,
					   const CString& id,
					   const CString& sub_id = _T(""),
					   const CString& value_01 = _T(""),
					   const CString& value_02 = _T(""),
					   const CString& value_03 = _T(""),
					   const CString& value_04 = _T(""),
					   const CString& value_05 = _T(""),
					   const CString& value_06 = _T(""));
	
	// ���ݱ�ʶ����ȡ��Ӧ����Ϣ����������²������ظ���ֻ������0������1��������û����˴�������ݣ�
	// ���³���ʶ��ͬ�������ж��У���ֻ�᷵����ǰ���һ��������ҵ��򷵻�true���򷵻�false
	CString get_component() const; // ����ʶ�õ���Ŀ��Ϣ
	bool get_design_item(IN const CString& id, OUT steel_design_item& design) const; // ����ʶ�õ���Ŀ��Ϣ
	bool get_basic_item(IN const CString& id, OUT steel_basic_item& basic_item) const; // ����ʶ�õ���Ŀ��Ϣ
	bool get_layout_item(IN const CString& id, IN const CString& sub_id, OUT steel_layout_item& layout) const; // ����ʶ�õ���Ŀ��Ϣ
	bool get_edge_item(IN const CString& id, IN const CString& sub_id, OUT steel_edge_item& edge) const; // ����ʶ�õ���Ŀ��Ϣ
	bool get_detail_item(IN const CString& id, IN const CString& sub_id, OUT steel_detail_item& detail) const; // ����ʶ�õ���Ŀ��Ϣ

	// ������Ŀֵ�����ָ������Ŀ�У���ʶ�ڱ���в����������������κβ����������ֵ���õ���Ӧ����Ŀ�У�
	// ע�����ú���ֻ�Ա��ֽ��ӱ��P0������Ч������ͬһ���ֽ�DataBase�еĲ�ͬ�ֽ��ӱ����Ե��ö�Ӧ�ĳ�
	// Ա�����ߵ������иֽ����ܵ�Set����
	void set_component(IN const CString& component);
	void set_design_item(IN const steel_design_item& design);
	void set_basic_item(IN const steel_basic_item& basic_item);
	void set_layout_item(IN const steel_layout_item& layout);
	void set_edge_item(IN const steel_edge_item& edge);
	void set_detail_item(IN const steel_detail_item& detail);

	void serialize_members(member_rw_interface& mt);

public: // Ϊ��֧��һЩ����Ĳ�������Щ���ݶ���Ϊ���еģ�����ķ���ֻ��Ϊ�˲����ı������ṩ�ġ�
	CString								m_component;		// ��ʹ�õĲ�����,��Ӧ�ò���
	std::vector<steel_design_item>		m_design_items;		// ���˼���
	std::vector<steel_basic_item>		m_basic_items;		// ������Ϣ��
	std::vector<steel_layout_item>		m_layout_items;		// ������Ϣ��
	std::vector<steel_edge_item>		m_edge_items;		// �߾���Ϣ��
	std::vector<steel_detail_item>		m_detail_items;		// ������Ϣ��
};

//////////////////////////////////////////////////////////////////////////

// ͨ�øֽ��񣨰���һϵ�еĵ���ͨ�øֽ��
class X_FRAMEWORK_API steel_data : public data_node_base
{
public:
	steel_data();
	~steel_data();

public:
	virtual void serialize_members(member_rw_interface& mt);

public:
	// ����һ����Ŀֵ�����ָ������Ŀ�У���ʶ�ڱ���в����������������κβ���������
	// ��ֵ���õ���Ӧ����Ŀ�С�ע�����ú��������еĸֽ��ӱ���Ч�����ֻ��ĳ���ֽ�
	// �ӱ��е����ݣ��������Ӧ�ӱ��set������
	void set_all_component(IN const CString& component);

	void set_all_design_item(IN const steel_design_item& design);
	void set_all_design_item(const CString& id, const CString& value);

	void set_all_basic_item(IN const steel_basic_item& basic_item);
	void set_all_basic_item(const CString& id, const CString& diameter, const CString& steel_type, const CString& hook_type, const CString& distance, const CString& force_type);
	void set_all_basic_item_diameter(const CString& id, const CString& diameter);
	void set_all_basic_item_steel_type(const CString& id, const CString& steel_type);
	void set_all_basic_item_hook_type(const CString& id, const CString& hook_type);
	void set_all_basic_item_distance(const CString& id, const CString& distance);
	void set_all_basic_item_force_type(const CString& id, const CString& force_type);
	
	void set_all_layout_item(IN const steel_layout_item& layout);
	void set_all_layout_item(const CString& id, const CString& sub_id, const CString& space_distance, const CString& space_count);
	void set_all_layout_item_distance(const CString& id, const CString& sub_id, const CString& space_distance);
	void set_all_layout_item_count(const CString& id, const CString& sub_id, const CString& space_count);

	void set_all_edge_item(IN const steel_edge_item& edge);
	void set_all_edge_item(const CString& id, const CString& sub_id, const CString& location, const CString& logic_type, const CString& edge_distance, const CString& round_level, const CString& round_type, const CString& expecting_edge);
	void set_all_edge_item_location(const CString& id, const CString& sub_id, const CString& location);
	void set_all_edge_item_logic_type(const CString& id, const CString& sub_id, const CString& logic_type);
	void set_all_edge_item_distance(const CString& id, const CString& sub_id, const CString& edge_distance);
	void set_all_edge_item_round_level(const CString& id, const CString& sub_id, const CString& round_level);
	void set_all_edge_item_round_type(const CString& id, const CString& sub_id, const CString& round_type);
	void set_all_edge_item_edge_expect(const CString& id, const CString& sub_id, const CString& expecting_edge);
	
	void set_all_detail_item(IN const steel_detail_item& detail);
	void set_all_detail_item(const CString& id, const CString& sub_id, const CString& value);	

	// �����������һ����Ŀ���ڲ����õ��Ǹ�������Ӧ��Ӻ�������������Ѿ����ڶ�Ӧ��
	// ��Ŀ���ݣ�����ӣ���������Ϊ��������������һ������ģ��δ������Ϊ���й�����
	// �뱣֤��Ҫ���Ѿ����ڶ�Ӧ��Ŀ������ʹ����Ӻ�����
	// ������������ӣ�ֻ�ܸ���
	void add_all_design_item(IN const steel_design_item& design);
	void add_all_base_item(IN const steel_basic_item& basic_item);
	void add_all_layout_item(IN const steel_layout_item& layout);
	void add_all_edge_item(IN const steel_edge_item& edge);
	void add_all_detail_item(IN const steel_detail_item& detail);

	single_steel_data& get_steel_data(size_t index);
	const single_steel_data& get_steel_data(size_t index) const;
	bool has_steel_data() const;
	size_t get_steel_count() const;
	void add_single_steel(const single_steel_data& steel);
	void clear_steel_data();
	void delete_single_steel(size_t index);
	void resize_steel_count(size_t index); // ��Ϊ��vector��resize����һ��

public: // Ϊ��֧��һЩ����Ĳ�������Щ���ݶ���Ϊ���еģ�����ķ���ֻ��Ϊ�˲����ı������ṩ�ġ�
	// ͨ�øֽ��(ÿһ��Ԫ����һ�������ĸֽ���������˻�����Ϣ�����˼������С���ӱ�)
	std::vector<single_steel_data> m_all_steel_grids;
};
