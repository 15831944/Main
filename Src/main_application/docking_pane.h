#pragma once


// һ����壨һ�����ض���һ�������У�
class single_pane_data : public CObject
{
	DECLARE_SERIAL(single_pane_data)

public:
	single_pane_data();
	single_pane_data(const single_pane_data& src);
	virtual ~single_pane_data();

public:
	single_pane_data& operator=(const single_pane_data& control);

public:
	// ��xml�����м������ò�������ǰ��node���ڵ�Ϊcontainer
	void init_single_pane_data(tinyxml2::XMLElement* node);

public:
	int					m_id; // ���id���ɴ���ʱ�Զ�����
	CString				m_pane_title;
	CString				m_tab_title;
	bool				m_start_load;		// ����ʱ�Ƿ����
	bool				m_no_close_button;	// ���������������޹رհ�ť
	bool				m_no_caption;		// ���������������ޱ�����-->
	bool				m_no_dockable;		// �������������ܷ�ͣ��-->
	bool				m_no_hideable;		// �������������������ذ�ť-->
	bool				m_select;
	CString				m_icon_file;
	CString				m_child_window_class;
	int					m_child_window_id;	// ������Ӵ���id���ɴ���ʱ�Զ�����
	CString				m_app_identify;		// ����Ӧ�ó����ڲ���ʶĳ���������һ��Ϊ�գ�ĿǰΪproperty_wnd��ʾ�������Ա���壬Ϊcommand_line��ʾ�����������
};

// һ���������
class pane_container_data : public CObject
{
	DECLARE_SERIAL(pane_container_data)

public:
	pane_container_data();
	pane_container_data(const pane_container_data& src);
	virtual ~pane_container_data();

public:
	pane_container_data& operator=(const pane_container_data& control);

public:
	// ��xml�����м������ò�������ǰ��node���ڵ�Ϊcontainer
	void init_pane_container_data(tinyxml2::XMLElement* node);

public:
	CString				m_direction_by_frame;
	CString				m_app_identify;
	CString				m_neighbor_identify;
	int					m_width;
	int					m_height;
	bool				m_show;
	std::vector<single_pane_data> m_panes;
};

// ͣ�������������
class docking_pane_data: public CObject
{
	DECLARE_SERIAL(docking_pane_data)

public:
	docking_pane_data();
	docking_pane_data(const docking_pane_data& src);
	virtual ~docking_pane_data();

public:
	docking_pane_data& operator=(const docking_pane_data& control);

public:
	// ��xml�����м������ò�������ǰ��node���ڵ�Ϊdb_docking_panes
	void init_docking_pane_data(tinyxml2::XMLElement* node);

	// ��ȡָ��id������������ݣ�pane�Ѿ�����������id�Ѿ�����ú��ٵ��ò������壩
	single_pane_data* get_pane_data_by_pane_wnd_id(int id);

public:
	CString m_version;
	bool m_force_create_by_frame;
	bool m_auto_layout;
	std::vector<pane_container_data> m_container;
};

