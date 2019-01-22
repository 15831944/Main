#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_XML_TOOLS_H__	
#define __HMGLOBALTOOL_TOOLS_XML_TOOLS_H__	

#include "tinyxml2.h"

// ��ȡָ����xml�ڵ�����ԣ������ȡʧ�ܻ򲻴��ڣ��򷵻�Ĭ��ֵ
enum class xml_tree_encoding { unknown, utf_8, gb_2312};
HM_GlobalTool_EXT xml_tree_encoding get_xml_tree_encoding(const tinyxml2::XMLElement* node);

// �ַ�����ȡʱ��������뷽ʽָ��Ϊunknown�����ڲ����Զ�ʶ��xml����ͷ�жϱ��룬����ָ������ֱ�Ӷ�ȡ����Ҫ��Ϊ����֪����ʱ���Ч�ʣ�
// xml�Ľڵ�Ϊ��ʱ�᷵��һ��Ĭ��ֵ������node����nullptr
HM_GlobalTool_EXT CString get_xml_node_text(const tinyxml2::XMLElement* node, xml_tree_encoding ec = xml_tree_encoding::unknown);
HM_GlobalTool_EXT CString get_xml_node_attribute_str(const tinyxml2::XMLElement* node, const CString& attribute, xml_tree_encoding ec = xml_tree_encoding::unknown);
HM_GlobalTool_EXT bool get_xml_node_attribute_bool(const tinyxml2::XMLElement* node, const CString& attribute); // ��д�������򷵻�false
HM_GlobalTool_EXT int get_xml_node_attribute_int(const tinyxml2::XMLElement* node, const CString& attribute);
HM_GlobalTool_EXT unsigned int get_xml_node_attribute_uint(const tinyxml2::XMLElement* node, const CString& attribute);
HM_GlobalTool_EXT float get_xml_node_attribute_float(const tinyxml2::XMLElement* node, const CString& attribute);
HM_GlobalTool_EXT double get_xml_node_attribute_double(const tinyxml2::XMLElement* node, const CString& attribute);

#endif //__HMGLOBALTOOL_TOOLS_XML_TOOLS_H__
