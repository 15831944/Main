#pragma once


// ͼֽͼ�λ��༭���ڹ۲��߻���
class UI_MANAGER_API graph_graphic_ui_observer_base : public CObject
{
	DECLARE_SERIAL(graph_graphic_ui_observer_base)

public:
	graph_graphic_ui_observer_base();
	virtual ~graph_graphic_ui_observer_base();

public:
	virtual void save_data_from_ui();
	virtual void load_data_to_ui();

public:
	// ͼֽ���ݵ�tag�����ڴ�����������ȡͼֽ����
	CString m_graph_data_tag;
};
