#include "StdAfx.h"
#include "resource.h"

#include "x_framework/macro.h"
#include "x_framework/interface/console_interface.h"
#include "x_framework/interface/window_interface.h"
#include "x_framework/interface/cmd_container_interface.h"

#include "main_frame.h"

#include "client_tab_view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


BEGIN_MESSAGE_MAP(client_tab_view, CXTPTabControl)
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


client_tab_view::client_tab_view()
{
	m_last_right_clicked_tab = nullptr;
}

client_tab_view::~client_tab_view()
{

}

// ���ݵ�ǰ����ڼ����������Ӧ��ribbon�����ı�ǩ,������ܵ��ã������⿪�ţ����⿪�Ľӿ���˲�ͬ��
void auto_active_ribbon_context_tabs(const vector<CString>& all_active_view_class)
{
	main_frame* db_frame = dynamic_cast<main_frame*>(AfxGetMainWnd());
	if (!db_frame || !db_frame->GetSafeHwnd()) return;

	app_ribbon_bar* bar = db_frame->get_db_ribbon_bar();
	if (!bar || !bar->GetSafeHwnd()) return;

	bar->active_context_tabs_by_window_class(all_active_view_class, true);
}

void client_tab_view::reset_tab_status()
{
	int tab_count = GetItemCount();

	// ֻ��һ��ѡ�ʱ���ر�ǩ
	if (tab_count <= 1)
		GetPaintManager()->m_bShowTabs = FALSE;
	else
		GetPaintManager()->m_bShowTabs = TRUE;
	
	int sel_tab = GetCurSel();

	if (tab_count >= 1)
	{
		ShowWindow(SW_SHOW);
		SetCurSel(sel_tab);

		vector<CString> all_active_view_class;
		tuple<CString, CString, CWnd*> info;
		if (get_tab_window_and_info(info, sel_tab))
			all_active_view_class.push_back(std::get<0>(info));

		auto_active_ribbon_context_tabs(all_active_view_class); // ���������������ribbon��ǩ���������޹�����������ribbon��ǩ

		// ����ѡ��״̬�仯�󷢸������Щ����Ҫ���������Ϣ����Ӧ���������ڵ�Ҫ��ʾ��Ӧ�ļ��ر��
		cmd_trigger_manager::fire(_T("sys"), _T("editor_loaded"), std::get<1>(info));
	}
	else
	{
		ShowWindow(SW_HIDE);
		auto_active_ribbon_context_tabs(vector<CString>()); // �ر����е�������ribbon��ǩ
		
		// ����ѡ��״̬�仯�󷢸������Щ����Ҫ���������Ϣ����Ӧ���������ڵ�Ҫ��ʾ��Ӧ�ļ��ر��
		cmd_trigger_manager::fire(_T("sys"), _T("editor_loaded"), _T("")); // ��·���൱��û���κα༭��������
	}

	// ��ǩ���ػ���ʾ����ͻ����Ĵ��ڲ��Զ�����������һ����Ӱ����ʱtab��û�б䶯�ߴ磬���Բ��ᴥ��size�������ƣ�����ֶ�����һ��
	SendMessage(WM_SIZE, 0, 0);
}

int client_tab_view::get_tab_count() const
{
	// һ����˵�ڲ������ݸ����ͽ���ı�ǩ�����Ƕ�Ӧ�ģ��������Ӧ��˵�����������⣬���ֻҪ�����ڲ����ݵĸ�������

	return (int) m_all_windows.size();
}

bool client_tab_view::close_tab_and_clear_info(int index, bool force_close/* = false*/)
{
	CWaitCursor wt_cursor;

	tuple<CString, CString, CWnd*> info;
	if (false == get_tab_window_and_info(info, index)) return false;

	// ���ڱ�־���ǰ���ã���Щ������pre_close�л���һЩ���ݱ��棬���ڲ�ѯ�����־ʱ����false��Ҫ������������������
	CWnd* wnd = std::get<2>(info);
	if (!wnd) return false;

	window_interface* wnd_ix = dynamic_cast<window_interface*>(wnd);
	if (!wnd_ix) return false;

	CString close_detail;
	if (!force_close && !wnd_ix->can_be_close(close_detail))
	{
		app_print_info(MT_DEBUG, _T("workspace window can't be close, windows class name : %s , tab index : %d"), std::get<0>(info), index);
		AfxMessageBox(close_detail, MB_OK | MB_ICONWARNING);
		return false;
	}

	if (!force_close && wnd_ix && wnd_ix->need_save_ui_data())
		save_tab_window_data(index); // ֻ�ǽ������ݸ��µ�ģ����

	// CWnd�ĳ�ԱDestroyWindow�������ٴ��ں��þ��Ϊ�գ�api����::DestroyWindow�򲻻ᣬΪ��֤�������벻���⴦��ͳһ�����ٺ��ֶ���Ϊ��
	if (wnd->GetSafeHwnd())
	{
 		::ShowWindow(wnd->GetSafeHwnd() , SW_HIDE);
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

		// ��Ҫ��WM_DESTORY��Ϣ������ÿ�����ڵ��������������ٵ���Ϣ��Ӧ�д�����Դ�������Ĵ��ڻ���������Ի���ͨ����ȷ��WM_CLOSE��Ϣ
		// ��Ҳ��Ҫֱ�ӵ���wnd->Destory()��Ҫ��Ȼ������osg��������Ⱦ�̵߳Ĵ��ڻᵼ���߳��쳣�����ʹ��������Ϣ����������
		wnd->SendMessage(WM_CLOSE);
		wnd->m_hWnd = nullptr;
		wnd->m_hWndOwner = nullptr;
	}

	// ɾ��ѡ����ڲ�������ɾ�����ӹܵĶ���ָ�룬�����ͷ�mfc�ڲ���cmdtarget�ǽ�����Դ(����Ϣ���������Դ��com��Դ��)
	DeleteItem(index);

	// CView���ڴ�������֮��OnPostDestroy����delete this;�ⲿָ������գ���ˣ��˴���ɾ������������˶�CView��
	// ��������Ҫ���⴦������PostNcDestroy�麯������ֹ����CView�İ汾���ɣ�Ҳ�����ٵ���delete����������ؼ���
	// ��ȷ�������Ⲣ��������ڴ�й©
	safe_delete(wnd); // �������Ĵ����Ƕ�̬�����ģ��϶����ڶ��ϣ����Կ���delete
	std::get<2>(info) = nullptr;

	// ����Ǵ�����Դ
	m_last_right_clicked_tab = nullptr;

	if (index >= 0 && index < (int) m_all_windows.size())
		m_all_windows.erase(m_all_windows.begin() + index);

	// ���ݵ�ǰѡ�״̬����ѡ�����
	reset_tab_status();

	return true;
}

bool client_tab_view::close_all_tab_and_clear_info(bool force_close/* = false*/)
{
	// ÿ��ѡ��رյ�ʱ�򶼻ᱣ�棬���ﲻ����
	// save_all_tab_window_data();

	// �����һ����ǰ��һ�ǵ����ٶ���ʱ�Ӿ�Ч���ã����Ƿ�ֹɾ��ѡ����������仯
	for (int i = get_tab_count() - 1; i >= 0; --i)
		if (!close_tab_and_clear_info(i, force_close)) return false;

	return true;
}

void client_tab_view::save_tab_window_data(int index)
{
	tuple<CString, CString, CWnd*> info;
	if (false == get_tab_window_and_info(info, index)) return;

	CWnd* wnd = std::get<2>(info);
	if (!wnd) return;

	window_interface* win_ix = dynamic_cast<window_interface*>(wnd);
	if (!win_ix) return;

	win_ix->save_data_from_ui();
	m_last_right_clicked_tab = nullptr;
}

void client_tab_view::save_all_tab_window_data()
{
	for (auto& x : m_all_windows)
	{
		CWnd* wnd = std::get<2>(x);

		window_interface* win_ix = dynamic_cast<window_interface*>(wnd);
		if (!win_ix) continue;

		win_ix->save_data_from_ui();
	}

	m_last_right_clicked_tab = nullptr;
}

bool client_tab_view::is_special_window_exist(const CString& window_class, const CString& object_path) const
{
	for (auto& x : m_all_windows)
		if (std::get<0>(x) == window_class && 0 == object_path.CompareNoCase(std::get<1>(x)))
			return true;
	
	return false;
}

int client_tab_view::get_special_window_index(const CString& window_class, const CString& object_path) const
{
	for (size_t i = 0; i < m_all_windows.size(); ++i)
		if (std::get<0>(m_all_windows[i]) == window_class && 0 == object_path.CompareNoCase(std::get<1>(m_all_windows[i])))
			return i;

	return -1;
}

void client_tab_view::remove_special_window_info(const CString& window_class, const CString& object_path)
{
	vector<tuple<CString, CString, CWnd*>> all_new_windows;
	for (auto& x : m_all_windows)
		if (!(std::get<0>(x) == window_class && 0 == object_path.CompareNoCase(std::get<1>(x))))
			all_new_windows.push_back(x);

	m_all_windows = all_new_windows;
}

bool client_tab_view::remove_windows_by_path(const CString& object_path)
{
	// �������������������û�ж�Ӧ��ϵ�Ĵ��ڣ����ִ���·��Ϊ�գ���ͬһ��������ֻ�ᴴ��һ�����������쳣������ж��Ҳȫ��
	// if (object_path.IsEmpty()) return;

	// ����ʹ�õ������ķ�ʽ����Ϊ�ر�ѡ���ʱ��������ں���Ķ�Ӧ���ݣ���ʱ������ָ��Ķ���ɾ���ˣ��ᵼ����++ʱʧЧ
	// for (auto& x : m_all_windows)
	
	if (m_all_windows.empty()) return true; // û�д���Ҳ�����رճɹ�
	for (int i = (int) m_all_windows.size() - 1; i >= 0; --i)
	{
		if (0 == object_path.CompareNoCase(std::get<1>(m_all_windows[i])))
		{
			int index = get_special_window_index(std::get<0>(m_all_windows[i]), std::get<1>(m_all_windows[i]));
			if (-1 == index) continue;
			if (!close_tab_and_clear_info(index)) return false;
		}
	}

	return true;
}

int client_tab_view::add_tab_window_and_info(const CString& window_class, const CString& node_tag, int index/* = -1*/, int image_index/* = -1*/)
{
	CWaitCursor wt_cursor;
	// ����·��Ϊ�գ���ʾû�й���������ര��ֻ�ܼ�һ��������ʼҳ
	//if (node_tag.IsEmpty())
	//{
	//	AfxMessageBox(_T("����û�й����Ĺ������"), MB_OK | MB_ICONERROR);
	//	return -1;
	//}

	if (!GetSafeHwnd())
	{
		AfxMessageBox(_T("�������������ھ��Ϊ�գ��޷���ʾ����"), MB_OK | MB_ICONERROR);
		return -1;
	}

	static int window_id = 1500; // �ڲ��Զ�ά������id���ȼ򵥵ݼӣ�������ʱ�������ܴ���

	if (window_class.IsEmpty())
	{
		AfxMessageBox(_T("��������Ϊ�գ��޷�ʵ����������xml����ȷ���ô�������"), MB_OK | MB_ICONERROR);
		return -1;
	}

	int index_exist = get_special_window_index(window_class, node_tag);
	if (index_exist >= 0) return index_exist;

	CWnd* wnd = dynamic_cast<CWnd*>(CRuntimeClass::CreateObject(window_class));
	if (!wnd)
	{
		AfxMessageBox(_T("������[") + window_class + _T("]δ��ȷע�ᣬ�޷���̬����"), MB_OK | MB_ICONERROR);
		return -1;
	}

	DWORD wnd_sytle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	wnd_sytle &= ~WS_BORDER;

	CRect rect = CXTPTabManager::GetClientRect();
	
	// ������1500-35000֮�䣬��������������ͬʱ���ڼ�������ڿ����ڣ�����ѭ����û�����
	// �����ѭ���Ļ������ڴ򿪵Ĵ����Ǻܿ������������ϳ�ʱ��ͳ����ˣ�����ɹ��������ڴ�������
	// �û�������ͬʱ��3������ڣ���˶�β�����ǰ���id�Ϳ����ˣ���˿����ٻ�ͷ����
	unsigned int id = ++window_id;
	if (id > 35000) { id = 1500; window_id = 1500; }

	if (!wnd->Create(NULL, NULL, wnd_sytle, rect, this, id))
	{
		AfxMessageBox(_T("������[") + window_class + _T("]ʵ��������ô��ڴ�������Createʧ��"), MB_OK | MB_ICONERROR);
		return -1;
	}
	
	window_interface* wnd_ix = dynamic_cast<window_interface*>(wnd);
	if (!wnd_ix)
	{
		AfxMessageBox(_T("������[") + window_class + _T("]û�д�window_interface�ӿ���public��ʽ�������޷���ӵ�������"), MB_OK | MB_ICONERROR);
		wnd->DestroyWindow();
		safe_delete(wnd);
		return -1;
	}

	return implement_add_tab_window_and_info(wnd_ix, node_tag, index, image_index);
}

int client_tab_view::add_tab_window_and_info(window_interface* wnd, const CString node_tag, int index /*= -1*/, int image_index /*= -1*/)
{
	CWaitCursor wt_cursor;

	if (!wnd) return -1;

	return implement_add_tab_window_and_info(wnd, node_tag, index, image_index);
}

CWnd* client_tab_view::get_tab_window(int index) const
{
	tuple<CString, CString, CWnd*> info;
	if (false == get_tab_window_and_info(info, index)) return nullptr;

	return std::get<2>(m_all_windows[index]);
}

window_interface* client_tab_view::get_tab_window_interface(int index) const
{
	return dynamic_cast<window_interface*>(get_tab_window(index));
}

bool client_tab_view::get_tab_window_and_info(std::tuple<CString, CString, CWnd*>& info, int index) const
{
	int tab_count = GetItemCount();
	if (tab_count <= 0) return false;

	if (index < 0 || index >= tab_count) index = tab_count - 1;

	// ָ�벻��ͨ��tab�ؼ���HANDLE�����죬�����õ�����������CWndָ�룬�Ѿ��޷�֪��ԭ������������ʲô�ˣ�����Ҫ�Լ���¼��ά��CWndָ��
	if (index >= (int)m_all_windows.size()) return false;

	info = m_all_windows[index];
	return true;
}

int client_tab_view::get_last_right_clicked_tab_index() const
{
	if (!m_last_right_clicked_tab) return -1;

	return m_last_right_clicked_tab->GetIndex();
}

void client_tab_view::clear_last_right_clicked_tab_index()
{
	m_last_right_clicked_tab = nullptr;
}

int client_tab_view::implement_add_tab_window_and_info(window_interface* wnd_ix, const CString node_tag, int index, int image_index)
{
	if (!wnd_ix) return -1;

	CWnd* wnd = dynamic_cast<CWnd*>(wnd_ix);
	if (!wnd || !wnd->GetSafeHwnd()) return -1;

	wnd->SetOwner(this);
	wnd->SetParent(this);
	wnd->ModifyStyle(WS_BORDER, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP);

	// �û�ָ������ɾ��ǰ��������ѡ����ˣ�����ҲҪ��Ӧ���ż�
	int current_tab_count = get_tab_count();
	
	// -1��������ֱ���õģ���Ϊ�˺ͷ��������ͬ������Ϊ��ʵ��λ��
	int tab_count = GetItemCount();
	if (index < 0 || index > tab_count) index = tab_count;

	CString window_class = wnd->GetRuntimeClass()->m_lpszClassName;
	m_all_windows.push_back(make_tuple(window_class, node_tag, wnd));

	// ʼ����ӵ����ѡ���������ݼ��غ�������
	CXTPTabManagerItem* added_item = InsertItem(index, _T(""), wnd->GetSafeHwnd(), image_index);

	// �������У��ٸ��½��棬���ݼ���Ҫ����OnInitialUpdateǰ�棬�Ա㴰���ܸ��������Զ�������OnInitialUpdateʱ�������ַ�ʽ
	// �����ݶ���Ҳԭ�����룬�ò����ɴ����Լ�������Щ����ȷʵ�ǲ���Ҫ���ݵģ�����ʼҳ
	wnd_ix->load_data_to_ui(node_tag);

	// ���ص����������ټ������ݣ������view���ȵ���OnInitialUpdate
	CView* view = dynamic_cast<CView*>(wnd);
	if (view) view->OnInitialUpdate();

	// �������������ã��Ա㴰���ܸ����������յ�״̬��������
	if (added_item) added_item->SetCaption(wnd_ix->get_title());

	// �����ɺ󷢸������Щ����Ҫ���������Ϣ����Ӧ���������ڵ�Ҫ��ʾ��Ӧ�ļ��ر��
	cmd_trigger_manager::fire(_T("sys"), _T("editor_loaded"), node_tag);

	return index;
}

void client_tab_view::OnItemClick(CXTPTabManagerItem* pItem)
{
	int tab_index_before_click = GetCurSel();
	CXTPTabControl::OnItemClick(pItem);
	int tab_index_after_click = GetCurSel();

	if (tab_index_before_click != tab_index_after_click)
	{
		vector<CString> all_active_view_class;
		tuple<CString, CString, CWnd*> info;
		if (get_tab_window_and_info(info, tab_index_after_click))
		{
			all_active_view_class.push_back(std::get<0>(info));
			CWnd* wnd = std::get<2>(info);
			if (wnd && wnd->GetSafeHwnd()) wnd->SetFocus(); // ����һ�±༭�����ڵ�OnFocus���Ա��䴦�������Ϣ�����ȡ�����лỰȨ��
		}

		auto_active_ribbon_context_tabs(all_active_view_class); // ���������������ribbon��ǩ���������޹�����������ribbon��ǩ
	}

	//reset_tab_status();
}

void client_tab_view::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_last_right_clicked_tab = HitTest(point);
	if (!m_last_right_clicked_tab) return;

	CMenu tab_right_menu;
	tab_right_menu.LoadMenu(IDR_MENU_WORKSPACE_TAB);
	CMenu* first_pop_up = tab_right_menu.GetSubMenu(0);
	if (!first_pop_up) return;

	ClientToScreen(&point);
	// �ɸ�������Ӧ�˵���Ϣ������workspace_view��Ӧ��
	first_pop_up->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, GetParent());
}

BOOL client_tab_view::OnEraseBkgnd(CDC* dc)
{
	return TRUE;
}
