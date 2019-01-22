#include "stdafx.h"
#include <ostream>
#include <istream>
#include "../interface/serialize_interface.h"
#include "../tools/file_tools.h"
#include "../tools/string_tools.h"
#include "../tools/xml_tools.h"
#include "../tools/json.h"
#include "../tools/base64_tools.h"


#include "data_center_interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


//////////////////////////////////////////////////////////////////////////

static bool load_x_framework_config()
{
	// ��ʼ��ȡxml����
	CString xml_file_full_path = file_tools::get_app_path_with_xg() + _T("gui_xml\\x_framework_configs.xml");
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(string_tools::CString_to_string(xml_file_full_path).c_str())) return false;

	tinyxml2::XMLElement* node = doc.FirstChildElement("x_framework_configs");
	if (!node || node->NoChildren()) return false;

	node = node->FirstChildElement("data_center");
	if (!node || node->NoChildren()) return false;

	xml_tree_encoding encoding = get_xml_tree_encoding(node);

	tinyxml2::XMLElement* node_suffix = node->FirstChildElement("project_file_suffix");
	if (node_suffix)
	{
		const char* text_buffer = node_suffix->GetText();
		string text = (text_buffer ? text_buffer : "");
		// safe_delete(text_buffer); // ����ɾ����xml�ڲ��ᴦ��

		CString t_text;
		if (xml_tree_encoding::utf_8 == encoding)
			t_text = string_tools::utf8_to_CString(text);
		else
			t_text = string_tools::string_to_CString(text);

		array<TCHAR, 4> file_suffix;
		for (int i = 0; i < (int)file_suffix.size(); ++i)
			file_suffix[i] = (t_text.GetLength() > i ? t_text[i] : _T(' '));
		
		data_center::set_data_file_suffix(file_suffix);
	}

	tinyxml2::XMLElement* node_filter = node->FirstChildElement("project_file_filter");
	if (node_filter)
	{
		const char* text_buffer = node_filter->GetText();
		string text = (text_buffer ? text_buffer : "");
		// safe_delete(text_buffer); // ����ɾ����xml�ڲ��ᴦ��

		CString filter;
		if (xml_tree_encoding::utf_8 == encoding)
			filter = string_tools::utf8_to_CString(text);
		else
			filter = string_tools::string_to_CString(text);

		data_center::set_data_file_filter(filter);
	}

	return true;
}

// �ȳ�ʼ��һ��Ĭ�ϲ���
// CString data_center::m_file_filter = _T("main_application v2��Ŀ�ļ�(*.mapx)|*.mapx|�����ļ�(*.*)|*.*||");
CString data_center::m_file_filter = _T("main_application v2��Ŀ�ļ�(*.mapx)|*.mapx||");
std::array<TCHAR, 4> data_center::m_file_suffix = {_T('m'), _T('a'), _T('p'), _T('x')}; // _T("mapx");

// ����ȫ�ֱ����ڳ�����ص�ʱ���Զ���ʼ����������xml��صı�̬��Ա���������Ĭ�ϳ�ʼ������֮�󣬷����޷����ǣ�
static bool initlize_x_framework_config = load_x_framework_config();

// ��������xml�����޹صĳ�Ա��ʼ��
CString data_center::m_full_path_name = _T(""); // ��ǰ��Ŀ�Ĵ洢·�������Ϊ�ձ�ʾδ�����
bool data_center::m_is_open = false;
project_data_base* data_center::m_self_data_node = nullptr;
std::set<data_node_notify_interface*> data_center::m_registed_notification_responsing_objects;
std::mutex data_center::m_notify_mx;
std::deque<std::pair<CString, notifer_action>> data_center::m_notify_queue;
int data_center::m_current_notify_queue_index = 0;


//////////////////////////////////////////////////////////////////////////


notify_post_doing_function::notify_post_doing_function()
{

}

notify_post_doing_function::~notify_post_doing_function()
{

}

void notify_post_doing_function::call()
{
	data_center::notify_if_enable(m_node_tag, m_action);
}

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL(data_node_base, CObject, 1)
data_node_base::data_node_base(const CString& node_name/* = _T("")*/, bool auto_recycle/* = true*/,
children_recycling_type children_recycling/* = children_recycling_type::custom*/)
{
	m_name = node_name;
	m_parent = nullptr;
	m_node_tag = string_tools::generate_guid();
	m_auto_recycle = auto_recycle;
	m_children_recycling_type = children_recycling;
	
	// ���캯��Ҫ��Ӧ�õ�������Ĭ��ֵ�Ľӿڣ�����ᴥ�����ݱ䶯֪ͨ���򿪻��½��ļ���ʱ��Ͷ�δ����ˣ������ֻ��
	// ���½��ļ���ʱ�򴥷����򿪲�Ҫ������
	// set_default_value();
}

data_node_base::data_node_base(const data_node_base& src)
{
	// �����ʱ��֪ͨ�������ӳٵ��ҽӵ���������ʱ������Ӧ֪ͨ
	data_node_notify_guard tmp(false);

	copy(src, true);
	m_parent = nullptr;

	// ���캯��Ҫ��Ӧ�õ�������Ĭ��ֵ�Ľӿڣ�����ᴥ�����ݱ䶯֪ͨ���򿪻��½��ļ���ʱ��Ͷ�δ����ˣ������ֻ��
	// ���½��ļ���ʱ����ô˽ӿڣ��򿪲�Ҫ���ã�
	// set_default_value();
}

data_node_base::~data_node_base()
{
	// ������ʱ��֪ͨ��������̻�ǡ�����������Ƴ���֪ͨ��ʱ�������Ӵ˴�������������˵�ǲ�����֪��Ҫ��Ҫ
	// ����֪ͨ�ģ��п����ǳ����ڹر���ɲ��ͳ������ˣ���˴˴�����֪ͨ
	data_node_notify_guard tmp(false);
	delete_all_children();
}

data_node_base& data_node_base::operator=(const data_node_base& node)
{
	if (&node == this) return *this;

	copy(node, true);
	return *this;
}

bool data_node_base::operator==(const data_node_base& node) const
{
	if (this == &node) return true;
	return is_equal(node);
}

const data_node_base* data_node_base::operator[](size_t index) const
{
	return m_children[index];
}

data_node_base* data_node_base::operator[](size_t index)
{
	return m_children[index];
}

void data_node_base::swap(data_node_base& right, bool swap_tag/* = true*/)
{
	if (this == &right) return;

	data_node_base* tmp = clone(swap_tag);
	if (!tmp) return;

	copy(right, swap_tag);
	right.copy(*tmp, swap_tag);
}

void data_node_base::swap(data_node_base& left, data_node_base& right, bool swap_tag/* = true*/)
{
	left.swap(right, swap_tag);
}

void data_node_base::serialize_members(member_rw_interface& mt)
{
	x_object_flag_serialization_guard tmp_object_flag(_T("data_node_base"), mt);

	int version = 2;

	if (mt.is_storing())
	{
		mt.write(_T("version"), version);

		mt.write(_T("m_node_tag"), m_node_tag);
		mt.write(_T("m_name"), m_name);
		mt.write(_T("m_node_info"), m_node_info);
		mt.write(_T("m_children_recycling_type"), (int)m_children_recycling_type);
		mt.write(_T("m_auto_recycle"), m_auto_recycle);
		serialize_children(mt);
	}
	else
	{
		mt.read(_T("version"), version);

		if (1 == version)
		{
			mt.read(_T("m_node_tag"), m_node_tag);
			mt.read(_T("m_name"), m_name);
			mt.read(_T("m_node_info"), m_node_info);
			serialize_children(mt);
		}
		else if (2 == version)
		{
			mt.read(_T("m_node_tag"), m_node_tag);
			mt.read(_T("m_name"), m_name);
			mt.read(_T("m_node_info"), m_node_info);

			int tmp_children_recycling_tye = 0;
			mt.read(_T("m_children_recycling_type"), (int) tmp_children_recycling_tye);
			m_children_recycling_type = (children_recycling_type)tmp_children_recycling_tye;

			mt.read(_T("m_auto_recycle"), m_auto_recycle);
			serialize_children(mt);
		}
		else
		{
			// nothing;
		}
	}
}

void data_node_base::serialize_children(member_rw_interface& mt)
{
	int children_version = 1;

	if (mt.is_storing())
	{
		mt.write(_T("children_version"), children_version);
		mt.write(_T("children_items"), serialize_flag::array_bg);

		mt.write(_T("children_count"), (int)(m_children.size()));
		for (data_node_base* x : m_children) // �ڲ�����Ϊָ�룬���л����߲�֧�֣�Ҫ�ֹ����л�
		{
			mt.write(_T("child_object"), serialize_flag::sub_obj_bg);
			mt.write(_T("is_nullptr"), (bool)(nullptr == x)); // ��ʽд��boolֵ����ǿ����ɶ���
			if (x)
			{
				CString class_name = x->GetRuntimeClass()->m_lpszClassName;
				mt.write(_T("class_name"), class_name);
				x->serialize_members(mt);
			}
			mt.write(_T("child_object"), serialize_flag::sub_obj_ed);
		}

		mt.write(_T("children_items"), serialize_flag::array_ed);
	}
	else
	{
		mt.read(_T("children_version"), children_version);

		if (1 == children_version)
		{
			mt.read(_T("children_items"), serialize_flag::array_bg);

			int count = 0;
			mt.read(_T("children_count"), count);

			// ԭ�ж���Ҫ���������ܼ򵥵����
			// m_children.clear();
			delete_all_children();

			for (int i = 0; i < count; ++i) // �ڲ�����Ϊָ�룬���л����߲�֧�֣�Ҫ�ֹ����л�
			{
				mt.read(_T("child_object"), serialize_flag::sub_obj_bg);

				bool is_nullptr = false;
				mt.read(_T("is_nullptr"), is_nullptr); 
				if (is_nullptr)
				{
					m_children.push_back(nullptr);
				}
				else
				{
					CString class_name;
					mt.read(_T("class_name"), class_name);

					CObject* obj = CRuntimeClass::CreateObject(class_name);
					if (!obj)
					{
						m_children.push_back(nullptr); // �Ÿ��ն��󣬱�֤��д��ʱ���ݸ���һ����
					}
					else
					{
						data_node_base* nd = dynamic_cast<data_node_base*>(obj);
						if (!nd)
						{
							safe_delete(obj);
						}
						else
						{
							nd->m_parent = this;
							nd->serialize_members(mt);
							m_children.push_back(nd);
						}
					}
				}
				mt.read(_T("child_object"), serialize_flag::sub_obj_ed);
			}

			mt.read(_T("children_items"), serialize_flag::array_ed);
		}
		else
		{
			// nothing;
		}
	}
}

void data_node_base::set_default_value()
{
	// ����ԭtag����
	// m_node_tag = _T("");
	m_name = _T("");
	
	// �������ͼ�Ϊ��Ҫ�������û������ָ��ʱ���������޸ģ��Ҵ�����������������ֵ��һ��������������ڿ�ܹ���
	// ��Ĳ���������Ĭ��ֵӦ�ý���������ĳ�Ա�������ü���
	//m_auto_recycle = node.m_auto_recycle;
	//m_children_recycling_type = node.m_children_recycling_type;

	for (data_node_base* x : m_children)
		if (x) x->set_default_value();

	notify_if_enable(notifer_action::post_updated_node);
}

void data_node_base::copy(const data_node_base& node, bool copy_tag/* = true */)
{
	if (&node == this) return;

	if (copy_tag) m_node_tag = node.m_node_tag;

	m_name = node.m_name;
	m_auto_recycle = node.m_auto_recycle;
	m_children_recycling_type = node.m_children_recycling_type;

	// ���������ѹ��һ�£��������ͳһ��һ�θ���֪ͨ���ɣ��ݹ鲿��ҲҪѹ����Ϣ
	{
		data_node_notify_guard tmp(false);
		delete_all_children();

		for (data_node_base* x : node.m_children)
		{
			if (!x) continue;

			CRuntimeClass* rt = x->GetRuntimeClass();
			if (!rt) continue;

			CObject* obj = CRuntimeClass::CreateObject(rt->m_lpszClassName);
			if (!obj) continue;

			data_node_base* new_node = dynamic_cast<data_node_base*>(obj);
			if (!new_node)
			{
				safe_delete(obj);
				continue;
			}

			new_node->copy(*x, copy_tag);
			// new_node->operator=(*x); // ������=�ţ���Ϊû�и���tag
			new_node->m_parent = this;

			m_children.push_back(new_node);
		}
	}

	notify_if_enable(notifer_action::post_updated_node);
}

bool data_node_base::is_equal(const data_node_base& src, bool compare_children /*= false*/) const
{
	if (m_name != src.m_name) return false;
	if (m_node_info != src.m_node_info) return false;
	
	// Ĭ��ʵ�ֲ��ȽϽڵ�Ļ������ò���
	// if (m_auto_recycle != src.m_auto_recycle) return false;
	// if (m_children_recycling_type != src.m_children_recycling_type) return false;
	
	if (compare_children)
	{
		if (m_children.size() != src.m_children.size()) return false;
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (nullptr == m_children[i] && nullptr == src.m_children[i]) continue; // both are nullptr means equal
			if (!m_children[i] || !src.m_children[i]) return false;
			if (!m_children[i]->is_equal(*src.m_children[i], compare_children)) return false;
		}
	}

	return true;
}

void data_node_base::set_recycling_type(bool auto_recycle)
{
	// �������ͼ�Ϊ��Ҫ�������û������ָ��ʱ���������޸ģ��Ҵ�����������������ֵ��һ��������������ڿ�ܹ���
	// ��Ĳ���������ֵʱ��Ӧ�ô��������޸�֪ͨ
	m_auto_recycle = auto_recycle;
}

bool data_node_base::is_auto_recyclable() const
{
	return m_auto_recycle;
}

void data_node_base::set_children_recycling_type(children_recycling_type recycling_type)
{
	// �������ͼ�Ϊ��Ҫ�������û������ָ��ʱ���������޸ģ��Ҵ�����������������ֵ��һ��������������ڿ�ܹ���
	// ��Ĳ���������ֵʱ��Ӧ�ô��������޸�֪ͨ
	m_children_recycling_type = recycling_type;
}

children_recycling_type data_node_base::get_children_recycling_type() const
{
	return m_children_recycling_type;
}

bool data_node_base::need_recycle() const
{
	// �����뱾�ڵ���������ӽڵ�������Ͳ�Ϊ�Զ��巽ʽ�Ľڵ�
	data_node_base* parent = m_parent;

	// nothing_to_do���ͽ���ֱ���ӽڵ���Ч������Ҫ�༶����
	if (parent && children_recycling_type::nothing_to_do == parent->m_children_recycling_type) return false;

	while (true)
	{
		if (!parent) break;
		if (children_recycling_type::custom != parent->m_children_recycling_type) break;
		parent = parent->m_parent;
	}

	if (nullptr == parent)
		return m_auto_recycle;
	else
		return children_recycling_type::recyclable == parent->m_children_recycling_type;
}

data_node_base* data_node_base::clone(bool clone_tag/* = false*/) const
{
	// ��̬�������Ķ������ȫ��Ψһ��tag
	CString target_class_name = GetRuntimeClass()->m_lpszClassName;
	data_node_base* result = dynamic_cast<data_node_base*>(CRuntimeClass::CreateObject(target_class_name));
	if (result) result->copy(*this, clone_tag); // ���Ḵ��tag
	return result;
}

path_object data_node_base::get_node_full_name_path_object(bool tail_is_folder) const
{
	path_object res;
	get_node_full_name_path(res, tail_is_folder);
	return res;
}

void data_node_base::get_node_full_tag_path(path_object& nd_path, bool tail_is_folder) const
{
	nd_path.clear();

	nd_path.set_node_data(m_node_tag, tail_is_folder);
	const data_node_base* cur_nd = m_parent;
	while (cur_nd)
	{
		nd_path.push_front(cur_nd->m_node_tag, true);
		cur_nd = cur_nd->m_parent;
	}
}

void data_node_base::get_node_full_tag_path(CString& nd_path) const
{
	const data_node_base* cur = this;
	path_object path_obj;
	cur->get_node_full_tag_path(path_obj, false); // ����ֻȡ·��������Ҫ֪�����һ�����ļ������ļ���
	path_obj.to_path_string(nd_path, false, _T("\\"));
}

CString data_node_base::get_node_full_tag_path() const
{
	CString res;
	get_node_full_tag_path(res);
	return res;
}

void data_node_base::get_node_full_name_path(path_object& nd_path, bool tail_is_folder) const
{
	nd_path.clear();

	nd_path.set_node_data(m_name, tail_is_folder);
	const data_node_base* cur_nd = m_parent;
	while (cur_nd)
	{
		nd_path.push_front(cur_nd->m_name, true);
		cur_nd = cur_nd->m_parent;
	}
}

void data_node_base::get_node_full_name_path(CString& nd_path) const
{
	path_object path_obj;
	get_node_full_name_path(path_obj, false); // ����ֻȡ·��������Ҫ֪�����һ�����ļ������ļ���
	path_obj.to_path_string(nd_path, false, _T("\\"));
}

CString data_node_base::get_node_full_name_path() const
{
	CString res;
	get_node_full_name_path(res);
	return res;
}

path_object data_node_base::get_node_full_tag_path_object(bool tail_is_folder) const
{
	path_object res;
	get_node_full_tag_path(res, tail_is_folder);
	return res;
}

const data_node_base* data_node_base::get_node_by_full_tag_path(const path_object& nd_path) const
{
	const path_object* cur_path = &nd_path;
	const data_node_base* cur_node = this;

	while (true)
	{
		if (nullptr == cur_path->get_child()) return cur_node; // ��β�ڵ�˵���Ѿ��ҵ����·����ƥ��
		if (0 != cur_node->m_node_tag.CompareNoCase(cur_path->get_node_name())) return nullptr;

		cur_path = cur_path->get_child();

		bool finded_valid_node = false;
		for (size_t i = 0; i < cur_node->get_children_count(); ++i)
		{
			const data_node_base* tmp_node = cur_node->get_child(i);
			if (!tmp_node) continue;

			if (0 == tmp_node->m_node_tag.CompareNoCase(cur_path->get_node_name()))
			{
				cur_node = tmp_node;
				finded_valid_node = true;
				break;
			}
		}

		if (!finded_valid_node) return nullptr;
	}
}

data_node_base* data_node_base::get_node_by_full_tag_path(const path_object& nd_path)
{
	const data_node_base* cur = this;
	return const_cast<data_node_base*>(cur->get_node_by_full_tag_path(nd_path));
}

const data_node_base* data_node_base::get_node_by_full_tag_path(const CString& nd_path) const
{
	path_object path(nd_path, _T("\\"));
	return get_node_by_full_tag_path(path);
}

data_node_base* data_node_base::get_node_by_full_tag_path(const CString& nd_path)
{
	path_object path(nd_path, _T("\\"));
	return get_node_by_full_tag_path(path);
}

const data_node_base* data_node_base::get_node_by_full_name_path(const path_object& nd_path) const
{
	const path_object* cur_path = &nd_path;
	const data_node_base* cur_node = this;
	
	while (true)
	{
		if (nullptr == cur_path->get_child()) return cur_node; // ��β�ڵ�˵���Ѿ��ҵ����·����ƥ��
		if (0 != cur_node->m_name.CompareNoCase(cur_path->get_node_name())) return nullptr;

		cur_path = cur_path->get_child();

		bool finded_valid_node = false;
		for (size_t i = 0; i < cur_node->get_children_count(); ++i)
		{
			const data_node_base* tmp_node = cur_node->get_child(i);
			if (!tmp_node) continue;

			if (0 == tmp_node->m_name.CompareNoCase(cur_path->get_node_name()))
			{
				cur_node = tmp_node;
				finded_valid_node = true;
				break;
			}
		}

		if (!finded_valid_node) return nullptr;
	}
}

data_node_base* data_node_base::get_node_by_full_name_path(const path_object& nd_path)
{
	const data_node_base* cur = this;
	return const_cast<data_node_base*>(cur->get_node_by_full_name_path(nd_path));
}

const data_node_base* data_node_base::get_node_by_full_name_path(const CString& nd_path) const
{
	path_object path(nd_path, _T("\\"));
	return get_node_by_full_name_path(path);
}

data_node_base* data_node_base::get_node_by_full_name_path(const CString& nd_path)
{
	path_object path(nd_path, _T("\\"));
	return get_node_by_full_name_path(path);
}

CString data_node_base::get_node_tag() const
{
	return m_node_tag;
}

CString data_node_base::rebuild_node_tag(bool return_new_tag /*= true*/)
{
	CString new_tag = string_tools::generate_guid();
	CString old_tag = get_node_tag();
	set_node_tag(new_tag);
	return (return_new_tag ? new_tag : old_tag);
}

void data_node_base::set_node_tag(const CString& tag)
{
	m_node_tag = tag;
}

void data_node_base::swap_node_tag(data_node_base& node)
{
	CString tmp = m_node_tag;
	m_node_tag = node.m_node_tag;
	node.m_node_tag = tmp;
}

CString data_node_base::get_node_name() const
{
	return m_name;
}

void data_node_base::set_node_name(const CString& name)
{
	m_name = name;
	notify_if_enable(notifer_action::post_updated_node);
}

CString data_node_base::get_node_info() const
{
	return m_node_info;
}

void data_node_base::set_node_info(const CString& info, bool need_notify_if_enable/* = false*/)
{
	m_node_info = info;
	if (need_notify_if_enable) notify_if_enable(notifer_action::post_updated_node);
}

bool data_node_base::has_children() const
{
	return (!m_children.empty());
}

data_node_base* data_node_base::add_child_node(data_node_base* node)
{
	node->m_parent = this;
	m_children.push_back(node);
	node->notify_if_enable(notifer_action::post_added_node);
	return node;
}

data_node_base* data_node_base::insert_child_node(data_node_base* node, int index /*= -1*/, bool is_forward /*= true*/)
{
	if (!node) return nullptr;

	// �Ƿ������ݻ�λ��ʱֱ�������һ��λ�ò���
	if ((int) m_children.size() <= index || m_children.empty() || index < -1)
	{
		m_children.push_back(node);
		node->m_parent = this;
		node->notify_if_enable(notifer_action::post_added_node);
		return node;
	}

	// �������-1ת��Ϊ�������±꣨ǰ���Ѿ����ӽڵ�Ϊ����Ϊ����
	int last_index = (int)m_children.size();
	if (-1 == index) index = last_index;

	// �����еĺ����ͳһ��Ϊǰ���룬��Ϊ��׼��ʹ�õ���ǰ���룬����ת�����߼����򵥣�����һ�����⣬�������һ��Ԫ��
	// �ĺ�������޷���Ϊǰ���룬��Ϊʹ���±�ķ�ʽ���޷��޷���ʾend()�����±�ģ����Ҫ���⴦��
	if (last_index == index && false == is_forward)
	{
		m_children.push_back(node);
		node->m_parent = this;
		node->notify_if_enable(notifer_action::post_added_node);
		return node;
	}

	// �����ĺ�����������ת��Ϊǰ����
	if (false == is_forward)
	{
		++index;
		is_forward = true;
	}

	auto it = m_children.begin();
	advance(it, index);
	m_children.insert(it, node);
	node->m_parent = this;
	node->notify_if_enable(notifer_action::post_added_node);
	return node;
}

data_node_base* data_node_base::get_parent()
{
	return m_parent;
}

const data_node_base* data_node_base::get_parent() const
{
	return m_parent;
}

bool data_node_base::is_parent(data_node_base* node, bool find_all_parent /*= false*/)
{
	if (!node) return false;

	data_node_base* parent = const_cast<data_node_base*>(this);
	while (true)
	{
		parent = parent->get_parent();
		if (!parent) return false;

		if (parent == node) return true;
		if (!find_all_parent) return false;
	}

	return false;
}

bool data_node_base::is_parent(const CString& node_tag, bool find_all_parent /*= false*/)
{
	data_node_base* node = data_center::get_data_node(node_tag);
	return is_parent(node, find_all_parent);
}

data_node_base* data_node_base::get_root_node()
{
	const data_node_base* const_self = this;
	data_node_base* result = const_cast<data_node_base*>(const_self->get_root_node());
	return result;
}

const data_node_base* data_node_base::get_root_node() const
{
	const data_node_base* self_const = this;
	if (!m_parent) return self_const; // û�и��ڵ�˵��������Ǹ�

	data_node_base* last_parent = m_parent;
	data_node_base* current_parent = m_parent;

	while (true)
	{
		current_parent = current_parent->get_parent();
		if (!current_parent) return last_parent;

		last_parent = current_parent;
	}

	return nullptr;
}

std::vector<data_node_base*>& data_node_base::get_children()
{
	return m_children;
}

std::vector<const data_node_base*> data_node_base::get_children() const
{
	vector<const data_node_base*> const_children;
	for (data_node_base* x : m_children) const_children.push_back(x);

	return const_children;
}

size_t data_node_base::get_children_count() const
{
	return m_children.size();
}

void data_node_base::resize_children(size_t sz)
{
	m_children.resize(sz, nullptr);
}

bool data_node_base::is_child_exist_by_tag(const CString& node_tag, bool find_all_children /*= false*/)
{
	for (data_node_base* x : m_children)
	{
		if (!x) continue;
		if (x->m_node_tag == node_tag) return true;
		if (find_all_children)
		{
			bool flag = x->is_child_exist_by_tag(node_tag, true);
			if (flag) return true;
		}
	}

	return false;
}

bool data_node_base::is_child_exist_by_name(const CString& node_name, bool find_all_children /*= false*/)
{
	for (data_node_base* x : m_children)
	{
		if (!x) continue;
		if (x->m_name == node_name) return true;
		if (find_all_children)
		{
			bool flag = x->is_child_exist_by_name(node_name, true);
			if (flag) return true;
		}
	}

	return false;
}

data_node_base* data_node_base::get_child(const CString& node_tag, bool find_all_children/* = false*/)
{
	const data_node_base* const_self = this;
	data_node_base* result = const_cast<data_node_base*>(const_self->get_child(node_tag, find_all_children));
	return result;
}

data_node_base* data_node_base::get_child(size_t index)
{
	const data_node_base* const_self = this;
	data_node_base* result = const_cast<data_node_base*>(const_self->get_child(index));
	return result;
}

const data_node_base* data_node_base::get_child(const CString& node_tag, bool find_all_children /*= false*/) const
{
	for (data_node_base* x : m_children)
	{
		if (!x) return nullptr;
		if (x->m_node_tag == node_tag) return x;
		if (find_all_children)
		{
			data_node_base* tmp = x->get_child(node_tag, true);
			if (tmp) return tmp;
		}
	}

	return nullptr;
}

const data_node_base* data_node_base::get_child(size_t index) const
{
	if (index >= m_children.size())
		return nullptr;
	else
		return m_children[index];
}

void data_node_base::set_child(size_t index, data_node_base* node)
{
	if (m_children.size() <= index) return;
	
	// Ϊ��Ҳ��
	if (!m_children[index])
	{
		data_node_notify_guard tmp(false);
		m_children[index] = node;
	}
	else
	{
		data_node_notify_guard tmp(false);

		m_children[index]->delete_all_children();
		if (m_children[index]->need_recycle()) safe_delete(m_children[index]);

		m_children[index] = node;
		node->m_parent = this;
	}

	notify_if_enable(notifer_action::post_updated_node);
}

void data_node_base::notify_if_enable(notifer_action ac)
{
	data_center::notify_if_enable(get_node_tag(), ac);
}

void data_node_base::post_notify_if_enable(notifer_action ac)
{
	data_center::post_notify_if_enable(get_node_tag(), ac);
}

bool data_node_base::is_joined_data_center()
{
	return data_center::is_node_joined_data_center(get_node_tag());
}

data_node_base* data_node_base::release_child(size_t index)
{
	if (m_children.empty() || index >= m_children.size()) return nullptr;
	
	data_node_base* res = m_children[index];
	m_children[index] = nullptr;
	return res;
}

void data_node_base::delete_data_node_self(bool force_remove_items/* = false*/)
{
	if (!need_recycle()) return;

	// �����ǰ�ڵ�Ϊ���ݸ��ڵ㣬��ֱ��������������(��ʵ�������Ӧ���ǲ�����ͨ���������û�ֱ�ӵ���ڵ�ɾ��������)
	if (data_center::get_data_root() == this)
	{
		// ���������ó�Աָ��Ϊ��û��ϵ��ֻҪ�����õ�this����
		data_center::close_file();
		data_center::destroy_data();
		return;
	}

	// �����ӽڵ㣨����ͨ��ɾ����������������������ɵݹ�����������Ϊ��ǰ�ڵ��п����ǲ����Զ����յģ����ӽڵ�ȴ��Ҫ���գ�
	// �ڲ��ᷢ��Ӧ��֪ͨ
	delete_all_children(force_remove_items);

	// ���Ҫɾ���Ľڵ�ĸ��ڵ���ڣ���ֱ��ͨ�����ڵ����ɾ�������أ�û�ҽӵ���������Ҳ��ɾ����
	data_node_base* parent = get_parent();
	if (parent)
	{
		for (size_t i = 0; i < parent->get_children_count(); ++i)
		{
			data_node_base* cur_node = parent->get_child(i);
			if (cur_node != this) continue;

			if (children_recycling_type::nothing_to_do == parent->m_children_recycling_type)
			{
				if (force_remove_items)
				{
					notify_if_enable(notifer_action::pre_removed_node);
					parent->m_children.erase(parent->m_children.begin() + i);
					notify_if_enable(notifer_action::post_removed_node);
				}

				return;
			}

			// ����ɾ����ʱ��������û��ִ�н�����ɾ��������ر��������ڷ���ǰɾ��
			//safe_delete(parent->m_children[i]);
			notify_if_enable(notifer_action::pre_removed_node);
			parent->m_children.erase(parent->m_children.begin() + i);
			notify_if_enable(notifer_action::post_removed_node);
			break;
		}
	}

	// ���Ҫɾ���Ľڵ�ĸ��ڵ㲻���ڣ���ֱ��������ڵ㣨û�ҽӵ���������Ҳ��ɾ����
	if (need_recycle()) delete this; // ��������Ϊnullptr����Ϊ�����Ѿ���������
}

void data_node_base::delete_all_children(bool force_remove_items/* = false*/)
{
	if (children_recycling_type::nothing_to_do == m_children_recycling_type && !force_remove_items) return;
	if (m_children.empty()) return;

	// ��ʱѹ��֪ͨ�����Ч�ʣ�����ÿ���ڵ�ɾ��������֪ͨ��ɾ����ɺ�ͳһ����һ�μ���
	{
		data_node_notify_guard tmp(false);
		for (data_node_base* x : m_children)
		{
			if (!x) continue;

			// �����ֹ��ݹ���ã����������ɽ�����������ʵ�ֵݹ�������Ϊ��Щ�ڵ��ǲ���ɾ���ģ����ӽڵ�ȴ��Ҫɾ
			// ��������û�е���delete���������������������ӽڵ�δ�������������ڴ�й©��
			if (x->has_children()) x->delete_all_children(force_remove_items);
			if (x->need_recycle()) safe_delete(x);
		}
	}

	m_children.clear(); // �Ƴ������ӽڵ�	
	notify_if_enable(notifer_action::post_updated_node); // ֪ͨ��ǰ�ڵ��и���
}

void data_node_base::delete_child_node(data_node_base* node,
	bool find_all_children /*= false*/, bool force_remove_items/* = false*/)
{
	if (!node) return;
	delete_child_node(node->get_node_tag(), find_all_children, force_remove_items);
}

void data_node_base::delete_child_node(const CString& tag,
	bool find_all_children /*= false*/, bool force_remove_items/* = false*/)
{
	if (children_recycling_type::nothing_to_do == m_children_recycling_type && !force_remove_items) return;

	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (!(*it)) continue;
		if ((*it)->m_node_tag == tag)
		{
			if ((*it)->need_recycle())
			{
				// �ȷ���֪ͨ��ָ֮���ڵ㼴����ɾ��
				(*it)->notify_if_enable(notifer_action::pre_removed_node);
			
				// ���ʹ�ü�ʱ֪ͨ����Ҫ����delete֮�󣬴�ʱ�����Ѿ�ɾ���ˣ��ٵ��÷Ǿ�̬��Ա���������֮ǰ�����û��ɾ����
				// ����֪ͨ����Щ����Ҫ���������Ϣ���½��棬�������û��ɾ���������������ʾ������˷���֮ǰ����ʹ��
				// post�ķ�ʽ��֪ͨ���������������
				(*it)->post_notify_if_enable(notifer_action::post_removed_node);

				safe_delete(*it);
				m_children.erase(it);
			}
			else
			{
				// ����Ҫ����ʱ����ذ������������ĵĹ�����ϵ�Ͽ����Է�ֹ�������ı�����
				(*it)->m_parent = nullptr;
			}

			return;
		}
		else
		{
			if (find_all_children) (*it)->delete_child_node(tag, find_all_children, force_remove_items);
		}
	}
}

void data_node_base::delete_child_node(size_t index, bool force_remove_items /*= false*/)
{
	if (children_recycling_type::nothing_to_do == m_children_recycling_type && !force_remove_items) return;
	if (index >= m_children.size()) return;

	delete_child_node(m_children.begin() + index, force_remove_items);
}

std::vector<data_node_base*>::iterator data_node_base::delete_child_node(std::vector<data_node_base*>::iterator it, bool force_remove_items /*= false*/)
{
	if (it == m_children.end()) return m_children.end();

	// ��ò�Ҫֱ����it + 1���Է������������
	std::vector<data_node_base*>::iterator ret_it = it;
	++ret_it;

	data_node_base* node = *it;

	if (children_recycling_type::nothing_to_do == m_children_recycling_type)
	{
		if (!force_remove_items) return ret_it; // ʲôҲ����
		
		if (node)
		{
			// �ȷ���֪ͨ��ָ֮���ڵ㼴����ɾ��
			node->notify_if_enable(notifer_action::pre_removed_node);

			// ���ʹ�ü�ʱ֪ͨ����Ҫ����delete֮�󣬴�ʱ�����Ѿ�ɾ���ˣ��ٵ��÷Ǿ�̬��Ա���������֮ǰ�����û��ɾ����
			// ����֪ͨ����Щ����Ҫ���������Ϣ���½��棬�������û��ɾ���������������ʾ������˷���֮ǰ����ʹ��
			// post�ķ�ʽ��֪ͨ���������������
			node->post_notify_if_enable(notifer_action::post_removed_node);
		}

		ret_it = m_children.erase(it); // ǿ��ɾ��ʱֻ�Ƴ�
		if (!node) notify_if_enable(notifer_action::post_updated_node); // ֪ͨ��ǰ�ڵ��и���
		
		return ret_it;
	}

	// �±귽ʽɾ��Ԫ��ʱ����ָ��ҲҪ�������Ƴ�
	if (!node)
	{
		ret_it = m_children.erase(it);
		notify_if_enable(notifer_action::post_updated_node); // ֪ͨ��ǰ�ڵ��и���
		return ret_it;
	}

	// �ȷ���֪ͨ��ָ֮���ڵ㼴����ɾ��
	node->notify_if_enable(notifer_action::pre_removed_node);
	
	// ���ʹ�ü�ʱ֪ͨ����Ҫ����delete֮�󣬴�ʱ�����Ѿ�ɾ���ˣ��ٵ��÷Ǿ�̬��Ա���������֮ǰ�����û��ɾ����
	// ����֪ͨ����Щ����Ҫ���������Ϣ���½��棬�������û��ɾ���������������ʾ������˷���֮ǰ����ʹ��
	// post�ķ�ʽ��֪ͨ���������������
	node->post_notify_if_enable(notifer_action::post_removed_node);

	// ������û�л��ն��Ƴ�
	ret_it = m_children.erase(it);

	// �������Ƴ������٣�����ָ���Ԫ�صĵ����������𻵣�����
	if (node->need_recycle()) safe_delete(node);

	return ret_it;
}

void data_node_base::delete_children_nodes(std::vector<data_node_base*>::iterator be,
	std::vector<data_node_base*>::iterator en, bool force_remove_items /*= false*/)
{
	if (children_recycling_type::nothing_to_do == m_children_recycling_type
		&& !force_remove_items)
		return;

	// ɾ��Ԫ�ص�ʱ������be,en��Ϊѭ������Ϊɾ��Ԫ�غ�ᵼ��enʧЧ��ÿ�ζ�Ҫʵʱ��ȡend()��������û����������
	// ����ȡ����˰�Ҫɾ����Ԫ���ȼ����������ж�Ҫ��Ҫ���٣��������ֻ�ܵ���һ��erase����ɾ�������������ʧЧ��
	// ���Ҳ���ͨ����ε���ɾ������Ԫ�صĽӿ����������ｫ������תΪ�±�������ܿ��������

	vector<size_t> will_del_index;
	size_t be_index = std::distance(children_begin(), be);

	for (auto it = be; it != en; ++it)
	{
		will_del_index.push_back(be_index);
		++be_index;
	}

	// ���±�ķ�ʽɾ����Ϊ��ֹ�±�仯������Ӻ���ǰɾ��
	{
		data_node_notify_guard tmp(false);
		for (int i = (int)will_del_index.size() - 1; i >= 0; --i)
			delete_child_node(will_del_index[i], force_remove_items);
	}

	notify_if_enable(notifer_action::post_updated_node);
}

std::vector<data_node_base*>::iterator data_node_base::children_begin()
{
	return m_children.begin();
}

std::vector<data_node_base*>::iterator data_node_base::children_end()
{
	return m_children.end();
}

std::vector<data_node_base*>::reverse_iterator data_node_base::children_rbegin()
{
	return m_children.rbegin();
}

std::vector<data_node_base*>::reverse_iterator data_node_base::children_rend()
{
	return m_children.rend();
}

std::vector<data_node_base*>::const_iterator data_node_base::children_cbegin()
{
	return m_children.cbegin();
}

std::vector<data_node_base*>::const_iterator data_node_base::children_cend()
{
	return m_children.cend();
}

std::vector<data_node_base*>::const_reverse_iterator data_node_base::children_crbegin()
{
	return m_children.crbegin();
}

std::vector<data_node_base*>::const_reverse_iterator data_node_base::children_crend()
{
	return m_children.crend();
}

bool travel_current_node_tree_implement(int current_depth, data_node_base* current_node, std::function<bool(data_node_base*)> access_func, int depth /*= -1*/)
{
	// ����нڵ���ʺ�������true���򱾺�������true���������������false�����쳣����������ʱ����false

	if (!current_node || !access_func) return false;
	if (current_depth < 0) return false;

	bool need_access_children = false;
	bool need_access_current_node = false;
	if (0 == depth)
	{
		if (0 == current_depth) need_access_current_node = true;
	}
	else if (-1 == depth)
	{
		need_access_children = true;
		need_access_current_node = true;
	}
	else if (depth > 0)
	{
		if (current_depth <= depth) need_access_current_node = true;
		if (current_depth < depth) need_access_children = true;
	}
	else
	{
		// nothing;
	}

	if (need_access_current_node && access_func(current_node)) return true;

	if (need_access_children)
	{
		for (size_t i = 0; i < current_node->get_children_count(); ++i)
			if (travel_current_node_tree_implement(current_depth + 1,
				current_node->get_child(i), access_func, depth))
				return true;
	}

	return false;
}

bool travel_current_cosnt_node_tree_implement(int current_depth, const data_node_base* current_node, std::function<bool(const data_node_base*)> access_func, int depth /*= -1*/)
{
	// ����нڵ���ʺ�������true���򱾺�������true���������������false�����쳣����������ʱ����false

	if (!current_node || !access_func) return false;
	if (current_depth < 0) return false;

	bool need_access_children = false;
	bool need_access_current_node = false;
	if (0 == depth)
	{
		if (0 == current_depth) need_access_current_node = true;
	}
	else if (-1 == depth)
	{
		need_access_children = true;
		need_access_current_node = true;
	}
	else if (depth > 0)
	{
		if (current_depth <= depth) need_access_current_node = true;
		if (current_depth < depth) need_access_children = true;
	}
	else
	{
		// nothing;
	}

	if (need_access_current_node && access_func(current_node)) return true;

	if (need_access_children)
	{
		for (size_t i = 0; i < current_node->get_children_count(); ++i)
			if (travel_current_cosnt_node_tree_implement(current_depth + 1,
				current_node->get_child(i), access_func, depth))
				return true;
	}

	return false;
}

void data_node_base::travel_current_node_tree(std::function<bool(data_node_base*)> access_func, int depth /*= -1*/)
{
	travel_current_node_tree_implement(0, this, access_func, depth); // ��ǰΪ������0��
}

void data_node_base::travel_current_node_tree(std::function<bool(const data_node_base*)> access_func, int depth /*= -1*/) const
{
	travel_current_cosnt_node_tree_implement(0, this, access_func, depth); // ��ǰΪ������0��
}

bool data_node_base::import_data(int type, const CString& full_path_name)
{
	//CWaitCursor wait_cur;

	if (full_path_name.IsEmpty()) return false;

	// ���л��������Ƚ���֪ͨ����
	{
		data_node_notify_guard tmp_guard(false);
		
		delete_all_children();

		if (0 == type)
		{
			CFile new_file;
			BOOL flag = new_file.Open(full_path_name, CFile::modeRead | CFile::modeNoTruncate);
			if (!flag) return false;

			CArchive mfc_ar(&new_file, CArchive::load);
			bin_archive ar(mfc_ar);
			serialize_members(ar); // ����Ϊ���ã����ж�̬��
			mfc_ar.Close();
			new_file.Close();
		}
		else if (1 == type)
		{
			ifstream new_file(string_tools::CString_to_string(full_path_name), ios::in);
			if (!new_file.is_open() && !new_file.good()) return false;

			// ���������һ���԰������ļ����루��ԭʼ�ļ����ļ������ٶ�ȡ�ķ���������ã�
			// ���뵼��������Ҫ���ڸ������ܣ��ҳ��򲢲����㽫����������ھ޴�Ķ������ϣ�����ļ�
			// ����ܴ�30MB���ڣ���һ���Է��䲢ȫ����ȡ
			string full_json_text((istreambuf_iterator<char>(new_file)), istreambuf_iterator<char>());
			new_file.close();

			Json::Value json_obj;
			Json::Reader reader;
			if (!reader.parse(full_json_text, json_obj)) return false; // json���ݱ���Ϊutf-8����
			json_archive ar(&json_obj, false);
			serialize_members(ar);
		}
		else if (2 == type)
		{
			tinyxml2::XMLDocument doc;
			if (tinyxml2::XML_SUCCESS != doc.LoadFile(string_tools::CString_to_string(full_path_name).c_str())) return false;

			tinyxml2::XMLElement* xml = doc.FirstChildElement("xml_root"); // д���ʱ��������ǿ�Ƽ��ϴ˽ڵ㣬�����޷����л�
			if (!xml) return false;

			xml_archive ar(xml, false);
			serialize_members(ar);
		}
		else
		{
			return false;
		}
	}

	notify_if_enable(notifer_action::post_updated_node);

	return true;
}


bool data_node_base::import_data(const char* value, int length_in_byte)
{
	if (length_in_byte <= 0 || !value) return false;

	CMemFile file_reading((BYTE*)value, (UINT)length_in_byte);
	CArchive ar_reading(&file_reading, CArchive::load);
	bin_archive ar(ar_reading);
	serialize_members(ar); // ����Ϊ���ã����ж�̬��
	ar_reading.Close();
	file_reading.Detach();
	file_reading.Close();

	return true;
}

bool data_node_base::import_data(const std::string& bin_data_base_64)
{
	vector<char> output_buffer;
	base64_tools::decode_string_to_buffer(bin_data_base_64, output_buffer);
	if (output_buffer.empty()) return false;
	
	return import_data((char*)output_buffer.data(), (int)output_buffer.size());
}

bool data_node_base::import_data(const CString& value, int type)
{
	if (value.IsEmpty()) return false;

	// ���л��������Ƚ���֪ͨ����
	{
		data_node_notify_guard tmp_guard(false);

		delete_all_children();

		if (0 == type)
		{
			// base64���벻�����Ǳ��룬ת��Ϊͨ���ַ���ʱ��Ҫ�����ַ���ת��������ֱ��ǿת��Ч��Ҫ�ߺü�������Ϊ�ַ���
			// ת������Ҫ���ݱ��ػ���Ϣ��ת������
			string bin_data_base_64;
			for (int i = 0; i < value.GetLength(); ++i)
				bin_data_base_64 += (char)value[i];

			if (!import_data(bin_data_base_64)) return false;
		}
		else if (1 == type)
		{
			string full_json_text = string_tools::CString_to_string(value);
			Json::Value json_obj;
			Json::Reader reader;
			if (!reader.parse(full_json_text, json_obj)) return false; // json���ݱ���Ϊutf-8����
			json_archive ar(&json_obj, false);
			serialize_members(ar);
		}
		else if (2 == type)
		{
			tinyxml2::XMLDocument doc;
			string full_xml_text = string_tools::CString_to_string(value);
			if (tinyxml2::XML_SUCCESS != doc.LoadFile(full_xml_text.c_str())) return false;

			tinyxml2::XMLElement* xml = doc.FirstChildElement("xml_root"); // д���ʱ��������ǿ�Ƽ��ϴ˽ڵ㣬�����޷����л�
			if (!xml) return false;

			xml_archive ar(xml, false);
			serialize_members(ar);
		}
		else
		{
			return false;
		}
	}

	notify_if_enable(notifer_action::post_updated_node);

	return true;
}

bool data_node_base::export_data(int type, const CString& full_path_name)
{
	//CWaitCursor wait_cur;

	if (full_path_name.IsEmpty()) return false;

	CString path = file_tools::get_path_from_file_path_name_with_xg(full_path_name);
	if (!file_tools::is_path_or_file_exist(path))
		file_tools::create_directory_nested(path);

	// ���л��������Ƚ���֪ͨ����
	{
		data_node_notify_guard tmp_guard(false);

		if (0 == type)
		{
			CFile new_file;
			BOOL flag = new_file.Open(full_path_name, CFile::modeCreate | CFile::modeWrite);
			if (!flag) return false;

			CArchive mfc_ar(&new_file, CArchive::store);
			bin_archive ar(mfc_ar);
			serialize_members(ar);
			mfc_ar.Close();
			new_file.Close();
		}
		else if (1 == type)
		{
			Json::Value json;
			json_archive serialize_ar(&json, true);
			serialize_members(serialize_ar);
			string res = json.toStyledString();

			ofstream new_file(string_tools::CString_to_string(full_path_name), ios::out);
			if (!new_file.is_open() && !new_file.good()) return false;

			new_file.write(res.c_str(), res.length());
			new_file.close();
		}
		else if (2 == type)
		{
			const char* declaration = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			tinyxml2::XMLDocument doc;
			doc.Parse(declaration);// ������ݲ�����һ����Ĭ��������xml�ĵ������ַ�ʽ��ʹ�ýӿ�����������򵥣�

			tinyxml2::XMLElement* new_node = doc.NewElement("xml_root"); // д���ʱ��������ǿ�Ƽ��ϴ˽ڵ㣬�����޷����л�
			if (!new_node) return false;

			xml_archive ar(new_node, true);
			serialize_members(ar);
			doc.InsertEndChild(new_node);

			tinyxml2::XMLError err = doc.SaveFile(string_tools::CString_to_string(full_path_name).c_str());
			if (tinyxml2::XML_NO_ERROR != err) return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool data_node_base::export_data(char*& value, int& length_in_byte)
{
	safe_delete_array(value);
	length_in_byte = 0;

	// ����MFC���ڴ��ļ���һ������ͨ��MFC��CArchive��ʽ���л�Ϊһ���ڴ��
	pair<long, char*> buffer = { 0, nullptr };
	CMemFile file_writting;
	CArchive ar_writting(&file_writting, CArchive::store);
	bin_archive ar(ar_writting);
	serialize_members(ar);
	ar_writting.Close();
	long tmp_len = (long) file_writting.GetLength(); // �������CArchive�رպ���ã���������û�д�CArchive�Ļ�����д���ļ������������صĳ������ֽ�Ϊ��λ
	if (tmp_len > 0) buffer = make_pair(tmp_len, (char*) file_writting.Detach());
	file_writting.Close();

	if (tmp_len <= 0 || !buffer.second)
	{
		safe_delete_array(buffer.second);
		return false;
	}

	value = buffer.second;
	length_in_byte = buffer.first;

	return true;
}

bool data_node_base::export_data(std::string& bin_data_base_64)
{
	bin_data_base_64 = "";

	int length_in_byte = 0;
	char* value = nullptr;
	if (!export_data(value, length_in_byte)) return false;
	if (length_in_byte <= 0 || !value) return false;

	base64_tools::encode_buffer_to_string(value, length_in_byte, bin_data_base_64);
	safe_delete_array(value);
	return true;
}

bool data_node_base::export_data(CString& value, int type)
{
	value = _T("");

	// ���л��������Ƚ���֪ͨ����
	data_node_notify_guard tmp_guard(false);

	if (0 == type)
	{
		string bin_data_base_64;
		if (!export_data(bin_data_base_64)) return false;
		if (bin_data_base_64.length() <= 0) return false;

		// base64���벻�����Ǳ��룬ת��Ϊͨ���ַ���ʱ��Ҫ�����ַ���ת��������ֱ��ǿת��Ч��Ҫ�ߺü�������Ϊ�ַ���
		// ת������Ҫ���ݱ��ػ���Ϣ��ת������
		for (size_t i = 0; i < bin_data_base_64.length(); ++i)
			value += (TCHAR)bin_data_base_64[i];
	}
	else if (1 == type)
	{
		Json::Value json;
		json_archive serialize_ar(&json, true);
		serialize_members(serialize_ar);
		string res = json.toStyledString();
		value = string_tools::string_to_CString(res);
	}
	else if (2 == type)
	{
		const char* declaration = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
		tinyxml2::XMLDocument doc;
		doc.Parse(declaration);// ������ݲ�����һ����Ĭ��������xml�ĵ������ַ�ʽ��ʹ�ýӿ�����������򵥣�

		tinyxml2::XMLElement* new_node = doc.NewElement("xml_root"); // д���ʱ��������ǿ�Ƽ��ϴ˽ڵ㣬�����޷����л�
		if (!new_node) return false;

		xml_archive ar(new_node, true);
		serialize_members(ar);
		doc.InsertEndChild(new_node);

		tinyxml2::XMLPrinter printer;
		doc.Accept(&printer);
		string res = printer.CStr();

		// base64���벻�����Ǳ��룬ת��Ϊͨ���ַ���ʱ��Ҫ�����ַ���ת��������ֱ��ǿת��Ч��Ҫ�ߺü�������Ϊ�ַ���
		// ת������Ҫ���ݱ��ػ���Ϣ��ת������
		for (size_t i = 0; i < res.length(); ++i)
			value += (TCHAR)res[i];
	}
	else
	{
		return false;
	}

	return true;
}

bool data_node_base::take_snapshot(const CString& name)
{
	if (!is_joined_data_center()) return false;

	// ...

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool data_node_notify_interface::m_notify_enable = true;

data_node_notify_interface::data_node_notify_interface()
{
	data_center::m_registed_notification_responsing_objects.insert(this);
}

data_node_notify_interface::data_node_notify_interface(const data_node_notify_interface&)
{
	data_center::m_registed_notification_responsing_objects.insert(this);
}

data_node_notify_interface::~data_node_notify_interface()
{
	auto it = data_center::m_registed_notification_responsing_objects.find(this);
	if (it != data_center::m_registed_notification_responsing_objects.end())
		data_center::m_registed_notification_responsing_objects.erase(it);
}

void data_node_notify_interface::enable_notify(bool enable)
{
	m_notify_enable = enable;
}

bool data_node_notify_interface::is_notify_disabled()
{
	return !is_notify_enable();
}

bool data_node_notify_interface::is_notify_enable()
{
	return m_notify_enable;
}

data_node_notify_guard::data_node_notify_guard(bool enable)
{
	m_old_enable = data_node_notify_interface::is_notify_enable();
	data_node_notify_interface::enable_notify(enable);
}

data_node_notify_guard::~data_node_notify_guard()
{
	data_node_notify_interface::enable_notify(m_old_enable);
}


//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL(project_data_base, data_node_base, 1)

project_data_base::project_data_base(const CString& node_name /*= _T("")*/,
	bool auto_recycle /*= true*/, children_recycling_type children_recycling /*= children_recycling_type::custom*/)
	: data_node_base(node_name, auto_recycle, children_recycling)
{

}

project_data_base::~project_data_base()
{

}

void project_data_base::serialize_members(member_rw_interface& mt)
{
	x_object_flag_serialization_guard tmp_object_flag(_T("project_data_base"), mt);
	data_node_base::serialize_members(mt);

	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
	}
	else
	{
		mt.read(_T("version"), version);

		if (1 == version)
		{
		}
		else
		{
			// nothing;
		}
	}
}

void project_data_base::initialize_project()
{

}


//////////////////////////////////////////////////////////////////////////


CString data_center::get_data_file_filter()
{
	return m_file_filter;
}

void data_center::set_data_file_filter(const CString& filter)
{
	m_file_filter = filter;
}

CString data_center::get_data_file_suffix()
{
	array<TCHAR, 4> suffix = get_data_file_suffix_array();
	CString result;
	result += suffix[0];
	result += suffix[1];
	result += suffix[2];
	result += suffix[3];
	return result;
}

std::array<TCHAR, 4> data_center::get_data_file_suffix_array()
{
	return m_file_suffix;
}

void data_center::set_data_file_suffix(std::array<TCHAR, 4>& suffix)
{
	m_file_suffix = suffix;
}

bool data_center::is_open()
{
	return m_is_open;
}

bool data_center::is_closed()
{
	return !is_open();
}

CString data_center::get_data_file_full_path_name()
{
	if (is_closed()) return _T("");
	return m_full_path_name;
}


CString data_center::get_data_file_name(bool include_suffix /*= false*/)
{
	CString path_name = get_data_file_full_path_name();
	
	if (include_suffix)
		return file_tools::get_full_name_from_file_path_name(path_name);
	else
		return file_tools::get_name_from_file_path_name(path_name);
}

CString data_center::get_project_working_folder_path(bool end_with_backslashed /*= true*/)
{
	if (!is_open()) return _T("");

	CString result = file_tools::get_path_from_file_path_name(get_data_file_full_path_name());
	if (result.IsEmpty()) return _T("");

	// ���һ����׺������ͬ��ϵͳ��ͬ����Ŀ����ͬһĿ¼��ʱ�ᵼ�¹����ļ�����ͬ
	result += _T("\\") + get_data_file_name(false) + _T("_") + get_data_file_suffix();

	if (end_with_backslashed) result.Append(_T("\\"));

	return result;
}

bool data_center::open_file(const CString& path_name)
{
	if (!m_self_data_node)
	{
		AfxMessageBox(_T("�����ڲ������޷����ļ�"), MB_OK | MB_ICONERROR);
		return false;
	}

	// ��׺�����ͬ���һ��ȡ���Լ����ļ������ɺ�׺
	CString suffix_with_dot = path_name.Right(get_data_file_suffix().GetLength() + 1);
	if (0 != suffix_with_dot.CompareNoCase(_T(".") + get_data_file_suffix()))
	{
		AfxMessageBox(_T("�ļ���ʽ����"), MB_OK | MB_ICONERROR);
		return false;
	}

	CString full_path_name;
	if (file_tools::is_relative_path(path_name))
	{
		CString path_name_none_header_xg = path_name;
		path_name_none_header_xg.Trim(_T('\\'));
		path_name_none_header_xg.Trim(_T('/'));

		full_path_name = file_tools::get_app_path_with_xg() + path_name_none_header_xg;
	}
	else
	{
		full_path_name = path_name;
	}
	
	// ��ȡ
	if (full_path_name.IsEmpty())
	{
		AfxMessageBox(_T("�ļ�������Ϊ��"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (0 == full_path_name.CompareNoCase(m_full_path_name))
	{
		AfxMessageBox(_T("���ļ��Ѿ����ڴ�״̬"), MB_OK | MB_ICONINFORMATION);
		return false;
	}

	// ���ļ���ʱ��Ҫ���л����ݣ����ڵ���պ���Ҫ����set_default_value()��������ֱ�ӹ�������
	if (is_open()) close_file();

	// ���ô˽ӿڳ�ʼ��
	m_self_data_node->initialize_project();
	m_self_data_node->set_default_value();

	CFile new_file;
	BOOL flag = new_file.Open(full_path_name, CFile::modeRead | CFile::modeNoTruncate);
	if (!flag) return false;

	// һЩ������ڳ���Ϊ0���ļ������⴦��������ļ�ͷ��ֱ�ӵ�������һ��Ĭ���ĵ��򿪣������򲻼ƻ�������
	// ֧�֣�ֱ�ӵ�����ʽ�����Ժ�Ҫ֧�ֿ����ڴ����л�ǰ�����������ע����������

	CArchive mfc_ar(&new_file, CArchive::load);
	try
	{
		bin_archive serialize_ar(mfc_ar);
		
		// �ȶ�ȡ�汾�ţ�����û�����ļ�ͷ���汾����
		int version = 0;
		serialize_ar.read(_T("format_version"), version); // ��Ҫ��CArchive������json��xml���ͽ��޷����������л�

		// �ٶ�ȡ��ʽ��Ϣ����ʽǧ��Ҫʹ���ַ������л���ȥ����Ϊ�ַ������л���д�볤��Ȼ���д�����ݣ�����ļ�
		// ��ʽ���󣬿��ܵ������������һ���ܴ�����ݣ����ʱ�򰴳��ȶ�ȡ�ַ���Ҫô�ڴ�ű���Ҫôֱ�ӵ��ļ�β��
		// ����������������ݷ���������ˣ����ҵ��´����ļ��ĸ�ʽʶ��Ҫ���ܳ�ʱ�䣨���Ŷ����ַ����������Ը�Ϊֱ
		// �Ӷ�ȡ3���ַ�
		
		serialize_ar.read(_T("format_flag"), serialize_flag::array_bg);
		wchar_t ch[5] = { 0 };
		serialize_ar.read(_T("[0]"), ch[0]);
		serialize_ar.read(_T("[1]"), ch[1]);
		serialize_ar.read(_T("[2]"), ch[2]);
		serialize_ar.read(_T("[3]"), ch[3]);
		ch[4] = L'\0';
		serialize_ar.read(_T("format_flag"), serialize_flag::array_ed);

		wstring readed_w_suffix(ch);
		CString readed_suffix = string_tools::wstring_to_CString(readed_w_suffix);
		if (data_center::get_data_file_suffix() != readed_suffix) // �ļ�ͷд��ʱֻдСд��ʽ
		{
			AfxMessageBox(_T("�ļ���ʽ����"), MB_OK | MB_ICONERROR);
			return false;
		}

		if (0 == version)
		{
			m_self_data_node->serialize_members(serialize_ar);
		}
		else
		{
			AfxMessageBox(_T("�ļ���ʽ����"), MB_OK | MB_ICONERROR);
			return false;
		}
	}
	catch (CException* e)
	{
		if (e) e->ReportError();

		mfc_ar.Close();
		new_file.Close();

		return false;
	}

	mfc_ar.Close();
	new_file.Close();

	// �ɹ����ٴ���·���ͱ�־
	m_full_path_name = full_path_name;
	m_is_open = true;

	app_ui_interface* app = get_app_ui_interface();
	if (app) app->add_recent_file_list(m_full_path_name);

	m_self_data_node->notify_if_enable(notifer_action::post_project_loaded);

	return true;
}

bool data_center::save_as_file(const CString& path_name)
{
	if (!m_self_data_node)
	{
		AfxMessageBox(_T("�����ڲ������޷������ļ�"), MB_OK | MB_ICONERROR);
		return false;
	}

	CString full_path_name;
	if (file_tools::is_relative_path(path_name))
	{
		CString path_name_none_header_xg = path_name;
		path_name_none_header_xg.Trim(_T('\\'));
		path_name_none_header_xg.Trim(_T('/'));

		full_path_name = file_tools::get_app_path_with_xg() + path_name_none_header_xg;
	}
	else
	{
		full_path_name = path_name;
	}

	// �洢
	if (full_path_name.IsEmpty())
	{
		AfxMessageBox(_T("�ļ�������Ϊ��"), MB_OK | MB_ICONWARNING);
		return false;
	}

	CFile file;
	BOOL flag = file.Open(full_path_name, CFile::modeCreate | CFile::modeWrite);
	if (!flag) return false;

	CArchive mfc_ar(&file, CArchive::store);
	try
	{
		bin_archive serialize_ar(mfc_ar);

		// ��д��汾�ţ�����û�����ļ�ͷ���汾����
		int version = 0;
		serialize_ar.write(_T("format_version"), version);

		// ��д���ʽ��Ϣ����ʽǧ��Ҫʹ���ַ������л���ȥ����Ϊ�ַ������л���д�볤��Ȼ���д�����ݣ�����ļ�
		// ��ʽ���󣬿��ܵ������������һ���ܴ�����ݣ����ʱ�򰴳��ȶ�ȡ�ַ���Ҫô�ڴ�ű���Ҫôֱ�ӵ��ļ�β��
		// ����������������ݷ���������ˣ����ҵ��´����ļ��ĸ�ʽʶ��Ҫ���ܳ�ʱ�䣨���Ŷ����ַ����������Ը�Ϊֱ
		// ��д��3���ַ������⣬ʹ��wchar_t����ȷ���ļ�ͷ�ı�ʶͬʱ����unicode��multi-byte�ĳ���
		
		serialize_ar.write(_T("format_flag"), serialize_flag::array_bg);

		wstring suffix = string_tools::CString_to_wstring(data_center::get_data_file_suffix());
		wchar_t ch[5] = L""; // �ļ�ͷд��ʱֻдСд��ʽ
		for (int i = 0; i < (int)suffix.length(); ++i)
			ch[i] = ((int)suffix.length() > i ? suffix[i] : L' ');

		ch[4] = L'\0';

		serialize_ar.write(_T("[0]"), ch[0]);
		serialize_ar.write(_T("[1]"), ch[1]);
		serialize_ar.write(_T("[2]"), ch[2]);
		serialize_ar.write(_T("[3]"), ch[3]);
		serialize_ar.write(_T("format_flag"), serialize_flag::array_ed);

		m_self_data_node->serialize_members(serialize_ar);
	}
	catch (CException* e)
	{
		if (e) e->ReportError();

		mfc_ar.Close();
		file.Close();

		return false;
	}

	mfc_ar.Close();
	file.Close();

	// �������ٴ����־
	m_full_path_name = full_path_name;
	app_ui_interface* app = get_app_ui_interface();
	if (app) app->add_recent_file_list(m_full_path_name);

	return true;
}

bool data_center::save_file()
{
	if (is_closed()) return false;
	if (m_full_path_name.IsEmpty()) return false;
	return save_as_file(m_full_path_name);
}

bool data_center::new_file(const CString& path_name)
{
	if (!m_self_data_node) return false;
	if (is_open()) close_file();

	// ��ʱ��Ҫ����֪ͨ
	{
		data_node_notify_guard tmp(false);

		m_is_open = true;
		m_full_path_name = path_name;

		// ��ʼ�����������ڴ����
		m_self_data_node->initialize_project();
		m_self_data_node->set_default_value();
	}

	// �ȱ��棬���ⴥ�����汣��
	save_file();

	m_self_data_node->notify_if_enable(notifer_action::post_project_loaded);
	return true;
}

void data_center::close_file()
{
	notify_if_enable(m_self_data_node ? m_self_data_node->get_node_tag() : _T(""),
		notifer_action::pre_project_closed);

	// �ر��ļ�����Ҫ�ٴ������ݱ䶯��֪ͨ�ˣ�����ϻ�Ѵ��ڶ��ص��ˣ����·������������
	data_node_notify_guard tmp(false);

	if (!m_self_data_node) return;
	if (is_closed()) return;

	m_is_open = false;
	m_full_path_name = _T("");
	
	m_self_data_node->delete_all_children();
	// �ر���Ŀʱ����ɾ����ǰ���ݽڵ㣬��������ǳ����ʵ�����������ģ����Ǹ�����Ŀ����ʵ���������ģ�ֻ���˳������Ҫɾ��
	// safe_delete(m_self_data_node);
	
	// ����������Դ
	m_current_notify_queue_index = 0;

	lock_guard<mutex> tmp_lock(m_notify_mx);
	m_notify_queue.clear();
}

void data_center::set_data(project_data_base* data)
{
	if (m_self_data_node && m_self_data_node->need_recycle())
		safe_delete(m_self_data_node);

	m_self_data_node = data;
}

void data_center::destroy_data()
{
	// ��������˵���������Ĳ�Ҫ�ˣ�ֻ�����ڳ����˳�ʱ����Ҫ�ٴ������ݱ䶯��֪ͨ��
	data_node_notify_guard tmp(false);
	if (!m_self_data_node) return;

	// �����ӽڵ㣨����ͨ��ɾ����������������������ɵݹ�����������Ϊ��ǰ�ڵ��п����ǲ����Զ����յģ����ӽڵ�ȴ��Ҫ���գ�
	m_self_data_node->delete_all_children();
	if (m_self_data_node->need_recycle())
		safe_delete(m_self_data_node);
	
	m_self_data_node = nullptr;
}

data_node_base* data_center::get_data_root()
{
	return m_self_data_node;
}

data_node_base* data_center::get_data_node(const CString& node_tag)
{
	if (!m_self_data_node) return nullptr;

	if (node_tag == m_self_data_node->get_node_tag())
		return m_self_data_node;
	else
		return m_self_data_node->get_child(node_tag, true);
}

bool data_center::has_node(const data_node_base* src)
{
	if (!src) return false;

	data_node_base* nd = get_data_node(src->get_node_tag());
	return (nullptr != nd);
}

bool data_center::has_node(const CString& tag)
{
	data_node_base* nd = get_data_node(tag);
	return (nullptr != nd);
}

bool data_center::is_node_joined_data_center(const data_node_base* src)
{
	return has_node(src);
}

bool data_center::is_node_joined_data_center(const CString& tag)
{
	return has_node(tag);
}

void data_center::delete_data_node(data_node_base* src, bool force_remove_items/* = false*/)
{
	if (src) src->delete_data_node_self(force_remove_items);
}

void data_center::delete_data_node(const CString& tag, bool force_remove_items/* = false*/)
{
	data_node_base* node = get_data_node(tag);
	if (node) node->delete_data_node_self(force_remove_items);
}

void data_center::notify_if_enable(const CString& tag, notifer_action ac)
{
	if (data_node_notify_interface::is_notify_disabled()) return;

	// ��Щ֪ͨ����post_delete������ʱ�ڵ��Ѿ��������ˣ�Ҫ����֧��,�����Ľڵ㲻����֪ͨ
	data_node_base* nt_node = get_data_node(tag);
	if (notifer_action::post_removed_node != ac
		&& (!nt_node || !nt_node->is_joined_data_center())) return;

	// Ϊ�˱���ѭ��֪ͨ���³�����ѭ�����ظ�֪ͨ��������˴���ԭ����ֱ��֪ͨ��Ϊʹ�ö�������������������ѭ��֪ͨ
	// Ҳ����ͨ������֪ͨ�����Ĳ��������ƣ�������ǰ�������Ѿ������֪ͨ��û���ѭ��֪ͨ�����������Ʋ���������
	// ���Ҫ֪ͨ�Ķ�����û���Ѿ����չ�֪ͨ�����������֪ͨ���������������֪ͨ���ж೤�����տ϶������ظ�֪ͨ��
	// ����Ҳ����©��֪ͨ���ϸ�֪ͨ�Ķ�������Ҫ����ϵͳ��ע������վ������޵ģ���������´���ȫ������֪ͨ�ˣ�
	// ��ʱ֪ͨ��Ҳ��Ϊ��⵽ѭ��֪ͨ������������������ѭ���ķ��ա�
	
	// ���ʹ��std::lock_guardҪע������ķ�Χ�������Ų����٣�ʹ��std::lock_guard�����μ����ķ�ʽ��win7�»�
	// ������ʱ��������,win10�򲻻ᣬ��˸�Ϊstd::unique_lock������һ�£�����ԭ�����飩
	// �Ƚ����߳�ͬ��ȥ������ʱû�б�Ҫ���ǣ���Ϊֻ����֪ͨ������ʱ�Ż�����µ�֪ͨ����˲�����ָ����еĶ���ʧЧ
	// ���±�������������Կ�����ע��
	// std::unique_lock<std::mutex> lk_tmp(m_notify_mx);
	for (const auto& item : m_notify_queue)
		if (0 == tag.CompareNoCase(item.first)) return; // ѭ��֪ͨʱ����֪ͨ�����������ٴ���

	m_notify_queue.push_back(make_pair(tag, ac));
	++m_current_notify_queue_index; // ��ǰ��׼��������һ��

	// lk_tmp.unlock();

	// ������֪ͨ�������ж���֪ͨһ�£��˲��ִ��벻����ǰ��Ļ����������
	// һ����ͨ�������в����µ�֪ͨ���ݹ����˺���
	for (data_node_notify_interface* x : data_center::m_registed_notification_responsing_objects)
		if (x) x->on_project_data_notify(tag, ac);

	// lk_tmp.lock();

	// ��������Ϣ��һ�£���֪ͨ�Ķ���û���ٲ����µ�֪ͨʱ˵������֪ͨȫ����ɣ��������
	if (m_notify_queue.size() == m_current_notify_queue_index) // �Ѿ��������
	{
		m_current_notify_queue_index = 0;
		m_notify_queue.clear();
	}

	// ���ݵı䶯���ܻ������ѡ����ڵı���Ҳ�仯�����Ҫ����һ�ֱ��⣬�˴������ڿ�ܴ��ڵ�notify����ӿ�����
	// ��Ϊ֪ͨ�ǿ��Ա���;������ضϵģ���ܴ��ڲ�һ�����յ������ڴ˴�������϶����ᱻ�ض�
	app_ui_interface* app = get_app_ui_interface();
	if (app) app->update_all_working_space_window_captions();

	// lk_tmp.unlock(); // ���Բ�Ҫ�������������Զ�unlock
}

void data_center::post_notify_if_enable(const CString& tag, notifer_action ac)
{
	notify_post_doing_function* m_post_doing_function = new notify_post_doing_function();
	m_post_doing_function->m_action = ac;
	m_post_doing_function->m_node_tag = tag;

	app_ui_interface* app = get_app_ui_interface();
	if (app) app->do_function_by_post_message(m_post_doing_function);
}

bool data_center::take_snapshot(const CString& tag, const CString& name)
{
	data_node_base* node = get_data_node(tag);
	if (!node) return false;

	return node->take_snapshot(name);
}

bool data_center::restore_snapshot(int step_count /*= 1*/)
{
	if (step_count <= 0 || get_snapshot_count() <= 0) return false;

	// ...
	return false;
}

void data_center::set_max_snapshot_count(int max_count)
{
	// ...
}

void data_center::clear_all_snapshots()
{
	// ...
}

int data_center::get_snapshot_count()
{
	// ...

	return 0;
}

void data_center::get_all_snapshot_names(std::vector<CString>& names)
{
	names.clear();

	// ...
}

project_data_base* data_center::get_project_data_base()
{
	return m_self_data_node;
}


data_mfc_object_can_copy_base::data_mfc_object_can_copy_base()
{

}

data_mfc_object_can_copy_base::data_mfc_object_can_copy_base(const data_mfc_object_can_copy_base& src)
{

}

data_mfc_object_can_copy_base::~data_mfc_object_can_copy_base()
{

}

void data_mfc_object_can_copy_base::operator=(const data_mfc_object_can_copy_base& src)
{

}

data_class_none_copy_base::data_class_none_copy_base()
{

}

data_class_none_copy_base::~data_class_none_copy_base()
{

}
