#include "stdafx.h"
#include <set>

#include "x_framework/macro.h"
#include "x_framework/tools/math_tools.h"
#include "x_framework/tools/file_tools.h"
#include "x_framework/tools/xml_tools.h"
#include "x_framework/tools/global_parameters.h"
#include "x_framework/interface/console_interface.h"
#include "x_framework/interface/window_interface.h"
#include "x_framework/interface/cmd_container_interface.h"
#include "x_framework/interface/app_ui_interface.h"

#include "THGridControl/THGridControlMac.h"
#include "THGridControl/GridGlobalData.h"

#include "app_ribbon_bar.h"
#include "help_configs.h"
#include "app_configs.h"
#include "app_skin_manager.h"
#include "main_application.h"

#include "main_frame.h"



#define MSG_DO_FUNCTION      WM_USER+1101



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


//////////////////////////////////////////////////////////////////////////

struct private_workspace_close_postmessage_parameters
{
	// ���²����ڹر�ʱ��Ч
	int			closing_action_type = 0;	// 0��ʾ�����������رգ�1��ʾ�����������ر�
	bool		force_close = false;		// ����ʾ������Ϣ��ֱ�ӵ��ô��ڹر�
	CString		window_class;
	int			window_index = -1;

	// ���²��������ʱ��Ч
	CString		node_tag;
	int			index = -1;
	int			image_index = -1;
};

struct private_workspace_added_postmessage_parameters
{
	// ���²����ڹر�ʱ��Ч
	CString		window_class;
	CString		node_tag;
	int			index = -1;
	int			image_index = -1;
};

struct private_do_command_postmessage_parameters
{
	CString domain;
	CString cmd;
	CString param;
};

//////////////////////////////////////////////////////////////////////////

main_frame::main_frame()
{
	m_is_style_changing.store(false);

	set_app_ui_interface(this);
	initialize_theme_style_settings();

	m_docking_pane_layout = nullptr;
	m_ribbon_bar = nullptr;

	m_current_theme = ID_THEME_STYLEBLUE2010; // Ĭ��ΪOffice 2010��ɫ���
	m_is_ribbon_mini_size = false;
	m_docking_panes_be_created = false;

	main_application* app = (main_application*) AfxGetApp();
	if (app)
	{
		int nValue = (int) app->GetProfileInt(_T("Skin"), _T("ThemeType"), ID_THEME_STYLESILVER2010);
		if (nValue == ID_THEME_STYLEBLUE2007
			|| nValue == ID_THEME_STYLESILVER2007
			|| nValue == ID_THEME_STYLEBLACK2007
			|| nValue == ID_THEME_STYLEBLUE2010
			|| nValue == ID_THEME_STYLESILVER2010
			|| nValue == ID_THEME_STYLEBLACK2010
			|| nValue == ID_THEME_STYLE_LIGHT2013
			|| nValue == ID_THEME_STYLE_DARK2013)
		{
			m_current_theme = nValue;
		}
		else
		{
			m_current_theme = ID_THEME_STYLEBLUE2010;
		}

		nValue = (int) app->GetProfileInt(_T("Skin"), _T("RibbonMini"), 0);
		m_is_ribbon_mini_size = (1 == nValue ? true : false);
	}

	// ��ʼ״̬��ǰ�л��ÿ��Ա����һ�ν�����ʾʱ��������л�������˸
	theme_style_settings setting = get_style_and_settings();
	m_bAllowDwm = (setting.m_enable_aero ? TRUE : FALSE);
}

main_frame::~main_frame()
{
	m_docking_pane_config.m_container.clear(); // ���uiData
	m_docking_pane_layout = nullptr; // ��Ҫdelete����ܻ��ڹرմ���ǰ����״̬���ͷŶ���
	XTPSkinManager()->LoadSkin(NULL); // ����Ƥ����Դ
}


BEGIN_MESSAGE_MAP(main_frame, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_WM_DROPFILES()
	ON_XTP_CREATECONTROL()

	ON_COMMAND_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, on_recent_file_list)
	ON_UPDATE_COMMAND_UI(ID_FILE_RECENTDOCUMENTS, on_update_ribbon_recentdocs)

	// ͣ����崴������ļ�����λ�úʹ�Сʱ����ã�Ҳ����ʹ��������ʵ���麯���ķ�ʽ��Ӧ��
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, on_docking_pane_notify)

	// Ribbon��Ϣ��Ӧ
	ON_COMMAND_RANGE(IDS_RIBBON_UI_MIN, IDS_RIBBON_UI_MAX, on_all_ribbon_tab_command)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_RIBBON_UI_MIN, IDS_RIBBON_UI_MAX, on_update_all_ribbon_tab_command)
	
	// Ribbon file menu bar��Ϣ��Ӧ
	ON_COMMAND_RANGE(IDS_RIBBON_FILE_MENU_MIN, IDS_RIBBON_FILE_MENU_MAX, on_all_ribbon_file_menu_command)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_RIBBON_FILE_MENU_MIN, IDS_RIBBON_FILE_MENU_MAX, on_update_all_ribbon_file_menu_command)

	// Ribbon quick access bar��Ϣ��Ӧ
	ON_COMMAND_RANGE(IDS_RIBBON_QUICK_ACCESS_MIN, IDS_RIBBON_QUICK_ACCESS_MAX, on_all_ribbon_quick_access_command)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_RIBBON_QUICK_ACCESS_MIN, IDS_RIBBON_QUICK_ACCESS_MAX, on_update_all_ribbon_quick_access_command)

	// Ribbon bar options������Ϣ��Ӧ
	ON_COMMAND_RANGE(IDS_RIBBON_OPTIONS_MIN, IDS_RIBBON_OPTIONS_MAX, on_all_ribbon_options_command)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_RIBBON_OPTIONS_MIN, IDS_RIBBON_OPTIONS_MAX, on_update_all_ribbon_options_command)

	// ���ټ���Ϣ��Ӧ
	ON_COMMAND_RANGE(ID_ACCELERATOR_CMD_MIN, ID_ACCELERATOR_CMD_MAX, on_all_accelerator_command)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACCELERATOR_CMD_MIN, ID_ACCELERATOR_CMD_MAX, on_update_all_accelerator_command)

	// �����л���Ϣ��Ӧ
	ON_COMMAND_RANGE(ID_THEME_STYLEBLUE2007, ID_THEME_STYLE_DARK2013, on_command_skin_and_theme_menu_item)
	ON_UPDATE_COMMAND_UI_RANGE(ID_THEME_STYLEBLUE2007, ID_THEME_STYLE_DARK2013, on_update_skin_and_theme_menu_item)

	ON_MESSAGE(MSG_DO_FUNCTION, on_msg_do_functor)

END_MESSAGE_MAP()



int main_frame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{  
	if (lpCreateControl->nID == ID_FILE_RECENTDOCUMENTS)
	{
		lpCreateControl->pControl = new CXTPRibbonControlSystemRecentFileList();
		return TRUE;
	}
 
	return FALSE;
}

BOOL main_frame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.hMenu = nullptr;
	cs.lpszClass = _T("app_main_frame");

	if (!CXTPFrameWnd::PreCreateWindow(cs)) return FALSE;

	// ע�ᴰ�����Ŀ����Ϊ�˸ı䴰�ڱ�����������ʽû���ַ�ʽƽ�����ҿ��Ը����׵�ȥ���˵���
	// ע�ᴰ��ʱ�������CS_HREDRAW��񣬷��򴰿ڿ��ܻ��ڶ�������һ����ɫ�Ĳ�Ӱ������CS_HREDRAW | CS_VREDRAW��������Ҫ���
	// ���ڱ����������Դ�Զ������˴��ݸ�����ɫ���Է�ֹ������û��HBRUSH�����ƿͻ������´����в�Ӱ
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass,  /*CS_HREDRAW |CS_VREDRAW | */CS_DBLCLKS,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME), (HBRUSH) (COLOR_WINDOW + 1));

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE; // �˷��Ҫ����ע����������Ч

	// cs.style |= WS_MAXIMIZE; ���ڴ˴����ô������,���������ʱ���ڴ�С������Ӿ�����

	// ��mfc��ע�ắ��Ҳһ������
	//cs.lpszClass = AfxRegisterWndClass(/*CS_HREDRAW | CS_VREDRAW | */CS_DBLCLKS, 
	//	::LoadCursor(NULL, IDC_ARROW), (HBRUSH) (COLOR_WINDOW + 1), AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

BOOL main_frame::PreTranslateMessage(MSG* pMsg)
{
	if (m_accelerator_map.get_accelerator_handle() && WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		// ���ټ�������������˾Ͳ�Ҫ�õײ㴦���������Ա��ⴥ��������δ����ļ��ټ�
		if (::TranslateAccelerator(GetSafeHwnd(), m_accelerator_map.get_accelerator_handle(), pMsg)) return TRUE;
	}

	return CXTPFrameWnd::PreTranslateMessage(pMsg);
}

CXTPControl* main_frame::get_ribbon_control(const CString& domain, const CString& cmd, const CString& param /*= _T("")*/, bool compare_param /*= false*/)
{
	app_ribbon_bar* rib = get_db_ribbon_bar();
	if (!rib || !rib->GetSafeHwnd()) return nullptr;

	int id = m_ribbon_config.get_id(domain, cmd, param, compare_param);
	if (-1 == id) return nullptr;

	return rib->get_control(id);
}

void main_frame::select_ribbon_tab(const CString& tab_name)
{
	if (!m_ribbon_bar || !m_ribbon_bar->GetSafeHwnd()) return;
	m_ribbon_bar->select_tab(tab_name);
}

void main_frame::show_ribbon_context_tabs(const std::vector<CString>& tab_names)
{
	if (!m_ribbon_bar || !m_ribbon_bar->GetSafeHwnd()) return;
	m_ribbon_bar->active_context_tabs_always_by_tab_name(tab_names);
}

void main_frame::add_recent_file_list(const CString& file_path_name)
{
	for (int i = 0; i < the_main_app.get_recent_list_item_count(); ++i)
		if (0 == file_path_name.CompareNoCase(the_main_app.get_recent_list_item_full_path(i))) return;

	try
	{
		// mfc��ܶ���ʷ�б�����ʱ����Ҫ�󣬱��������ⲿ�ֶ���ӣ���û��ʹ��mfc���ĵ���ͼ���ƣ���˻����쳣����
		// ���쳣��Ӱ�������ʷ�б�Ĺ��ܣ����ε�
		// ���⣬�����ʷ�б�ʱmfc�ڲ�ʹ����һЩ���ϵ�API�ӿڣ��ӿ�Ҫ�����·������ʹ��"/"����Ҫʹ��"\\"������
		// ���ж���
		CString tmp_file_name = file_path_name;
		tmp_file_name.Replace(_T('/'), _T('\\'));
		the_main_app.AddToRecentFileList(tmp_file_name);
	}
	catch (...)
	{
		// nothing;
	}
}

void main_frame::get_recent_file_list(std::vector<CString>& file_list)
{
	file_list.clear();

	for (int i = 0; i < the_main_app.get_recent_list_item_count(); ++i)
		file_list.push_back(the_main_app.get_recent_list_item_full_path(i));
}

bool main_frame::docking_panes_be_created()
{
	return m_docking_panes_be_created;
}

bool main_frame::do_function_by_post_message(functor_by_message* fun)
{
	return (TRUE == PostMessage(MSG_DO_FUNCTION, (WPARAM) fun, NULL));
}

bool main_frame::do_function_by_send_message(functor_by_message* fun)
{
	return (TRUE == (BOOL)SendMessage(MSG_DO_FUNCTION, (WPARAM) fun, NULL));
}

CDocument* main_frame::GetActiveDocument()
{
	// ��������麯���ܿ�mfc����Ч�Լ��
	return nullptr;
}

bool main_frame::close_working_space_view_by_class(const CString& class_name, bool force_close /*= false*/)
{
	for (int i = m_workspace_view.get_tab_count() - 1; i >= 0; --i)
	{
		tuple<CString, CString, CWnd*> info;
		if (false == m_workspace_view.get_tab_window_and_info(info, i)) continue;

		if (std::get<0>(info) == class_name) m_workspace_view.close_tab_by_index(i, force_close);
	}

	return true;
}

bool main_frame::close_working_space_view_by_tag(const CString& tag, bool force_close /*= false*/)
{
	for (int i = m_workspace_view.get_tab_count() - 1; i >= 0; --i)
	{
		tuple<CString, CString, CWnd*> info;
		if (false == m_workspace_view.get_tab_window_and_info(info, i)) continue;

		if (std::get<1>(info) == tag) m_workspace_view.close_tab_by_index(i, force_close);
	}

	return true;
}

bool main_frame::close_working_space_view_by_index(int index, bool force_close/* = false*/)
{
	return m_workspace_view.close_tab_by_index(index, force_close);
}

int main_frame::get_working_space_view_count()
{
	return m_workspace_view.get_tab_count();
}

bool main_frame::get_tab_window_and_info(std::tuple<CString, CString, window_interface*>& info, int index) const
{
	tuple<CString, CString, CWnd*> wnd_info;
	if (!m_workspace_view.get_tab_window_and_info(wnd_info, index)) return false;

	window_interface* wx = dynamic_cast<window_interface*>(std::get<2>(wnd_info));
	info = make_tuple(std::get<0>(wnd_info), std::get<1>(wnd_info), wx);

	return true;
}

int main_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	//lpCreateStruct->dwExStyle |= WS_EX_ACCEPTFILES; // ֧���ļ��ϷŴ�
	if (-1 == CXTPFrameWnd::OnCreate(lpCreateStruct)) return -1;

	// ֧���ļ��ϷŴ򿪣���vista֮���ϵͳ����uac���ƣ��¼�����������api�ӿڣ����ڴ����Ȩ�޵ĳ������߰汾�ĳ�
	// �򷢲�����Ϣ������������Թ���ԱȨ�������ĳ�����޷�ͨ���Ϸ��ļ��ķ�ʽ�����ļ���xp��û������ӿڣ�Ҳ��
	// �ô����������Ҳ�����������Ϸ���Ϣ��û��Ȩ�����ƣ�
#ifdef WINVER
#	if WINVER >= 0x0600 // Vista

	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD); // 0x0049 == WM_COPYGLOBALDATA

#	endif
#endif

	DragAcceptFiles(TRUE);

	// ��Ҫ����ŵ�Ӧ�ó����ʵ�����У��������Ƥ������������ס���򣬵��ǵ���������Ƥ���Ĺ���ģ��֮ǰ
	// ���ú�Ƥ��������ǰ��Ҫʹ�ýӿڻ�ȡ����������������һ��������ʹ�õ�ͬʱɾ��������������⣬XTPƤ����������
	// ���캯�����Զ���this����ΪĬ�ϵĹ�����������ʱ������ɾ��������������ɾ����
	CXTPSkinManager::SetSkinManager(new app_skin_manager(), FALSE);

	// ��ܳ�ʼ��������
	if (!InitCommandBars()) return -1;

	// ����Ribbon������
	SetMenu(nullptr); // ��ȥ�������ڵĲ˵�
	CXTPCommandBars* bars = GetCommandBars();
	if (!bars) return -1;
	if (!create_ribbon_bar(bars)) return -1;

	// ����״̬��
	if (!create_status_bar()) return -1;

	// ����ͣ����������
	create_docking_pane_manager();
	initialize_docking_panes_config();
	if (m_docking_pane_config.m_force_create_by_frame) create_docking_panes();

	// ���ó���Ĭ�ϱ���
	set_title(_T(""));

	// �����������Ӵ���
	create_working_space();

	// ������ɺ��л�����Ҫ��ʾ������
	on_command_skin_and_theme_menu_item(m_current_theme); // ���ý�����

	// ���ؼ��ټ�ӳ���
	m_accelerator_map.create_accelerator_table_by_xml();

	return 0;
}

void main_frame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CXTPFrameWnd::OnGetMinMaxInfo(lpMMI);

	// ��ֹ���ڹ�Сribbon�����ƻ���������
	lpMMI->ptMinTrackSize.x = 400;
	lpMMI->ptMinTrackSize.y = 300;
}

void main_frame::OnSysCommand(UINT nID, LPARAM lParam)
{
	CXTPFrameWnd::OnSysCommand(nID, lParam);
}

void main_frame::OnDropFiles(HDROP drop_info)
{
	int drop_count = DragQueryFile(drop_info, -1, NULL, 0);
	if (drop_count >= 1)
	{
		TCHAR file_path_name[MAX_PATH + 1] = { 0 };
		DragQueryFile(drop_info, 0, file_path_name, MAX_PATH); // ֻҪ��һ��
		DragFinish(drop_info); // �ϷŽ�����,�ͷ��ڴ�

		CString ext = file_tools::get_ext_from_file_path_name(file_path_name);
		if (0 != ext.CompareNoCase(_T("dbr")))
		{
			AfxMessageBox(_T("�޷�ʶ����ļ�����"), MB_OK | MB_ICONERROR);
			return;
		}

		cmd_trigger_manager::fire(_T("app"), _T("open"), file_path_name);
	}

	// ��Ҫ�ٵ��û�����ļ���
	// CXTPFrameWnd::OnDropFiles(drop_info);
}

BOOL main_frame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ������ǰ�Ų����������������������в��ϳ���Ĵ���Ҫ��������ע�����ã���Ϊ��������������Ĺ����п���
	// ��Ҫ��رգ����õ����������ٴλ�ȡ����������ʱ�����Ѿ��������ٸ��ݶ�̬���Ͳ�ѯ��ض���ʱ�����
	// [��������������������л�����Ȼ�����ڵ�ġ���ա�]������
	//window_interface* wnd_ix = get_current_working_space_windows();
	//CWnd* wnd = dynamic_cast<CWnd*>(wnd_ix);
	//
	//// let the view have first crack at the command
	//if (wnd && wnd->GetSafeHwnd() && wnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// otherwise, do default handling
	return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void main_frame::OnSize(UINT flag, int cx, int cy)
{
	CXTPFrameWnd::OnSize(flag, cx, cy); 
}

void main_frame::OnClose()
{
	CRect rt;
	if (!IsZoomed())
	{
		GetWindowRect(&rt);
	}
	else
	{
		// �Ѿ�����ˣ�Ҫ�õ����֮ǰ�Ĵ��ڳߴ��λ��
		WINDOWPLACEMENT pl;
		GetWindowPlacement(&pl);
		rt.left = pl.rcNormalPosition.left;
		rt.top = pl.rcNormalPosition.top;
		rt.right = pl.rcNormalPosition.right;
		rt.bottom = pl.rcNormalPosition.bottom;
	}

	app_size_config_helper::save_last_main_frame_pos_and_size(rt);
	app_size_config_helper::save_maxmize_flag(IsZoomed() ? true : false);

	// �����Ƚ���������Ӧ��������ȷ������ڰ������ݺͽ�����Ⱥ�����ƣ�Ȼ�����ٴ������
	cmd_trigger_manager::fire(_T("sys"), _T("quit"), _T(""));
	
	// �ƺ���һ���������ģ����棬��ʹ��������Ѿ��������ٽ��д���Ҳû���⣬˳�򲻹ܣ����Ҫ���棬����
	// ���������Լ���Ӧ����Ϣ����ȷ����ý����߼���������������ط���
	if (data_center::is_open()) data_center::close_file();
	close_all_workspace_window(true);
	destroy_all_docking_panes();

	CXTPFrameWnd::OnClose();
}

XTPToolTipStyle main_frame::get_tooltip_style() const
{
	return get_style_and_settings().m_tooltip_style;
}

std::pair<XTPPaintTheme, CString> main_frame::get_paint_style() const
{
	theme_style_settings setting = get_style_and_settings();
	return make_pair(setting.m_paint_theme, setting.m_theme_dll_ini_config_file_name);
}

XTPDockingPanePaintTheme main_frame::get_docking_panes_style() const
{
	return get_style_and_settings().m_docking_pane_theme;
}

bool main_frame::create_ribbon_bar(CXTPCommandBars* command_bars)
{
	// �����uiData
	m_ribbon_config.m_all_tabs.clear();

	if (!command_bars) return false;

	// ��ʼ��ȡxml����
	CString xml_file_full_path = file_tools::get_app_path_with_xg() + _T("gui_xml\\app_ribbon.xml");
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(string_tools::CString_to_string(xml_file_full_path).c_str())) return false;

	tinyxml2::XMLElement* node = doc.FirstChildElement("app_ribbon");
	if (!node || node->NoChildren()) return true;

	m_ribbon_config.initlize_config_data(node);

	m_ribbon_bar = (app_ribbon_bar*) command_bars->Add(_T("app_ribbon_bar"), xtpBarTop, RUNTIME_CLASS(app_ribbon_bar));
	if (!m_ribbon_bar) return false;

	// Ribbon �Ľ������������Ѿ�������
	m_ribbon_bar->initlize_ribbon_bar(m_ribbon_config);
	m_ribbon_bar->EnableQuickAccessCustomItem(FALSE);

	EnableToolTips(FALSE); // ����ϵͳԭ�ȵ�ToolTip

	// ����Ribbon����ToolTip
	CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
	if (pToolTipContext)
	{
		CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
		pToolTipContext->SetStyle(get_tooltip_style());
		pToolTipContext->ShowTitleAndDescription();
		pToolTipContext->ShowImage(TRUE, 0);
		pToolTipContext->SetMargin(XTP_DPI(CRect(2, 2, 2, 2)));
		pToolTipContext->SetMaxTipWidth(XTP_DPI_X(200));
		pToolTipContext->SetFont(command_bars->GetPaintManager()->GetIconFont());
		pToolTipContext->SetDelayTime(TTDT_INITIAL, 300);
	}

	// ��ʾRibbon�����õĿ�ݼ�
	command_bars->GetCommandBarsOptions()->bShowKeyboardTips = TRUE;
	command_bars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;

	// ����DPI����Ӧģ�ͣ��ڸ߷���ģʽ��ͼ���Զ�����
	command_bars->GetPaintManager()->m_bAutoResizeIcons = TRUE;
	command_bars->GetCommandBarsOptions()->SetDPIScallingOptions(TRUE);

	return true;
}

bool main_frame::create_status_bar()
{
	return true; // Ŀǰ�ò�������ʱע��

	//std::vector<UINT> indicators(1, ID_SEPARATOR);

	//if (-1 == m_status_bar.Create(this)) return false;
	//m_status_bar.SetIndicators(&indicators[0], 10001);

	//m_status_bar.SetCommandBars(GetCommandBars());
	//m_status_bar.SetDrawDisabledText(false);
	//m_status_bar.GetStatusBarCtrl().SetMinHeight(22);

	////////add_status_progress();

	////////std::vector<UINT> xmlindicators;
	////////Cx_ConfigSection items(m_frameNode.GetSection(L"statusbar/indicators"));
	////////for (int i = 0;; i++)
	////////{
	////////	Cx_ConfigSection item(items.GetSectionByIndex(L"item", i));
	////////	UINT id = GetNodeID(item, L"id");
	////////	if (0 == id)
	////////		break;
	////////	xmlindicators.push_back(id);
	////////	if (ID_SEPARATOR == id && xmlindicators.front() == id)
	////////		xmlindicators.erase(xmlindicators.begin());
	////////}

	////////for (size_t i = 0; i < xmlindicators.size(); ++i)
	////////{
	////////	m_status_bar.AddIndicator(xmlindicators[i]);
	////////}

	//m_status_bar.EnableCustomization(TRUE);

	//return true;
}

void main_frame::add_status_progress()
{
	//// Create the edit control and add it to the status bar
	//if (!m_wndProgCtrl.Create(WS_CHILD | WS_VISIBLE/*|PBS_SMOOTH*/,
	//	CRect(0, 0, 0, 0), &m_status_bar, 0))
	//{
	//	TRACE0("Failed to create edit control.\n");
	//	return;
	//}

	//// add the indicator to the status bar.
	//CXTPStatusBarPane* pPane = m_status_bar.AddIndicator(ID_INDICATOR_PROG);

	//// Initialize the pane info and add the control.
	//int nIndex = m_status_bar.CommandToIndex(ID_INDICATOR_PROG);
	//ASSERT(nIndex != -1);

	//pPane->SetCaption(pPane->GetText());
	//pPane->SetText(_T(""));

	//m_status_bar.SetPaneWidth(nIndex, 180);
	//m_status_bar.SetPaneStyle(nIndex, m_status_bar.GetPaneStyle(nIndex) | SBPS_NORMAL);
	//m_status_bar.AddControl(&m_wndProgCtrl, ID_INDICATOR_PROG, FALSE);

	//pPane->SetCustomizationVisible(FALSE);
}

void main_frame::hide_docking_pane_by_id(int id)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (id == y.m_child_window_id)
			{
				m_pane_manager.ClosePane(y.m_id);
				return;
			}
		}
	}
}

void main_frame::hide_docking_pane_by_class(const CString& class_name)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (0 == y.m_child_window_class.CompareNoCase(class_name))
			{
				m_pane_manager.ClosePane(y.m_id);
				return;
			}
		}
	}
}

void main_frame::hide_docking_pane_by_identify(const CString& identify)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (0 == y.m_app_identify.CompareNoCase(identify))
			{
				m_pane_manager.ClosePane(y.m_id);
				return;
			}
		}
	}
}

void main_frame::show_docking_pane_by_id(int id)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (id == y.m_child_window_id)
			{
				m_pane_manager.ShowPane(y.m_id, FALSE);
				return;
			}
		}
	}
}

void main_frame::show_docking_pane_by_class(const CString& class_name)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (0 == y.m_child_window_class.CompareNoCase(class_name))
			{
				m_pane_manager.ShowPane(y.m_id, FALSE);
				return;
			}
		}
	}
}

void main_frame::show_docking_pane_by_identify(const CString& identify)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (0 == y.m_app_identify.CompareNoCase(identify))
			{
				m_pane_manager.ShowPane(y.m_id, FALSE);
				return;
			}
		}
	}
}

void main_frame::reload_docking_pane_by_id(int id, const CString& node_tag)
{
	window_interface* ix = get_docking_pane_window_by_id(id);
	if (ix) ix->load_data_to_ui(node_tag);
}

void main_frame::reload_docking_pane_data_by_identify(const CString& identify, const CString& node_tag)
{
	for (auto& x : m_docking_pane_config.m_container)
		for (single_pane_data& y : x.m_panes)
			if (0 == y.m_app_identify.CompareNoCase(identify))
				reload_docking_pane_by_id(y.m_child_window_id, node_tag);
}

void main_frame::reload_docking_pane_data_by_class(const CString& class_name, const CString& node_tag)
{
	for (auto& x : m_docking_pane_config.m_container)
		for (single_pane_data& y : x.m_panes)
			if (0 == y.m_child_window_class.CompareNoCase(class_name))
				reload_docking_pane_by_id(y.m_child_window_id, node_tag);
}

void main_frame::destroy_all_docking_panes()
{
	// Ҫ�ڴ��ڹر�֮ǰ���浱ǰ���ֺͰ汾
	app_size_config_helper::save_docking_panes_config_version(m_docking_pane_config.m_version);

	if (m_docking_pane_layout)
	{
		m_pane_manager.GetLayout(m_docking_pane_layout);
		m_docking_pane_layout->Save(_T("docking_pane_layout"));
		safe_delete(m_docking_pane_layout); // �ر�ǰ����
	}

	// ����������ڲ����Ӵ���
	// ����̬������ͣ����崰��
	for (auto& x : m_all_created_panes_child_wnd)
	{
		if (nullptr == x) continue;

		CView* view_wnd = dynamic_cast<CView*>(x);
		CFrameWnd* frame_wnd = dynamic_cast<CFrameWnd*>(x);
		window_interface* wnd_x = dynamic_cast<window_interface*>(x);

		if (wnd_x) wnd_x->pre_close(_T(""));

		if (x->GetSafeHwnd()) x->SendMessage(WM_CLOSE);// x->DestroyWindow();
		if (view_wnd || frame_wnd)
			x = nullptr; // ���������Ѿ�delete,��û����Ϊnullptr
		else
			safe_delete(x);
	}
	m_all_created_panes_child_wnd.clear();

	// ���������
	m_pane_manager.DestroyAll();
	m_docking_panes_be_created = false;
}

void main_frame::create_all_docking_panes()
{
	if (!m_docking_panes_be_created) create_docking_panes();
}

void main_frame::get_all_working_space_windows(std::vector<window_interface*>& wnds)
{
	wnds.clear();

	int tab_count = m_workspace_view.get_tab_count();
	for (int i = 0; i < tab_count; ++i)
	{
		window_interface* wnd = dynamic_cast<window_interface*>(m_workspace_view.get_tab_window(i));
		if (wnd) wnds.push_back(wnd);
	}
}

void main_frame::update_all_working_space_window_captions()
{
	if (!GetSafeHwnd() || !m_workspace_view.GetSafeHwnd()) return; // �����ڴ���ǰҲ������Ϣ�������������£�Ҫ���ж�

	m_workspace_view.update_tab_caption(-1);
}

void main_frame::get_current_working_space_active_window(std::pair<CString, CString>& info)
{
	int cur_index = m_workspace_view.get_current_tab_index();
	if (-1 == cur_index)
	{
		info = make_pair(_T(""), _T(""));
		return;
	}

	// 0������������ 1���������tag��2������ָ��
	tuple<CString, CString, CWnd*> info_detail;
	bool is_ok = m_workspace_view.get_tab_window_and_info(info_detail, cur_index);
	if (!is_ok)
	{
		info = make_pair(_T(""), _T(""));
		return;
	}

	info = make_pair(std::get<0>(info_detail), std::get<1>(info_detail));
}

window_interface* main_frame::get_current_working_space_windows()
{
	int cur_index = m_workspace_view.get_current_tab_index();
	if (-1 == cur_index) return nullptr;

	// 0������������ 1���������tag��2������ָ��
	tuple<CString, CString, CWnd*> info_detail;
	bool is_ok = m_workspace_view.get_tab_window_and_info(info_detail, cur_index);
	if (!is_ok) return nullptr;

	return dynamic_cast<window_interface*>(std::get<2>(info_detail));
}

void main_frame::get_all_working_space_windows_info(std::vector<std::pair<CString, CString>>& info)
{
	info.clear();

	int tab_count = m_workspace_view.get_tab_count();
	for (int i = 0; i < tab_count; ++i)
	{
		// ��ȡָ����ǩ�Ĵ��ڸ�����Ϣ 0������������ 1���������tag��2������ָ��
		tuple<CString, CString, CWnd*> window_info;
		if (!m_workspace_view.get_tab_window_and_info(window_info, i)) continue;

		info.push_back(make_pair(std::get<0>(window_info), std::get<1>(window_info)));
	}
}

void main_frame::get_all_docking_pane_window(std::vector<window_interface*>& wnds)
{
	wnds.clear();

	CXTPDockingPaneInfoList& pane_list = m_pane_manager.GetPaneList();
	POSITION pos = pane_list.GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = pane_list.GetNext(pos);
		CXTPDockingPane* pane = info.pPane;
		if (!pane || pane->IsClosed()) continue;

		CWnd* child = pane->GetChild();
		if (!child || !child->GetSafeHwnd()) continue;

		window_interface* ix = dynamic_cast<window_interface*>(child);
		if (ix) wnds.push_back(ix);
	}
}

void main_frame::get_all_docking_pane_window_by_class(std::vector<window_interface*>& wnds, const CString& class_name)
{
	wnds.clear();

	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			// Ϊ���򲻹���
			if (!class_name.IsEmpty() && 0 != y.m_child_window_class.CompareNoCase(class_name)) continue;

			CXTPDockingPane* pane = m_pane_manager.FindPane(y.m_id);
			if (!pane) continue;;

			CWnd* child = pane->GetChild();
			window_interface* child_interface = dynamic_cast<window_interface*>(child);
			if (child_interface) wnds.push_back(child_interface);
		}
	}
}

window_interface* main_frame::get_docking_pane_window_by_identify(const CString& app_identify)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (0 != y.m_app_identify.CompareNoCase(app_identify)) continue;

			CXTPDockingPane* pane = m_pane_manager.FindPane(y.m_id);
			if (!pane) return (LRESULT)nullptr;

			CWnd* child = pane->GetChild();
			window_interface* child_interface = dynamic_cast<window_interface*>(child);
			if (child_interface) return child_interface;
		}
	}

	return nullptr;
}

void main_frame::save_all_workspace_window_ui_data()
{
	m_workspace_view.save_all_window_data();
}

void main_frame::save_all_docking_pane_ui_data()
{
	CXTPDockingPaneInfoList& pane_list = m_pane_manager.GetPaneList();

	POSITION pos = pane_list.GetHeadPosition();
	while (pos)
	{
		XTP_DOCKINGPANE_INFO& info = pane_list.GetNext(pos);
		CXTPDockingPane* pane = info.pPane;
		if (!pane || pane->IsClosed()) continue;

		CWnd* child = pane->GetChild();
		if (!child || !child->GetSafeHwnd()) continue;

		window_interface* ix = dynamic_cast<window_interface*>(child);
		if (ix) ix->save_data_from_ui();
	}
}

bool main_frame::close_all_workspace_window(bool force_close/* = false*/)
{
	return m_workspace_view.close_all_window(force_close);
}

void main_frame::set_title(const CString& title)
{
	if (!GetSafeHwnd()) return;

	CString app_name = app_configs::m_application_name;
	if (title.IsEmpty())
		SetWindowText(app_name);
	else
		SetWindowText(app_name + _T(" - ") + title);
}

CString main_frame::get_title() const
{
	CString app_name = app_configs::m_application_name;
	app_name += _T(" - ");

	CString title;
	if (GetSafeHwnd()) GetWindowText(title);

	if (0 == app_name.CompareNoCase(title.Left(app_name.GetLength())))
	{
		title.Delete(0, app_name.GetLength());
	}

	return title;
}


void main_frame::update_working_space_tab_caption(int index)
{
	if (index < 0 || index >= get_working_space_view_count()) return;

	tuple<CString, CString, window_interface*> info;
	if (!get_tab_window_and_info(info, index)) return;

	if (nullptr == std::get<2>(info)) return;
	CString new_caption = std::get<2>(info)->get_title();

	m_workspace_view.set_tab_caption(index, new_caption);
}

void main_frame::create_docking_pane_manager()
{
	m_pane_manager.InstallDockingPanes(this);

	m_pane_manager.SetCaptionDirection(xtpPaneCaptionHorizontal);
	m_pane_manager.SetDefaultPaneOptions(0); // Ĭ��״̬���޲˵���ť
	m_pane_manager.SetShowContentsWhileDragging(TRUE);
	m_pane_manager.EnableKeyboardNavigate(xtpPaneKeyboardUseCtrlTab);
	m_pane_manager.SetAlphaDockingContext(TRUE);
	m_pane_manager.UseSplitterTracker(TRUE); // m_pane_manager.UseSplitterTracker(FALSE);
	m_pane_manager.SetStickyFloatingFrames(TRUE);
	m_pane_manager.SetThemedFloatingFrames(TRUE);
	m_pane_manager.SetSideDockingMargin(CRect(2, 2, 2, 2));
	m_pane_manager.EnableSideDocking();

	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_clientFrame = xtpTabFrameSingleLine;
	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_bStaticFrame = TRUE;
	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_bShowIcons = FALSE;
	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_bFillBackground = TRUE;
	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_bShowTabs = TRUE;
	m_pane_manager.GetPaintManager()->GetTabPaintManager()->m_bBoldSelected = TRUE;

	if (app_configs::is_skin_enable())
	{
		m_pane_manager.SetTheme(get_docking_panes_style());
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2007);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetColor(xtpTabColorDefault);
	}
	else
	{
		m_pane_manager.SetTheme(xtpPaneThemeWinNative);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearancePropertyPage);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetColor(xtpTabColorWinNative);
	}

	m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetLayout(xtpTabLayoutSizeToFit);
}

void main_frame::initialize_docking_panes_config()
{
	// �����uiData
	m_docking_pane_config.m_container.clear();

	// ��ʼ��ȡxml����
	CString xml_file_full_path = file_tools::get_app_path_with_xg() + _T("gui_xml\\app_docking_panes.xml");
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(string_tools::CString_to_string(xml_file_full_path).c_str())) return;

	tinyxml2::XMLElement* node = doc.FirstChildElement("app_docking_panes");
	if (!node || node->NoChildren()) return;
	m_docking_pane_config.m_force_create_by_frame = get_xml_node_attribute_bool(node, _T("force_create_by_frame"));
	m_docking_pane_config.m_auto_layout = get_xml_node_attribute_bool(node, _T("auto_layout"));
	m_docking_pane_config.m_version = get_xml_node_attribute_str(node, _T("config_version"));

	m_docking_pane_config.init_docking_pane_data(node);
}

void main_frame::create_docking_panes()
{
	m_docking_panes_be_created = true;

	CString docking_panes_config_version = app_size_config_helper::get_docking_panes_config_version();

	const int pane_wnd_min_id = 10100; // ��Ҫ���������ڳ�ͻ
	const int pane_wnd_max_id = 19100; // ��Ҫ���������ڳ�ͻ
	int pane_wnd_id = pane_wnd_min_id;

	// 1.�ȷ���ø����ڵ�id���Ӵ��ڵ�id
	for (pane_container_data& container : m_docking_pane_config.m_container)
	{
		for (single_pane_data& pane_data : container.m_panes)
		{
			pane_data.m_id = pane_wnd_id;
			
			// һ�㲻�����꣬Ϊ��ֹ�û����Ϲش��ڣ����ϴ���������������ȴռ�ô���id,��˿����ڴﵽ���ֵʱ������Сֵ
			++pane_wnd_id;
			if (pane_wnd_id > pane_wnd_max_id) pane_wnd_id = pane_wnd_min_id;


			pane_data.m_child_window_id = pane_wnd_id;
			
			++pane_wnd_id;
			if (pane_wnd_id > pane_wnd_max_id) pane_wnd_id = pane_wnd_min_id;
			++pane_wnd_id;
		}
	}

	safe_delete(m_docking_pane_layout);
	m_docking_pane_layout = m_pane_manager.CreateLayout();

	// 2.��������ּ���///////////////////////////////////////////////////////////////
	if (m_docking_pane_config.m_version == docking_panes_config_version
		&& m_docking_pane_config.m_auto_layout
		&& m_docking_pane_layout->Load(_T("docking_pane_layout")))
	{
		m_pane_manager.SetLayout(m_docking_pane_layout);
		
		// xtp�����𴴽��Ӵ��ڣ�Ҫ�ֶ�����
		for (pane_container_data& container : m_docking_pane_config.m_container)
		{
			for (single_pane_data& pane_data : container.m_panes)
			{
				CXTPDockingPane* pane_cur = m_pane_manager.FindPane(pane_data.m_id);
				if (!pane_cur)
				{
					app_print_info(MT_ERROR, _T("�����ڲ�����m_pane_manager.SetLayout(m_docking_pane_layout)δ����ȷ�������"));
					continue;
				}

				if (!create_single_pane_child_window(pane_data, pane_data.m_child_window_id, pane_cur)) continue;
				// reload_docking_pane_by_id(pane_data.m_child_window_id, _T("")); // ͣ������Ŀǰ�ò���·�� // �����������ڲ�
			}
		}

		return;
	}

	// 3.����ǰ�xml����///////////////////////////////////////////////////////////////

	// �ȴ������е���壬�ٸ���xml��ͣ����������ϵ���в���
	for (pane_container_data& container : m_docking_pane_config.m_container)
	{
		CRect container_rect(0, 0, container.m_width <= 0 ? 300 : container.m_width, container.m_height <= 0 ? 120 : container.m_height);

		CXTPDockingPane* pane_pre = nullptr;
		CXTPDockingPane* pane_select = nullptr;
		for (single_pane_data& pane_data : container.m_panes)
		{			
			CXTPDockingPane* pane_cur = m_pane_manager.CreatePane(pane_data.m_id, container_rect, xtpPaneDockLeft, pane_pre);
			if (!pane_cur) continue;

			// �Ӵ��ڴ���ʧ�ܲ�Ҫcontinue��������������������ز�������Ϊ��ܲ������Ӵ��ڣ�ֻ����pane
			// if (!create_single_pane_child_window(pane_data, pane_wnd_id, pane_cur)) continue;
			create_single_pane_child_window(pane_data, pane_wnd_id, pane_cur);

			// ����������
			pane_cur->SetTitle(pane_data.m_pane_title + _T("\n") + pane_data.m_tab_title);

			DWORD options = 0;
			if (pane_data.m_no_close_button) options |= xtpPaneNoCloseable;
			if (pane_data.m_no_caption) options |= xtpPaneNoCaption;
			if (pane_data.m_no_hideable) options |= xtpPaneNoHideable;
			if (pane_data.m_no_dockable) options |= xtpPaneNoDockable;
			pane_cur->SetOptions(options);
			
			if (pane_pre) m_pane_manager.AttachPane(pane_cur, pane_pre);
			pane_pre = pane_cur;
		}
	}

	auto direction_text_to_pos = [](const CString& direction)
	{
		XTPDockingPaneDirection pos;
		if (0 == direction.CompareNoCase(_T("left")))
			pos = xtpPaneDockLeft;
		else if (0 == direction.CompareNoCase(_T("right")))
			pos = xtpPaneDockRight;
		else if (0 == direction.CompareNoCase(_T("top")))
			pos = xtpPaneDockTop;
		else if (0 == direction.CompareNoCase(_T("bottom")))
			pos = xtpPaneDockBottom;
		else
			pos = xtpPaneDockLeft;

		return pos;
	};

	// ���������Ĳ���λ�ü�ѡ�С��Ƿ����ص�����
	for (pane_container_data& container : m_docking_pane_config.m_container)
	{
		// ������ϵ���ֻ����ǰ��������������β���
		// �Ȳ��������������
		CXTPDockingPane* pane_neighbor = nullptr;
		if (!container.m_neighbor_identify.IsEmpty())
		{
			for (pane_container_data& container_temp : m_docking_pane_config.m_container)
			{
				if (&container_temp == &container) continue; // �����Լ������Լ�
				if (0 != container.m_neighbor_identify.CompareNoCase(container_temp.m_app_identify)) continue;

				for (single_pane_data& pane_data_temp : container_temp.m_panes)
				{
					CXTPDockingPane* pane_cur = m_pane_manager.FindPane(pane_data_temp.m_id);
					if (pane_cur && !pane_cur->IsClosed()) // �رյ��޷�����
					{
						pane_neighbor = pane_cur;
						break;
					}
				}
				
				if (pane_neighbor) break;
			}
		}

		CXTPDockingPane* pane_pre = nullptr;
		CXTPDockingPane* pane_select = nullptr;

		for (single_pane_data& pane_data : container.m_panes)
		{
			CXTPDockingPane* pane_cur = m_pane_manager.FindPane(pane_data.m_id);
			if (!pane_cur) continue;

			// һ�������е���������������ѡ���ʽ����һ��
			if (pane_pre)
				m_pane_manager.AttachPane(pane_cur, pane_pre);
			else
				m_pane_manager.DockPane(pane_cur, direction_text_to_pos(container.m_direction_by_frame), pane_neighbor); // ͬһ����ֻ�е�һ��m_start_loadΪtrue��ѡ�����������֧

			if (!pane_data.m_start_load) // �����ص�ҲҪ������ֻ�����ڱ�Ҫ��ʱ�����ʾ��������Ҫ��ʱ��m_pane_manager��֪����ô����
			{
				m_pane_manager.ClosePane(pane_cur); // ֻҪ�����٣�m_pane_manager�����ٴ�����Ҫ��ʱ�򴴽����������һ���closeʱ��λ�ô�������
				// pane_pre = pane_cur; // �رջ�Ӱ�츽�ӹ�ϵ�����pane_preֻ������һ��û�йرյ����
			}
			else
			{
				pane_pre = pane_cur; // ��close�Ĳ���������attach����hide�Ŀ��ԣ���˷��ڴ������Ƿ���ʾ֮��
				if (pane_data.m_select && nullptr == pane_select) pane_select = pane_cur; // ֻ��¼��һ��ѡ�еģ���showΪtrue��
			}
		}

		if (nullptr == pane_select) pane_select = pane_pre; // ѡ�����һ��������
		if (pane_select) // ָ��Ϊ��ʱ����Ĭ����Ϊ
		{
			if (container.m_show)
				m_pane_manager.ShowPane(pane_select);
			else
				m_pane_manager.HidePane(pane_select); // ����Ϊһ��ͣ����ǩ
		}
	}

	// �������Ժ��ȡ��ǰ���֣��Ա㱣����
	m_pane_manager.GetLayout(m_docking_pane_layout);
}

bool main_frame::create_single_pane_child_window(single_pane_data& pane_data, int pane_wnd_id, CXTPDockingPane* pane_cur)
{
	if (!pane_cur) return false;
	if (pane_data.m_child_window_class.IsEmpty()) return true; // û���Ӵ���Ҫ����Ҳ���Ǵ����ɹ�

	// Ҫ�󴰿�����붨��DECLARE_SERIAL�꣨���Բ���д���л�����,������Բ��������л��꣩�������Ƕ���DECLARE_CREATE�����򴴽����ɹ�
	CObject* obj = CRuntimeClass::CreateObject(pane_data.m_child_window_class);
	if (!obj)
	{
		safe_delete(obj);
		return false;
	}

	CWnd* child_wnd = dynamic_cast<CWnd*>(obj);
	if (!child_wnd)
	{
		safe_delete(obj);
		return false;
	}

	m_all_created_panes_child_wnd.push_back(child_wnd);

	child_wnd->Create(nullptr, nullptr, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		CRect(), this, pane_data.m_child_window_id);

	pane_cur->Attach(child_wnd); // ���ڽ���xtp����

	CView* view = dynamic_cast<CView*>(child_wnd);
	if (view) view->OnInitialUpdate();

	reload_docking_pane_by_id(pane_data.m_child_window_id, _T("")); // ͣ������Ŀǰû����Ҫ·�������

	return true;
}

void main_frame::reload_all_docking_panes_data()
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			CXTPDockingPane* pane = m_pane_manager.FindPane(y.m_id);
			if (!pane) continue;

			CWnd* child = pane->GetChild();
			if (!child) continue;

			window_interface* ix = dynamic_cast<window_interface*>(child);
			if (ix) ix->load_data_to_ui(_T("")); // Ŀǰͣ�����ڻ�û����Ҫ·�������
		}
	}
}

void main_frame::create_working_space()
{
	// ���ͿڵĿͻ����Ӵ��ڣ�����ʹ���ض��Ĵ������ID�����ܱ�mfc�ӹܣ�����Ҫ�Լ��������ĵĴ���λ�úʹ�С������Ҫ�Լ�
	// ��Ӧsize��Ϣ����ͬ������ͽ��沼�ֻᵼ�����λ�ò�һ��������ֱ�ӻ�ȡ�����ڵĿͻ����ǰ���ͣ�����ڡ����������˵�
	// �����ڵ�
	m_workspace_view.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(), this, AFX_IDW_PANE_FIRST);

	return;
}

void main_frame::initialize_theme_style_settings()
{
	m_all_style_settings.clear();
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLEBLUE2007;
		style.m_style_type = app_theme_type::theme_office_2007_blue;
		style.m_tooltip_style = xtpToolTipOffice2007;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonAutomatic;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2007BLUE.ini");
		style.m_enable_aero = false;
		style.m_skin_file_name = _T("Office2007.cjstyles");
		// style.m_skin_file_ini_section_name = _T("NormalBlue.ini");
		style.m_skin_file_ini_section_name = _T("LightBlue.ini");
		
		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(240, 248, 255);
		
		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLEBLACK2007;
		style.m_style_type = app_theme_type::theme_office_2007_black;
		style.m_tooltip_style = xtpToolTipOffice2007;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonAutomatic;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2007BLACK.ini");
		style.m_enable_aero = false;
		style.m_skin_file_name = _T("Office2007.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalBlack.ini");

		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(230, 230, 230);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLESILVER2007;
		style.m_style_type = app_theme_type::theme_office_2007_silver;
		style.m_tooltip_style = xtpToolTipOffice2007;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonAutomatic;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2007SILVER.ini");
		style.m_enable_aero = false;
		style.m_skin_file_name = _T("Office2007.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalSilver.ini");

		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(250, 250, 250);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLESILVER2010;
		style.m_style_type = app_theme_type::theme_office_2010_silver;
		style.m_tooltip_style = xtpToolTipOffice2007; // xtpToolTipOffice;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonCaption;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2010SILVER.ini");
		style.m_enable_aero = true;
		style.m_skin_file_name = _T("Office2010.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalSilver.ini");

		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(250, 250, 250);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLEBLUE2010;
		style.m_style_type = app_theme_type::theme_office_2010_blue;
		style.m_tooltip_style = xtpToolTipOffice2007; // xtpToolTipOffice;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonCaption;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2010BLUE.ini");
		style.m_enable_aero = true;
		style.m_skin_file_name = _T("Office2010.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalBlue.ini");

		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(240, 248, 255);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLEBLACK2010;
		style.m_style_type = app_theme_type::theme_office_2010_black;
		style.m_tooltip_style = xtpToolTipOffice2007; // xtpToolTipOffice;
		style.m_docking_pane_theme = xtpPaneThemeResource;
		style.m_tab_appearance = xtpTabAppearanceVisualStudio2010;
		style.m_paint_theme = xtpThemeRibbon;
		style.m_ribbon_system_button_style = xtpButtonCaption;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2010BLACK.ini");
		style.m_enable_aero = true;
		style.m_skin_file_name = _T("Office2010.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalBlack.ini");

		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorResource;
		style.m_window_back_color = RGB(230, 230, 230);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLE_LIGHT2013;
		style.m_style_type = app_theme_type::theme_office_2013_white;
		style.m_tooltip_style = xtpToolTipOffice2013;
		style.m_docking_pane_theme = xtpPaneThemeOffice2013;
		style.m_tab_appearance = xtpTabAppearanceOffice2013;
		style.m_paint_theme = xtpThemeOffice2013;
		style.m_ribbon_system_button_style = xtpButtonCaption;
		style.m_theme_dll_ini_config_file_name = _T("Office2013Word.ini"); // _T("Office2013WordGrayLight.ini");
		style.m_enable_aero = false;
		style.m_skin_file_name = _T("Office2013.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalWhite.ini");
		
		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorOffice2013;
		style.m_window_back_color = RGB(250, 250, 250);

		m_all_style_settings[style.m_style_id] = style;
	}
	{
		theme_style_settings style;
		style.m_style_id = ID_THEME_STYLE_DARK2013;
		style.m_style_type = app_theme_type::theme_office_2013_dark;
		style.m_tooltip_style = xtpToolTipOffice2013;
		style.m_docking_pane_theme = xtpPaneThemeOffice2013;
		style.m_tab_appearance = xtpTabAppearanceOffice2013;
		style.m_paint_theme = xtpThemeOffice2013;
		style.m_ribbon_system_button_style = xtpButtonCaption;
		style.m_theme_dll_ini_config_file_name = _T("OFFICE2013WORDGRAYDARK.ini");
		style.m_enable_aero = false;
		style.m_skin_file_name = _T("Office2013.cjstyles");
		style.m_skin_file_ini_section_name = _T("NormalDark.ini");
		
		// ���ʹ��Ƥ����һ��ʹ��xtpTabColorDefault��Ƥ�����أ�����ָ��������ʽ
		style.m_tab_color = xtpTabColorDefault; // xtpTabColorOffice2013;
		style.m_window_back_color = RGB(240, 240, 240);

		m_all_style_settings[style.m_style_id] = style;
	}
}

theme_style_settings main_frame::get_style_and_settings() const
{
	auto it = m_all_style_settings.find(m_current_theme);
	if (it == m_all_style_settings.end())
		return app_ui_interface::get_style_and_settings();
	else
		return it->second;
}

void main_frame::on_project_data_notify(const CString& node_tag, notifer_action ac)
{
}

bool main_frame::is_style_changing() const
{
	// ���֧�ֶ��̣߳�����OSG��Ⱦ�̺߳�������ͬʱ���ʻ�������
	return m_is_style_changing.load();
}

void show_or_hide_window_tree(CWnd* top_wnd, CWnd* bottom_wnd, bool show /*= false*/)
{
	if (!bottom_wnd) return;
	if (!top_wnd) return;
	if (bottom_wnd->GetSafeHwnd() == GetDesktopWindow()) return;
	if (top_wnd == bottom_wnd) return;

	if (!bottom_wnd->GetSafeHwnd()) return;
	if (show)
		bottom_wnd->ShowWindow(SW_SHOW);
	else
		bottom_wnd->ShowWindow(SW_HIDE);

	show_or_hide_window_tree(top_wnd, bottom_wnd->GetParent(), show);
}

class select_flage_setting_raii_helper
{
public:
	select_flage_setting_raii_helper(std::atomic<bool>& select_flag)
		: m_select_flag(select_flag)
	{
		m_select_flag = true;
	}

	~select_flage_setting_raii_helper()
	{
		m_select_flag = false;
	}

private:
	std::atomic<bool>& m_select_flag;
};

void main_frame::on_command_skin_and_theme_menu_item(UINT id)
{
	if (!app_configs::is_skin_enable()) return;

	m_is_style_changing.store(true);
	m_current_theme = id;

	set_grid_color();
	set_skin_style();
	set_frame_theme();
	redraw_frame();

	// �������ɺ�֪ͨ���е�wwindow_interface�����Ѿ��ı�
	theme_style_settings setting = get_style_and_settings();
	set<window_interface*> all_wnds = window_interface::get_all_window_instances();
	for (window_interface* x : all_wnds)
		if (x) x->app_theme_changed(setting);

	// ���浱ǰ�����Ա��´�����ʱԭ����ʾ
	main_application* app = (main_application*) AfxGetApp();
	if (app) app->WriteProfileInt(_T("Skin"), _T("ThemeType"), m_current_theme);

	m_is_style_changing.store(false);
}

void main_frame::set_frame_theme()
{
	CXTPCommandBars* command_bars = GetCommandBars();
	if (!command_bars) return;

	app_ribbon_bar* ribbon_bar = DYNAMIC_DOWNCAST(app_ribbon_bar, command_bars->GetMenuBar());
	if (!ribbon_bar) return;

	theme_style_settings setting = get_style_and_settings();
	ribbon_bar->GetSystemButton()->SetStyle(setting.m_ribbon_system_button_style);

	if (app_configs::is_skin_enable())
	{
		CXTPToolTipContext* tool_tip_context = command_bars->GetToolTipContext();
		if (tool_tip_context) tool_tip_context->SetStyle(get_tooltip_style());

		// ��aeroЧ��Ҫ�Ƚ��ÿ�����⣬�����ٸ�����Ҫ�ٵ�һ����������ã�ԭ����xtp��acroЧ����Ǳ����Ǿ�̬��������ֻ
		// �������������ʱ��Ż����ڳ�ʼ���ڲ�״̬������demo�������÷������˴�Ϊ���Ч�ʣ������л�ʱ�Ĵ����ػ������
		// ��˸��Υ�͸У��Ż�Ϊֻ��aero״̬�仯ʱ�ŵ���
		BOOL old_bAllowDwm = m_bAllowDwm;
		m_bAllowDwm = (setting.m_enable_aero ? TRUE : FALSE);
		if (old_bAllowDwm != m_bAllowDwm) ribbon_bar->EnableFrameTheme(FALSE);

		XTPSetOfficeSetBackImage(GetCommandBars(), XTPSpring); // ֻ��office 2013�������Ч
		ribbon_bar->EnableFrameTheme(TRUE);
		RefreshFrameStyle();
	}
	else
	{
		CXTPToolTipContext* tool_tip_context = command_bars->GetToolTipContext();
		if (tool_tip_context) tool_tip_context->SetStyle(xtpToolTipStandard);

		ribbon_bar->EnableFrameTheme(FALSE);
		XTPSetOfficeSetBackImage(GetCommandBars(), XTPNone); // ֻ��office 2013�������Ч
		ribbon_bar->EnableFrameTheme(FALSE);
		RefreshFrameStyle();
	}

	// �����ڴ��������ͳһ���ã��˴���Ҫ������
	//command_bars->GetPaintManager()->RefreshMetrics();
	//command_bars->RedrawCommandBars();

	//SendMessage(WM_NCPAINT);
	//RedrawWindow(0, 0, RDW_ALLCHILDREN | RDW_INVALIDATE);
}

void main_frame::set_skin_style()
{
	CXTPSkinManager* skin_manager = XTPSkinManager();
	if (!skin_manager) return;

	if (!app_configs::is_skin_enable())
	{
		skin_manager->LoadSkin(nullptr, nullptr); // ��ʹ��Ƥ��
		CXTPPaintManager::SetTheme(xtpThemeNone);
		return;
	}

	theme_style_settings setting = get_style_and_settings();

	// ��Ҫ�ֹ�����xtp���Ѿ��ں�̨���Զ��������Դ�����Դdll�ˣ������õĲ����Դ���
	//////////////////////////////////////////////////////////////////////////
	// HMODULE handle = AfxGetResourceHandle(); // �����Դ�����ڣ������mfc��
	// HMODULE handle = ::LoadLibrary(file_tools::get_app_path_with_xg() + _T("styles\\Office2007.dll"));
	// HMODULE handle = ::LoadLibrary(file_tools::get_app_path_with_xg() + _T("styles\\Office2010.dll"));
	// if (handle) ((CXTPResourceTheme*) GetCommandBars()->GetPaintManager())->SetImageHandle(handle, setting.m_paint_theme_ini_section_name);
	//////////////////////////////////////////////////////////////////////////

	// xtp����bug���˵�Ҳʹ��Ƥ����ᵼ�´򿪶Ի������Ҽ��˵���ʾ�쳣����ʱû��ʱ�����������⣬��ע��
	skin_manager->SetApplyOptions(xtpSkinApplyMetrics | xtpSkinApplyFrame | xtpSkinApplyColors/* | xtpSkinApplyMenus*/);
	if (setting.m_skin_file_name.IsEmpty())
		skin_manager->LoadSkin(nullptr, nullptr); // ��ʹ��Ƥ��
	else
		skin_manager->LoadSkin(file_tools::get_app_path_with_xg() + _T("styles\\") + setting.m_skin_file_name, setting.m_skin_file_ini_section_name);
	
	XTPThemeDLL()->SetHandle(setting.m_theme_dll_ini_config_file_name);
	if (app_configs::is_skin_enable())
		CXTPPaintManager::SetTheme(setting.m_paint_theme);
	else
		CXTPPaintManager::SetTheme(xtpThemeNone);
}

void main_frame::set_grid_color()
{
	theme_style_settings setting = get_style_and_settings();

	if (app_theme_type::theme_office_2007_blue == setting.m_style_type
		|| app_theme_type::theme_office_2010_blue == setting.m_style_type)
	{
		CGridGlobalData::SetFilePathName(file_tools::get_app_path_with_xg() + _T("styles\\blue_grid_settings.ini"), true);
		CGridGlobalData::Refresh();
	}
	else if (app_theme_type::theme_office_2007_black == setting.m_style_type
		|| app_theme_type::theme_office_2010_black == setting.m_style_type)
	{
		CGridGlobalData::SetFilePathName(file_tools::get_app_path_with_xg() + _T("styles\\black_grid_settings.ini"), true);
		CGridGlobalData::Refresh();
	}
	else if (app_theme_type::theme_office_2007_silver == setting.m_style_type
		|| app_theme_type::theme_office_2010_silver == setting.m_style_type
		|| app_theme_type::theme_office_2013_white == setting.m_style_type)
	{
		CGridGlobalData::SetFilePathName(file_tools::get_app_path_with_xg() + _T("styles\\white_grid_settings.ini"), true);
		CGridGlobalData::Refresh();
	}
	else if (app_theme_type::theme_office_2013_dark == setting.m_style_type)
	{
		CGridGlobalData::SetFilePathName(file_tools::get_app_path_with_xg() + _T("styles\\dark_grid_settings.ini"), true);
		CGridGlobalData::Refresh();
	}
	else
	{
		// nothing;
	}
}

int main_frame::add_working_space_view(const CString window_class, const CString node_tag, int index, int image_index)
{
	// ������Ŀ�����ʱ���Ѵ򿪵Ĺ���������Ҳ���������������ڹ����������������������仯ʱ�����´򿪾���Ŀ��
	// ���ڱ�ʵ���������Ķ������𻵵ģ���������ִ���Ҳ�ָ���ʾ�����ᵼ�½����������ˣ��������ֽ��������Ѿ�����
	// �����Ŀ���ṩ��һ�ַ�ʽ����������ڴ�ʱ��ס��Ctrl������������˴��ڴ򿪲���ӵ���������Ϊ�˷�ֹ���û���
	// ��������˰�סCtrlʱ������ʾ�Ա�ѡ��
	// ����Ctrl�򿪹������ᱻ��Ϊ���޸�ģʽ
	if (0xff == HIBYTE(GetKeyState(VK_CONTROL))) // ��λȫ1��ʾ���£�����û��
	{
		if (IDYES == AfxMessageBox(_T("�޸�ģʽ�½������򿪹��������ڲ�����ȷ�����޸�ģʽ������"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING))
			return -1;
	}

	// ��ӹ��������ڿ��ܻ������Ҫ���ж��Ƿ�Ҫ�ص�һЩ���������ڣ��糬������󴰿���������ʱ�ص��Ĵ��ڻ������Ӧ��
	// ���湦�ܣ����ڱ���ʱ���ܴ����������ĵ�������Ӧ����ӵ������ؼ��Ľڵ����ټ��ؼ��أ�����ʱ��ǰ�Ѿ��͹�������Ϣ
	// �����Ѿ�ʧЧ�����ˣ���˲������ʹ�����������Ҳ��Ч�ˣ����Ժ���ԭ�͵Ĳ������Ͳ���ʹ������

	if (!m_workspace_view.GetSafeHwnd()) return -1;

	//modify cg 2015-10-16
	int flag = m_workspace_view.add_tab_window_and_info(window_class, node_tag, index, image_index);
	if (flag != -1)
	{
		// ��ܲ�Ҫ���潹������飬������Щ�Ӵ��ڵĽ��㽫����ܸ��Ŷ�������
		//db_command_line* pComdLine = dynamic_cast<db_command_line*>(GetCommandLineInterface());
		//if (pComdLine) pComdLine->SetFocus();
	}

	return flag;
}

int main_frame::add_working_space_view(window_interface* wnd, const CString node_tag, int index /*= -1*/, int image_index /*= -1*/)
{
	if (!wnd) return -1;

	return m_workspace_view.add_tab_working_space_view(wnd, node_tag, index, image_index);
}

window_interface* main_frame::get_docking_pane_window_by_id(int id)
{
	for (auto& x : m_docking_pane_config.m_container)
	{
		for (single_pane_data& y : x.m_panes)
		{
			if (id == y.m_child_window_id)
			{
				CXTPDockingPane* pane = m_pane_manager.FindPane(y.m_id);
				if (!pane) continue;

				CWnd* child = pane->GetChild();
				if (!child) continue;

				return dynamic_cast<window_interface*>(child);
			}
		}
	}

	return nullptr;
}

int main_frame::get_docking_pane_window_id(window_interface* wnd)
{
	CWnd* wnd_object = dynamic_cast<CWnd*>(wnd);
	if (!wnd_object || !wnd_object->GetSafeHwnd()) return 0;

	// �ǶԻ���Ҳ�����������Ա�õ�idֵ
	return wnd_object->GetDlgCtrlID();
}

void main_frame::redraw_frame()
{
	if (!GetSafeHwnd()) return;

	CXTPCommandBars* bars = GetCommandBars();
	if (!bars) return;

	bars->GetPaintManager()->RefreshMetrics();

	// ����DPI����Ӧģ�ͣ��ڸ߷���ģʽ��ͼ���Զ�����
	bars->GetPaintManager()->m_bAutoResizeIcons = TRUE;
	bars->GetCommandBarsOptions()->SetDPIScallingOptions(TRUE);

	bars->GetImageManager()->RefreshAll();
	bars->RedrawCommandBars();
	SendMessage(WM_NCPAINT);

	// ״̬�������ػ�
	//if (m_status_bar.GetSafeHwnd()) m_status_bar.Invalidate();

	// ͣ�������ػ�
	if (m_pane_manager.GetSafeHwnd())
	{
		app_ui_interface* app = get_app_ui_interface();
		if (!app) return;

		theme_style_settings theme = app->get_style_and_settings();

		m_pane_manager.SetTheme(theme.m_docking_pane_theme);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(theme.m_tab_appearance);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetLayout(xtpTabLayoutAutoSize);
		m_pane_manager.GetPaintManager()->GetPanelPaintManager()->SetColor(theme.m_tab_color);

		m_pane_manager.GetPaintManager()->RefreshMetrics();
		m_pane_manager.UpdateWindow(); // ͣ��������ػ�
		m_pane_manager.RedrawPanes(); // ���б������ͣ����崰���ػ�
	}

	RedrawWindow(0, 0, RDW_ALLCHILDREN | RDW_INVALIDATE);
}

app_ribbon_bar* main_frame::get_db_ribbon_bar()
{
	CXTPCommandBars* pBars = GetCommandBars();
	if (!pBars) return NULL;

	for (int i = 0; i < pBars->GetCount(); ++i)
	{
		CXTPCommandBar* tmp = pBars->GetAt(i);
		if (0 == tmp->GetTitle().CompareNoCase("app_ribbon_bar")) // �����ɴ���ʱָ��
			return dynamic_cast<app_ribbon_bar*>(tmp);
	}

	return nullptr;
}

void main_frame::on_update_skin_and_theme_menu_item(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_current_theme == (int) pCmdUI->m_nID);
}

LRESULT main_frame::on_docking_pane_notify(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void main_frame::on_recent_file_list(UINT id)
{
	int index = (int)(id - ID_FILE_MRU_FILE1);
	CString full_path = the_main_app.get_recent_list_item_full_path(index);
	if (full_path.IsEmpty()) return;

	cmd_trigger_manager::fire(_T("app"), _T("open"), full_path);
}

void main_frame::on_all_ribbon_tab_command(UINT id)
{
	// Ribbon�п���xml�ĵ��������Լ������Զ�����Ϣ��Ӧ�Ĺ����ڴ˺�������ɣ��ļ��˵��Ϳ��ٷ�������
	// �Ĺ�����ʹ��MFC����Ϣ��Ӧ�����
	app_ribbon_bar* rib = get_db_ribbon_bar();
	if (!rib) return;
	if (!rib->GetSafeHwnd()) return;

	array<CString, 3> cmd_info;
	if (m_ribbon_config.get_command_and_parameter(id, cmd_info))
		cmd_trigger_manager::fire(cmd_info[0], cmd_info[1], cmd_info[2]);
}

// ����һ��CWnd�õ�����window_interfaceָ�룬������ڲ���window_interface�����࣬���������������������һ��������Ҳ�������nullptr
window_interface* get_window_interface_by_CWnd(CWnd* wnd)
{
	if (!wnd) return nullptr;

	window_interface* pInterface = dynamic_cast<window_interface*>(wnd);
	HWND desk = GetDesktopWindow();
	while (!pInterface)
	{
		wnd = wnd->GetParent();
		if (wnd->GetSafeHwnd() == desk) break;;

		pInterface = dynamic_cast<window_interface*>(wnd);
	}

	return pInterface;
}

void main_frame::on_update_all_ribbon_tab_command(CCmdUI *pCmdUI)
{
	if (!m_workspace_view.GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	app_ribbon_bar* pRibbonBar = get_db_ribbon_bar();
	if (!pRibbonBar || !pRibbonBar->GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	if (IDS_RIBBON_OPTIONS_MINI_MODE == pCmdUI->m_nID)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(0);
		return;
	}

	// �ȵõ���Ӧ��������Ϣ
	std::array<CString, 3> cmd_info;
	if (!m_ribbon_config.get_command_and_parameter(pCmdUI->m_nID, cmd_info)) return;

	CString domain = cmd_info[0];
	CString cmd = cmd_info[1];

	// û����xml���õ�ribbon�������Լ�ר�ŵ���Ӧ��������������������ÿؼ��Ĵ������̣��������
	if (cmd.IsEmpty())
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(0);
		return;
	}

	// �����ǰ������޷������δ������ת�����������д���
	bool is_enable = false, is_checked = false, is_transparent = false;
	cmd_trigger_manager::get_command_state(domain, cmd, is_enable, is_checked, is_transparent);
	pCmdUI->Enable(is_enable ? TRUE : FALSE);

	// EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
	if (is_enable)
		pCmdUI->SetCheck(is_checked ? 1 : 0);
	else
		pCmdUI->SetCheck(0);
}

void main_frame::on_all_accelerator_command(UINT id)
{
	CString cmd = m_accelerator_map.get_accelerator_command_by_id(id);
	cmd.Trim();
	if (cmd.IsEmpty()) return;

	CString cmd_domain, cmd_text;
	cmd_container_interface::parse_and_fill_command(cmd, cmd_domain, cmd_text);
	if (cmd_text.IsEmpty()) return;

	cmd_trigger_manager::fire(cmd_domain, cmd_text, _T("")); // ���ټ�������������û��·����
}

void main_frame::on_update_all_accelerator_command(CCmdUI *pCmdUI)
{
	// Ҫ���������Ƿ���EnableΪtrue�ŵ��������򲻵���
	// ...
	pCmdUI->Enable(TRUE);
}

LRESULT main_frame::on_msg_do_functor(WPARAM wparam, LPARAM lparam)
{
	functor_by_message* fun = (functor_by_message*) wparam;
	if (!fun) return FALSE;

	fun->call();
	safe_delete(fun);

	return TRUE;
}

void main_frame::on_all_ribbon_quick_access_command(UINT id)
{
	// Ribbon�п���xml�ĵ��������Լ������Զ�����Ϣ��Ӧ�Ĺ����ڴ˺�������ɣ��ļ��˵��Ϳ��ٷ�������
	// �Ĺ�����ʹ��MFC����Ϣ��Ӧ�����

	ribbon_quick_access_bar& quick_access_bar = m_ribbon_config.m_quick_access;
	array<CString, 3> cmd_info;
	if (quick_access_bar.get_command_and_parameter(id, cmd_info))
		cmd_trigger_manager::fire(cmd_info[0], cmd_info[1], cmd_info[2]);
}

void main_frame::on_update_all_ribbon_quick_access_command(CCmdUI *pCmdUI)
{
	if (!m_workspace_view.GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	app_ribbon_bar* pRibbonBar = get_db_ribbon_bar();
	if (!pRibbonBar || !pRibbonBar->GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	// �ȵõ���Ӧ��������Ϣ
	std::array<CString, 3> cmd_info;
	ribbon_quick_access_bar quick_access_bar = m_ribbon_config.m_quick_access;
	if (!quick_access_bar.get_command_and_parameter(pCmdUI->m_nID, cmd_info)) return;

	CString domain = cmd_info[0];
	CString cmd = cmd_info[1];

	// û����xml���õ�ribbon�������Լ�ר�ŵ���Ӧ��������������������ÿؼ��Ĵ������̣��������
	if (cmd.IsEmpty())
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(0);
		return;
	}

	// �����ǰ������޷������δ������ת�����������д���
	bool is_enable = false, is_checked = false, is_transparent = false;
	cmd_trigger_manager::get_command_state(domain, cmd, is_enable, is_checked, is_transparent);
	pCmdUI->Enable(is_enable ? TRUE : FALSE);

	// EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
	if (is_enable)
		pCmdUI->SetCheck(is_checked ? 1 : 0);
	else
		pCmdUI->SetCheck(0);
}

void main_frame::on_all_ribbon_file_menu_command(UINT id)
{
	ribbon_file_menu_bar& file_menu_bar = m_ribbon_config.m_file_menu;
	array<CString, 3> cmd_info;
	if (file_menu_bar.get_command_and_parameter(id, cmd_info))
		cmd_trigger_manager::fire(cmd_info[0], cmd_info[1], cmd_info[2]);
}

void main_frame::on_update_all_ribbon_file_menu_command(CCmdUI *pCmdUI)
{
	if (!m_workspace_view.GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	app_ribbon_bar* pRibbonBar = get_db_ribbon_bar();
	if (!pRibbonBar || !pRibbonBar->GetSafeHwnd())
	{
		pCmdUI->Enable(FALSE);
		// pCmdUI->SetCheck(0); // EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
		return;
	}

	// �ȵõ���Ӧ��������Ϣ
	std::array<CString, 3> cmd_info;
	ribbon_file_menu_bar file_menu_bar = m_ribbon_config.m_file_menu;
	if (!file_menu_bar.get_command_and_parameter(pCmdUI->m_nID, cmd_info)) return;

	CString domain = cmd_info[0];
	CString cmd = cmd_info[1];

	// û����xml���õ�ribbon�������Լ�ר�ŵ���Ӧ��������������������ÿؼ��Ĵ������̣��������
	if (cmd.IsEmpty())
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(0);
		return;
	}

	// �����ǰ������޷������δ������ת�����������д���
	bool is_enable = false, is_checked = false, is_transparent = false;
	cmd_trigger_manager::get_command_state(domain, cmd, is_enable, is_checked, is_transparent);
	pCmdUI->Enable(is_enable ? TRUE : FALSE);

	// EnableΪfalseʱ�������bug����ǿ�Ƹ�ΪUnCheck���������ѡ�������˸
	if (is_enable)
		pCmdUI->SetCheck(is_checked ? 1 : 0);
	else
		pCmdUI->SetCheck(0);
}

void main_frame::on_all_ribbon_options_command(UINT id)
{
	// Ribbon�п���xml�ĵ��������Լ������Զ�����Ϣ��Ӧ�Ĺ����ڴ˺�������ɣ��ļ��˵��Ϳ��ٷ�������
	// �Ĺ�����ʹ��MFC����Ϣ��Ӧ�����

	app_ribbon_bar* pRibbonBar = get_db_ribbon_bar();
	if (!pRibbonBar) return;
	if (!pRibbonBar->GetSafeHwnd()) return;

	if (id == IDS_RIBBON_OPTIONS_MINI_MODE)
	{
		m_is_ribbon_mini_size = !(pRibbonBar->IsRibbonMinimized()); // ����Ϊ�෴��״̬
		pRibbonBar->SetRibbonMinimized(m_is_ribbon_mini_size);
	}
	else if(id == IDS_RIBBON_OPTIONS_ABOUT)
	{
		CObject* dialog_obj = CRuntimeClass::CreateObject(m_ribbon_config.m_dialog_info.m_about_dialog_class_name);
		CDialog* about_dialog = dynamic_cast<CDialog*>(dialog_obj);
		if (about_dialog) about_dialog->DoModal();
		safe_delete(dialog_obj);
	}
	else
	{
		/*nothing*/
	}
}

void main_frame::on_update_all_ribbon_options_command(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void main_frame::on_update_ribbon_exit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void main_frame::on_update_ribbon_recentdocs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

BOOL main_frame::OnHelpInfo(HELPINFO* pHelpInfo)
{
	pair<CString, CString> info; // firstΪ�����࣬secondΪ����ڵ��·��
	get_current_working_space_active_window(info);

	CString page_full_path = help_configs::get_default_page_full_path();

	if (!info.first.IsEmpty())
	{
		page_full_path = help_configs::get_page_full_path_by_window_class_name(info.first);
	}

	// ���ַ�����Ҫ���ã�����ʾΪ�հ״���
	if (!page_full_path.IsEmpty())
		::HtmlHelp(GetSafeHwnd(), page_full_path, HH_DISPLAY_TOPIC, 0);
	else
		::HtmlHelp(GetSafeHwnd(), help_configs::get_default_page_full_path(), HH_DISPLAY_TOPIC, 0);

	return CXTPFrameWnd::OnHelpInfo(pHelpInfo);
}
