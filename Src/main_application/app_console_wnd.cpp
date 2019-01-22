#include "stdafx.h"

#include <vector>

#include "x_framework/macro.h"
#include "x_framework/interface/console_interface.h"
#include "x_framework/interface/window_interface.h"
#include "x_framework/interface/cmd_container_interface.h"
#include "x_framework/tools/string_tools.h"
#include "x_framework/tools/math_tools.h"
#include "x_framework/tools/common_tools.h"


#include "app_console_wnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

#define MSG_COMMAND_EXECUTE_COMMAND		WM_USER + 1202
#define ID_COMMAND_EDIT_WND				201
#define ID_COMMAND_LIST_WND				202
#define ID_MENU_ITEM_COPY_ITEMS			300
#define ID_MENU_ITEM_CLEAR_ALL			301
#define ID_MENU_ITEM_CLEAR_REPEAT		302
#define ID_MENU_ITEM_SELECT_ALL			303
#define ID_MENU_ITEM_AUTO_SCROLL		304




BEGIN_MESSAGE_MAP(app_edit_for_console_wnd, CXTPEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

app_edit_for_console_wnd::app_edit_for_console_wnd()
{
	m_console_wnd = nullptr;

	m_text_color = RGB(0, 0, 0);
	m_back_color = RGB(255, 255, 255);
	m_border_color = RGB(0, 0, 0);
	m_back_brush.CreateSolidBrush(m_back_color);
}

app_edit_for_console_wnd::~app_edit_for_console_wnd()
{
}

void app_edit_for_console_wnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_console_wnd) return;

	if (VK_ESCAPE == nChar)
	{
		if (m_console_wnd) m_console_wnd->notify_console_action(0);
	}
	else if (VK_RETURN == nChar)
	{
		if (m_console_wnd) m_console_wnd->notify_console_action(1);
	}
	else if (VK_SPACE == nChar)
	{
		if (m_console_wnd) m_console_wnd->notify_console_action(2);
	}
	else
	{
		CXTPEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

HBRUSH app_edit_for_console_wnd::CtlColor(CDC* pDC, UINT nCtlColor)
{
	CDC* dc = GetDC(); // ����Ҫ�µ�dc,����ʹ�ô����pDC������Ĳ�������ֱ�ӻ��Ƶ������ϣ�Ҫ�˺������غ����ϵͳ���Ƶ�������
	if (dc && dc->GetSafeHdc())
	{
		CRect rect;
		GetWindowRect(&rect); // ����ͨ���ͻ����õ��߿���Ϊ�ͻ������Ķ����Ա������ִ�ֱ���У�Ҫͨ�������������õ�
		rect.MoveToXY(0, 0);
		CBrush border_brush(m_border_color);
		dc->FrameRect(rect, &border_brush); // ���ƾ��α߿�
	}

	pDC->SetTextColor(m_text_color);
	pDC->SetBkColor(m_back_color);
	return m_back_brush; // ���ػ�ˢ,�������������ؼ�����
}

void app_edit_for_console_wnd::set_care_to_end()
{
	int nLength = SendMessage(WM_GETTEXTLENGTH);
	SetSel(nLength, nLength);
}

void app_edit_for_console_wnd::set_back_color(COLORREF bk_color)
{
	if (bk_color == m_back_color) return;

	m_back_color = bk_color;
	if (m_back_brush.GetSafeHandle())
	{
		m_back_brush.DeleteObject();
		m_back_brush.CreateSolidBrush(m_back_color);
	}

	Invalidate();
}

void app_edit_for_console_wnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (VK_BACK == nChar || VK_DELETE == nChar)
	{
		CString cur_text;
		GetWindowText(cur_text);

		int start_sel = 0, end_sel = 0;
		GetSel(start_sel, end_sel); // ע��˽ӿڵõ������ַ������������ֽ���

		CString default_prompt = m_console_wnd->get_default_prompt();
		cur_text = cur_text.Left(default_prompt.GetLength());
		if (0 != cur_text.CompareNoCase(default_prompt))
		{
			CXTPEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			wstring wide_prompt = string_tools::CString_to_wstring(default_prompt);
			int default_prompt_lenght_in_tchar = wide_prompt.length();

			// ��ǰ����������ɾ���ַ�
			if (VK_BACK == nChar && start_sel > default_prompt_lenght_in_tchar // ���ܵ���
				|| VK_DELETE == nChar && start_sel >= default_prompt_lenght_in_tchar)
				CXTPEdit::OnChar(nChar, nRepCnt, nFlags);
		}

		return;
	}

	// �ݲ����ǲ������пո������������в��������룬�ո�Ŀǰ������������������open�����ָ���ļ�������
	// �����������뽻�����ʽ�����򿪶Ի�����ˣ�
	if (nChar == VK_ESCAPE && nChar == VK_RETURN && nChar == VK_SPACE) return;

	CString wnd_text;
	GetWindowText(wnd_text);

	// ��������ǰ׺�м䣬���Ƶ�ĩβ���Ա�������������
	int start, end, row, column;
	GetSel(start, end); //start��end��ֵ���ǲ�������ַ�����
	row = LineFromChar(start); //��ȡ�������������
	column = start - LineIndex(row); //��ȡ�������������(ע�������ַ�Ϊ��λ�������ֽ�)

	// ǰ������������»�������������
	// 1.ϵͳĬ�ϵ�ǰ����DB_COMMAND_PROPT
	// 2.�û��Զ����ǰ����ʾ��Ϣ
	// 3.DB_COMMAND_PROPT + �û��Զ����ǰ����ʾ��Ϣ�����ʼִ��ʱ�����У�����ɲο�CAD2010��
	// 4.��ǰ����

	if (!m_console_wnd) return;
	CString prompt_default = m_console_wnd->get_default_prompt();
	CString prompt_custom = m_console_wnd->m_last_prompt;
	CString prompt_mixed = prompt_default + prompt_custom;


	// �жϹ��λ�����ַ�Ϊ��λ,���ʹ��unicode���Լ��㷨�߼�,�����ֶ������ַ��ϲ����߼�
	std::wstring unicode_prompt_default = string_tools::CString_to_wstring(prompt_default);
	std::wstring unicode_prompt_custom = string_tools::CString_to_wstring(prompt_custom);
	std::wstring unicode_prompt_mixed = string_tools::CString_to_wstring(prompt_mixed);

	if (wnd_text.Left(prompt_default.GetLength()) == prompt_default)
	{
		if (column < (int) unicode_prompt_default.length()) // ������ǰ׺��ĩβ
		{
			int nLength = SendMessage(WM_GETTEXTLENGTH);
			SetSel(nLength, nLength);
		}
	}
	else if (wnd_text.Left(prompt_custom.GetLength()) == prompt_custom)
	{
		if (column < (int) unicode_prompt_custom.length()) // ������ǰ׺��ĩβ
		{
			int nLength = SendMessage(WM_GETTEXTLENGTH);
			SetSel(nLength, nLength);
		}
	}
	else if (wnd_text.Left(prompt_mixed.GetLength()) == prompt_mixed)
	{
		if (column < (int) unicode_prompt_mixed.length()) // ������ǰ׺��ĩβ
		{
			int nLength = SendMessage(WM_GETTEXTLENGTH);
			SetSel(nLength, nLength);
		}
	}
	else
	{
		// nothing;
	}

	CXTPEdit::OnChar(nChar, nRepCnt, nFlags);
}



BEGIN_MESSAGE_MAP(app_listbox_for_console_wnd, CXTPListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_ITEM_CLEAR_ALL, on_clear_all_items)
	ON_COMMAND(ID_MENU_ITEM_CLEAR_REPEAT, on_clear_repeat_items)
	ON_COMMAND(ID_MENU_ITEM_COPY_ITEMS, on_copy_selected_items_to_clipboard)
	ON_COMMAND(ID_MENU_ITEM_SELECT_ALL, on_select_all_items)
	ON_COMMAND(ID_MENU_ITEM_AUTO_SCROLL, on_auto_scroll_to_end_item)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

app_listbox_for_console_wnd::app_listbox_for_console_wnd()
{
	m_auto_scroll = true;
	m_back_color = RGB(255, 255, 255);
	m_back_brush.CreateSolidBrush(m_back_color);
}

app_listbox_for_console_wnd::~app_listbox_for_console_wnd()
{
}

void app_listbox_for_console_wnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	CString text = _T("");
	int index = (int) (lpDrawItemStruct->itemID);
	if (index >= 0 && index < GetCount()) GetText(lpDrawItemStruct->itemID, text);

	// ������Ŀ�Ļ����Ǽ�ʱ��ʾ�ģ���˱������ֻ�����ܶž���˸
	CRect rect = lpDrawItemStruct->rcItem;
	CDC mem_dc;
	CBitmap mem_map;
	mem_dc.CreateCompatibleDC(dc);
	mem_map.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
	mem_dc.SelectObject(&mem_map);
	mem_dc.SelectObject(GetFont()->GetSafeHandle());

	CRect mem_rect = rect;
	mem_rect.MoveToXY(0, 0);

	//-----------------------------------------------------------

	// ��������ȡɫ��ʽ����XTP��ListBox���ƴ���
	COLORREF list_back_color = m_back_color; // GetBackColor(); ��ȡxtp���õı���ɫ
	COLORREF list_text_color = IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT) : GetXtremeColor(COLOR_GRAYTEXT);

	bool has_focus = (TRUE == HasFocus());
	bool is_selected = (ODS_SELECTED == (lpDrawItemStruct->itemState & ODS_SELECTED));
	//bool is_highlighted = (lpDrawItemStruct->itemID == m_nHotItem);
	if ((is_selected /*|| is_highlighted*/) && IsWindowEnabled() && has_focus)
	{
		// ���»��ƴ�������XTP��ListBox���ƴ��룬�˷������������ļ���Ƥ���ļ�����������仯
		CXTPResourceImages* resource_images = XTPResourceImages();
		CXTPResourceImage* list_image = (resource_images ? resource_images->LoadFile(_T("LISTBOX")) : nullptr);
		if (list_image)
			list_image->DrawImage(&mem_dc, mem_rect, list_image->GetSource(1, 4), CRect(4, 4, 4, 4), COLORREF_NULL);
		else
			mem_dc.FillSolidRect(&mem_rect, RGB(190, 190, 190)); // �����ļ��в�һ��������listbox�ı���ͼƬ���п�������ɫ���ģʽ
	}
	else
	{
		mem_dc.FillSolidRect(&mem_rect, list_back_color);
	}

	mem_rect.left += 3; // ���ֲ�Ҫ����ͷ�����򲻺ÿ�
	mem_dc.SetBkMode(TRANSPARENT);
	mem_dc.SetTextColor(get_text_color(text));
	mem_dc.DrawText(text, &mem_rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_EXPANDTABS);

	//-----------------------------------------------------------

	dc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);

	mem_dc.DeleteDC();
	mem_map.DeleteObject();
}

BOOL app_listbox_for_console_wnd::PreTranslateMessage(MSG* msg)
{
	// ��λȫ1��ʾ���£�����û��
	bool is_control_key_be_pressed = (0xff == HIBYTE(GetKeyState(VK_CONTROL)));
	bool is_shift_key_be_pressed = (0xff == HIBYTE(GetKeyState(VK_SHIFT)));
	bool is_alt_key_be_pressed = (0xff == HIBYTE(GetKeyState(VK_MENU)));

	if (is_control_key_be_pressed && !is_shift_key_be_pressed && !is_alt_key_be_pressed)
	{
		int key_value = msg->wParam;
		if ('C' == key_value || 'c' == key_value)
			on_copy_selected_items_to_clipboard();
		else if ('D' == key_value || 'd' == key_value)
			on_clear_all_items();
		else if ('R' == key_value || 'r' == key_value)
			on_clear_repeat_items();
		else if ('A' == key_value || 'a' == key_value)
			on_select_all_items();
		else
			; // nothing
	}

	return CXTPListBox::PreTranslateMessage(msg);
}

COLORREF app_listbox_for_console_wnd::GetBackColor()
{
	return m_back_color;
}

void app_listbox_for_console_wnd::on_clear_repeat_items()
{
	if (!GetSafeHwnd()) return;

	CWaitCursor tmp;

	// ԭ���Ĵ���Ч��̫�ͣ�һ����Ŀ����������һ�������¼ȥ�ظ���Ҫ����ӣ���˸�Ϊ�麯����ʵ��һ�£����������ϴ���
	vector<CString> all_info;
	for (const auto& item : m_all_text)
	{
		auto it = std::find(all_info.begin(), all_info.end(), item);
		if (it == all_info.end()) all_info.push_back(item);
	}

	m_all_text = all_info;

	// ���µ�������
	LockWindowUpdate();
	ResetContent();
	for (const auto& x : all_info) AddString(x);
	UnlockWindowUpdate();
	UpdateWindow();
}

void app_listbox_for_console_wnd::on_clear_all_items()
{
	if (!GetSafeHwnd()) return;

	CWaitCursor tmp;
	m_all_text.clear(); // ���ص�Ŀ���ǰѻ��������Ҳ��գ������ڴ�ռ�ò���ȷ������ͳ��Ҳ������
	ResetContent();

	m_auto_scroll = true;
}

void app_listbox_for_console_wnd::on_copy_selected_items_to_clipboard()
{
	CWaitCursor wait_cur;

	// ����գ������������Ϸţ�ȷ����ĳ���ŵ����������
	if (!OpenClipboard()) return;
	EmptyClipboard();
	CloseClipboard();

	int row_count = GetCount();
	if (row_count <= 0) return;

	// ��ҪԤ����ֵ����Ϊ��֪��MFC�Ƿ񻺴����ֵ�����û�л��棬��ÿ�ε��ö�Ҫ������ʱ�����Ĵ󣬺���ȡѡ�����ݵ�
	// ʱ���ֹ�ͳ�Ƹ���ʡ��ʱ��
	int selected_row_count = 0; // GetSelCount();
	
	std::wstring selected_merge_string;
	CString item_text;
	for (int i = 0; i < row_count; i++)
	{
		if (GetSel(i) > 0)
		{
			GetText(i, item_text);
			item_text += _T("\n");
			selected_merge_string += string_tools::CString_to_wstring(item_text);
			++selected_row_count;
		}
	}

	if (selected_row_count <= 0 || item_text.IsEmpty()) return;

	int buffer_size_in_byte = (selected_merge_string.length() + 1) * sizeof(wchar_t);

	if (!OpenClipboard()) return;
	
	EmptyClipboard();

	HGLOBAL global_buffer_handle = nullptr;
	global_buffer_handle = GlobalAlloc(GHND, buffer_size_in_byte);
	char* buffer = (char*) GlobalLock(global_buffer_handle);
	
	// ���ֽ�Ϊ��λ���ƣ��������ַ������ƺ�������Ϊ������������ͣ������unicodeӢ����ĸ��һ�ֽ�Ϊ'\0'��
	memcpy_s(buffer, buffer_size_in_byte, (char*) selected_merge_string.c_str(), buffer_size_in_byte);

	GlobalUnlock(global_buffer_handle);
	SetClipboardData(CF_UNICODETEXT, global_buffer_handle);

	CloseClipboard();
}

void app_listbox_for_console_wnd::on_select_all_items()
{
	CWaitCursor wait_cur;

	SetFocus();

	// ����ѡ��״̬ʱ���Զ���������Ӧ��Ŀ���˷Ѵ���ʱ���ڽ���ˢ���ϣ����Ϊ���Ч����������
	LockWindowUpdate();
	int row_count = GetCount();
	for (int i = 0; i < row_count; i++) SetSel(i, TRUE);
	UnlockWindowUpdate();
	UpdateWindow();
}

void app_listbox_for_console_wnd::on_auto_scroll_to_end_item()
{
	m_auto_scroll = !m_auto_scroll;
}

void app_listbox_for_console_wnd::OnLButtonDown(UINT flag, CPoint pt)
{
	m_auto_scroll = false;
	CXTPListBox::OnLButtonDown(flag, pt);
}

void app_listbox_for_console_wnd::OnLButtonDblClk(UINT flag, CPoint pt)
{
	m_auto_scroll = false;
	CXTPListBox::OnLButtonDblClk(flag, pt);
}

HBRUSH app_listbox_for_console_wnd::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkColor(m_back_color);
	return m_back_brush; // ���ػ�ˢ,�������������ؼ�����
}

void app_listbox_for_console_wnd::add_string(const CString& text)
{
	m_all_text.push_back(text);
	int added_index = AddString(text);
	if (m_auto_scroll) SetTopIndex(added_index);
}

void app_listbox_for_console_wnd::set_back_color(COLORREF bk_color)
{
	if (bk_color == m_back_color) return;

	m_back_color = bk_color;
	if (m_back_brush.GetSafeHandle())
	{
		m_back_brush.DeleteObject();
		m_back_brush.CreateSolidBrush(m_back_color);
	}

	Invalidate();
}

COLORREF app_listbox_for_console_wnd::get_text_color(const CString& text) const
{
	COLORREF crText = RGB(0, 0, 0);
	if (text.Left(CString(_T("�ܼ�")).GetLength()) == _T("�ܼ�"))
		crText = RGB(0, 0, 0);
	else if (text.Left(CString(_T("���ؾ���")).GetLength()) == _T("���ؾ���"))
		crText = RGB(255, 0, 255);
	else if (text.Left(CString(_T("����")).GetLength()) == _T("����"))
		crText = RGB(0, 0, 255);
	else if (text.Left(CString(_T("����")).GetLength()) == _T("����"))
		crText = RGB(255, 0, 0);
	else
		crText = RGB(0, 0, 0);

	return crText;
}

void app_listbox_for_console_wnd::OnContextMenu(CWnd* wnd, CPoint pt)
{
	CXTPListBox::OnContextMenu(wnd, pt);

	SetFocus();

	CMenu pop_up_menu;
	pop_up_menu.CreatePopupMenu();
	pop_up_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_COPY_ITEMS, _T("����(&C)\tCtrl+C"));
	pop_up_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_CLEAR_ALL, _T("���(&D)\tCtrl+D"));
	pop_up_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_CLEAR_REPEAT, _T("ȥ���ظ�(&R)\tCtrl+R"));
	pop_up_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_SELECT_ALL, _T("ȫѡ(&R)\tCtrl+A"));
	pop_up_menu.AppendMenu(MF_SEPARATOR);
	pop_up_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_AUTO_SCROLL, _T("�Զ�����"));
	
	//pop_up_menu.EnableMenuItem(ID_MENU_ITEM_COPY_ITEMS, MF_BYCOMMAND | MF_ENABLED);
	//pop_up_menu.EnableMenuItem(ID_MENU_ITEM_CLEAR_ALL, MF_BYCOMMAND | MF_ENABLED);
	//pop_up_menu.EnableMenuItem(ID_MENU_ITEM_CLEAR_REPEAT, MF_BYCOMMAND | MF_ENABLED);
	//pop_up_menu.EnableMenuItem(ID_MENU_ITEM_SELECT_ALL, MF_BYCOMMAND | MF_ENABLED);
	pop_up_menu.CheckMenuItem(ID_MENU_ITEM_AUTO_SCROLL, MF_BYCOMMAND | (m_auto_scroll ? MF_CHECKED : MF_UNCHECKED));
	
	m_auto_scroll = false;
	pop_up_menu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	pop_up_menu.DestroyMenu();
}




IMPLEMENT_SERIAL(app_console_wnd, CWnd, 1)
BEGIN_MESSAGE_MAP(app_console_wnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


app_console_wnd::app_console_wnd() : CWnd(), console_interface()
{
}

app_console_wnd::~app_console_wnd()
{
}
 
BOOL app_console_wnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (CWnd::OnCreate(lpCreateStruct) == -1) return FALSE;

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_MULTIPLESEL | LBS_EXTENDEDSEL
		| WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_CLIPCHILDREN/* | WS_CLIPSIBLINGS*/;
	if (!m_output_list_box.Create(dwStyle, CRect(), this, ID_COMMAND_LIST_WND)) return FALSE;
	m_output_list_box.SetFont(CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)));
	
	// �б���±߿򸲸�ס�ı��򣬷�����������ʾ̫������б��ҪWS_CLIPSIBLINGS���ԣ�Ӧ�ü����ı�����
	if (!m_edit_box.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, CRect(), this, ID_COMMAND_EDIT_WND)) return FALSE;
	m_edit_box.SetFont(CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)));
	m_edit_box.m_console_wnd = this;
	
	//���ǰ�������廻�ɵȿ�����
	//{
	//	LOGFONT logfont;
	//	memset(&logfont, 0, sizeof(LOGFONT));
	//	logfont.lfHeight = 12;
	//	logfont.lfCharSet = GB2312_CHARSET;
	//	logfont.lfPitchAndFamily = (BYTE)FIXED_PITCH;
	//	//����_GB2312/������/����_GB2312/����/����-���������ַ���
	//	_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, "����");
	//	CFont  tempFont;
	//	tempFont.CreateFontIndirect(&logfont);
	//	m_pListCtrl.SetFont(&tempFont);
	//}

	// ����ע��������ƴ���
	set_console_interface(this);
	add_prompt(get_default_prompt(), false, false);

	// ���ڴ���ʱ�ֹ�������ʼ״̬������
	app_ui_interface* app = get_app_ui_interface();
	if (app)
	{
		theme_style_settings app_theme = app->get_style_and_settings();
		app_theme_changed(app_theme);
	}

	return TRUE;
}

void app_console_wnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetSafeHwnd()) return;
	if (!m_edit_box.GetSafeHwnd()) return;
	if (!m_output_list_box.GetSafeHwnd()) return;

	// ������һ�����أ����ı���ĵױ߿������ڵĵױ߿��ס��������������ʾ̫����������Ҳһ��
	CRect list_wnd_rect(0, 0, cx + 1, cy - 22 + 1);
	m_output_list_box.MoveWindow(list_wnd_rect);

	CRect edit_wnd_rect(-2, list_wnd_rect.bottom - 1, cx + 1, cy + 1);
	m_edit_box.MoveWindow(edit_wnd_rect);
}

void app_console_wnd::OnClose()
{
	// ���ⴰ�����ٺ��ٱ����õ��±���
	set_console_interface(nullptr);
	CWnd::OnClose();
}

CString app_console_wnd::get_current_input() const
{
	if (!GetSafeHwnd()) return _T("");
	if (!m_edit_box.GetSafeHwnd()) return _T("");

	CString strText;
	m_edit_box.GetWindowText(strText);

	// ��DB_COMMAND_PROPT��ͷʱ��ǰ����ȥ��
	CString prompt = get_default_prompt();
	if (strText.Left(prompt.GetLength()) == prompt)
		strText = strText.Mid(prompt.GetLength());

	strText.Trim();
	CString custom_propt = m_last_prompt; // �����״�ִ��ʱ����DB_COMMAND_PROPTǰ������׷���Զ�����ʾ������Ҫȥ���Σ�����ִ�й�����ֻ���û��Լ���ǰ����
	if (strText.Left(custom_propt.GetLength()) == custom_propt)
		strText = strText.Mid(custom_propt.GetLength());

	return strText;
}

void app_console_wnd::clear_current_input()
{
	if (!GetSafeHwnd()) return;
	if (!m_edit_box.GetSafeHwnd()) return;

	CString propt = get_default_prompt();

	CString strText;
	m_edit_box.GetWindowText(strText);

	// �����״�ִ��ʱ����DB_COMMAND_PROPTǰ������׷���Զ�����ʾ������Ҫȥ���Σ�����ִ�й�����ֻ���û��Լ���ǰ����
	if (strText.Left(m_last_prompt.GetLength()) == m_last_prompt && !m_last_prompt.IsEmpty())
		propt = m_last_prompt;

	m_edit_box.SetWindowText(propt);
	m_last_prompt = propt;
}

void app_console_wnd::clear_all_output()
{
	console_interface::clear_all_output();
	m_output_list_box.on_clear_all_items();
}

void app_console_wnd::add_prompt(const CString& prompt, bool new_line, bool end_line)
{
	if (!GetSafeHwnd()) return;
	if (!m_edit_box.GetSafeHwnd()) return;
	if (!m_output_list_box.GetSafeHwnd()) return;

	// �����ǰ�Ѿ���Ĭ����ʾ����û�������κβ������ö�ˢһ�οղ����е��б���
	if (get_current_input().IsEmpty() && prompt.IsEmpty()) return;

	CString text_in_edit; // �༭�������ݣ�����ǰ�������������
	m_edit_box.GetWindowText(text_in_edit);

	// ����������������Ϊ��ʱ���ѵ�ǰ�ı����е�����ˢ���б��У��һص�ԭ����Ĭ����ʾ��״̬��
	if (prompt.IsEmpty())
	{
		// ʹ��print_info�ܼ�¼����־������Ҫֱ��������б���
		print_info(OO_UI_LOG, MT_INFO, text_in_edit);

		m_last_prompt = get_default_prompt();
		m_edit_box.SetWindowText(get_default_prompt());
		m_edit_box.set_care_to_end();

		return;
	}

	// ��ס��һ����ʾ����(������Ϊ�գ����Ѿ�������)���Ա��û���һ�������������ʹ���Լ�����ʾ����ʱ�������ܰ�������ʾȥ����ֻ����������ݸ��û�
	// ��Ϊ���Ҳ���Ĭ����ʾ����ʾ��Ȼ��ִ����һ������Ĺ����У�������ʾ�ϴε���ʾ��
	CString added_prompt = (prompt.IsEmpty() ? get_default_prompt() : prompt);

	// �ȴ���༭���е����ݣ���д��Ҫ���������
	if (new_line)
	{
		// ����ԭ������ˢ���б������ֻ��ǰ������հ�����Ҳ���
		print_info(OO_UI_LOG, MT_INFO, text_in_edit); // ʹ�ô˽ӿ�����ܼ�¼��Ŀ¼
		
		m_last_prompt = added_prompt; // ���һ�ε���ʾ������Ϊ���µ�
		m_edit_box.SetWindowText(added_prompt);
		m_edit_box.set_care_to_end();
	}
	else
	{
		// �ѵ�ǰ����׷�ӵ�ԭ���༭�������ݵĺ��棬��Ϊһ����������ʾ���������루��ԭ����������ݲ�����ɾ����
		// ������������ֻ�е���ʾ��(�����ǲ���Ĭ�ϵ�)����û�����ݣ���Ҫ��ʾĬ�ϵ���ʾ��
		if (get_current_input().IsEmpty())
			text_in_edit = added_prompt; // �����ʾ��
		else
			text_in_edit += added_prompt; // �ۼӵ�ԭ�ַ����У����滹Ҫ�õ�

		m_last_prompt = text_in_edit;
		m_edit_box.SetWindowText(text_in_edit);
		m_edit_box.set_care_to_end();
	}

	if (end_line)
	{
		// ����ԭ������ˢ���б������ֻ��ǰ������հ�����Ҳ���
		print_info(OO_UI_LOG, MT_INFO, m_last_prompt); // ʹ�ô˽ӿ�����ܼ�¼��Ŀ¼

		// ��Ҫ�Զ����Ĭ����ʾ������һ�ε��û���ʾ��������Զ��֪���û�������Ҫ��ô����������ʾĬ����ʾ������
		// �û���ʾ�������û�������Ҫ��ʾĬ����ʾ��ʱ�û���������Ҫ��ʱ���ֶ�����
		m_last_prompt = _T(""); // ���һ�ε���ʾ������Ϊ���µ�
		m_edit_box.SetWindowText(_T(""));
		m_edit_box.set_care_to_end();
	}
	
	// ʵʱ���½���
	m_edit_box.RedrawWindow();
	m_output_list_box.RedrawWindow();
}

void app_console_wnd::add_input(const CString& str)
{
	// �Ѳ����͵��༭���У����ܰѲ�����ͬ��ʾ��ͨ��AddPrompt�͵��༭���У���������ᱻ��Ϊ����ʾ����һ����
	// add_prompt(m_last_prompt + str, true, false);
	if (m_edit_box.GetSafeHwnd())
	{
		CString text;
		m_edit_box.GetWindowText(text);
		m_edit_box.SetWindowText(text + str);

		// �ѹ����õ���ĩβ
		m_edit_box.set_care_to_end();
	}
}

void app_console_wnd::print_info_implement(output_object out, message_type type, const CString& text)
{
	console_interface::print_info_implement(out, type, text);

	// �������ʱʹ�õ����Ϲ��������Լ�������������ǰ׺������־�����ʱҪ�ֶ����
	CString ouput_text = text;

	if (MT_ERROR == type)
		ouput_text = _T("����") + ouput_text;
	else if (MT_WARNING == type)
		ouput_text = _T("���棺") + ouput_text;
	else if (MT_SERIOUS_WARNING == type)
		ouput_text = _T("���ؾ��棺") + ouput_text;
	else if (MT_INFO == type)
		; // nothing // ������Ϣǰ����ǰ׺
	else if (MT_DEBUG == type)
		ouput_text = _T("���ԣ�") + ouput_text;
	else
		; // nothing

	// ��������־���(������Ϣ�����������־��
	if (OO_UI_LOG == out || OO_LOG == out) save_text_to_log_file(ouput_text);

	if (!app_is_debug_mode() && MT_DEBUG == type) return; // �ǵ��԰汾�����������Ϣ

	m_output_list_box.add_string(ouput_text);
}

void app_console_wnd::on_char_input(UINT char_value, UINT repeat_count, UINT flags)
{
	if (!GetSafeHwnd()) return;
	if (!m_edit_box.GetSafeHwnd()) return;

	// ʼ�ս�����λ�ö�λ�����
	m_edit_box.set_care_to_end();

	// ����ֱ�ӵ�OnChar()ֱ�Ӵ�����Ϣ�Ľӿڣ�Ӧ��Ϊ����Ϣ������mfc������Ӧ���ڲ����ݼ�¼������Ȼ���������Ӧ������
	// �����ö�Ӧ����Ӧ���������ֱ�ӵ���������������һ���ڵ����ڷ�OnChar��Ϣ�е���OnTextʱʧЧ
	// ����MSDN��֪WM_CHAR��Ϣ�Ĳ�������:
	// �ظ��������ڵڶ���������0-15λ�������nFlagsȥ����16λ������nRepCnt��16λ������Ч������Χ�ķǷ�����
	unsigned int key_data = (flags & 0xffff0000) | (repeat_count & 0x0000ffff);

	// on_text_inputֻ����OnChar��Ϣ��KeyDown��Ϣ�༭�������Լ�����
	m_edit_box.SendMessage(WM_CHAR, (WPARAM)char_value, (LPARAM)key_data);
}

void app_console_wnd::app_theme_changed(theme_style_settings& settings)
{
	if (m_output_list_box.GetSafeHwnd())
		m_output_list_box.set_back_color(settings.m_window_back_color);

	if (m_edit_box.GetSafeHwnd())
		m_edit_box.set_back_color(settings.m_window_back_color);
}

