#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_PATH_OBJECT_H__
#define __HMGLOBALTOOL_TOOLS_PATH_OBJECT_H__

#include <utility>
#include <vector>


// ��ʾ·�������е�һ��·���ڵ�
class X_FRAMEWORK_API path_object
{
public:
	path_object(const CString& name = _T(""), bool is_folder = true);
	path_object(const CString& path_text, const CString& split);
	path_object(const std::vector<CString>& all_nodes_name, bool last_node_is_folder = true); // �ο�ԭ����ͬ��reset��Ա
	path_object(const std::vector<std::pair<CString, bool>>& all_nodes); // �ο�ԭ����ͬ��reset��Ա
	path_object(const path_object& src);
	virtual ~path_object();

public:
	path_object& operator=(const path_object& src);
	bool operator==(const path_object& src);

public:
	//�Ƿ����ļ���
	bool is_folder() const;

	//�Ƿ����ļ�
	bool is_file() const;

	//�õ��ӽڵ�򸸽ڵ��ָ��
	path_object* get_child();
	const path_object* get_child() const;
	path_object* get_parent();
	const path_object* get_parent() const;

	// ������Ϊ�ļ��������ж��Ƿ����ӽڵ㣬��������������ӽڵ�
	void set_as_folder();
	void set_as_file();

	// �жϴ����·����������·���ǲ��ǵ�ǰ�ڵ����·������ǰ�ڵ���ȡ����ڸ���ȫ·�������봫��·�����
	bool is_child_path(const path_object* src) const;
	
	// �жϴ����·����������·���ǲ����뵱ǰ�ڵ��·����ͬ����ǰ�ڵ���ȡ����ڸ���ȫ·�������봫��·�����
	bool is_self_path(const path_object* src) const;

	// �жϴ����·����������·���ǲ����뵱ǰ�ڵ��·����ͬ��������·������ǰ�ڵ���ȡ����ڸ���ȫ·�������봫��·�����
	bool is_self_or_child_path(const path_object* src) const;

	// ������Ϊ�ļ��ڵ㣬�����ж��Ƿ����ӽڵ㣬��������������ӽڵ�
	void set_node_data(const CString& name, bool is_folder);
	void get_node_data(CString& name, bool& is_folder) const;

	// �õ������ýڵ��ļ���
	CString get_node_name() const;
	void set_node_name(const CString& name);

	// ��ȡ�洢��·��������·���ַ�����Ĭ����\�ָҲ�ɰ�ָ���ָ����ָ�
	// end_with_split��ʾβ���Զ��Ӵ���һ��split�ָ���������Ҫ
	CString to_path_string(bool end_with_split, const CString& split = _T("\\")) const;
	void to_path_string(CString& path, bool end_with_split, const CString& split = _T("\\")) const;

	// �õ������ڵ�����ƣ���ǰ�ڵ���Ϊ���ڵ��0��Ԫ�أ���������
	std::vector<CString> get_all_node_name() const;
	void get_all_node_name(std::vector<CString>& result) const;

	// �õ������ڵ�����ƣ���ǰ�ڵ���Ϊ���ڵ��0��Ԫ�أ���������first��ʾ�ڵ�����second��ʾ�Ƿ�Ϊ�ļ��нڵ�
	std::vector<std::pair<CString, bool>> get_all_nodes() const;
	void get_all_nodes(std::vector<std::pair<CString, bool>>& result) const;

	//�����ṩ��·���ַ�������·����,Ĭ����\�ָ����������ʾ�ṩ�ڶ�������
	void reset(const CString& path_text, const CString& split = _T("\\"));

	// ����ָ���ĸ����ڵ㹹��·������0��Ԫ��Ϊ������������
	// last_node_is_folderΪtrue��ʾ���һ���ڵ㵱���ļ��д����������ļ�����
	void reset(const std::vector<CString>& all_items_name, bool last_node_is_folder = true);

	// ����ָ���ĸ����ڵ㹹��·������0��Ԫ��Ϊ������������first��ʾ�ڵ�����second��ʾ�Ƿ�Ϊ�ļ��нڵ�
	void reset(const std::vector<std::pair<CString, bool>>& all_items);

	// ����·���õ�ָ���Ľڵ�
	path_object* get_node(const CString& path_text, const CString& split = _T("\\"));

	// ����·���õ�ָ���Ľڵ㣬0��Ԫ�ر�ʾ���ڵ�����
	path_object* get_node(const std::vector<CString>& path_node);

	// �Ƴ�ָ���Ľڵ�
	void remove_node(const CString& path_text, const CString& split = _T("\\"));

	// �Ƴ�ָ���Ľڵ㣬0��Ԫ��Ϊ������������
	void remove_node(const std::vector<CString>& path_node);

	// �Ƴ�ָ���Ľڵ㣬���ָ���ڵ㲻�����Ľڵ��򲻴���
	void remove_node(path_object* node);

	// �ж�ָ���Ľڵ��Ƿ������ϵĽڵ�
	bool is_in_path_node(const CString& path_text, const CString& split = _T("\\")) const;

	// �ж�ָ���Ľڵ��Ƿ������ϵĽڵ㣬0��Ԫ��Ϊ������������
	bool is_in_path_node(const std::vector<CString>& path_node_name, bool last_node_is_folder = true) const;

	// �ж�ָ���Ľڵ��Ƿ������ϵĽڵ㣬���ָ���ڵ㲻�����Ľڵ��򲻴���
	bool is_in_path_node(path_object* node) const;

	// ��ָ���ڵ㴦�����½ڵ�,is_backΪtrue��ʾ��ָ���Ľڵ�ǰ���룬������ָ���Ľڵ�������
	// ���ָ���Ľڵ㲻���ڣ��򲻴���
	void insert_node(const CString& file_name, const CString& path_text, const CString& split = _T("\\"), bool is_back = true);

	// ��ָ���ڵ㴦�����½ڵ�,is_backΪtrue��ʾ��ָ���Ľڵ�ǰ���룬������ָ���Ľڵ�������
	// ���ָ���Ľڵ㲻���ڣ��򲻴���
	void insert_node(const CString& file_name, const std::vector<CString>& path_node, bool is_back = true);

	// ��ָ���ڵ㴦�����½ڵ�,is_backΪtrue��ʾ��ָ���Ľڵ�ǰ���룬������ָ���Ľڵ�������
	// ���ָ���Ľڵ㲻���ڣ��򲻴���
	// ע������Ľڵ�ֻ���Ƕ��ϵĶ����Ҳ���Ϊ����ڵ�
	void insert_node(const CString& file_name, path_object* node, bool is_back = true);

	// �����������һ���ڵ㣬�����µ�β�ڵ㣨����ʧ�ܷ���nullptr)
	path_object* push_back(const CString& node_text, bool is_folder);
	path_object* push_back(path_object* node); // ֻ��Ϊһ���ڵ㣬��������ڶ��Ϸ����ҽ��ɱ������

	// ����ǰ������һ���ڵ㲢����ͷ�ڵ㣨����ʧ�ܷ���nullptr)
	// �ӿ��ڻ�ȷ��ͷ�ڵ��ַ���ֲ��䣬����ǰ��ͷ�ڵ�ṹ���岻�䣬�����ݻ��Ϊ�²���Ľڵ�����
	path_object* push_front(const CString& node_text, bool is_folder);
	path_object* push_front(path_object* node); // ֻ��Ϊһ���ڵ㣬��������ڶ��Ϸ����ҽ��ɱ������

	// ����һ��·����(������һ����ڵ��·����)���ӵ���ǰ���ĺ��棬��������ڶ��Ϸ����ҽ��ɱ������
	void conect_path_object(path_object* path_obj);

	// �õ����ڵ�
	path_object* get_root();
	const path_object* get_root() const;

	// �õ�ĩβ�ڵ�
	path_object* get_tail();
	const path_object* get_tail() const;

	// �ڶ��Ϲ����ӽڵ����
	path_object* make_child(const CString& name, bool is_folder = true);
	
	// ����ӽڵ�
	void clear();                  

protected:
	//�ڲ�ʹ�ã��õ������ӽڵ�
	std::vector<path_object*> get_all_children() const;

protected:
	CString m_name = _T("");
	path_object* m_child = nullptr;
	path_object* m_parent = nullptr;
	bool m_is_folder = true;
};

#endif //__HMGLOBALTOOL_TOOLS_PATH_OBJECT_H__
