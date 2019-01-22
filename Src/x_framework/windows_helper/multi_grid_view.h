#pragma once

class CTHExcel;
class CTHDataBase;


#ifndef ID_MULTI_GRID_START
#	define ID_MULTI_GRID_START 2000
#endif

#ifndef ID_MULTI_GRID_END
#	define ID_MULTI_GRID_END   5000
#endif

#ifndef WM_TAB_ITEM_BE_CLICKED
#define WM_TAB_ITEM_BE_CLICKED    WM_USER + 129
#endif


struct X_FRAMEWORK_API multi_grid_item_info
{
	UINT			m_wnd_id = -1;				// excel������Ĵ���id
	bool			m_is_visible = true;		// �Ƿ���Ҫ��ʾ
	CString			m_db_name;					// ���ʵ����������
	CTHExcel*		m_excel = nullptr;			// װ�ر��Ĵ���
	CTHDataBase*	m_db = nullptr;				// ���ʵ��
};


class X_FRAMEWORK_API multi_grid_view : public CFormView
{
	DECLARE_SERIAL(multi_grid_view);
	DECLARE_MESSAGE_MAP()

public:
	multi_grid_view(UINT nIDTemplate = 0, bool need_delete_self_post_nc_destroy = true);
	virtual ~multi_grid_view(void);
	
public:
	BOOL create_from_static(CWnd* parent, UINT id, BOOL has_border = TRUE);
	BOOL create_direct(CWnd* parent, UINT id, const RECT& rect, BOOL has_border );
	void assign_all_excels(const std::vector<CString>& template_names); // �����ڲ���ģ�������´������пձ�
	void assign_all_excels(const std::vector<std::pair<CString, CTHDataBase*>>& template_names); // ���ݱ���������ͱ��ʵ���������б�񣬱������Ƕ��ϵĶ����ڴ��ɿ�ܽӹ�
	void add_excel(const CString& template_name);
	void add_excel(const CString& index_name, CTHDataBase* instance); // ���ݱ���������ͱ��ʵ����ӱ�񣬱������Ƕ��ϵĶ����ڴ��ɿ�ܽӹ�
	void clear_all_excels_and_data(); // ������б��
	void set_back_color(COLORREF color);
	CTHExcel* find_excel(const CString& data_base_name);
	CTHExcel* find_excel(UINT id);
	CTHDataBase* find_data_base(const CString& data_base_name);
	CTHDataBase* find_data_base(UINT id);
	void set_excel_visible(const CString& data_base_name, bool show = false);
	void set_excel_visible(UINT id, bool show = false);
	bool is_excel_visible(const CString& data_base_name) const;
	bool is_excel_visible(UINT id) const;
	void get_all_data_base(std::map<CString, CTHDataBase*>& dbs);
	void enable_auto_resize_excel(bool auto_resize_excel);
	void resize_excels();

public:
	virtual void OnInitialUpdate();
	virtual void PostNcDestroy();
	
protected:
	std::vector<multi_grid_item_info> m_all_excels; // ���id���Ƿ�Ҫ��ʾ����񴰿�ʵ�������DBʵ��
	COLORREF						m_back_color;
	bool							m_auto_resize_excel;
	int								m_total_width;
	int								m_total_height;
	bool							m_is_standard_editor;
	bool							m_is_initialize;
	bool							m_need_delete_self_post_nc_destroy;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* dc);

	afx_msg BOOL on_remove_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_add_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_row_col_size_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_value_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_button_clicked(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_focus_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
};



// һ���ܽ��ձ����Ϣ�ı�ǩ�࣬��֧����Ӧѡ������ͷ��ؿ��Ƴߴ�(�����޶�ֻ�ܷŶ��ؼ�)
// �����ڲ���ʲô���ڣ�ֻҪ�б���򸸴��ڱ����Ǳ���ǩ�ؼ������߱�ǩҳ��
// �Ĵ�����multi_grid_viewҲ���ԣ�����Ҫ����Ϣ���ϲ㴫�ݣ�ֱ�����ؼ��ܽ��յ������򱾿ؼ��޷���Ӧ��
// ���⣬ֻ�б���ID��ID_MULTI_GRID_START��ID_MULTI_GRID_END֮��
class X_FRAMEWORK_API multi_grid_tab_control : public CXTPTabControl
{
	DECLARE_DYNCREATE(multi_grid_tab_control)
	DECLARE_MESSAGE_MAP()

public:
	multi_grid_tab_control();
	virtual ~multi_grid_tab_control();

public:
	int get_tab_header_height() const; // ��ѡ����Ŀؼ��߿���
	int get_tab_border_width() const;

public:
	// ��д���麯�����ڱ�֤ԭ���ܲ����������򸸴��ڷ�WM_TAB_ITEM_BE_CLICKED��Ϣ�����ڸ����ڲ����������ڼ���ʹ��
	virtual void OnItemClick(CXTPTabManagerItem* pItem);

private:
	afx_msg BOOL on_remove_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_add_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_row_col_size_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_value_changed(UINT id, NMHDR * notify_struct, LRESULT * result);
	afx_msg BOOL on_cell_button_clicked(UINT id, NMHDR * notify_struct, LRESULT * result);
	afx_msg BOOL on_cell_focus_changed(UINT id, NMHDR * notify_struct, LRESULT * result);
};
