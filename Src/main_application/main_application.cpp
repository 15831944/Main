#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include <memory>

#include "x_framework/macro.h"
#include "x_framework/interface/console_interface.h"
#include "x_framework/tools/file_tools.h"
#include "x_framework/tools/auto_load_module.h"
#include "x_framework/tools/table_templates.h"
#include "x_framework/interface/cmd_container_interface.h"

#include "splash_window.h"
#include "mini_dump.h"
#include "main_frame.h"
#include "help_configs.h"
#include "app_size_config_helper.h"
#include "app_configs.h"

#include "main_application.h"


using namespace Gdiplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////

// ����ȫ�ֱ���ʵ��ת�����ܵ����ȼ��أ�����the_main_appǰ����ʹ�þ�̬������ԭ���ǽ����������������ڵ�ǰ�ļ���
static bool the_auto_enable_dumper = (mini_dump_helper::enable_dump(true), true);

// Ψһ��һ�� main_application ����
main_application the_main_app;

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class about_dialog : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

public:
	about_dialog();

public:
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
};


BEGIN_MESSAGE_MAP(about_dialog, CDialogEx)
END_MESSAGE_MAP()

about_dialog::about_dialog() : CDialogEx(about_dialog::IDD)
{
}

void about_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(main_application, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, on_app_about)
END_MESSAGE_MAP()


main_application::main_application()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = NULL; // AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	// Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tonghao.main_application.v2"));

	m_gdiplusToken = NULL;
}

main_application::~main_application()
{
}

bool register_window_class_by_name(const CString& class_name)
{
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));

	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = nullptr;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = class_name;

	if (!AfxRegisterClass(&wndcls)) return false;

	return true;
}

BOOL main_application::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ�
	// �ӻ���ʽ������Ҫ InitCommonControlsEx()��  �����޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES; // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õĹ����ؼ��ࡣ
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);
	InitTooltipManager();

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()
	AfxInitRichEdit2();

	// ������ǰ����һ��������������ã���ȷ��mfc��ȷ��ʼ��
	app_configs::load_settings_from_xml();
	
	// �������ڴ洢���õ�ע�����,���δʹ����Щ���ܲ�ϣ����С,���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�
	SetRegistryKey(app_configs::m_application_name);
	// LoadStdProfileSettings(4);  // Load standard INI file options (including MRU) // ���ɿ�ܽӹܴ���������Ҫ

	// ����CWinAppEx::m_pszAppName��ֵ�Ϳ��Ը���AfxMessageBox()�ı�����
	set_application_name(app_configs::m_application_name);

	// LoadAccelerators(); // ���ɿ�ܽӹܴ���������Ҫ

	if (app_configs::m_only_one_instance)
	{
		m_single_instance_mutex = ::CreateMutex(NULL, FALSE, app_configs::m_application_name);
		if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			AfxMessageBox(_T("�����Ѿ�����!"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}

	// ���ڴ��������������EnableTaskbarInteraction֮��֮����Ҫע�ᴰ������Ϊ���洰��Ҫ�����Ļ�����Ҫ��WS_CHILD���
	// ������Ҫ��ָ�������ڣ�ָ��Ϊ��������쳣���������Ϊģ̬�Ի��������������ʾ���������������ڣ����Ҫע��һ���µĴ�����
	// ��ר�Ŵ���splash
	CWaitCursor tmp_wait_cursor;
	std::unique_ptr<splash_window> wnd(nullptr);
	if (app_configs::m_enable_splash && register_window_class_by_name(_T("app_splash")))
	{
		wnd.reset(new splash_window()); // ָ������ʱ���Զ����ٴ���
		wnd->Create();
	}

	// ������ʷ�б�
	load_recent_list_to_mfc();

	// ��������������Ϣ
	help_configs::initialize_by_xml();

	// ������ǰ��׼��������ɺ����
	// �����true��ʾ��ֹ���򴴽����ᰴ�������˳����̴�����Ҫǿ��ʹ��exit�˳������򲻻���ó���ʵ����
	// ���Ĵ��룬����һЩ������Դ���ܱ���ȷ�ͷţ���com����Դ������
	if (true == cmd_trigger_manager::fire(_T("sys"), _T("pre_create_main_window"), _T(""))) return FALSE;

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ��ڶ���Ȼ��������ΪӦ�ó���������ڶ���
	main_frame* frame = new main_frame();
	if (!frame) return FALSE;

	frame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr, nullptr);
	frame->ShowWindow(SW_HIDE);
	m_pMainWnd = frame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CString param = cmdInfo.m_strFileName;
	param.Trim();
	// ���ļ�Ҫ�Զ���ʱ�ȴ���ͣ�����ڵĴ�������
	if (!param.IsEmpty()) frame->create_docking_panes();
	cmd_trigger_manager::fire(_T("sys"), _T("post_create_main_window_pre_open_file"), _T("")); // �ŵ�ͣ����崴���󴥷�

	// ���������"|"��ͷ�����ʾ�Զ���Ĳ����������ղ���ϵͳ�ĳ�����Ϊ����
	if (_T("|") == param.Left(1))
	{
		CString custom_param = param.Mid(1); // �ѿ�ͷ��һ��"|"ȥ���������������Ϊ���û����ݣ�
		cmd_trigger_manager::fire(_T("sys"), _T("custom_command_when_start"), custom_param);
	}
	else if (!param.IsEmpty())
	{
		// �����ļ��򿪲�����ͨ�������ж���������������ĵ��ÿ��Լ��û��ӽ����Ͽ������еĲ����������¼����־�����Ҳ��ᴥ����Ӧ��notify
		cmd_trigger_manager::fire(_T("app"), _T("open"), param); // Ҫ��Ҫ����ʼҳ��app.open�����ڲ���������
	}
	else
	{
		if (app_configs::m_show_start_page)
			cmd_trigger_manager::fire(_T("sys"), _T("open_start_page"));
	}

	// splash���ڻ��Զ�������������ʱ����Ҫ�����ʱ�䣬��ʱ���������٣��ֲ㴰�ڵ�͸������ʧЧ��Ϊ�ڵģ�����ڴ�������
	// ǰ�ȰѴ������ؾͿ��������ֲ���г��������
	if (app_configs::m_enable_splash && wnd) wnd->ShowWindow(SW_HIDE);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	// if (!ProcessShellCommand(cmdInfo)) return FALSE; // ���е��ĵ�����mfc�Ѿ�û�������ˣ���Ϊ�Ų�û����mfc���ĵ���ͼģ��

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	frame->MoveWindow(app_size_config_helper::get_last_main_frame_pos_and_size());
	frame->redraw_frame(); // �������ں��棬�Լ�ĳЩ���ڴ���������������Ʋ�ȫ��ע�ⲻҪ������ʾǰ�������λ��ƿ��ܻ�����
	frame->ShowWindow(app_size_config_helper::get_maximize_flag() ? SW_MAXIMIZE : SW_NORMAL);
	frame->UpdateWindow();

	cmd_trigger_manager::fire(_T("sys"), _T("post_create_main_window_post_open_file"), _T(""));

	return TRUE;
}

int main_application::ExitInstance()
{
	if (m_single_instance_mutex)
		CloseHandle(m_single_instance_mutex);

	AfxOleTerm(FALSE);
	GdiplusShutdown(m_gdiplusToken);
	return CWinAppEx::ExitInstance();
}

CString main_application::get_recent_list_item_full_path(int index)
{
	if (!m_pRecentFileList) return _T("");
	if (index < 0 || index > m_pRecentFileList->GetSize() - 1) return _T("");

	return (*m_pRecentFileList)[index];
}

int main_application::get_recent_list_item_count()
{
	if (m_pRecentFileList)
		return m_pRecentFileList->GetSize();
	else
		return 0;
}

void main_application::set_application_name(const CString& name)
{
	// ����CWinAppEx::m_pszAppName��ֵ�Ϳ��Ը���AfxMessageBox()�ı�����
	// First free the string allocated by MFC at CWinAppEx startup.
	// The string is allocated before InitInstance is called.
	free((void*) m_pszAppName); // �ַ���ʹ��allocated����ģ�����ʹ��free�ͷţ�����delete
	// Change the name of the application file.
	// The CWinAppEx destructor will free the memory.
	m_pszAppName = _tcsdup(name);
}

void main_application::load_recent_list_to_mfc()
{
	if (m_pRecentFileList) safe_delete(m_pRecentFileList);

	CXTPRecentFileList* recent_list = new CXTPRecentFileList(0, _T("Recent File List"), _T("File%d"), 9);
	recent_list->ReadList();

	// ��·��ʧЧ����ʷ�б���Ŀɾ��
	for (int i = 0; i < recent_list->GetCount(); /* nothing */)
	{
		CXTPRecentFileListItem* item = recent_list->GetItem(i);
		if (item)
		{
			CString full_path = item->GetPathName();
			if (full_path.IsEmpty() || !PathFileExists(full_path))
			{
				recent_list->Remove(i);
				continue;
			}
		}

		++i;
	}
	m_pRecentFileList = recent_list;
}

void main_application::on_app_about()
{
	about_dialog aboutDlg;
	aboutDlg.DoModal();
}
