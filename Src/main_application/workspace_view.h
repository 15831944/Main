#pragma once
#include "client_tab_view.h"

// �����ռ��id��mfc�ڶ������������CView�����������ĵ�������ر�ʱ����
class workspace_view : public CWnd
{
	DECLARE_MESSAGE_MAP()

public:
	workspace_view();
	virtual ~workspace_view();

public:
	// ��õ�ǰ�ı�ǩ����
	int get_tab_count() const;

	// ��ȡ��ǰѡ�еı�ǩ��������0��ʼ��û���򷵻�-1
	int get_current_tab_index() const;

	// ���һ����Ŀ���ؼ��У����¼ԭ���ڵ�ָ�룩��������Ӻ��tab���ڵı�ǩ���
	// ���·��Ϊ�գ�˵��û�й����Ĺ���������ര��ֻ�����һ��������ʼҳ��
	// ע�⣺
	//   �������ΪCView�������࣬��CView���ڴ�������֮��OnPostDestroy����delete this�����޷��ÿգ����¿����
	//   ָ�����գ����ٴ���ʱ��������˶�CView����������Ҫ���⴦������PostNcDestroy�麯������ֹ����CView�İ汾
	//   ���ɣ�Ҳ�����ٵ���delete����������ؼ�����ȷ�������Ⲣ��������ڴ�й©
	int add_tab_window_and_info(const CString window_class, const CString node_tag, int index = -1, int image_index = -1);

	// ���һ���ⲿʵ�����õĴ��ڵ�����������������Ӻ��tab���ڵı�ǩ��ţ��ڴ��ɹ��������ܣ�ע�⣺������ڱ������ڶ��Ϸ����
	// �ⲿֻҪ�����ô��ڼ��ɣ����ݵļ��ص������Խӿں����������ɿ���Զ���ɣ������������󣬷����Ƽ�ʹ�ã�����ʹ�ô���������ذ汾��
	// ע�⣺
	//   �������ΪCView�������࣬��CView���ڴ�������֮��OnPostDestroy����delete this�����޷��ÿգ����¿����
	//   ָ�����գ����ٴ���ʱ��������˶�CView����������Ҫ���⴦������PostNcDestroy�麯������ֹ����CView�İ汾
	//   ���ɣ�Ҳ�����ٵ���delete����������ؼ�����ȷ�������Ⲣ��������ڴ�й©
	int add_tab_working_space_view(window_interface* wnd, const CString node_tag, int index = -1, int image_index = -1);

	// ȡ��ָ����ǩ�Ĵ���ָ��(����ʱ��ָ�룩
	CWnd* get_tab_window(int index);

	// �ر����д��ڣ����Զ����ñ���������ݵĽӿڣ�
	bool close_all_window(bool force_close = false);

	// �رղ�����ָ�������Ĵ��ڣ�������0��ʼ���ɹ�����true��ʧ�ܷ���false����ʱ��֤���Ƴ���
	bool close_tab_by_index(int index, bool force_close = false);

	// �������д���
	void save_all_window_data();

	// ɾ��ָ�������ݶ���·�������д��ڣ����۹������Ƿ���ͬ��ɾ��
	bool remove_windows_by_path(const CString& object_path);

	// ��ȡָ����ǩ�Ĵ��ڸ�����Ϣ 0������������ 1���������tag��2������ָ��
	bool get_tab_window_and_info(std::tuple<CString, CString, CWnd*>& info, int index) const;

	// ȷ��ѡ����ֻ��ָ���ĸ��������������Ŀֱ�ӷ��أ��������ǰ�濪ʼ�Ƴ�
	// �����ɹ�����true,���򷵻�false������removed_count�����Ƴ��Ĵ�����Ŀ������ʧ��Ҳ�����Ƴ���һ���֣�
	bool make_tab_max_count(int max_count, int& removed_count);

	// ����ָ��������ѡ��ı�������
	void set_tab_caption(int index, const CString& caption);

	// ����һ��ָ��������ǩ��ѡ�����,-1��ʾȫ������
	void update_tab_caption(int index);

protected:
	client_tab_view m_client_tab;
	int m_max_tab_count = 30;

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();

	afx_msg void on_workspace_popup_menu_save();
	afx_msg void on_workspace_popup_menu_close();
	afx_msg void on_workspace_popup_menu_save_all();
	afx_msg void on_workspace_popup_menu_close_all();

};
