#include "stdafx.h"

#include "THGridControl/THGridControlMac.h"
#include "THGridControl/common.h"
#include "THGridControl/THDataBase.h"
#include "THGridControl/THExcel.h"
#include "THGridControl/THBaseExcel.h"

#include "../interface/app_ui_interface.h"
#include "x_mvc_editor_base.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


IMPLEMENT_DYNAMIC(x_mvc_editor_base, CWnd)
BEGIN_MESSAGE_MAP(x_mvc_editor_base, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

x_mvc_editor_base::x_mvc_editor_base()
{
	m_ui_observer = nullptr;
}

x_mvc_editor_base::~x_mvc_editor_base()
{
	safe_delete(m_ui_observer);
}

bool x_mvc_editor_base::load_data_to_ui(const CString& data_tag)
{
	return true;
}

bool x_mvc_editor_base::save_data_from_ui()
{
	return true;
}

CString x_mvc_editor_base::get_title()
{
	return (m_ui_observer ? m_ui_observer->get_title() : _T(""));
}

void x_mvc_editor_base::app_theme_changed(theme_style_settings& settings)
{
}

void x_mvc_editor_base::on_project_data_notify(const CString& node_tag, notifer_action ac)
{
}

bool x_mvc_editor_base::need_save_ui_data()
{
	return false;
}

int x_mvc_editor_base::get_self_index_in_working_space() const
{
	app_ui_interface* ui = get_app_ui_interface();
	if (!ui) return -1;

	return ui->get_working_space_view_index(this);
}

void x_mvc_editor_base::set_data_node_tag(const CString& tag)
{
	m_data_tag = tag;
	if (m_ui_observer) m_ui_observer->m_data_node_tag = tag;
}

void x_mvc_editor_base::set_ui_observer(x_observer_base* observer)
{
	m_ui_observer = observer;
	if (m_ui_observer)
	{
		m_ui_observer->m_data_node_tag = m_data_tag;
		m_ui_observer->m_owner_window = this;
	}
}

CString x_mvc_editor_base::get_data_node_tag() const
{
	return m_data_tag;
}

x_observer_base* x_mvc_editor_base::get_ui_observer() const
{
	return m_ui_observer;
}

int x_mvc_editor_base::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		lpCreateStruct->style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		lpCreateStruct->style &= ~WS_BORDER;
		if (-1 == CWnd::OnCreate(lpCreateStruct)) return -1;

		// ���ڴ���ʱ�ֹ�������ʼ״̬������
		app_ui_interface* app = get_app_ui_interface();
		if (app)
		{
			theme_style_settings app_theme = app->get_style_and_settings();
			app_theme_changed(app_theme);
		}

		return 0;
}

void x_mvc_editor_base::OnSize(UINT type, int cx, int cy)
{
	CWnd::OnSize(type, cx, cy);
}

void active_workspace_mvc_editor(const CString& data_node_tag,
	const CString& ui_observer_class_name, const CString& mvc_editor_class_name)
{
	if (mvc_editor_class_name.IsEmpty())
	{
		CString err_info = _T("�༭������������Ϊ�գ��༭������ʧ��");
		AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
		return;
	}

	// �ڲ�ʹ�ô���ʵ���ķ�ʽ����������Ӵ��ڣ�������ʹ��������ԭ���������ⲿָ����Ӧ�Ĺ۲�����ȣ����Ҫ�Լ����
	// ��Ӧ�ڵ�����ݴ�����û���Ѿ����
	app_ui_interface* app = get_app_ui_interface();
	if (!app)
	{
		CString err_info = _T("�޷���ȡ����ʵ���ӿڣ��༭������ʧ��");
		AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
		return;
	}

	// �Ѵ����˴������л������ڲ����أ��˽ӿ�֧������Ϊ�յļ��
	if (app->is_working_space_view_exist(mvc_editor_class_name, data_node_tag))
	{
		app->select_working_space_view(mvc_editor_class_name, data_node_tag);
		return;
	}

	// ����Ϊ��
	//if (data_node_tag.IsEmpty() || ui_observer_class_name.IsEmpty())
	//{
	//	CString err_info = _T("����������������������������Ϊ�գ��༭������ʧ��");
	//	AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
	//	return;
	//}

	unique_ptr<x_observer_base> ui_observer;
	if (!ui_observer_class_name.IsEmpty())
	{
		CObject* ui_observer_obj = CRuntimeClass::CreateObject(ui_observer_class_name);
		if (!ui_observer_obj)
		{
			CString err_info = _T("�� [ ") + ui_observer_class_name + _T(" ] �޷�ʵ�������༭������ʧ��");
			AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
			return;
		}

		ui_observer.reset(dynamic_cast<x_observer_base*>(ui_observer_obj));
		if (!ui_observer)
		{
			safe_delete(ui_observer_obj);
			CString err_info = _T("�� [ ") + ui_observer_class_name + _T(" ] �޷�ʵ�������༭������ʧ��");
			AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
			return;
		}
	}

	unique_ptr<x_mvc_editor_base> editor_wnd;
	{
		CObject* editor_base = CRuntimeClass::CreateObject(mvc_editor_class_name);
		if (!editor_base)
		{
			CString err_info = _T("�༭���� [ ") + mvc_editor_class_name + _T(" ] �޷�ʵ�������༭������ʧ��");
			AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
			return;
		}

		editor_wnd.reset(dynamic_cast<x_mvc_editor_base*>(editor_base));
		if (!editor_wnd)
		{
			safe_delete(editor_base);
			CString err_info = _T("�༭���� [ ") + mvc_editor_class_name + _T(" ] �޷�ʵ�������༭������ʧ��");
			AfxMessageBox(err_info, MB_OK | MB_ICONERROR);
			return;
		}
	}

	// �������ⲿ�����ȷ��ڱ༭���ڲ������ã�һ�ǿ��Աܿ����ݴ���ʧ��ʱ���µı༭����ʵ�������������Ǹ�����չʾ����
	// �����ݰ����ԭ�������Ժ���һ������Ч�������ǿ����ⲿʹ�û������������Ч�ʲ�Ϊ�༭��ֱ���ṩ���ݶ�����
	// ����һ�Ժ�������ҵ��仯�������̱临�ӻ�����Ժ��ʺ�ʹ����������ģʽ�������̽�����֯
	editor_wnd->set_ui_observer(ui_observer.release());
	editor_wnd->set_data_node_tag(data_node_tag);

	// �ڲ��Զ������������id
	static unsigned int editor_wnd_start_id = 1101;

	// ���������ٴ��������ڱ���ָ�������ڣ����򴴽�ʧ�ܣ��˴�����ָ��һ������ӵ���������ܻ��Զ�����Ϊ��ȷ�ĸ�����
	// ��������ӵ��������ᱻǿ�Ƹ�Ϊ���������ڣ����˴�����Ҫָ��һ���ǿյĴ��ڣ�����mfc�޷���������
	BOOL flag = editor_wnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
		CRect(), AfxGetApp()->m_pMainWnd, ++editor_wnd_start_id);

	if (TRUE != flag)
	{
		//safe_delete(editor_wnd);
		AfxMessageBox(_T("�༭������ʧ��"), MB_OK | MB_ICONERROR);
		return;
	}

	app->add_working_space_view(editor_wnd.release(), data_node_tag);
}

