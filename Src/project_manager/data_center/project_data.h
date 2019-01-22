#pragma once

#include <vector>
#include <map>
#include "x_framework/macro.h"
#include "x_framework/interface/serialize_interface.h"
#include "x_framework/interface/data_center_interface.h"


namespace tinyxml2 { class XMLElement; }


// ͼֽ����ע����Ϣ
class PROJECT_MANAGER_API graph_register_info : public CObject, public serialize_interface
{
	DECLARE_SERIAL(graph_register_info)

public:
	graph_register_info();
	graph_register_info(const graph_register_info& src);
	graph_register_info& operator=(const graph_register_info& src);
	virtual ~graph_register_info();

public:
	virtual void serialize_members(member_rw_interface& mt);

public:
	void initiate_by_xml(tinyxml2::XMLElement* node);

public:
	CString		m_type_name;						// ͼֽ�������ƣ��硰���θ�����ʽ��һ�㹹��ͼ��
	CString		m_data_class_name;					// ���������������
	CString		m_common_grid_class_name;			// �������ñ���������������
	CString		m_advance_grid_class_name;			// �����߼�����������������
	CString		m_graphic_editor_class_name;		// ͼ�λ������༭�����������
	CString		m_drawing_strategy_class_name;		// ͼֽ���Ʋ����������
};

//ͼֽ������Ϣ
class graph_data_manager : public data_node_base
{
public:
	graph_data_manager();
	virtual ~graph_data_manager();
};
//�����������Ϣ
class accessory_data_manager : public data_node_base
{
public:
	accessory_data_manager();
	virtual ~accessory_data_manager();
};


// ��Ŀ����(ֱ��ע�ᵽ�������ģ���˲���д���л��꼴����������)
class PROJECT_MANAGER_API project_data : public data_node_base
{
public:
	project_data();
	virtual ~project_data();

public:
	virtual void serialize_members(member_rw_interface& mt);
	virtual void set_default_value();

public:
	// ������Ŀ�ļ����׵���ʱ�ļ��и�·��
	CString get_project_temp_path(bool end_with_backslashed = true);

	// ������Ŀ�ļ����׵�ͼֽ�ļ��и�·��
	CString get_project_graph_path(bool end_with_backslashed = true);

	graph_data_manager& get_graph_data_manager();

	accessory_data_manager& get_accessory_data_manager();

public:
	// ��xml�����ļ��м��ع���������Ϣ
	static void initiate_graph_register_info_by_xml();
	static bool get_graph_register_info(const CString& type_name, graph_register_info& info);
	static void add_graph_register_info(const graph_register_info& info);
	static void get_graph_register_info_list(std::vector<graph_register_info*>& all_items);

private:
	// ͼֽ����ע����Ϣ�����ݻ��ڳ�������ʱ������ɣ��˴���������Ŀ�й���ѯʹ�ã������л�����Ŀ������
	static std::map<CString, graph_register_info> m_all_type_info;
};

project_data& get_project_data();
