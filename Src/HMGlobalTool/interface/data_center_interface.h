#ifndef __HMGLOBALTOOL_INTERFACE_DATA_CENTER_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_DATA_CENTER_INTERFACE_H__


#pragma once

#include <set>
#include <mutex>
#include <utility>
#include <deque>

#include "serialize_interface.h"
#include "app_ui_interface.h"
#include "../tools/path_object.h"


class data_center;

// ע�⣬֪ͨ��������ݱ䶯��������Խ��洰�ڱ䶯�����ģ�����ı䶯���û�е������ݱ䶯�ǲ��ᴥ��֪ͨ��
enum class notifer_action
{
	post_project_loaded,		// �������ı����أ��򿪻��½���
	post_project_saved_as,		// �������ı���棨һ��������Ҫ����Ŀ���رյ��������Ӧ��Ŀ�����ˣ���Ϊ��Ŀ����������������ģ�
	pre_project_closed,			// �������ļ������رգ��ر���Ŀ���˳����򣬴���Ϣ������ʹ��post��ʽ������
	post_added_node,			// �������ĵ�ǰ�ڵ�������
	pre_removed_node,			// �������ĵ�ǰ�ڵ�ɾ��ǰ������Ϣ������ʹ��post��ʽ������
	post_removed_node,			// �������ĵ�ǰ�ڵ�ɾ����
	post_updated_node,			// �������ĵ�ǰ�ڵ����ݸ��º�
};


// ���������ӽڵ���Զ���������
// ע�⣬�������ͻ�Ե�ǰ�ڵ��µ������ӽڵ���Ч����������ӽڵ��ǲ���ֱ���ӽڵ�
// ϸ�ڲο��ڵ��need_recycle()����ע������
// �������ͽ��ڻ���ʱ��Ч������� ����ʱ���ܴ�Ӱ�죬�����л������û��Լ�ɾ�������
// nothing_to_do����Ŀǰ�������½ӿ�����Ч(����ο��ӿ�ע������)�������ӿ���Ϊ�ݲ��ܴ�����Ӱ�죺
//   need_recycle();
//   delete_data_node_self();
//   delete_all_children();
//   delete_child_node();
enum class children_recycling_type
{
	nothing_to_do,				// ��������ʱ��Ч����ʱ�����κ�ֱ���ӽڵ���������������Ͳ���������Զ���������Ҳ�����ӽڵ��������Ƴ�Ԫ�أ�����ȫȨ������������������д���
	non_recycle,				// ���������Զ����գ������ӽڵ�ָ���Ļ��շ�ʽ��ʲô�����ԣ����ڵ�������������Ƴ�
	recyclable,					// �ɿ���Զ����ղ������ӽڵ㣬�����ӽڵ�ָ���Ļ��շ�ʽ��ʲô�����Բ��ܣ��ҽ��ӽڵ�������������Ƴ�
	custom,						// ���ӽڵ�ָ���Ļ��շ�ʽ�������ӽڵ㽫���������Ƴ�
};


// ʹ��post��ʽ�첽����֪ͨ
class HM_GlobalTool_EXT notify_post_doing_function : public functor_by_message
{
public:
	notify_post_doing_function();
	virtual ~notify_post_doing_function();

public:
	notifer_action m_action;
	CString m_node_tag;

private:
	virtual void call();
};


// ��Ŀ�ڵ����ݣ�һ��ӿڲ�Ҫ��ʵ���������������������interface��������������Ҫ��̬���������Ҫ֧��mfc��̬������
class HM_GlobalTool_EXT data_node_base : public CObject, public serialize_interface
{
	DECLARE_SERIAL(data_node_base)

public:
	// ���auto_recycleΪtrue����������Ҫɾ���ı��ڵ�ʱ�����delete�����򲻵���
	// children_recyclingΪ�ӽڵ���ջط�ʽ��ע�⣬�������ͻ�Ե�ǰ�ڵ��µ������ӽڵ���Ч����������ӽڵ��ǲ���ֱ���ӽڵ�
	// ������ڵ��ж�����ڵ�ָ�����ӽڵ������ʽ����Ϊ�Զ��巽ʽ�������뱾�ڵ�����ĸ��ڵ�Ϊָ��ԭ�����need_recycle()�ӿ�
	// need_clear_childrenΪtrue��ʾ�����ӽڵ��Ὣ�ӽڵ��������գ����Ϊfalse�����ʾ����
	data_node_base(const CString& node_name = _T(""), bool auto_recycle = true,
		children_recycling_type children_recycling = children_recycling_type::custom);
	
	// ���ڵ����Ϊ�գ��ӽڵ������ϵԭ������,tagҲ�´�������Ҫ������tag����ʹ��copy��ָ��������tag��
	// �߶Ը��ƺ��Ŀ��������rebuild_tag()�����µ�tag
	data_node_base(const data_node_base& src);
	virtual ~data_node_base();

	// ���ݿ�����ע�⿽������ͬtagһ��ԭ�����ƣ����Ҫ������tag����ʹ��copy��ָ��������tag���߶Ը��ƺ��Ŀ
	// ��������rebuild_tag()�����µ�tag
	data_node_base& operator=(const data_node_base& node);
	
	// ����is_equal�麯�����бȽ�
	bool operator==(const data_node_base& node) const;

	// �±������
	const data_node_base* operator[](size_t index) const;
	data_node_base* operator[](size_t index);

	// �������������ֵ���߱���̬�ԣ�����ʱ��ر�֤�����������������һ�£�
	// swap_tagΪtrueʱ�˽ӿڻὫ���������tagҲ����
	void swap(data_node_base& right, bool swap_tag = true);
	static void swap(data_node_base& left, data_node_base& right, bool swap_tag = true);
	
public:
	// ���ڵ��������л���Ĭ��ʵ��Ϊ�����л��������ݣ��ٵ���serialize_children���л��ӽڵ�����
	virtual void serialize_members(member_rw_interface& mt);

	// �����ӽڵ�����л������л����ڵ����ݹ����л���ô˽ӿڽ����ӽڵ�����л����ṩ�˽ӿ��Զ�������Ϊ
	// ����һЩ�������ӵ����ݽڵ㣬�д˽ڵ��ǹ̻����������������ͬ���б�Ҫ����˳����ʽ���ж��ƺͰ汾����
	// �ڲ�Ĭ��ʵ�ְ��ӽڵ�˳���������л�
	virtual void serialize_children(member_rw_interface& mt);

	// ��������ΪĬ��ֵ�������ڵ��������Ҫʱ���ã����½���Ŀ�����ã���ʵ��ʱ��ע���Ƿ�Ҫ���ԭ����
	// Ŀǰ��ܻ�������������ã��������ϸ����û���Ҫ�ֹ����ã�
	// 1.�½���Ŀʱ�����������ĺ�����
	// 2.����Ŀǰ�����
	// 3.�������ɺ���Ҫ��ʼ��ʱ
	virtual void set_default_value();

	// ���ݿ�����ע�⿽������ͬtagһ��ԭ�����ƣ����Ҫ������tag����ʹ��copy��ָ��������tag���߶Ը��ƺ��Ŀ
	// ע�⣺�˽ӿ�ʵ��ʱ���������͵Ķ����Ʋ��õ���=�Ų�����������������ѭ����=�Ų������ڲ�����copy��ɣ�
	// �˽ӿھ߱���̬�ԣ�����ʱ��ر�֤�����������������һ��
	// ���ǵ�ҵ���ܵĸ����ԣ�����tag�ĸ��ƣ�copy_tagΪtrueʱ������tag
	virtual void copy(const data_node_base& node, bool copy_tag = true);

	// �ж������ڵ������Ƿ���ͬ,�ڵ��ʶtag������Ƚϣ����ڵ㲻����Ƚ�
	// compare_childrenΪtrue��ʾ�ӽڵ�Ҳ����Ƚϣ����򲻱��ӽڵ�
	// ע�⣬Ĭ��ʵ�ֲ��ȽϽڵ�Ļ������ò���
	// �˽ӿھ߱���̬�ԣ�����ʱ��ر�֤�����������������һ��
	virtual bool is_equal(const data_node_base& src, bool compare_children = false) const;

public:
	// ��ȡ�����ñ��ڵ�Ļ������ͣ��˲������Ե����ڵ����ã����������ӽڵ㣨����ʱ���ᴥ�����ݸ��µ�֪ͨ��
	// ���auto_recycleΪtrue����������Ҫɾ���ı��ڵ�ʱ�����delete�����򲻵���
	void set_recycling_type(bool auto_recycle);
	bool is_auto_recyclable() const;

	// ��ȡ�����ñ��ڵ���ӽڵ�������ͣ��˲������������ӽڵ����Ч�������Ƿ�Ϊֱ���ӽڵ㣨����ʱ���ᴥ�����ݸ��µ�֪ͨ��
	void set_children_recycling_type(children_recycling_type recycling_type);
	children_recycling_type get_children_recycling_type() const;

	// ��ǰ�ڵ�������ʱ�Ƿ���Ҫdelete���գ������Ҫ��ִ��delete����������Ƴ������������������delete
	// ע�⣺�˽ӿڻ��鸸�ڵ��children_recycling_type��������ڵ�ָ����ǿ�ƻ����ӽڵ㣬�򲻻����ӽڵ㣬��
	// �����ڵ��ָ������ִ�У���ʱ���Ա��ڵ���Զ��������á��������ڵ�ָ�����Զ�����շ�ʽ�Żᰴ�ձ��ڵ�Ļ��շ�
	// ʽ�����˴�˵�ĸ��ڵ�Ϊ���ڵ��ֱ�Ӹ��ڵ�һֱ���ϵ���������һ�����ڵ㣬����ж�����ڵ�������ӽڵ������
	// ��Ϊ���ջ򲻻��գ������Զ��巽ʽ������������ĸ��ڵ�Ϊ�ж�ԭ��
	bool need_recycle() const;

	// ���ݵ�ǰ�ڵ��¡һ����ͬ�Ľڵ㣬�˽ӿڿ�¡�����Ķ����ʶ��ԭ����һ��������ȷ��ȫ��Ψһ��
	// �˽ӿڿ�¡���Ķ����뵱ǰ�������ʵ��������ͬ�������ǰ������data_node_base�������࣬���¡�����Ķ���Ҳ��
	// �����������һ����data_node_base�������캯����������
	// �˹��ܳ����ڲ���Ҫ����������ͷ�ļ�ʵ����������������
	// ���ɵĶ���Ϊ�������󣬼���δ�ҽӵ��������ģ����ɶ�����ڴ���ʹ�������й���
	// ע�⣺��������ע����ȷʵ��DECLARE_SERIAL����
	// ���ǵ�ҵ���ܵĸ����ԣ�����tag�Ŀ�¡��clone_tagΪtrueʱ�����¡tag
	data_node_base* clone(bool clone_tag = false) const;

	// ��ȡ��ǰ�ڵ�tag��ʽ��������ʽ��·��
	// �ڵ�tag��ʽ��·����ʾ�ɽڵ��tag��ɵ�·������'\\'�ָ���
	// �ڵ�������ʽ��·����ʾ�ɽڵ��������ɵ�·������'\\'�ָ���
	// tail_is_folderΪtrue��ʾβ�ڵ㵱���ļ��нڵ㣬�����ʾ�ļ��ڵ�
	CString get_node_full_tag_path() const;
	CString get_node_full_name_path() const;
	path_object get_node_full_tag_path_object(bool tail_is_folder) const;
	path_object get_node_full_name_path_object(bool tail_is_folder) const;
	void get_node_full_tag_path(CString& nd_path) const;
	void get_node_full_name_path(CString& nd_path) const;
	void get_node_full_tag_path(path_object& nd_path, bool tail_is_folder) const;
	void get_node_full_name_path(path_object& nd_path, bool tail_is_folder) const;

	// ���ݽڵ�tag��ʽ��������ʽ��·�����ؽڵ㣬���ĳһ������ͬ���Ľڵ���ֻ�ᰴ��һ�����в��ң��Ҳ����򷵻�nullptr
	// �ڵ�tag��ʽ��·����ʾ�ɽڵ��tag��ɵ�·������'\\'�ָ���
	// �ڵ�������ʽ��·����ʾ�ɽڵ��������ɵ�·������'\\'�ָ���
	// �����·��������ǵ�ǰ�ڵ���߲��ǵ�ǰ�ڵ���ӽڵ�·�����򷵻�nullptr
	// ����ʱ���ӵ�ǰ�ڵ㿪ʼ���²��ң��Ҵ���ڵ��·����ʾ�ӵ�ǰ�ڵ㿪ʼ�Ĳ���·��
	// ע����·�����ҽڵ��Ч��Զ���ڸ��ݵ����ڵ��tag������������Ϊ��·������������·�����ϵĽڵ㣬���ڵ��tag��
	//    ����������������������������Ӧ�������Ȱ�·����ʽ����
	data_node_base* get_node_by_full_tag_path(const CString& nd_path);
	const data_node_base* get_node_by_full_tag_path(const CString& nd_path) const;
	data_node_base* get_node_by_full_name_path(const CString& nd_path);
	const data_node_base* get_node_by_full_name_path(const CString& nd_path) const;
	data_node_base* get_node_by_full_tag_path(const path_object& nd_path);
	const data_node_base* get_node_by_full_tag_path(const path_object& nd_path) const;
	data_node_base* get_node_by_full_name_path(const path_object& nd_path);
	const data_node_base* get_node_by_full_name_path(const path_object& nd_path) const;

	// ���ؽڵ��ʶ
	CString get_node_tag() const;
	
	// ���������µ�tag�滻���ڲ���tagֵ��return_new_tagΪtrue��ʾ���������ɵ�tag��Ϊfalse��ʾ����ԭ����tag
	CString rebuild_node_tag(bool return_new_tag = true);
	
	// ԭ����Ϊȷ�������ȫ��Ψһ��tag���ṩ�ӿڹ��ⲿ�޸ģ������ǵ�ҵ���ܵĸ����ԣ�����tag�Ľӿڣ���Ǳ�Ҫ�����ã�
	void set_node_tag(const CString& tag);

	// �������ڵ��tag�����ᴥ���ڵ������޸ĵ�֪ͨ��������Ҫ���ֹ�������
	void swap_node_tag(data_node_base& node);

	// ���ػ��������ڵ�����ƣ�������һ�����ڽ�����ʾ
	CString get_node_name() const;
	void set_node_name(const CString& name);

	// ���ػ����ýڵ���Ϣ�������û��ⲿ�洢�κνڵ�ע�͡���Ϣ��������Ϣ�����ݺ���Ľ������û�����
	CString get_node_info() const;
	
	// �޸Ľڵ㸽����Ϣ������Ϣ������������û�������Ϣ�洢���޸�����ʱ���Ĭ�ϲ���֪ͨ
	void set_node_info(const CString& info, bool need_notify_if_enable = false);

	// ����һ���ڵ㣬�ڵ�����ڴ��ɱ���ӹ�,��������Ӻ�Ľڵ㣬�Ա����ʹ���ߵı�����
	// ע�⣬�ӽڵ���������κνڵ㲻���������������Ѿ����ڵĽڵ�
	data_node_base* add_child_node(data_node_base* node);

	// ��ָ�������Ľڵ�ǰ/������ӽڵ�
	// ���Ϊ-1��ʾ�����һ��Ԫ����ǰ�������룬���ָ�����±겻���ڣ���ֱ�Ӳ��뵽���һ��Ԫ�غ���
	// ע�⣬�ӽڵ���������κνڵ㲻���������������Ѿ����ڵĽڵ�
	data_node_base* insert_child_node(data_node_base* node, int index = -1, bool is_forward = true);

	// �õ�ֱ�Ӹ��ڵ㣨���������ã��п��ά���������Ҫ�����set_child���ʵ�֣�
	data_node_base* get_parent();
	const data_node_base* get_parent() const;

	// �жϽڵ��Ƿ��Ǳ��ڵ�ĸ��ڵ�,find_all_parentΪtrue��ʾ��ݹ������ң�����ֻ��ֱ�Ӹ��ڵ�
	bool is_parent(data_node_base* node, bool find_all_parent = false);
	bool is_parent(const CString& node_tag, bool find_all_parent = false);

	// �õ����ڵ����������ĸ���������Ѿ����������������ˣ��򷵻صĽڵ���ֱ�Ӵ���������ȡ�õ����ݽڵ���ͬһ�ڵ㣬
	// �����Ϊ��ǰ���ĸ��ڵ㣬��������������û��Լ�������ά�������������������ĵģ�����ô˺����������������
	// ���еĽڵ㣬����ֱ�ӵ����������Ļ�ȡ���ݸ��ڵ㣬��Ϊ����������ȡ��ʱ�临�Ӷ�ΪO(1)�����ӿڵ�ʱ�临�Ӷ�
	// ΪO(n)��
	data_node_base* get_root_node();
	const data_node_base* get_root_node() const;

	// �õ��ӽڵ㣬�ڵ�����Ŀ���ļ��ڲ�ά��
	std::vector<data_node_base*>& get_children();
	std::vector<const data_node_base*> get_children() const; // �˽ӿڲ��ܷ�����������

	// ����tag�õ��ڵ㣬find_all_childrenΪfalse��ʾ���ӱ��ڵ��ֱ���ӽڵ����ң�true��ʾ�ݹ���������ӽڵ�
	// ����������������£��ɿ���ʹ��·����ʽ��ȡ�ڵ㽫�����Ч������μ���Ӧ�ӿ�
	data_node_base* get_child(const CString& node_tag, bool find_all_children = false);
	const data_node_base* get_child(const CString& node_tag, bool find_all_children = false) const;

	// �Ƿ����ӽڵ�
	bool has_children() const;

	// ��û����õ�ǰ�ڵ��ֱ���ӽڵ����,���ý����STL��resize��Ϊһ��,�������ݱ��ֲ��䣬���ɾ���������Ĳ�nullptr
	size_t get_children_count() const;
	void resize_children(size_t sz);

	// ָ�����ӽڵ��Ƿ���ڣ�find_all_childrenΪfalse��ʾ���ӱ��ڵ��ֱ���ӽڵ����ң�true��ʾ�ݹ���������ӽڵ�
	bool is_child_exist_by_tag(const CString& node_tag, bool find_all_children = false);
	bool is_child_exist_by_name(const CString& node_name, bool find_all_children = false);

	// �ӵ�ǰ�ڵ��ֱ���ӽڵ��з���ָ�������Ľڵ�(��0��ʼ)
	data_node_base* get_child(size_t index);
	const data_node_base* get_child(size_t index) const;

	// ��дָ��������Ԫ��
	void set_child(size_t index, data_node_base* node);

	// ֪ͨ����������ĵĵ�ǰ�ڵ㴥������Ϣ�����֪ͨ�����û�ڵ�û�йҽӵ����������ϣ��ǲ��ᴥ��֪ͨ��
	void notify_if_enable(notifer_action ac);

	// ��post��ʽ֪ͨ����������ĵĵ�ǰ�ڵ㴥������Ϣ����Ҫ�����н�����������ʱȷ�����洴���Ļ�������ٴ������£�
	// ��֪ͨ������ʱ���ܽڵ��Ѿ���ɾ���ˣ���ʱ�����ᴥ����������еĲ������������������������ɾ���ڵ�ʱ������
	// ����ʹ����������ֱ�ӷ�post֪ͨ������
	void post_notify_if_enable(notifer_action ac);

	// �жϱ��ڵ��Ƿ��Ѿ��ҽӵ���������
	bool is_joined_data_center();

	// �ͷ�ָ���ڵ�Ŀ���Ȩ������ԭ�ڵ�
	// �ͷ�ʱ��ɾ���ڵ��ڴ棬Ҳ���Ƴ�������nullptr�滻��ԭ�ڵ�
	data_node_base* release_child(size_t index);

	// ɾ������ڵ㼰�����ӽڵ㣨������������ĵĸ��ڵ�Ҳ��ɾ����������֪ͨ
	// ���ӿ��ڲ������ж�������޸��ڵ㣬������������������ٽ���Ӹ��ڵ����Ƴ�����������ͨ
	// ��delete_all_children()�ӿ���ɣ�ϸ�ڿɲο���ע��
	// force_remove_items����ָ����children��������Ϊnothing_to_doʱ��Ч�����Ϊtrue��ʾ����Ҫ���յ�
	// �ڵ���������Ƴ�������������������Ҳ���Ƴ�
	void delete_data_node_self(bool force_remove_items = false);

	// ɾ������������ɾ���ڵ�ʱ��������֪ͨ����ȫɾ���󴥷�һ�θ��ڵ�ĸ���֪ͨ��ɾ���������£�
	// 1.���λ�ȡҪɾ�����ӽڵ�
	// 2.�����ǰ�ڵ�Ϊ���ݸ��ڵ㣬��ֱ��������������(��ʵ�������Ӧ���ǲ�����ͨ���������û�ֱ�ӵ���ڵ�ɾ��������)
	// 3.���Ҫɾ���Ľڵ�ĸ��ڵ���ڣ���ֱ��ͨ�����ڵ����ɾ�������أ�û�ҽӵ���������Ҳ��ɾ����
	// 4.���Ҫɾ���Ľڵ�ĸ��ڵ㲻���ڣ���ֱ��������ڵ㣨û�ҽӵ���������Ҳ��ɾ����
	// ע�⣺�����ǰ�ڵ�Ϊջ�϶���ɾ����������
	//		�����ǰ������ӽڵ�Ϊ���Զ��������ͣ�������������delete����������ӽڵ���Ȼ����ӽڵ�Ԫ�����Ƴ�
	//		�����ӽڵ�ʱҲ�ῼ��ָ��children��������
	// force_remove_items����ָ����children��������Ϊnothing_to_doʱ��Ч�����Ϊtrue��ʾ����Ҫ���յ�
	// �ڵ���������Ƴ�������������������Ҳ���Ƴ�
	void delete_all_children(bool force_remove_items = false);

	// ɾ��һ���ӽڵ㼰��������ӽڵ㣨��������ݵĸ�Ҳɾ���������Ҫɾ���Ľڵ����ӽڵ㣬��һ��ɾ����ɾ���������£�
	// 1.�ȷ���֪ͨ��ָ֮���ڵ㼴����ɾ��
	// 2.�����ǰ�ڵ�Ϊ���ݸ��ڵ㣬��ֱ��������������
	// 3.���Ҫɾ���Ľڵ�ĸ��ڵ���ڣ���ֱ��ͨ�����ڵ����ɾ�������أ�û�ҽӵ���������Ҳ��ɾ����
	// 4.���Ҫɾ���Ľڵ�ĸ��ڵ㲻���ڣ���ֱ��������ڵ㣨û�ҽӵ���������Ҳ��ɾ����
	// ���find_all_childrenΪfalse��ʾ���ӵ�ǰ�ڵ���ӽڵ��в���node���Ҳ����򷵻أ����Ϊtrue��ʾ��ݹ����
	// ��ǰ�ڵ�������������鵽����ҵ��Ľڵ㿪ʼɾ������
	// ע�⣺�Ӹ��ڵ����Ƴ�������ʱ���ܸ��ڵ���ָ����children��������Ӱ�죬�����ӽڵ�ʱҲ�ῼ��ָ��children��������
	// force_remove_items����ָ����children��������Ϊnothing_to_doʱ��Ч�����Ϊtrue��ʾ����Ҫ���յ�
	// �ڵ���������Ƴ�������������������Ҳ���Ƴ�
	void delete_child_node(data_node_base* node, bool find_all_children = false, bool force_remove_items = false);
	void delete_child_node(const CString& tag, bool find_all_children = false, bool force_remove_items = false);
	void delete_child_node(size_t index, bool force_remove_items = false);

	// ���ݵ������Ƴ�һ���ڵ㣬�������Ƴ�ǰ�ĺ�һ��Ԫ�صĵ����������û���򷵻�end()
	// ע�⣬����ĵ������������ӽڵ�ĵ��������Ƴ�ʧ��Ҳ��ر�֤������һ���ڵ�ĵ�����
	std::vector<data_node_base*>::iterator delete_child_node(
		std::vector<data_node_base*>::iterator it, bool force_remove_items = false);

	// ɾ������ڵ�,���ڽ����󴥷�һ�α��ڵ�ĸ���֪ͨ
	void delete_children_nodes(std::vector<data_node_base*>::iterator be,
		std::vector<data_node_base*>::iterator en, bool force_remove_items = false);

	// ����children����ص������������ⲿ���ʣ�����ǰ�ڵ��ֱ���ӽڵ㣬����ݹ���������ӽڵ㣩
	std::vector<data_node_base*>::iterator children_begin();
	std::vector<data_node_base*>::iterator children_end();
	std::vector<data_node_base*>::reverse_iterator children_rbegin();
	std::vector<data_node_base*>::reverse_iterator children_rend();
	std::vector<data_node_base*>::const_iterator children_cbegin();
	std::vector<data_node_base*>::const_iterator children_cend();
	std::vector<data_node_base*>::const_reverse_iterator children_crbegin();
	std::vector<data_node_base*>::const_reverse_iterator children_crend();

	// �ӵ�ǰ�ڵ������������
	// find_all_childrenΪtrue��ʾ�ݹ������������
	// depth��ʾ�ݹ����ȣ�-1��ʾ���޶�ֱ��ȫ�������꣬0��ʾ��������ǰ�ڵ������������0��ʾ������±���n��(�������Ƶ�ǰ�ڵ��)
	// ָ���Ľڵ���ʺ����������true��ʾ��ֹ���������򰴲�����ԭ���������
	void travel_current_node_tree(std::function<bool(data_node_base*)> access_func, int depth = -1);
	void travel_current_node_tree(std::function<bool(const data_node_base*)> access_func, int depth = -1) const;

	// ʵ�ֵ�ǰ�ڵ㼰�ӽڵ�����ݵ��룬�������ܣ���Ҫ�����ڴ����ݵ�ת�������������ã�����ڲ���δ�����쳣��׽������
	// ��Ҫ�����ⲿ���ô˽ӿ�ʱ����ʹ��
	// typeΪ0��ʾ�����Ƹ�ʽ��1��ʾjson��ʽ��2��ʾxml��ʽ
	// path_name�ڲ���������ļ���׺�����������
	// �ɹ�����true�����򷵻�false
	bool import_data(int type, const CString& full_path_name);
	bool export_data(int type, const CString& full_path_name);

	// �Զ��������л���ʽ���롢��������
	bool import_data(const char* value, int length_in_byte); // �ڴ�����ⲿ�������
	bool export_data(char*& value, int& length_in_byte); // �ڲ�����ɾ��value�����ڴ棬�����·��䣬�����ڴ����ⲿ�������

	// �Զ��������л���ʽ���롢��������base64�������ַ������ݣ�������ֱ�ӷ�����Ӧ�Ķ������ڴ������
	// ����base64�����в����ܰ��������ַ���������unicode�����⣬��˲�Ҫʹ��wstring��CString��������Unicodeģ
	// ʽ���˷�˫�����ڴ�ռ䣬���������С�Ҳ��������ڴ�ռ䣬�����ʹ���ַ��������߽�ͨ���ַ���תΪstring����
	// �򽫽��תΪͨ���ַ�������ʹ�ã�����ʹ��CString��ʽ�Ľӿ�
	bool import_data(const std::string& bin_data_base_64);
	bool export_data(std::string& bin_data_base_64);

	// ��json��xml��ʽ����򵼳��ַ�����ʽ�����ݣ�typeΪ0��ʾ�����Ƹ�ʽ��1��ʾjson��ʽ��2��ʾxml��ʽ��Ϊ����ǰ��Ĺ��ɺϷ����أ������������˳��
	// ������Ϊ�����Ʒ�ʽʱ������Ὣ�������ڴ�鰴base64���봦��󲢷����ַ�����ʽ��ǿ�ҽ���û������ԭ��ʱ��Ҫת
	// ��Ϊ�����Ƶ�base����ʱ������������������ʹ��std::string��ʽ�������Ǳ��ӿڣ�Ŀ������unicodeģʽ��
	// ���Խ�ʡһ����ڴ棬����ο���ӿ�ע��
	bool import_data(const CString& value, int type);
	bool export_data(CString& value, int type);

	// �Ե�ǰ���ݽڵ��Ŀ��գ��ɹ�����true�����򷵻�false������Ŀ��ղ����ӿڼ����������ࣩ
	bool take_snapshot(const CString& name);

private:
	CString m_node_tag; // ȫ��Ψһ�Ľڵ��ʶ���������޸ģ������Ҳ������ģ�ֻ�ڶ�����ʱ����
	CString m_name;
	std::vector<data_node_base*> m_children;
	data_node_base* m_parent; // ���游�ڵ�����߱������ٶȺ������
	CString m_node_info; // �ڵ���Ϣ�������û��ⲿ�洢�κνڵ�ע�͡���Ϣ��������Ϣ�����ݺ���Ľ������û�����
	children_recycling_type m_children_recycling_type; // �ӽڵ���ջط�ʽ��ע�⣬�������ͻ�Ե�ǰ�ڵ��µ������ӽڵ���Ч����������ӽڵ��ǲ���ֱ���ӽڵ�
	bool m_auto_recycle; // Ϊtrue����������Ҫɾ���ı��ڵ�ʱ�����delete�����򲻵��ã��˲������Ե����ڵ����ã����������ӽڵ�

private:
	friend class data_center;
	friend class CDBUndoRedoDelDataCommand; // ��ʱΪ�������Լ���undo,redo�ṩ֧�֣�����ʵ��undo,redo���ܺ��ɾ��
	friend class data_object_base; // ��ʱΪ�������Լ���undo,redo�ṩ֧�֣�����ʵ��undo,redo���ܺ��ɾ��
};



// �ṩ�������װ�࣬�Ա������ܷ���Ĺҽӵ���������
// Ҫ��T���ͱ���Ϊ������л�֧�ֵ����ͻ�߱�void serialize_members(member_rw_interface& mt)�ӿڵ��Զ�����
template<typename T>
class data_node_vector_wrapper : public data_node_base
{
public:
	CString m_serialize_name; // ���л�������

public:
	data_node_vector_wrapper(const CString& serialize_name = _T("data_node_vector_wrapper"), const CString& node_name = _T(""),
		bool auto_recycle = true, children_recycling_type children_recycling = children_recycling_type::recyclable)
		: data_node_base(node_name, auto_recycle, children_recycling)
	{
		m_serialize_name = serialize_name;
	}

	virtual ~data_node_vector_wrapper() {}

public:
	virtual void serialize_members(member_rw_interface& mt)
	{
		x_object_flag_serialization_guard tmp_object_flag(m_serialize_name, mt);
		data_node_base::serialize_members(mt);

		int version = 1;

		if (mt.is_storing())
		{
			mt & x_wrapper_macro(version);

			mt & x_wrapper_macro(m_serialize_name);
		}
		else
		{
			mt & x_wrapper_macro(version);

			if (1 == version)
			{
				mt & x_wrapper_macro(m_serialize_name);
			}
		}
	}

public:
	T* get_wrapped_data(size_t index)
	{
		return dynamic_cast<T*>(get_child(index));
	}

	const T* get_wrapped_data(size_t index) const
	{
		return dynamic_cast<const T*>(get_child(index));
	}

	T* get_wrapped_data(const CString& node_tag, bool find_all_children = false)
	{
		return dynamic_cast<T*>(get_child(tag, find_all_children));
	}

	const T* get_wrapped_data(const CString& node_tag, bool find_all_children = false) const
	{
		return dynamic_cast<const T*>(get_child(tag, find_all_children));
	}

public:
	const T* operator[](size_t index) const
	{
		return dynamic_cast<const T*>(get_child(index));
	}

	T* operator[](size_t index)
	{
		return dynamic_cast<T*>(get_child(index));
	}
};

// ���ݰ�װ�࣬Ŀ���Ǳ��ڴ�CObject�����������ֱ�ӿ���CObject��˽�и��ƹ���
class HM_GlobalTool_EXT data_mfc_object_can_copy_base : public CObject
{
public:
	data_mfc_object_can_copy_base();
	data_mfc_object_can_copy_base(const data_mfc_object_can_copy_base& src);
	void operator=(const data_mfc_object_can_copy_base& src);
	virtual ~data_mfc_object_can_copy_base();
};

// ���ݰ�װ�࣬Ŀ�����ṩһ���򵥽ӿ�ֱ�ӽ�ֹ��Ŀ�������������
class HM_GlobalTool_EXT data_class_none_copy_base
{
public:
	data_class_none_copy_base();
	virtual ~data_class_none_copy_base();

private:
	data_class_none_copy_base(const data_class_none_copy_base& src) = delete;
	void operator=(const data_class_none_copy_base& src) = delete;
	void operator=(const data_class_none_copy_base&& src) = delete;
};

// ���ݰ�װ�࣬Ŀ���Ǳ���ֱ���ڴ˻�����д�����ࣨ����̬�࣬�൱��һ������ĵ����ࣩ
class HM_GlobalTool_EXT data_class_toolkit_base
{
private:
	data_class_toolkit_base() = delete;
	~data_class_toolkit_base() = delete;
	data_class_toolkit_base(const data_class_toolkit_base& src) = delete;
	void operator=(const data_class_toolkit_base& src) = delete;
	void operator=(const data_class_toolkit_base&& src) = delete;
};



// ��Ŀ���ݱ䶯ʱ����Ӧ�ӿڣ�ע�������Ŀ���½���Ŀ֮��ᴥ��һ����Ŀ���ض�����tag�������Ǹ�
class HM_GlobalTool_EXT data_node_notify_interface
{
public:
	data_node_notify_interface();
	data_node_notify_interface(const data_node_notify_interface&);
	virtual ~data_node_notify_interface();

public:
	virtual void on_project_data_notify(const CString& node_tag, notifer_action ac) = 0;

public:
	// ���û����֪ͨ����������ã���ر���Ŀ��ʱ������Ϊ���ݱ䶯����ɴ����ȣ�
	static void enable_notify(bool enable);
	static bool is_notify_disabled();
	static bool is_notify_enable();

private:
	static bool m_notify_enable;
};

// �Զ��ػ�����֪ͨ״̬����������ʹ�ô˶���������֪ͨ�Ƿ����ã������ֹ�����֪ͨ�ӿ��еķ���
// ��ʹ��raii�����ڹ��캯��������״̬�����������Զ��ָ�
class HM_GlobalTool_EXT data_node_notify_guard
{
public:
	data_node_notify_guard(bool enable);
	data_node_notify_guard(const data_node_notify_guard&) = delete;
	~data_node_notify_guard();
	
private:
	bool m_old_enable;
};


// �������Ļ���
class HM_GlobalTool_EXT project_data_base : public data_node_base
{
	DECLARE_SERIAL(project_data_base)

public:
	project_data_base(const CString& node_name = _T(""), bool auto_recycle = true,
		children_recycling_type children_recycling = children_recycling_type::custom);

	virtual ~project_data_base();

public:
	// �򿪻��½���Ŀʱ���ã��ر���Ŀ��ʱ��һ������ע����������ģ���һ�����ô˽ӿڣ�
	virtual void initialize_project();

	virtual void serialize_members(member_rw_interface& mt);
};


// ��Ŀ�������ģ����׵���ģʽ���������ƣ��������ⲿ����
class HM_GlobalTool_EXT data_center
{
public:
	// ���ػ��������������ļ�֧�ֵĺ�׺���˴�
	// ���˴�Ĭ��ʹ��|�ָ�������\0������ζ�Ų��������ļ��Ի���ĳ�Ա��ֵ�ķ�ʽ��ֻ��ʹ�ù��캯������Ϊmfc����
	// �ַ�ʽʹ�õķָ�����һ�������б�Ҫ�����е������ú�����������
	static CString get_data_file_filter();
	static void set_data_file_filter(const CString& filter);

	// ���غ�׺���ͣ�������ţ����ؽ������Ϊ4���ַ����ַ�����
	static CString get_data_file_suffix();
	static std::array<TCHAR, 4> get_data_file_suffix_array(); // ע��û�н����������ǺϷ����ַ���
	static void set_data_file_suffix(std::array<TCHAR, 4>& suffix);

	// ���������Ƿ��Ѿ�����Ŀ�ļ�
	static bool is_open();
	static bool is_closed();

	// ���ص�ǰ�򿪵���Ŀ�ļ���·����������׺�������û�д򿪣��򷵻�_T("")
	static CString get_data_file_full_path_name();
	
	// ��ȡ��Ŀ�ļ���������·����include_suffixΪtrue��ʾ����׺����
	static CString get_data_file_name(bool include_suffix = false);

	// ������Ŀ�ļ����׵Ĺ����ļ���·����end_with_backslashedΪtrue��ʾ����б�ܽ��������򲻴�
	// ע�⣬�˽ӿڷ��ص����������ļ���·�����ҽ�����Ҫʱ�Ŵ������õ�·����Ҫ�ٶ����Ѿ����ڣ���
	// ��û�д򿪵���Ŀ�򷵻ؿգ����ӿڲ����ж�·���Ƿ���Ч
	static CString get_project_working_folder_path(bool end_with_backslashed = true);
	
	// ���ļ����ɹ�����true��ʧ�ܷ���false
	// ����Ѿ�����Ŀ�򿪣�����Զ��رգ�Ȼ���ٴ�
	static bool open_file(const CString& path_name);

	// ���
	static bool save_as_file(const CString& path_name);

	// ����
	static bool save_file();

	// �½�һ����Ŀ�ļ����½���ʱ�����ָ��·����
	static bool new_file(const CString& path_name);

	// �ر��ļ�
	static void close_file();

	// ���������������ݣ���������ڶ��Ϸ��䣬���ڴ����������Ľӹܣ�ԭ����������delete
	// ����Ϊnullptr���൱�ڵ���destory_data()
	// ������õ��������ĵ����ݲ����ڱ�ģ�飬��Ӧ��������ģ��dllж�ص�ʱ���ֹ�����nullptr�����ٶ����Լ�ģ��ж�ص����������ĵĶ���ʧЧ
	// Ҫע����������ǰ������ȷʵ���������ĵ�set_default_value�麯������������ݵ���ʼ����״̬��������Ӻ������
	// ����ȣ�����ܻ����½���Ŀ���ǰ����
	static void set_data(project_data_base* data);

	// ���ٽӹܵ��������Ķ��󣨶�������ڶ��Ϸ��䣩
	// ��������˵���������Ĳ�Ҫ�ˣ�ֻ�����ڳ����˳�ʱ�������ٴ������ݱ䶯��֪ͨ��
	static void destroy_data();

	// �õ����������������ݶ���ĸ��ڵ�
	static data_node_base* get_data_root();
	static project_data_base* get_project_data_base();

	// �õ���������ָ���ڵ�
	static data_node_base* get_data_node(const CString& node_tag);

	// �������������û��ָ���Ľڵ�
	static bool has_node(const data_node_base* src);
	static bool has_node(const CString& tag);

	// �ж�һ���ڵ��Ƿ��Ѿ��ҽӵ���������
	static bool is_node_joined_data_center(const data_node_base* src);
	static bool is_node_joined_data_center(const CString& tag);

	// ɾ��һ���ڵ㣨��������ݵĸ�Ҳɾ������ɾ���������£�
	// 1.�ȷ���֪ͨ��ָ֮���ڵ㼴����ɾ��
	// 2.�����ǰ�ڵ�Ϊ���ݸ��ڵ㣬��ֱ��������������
	// 3.���Ҫɾ���Ľڵ�ĸ��ڵ���ڣ���ֱ��ͨ�����ڵ����ɾ�������أ�û�ҽӵ���������Ҳ��ɾ����
	// 4.���Ҫɾ���Ľڵ�ĸ��ڵ㲻���ڣ���ֱ��������ڵ㣨û�ҽӵ���������Ҳ��ɾ����
	// force_remove_items����ָ����children��������Ϊnothing_to_doʱ��Ч�����Ϊtrue��ʾ����Ҫ���յ�
	// �ڵ���������Ƴ�������������������Ҳ���Ƴ�
	static void delete_data_node(data_node_base* src, bool force_remove_items = false);
	static void delete_data_node(const CString& tag, bool force_remove_items = false);

	// ͨ����������ֱ�Ӵ���ָ��tag��֪ͨ�����½ڵ㲻����Ҳ���Դ���
	static void notify_if_enable(const CString& tag, notifer_action ac);
	static void post_notify_if_enable(const CString& tag, notifer_action ac);

	// �������ĵĳ������ָ��ӿڣ��ɹ�����true�����򷵻�false
	// tagΪ�ջ��������ĸ�tag������ʾ���������Ľ��п��գ���ʱ�ڴ����Ĵ�Ӧ����ȷ��Ҫ���յĽڵ�����С
	static bool take_snapshot(const CString& tag, const CString& name);
	static bool restore_snapshot(int step_count = 1);
	static void set_max_snapshot_count(int max_count); // Ĭ��15��
	static void clear_all_snapshots();
	static int get_snapshot_count(); // �޷��鵽ʱ����0
	static void get_all_snapshot_names(std::vector<CString>& names); // ����ĵĿ����ڵ�0��Ԫ�أ����������

private:
	static CString m_file_filter;
	static std::array<TCHAR, 4> m_file_suffix;
	static CString m_full_path_name; // ��ǰ��Ŀ�Ĵ洢·�������Ϊ�ձ�ʾδ�����
	static bool m_is_open;
	static project_data_base* m_self_data_node; // ע����������ı���Ϊ���Ϸ����

	// �Ѿ�ע�������Ӧ����֪ͨ��������Ӧ���󣬶�����ڴ��������Ĳ��������!!!ʹ�����Լ�����ά��
	static std::set<data_node_notify_interface*> m_registed_notification_responsing_objects;

	static std::mutex m_notify_mx;
	static int m_current_notify_queue_index;
	static std::deque<std::pair<CString, notifer_action>> m_notify_queue;

private:
	// ���ӿ��౾��ɷ��ʣ��������಻����Ԫ��
	friend class data_node_notify_interface;
	friend class data_node_base;
	friend class project_data_base;

private:
	data_center() = delete;
	data_center(const data_center&) = delete;
	~data_center() = delete;
};


// ����������ע��ҵ��������,��ע��ʱ��Ҫ�����ţ��Ա������ʹ��ʱ�����ţ���ʽ�ϱ����뺯������һ��
// Ҫע����������ǰ������ȷʵ���������ĵ�set_default_value�麯������������ݵ���ʼ����״̬��������Ӻ������
// ����ȣ�����ܻ����½���Ŀ���ǰ����
#define register_data_center_node(class_name) data_center::set_data(new class_name())
#define unregister_data_center_node data_center::destroy_data

#endif //__HMGLOBALTOOL_INTERFACE_DATA_CENTER_INTERFACE_H__

