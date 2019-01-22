#include "stdafx.h"

#include "THGridControl/THGridControlInc.h"
#include "THGridControl/THDataBase.h"

#include "frame_data.h"
#include "frame_grid_tools.h"
#include "../tools/table_templates.h"
#include "../tools/string_tools.h"

#include "hook_setting_dialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


IMPLEMENT_DYNAMIC(dialog_node_hook_setting, CDialog)
BEGIN_MESSAGE_MAP(dialog_node_hook_setting, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY_EX(GN_FOCUSCHANGE_PRE, IDC_STATIC_HOOKS, on_focus_changed)
END_MESSAGE_MAP()


dialog_node_hook_setting::dialog_node_hook_setting(CWnd* parent /*=NULL*/)
	: CDialog(dialog_node_hook_setting::IDD, parent)
{
	m_hook_excel = new CTHExcel();
	m_hook_settings_db = NULL;

}

dialog_node_hook_setting::~dialog_node_hook_setting()
{
	safe_delete(m_hook_excel);
	safe_delete(m_hook_settings_db);
}

void dialog_node_hook_setting::DoDataExchange(CDataExchange* dx)
{
	CDialog::DoDataExchange(dx);
}

BOOL dialog_node_hook_setting::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_hook_settings_db = table_templates::create_data_base(FRAME_HOOKS_SUB_TABLE);
	if (NULL == m_hook_settings_db)
	{
		_ASSERTE((_T("m_pDB = pTemplate->CreateDataBase(...)"), false));
		return TRUE;
	}

	m_hook_excel->CreateFromStatic(this, IDC_STATIC_HOOKS);
	m_hook_excel->SetUsingDataBase(m_hook_settings_db);
	m_hook_settings_db->SetDefRowHeight(22); // �����뷽�����ʦ��Ĭ���и�һ��

	m_hook_settings_db->SetEnvVariable(_T("m_strHookName"), m_hook_name_list);
	m_hook_settings_db->SetEnvVariable(_T("m_strRefLine"), m_ref_lines);
	m_hook_settings_db->SetEnvVariable(_T("m_strSegName"), m_ref_segment);

	vector<frame_hook_setting> vecSettings;
	string_to_hook_settings(m_setting_info_text, vecSettings);
	set_data_to_grid(vecSettings);

	m_hook_settings_db->ReFresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// ���һ���ַ������Ƿ���ĳ���ַ�,���򷵻�true
bool IsCharExist(const CString& strText, TCHAR chr)
{
	return (-1 != strText.Find(chr));
}

// ���һ���ַ������Ƿ��� [ ] , ; ���ĸ��ַ�,��ȷ�򷵻�true
bool is_right_hook_setting_item_text(const CString& strText)
{
	if (IsCharExist(strText, _T('['))) return false;
	if (IsCharExist(strText, _T(']'))) return false;
	if (IsCharExist(strText, _T(','))) return false;
	if (IsCharExist(strText, _T(';'))) return false;
	
	return true;
}

// ����乳�е������Ƿ���ȷ,������в����е��ַ������,��ȷ�򷵻�true
bool check_hook_setting_data(const frame_hook_setting& setting)
{
	if (!is_right_hook_setting_item_text(setting.m_hook_name)) return false;
	if (!is_right_hook_setting_item_text(setting.m_ref_line_name)) return false;
	if (!is_right_hook_setting_item_text(setting.m_is_top_hook)) return false;
	if (!is_right_hook_setting_item_text(setting.m_segment_name)) return false;
	if (!is_right_hook_setting_item_text(setting.m_node_begin_layer)) return false;
	if (!is_right_hook_setting_item_text(setting.m_node_index)) return false;

	return true;
}
void dialog_node_hook_setting::OnOK()
{
	vector<frame_hook_setting> hook_settings;
	get_data_from_grid(hook_settings);
	// [2011-08-11]���ڸ����˷ָ���Ϊ����ʾ��[ ] , ; �û�����������Щ��������ݵ��½����쳣
	// ����Ҫ���,���������,����ʾ,�����ֶԻ��򲻹ر�.
	
	bool bIsWrong = false;
	for (size_t i = 0; i != hook_settings.size(); ++i)
	{
		if (!check_hook_setting_data(hook_settings[i]))
		{
			bIsWrong = true;
			break;
		}
	}

	if (!bIsWrong)
	{
		hook_settings_to_string(hook_settings, m_setting_info_text);
		CDialog::OnOK();
	}
	else
	{
		MessageBox(_T("����������в�������� [ ] , ; �ĸ��ַ�"));
	}

}

void dialog_node_hook_setting::OnCancel()
{
	CDialog::OnCancel();
}

void dialog_node_hook_setting::OnClose()
{
	CDialog::OnClose();
}

void dialog_node_hook_setting::get_data_from_grid(OUT vector<frame_hook_setting>& setting_data)
{
	setting_data.clear();
	
	if (NULL == m_hook_settings_db)
	{
		_ASSERTE((_T("if (NULL == m_pHooksSettingDB)"), false));
		return;
	}

	int row_count = m_hook_settings_db->GetNumberRows(); // �������������Ǳ�����
	if (row_count <= 0) return; // �ձ�û������

	frame_hook_setting setting;
	for (int i = 0; i < row_count; ++i)
	{
		m_hook_settings_db->Get_Prop(i, _T("m_hook_name"), setting.m_hook_name);
		m_hook_settings_db->Get_Prop(i, _T("m_ref_line_name"), setting.m_ref_line_name);
		m_hook_settings_db->Get_Prop(i, _T("m_is_top_hook"), setting.m_is_top_hook);
		m_hook_settings_db->Get_Prop(i, _T("m_segment_name"), setting.m_segment_name);
		m_hook_settings_db->Get_Prop(i, _T("m_node_begin_layer"), setting.m_node_begin_layer);
		m_hook_settings_db->Get_Prop(i, _T("m_node_index"), setting.m_node_index);
		
		setting_data.push_back(setting);

		// ���汣���û���������Ϊ�������Ǵ�����乳���������洢������������ͼ�����䴦��

	} // end of for(...)

}

void dialog_node_hook_setting::set_data_to_grid(IN const vector<frame_hook_setting>& setting_data)
{
	int row_count = m_hook_settings_db->GetNumberRows();

	// ɾ�����е�������
	m_hook_settings_db->RemoveRowsAt(2, row_count);

	int nInsertRowsCount = int(setting_data.size());
	if (nInsertRowsCount <= 0) nInsertRowsCount = 5; // �����Ϊ��,�����5��

	// �������ɾ����Ȳ����㹻����У�Ϊ���Ч��û����ѭ��д�����ݵ�ʱ��һ��һ�еĲ��룬
	// ������д����֮ǰһ���Բ����㹻����к���������ݡ�
	OpReturnValue opFlag = m_hook_settings_db->InsertRowPost( 1, nInsertRowsCount );
	if (-1 == opFlag.count)
	{
		_ASSERTE((_T("���в������ʧ�ܣ��޷�������ɱ�����ݵĲ������"), false ));
		return;
	}

	// ���乳���ݲ��뵽�乳�����
	for (int i = 0; i < int(setting_data.size()); ++i) // ���±�Ϊ1���п�ʼ���룬�±�Ϊ0����ͨ���乳��
	{
		m_hook_settings_db->Set_Prop(i, _T("m_hook_name"), CTHVariant(setting_data[i].m_hook_name));
		m_hook_settings_db->Set_Prop(i, _T("m_ref_line_name"), CTHVariant(setting_data[i].m_ref_line_name));
		m_hook_settings_db->Set_Prop(i, _T("m_is_top_hook"), CTHVariant(setting_data[i].m_is_top_hook));
		m_hook_settings_db->Set_Prop(i, _T("m_segment_name"), CTHVariant(setting_data[i].m_segment_name));
		m_hook_settings_db->Set_Prop(i, _T("m_node_begin_layer"), CTHVariant(setting_data[i].m_node_begin_layer));
		m_hook_settings_db->Set_Prop(i, _T("m_node_index"), CTHVariant(setting_data[i].m_node_index));
	}
}

BOOL dialog_node_hook_setting::on_focus_changed( UINT nID, NMHDR* notify_struct, LRESULT* result )
{
	if (!m_hook_settings_db)
	{
		_ASSERTE((_T("CTHDataBase* pDataBase = pExcel->GetDataBase(); ����ֵΪ��ָ��"), false));
		return FALSE;
	}
	NM_THGRID* msg = (NM_THGRID*)notify_struct;
	if (NULL == msg)
	{
		_ASSERTE((_T("NM_THGRID *msg=(NM_THGRID *)pNotifyStruct; ����ֵΪ��ָ��"), false));
		return FALSE;
	}
	int row = msg->rowNo;
	int col = msg->colNo;

	if (2 != col || row < 2) return FALSE; // �ڶ����ǲο��Σ������д�2�п�ʼ

	CTHCell* cell = m_hook_settings_db->GetCell(row, col);
	if (!cell) return FALSE;

	cell->SetSketchMapName(m_setment_sketch);
	
	return TRUE;
}