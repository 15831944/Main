#pragma once

class CXTPRibbonTab;


typedef struct tag_ribbon_control_id_and_icon_id
{
	int			id;			// ����ID
	int			icon_id;	// -1��ʾ��Ч
} ribbon_control_id_and_icon_id;


class option_info
{
public:
	option_info();
	~option_info();

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊoption_info
	void initlize_config_data(tinyxml2::XMLElement* node);

public:
	CString  m_about_dialog_class_name;
};


class ribbon_bar_data_quick_access_button
{
public:
	ribbon_bar_data_quick_access_button();
	~ribbon_bar_data_quick_access_button();

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊquick_access_button
	void initlize_config_data(tinyxml2::XMLElement* node);

	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

public:
	int					m_id;			// ID
	bool				m_is_hide;		// �Ƿ�����
	CString				m_caption;		// ����
	CString				m_tool_tip;		// ��ʾ��Ϣ	
	int					m_icon_id;		// ͼ��ID��ÿһ��ѡ��µ�����ͼ��ID������ͬ�������ͬ��ͼ��һ����
	CString				m_acc_key;		// ����ѡ���
	CString				m_cmd;			// �ÿؼ���Ҫִ�е����Ϊ�ձ�ʾ��ִ���κβ���
	CString				m_cmd_params;   // �������
};


class ribbon_quick_access_bar
{
public:
	ribbon_quick_access_bar();
	~ribbon_quick_access_bar();

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊquick_access
	void initlize_config_data(tinyxml2::XMLElement* node);

	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

private:
	//����Աm_quick_access_bar���Ƿ������id
	bool has_this_id(UINT id)const;

public:
	std::vector<ribbon_bar_data_quick_access_button> m_all_buttons;
	CString m_file_name;
	bool	m_image_has_alpha;
};


class ribbon_bar_data_file_menu_item
{
public:
	ribbon_bar_data_file_menu_item();
	~ribbon_bar_data_file_menu_item();

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊfile_menu_item
	void initlize_config_data(tinyxml2::XMLElement* node) ;

	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

public:
	int					m_id;			// ID
	CString				m_caption;		// ����
	int					m_icon_id;		// ͼ��ID��ÿһ��ѡ��µ�����ͼ��ID������ͬ�������ͬ��ͼ��һ����
	CString				m_acc_key;		// ����ѡ���
	CString				m_cmd;			// �ÿؼ���Ҫִ�е����Ϊ�ձ�ʾ��ִ���κβ���
	CString				m_cmd_params;	// �������
};


class ribbon_file_menu_bar
{
public:
	ribbon_file_menu_bar();
	~ribbon_file_menu_bar();

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊfile_menu
	void initlize_config_data(tinyxml2::XMLElement* node) ;

	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

public:
	std::vector<ribbon_bar_data_file_menu_item> m_menu_items;
	CString										m_image_file_name;
	bool										m_image_has_alpha;
};


//////////////////////////////////////////////////////////////////////////
// һ���ؼ�������Ļ���,��ͬ�ؼ����������������������
// ����ֱ��ʹ������࣬�ڲ���ʼ�������ݿ��ܲ�������һ�����������Ŀؼ�
class ribbon_bar_data_controlbase : public CObject
{
	DECLARE_SERIAL(ribbon_bar_data_controlbase)

public:
	ribbon_bar_data_controlbase();
	ribbon_bar_data_controlbase(const ribbon_bar_data_controlbase& control);
	virtual ~ribbon_bar_data_controlbase();

public:
	ribbon_bar_data_controlbase* operator=(const ribbon_bar_data_controlbase& control);

public:
	// ����������������һ������ؼ�,�ڴ����ⲿ�����߹���
	virtual void init_ribbon_control(CXTPControl* control);
	
	// �ؼ����ݵĻ��࣬��������ΪxtpControlError�������෵����Ӧ����ȷ����
	virtual XTPControlType get_ribbon_control_type() const;
	
	// ���ڵ�Ϊĳ��control.���ݴ���Ŀؼ�����Ӧ��xml�ڵ����ó�ʼ����������
	virtual void init_ribbon_control_data(tinyxml2::XMLElement* node);
	
	// �Ӷ���ʹ��new������������󲢿������ݺ󷵻�������ָ�룬���ɵĶ�����ռ�ڴ����ⲿ���õĵط�
	// �����������Լ������Լ��Ķ���ʹ�ÿ��������������ݲ����ػ���ָ�룬��ò�Ҫ���û����Ľӿ�
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
	
	// ���ر��ؼ��������Ŀؼ�ID��ͼ��ID�������Ϊ1������Щ����ģ���������˵��ģ���Ҫ��������˵���
	// ��ĸ�����,������������Լ��¼ӵģ�Ȼ���ټ�������еģ����շ��������ؼ��õ���ID
	virtual std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;

	// ��start_id��ʼ���ÿؼ�ID�������ڲ����һ�����õ�IDֵ�ټ�1��������ֻҪ�����Լ��¼ӵ�IDֵ��Ȼ��
	// �ٵ��û���ľͿ�����
	virtual int set_control_id(int start_id);

	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

	// ��ȡ��ǰ�ؼ��������ӿؼ�������еĻ����еĿؼ�id�Լ���Ӧ������
	virtual void get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const;
	
public:
	// �ؼ�ID��Χ[20000,28999]�����԰����߽磬����Ribbon�в���
	// �ظ�����Χ�ɺ� IDS_RIBBON_UI_MIN �� IDS_RIBBON_UI_MAX �޶�����Ҫȥ���Ѿ�ռ�õ�29000��29999,
	// ����ֻ����20000��28999ֵ֮�䣬������������ͬһ�����ϵĲ˵������������е�ID����ͬ
	// �ڲ�ID�ɳ���ͳһ��������xml�ļ���ȡ
	int					m_id;

	CString				m_caption;
	XTPButtonStyle		m_style;
	int					m_icon_id;		// ͼ��ID��ÿһ��ѡ��µ�����ͼ��ID������ͬ�������ͬ��ͼ��һ����
	CString				m_tool_tip;
	CString				m_acc_key;		// ����ѡ���
	CString				m_command;		// �ÿؼ���Ҫִ�е����Ϊ�ձ�ʾ��ִ���κβ���
	CString				m_cmd_params;	// �������
};


// һ������
class ribbon_bar_data_group
{
public:
	ribbon_bar_data_group();
	ribbon_bar_data_group(const ribbon_bar_data_group& group);
	~ribbon_bar_data_group();

public:
	ribbon_bar_data_group* operator=(const ribbon_bar_data_group& group);
	void InitRibbonGroupData(tinyxml2::XMLElement* node); // ���ڵ�Ϊĳ��group
	int GetMaxIconIndex() const; // ���ص�ǰtab������ͼ��IDֵ(�Ϸ�ͼ��ID���ڵ���0),����-1��ʾ�޷���ȡ
	std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;

public:
	CString				m_group_caption; // ������⣨�ڷ����±ߣ�
	std::vector<ribbon_bar_data_controlbase*> m_control_in_group; // ������ֻ������new������Ŀռ䣬���ڴ����֮�������ڲ�����
};


// һ��ѡ�
class ribbon_bar_data_tab
{
public:
	ribbon_bar_data_tab();
	~ribbon_bar_data_tab();

public:
	void InitRibbonTabData(tinyxml2::XMLElement* node); // ���ڵ�Ϊĳ��tab
	int GetMaxIconIndex() const; // ���ص�ǰtab������ͼ��IDֵ(�Ϸ�ͼ��ID���ڵ���0),����-1��ʾ�޷���ȡ
	std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;

public:
	CString		m_tab_caption;
	CString		m_tab_acc_key;
	
	// ÿ��ѡ��µ����пؼ���Ӧһ��ͼ���ļ����ļ���ʽΪpng��ͼ��ߴ�32*32,��ֻ����һ�ţ�ͼƬ����
	// �����뵱ǰdllģ��ͬһĿ¼���ļ��С�image����
	CString		m_icon_file_name;

	// ͼ���Ƿ���alpahͨ����Ϣ��32bitλͼ��pngһ�㸳true��png����û��alphaͨ���������ֵ���ò���ȷ���ᵼ��ͼ���аױ߻��޷�ʶ��ͼ��
	bool		m_image_has_alpha;

	// tab�в�ָ��context����ָ��Ϊ���ַ�������ʾ��ǩΪ�̶��ģ������ʾ��ǩ����������صģ���������
	// ��ͬ�ı�ǩ���Ϊһ����ǩ�飬�Ա㶯̬����ʾ������
	CString		m_context_name;
	
	// tab�е��������ַ�����Ϊ��ʱ��context_color��ʾ�����ĵı�ǩ��ɫ
	XTPRibbonTabContextColor m_context_color;
	
	// tab��active_view_class����ָ�����ǩ�������Ĵ���������ֻҪ��������ǰ����ڵ�������֮ƥ��ͻ��Զ���������Զ�����
	CString		m_active_view_class;

	// ��Ϊ�����ı�ǩ�ҹ�������ʱ����ֵΪtrue���Զ�����tab��Ϊfalse�򲻼����ʱ�����û��Լ����Ĭ��Ϊfalse
	bool		m_context_auto_active;
	
	std::vector<ribbon_bar_data_group> m_all_groups;
};


// һ��Ribbon������󲼾�����
class ribbon_bar_config_data
{
public:
	ribbon_bar_config_data();
	~ribbon_bar_config_data();

public:
	// ��ȡ��ǰ�ؼ��������ӿؼ�������еĻ����еĿؼ�id�Լ���Ӧ������
	virtual void get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const;

public:
	// ��xml�����м���ribbon���ò�������ǰ��node���ڵ�Ϊdb_ribbon
	void initlize_config_data(tinyxml2::XMLElement* node);
	
	// �������� id ��ȡ Ribbon �ؼ�����Ҫִ�е�������Ϣ
	// ��һ������Ϊ�򣬵ڶ�������Ϊ�������������Ϊ����Ĳ���
	// �����������򷵻�false�����򷵻�true
	bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

	// �������������ȡ��Ӧ������id, �Ҳ����򷵻�-1
	// ע�⣺
	//   1.�������еĿؼ����������Ӧ��û������Ŀؼ���ʾ���߱���Ϊ������ؼ�û�����塣
	//   2.��һ��ؼ���Ϊ���ƣ�ʹ����ͬ�����ָ����ͬ�Ĳ�����ɶ����Ĺ���������������Ҫ������ҵĲ�����
	//     ��compare_param��Ϊtrue�����ܻ��ڲ��ҿؼ�ʱ��ͬ����Ҳ���бȽϣ�����������ıȽϲ����ִ�Сд��
	//   3.��Щ�ؼ��ǲ�����ָ�������ģ�ָ����Ҳ��Ч����combo�ؼ��������ɿ�ܾ����������û����ã�����μ�ribbon��
	//     xml�����ļ����ݣ������ֿؼ�����ʹ�ò�ͬ�����������ʹ�ò�����֮����
	int get_id(const CString& domain, const CString& cmd, const CString& param = _T(""), bool compare_param = false);

	// ����ָ���Ĵ����������������й����������ı�ǩ�е���������
	void get_all_command_items_of_special_window_class_in_context_tab(std::vector<CString>& cmd_list, const CString& window_class);

public:
	ribbon_quick_access_bar          m_quick_access;
	ribbon_file_menu_bar			 m_file_menu;
	std::vector<ribbon_bar_data_tab> m_all_tabs;
	option_info						 m_dialog_info;
};


//////////////////////////////////////////////////////////////////////////
// ����Ϊ��ǰ�õ�������Ribbon����ؼ����Ժ����µ���������������

// һ�������ť
class ribbon_bar_data_button : public ribbon_bar_data_controlbase
{
	DECLARE_SERIAL(ribbon_bar_data_button)

public:
	ribbon_bar_data_button();
	virtual ~ribbon_bar_data_button();

public:
	virtual void init_ribbon_control(CXTPControl* control);
	virtual XTPControlType get_ribbon_control_type() const;
	virtual void init_ribbon_control_data(tinyxml2::XMLElement* node);
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
	virtual std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;
	virtual int set_control_id(int start_id);
};

// һ����ѡ��
class ribbon_bar_data_check_box : public ribbon_bar_data_controlbase
{
	DECLARE_SERIAL(ribbon_bar_data_check_box)

public:
	ribbon_bar_data_check_box();
	virtual ~ribbon_bar_data_check_box();

public:
	virtual void init_ribbon_control(CXTPControl* control);
	virtual XTPControlType get_ribbon_control_type() const;
	virtual void init_ribbon_control_data(tinyxml2::XMLElement* node);
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
	virtual std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;
	virtual int set_control_id(int start_id);

public:
	bool	m_is_checked;
};

// һ��������Ͽ�
class ribbon_bar_data_combo : public ribbon_bar_data_controlbase
{
	DECLARE_SERIAL(ribbon_bar_data_combo)

public:
	ribbon_bar_data_combo();
	virtual ~ribbon_bar_data_combo();

public:
	virtual void init_ribbon_control(CXTPControl* control);
	virtual XTPControlType get_ribbon_control_type() const;
	virtual void init_ribbon_control_data(tinyxml2::XMLElement* node);
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
	virtual std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;
	virtual int set_control_id(int start_id);
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;

public:
	int						m_width; // �ؼ����

	typedef struct
	{
		CString		text;
		bool		is_selected;
	} list_item;
	std::vector<list_item>	m_all_item_list; // ѡ���ĸ���Ŀ����Ͽ����еĽӿڿ��Եõ�
};

// һ��������ѡ������尴ť���������б������Ӧ�����ť����Ӧ��ֻ�ǵ����б�
class ribbon_bar_data_whole_popup_button : public ribbon_bar_data_controlbase
{
	DECLARE_SERIAL(ribbon_bar_data_whole_popup_button)

public:
	ribbon_bar_data_whole_popup_button();
	virtual ~ribbon_bar_data_whole_popup_button();

public:
	virtual void init_ribbon_control(CXTPControl* control);
	virtual XTPControlType get_ribbon_control_type() const;
	virtual void init_ribbon_control_data(tinyxml2::XMLElement* node);
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
	virtual std::vector<ribbon_control_id_and_icon_id> get_control_and_icon_id() const;
	virtual int set_control_id(int start_id);
	virtual bool get_command_and_parameter(int id, std::array<CString, 3>& info) const;
	virtual void get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const;

public:
	typedef struct
	{
		int			id;				// ÿһ��������Ŀ����Ӧһ������ID
		CString		text;			// �������Ӳ˵���ʾ���֣������˵����������Ӳ˵�����������ڲ��������
		int			icon_id;
		CString		cmd;			// �Բ˵��ķ�ʽ���������֧�ָ���Ŀָ������
		CString		cmd_params;		// �������
	}popup_list_item;
	std::vector<popup_list_item> m_all_item_list;
};

// һ��������ѡ��Ĳ�ְ�ť����ť�������б��������Ӧ���
class ribbon_bar_data_split_popup_button : public ribbon_bar_data_whole_popup_button
{
	DECLARE_SERIAL(ribbon_bar_data_split_popup_button)

public:
	ribbon_bar_data_split_popup_button();
	virtual ~ribbon_bar_data_split_popup_button();

public:
	virtual XTPControlType get_ribbon_control_type() const;
	virtual ribbon_bar_data_controlbase* clone_control_data() const;
};

//////////////////////////////////////////////////////////////////////////

// ������ʹ�õ�RibbonBar������
class app_ribbon_bar : public CXTPRibbonBar
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(app_ribbon_bar)

public:
	app_ribbon_bar(void);
	~app_ribbon_bar(void);

public:
	// ��ʼ���ؼ���ۣ�����֮ǰ���ȴ�������Ӧ�Ĺ���������ʵ�����������Ribbonû��ʹ�ñ���������
	// ��ͨ��������ʽ��ɣ���ô���ܵ������⣬�����޷�����ʹ�������ı�ǩ�����
	bool initlize_ribbon_bar(const ribbon_bar_config_data& uiData);
	void initlize_ribbon_bar_file_menu(const ribbon_bar_config_data &ribbon_ui_config);

	// ����һ�������ı�ǩ���������˼���ָ���ı�ǩ�⣬�����ر�����û���ڼ����б�all_active_view_class�е������ı�ǩ
	// ���ָ���������ı�ǩ�����Զ�����Ϊfalse��[������]
	// ���all_active_view_classΪ�ձ�ʾ�������е������ı�ǩ�����Ĵ�������
	// is_selected���Ϊtrue��ʾ����������ѡ�����ʱ�Զ��л�����Ӧ��ѡ��������������л�
	void active_context_tabs_by_window_class(const std::vector<CString>& all_active_view_class = std::vector<CString>(0), bool is_selected = true);

	// ����һ�������ı�ǩ���������˼���ָ���ı�ǩ�⣬�����ر�����û���ڼ����б�all_active_tabs�е������ı�ǩ
	// ���ָ���������ı�ǩ�����Զ�����ΪfalseҲ[ǿ�Ƽ���]
	// ���all_active_tabsΪ�ձ�ʾ�������е������ı�ǩ
	void active_context_tabs_always_by_tab_name(const std::vector<CString>& all_active_tabs);

	// ѡ��ָ�����Ƶı�ǩ���������ǲ������ǩͬ���ģ�ͬ���ľͷŵ�һ���ˣ������Ĵ�д��xml����ͬ���ģ���ѡ�е�һ����
	void select_tab(const CString& tab_name);

	// ����id���ؿؼ�����
	CXTPControl* get_control(int id) const;

	// �������ļ��б��е��ļ�·�����������޼��ַ������˵Ŀհ��ַ����ټ��·���Ƿ���Ч����Ч·�������б���ɾ��
	void check_recent_file_list();

protected:
	virtual void OnTabChanged(CXTPRibbonTab* tab);

private:
	int						m_recent_file_list_count;		// ����ĵ���¼������
	int						m_file_path_max_length;			// ����ĵ�·���ַ�����󳤶�
	std::vector<CString>	m_recent_list;					// ��������ĵ�

	// �����ڴ洢���·���ǩ���򶨱�ǩ�ǲ���洢��
	// CXTPRibbonTabָ����ݴ棬�������ڴ�
	// ��һ��ֵΪ����������tab���󣬵ڶ���ֵΪ�����ļ�����࣬������ֵΪcontext_auto_active��xml�е�ֵ
	std::vector<std::tuple<CXTPRibbonTab*, CString, bool>> m_context_tab_info_cache;
};

