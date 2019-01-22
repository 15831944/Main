#include "StdAfx.h"

#include "../tools/string_tools.h"

#include "xml_tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


inline string get_declaration_encoding(const string& dec_text)
{
	string text_lower(dec_text);
	for (char& ch : text_lower) ch = tolower(ch);

	string::size_type pos_s = dec_text.find("encoding"); // ������find_first_of����ֻ��ƥ���ַ����е������ַ�������λ��
	if (string::npos == pos_s) return "";

	pos_s = text_lower.find_first_of('\"', pos_s);
	if (string::npos == pos_s) return "";

	string encoding = "";
	string::size_type pos_e = text_lower.find_first_of('\"', pos_s + 1);
	if (string::npos == pos_e)
		encoding = text_lower.substr(pos_s + 1);
	else
		encoding = text_lower.substr(pos_s + 1, pos_e - pos_s - 1); // pos_eֻ���ܴ���pos_s�������Ӵ����Ȳ���Ϊ0����

	return encoding;
}

xml_tree_encoding get_xml_tree_encoding(const tinyxml2::XMLElement* node)
{
	if (!node) return xml_tree_encoding::unknown;

	const tinyxml2::XMLDocument* doc = node->GetDocument();
	tinyxml2::XMLDeclaration* dec = doc ? const_cast<tinyxml2::XMLDocument*>(doc)->NewDeclaration() : nullptr;
	const char* dec_text = dec ? dec->Value() : nullptr;
	string dec_info(dec_text ? dec_text : "");
	safe_delete_array(dec_text); // �ڴ�Ҫ�Լ��ͷ�

	string encoding = get_declaration_encoding(dec_info);
	for (char& ch : encoding) ch = tolower(ch);

	if ("utf-8" == encoding)
		return xml_tree_encoding::utf_8;
	else if ("gb2312" == encoding)
		return xml_tree_encoding::gb_2312; // ΪMBCS����
	else
		return xml_tree_encoding::unknown;
}

CString get_xml_node_attribute_str(const tinyxml2::XMLElement* node, const CString& attribute, xml_tree_encoding ec/* = xml_tree_encoding::unknown*/)
{
	CString result = _T("");
	if (!node) return result;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	const char* res = node->Attribute(string_tools::CString_to_string(attribute).c_str());
	if (res)
	{
		if (xml_tree_encoding::unknown == ec) ec = get_xml_tree_encoding(node);

		string attr(res);
		if (xml_tree_encoding::utf_8 == ec)
			result = string_tools::utf8_to_CString(attr); // utf8
		else if (xml_tree_encoding::utf_8 == ec)
			result = string_tools::string_to_CString(attr); // ΪMBCS����
		else
			result = string_tools::string_to_CString(attr); // ���������ݲ�֧�֣�����MBCS����
	}

	// safe_delete_array(res); �ڴ���tinyxml�Զ�����

	return result;
}

bool get_xml_node_attribute_bool(const tinyxml2::XMLElement* node, const CString& attribute)
{
	if (!node) return false;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	return node->BoolAttribute(string_tools::CString_to_string(attribute).c_str());
}

int get_xml_node_attribute_int(const tinyxml2::XMLElement* node, const CString& attribute)
{
	if (!node) return 0;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	return node->IntAttribute(string_tools::CString_to_string(attribute).c_str());
}

unsigned int get_xml_node_attribute_uint(const tinyxml2::XMLElement* node, const CString& attribute)
{
	if (!node) return 0;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	return node->UnsignedAttribute(string_tools::CString_to_string(attribute).c_str());
}

float get_xml_node_attribute_float(const tinyxml2::XMLElement* node, const CString& attribute)
{
	if (!node) return 0.0f;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	return node->FloatAttribute(string_tools::CString_to_string(attribute).c_str());
}

double get_xml_node_attribute_double(const tinyxml2::XMLElement* node, const CString& attribute)
{
	if (!node) return 0.0;

	// ������һ�㲻�����ģ�һ�������ֻ��Ӣ�ĺ����ֻ����»�����ϣ��Ҳ������ֿ�ͷ����˲��ö����԰�xml�������ת��
	return node->DoubleAttribute(string_tools::CString_to_string(attribute).c_str());
}

CString get_xml_node_text(const tinyxml2::XMLElement* node, xml_tree_encoding ec /*= xml_tree_encoding::unknown*/)
{
	if (!node) return _T("");

	const char* text_buffer = node->GetText();
	string text = (text_buffer ? text_buffer : "");
	// safe_delete(text_buffer); // ����ɾ����xml�ڲ��ᴦ��

	if (xml_tree_encoding::unknown == ec) ec = get_xml_tree_encoding(node);

	CString t_text;
	if (xml_tree_encoding::utf_8 == ec)
		t_text = string_tools::utf8_to_CString(text);
	else if (xml_tree_encoding::gb_2312 == ec)
		t_text = string_tools::string_to_CString(text);
	else
		t_text = string_tools::string_to_CString(text);

	return t_text;
}
