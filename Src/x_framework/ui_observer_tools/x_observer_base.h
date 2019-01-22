#pragma once

#include <map>
#include <vector>
#include "../interface/data_center_interface.h"


class x_mvc_editor_base;


class X_FRAMEWORK_API x_observer_base : public CObject
{
	DECLARE_SERIAL(x_observer_base)

public:
	x_observer_base(x_mvc_editor_base* wnd = nullptr, const CString& node_tag = _T(""));
	virtual ~x_observer_base();

public:
	virtual void project_data_notify(const CString& node_tag, notifer_action ac);
	virtual CString get_title();

public:
	const CString& get_data_node_tag() const;
	x_mvc_editor_base* get_owner_window();

	// �����Ľṹ�������������б仯ʱ���Ǵ������£����������л��ƴ���
	// �������ݵ�ʱ���ô�������Ѿ�����
	void update_observer_ui();
	void update_window_title();

private:
	// ���ݺʹ���ֻ���ɴ���ʱ���༭����ָ����observerֻ�з���Ȩ
	CString m_data_node_tag;			// ������������tag
	x_mvc_editor_base* m_owner_window;

private:
	friend class x_mvc_editor_base;

private:
	// ������ض���������
	x_observer_base(x_observer_base&);
	x_observer_base(x_observer_base&&);
	void operator=(const x_observer_base&);
	void operator=(const x_observer_base&&);
};

