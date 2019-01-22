#pragma once

// �����ռ��ǩ�ؼ�
class client_tab_view : public CXTPTabControl
{
	DECLARE_MESSAGE_MAP()

public:
	client_tab_view();
	virtual ~client_tab_view();

public:
	// �����ǩ״̬�������ݵ�ǰ�����ǩ����ribbon�����ı�ǩ״̬��������ɻ��ǩ��Ŀ�����仯ʱ����
	void reset_tab_status();

	// ���ر�ǩ����
	int get_tab_count() const;

	// �رձ�ǩ��������Ӧ�Ļ�����Ϣ���б�ǩ��ɾ�����򷵻�true�����򷵻�false
	// force_closeΪtrue��ʾ����ϲ��жϴ��������Ƿ�Ҫ���棬���жϴ����ܲ��ܱ��رգ�ֱ��ǿ�ƹرգ���Ҫ����������ֱ�ӹر�
	bool close_tab_and_clear_info(int index, bool force_close = false);
	bool close_all_tab_and_clear_info(bool force_close = false);
	
	// �����ǩ��Ӧ�Ĵ�������
	void save_tab_window_data(int index);
	void save_all_tab_window_data();

	// ����ָ�������������Ҷ���·����ͬ�Ĵ����Ƿ��Ѿ�����
	bool is_special_window_exist(const CString& window_class, const CString& object_path) const;
	
	// ����ָ�������������Ҷ���·����ͬ�Ĵ������ڱ�ǩ����������������򷵻�-1
	int get_special_window_index(const CString& window_class, const CString& object_path) const;

	// ɾ��ָ�������������Ҷ���·����ͬ�Ĵ�����ʱ��¼��Ϣ
	void remove_special_window_info(const CString& window_class, const CString& object_path);

	// ɾ��ָ�������ݶ���·�������д��ڣ����۹������Ƿ���ͬ��ɾ��
	bool remove_windows_by_path(const CString& object_path);

	// ���һ�����ڵ��ؼ��У����¼ԭ���ڵ�ָ�룩��������Ӻ��tab���ڵı�ǩ���
	// �����Ӧ�Ķ����Ѿ���ӹ�����ֱ�ӷ�������tab�����������޷�����ִ�з���-1
	// ע�Ȿ��������ӱ�ǩ��������Ա�ǩ�ļ���״̬���䶯������ԭ�����ǩ�����ڵ����������
	int add_tab_window_and_info(const CString& window_class, const CString& node_tag, int index = -1, int image_index = -1);

	// ���һ���ⲿʵ�����õĴ��ڵ��ؼ��У���������Ӻ��tab���ڵı�ǩ��ţ��ڴ��ɹ��������ܣ�ע�⣺������ڱ������ڶ��Ϸ����
	// �ⲿֻҪ�����ô��ڼ��ɣ����ݵļ��ص������Խӿں����������ɿ���Զ����
	int add_tab_window_and_info(window_interface* wnd, const CString node_tag, int index = -1, int image_index = -1);

	// ȡ��ָ����ǩ�Ĵ���ָ��(����ʱ��ָ�룩
	CWnd* get_tab_window(int index) const;
	window_interface* get_tab_window_interface(int index) const;
	
	// ��ȡָ����ǩ�Ĵ��ڸ�����Ϣ 0������������ 1���������tag��2������ָ��
	bool get_tab_window_and_info(std::tuple<CString, CString, CWnd*>& info, int index) const;

	// ��ȡ��һ���Ҽ�����ı�ǩ��ţ������Ч�򷵻�-1�����������ⲿά����һ���˵���Ӧ���������Ϊ-1���ڲ�����ǩ��Ŀ�䶯��Ҳ���Զ���Ϊ-1
	int get_last_right_clicked_tab_index() const;
	void clear_last_right_clicked_tab_index();

protected:
	int implement_add_tab_window_and_info(window_interface* wnd, const CString node_tag, int index, int image_index);

protected:
	// �ϴ��Ҽ������ѡ���ֻ�����ݼǣ�������׷��ѡ��Ƿ���Ч��������tab��ǩ���Ҽ��˵���Ӧ��
	CXTPTabManagerItem* m_last_right_clicked_tab;

	// ֻ��¼���󣬲�����ָ��(0������������ 1���������tag��2������ָ��)
	std::vector<std::tuple<CString, CString, CWnd*>> m_all_windows;

protected:
	// ��ǩ��������϶����»��ǩ״̬�仯ʱ����
	virtual void OnItemClick(CXTPTabManagerItem* pItem);

protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* dc);
};

