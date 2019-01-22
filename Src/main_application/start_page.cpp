#include "stdafx.h"

#include "x_framework/macro.h"
#include "x_framework/interface/window_interface.h"
#include "x_framework/interface/cmd_container_interface.h"
#include "x_framework/interface/app_ui_interface.h"
#include "x_framework/tools/file_tools.h"
#include "x_framework/tools/string_tools.h"

#include "app_configs.h"

#include "start_page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


IMPLEMENT_SERIAL(start_page, CWnd, 1)
BEGIN_MESSAGE_MAP(start_page, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

start_page::start_page() : CWnd(), CXTPMarkupContext(), window_interface()
{
	m_markup_page = nullptr;
	m_is_page_in_rendering = false;
	
	// �����¼���Ӧ�ķ�ʽ����Ҫ��xaml��Ҫ����Click�¼��������xaml�ж������¼��������˴�û�й�����Ӧ������xtp��
	// ����Ϊxaml��ʽ������ʾ�հף������������������㣬��˸���AddHandler�ķ�ʽ
	// SetDelegate(L"new_project_clicked", CreateMarkupClassDelegate(this, &start_page::on_hyperlink_clicked));
	AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &start_page::on_hyperlink_clicked));
}

start_page::~start_page(void)
{
	if (m_markup_page)
	{
		// Release��delete(this)��û����Ϊnullptr
		m_markup_page->Release();
		m_markup_page = nullptr;
	}
}

CString start_page::get_title()
{
	return _T("��ʼҳ");
}

BOOL start_page::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CWnd::PreCreateWindow(cs);
}

CString start_page::get_markup_xml_file_path_name(app_theme_type& style_type)
{
	app_ui_interface* app = get_app_ui_interface();
	if (!app) return _T("");

	theme_style_settings settings = app->get_style_and_settings();

	// ���������ʱ��ͷ��ذ�ɫ����ʼҳ��
	if (!app_configs::is_skin_enable())
		return (file_tools::get_app_path_with_xg() + _T("gui_xml\\start_page_white.xaml")); 

	if (app_theme_type::theme_office_2007_blue == style_type
		|| app_theme_type::theme_office_2010_blue == style_type)
	{
		return file_tools::get_app_path_with_xg() + _T("gui_xml\\start_page_blue.xaml");
	}
	else if (app_theme_type::theme_office_2007_black == style_type
		|| app_theme_type::theme_office_2010_black == style_type)
	{
		return (file_tools::get_app_path_with_xg() + _T("gui_xml\\start_page_black.xaml"));
	}
	else if (app_theme_type::theme_office_2007_silver == style_type
		|| app_theme_type::theme_office_2010_silver == style_type
		|| app_theme_type::theme_office_2013_white == style_type)
	{
		return (file_tools::get_app_path_with_xg() + _T("gui_xml\\start_page_white.xaml"));
	}
	else if (app_theme_type::theme_office_2013_dark == style_type)
	{
		return (file_tools::get_app_path_with_xg() + _T("gui_xml\\start_page_dark.xaml"));
	}
	else
	{
		return _T("");
	}
}

void start_page::create_markup_page(const CString& file_path_name)
{
	if (file_path_name.IsEmpty()) return;

	// ���ļ��ڶ�ȡʱ��utf - 8���봦�����������޷�������2016 - 7 - 28�Ű�xtp��15.3.1������17.2.0ʱ����xtp�Ľű����治��֧��
	// ���ֽڣ����Կ���һ��Դ�룬Ӧ��ֻ֧��utf - 8, utf - 16��utf - 32��֧�֣�ucs - 16, ucs - 32Ҳ����֧�֣���������ע�Ͳ������ļ�����
	// Ϊ���ļ���ȡ���߼��������а���BOM��־��ȡ����С����BOM��ʽ�����༭ʱ��ע�ⲻ�ô�Ϊ��BOM�ı���
	// ���⣬17.2.0�汾�Ľű�����Ҫ��xmal�ĵ��������Ҫһ�����У������������
	// ���о�������°汾�������е��½�����Ƥ��������Ҳ�������˼���bug��
	// 15.3.0��ANSI��GB2312��������Ĵ��������ţ��ð汾��������������
	//CStringW xml_text;
	//try
	//{
	//	CStdioFile xml_file;
	//	if (TRUE == xml_file.Open(file_path_name, CFile::modeRead))
	//	{
	//		CString row;
	//		while (true)
	//		{
	//			if (FALSE == xml_file.ReadString(row)) break;

	//			// ����д��xml_file += row + _T("\n");����������ʱ�����ļ��о����кܳ����У���˷ֳ�����
	//			xml_text += row;
	//			xml_text += _T("\r\n");
	//		}
	//	}
	//	xml_file.Close();
	//}
	//catch (...)
	//{
	//	// nothing;
	//}

	// �˴���Ϊ��ȡutf-8/utf-16/utf-32������ı��ļ�����Դ��xtp 17.2.0 Markup.exeԴ���룬�����޸�
	CStringW xml_text;
	try
	{
		CFile xml_file;
		if (TRUE == xml_file.Open(file_path_name, CFile::modeRead))
		{
			wchar_t c;
			char t;
			while (xml_file.Read(&t, 1) == 1)
			{
				if (0 == (t & '\x80'))
				{
					c = t;
				}
				else if ('\xF0' == (t & '\xF0')) // 1111 - error, more than 16-bit char
				{
					c = L'\0';
				}
				else if ('\xE0' == (t & '\xF0')) // 1110xxxx 10xxxxxx 10xxxxxx
				{
					char t2,t3;
					xml_file.Read(&t2, 1);
					xml_file.Read(&t3, 1);

					c = (WCHAR) ((WCHAR(t & '\x0F') << 12) | (WCHAR(t2 & '\x3F') << 6) | WCHAR(t3 & '\x3F'));
				}
				else if ('\xC0' == (t & '\xE0')) // 110xxxxx 10xxxxxx 
				{
					char t2;
					xml_file.Read(&t2, 1);

					c = (WCHAR) ((WCHAR(t & '\x1F') << 6) | (t2 & '\x3F'));
				}
				else
				{
					c = L'\0';
				}

				if (c != L'\0') xml_text += c;
			}
		}
		xml_file.Close();
	}
	catch (...)
	{
		// nothing;
	}

	// 17.2.0�汾�Ľű�����Ҫ��xmal�ĵ��������Ҫһ�����У�������������¸�ʽ���ӻ��������
	xml_text += L"\r\n";
	
	const wchar_t* xml_text_buffer = xml_text.GetString();
	
	// ָ���������ע�ⲻ�ó���sizeof(wchar_t)��ԭ����xml_text_bufferΪwchar_t���͵ģ����ƫ��һ����Ԫ���Ѿ���һ��sizeof(wchar_t)�ĳ�����
	const wchar_t* xml_text_buffer_end = xml_text_buffer + (xml_text.GetLength()/* * sizeof(wchar_t)*/);

	CXTPMarkupParser sc;
	sc.SetBuffer(xml_text_buffer, xml_text_buffer_end);

	CXTPMarkupBuilder builder(this);
	CXTPMarkupUIElement* page = builder.Parse(&sc);
	CString last_error = builder.GetLastError();
	if (!last_error.IsEmpty())
	{
		AfxMessageBox(_T("start page error : ") + last_error, MB_OK | MB_ICONERROR);

		if (page)
		{
			page->Release();
			page = nullptr;
		}
	}

	if (m_markup_page)
	{
		// Release��delete(this)��û����Ϊnullptr
		m_markup_page->Release();
		m_markup_page = nullptr;
	}

	// �յ�Ҳ��������ȥ���Ա���ʾ�հ�ҳ��
	m_markup_page = page;

	return;
}

void start_page::load_dynamic_markup()
{
	if (!m_markup_page) return;

	CXTPMarkupStackPanel* stack_panel = dynamic_cast<CXTPMarkupStackPanel*>(m_markup_page->FindName(L"recent_list_items"));
	if (!stack_panel) return;

	// �ڲ�ÿһ��Ԫ��Ϊһ����ʷ�б�xml��ʽΪ��
	// <TextBlock FontSize = "11px" Height = "20">
	// 	<Hyperlink Foreground = "#FF15428B" TextDecorations = "" Tag = "command|sys.open">�����01</Hyperlink>
	// </TextBlock>

	app_ui_interface* app = get_app_ui_interface();
	if (!app) return;

	vector<CString> recent_list;
	app->get_recent_file_list(recent_list);

	for (CString& path_name : recent_list)
	{
		if (!PathFileExists(path_name)) continue;

		CString file_name = file_tools::get_name_from_file_path_name(path_name);
		if (file_name.IsEmpty()) continue;

		CXTPMarkupTextBlock* text_block = MARKUP_CREATE(CXTPMarkupTextBlock, this);
		if (!text_block) continue;
		text_block->SetTextAlignment(xtpMarkupTextAlignmentLeft);
		text_block->SetFontSize(11);
		text_block->SetHeight(20);

		CXTPMarkupHyperlink* hyperlink = MARKUP_CREATE(CXTPMarkupHyperlink, this);
		if (!hyperlink) continue;
		hyperlink->SetForeground((COLORREF) 0Xff8B4215); // 0xabgr
		hyperlink->SetTextDecorations(0);
		hyperlink->SetTag(new CXTPMarkupString(_T("command|sys.open|") + path_name));
		hyperlink->SetToolTip(path_name);
		
		CXTPMarkupRun* run_text = MARKUP_CREATE(CXTPMarkupRun, this);
		if (!run_text) continue;		
		run_text->SetText(new CXTPMarkupString(file_name));
		//run_text->SetForeground((COLORREF) 0Xff8B4215); // 0xabgr
		
		hyperlink->GetInlines()->Add(run_text);
		text_block->GetInlines()->Add(hyperlink);

		stack_panel->GetChildren()->Add(text_block);
	}
}

void start_page::DrawMarkup(CXTPMarkupDrawingContext* pDC, CRect rc)
{
	if (!m_markup_page) return;
	if (m_is_page_in_rendering) return;

	m_is_page_in_rendering = true;

	m_markup_page->Measure(pDC, rc.Size());
	m_markup_page->Arrange(rc);
	m_markup_page->Render(pDC);

	m_is_page_in_rendering = false;
}

void start_page::on_hyperlink_clicked_command(const CString& command, const CString& param)
{
	CString domain_text;
	CString cmd_text;
	cmd_container_interface::parse_command(command, domain_text, cmd_text);
	if (cmd_text.IsEmpty()) return;
	if (domain_text.IsEmpty()) domain_text = _T("app");

	cmd_trigger_manager::fire(domain_text, cmd_text, param);
}

void start_page::on_hyperlink_clicked_url(const CString& url)
{
	ShellExecute(GetSafeHwnd(), _T("open"), url, nullptr, nullptr, SW_SHOWNORMAL);
}

void start_page::on_hyperlink_clicked_file(const CString& file_path)
{
	ShellExecute(GetSafeHwnd(), _T("open"), file_path, nullptr, nullptr, SW_SHOWNORMAL);
}

int start_page::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	app_ui_interface* app = get_app_ui_interface();
	if (app)
	{
		theme_style_settings settings = app->get_style_and_settings();
		create_markup_page(get_markup_xml_file_path_name(settings.m_style_type));
		load_dynamic_markup();
	}

	// �����Ա�����ʼ������������ҳ���޷���Ӧ�κ���Ϣ
	m_hContextWnd = m_hWnd;

	return 0;
}

void start_page::OnSize(UINT type, int cx, int cy)
{
	CWnd::OnSize(type, cx, cy);
}

BOOL start_page::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void start_page::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CXTPClientRect rc(this);

	CXTPBufferDC dcMem(dc);
	dcMem.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));

	CXTPMarkupDrawingContext dcMarkup(this, dcMem);
	DrawMarkup(&dcMarkup, rc);
}

BOOL start_page::OnWndMsg(UINT message, WPARAM wparam, LPARAM lparam, LRESULT* result)
{
	if (m_markup_page && CXTPMarkupContext::OnWndMsg(m_markup_page, message, wparam, lparam, result))
		return TRUE;

	return CWnd::OnWndMsg(message, wparam, lparam, result);
}

void start_page::app_theme_changed(theme_style_settings& settings)
{
	create_markup_page(get_markup_xml_file_path_name(settings.m_style_type));
	load_dynamic_markup();
}

void start_page::OnCommandUpdate(const CString& strCommand, bool& enable, bool& checked)
{
	enable = false;
	checked = false;
}

void start_page::on_hyperlink_clicked(CXTPMarkupObject* sender, CXTPMarkupRoutedEventArgs* args)
{
	CXTPMarkupHyperlink* link_sender = dynamic_cast<CXTPMarkupHyperlink*>(sender);
	if (!link_sender) return;

	CXTPMarkupString* tag = static_cast<CXTPMarkupString*>(link_sender->GetTag());
	if (!tag) return;
		
	CString tag_text = string_tools::wstring_to_CString((LPCWSTR) (*tag));
	
	vector<CString> tag_items;
	string_tools::split_string(tag_items, tag_text, _T("|"));
	if (tag_items.empty())return;
	if (tag_items.front().IsEmpty()) return;

	if (0 == tag_items.front().CompareNoCase(_T("command")))
	{
		CString command = (tag_items.size() >= 2 ? tag_items[1] : _T(""));
		CString param = (tag_items.size() >= 3 ? tag_items[2] : _T(""));
		if (command.IsEmpty()) return;

		on_hyperlink_clicked_command(command, param);
	}
	else if (0 == tag_items.front().CompareNoCase(_T("url")))
	{
		CString url = (tag_items.size() >= 2 ? tag_items[1] : _T(""));
		if (url.IsEmpty()) return;

		on_hyperlink_clicked_url(url);
	}
	else if (0 == tag_items.front().CompareNoCase(_T("file")))
	{
		CString file_path = (tag_items.size() >= 2 ? tag_items[1] : _T(""));
		if (file_path.IsEmpty()) return;

		if (file_tools::is_relative_path(file_path))
		{
			CString first_char_on_left = file_path.Left(1);
			if (_T("/") == first_char_on_left || _T("\\") == first_char_on_left)
				first_char_on_left.Delete(0);

			file_path.Insert(0, file_tools::get_app_path_with_xg());
		}

		on_hyperlink_clicked_file(file_path);
	}
	else
	{
		// nothing;
	}


	args->SetHandled();
}
