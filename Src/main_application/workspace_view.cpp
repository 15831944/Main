#include "stdafx.h"
#include "resource.h"

#include "x_framework/macro.h"
#include "x_framework/interface/console_interface.h"
#include "x_framework/interface/window_interface.h"

#include "workspace_view.h"

#include "main_frame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define IDC_WORKING_SPACE_TAB_CONTROL_ID	13003


using namespace std;


BEGIN_MESSAGE_MAP(workspace_view, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()

	ON_COMMAND(ID_WORKSPACETABPOPUPMENU_SAVE, on_workspace_popup_menu_save)
	ON_COMMAND(ID_WORKSPACETABPOPUPMENU_CLOSE, on_workspace_popup_menu_close)
	ON_COMMAND(ID_WORKSPACETABPOPUPMENU_SAVE_ALL, on_workspace_popup_menu_save_all)
	ON_COMMAND(ID_WORKSPACETABPOPUPMENU_CLOSE_ALL, on_workspace_popup_menu_close_all)

END_MESSAGE_MAP()


workspace_view::workspace_view()
{
}

workspace_view::~workspace_view()
{
}

int workspace_view::get_tab_count() const
{
	if (!m_client_tab.GetSafeHwnd()) return 0;
	return m_client_tab.GetItemCount();
}

int workspace_view::get_current_tab_index() const
{
	if (!m_client_tab.GetSafeHwnd()) return 0;
	return m_client_tab.GetCurSel();
}

int workspace_view::add_tab_window_and_info(const CString window_class,
	const CString node_tag, int index /*= -1*/, int image_index /*= -1*/)
{
	if (!m_client_tab.GetSafeHwnd())
	{
		app_print_info(OO_UI_LOG, MT_ERROR, _T("���ھ����Ч���޷���ӵ�������ʵ����"));
		return -1;
	}

	// ��������Ѿ����ˣ��򼤻ֱ�ӷ���
	int index_exist = m_client_tab.get_special_window_index(window_class, node_tag);
	if (index_exist >= 0)
	{
		m_client_tab.SetCurSel(index_exist);
		return index_exist;
	}

	int removed_count = 0;
	if (false == make_tab_max_count(m_max_tab_count, removed_count))
	{
		app_print_info(OO_UI_LOG, MT_ERROR, _T("������ӵ�������ѡ��ؼ�ʱ����ʧ��"));
		AfxMessageBox(_T("�����޷��ڹ�������"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	
	// ��ӹ��������ڿ��ܻ������Ҫ���ж��Ƿ�Ҫ�ص�һЩ���������ڣ��糬������󴰿���������ʱ�ص��Ĵ��ڻ������Ӧ��
	// ���湦�ܣ����ڱ���ʱ���ܴ����������ĵ�������Ӧ����ӵ������ؼ��Ľڵ����ټ��ؼ��أ�����ʱ��ǰ�Ѿ��͹�������Ϣ
	// �����Ѿ�ʧЧ�����ˣ���˲������ʹ�����������Ҳ��Ч�ˣ����Ժ���ԭ�͵Ĳ������Ͳ���ʹ������
	int index_added = m_client_tab.add_tab_window_and_info(window_class, node_tag, index, image_index);
	if (-1 == index_added)
	{
		app_print_info(OO_UI_LOG, MT_ERROR, _T("���ھ���ӵ�������ѡ��ؼ�ʱ����ʧ��"));
		return -1;
	}
	
 	// ����Ѿ������л�����Ӧ�ı�ǩ
	m_client_tab.SetCurSel(index_added);

	m_client_tab.reset_tab_status(); // �����ǩǰ������ȷѡ�л��ǩ��������ݱ�ǩ����ribbon�����ı�ǩʱ����ȷ
	m_client_tab.clear_last_right_clicked_tab_index();
	
	return index_added;
}

int workspace_view::add_tab_working_space_view(window_interface* wnd, const CString node_tag, int index /*= -1*/, int image_index /*= -1*/)
{
	CWnd* window = dynamic_cast<CWnd*>(wnd);
	if (!window || !window->GetSafeHwnd()) return -1;

	CString window_class = window->GetRuntimeClass()->m_lpszClassName;
	
	// ��������Ѿ����ˣ��򼤻ֱ�ӷ���
	int index_exist = m_client_tab.get_special_window_index(window_class, node_tag);
	if (index_exist >= 0)
	{
		m_client_tab.SetCurSel(index_exist);
		return index_exist;
	}


	int removed_count = 0;
	if (false == make_tab_max_count(m_max_tab_count, removed_count))
	{
		app_print_info(OO_UI_LOG, MT_ERROR, _T("������ӵ�������ѡ��ؼ�ʱ����ʧ��"));
		AfxMessageBox(_T("�����޷��ڹ�������"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	
	// ��ӹ��������ڿ��ܻ������Ҫ���ж��Ƿ�Ҫ�ص�һЩ���������ڣ��糬������󴰿���������ʱ�ص��Ĵ��ڻ������Ӧ��
	// ���湦�ܣ����ڱ���ʱ���ܴ����������ĵ�������Ӧ����ӵ������ؼ��Ľڵ����ټ��ؼ��أ�����ʱ��ǰ�Ѿ��͹�������Ϣ
	// �����Ѿ�ʧЧ�����ˣ���˲������ʹ�����������Ҳ��Ч�ˣ����Ժ���ԭ�͵Ĳ������Ͳ���ʹ������
	int index_added = m_client_tab.add_tab_window_and_info(wnd, node_tag, index, image_index);
	if (-1 == index_added)
	{
		app_print_info(OO_UI_LOG, MT_ERROR, _T("���ھ���ӵ�������ѡ��ؼ�ʱ����ʧ��"));
		return -1;
	}
	
 	// ����Ѿ������л�����Ӧ�ı�ǩ
	m_client_tab.SetCurSel(index_added);

	m_client_tab.reset_tab_status(); // �����ǩǰ������ȷѡ�л��ǩ��������ݱ�ǩ����ribbon�����ı�ǩʱ����ȷ
	m_client_tab.clear_last_right_clicked_tab_index();

	return -1;
}

CWnd* workspace_view::get_tab_window(int index)
{
	if (!m_client_tab.GetSafeHwnd()) return nullptr;
	if (index < 0) return nullptr;

	return m_client_tab.get_tab_window(index);
}

bool workspace_view::close_all_window(bool force_close/* = false*/)
{
	return m_client_tab.close_all_tab_and_clear_info();
}

bool workspace_view::close_tab_by_index(int index, bool force_close/* = false*/)
{
	return m_client_tab.close_tab_and_clear_info(index, force_close);
}

void workspace_view::save_all_window_data()
{
	m_client_tab.save_all_tab_window_data();
}

bool workspace_view::remove_windows_by_path(const CString& object_path)
{
	return m_client_tab.remove_windows_by_path(object_path);
}

bool workspace_view::get_tab_window_and_info(std::tuple<CString, CString, CWnd*>& info, int index) const
{
	return m_client_tab.get_tab_window_and_info(info, index);
}

bool workspace_view::make_tab_max_count(int max_count, int& removed_count)
{
	removed_count = 0;
	int current_tab_count = get_tab_count();
	if (max_count <= current_tab_count) return true; // ����Ҫ��Ҳ��ɹ�

	do
	{
		if (current_tab_count >= max_count)
		{
			// ʼ��ɾ����ǰ��ģ��Ƴ�ʧ��ʱ�ڲ��ᱨ��Ϣ�򣬴˴������ٱ���
			if (false == close_tab_by_index(0)) return false;

			++removed_count;
		}

		current_tab_count = get_tab_count();
	} while (current_tab_count >= max_count && current_tab_count > 0);

	return true;
}

void workspace_view::set_tab_caption(int index, const CString& caption)
{
	if (index < 0 || index >= get_tab_count()) return;

	CXTPTabManagerItem* tab_item = m_client_tab.GetItem(index);
	if (tab_item) tab_item->SetCaption(caption);	
}

void workspace_view::update_tab_caption(int index)
{
	auto fun_update_one_tab = [&](int index)
	{
		CXTPTabManagerItem* tab_item = m_client_tab.GetItem(index);
		if (!tab_item) return;

		window_interface* wnd_x = m_client_tab.get_tab_window_interface(index);
		if (!wnd_x) return;

		tab_item->SetCaption(wnd_x->get_title());
	};

	if (index >= 0)
	{
		if (index >= m_client_tab.get_tab_count()) return;
		
		fun_update_one_tab(index);
		return;
	}

	for (int i = 0; i < m_client_tab.get_tab_count(); ++i)
		fun_update_one_tab(i);
}

int workspace_view::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= WS_CLIPCHILDREN | WS_CHILD | WS_CLIPSIBLINGS;
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	dwStyle &= ~WS_BORDER;
	m_client_tab.Create(dwStyle, CRect(), this, IDC_WORKING_SPACE_TAB_CONTROL_ID);
	// m_client_tab.GetPaintManager()->m_bBoldSelected = TRUE;
	// m_client_tab.GetPaintManager()->m_clientFrame = xtpTabFrameSingleLine;
	// m_client_tab.GetPaintManager()->m_rcButtonMargin = CRect(10, 5, 0, 5);
	// m_client_tab.GetPaintManager()->m_nMinTabWidth = 110;
	// m_client_tab.GetPaintManager()->m_nMaxTabWidth = 110;
	// m_client_tab.GetPaintManager()->m_nFixedTabWidth = 110;
	// m_client_tab.GetPaintManager()->m_bVerticalTextTopToBottom = FALSE;
	// m_client_tab.GetPaintManager()->SetLayout(xtpTabLayoutRotated);
	m_client_tab.GetPaintManager()->m_bBoldSelected = TRUE;
	m_client_tab.GetPaintManager()->m_clientFrame = xtpTabFrameSingleLine;
	m_client_tab.GetPaintManager()->m_rcButtonMargin = CRect(2, 2, 1, 2);
	m_client_tab.GetPaintManager()->m_rcClientMargin = CRect(-2, -2, 0, 0);
	m_client_tab.GetPaintManager()->SetPosition(xtpTabPositionTop);
	m_client_tab.GetPaintManager()->SetAppearance(xtpTabAppearanceVisualStudio2010);
	m_client_tab.GetPaintManager()->SetColor(xtpTabColorDefault);

	m_client_tab.reset_tab_status();

	return 0;
}

void workspace_view::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_client_tab.GetSafeHwnd()) m_client_tab.MoveWindow(0, 0, cx, cy);
}

BOOL workspace_view::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void workspace_view::OnPaint()
{
	CPaintDC dc(this);

	// �������У��������д����൱��CXTPWindowRect rcClient(this);
	CXTPCommandBars* command_bar = (dynamic_cast<main_frame*>(GetParentFrame()))->GetCommandBars();
	if (!command_bar) return;

	CXTPClientRect rc(this);
	CXTPWindowRect rcClient(GetDlgItem(AFX_IDW_PANE_FIRST));
	ScreenToClient(&rcClient);	
	((CXTPOffice2007Theme*) (command_bar->GetPaintManager()))->FillWorkspace(&dc, rc, rcClient);
}

void workspace_view::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//m_pView->OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL workspace_view::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CWnd::PreCreateWindow(cs);
}

int workspace_view::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void workspace_view::OnDestroy()
{
	close_all_window(true); // ��ܻ��л���������ڴ�������ʼҳ���಻��Ҫ���ݱ���Ĵ��ڻ�������˴�ֱ��ǿ�ƹرգ���������ڴ�й©
	CWnd::OnDestroy();
}

void workspace_view::on_workspace_popup_menu_save()
{
	if (!m_client_tab.GetSafeHwnd()) return;

	int index = m_client_tab.get_last_right_clicked_tab_index();
	m_client_tab.save_tab_window_data(index);
}

void workspace_view::on_workspace_popup_menu_save_all()
{
	if (!m_client_tab.GetSafeHwnd()) return;

	m_client_tab.save_all_tab_window_data();
}

void workspace_view::on_workspace_popup_menu_close()
{
	if (!m_client_tab.GetSafeHwnd()) return;

	int index = m_client_tab.get_last_right_clicked_tab_index();
	m_client_tab.close_tab_and_clear_info(index);
}

void workspace_view::on_workspace_popup_menu_close_all()
{
	if (!m_client_tab.GetSafeHwnd()) return;
	if (!m_client_tab.close_all_tab_and_clear_info()) return;
}
