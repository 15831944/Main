#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_TABLE_TEMPLATES_H__
#define __HMGLOBALTOOL_TOOLS_TABLE_TEMPLATES_H__

#include <array>
#include <vector>
#include "tinyxml2.h"

class CTHDataBase;

// �����ڹ���ͼ���txml��tab��tof�ȱ��ģ�壬���ṩʵ����������
class X_FRAMEWORK_API table_templates
{
private:
	table_templates();
	~table_templates();

public:
	static CTHDataBase* create_data_base(const CString& template_name); // ָ����ʹ�õ��˸������

public:
	// ��table_dirĿ¼�¼�������ģ��ģ�壬�˽ӿ��ɿ�ܵ���,
	static void load_table_template_name_from_dir(const CString& table_dir);

	// ��һ��database���ɱ�����������ģ��������������ʱ����ݴ˱������һ���µı��ʵ�������أ���
	// ����ı������ɱ��ฺ���ڴ����,ģ���������ᵼ�±�񷵻ش�������������������ֱ�ӷ���
	// ������֤����ı�����Ƹ�������һ�����Ƽ�ʹ�á�ҵ����_������_������_������������������磺��������Ϣ_��̨_�ϲ��ṹˮƽ�������͡�
	static void add_database_sample(const CString& name, CTHDataBase* db);

	// �����ģ���Լ��û���������ڱ�񴴽����������ʵ��������ָ���������򷵻�true�����򷵻�false;
	static bool has_template_name(const CString& name);
};

// xml��ʽ�ı��ģ�嵥Ԫ��������Ϣ
// ʹ�øú�����xml�ļ������ɱ��ģ��
// ���ģ��ʾ����
// 
// 	< ? xml version = "1.0" encoding = "utf-8" ? >
// 	<table_template_design>
// 	
// 	<table table_type = "line_table">
// 	<tr>
// 	<td width = '100' colspan = '8'  >< / td>
// 		...
// 	< / tr>
// 	  ...
// 	  ...
// 	< / table>
// 
// 	< / table_template_design>
// 
// 	���ģ�����ʹ��˵��:
// 	template_name	������ƣ��������ʱ���ɲ�д�����ļ�������Ϊ������ƣ�		
// 	table_type		������ͣ��б�line_table; ���Ա�attribute_table; ͨ�ñ�common_table��
// 	colspan			��������кϲ�����
// 	rowspan			������µ�Ԫ��ϲ�����
// 	read_only		Ĭ��Ϊֻ�������ֵΪ��д
// 	width			��Ԫ����
// 	cell_tag		��Ԫ���ǩ
// 	row_tag			�б�ǩ
// 	col_tag			�б�ǩ
class X_FRAMEWORK_API table_cell_config
{
public:
	table_cell_config();
	~table_cell_config();

public:
	CString		m_colspan;			// ��Ԫ�����Һϲ�����
	CString     m_rowspan;			// ��Ԫ�����ºϲ�����
	CString		m_valign;			// ���뷽ʽ
	CString		m_text;				// ��Ԫ������
	CString		m_label_text;		// ��������
	CString		m_type;				// ��Ԫ������
	CString     m_read;				// ֻ������
	CString		m_cell_tag;			// ��ǩ��
};

// xml��ʽ�ı��ģ����������Ϣ
class X_FRAMEWORK_API table_row_config
{
public:
	table_row_config();
	~table_row_config();

public:
	int								m_row_height;		//�и�
	CString							m_row_tag;			//�б�ǩ
	std::vector<table_cell_config>	m_all_cell_configs;	//ÿһ����Ϣ

private:
	friend class table_xml_template;
	friend class single_table_config;
};

//xml ����������Ϣ
class X_FRAMEWORK_API single_table_config
{
public:
	single_table_config();
	~single_table_config();

private:
	CString m_table_type;
	std::vector<table_row_config> m_all_row_configs;	//������Ϣ

private:
	friend class table_xml_template;
	friend class single_template_config;
};

//��ģ����
class X_FRAMEWORK_API single_template_config
{
public:
	single_template_config();
	~single_template_config();

public:
	CTHDataBase* generate_grid() const;
	void initialize_by_xml(tinyxml2::XMLElement* node);

public:
	void set_row_count(int count);
	int get_row_count() const;
	void set_table_size(int row_count, int column_count); // �С������������0
	void set_column_count(int count);
	int get_column_count() const;
	table_cell_config& get_cell_config(int row_index, int col_index);

public:
	//���ӿ���Ҫ���ڲ���������Ĺ���
	void get_rows_height(std::vector<std::pair<int, int> >& row_height);			// pair<����, �߶�>
	void get_cols_width(std::vector<std::pair<int, int> >& col_width);				// pair<����, ���>
	void get_merge_data(std::vector<std::array<int, 4>>& data) const;				// �õ��ϲ���Ϣ����,���кţ�������������

public:
	CString m_template_name;

private:
	void get_xml_table_node(tinyxml2::XMLElement* node);
	void get_xml_tr_node(tinyxml2::XMLElement* node, single_table_config& data);
	void set_cell_property(CTHDataBase* db) const;
	void set_group_property(CTHDataBase* db)const;
	void get_merge_info();
	void resize_verify_txml_data();
	void get_group_info();
	void set_title_info(CTHDataBase* db)const;
	bool verify_merge_info() const;

private:
	// ģ������б�ʵ������ı��С�Ķ࣬����Ҫʹ�������Ż������ݽṹ
	std::vector<single_table_config>	m_all_table_configs;	// �ܵı��ģ����Ϣ
	std::vector<std::array<int, 4>>		m_merge_info;			// �ϲ���Ϣ(��Ԫ������������������),δ�������С����е�����
	std::vector<CString>				m_width;				// ����п�
	std::vector<CString>				m_col_tag;				// ����б�ǩ
	std::vector<table_row_config>		m_self_row_info;		// �������Ϣ
	std::vector<std::array<CString, 3>> m_group;				// ���������Ϣ, array<��ʼ�У���ֹ�У���������>>
	int m_column_count;

private:
	friend class table_xml_template;
};

// xml��ʽ�ı��ģ��
class X_FRAMEWORK_API table_xml_template
{
public:
	table_xml_template();
	~table_xml_template();

public:
	void initialize_by_xml(CString& path_name);
	bool is_empty();
public:
	std::vector<single_template_config> m_all_table;
};
#endif //__HMGLOBALTOOL_TOOLS_TABLE_TEMPLATES_H__
