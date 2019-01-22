#include "StdAfx.h"
#include "Resource.h"

#include "x_framework/macro.h"
#include "x_framework/tools/file_tools.h"
#include "x_framework/interface/cmd_container_interface.h"
#include "x_framework/interface/window_interface.h"
#include "x_framework/interface/app_ui_interface.h"

#include "main_application.h"
#include "main_frame.h"

#include "app_ribbon_bar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


IMPLEMENT_DYNCREATE(app_ribbon_bar, CXTPRibbonBar)
BEGIN_MESSAGE_MAP(app_ribbon_bar, CXTPRibbonBar)
END_MESSAGE_MAP()

app_ribbon_bar::app_ribbon_bar(void)
{
	m_recent_file_list_count = 5;
	m_recent_list.clear();
	m_file_path_max_length = 35;
}

app_ribbon_bar::~app_ribbon_bar( void )
{

}

bool app_ribbon_bar::initlize_ribbon_bar(const ribbon_bar_config_data& ribbon_ui_config)
{
	if (!GetSafeHwnd()) return false; // ����Ҫ�ؼ��Ѿ��������ܵ��ó�ʼ������

	m_context_tab_info_cache.clear();

	SetMenu(nullptr); // ���ϵͳ�Դ������˵�������еĻ���
	initlize_ribbon_bar_file_menu(ribbon_ui_config);

	//////////////////////////////////////////////////////////////////////////

	CXTPRibbonTab* pTab = nullptr;
	CXTPRibbonGroup* pGroup = nullptr;
	CXTPControl* tmp_ctrl = nullptr;

	for (size_t i = 0; i < ribbon_ui_config.m_all_tabs.size(); ++i)
	{
		const ribbon_bar_data_tab& tabData = ribbon_ui_config.m_all_tabs[i];

		pTab = AddTab(tabData.m_tab_caption);
		if (!pTab) continue;
		
		// ���ŵ�ǰѡ��µ����пؼ�ͼ�꼰ͼ������
		int nControlMaxIconID = tabData.GetMaxIconIndex(); // ͳ�Ƶ�ǰѡ�������ͼ������ֵ
		if (nControlMaxIconID >= 0)
		{
			// ��ʼ��ͼ��������ֵ
			// xml�е�������0��ʼ����xtp�д���RibbonʱҪ��ͼ������������ID��ͬ���������Զ�ƥ�䣬����
			// �˴�Ӧ�ý����г�ʼֵ��ΪIDS_RIBBON_UI_INVALID��ʾ�޷���ȡͼ����������Ϊ�����ڲ��Զ�����
			// ������ID������Ϊ��ֵ��ͼ��������Ϊ���ֵ���Ա�֤�����޷����ʵ�����֮xtpҪ��ͼ������Ϊ
			// UINT�����ݣ��޷������������Բ���������
			UINT* pUINTArray = new UINT[nControlMaxIconID + 1]; // ������������һ��
			for (int iconIndex = 0; iconIndex <= nControlMaxIconID; ++iconIndex)
				pUINTArray[iconIndex] = IDS_RIBBON_UI_INVALID; // ȫ������Ч������

			// ��ʹ�õ���ͼ������������Ӧ��Ԫ��λ��
			vector<ribbon_control_id_and_icon_id> vecID = tabData.get_control_and_icon_id();
			for (size_t szIconIndex = 0; szIconIndex < vecID.size(); ++szIconIndex)
			{
				// ��Чͼ�����������������IDһ�²��ܱ�XTP��ƥ����ȷ
				if (vecID[szIconIndex].icon_id <= nControlMaxIconID && vecID[szIconIndex].icon_id >= 0)
					pUINTArray[vecID[szIconIndex].icon_id] = vecID[szIconIndex].id;
			}
			
			CString strPath = file_tools::get_app_path_with_xg() + _T("gui_xml\\") + tabData.m_icon_file_name;

			CImage img;
			// if (S_OK != img.Load(strPath)) continue; // ͼ����ز��ɹ�������������ѭ�������ý������������ͼ�����ʽ����
			if (S_OK == img.Load(strPath))
			{
				HBITMAP hBitmap = img.Detach();
				if (NULL != hBitmap)
				{
					CBitmap bitmap;
					bitmap.Attach(hBitmap);

					// ͼ��Ԫ�ظ������������һ����������������ͼ��
					GetImageManager()->SetIcons(bitmap, pUINTArray, nControlMaxIconID + 1,
						CSize(32, 32), xtpImageNormal, (tabData.m_image_has_alpha ? TRUE : FALSE));
				}
			}

			safe_delete_array(pUINTArray);
		}

		// ����ѡ������ڲ����пؼ�
		pTab->SetKeyboardTip(tabData.m_tab_acc_key);
		if (false == tabData.m_context_name.IsEmpty())
		{
			pTab->SetContextTab(tabData.m_context_color, tabData.m_context_name);
			pTab->SetVisible(FALSE); // ������ʱ������
			
			// ֻ�������ı�ǩ�ᱻ�洢���̶����ǲ��ᱻ�洢��
			if (false == tabData.m_context_name.IsEmpty())
				m_context_tab_info_cache.push_back( make_tuple(pTab, tabData.m_active_view_class, tabData.m_context_auto_active) );
		}

		for (size_t j = 0; j < tabData.m_all_groups.size(); ++j)
		{
			const ribbon_bar_data_group& group = tabData.m_all_groups[j];

			pGroup = pTab->AddGroup(group.m_group_caption);
			pGroup->ShowOptionButton(FALSE);

			for (size_t k = 0; k < group.m_control_in_group.size(); ++k)
			{
				ribbon_bar_data_controlbase* pCtrlData = group.m_control_in_group[k];
				if (!pCtrlData) continue;

				if (xtpControlError == pCtrlData->get_ribbon_control_type()) continue;

				tmp_ctrl = pGroup->Add(pCtrlData->get_ribbon_control_type(), pCtrlData->m_id);
				if (!tmp_ctrl) continue;

				pCtrlData->init_ribbon_control(tmp_ctrl);
			}
		}
	}



	//////////////////////////////////////////////////////////////////////////

	 //�����������ұ߼���ͼ��
	{
		UINT nIDs[] =
		{
			IDS_RIBBON_OPTIONS_MINI_MODE,
			IDS_RIBBON_OPTIONS_THEMECHANGE,
			IDS_RIBBON_OPTIONS_ABOUT
		};

		GetImageManager()->SetIcons(IDS_RIBBON_UI_OTHERS, nIDs, _countof(nIDs), CSize(16, 16), xtpImageNormal);

		tmp_ctrl = GetControls()->Add(xtpControlButton, IDS_RIBBON_OPTIONS_MINI_MODE);
		tmp_ctrl->SetFlags(xtpFlagRightAlign);
		tmp_ctrl->SetIconId(IDS_RIBBON_OPTIONS_MINI_MODE);
		tmp_ctrl->SetTooltip(_T("��������С��"));
	
		tmp_ctrl = GetControls()->Add(xtpControlButtonPopup /* xtpControlPopup */, IDS_RIBBON_OPTIONS_THEMECHANGE);
		CXTPControlPopup* theme = (CXTPControlPopup*)tmp_ctrl;
		theme->SetFlags(xtpFlagRightAlign);
		CMenu theme_menu;
		theme_menu.LoadMenu(IDR_MENU_OPTIONS);
		theme->SetCommandBar(theme_menu.GetSubMenu(0));
		theme->SetIconId(IDS_RIBBON_OPTIONS_THEMECHANGE);
		//theme->SetCaption(_T("����"));
		theme->SetTooltip(_T("�л�����������"));
		
		tmp_ctrl = GetControls()->Add(xtpControlButton, IDS_RIBBON_OPTIONS_ABOUT);
		tmp_ctrl->SetFlags(xtpFlagRightAlign);
		tmp_ctrl->SetIconId(IDS_RIBBON_OPTIONS_ABOUT);
		//tmp_ctrl->SetCaption(_T("����"));
		tmp_ctrl->SetTooltip(_T("��������ʾ"));
	}

	// ���ٷ�����
	{
		const ribbon_quick_access_bar& temp_access_bar = ribbon_ui_config.m_quick_access;

		vector<unsigned int> button_ids;
		for (const auto& x : temp_access_bar.m_all_buttons)
			button_ids.push_back(x.m_id);

		// icon id must be sort
		std::sort(button_ids.begin(), button_ids.end());
		
		if (button_ids[0] != IDS_RIBBON_QUICK_ACCESS_MIN)
			button_ids.push_back(IDS_RIBBON_QUICK_ACCESS_MIN);

		std::sort(button_ids.begin(), button_ids.end());

		for (int i = 0; i < (int)button_ids.size() - 1; ++i)
		{
			if ((button_ids[i + 1] - button_ids[i]) > 1)
			{
				button_ids.push_back(button_ids[i] + 1);
				std::sort(button_ids.begin(), button_ids.end());
			}
		}

		CString strPath = file_tools::get_app_path_with_xg() + _T("gui_xml\\") + temp_access_bar.m_file_name;

		CImage img;
		if (S_OK == img.Load(strPath))
		{
			HBITMAP hBitmap = img.Detach();
			if (NULL != hBitmap)
			{
				CBitmap bitmap;
				bitmap.Attach(hBitmap);

				// ͼ��Ԫ�ظ������������һ����������������ͼ��
				GetImageManager()->SetIcons(bitmap, button_ids.data(), button_ids.size(),
					CSize(16, 16), xtpImageNormal, (temp_access_bar.m_image_has_alpha ? TRUE : FALSE));
			}
		}

		for (const auto& x : temp_access_bar.m_all_buttons)
		{
			tmp_ctrl = GetQuickAccessControls()->Add(xtpControlButton, x.m_id);
			tmp_ctrl->SetCaption(x.m_caption);
			tmp_ctrl->SetTooltip(x.m_tool_tip);
			tmp_ctrl->SetIconId(x.m_id);
			tmp_ctrl->SetDescription(_T(""));
			tmp_ctrl->SetCaption(x.m_caption);
			tmp_ctrl->SetHideFlag(xtpHideGeneric,x.m_is_hide);
		}

		GetQuickAccessControls()->CreateOriginalControls();
	}

	SetCloseable(FALSE);

	// Call it only if your Ribbon is support customization using Ribbon Customization page.
	// EnableCustomization(TRUE);

	EnableFrameTheme(TRUE);

	// ����������������ⷢ���仯ʱ��Ҫ����
	//SetGroupsCaptionVisible(FALSE);
	//EnableQuickAccessPosContextMenuItem(FALSE);
	//EnableQuickAccessCustomItem(FALSE);

	return true;
}

void app_ribbon_bar::initlize_ribbon_bar_file_menu(const ribbon_bar_config_data &ribbon_ui_config)
{
	CMenu file_menu;
	file_menu.CreateMenu();

	for (const auto &x : ribbon_ui_config.m_file_menu.m_menu_items)
	{
		if (_T("-") == x.m_caption)
			file_menu.AppendMenu(MF_SEPARATOR);
		else
			file_menu.AppendMenu(MF_STRING | MF_UNCHECKED, x.m_id, x.m_caption);
	}
	file_menu.AppendMenu(MF_STRING | MF_UNCHECKED | MF_MENUBARBREAK, ID_FILE_RECENTDOCUMENTS, _T("����򿪵��ļ�"));

	SetMenu(nullptr); // ���ϵͳ�Դ������˵�������еĻ���

	const ribbon_file_menu_bar& temp_file_menu_bar = ribbon_ui_config.m_file_menu;

	vector<unsigned int> button_ids;
	for (const auto& x : temp_file_menu_bar.m_menu_items)
	{
		if (_T("-") != x.m_caption)
			button_ids.push_back(x.m_id);
	}

	std::sort(button_ids.begin(), button_ids.end());

	if (button_ids[0] != IDS_RIBBON_FILE_MENU_MIN)
		button_ids.push_back(IDS_RIBBON_FILE_MENU_MIN);

	std::sort(button_ids.begin(), button_ids.end());

	for (int i = 0; i < (int)button_ids.size() - 1; ++i)
	{
		if ((button_ids[i + 1] - button_ids[i]) > 1)
		{
			button_ids.push_back(button_ids[i] + 1);
			std::sort(button_ids.begin(), button_ids.end());
		}
	}

	CString strPath = file_tools::get_app_path_with_xg() + _T("gui_xml\\") + temp_file_menu_bar.m_image_file_name;

	CImage img;
	if (S_OK == img.Load(strPath))
	{
		HBITMAP hBitmap = img.Detach();
		if (NULL != hBitmap)
		{
			CBitmap bitmap;
			bitmap.Attach(hBitmap);

			// ͼ��Ԫ�ظ������������һ����������������ͼ��
			GetImageManager()->SetIcons(bitmap, button_ids.data(), button_ids.size(),
				CSize(32, 32), xtpImageNormal, (temp_file_menu_bar.m_image_has_alpha ? TRUE : FALSE));
		}
	}

	CXTPControlPopup* pControlFile = AddSystemButton();
	pControlFile->SetCommandBar(&file_menu);
	pControlFile->SetIconId(IDR_MAINFRAME); // ���ͼ��ߴ粻��32*32����Ҫ����ʹ��GetImageManager()->SetIcons()����
	pControlFile->GetCommandBar()->SetIconSize(CSize(32, 32));
	pControlFile->SetCaption(_T("�ļ�"));
	pControlFile->SetKeyboardTip(_T("F"));
	pControlFile->SetTooltip(_T("���ִ���ļ�����"));
}

void app_ribbon_bar::active_context_tabs_by_window_class(
	const std::vector<CString>& all_active_view_class /*= std::vector<CString>(0)*/,
	bool is_selected /*= true*/)
{
	if (m_context_tab_info_cache.empty()) return;

	// all_active_view_classΪ�ձ�ʾ���������Ĳ˵�������

	CXTPRibbonTab* sel_tab = nullptr; // ��¼��һ������������Ĳ˵�

	for (size_t i = 0; i < m_context_tab_info_cache.size(); ++i)
	{
		const tuple<CXTPRibbonTab*, CString, bool>& item = m_context_tab_info_cache[i];
		if (nullptr == std::get<0>(item)) continue;

		// ��⵱ǰtab���������д�����
		vector<CString> all_names;
		string_tools::split_string(all_names, std::get<1>(item));

		if (all_names.empty() || all_active_view_class.empty())
		{
			// û�м������all_active_view_classΪ�ձ�ʾ���ᱻ����
			std::get<0>(item)->SetVisible(FALSE);
			continue;
		}

		// ����˱�ǩ�����˲��Զ���������ʾ
		if (false == std::get<2>(item))
		{
			app_ui_interface* app = get_app_ui_interface();
			if (app)
			{
				window_interface* wnd = app->get_current_working_space_windows();
				if (wnd) wnd->active_context_ribbon_tab();
			}

			// ���Զ�������Ϊ�����û��ֶ�������ܲ��ܰ��û����ø����ˣ��л����´��ں������Ӧ��tab���˴���
			// ���أ�����Ĵ���Ҳ����Ϊ��tab�봫��Ĵ����಻ƥ��������أ���˲����к���֢[Add by WYZ]
			// std::get<0>(item)->SetVisible(FALSE);
			continue;
		}

		// ����ʹ�ô���Ĵ������ڵ�ǰtab���������д������в����Ƿ���ڣ������򼤻���򲻼���
		bool bExist = false;

		for (auto& x : all_active_view_class)
		{
			if (x.IsEmpty()) continue; // Ϊ�պ���

			for (auto& y : all_names)
			{
				if (0 == x.CompareNoCase(y))
				{
					bExist = true;
					break;
				}
			}

			if (bExist) break;
		}

		if (bExist)
		{
			std::get<0>(item)->SetVisible(TRUE);
		
			// �Ѿ���Ҫѡ�еı�ǩ���ٸ�ֵ
			if (nullptr == sel_tab) sel_tab = std::get<0>(item);
		}
		else
		{
			std::get<0>(item)->SetVisible(FALSE);
		}
	}

	// �����һ��
	if (sel_tab && is_selected) SetCurSel(sel_tab->GetIndex());
}

void app_ribbon_bar::active_context_tabs_always_by_tab_name(const std::vector<CString>& all_active_tabs)
{
	//for (const CString& tab_name : all_active_tabs)
	//{
	//	// ֻ���������ı�ǩ
	//	bool need_show = false;
	//	for (tuple<CXTPRibbonTab*, CString, bool>& x : m_context_tab_info_cache)
	//	{
	//		CXTPRibbonTab* tab = std::get<0>(x);
	//		if (!tab) continue;

	//		if (0 == tab->GetCaption().CompareNoCase(tab_name))
	//		{
	//			tab->SetVisible(TRUE);
	//			need_show = true;
	//			break; // �����ǩ���Զ�Ӧһ�����ڣ�ֻҪ�ҵ��Ϳ�����
	//		}
	//	}

	//	if (!need_show) tab->SetVisible(FALSE); // ѭ�����������֪��Ҫ����		
	//}


	// ֻ���������ı�ǩ
	for (tuple<CXTPRibbonTab*, CString, bool>& x : m_context_tab_info_cache)
	{
		CXTPRibbonTab* tab = std::get<0>(x);
		if (!tab) continue;

		bool finded_tab = false;
		for (const CString& tab_name : all_active_tabs)
		{
			if (0 == tab->GetCaption().CompareNoCase(tab_name))
			{
				finded_tab = true;
				break;
			}
		}

		tab->SetVisible(finded_tab ? TRUE : FALSE);
	}
}

void app_ribbon_bar::select_tab(const CString& tab_name)
{
	for (int i = 0; i < GetTabCount(); ++i)
	{
		CXTPRibbonTab* tab = GetTab(i);
		if (!tab) return;
		
		// ��ǩ����ʾ���л������������ı�ǩ��˵���������ֲ�ͬ����������ı�ǩ����ͬ������ͬ����ֻ��ʾ��Ӧ�ķ�
		// �飬��ʱ�����ϲ��ظ������Ҫ�����������,����ʾ���������б�ǩ�������������������Ϊһ��
		if (FALSE == tab->IsVisible()) continue;

		if (0 == tab->GetCaption().CompareNoCase(tab_name))
		{
			SetCurSel(i);
			return;
		}
	}
}

CXTPControl* app_ribbon_bar::get_control(int id) const
{
	for (int i = 0; i < GetTabCount(); ++i)
	{
		CXTPRibbonTab* tab = GetTab(i);
		if (!tab) continue;

		CXTPRibbonGroups* gps = tab->GetGroups();
		if (!gps) continue;

		for (int j = 0; j < gps->GetCount(); ++j)
		{
			CXTPRibbonGroup* gp = gps->GetAt(j);
			if (!gp) continue;

			CXTPControl* ctrl = gp->FindControl(id);
			if (ctrl) return ctrl;
		}
	}

	return nullptr;
}

void app_ribbon_bar::OnTabChanged(CXTPRibbonTab* tab)
{
	CXTPRibbonBar::OnTabChanged(tab);

	CString param = tab->GetContextCaption() + _T("#") + tab->GetCaption();
	cmd_trigger_manager::fire(_T("app"), _T("ribbon_tab_select_changed"), param);
}


IMPLEMENT_SERIAL(ribbon_bar_data_controlbase, CObject, 1)

ribbon_bar_data_controlbase::ribbon_bar_data_controlbase()
	: CObject()
{
	// �ؼ�ID������Ribbon�в����ظ���������������ͬһ�����ϵĲ˵������������е�ID����ͬ
	m_id = 0;

	m_caption = _T("");
	m_style = xtpButtonIconAndCaptionBelow;
	
	// ͼ��ID��ÿһ��ѡ��µ�����ͼ��ID������ͬ�������ͬ��ͼ��һ����
	// ע�⣬Ĭ��ֵ���ܸ�Ϊ���ڵ���0��ֵ����Ϊ��Щ�ؼ���û��ͼ��ģ�����Ĭ��Ϊ0ʱ�Ὣͼ��
	// ����Ϊ0�Ŀؼ�ͼ������-1��ʾ��Ч
	m_icon_id = -1;
	
	m_tool_tip = _T("");
	m_acc_key = _T("");		// ����ѡ���
	m_command = _T("");

}

ribbon_bar_data_controlbase::ribbon_bar_data_controlbase( const ribbon_bar_data_controlbase& control )
{
	*this = control;
}

ribbon_bar_data_controlbase::~ribbon_bar_data_controlbase()
{

}

void ribbon_bar_data_controlbase::init_ribbon_control(CXTPControl* control)
{
	if (NULL == control || FALSE == control->IsKindOf(RUNTIME_CLASS(CXTPControl))) return;

	control->SetID(m_id);
	control->SetCaption(m_caption);
	control->SetStyle(m_style);
	control->SetIconId(m_id);				// xml�е�ͼ��ID�Ƿ����û��༭�ģ������ڲ����ջᱻת���ɿؼ�ID�ˣ����Դ˴�Ҫʹ�ÿؼ���ͼ��ID
	control->SetTooltip(m_caption);
	control->SetDescription(m_tool_tip);
	
	control->SetKeyboardTip(m_acc_key);
}

void ribbon_bar_data_controlbase::init_ribbon_control_data( tinyxml2::XMLElement* node )
{
	if (!node) return;

	m_caption = get_xml_node_attribute_str(node, _T("caption"));
	m_style = (XTPButtonStyle) get_xml_node_attribute_int(node, _T("style"));
	m_icon_id = get_xml_node_attribute_int(node, _T("icon_index"));
	m_tool_tip = get_xml_node_attribute_str(node, _T("tooltip"));
	m_acc_key = get_xml_node_attribute_str(node, _T("key"));
	
	CString command_and_parameter_text = get_xml_node_attribute_str(node, _T("cmd"));
	vector<CString> cmd_items;
	string_tools::split_string(cmd_items, command_and_parameter_text, _T("|"));
	cmd_items.resize(2);
	m_command = cmd_items.front();
	m_cmd_params = cmd_items.back();
}

ribbon_bar_data_controlbase* ribbon_bar_data_controlbase::operator=( const ribbon_bar_data_controlbase& control )
{
	m_id			= control.m_id;
	m_caption	= control.m_caption;
	m_style		= control.m_style;
	m_icon_id		= control.m_icon_id;
	m_tool_tip	= control.m_tool_tip;
	m_acc_key		= control.m_acc_key;
	m_command		= control.m_command;
	m_cmd_params	= control.m_cmd_params;

	return this;
}

ribbon_bar_data_controlbase* ribbon_bar_data_controlbase::clone_control_data() const
{
	// �Ӷ���ʹ��new������������󲢿������ݺ󷵻�������ָ�룬���ɵĶ�����ռ�ڴ����ⲿ���õ�
	// �ط������������Լ������Լ��Ķ���ʹ�ÿ��������������ݲ����ػ���ָ�룬��ò�Ҫ����
	// �����Ľӿ�

	ribbon_bar_data_controlbase* control = new ribbon_bar_data_controlbase();
	*control = *this;
	return control;
}

XTPControlType ribbon_bar_data_controlbase::get_ribbon_control_type() const
{
	return xtpControlButton;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_controlbase::get_control_and_icon_id() const
{
	// ����������������Լ��¼ӵĴ�ID������ؼ����ټ��ϻ���ļ���
	vector<ribbon_control_id_and_icon_id> vecID;

	ribbon_control_id_and_icon_id nID;
	nID.id = m_id;
	nID.icon_id = m_icon_id;

	vecID.push_back(nID);

	return vecID;
}

int ribbon_bar_data_controlbase::set_control_id( int start_id )
{
	m_id = start_id;
	++start_id;

	return start_id;
}

bool ribbon_bar_data_controlbase::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (m_id != id) return false;

	cmd_container_interface::parse_command(m_command, info[0], info[1]);
	info[2] = m_cmd_params;

	return true;
}

void ribbon_bar_data_controlbase::get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const
{
	info.clear();
	info.push_back(make_pair(m_id, m_command));
}

ribbon_bar_data_group::ribbon_bar_data_group()
{
	m_control_in_group.clear();

}

ribbon_bar_data_group::ribbon_bar_data_group( const ribbon_bar_data_group& group )
{
	*this = group;
}

ribbon_bar_data_group::~ribbon_bar_data_group()
{
	for (size_t i = 0; i < m_control_in_group.size(); ++i)
		delete m_control_in_group[i];

	m_control_in_group.clear();
}

ribbon_bar_data_group* ribbon_bar_data_group::operator=( const ribbon_bar_data_group& group )
{
	m_group_caption = group.m_group_caption;

	for (size_t i = 0; i < m_control_in_group.size(); ++i)
		delete m_control_in_group[i];

	m_control_in_group.clear();

	for (size_t i = 0; i < group.m_control_in_group.size(); ++i)
		m_control_in_group.push_back(group.m_control_in_group[i]->clone_control_data());

	return this;
}

void ribbon_bar_data_group::InitRibbonGroupData( tinyxml2::XMLElement* node )
{
	if (!node) return;

	m_group_caption = get_xml_node_attribute_str(node, _T("caption"));

	tinyxml2::XMLElement* control_node = node->FirstChildElement("control");
	m_control_in_group.clear();
	if (!control_node) return;

	do
	{
		CString controlType = get_xml_node_attribute_str(control_node, _T("type"));

		CObject* pObj = CRuntimeClass::CreateObject(controlType);
		if (pObj)
		{
			ribbon_bar_data_controlbase* pControl = dynamic_cast<ribbon_bar_data_controlbase*>(pObj);
			if (!pControl)
			{
				safe_delete(pObj); // ��ֹ�ڴ�д������֧�ֶ�̬����������ɹ����������Ҫɾ��
				continue; // ������ǿؼ��������������
			}

			pControl->init_ribbon_control_data(control_node);
			m_control_in_group.push_back(pControl);
		}

		control_node = control_node->NextSiblingElement("control");
	} while (control_node);
}

int ribbon_bar_data_group::GetMaxIconIndex() const
{
	int nMax = -1;
	vector<ribbon_control_id_and_icon_id> vecIconID;
	for (size_t i = 0; i < m_control_in_group.size(); ++i)
	{
		vector<ribbon_control_id_and_icon_id> vecIconID = m_control_in_group[i]->get_control_and_icon_id();

		for (size_t j = 0; j < vecIconID.size(); ++j)
			if (nMax < vecIconID[j].icon_id) nMax = vecIconID[j].icon_id;
	}

	return nMax;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_group::get_control_and_icon_id() const
{
	std::vector<ribbon_control_id_and_icon_id> vecID;
	for (size_t i = 0; i < m_control_in_group.size(); ++i)
	{
		std::vector<ribbon_control_id_and_icon_id> vecGPID = m_control_in_group[i]->get_control_and_icon_id();
		if (!vecGPID.empty()) vecID.insert(vecID.end(), vecGPID.begin(), vecGPID.end());
	}

	return vecID;

}

ribbon_bar_data_tab::ribbon_bar_data_tab()
{
	m_tab_caption = _T("");
	m_icon_file_name = _T("");
	m_image_has_alpha = false;
	m_context_name = _T("");
	m_context_color = xtpRibbonTabContextColorNone;
	m_active_view_class = _T("");
	m_context_auto_active = false;

	m_all_groups.clear();
}

ribbon_bar_data_tab::~ribbon_bar_data_tab()
{

}

void ribbon_bar_data_tab::InitRibbonTabData( tinyxml2::XMLElement* node )
{
	if (!node) return;

	m_tab_caption = get_xml_node_attribute_str(node, _T("caption"));
	m_tab_acc_key = get_xml_node_attribute_str(node, _T("key"));
	m_icon_file_name = get_xml_node_attribute_str(node, _T("icons_file"));
	m_image_has_alpha = get_xml_node_attribute_bool(node, _T("image_has_alpha"));
	m_context_name = get_xml_node_attribute_str(node, _T("context"));
	m_context_color = (XTPRibbonTabContextColor) get_xml_node_attribute_int(node, _T("context_color"));
	m_active_view_class = get_xml_node_attribute_str(node, _T("active_view_class"));
	m_context_auto_active = get_xml_node_attribute_bool(node, _T("context_auto_active"));

	m_all_groups.clear();

	tinyxml2::XMLElement* group_node = node->FirstChildElement("group");
	if (!group_node || group_node->NoChildren()) return;

	do
	{
		ribbon_bar_data_group groupData;
		groupData.InitRibbonGroupData(group_node);

		// �յķ���Ҳ����Ч�ģ�ԭ������
		m_all_groups.push_back(groupData);

		group_node = group_node->NextSiblingElement("group");
	} while (group_node);
}

int ribbon_bar_data_tab::GetMaxIconIndex() const
{
	int nMax = -1;

	for (size_t i = 0; i < m_all_groups.size(); ++i)
	{
		int nGPMax = m_all_groups[i].GetMaxIconIndex();
		if (nMax < nGPMax) nMax = nGPMax;
	}

	return nMax;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_tab::get_control_and_icon_id() const
{
	std::vector<ribbon_control_id_and_icon_id> vecID;
	for (size_t i = 0; i < m_all_groups.size(); ++i)
	{
		std::vector<ribbon_control_id_and_icon_id> vecGPID = m_all_groups[i].get_control_and_icon_id();
		if (!vecGPID.empty()) vecID.insert(vecID.end(), vecGPID.begin(), vecGPID.end());
	}

	return vecID;
}

ribbon_bar_config_data::ribbon_bar_config_data()
{
	m_all_tabs.clear();
}

ribbon_bar_config_data::~ribbon_bar_config_data()
{

}

void ribbon_bar_config_data::initlize_config_data( tinyxml2::XMLElement* node )
{
	if (!node) return;

	//��opton_info�ж�ȡ��Ϣ�������Ի���
	tinyxml2::XMLElement* options_node = node->FirstChildElement("options");
	if (options_node) m_dialog_info.initlize_config_data(options_node);

	m_quick_access.m_all_buttons.clear();
	tinyxml2::XMLElement* quick_bar_node = node->FirstChildElement("quick_access_bar");
	if (quick_bar_node) m_quick_access.initlize_config_data(quick_bar_node);

	m_file_menu.m_menu_items.clear();
	tinyxml2::XMLElement* file_menu_node = node->FirstChildElement("file_menu");
	if (file_menu_node) m_file_menu.initlize_config_data(file_menu_node);

	m_all_tabs.clear();
	tinyxml2::XMLElement* tab_node = node->FirstChildElement("tab");
	if (!tab_node || tab_node->NoChildren()) return;

	do 
	{
		ribbon_bar_data_tab tabData;
		tabData.InitRibbonTabData(tab_node);

		// ����tab���Ƿ��пؼ�����Ϊ��Ч��Tab
		m_all_tabs.push_back(tabData);

		tab_node = tab_node->NextSiblingElement("tab");
	} while (tab_node);

	int nID = IDS_RIBBON_UI_MIN;

	// ���пؼ�����֮�����������ڲ��Ŀؼ�IDֵ
	for (size_t i = 0; i < m_all_tabs.size(); ++i)
	{
		ribbon_bar_data_tab& tab = m_all_tabs[i];

		for (size_t iGroup = 0; iGroup < tab.m_all_groups.size(); ++iGroup)
		{
			ribbon_bar_data_group& group = tab.m_all_groups[iGroup];

			for (size_t iControl = 0; iControl < group.m_control_in_group.size(); ++iControl)
			{
				if (!group.m_control_in_group[iControl]) continue;

				nID = group.m_control_in_group[iControl]->set_control_id(nID);
				if (nID > IDS_RIBBON_UI_AUTO_MAX)
				{
					nID = IDS_RIBBON_UI_MIN;
					_ASSERT((_T("Ribbon�п��Զ����õ�����IDֻ������ IDS_RIBBON_UI_MIN �� IDS_RIBBON_UI_AUTO_MAX ��Χ��"), false));
				}
			}
		}
	}
}

bool ribbon_bar_config_data::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	for (const ribbon_bar_data_tab& tab : m_all_tabs)
		for (const ribbon_bar_data_group& group : tab.m_all_groups)
			for (ribbon_bar_data_controlbase* ctrl : group.m_control_in_group)
				if (ctrl && ctrl->get_command_and_parameter(id, info)) return true;

	return false;
}

int ribbon_bar_config_data::get_id(const CString& domain, const CString& cmd, const CString& param /*= _T("")*/, bool compare_param /*= false*/)
{
	if (cmd.IsEmpty()) return -1; // ֻ���������Ϊ�գ���������

	vector<pair<int, CString>> id_info;
	get_all_control_id_and_cmd(id_info);

	for (auto& x : id_info)
	{
		array<CString, 3> cmd_info;
		if (!get_command_and_parameter(x.first, cmd_info)) continue;

		if (0 != domain.CompareNoCase(cmd_info[0])) continue;
		if (0 != cmd.CompareNoCase(cmd_info[1])) continue;
		if (compare_param && 0 != param.CompareNoCase(cmd_info[2])) continue;

		return x.first;
	}

	return -1;
}

void ribbon_bar_config_data::get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const
{
	info.clear();
	for (const ribbon_bar_data_tab& tab : m_all_tabs)
	{
		for (const ribbon_bar_data_group& group : tab.m_all_groups)
		{
			for (ribbon_bar_data_controlbase* ctrl : group.m_control_in_group)
			{
				if (!ctrl) continue;
				vector<pair<int, CString>> current_info;
				ctrl->get_all_control_id_and_cmd(current_info);
				if (!current_info.empty()) info.insert(info.end(), current_info.begin(), current_info.end());
			}
		}
	}
}

void ribbon_bar_config_data::get_all_command_items_of_special_window_class_in_context_tab(std::vector<CString>& cmd_list, const CString& window_class)
{
	cmd_list.clear();
	if (window_class.IsEmpty()) return;

	for (const ribbon_bar_data_tab& tab : m_all_tabs)
	{
		if (tab.m_context_name.IsEmpty() || tab.m_active_view_class.IsEmpty()) continue;
		if (tab.m_active_view_class != window_class) continue; // �����������ִ�Сд

		for (const ribbon_bar_data_group& group : tab.m_all_groups)
		{
			for (ribbon_bar_data_controlbase* ctrl : group.m_control_in_group)
			{
				if (!ctrl) continue;

				vector<pair<int, CString>> info;
				ctrl->get_all_control_id_and_cmd(info);
				for (auto& x : info) cmd_list.push_back(x.second);
			}
		}
	}
}



IMPLEMENT_SERIAL(ribbon_bar_data_button, ribbon_bar_data_controlbase, 1)
ribbon_bar_data_button::ribbon_bar_data_button()
	: ribbon_bar_data_controlbase()
{

}

ribbon_bar_data_button::~ribbon_bar_data_button()
{

}

void ribbon_bar_data_button::init_ribbon_control(CXTPControl* control)
{
	ribbon_bar_data_controlbase::init_ribbon_control(control);

	// if (!control || FALSE == control->IsKindOf(CXTPControlButton)) return;
	
}

void ribbon_bar_data_button::init_ribbon_control_data(tinyxml2::XMLElement* node)
{
	ribbon_bar_data_controlbase::init_ribbon_control_data(node);
}

ribbon_bar_data_controlbase* ribbon_bar_data_button::clone_control_data() const
{
	ribbon_bar_data_button* pButton = new ribbon_bar_data_button();
	*pButton = *this; // ���������Ҳ������

	return pButton;
}

XTPControlType ribbon_bar_data_button::get_ribbon_control_type() const
{
	return xtpControlButton;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_button::get_control_and_icon_id() const
{
	return ribbon_bar_data_controlbase::get_control_and_icon_id();
}

int ribbon_bar_data_button::set_control_id( int start_id )
{
	return ribbon_bar_data_controlbase::set_control_id(start_id);
}

IMPLEMENT_SERIAL(ribbon_bar_data_check_box, ribbon_bar_data_controlbase, 1)

ribbon_bar_data_check_box::ribbon_bar_data_check_box()
	: ribbon_bar_data_controlbase()
{
	m_is_checked = false;

	// ͼ��ID��ÿһ��ѡ��µ�����ͼ��ID������ͬ�������ͬ��ͼ��һ����
	// ע�⣬Ĭ��ֵ���ܸ�Ϊ���ڵ���0��ֵ����Ϊ��Щ�ؼ���û��ͼ��ģ�����Ĭ��Ϊ0ʱ�Ὣͼ��
	// ����Ϊ0�Ŀؼ�ͼ������-1��ʾ��Ч
	m_icon_id = -1;

}

ribbon_bar_data_check_box::~ribbon_bar_data_check_box()
{

}

void ribbon_bar_data_check_box::init_ribbon_control( CXTPControl* control )
{
	ribbon_bar_data_controlbase::init_ribbon_control(control);

	if (NULL == control || FALSE == control->IsKindOf(RUNTIME_CLASS(CXTPControlCheckBox))) return;

	control->SetChecked((m_is_checked ? TRUE : FALSE));

}

XTPControlType ribbon_bar_data_check_box::get_ribbon_control_type() const
{
	return xtpControlCheckBox;
}

void ribbon_bar_data_check_box::init_ribbon_control_data(tinyxml2::XMLElement* node)
{
	ribbon_bar_data_controlbase::init_ribbon_control_data(node);

	// ��ѡ���ֹ����ͼ�꣬��Ҫ����ID��������Ĭ��ͼ������Ϊ-1����ʾ������ͼ��
	// ��ȡxml�ĵ�ʱ���ܻ���Ϊ�û��������û��߰�װ��xml��ȡ���߶�ȡ��0�������ǳ�ֵ��Ӱ�쵽
	// ��ѡ���ͼ�����������Ի��ദ��֮�󣬸�ѡ����������뽫ͼ�������ٴα��Ϊ-1
	m_icon_id = -1;

	// ���������������������һ��,�����ڳ�ʼ�����������֮��,��Ҫ����һ��ѡ��״̬
	if (!node) m_is_checked = false;

	m_is_checked = get_xml_node_attribute_bool(node, _T("checked"));
}

ribbon_bar_data_controlbase* ribbon_bar_data_check_box::clone_control_data() const
{
	ribbon_bar_data_check_box* control = new ribbon_bar_data_check_box();
	*control = *this;
	return control;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_check_box::get_control_and_icon_id() const
{
	return ribbon_bar_data_controlbase::get_control_and_icon_id();
}

int ribbon_bar_data_check_box::set_control_id( int start_id )
{
	return ribbon_bar_data_controlbase::set_control_id(start_id);
}

IMPLEMENT_SERIAL(ribbon_bar_data_combo, ribbon_bar_data_controlbase, 1)

ribbon_bar_data_combo::ribbon_bar_data_combo()
{
	m_all_item_list.clear();
	m_width = 60;
}

ribbon_bar_data_combo::~ribbon_bar_data_combo()
{

}

void ribbon_bar_data_combo::init_ribbon_control( CXTPControl* control )
{
	// ��ʼ�������һЩ��������ѡ��֧�����ñ������֣����Ի����caption������Ч
	ribbon_bar_data_controlbase::init_ribbon_control(control);

	if (NULL == control || FALSE == control->IsKindOf(RUNTIME_CLASS(CXTPControlComboBox))) return;

	CXTPControlComboBox* pComCtrl = (CXTPControlComboBox*)control;
	pComCtrl->ResetContent(); // ��������б���Ŀ
	pComCtrl->SetWidth(m_width);

	int nSelectedIndex = -1; // ��¼��һ��������ѡ�����Ե�����
	
	for (size_t i = 0; i < m_all_item_list.size(); ++i)
	{
		pComCtrl->AddString(m_all_item_list[i].text);
		
		if (-1 == nSelectedIndex && m_all_item_list[i].is_selected)
			nSelectedIndex = (int)i;
	}

	if (-1 == nSelectedIndex) nSelectedIndex = 0; // û���κ��ѡ����Ĭ��ѡ�е�0��
	
	pComCtrl->SetCurSel(nSelectedIndex);
}

XTPControlType ribbon_bar_data_combo::get_ribbon_control_type() const
{
	return xtpControlComboBox;
}

void ribbon_bar_data_combo::init_ribbon_control_data(tinyxml2::XMLElement* node)
{
	m_all_item_list.clear();

	ribbon_bar_data_controlbase::init_ribbon_control_data(node);
	if (!node) return;

	m_width = get_xml_node_attribute_int(node, _T("width"));
	if (m_width <= 0) m_width = 60; // xml��û��ָ����ʹ��Ĭ��ֵ

	tinyxml2::XMLElement* control_node = node->FirstChildElement("list_item");
	if (!control_node) return;

	bool sel_index = false; // �Ƿ���ѡ����

	do
	{
		list_item item;
		item.text = get_xml_node_attribute_str(control_node, _T("text"));
		item.is_selected = get_xml_node_attribute_bool(control_node, _T("selected"));

		// ��һ�γ��ֱ�ѡ�е���Ŀ
		if (!sel_index && item.is_selected)
			sel_index = true;
		else		
			item.is_selected = false; // �������ж��ͬʱ����ѡ�У�ǿ�Ƹ�Ϊ��ѡ��

		m_all_item_list.push_back(item);
		control_node = control_node->NextSiblingElement("list_item");
	} while (control_node);
}

ribbon_bar_data_controlbase* ribbon_bar_data_combo::clone_control_data() const
{
	ribbon_bar_data_combo* control = new ribbon_bar_data_combo();
	*control = *this;
	return control;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_combo::get_control_and_icon_id() const
{
	return ribbon_bar_data_controlbase::get_control_and_icon_id();
}

int ribbon_bar_data_combo::set_control_id( int start_id )
{
	return ribbon_bar_data_controlbase::set_control_id(start_id);
}

bool ribbon_bar_data_combo::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (!ribbon_bar_data_controlbase::get_command_and_parameter(id, info)) return false;

	// ��������������ؼ��в���xmlָ����������ǽ���ǰ��ѡ������ı���Ϊ����
	main_application* app = (main_application*) AfxGetApp();
	if (!app) return false;

	main_frame* main_wnd = dynamic_cast<main_frame*>(app->m_pMainWnd);
	if (!main_wnd) return false;

	app_ribbon_bar* rib = main_wnd->get_db_ribbon_bar();
	if (!rib || !rib->GetSafeHwnd()) return false;

	CXTPControlComboBox* combo = dynamic_cast<CXTPControlComboBox*>(rib->get_control(id));
	if (!combo) return false;

	info[2] = combo->GetText();
	return true;
}

IMPLEMENT_SERIAL(ribbon_bar_data_whole_popup_button, ribbon_bar_data_controlbase, 1)

ribbon_bar_data_whole_popup_button::ribbon_bar_data_whole_popup_button()
{
	m_all_item_list.clear();
}

ribbon_bar_data_whole_popup_button::~ribbon_bar_data_whole_popup_button()
{

}

void ribbon_bar_data_whole_popup_button::init_ribbon_control(CXTPControl* control)
{
	ribbon_bar_data_controlbase::init_ribbon_control(control);

	if (NULL == control || FALSE == control->IsKindOf(RUNTIME_CLASS(CXTPControlPopup))) return;

	CXTPControlPopup* pPopUpCtrl = (CXTPControlPopup*)control;

	CMenu popMenu;
	popMenu.CreateMenu();
	if (nullptr == popMenu.GetSafeHmenu()) return;
	if (m_all_item_list.empty()) return;

	for (size_t i = 0; i < m_all_item_list.size(); ++i)
		popMenu.AppendMenu(MF_STRING, m_all_item_list[i].id,
		m_all_item_list[i].text);

	// xtp����ڼӲ˵���ʱ���Զ����Ҷ�ӦID��ͼ�겢������ʾ�����Բ������ֶ��������ò˵���ͼ��Ĵ���
	pPopUpCtrl->SetCommandBar(&popMenu);
}

XTPControlType ribbon_bar_data_whole_popup_button::get_ribbon_control_type() const
{
	return xtpControlButtonPopup;
}

void ribbon_bar_data_whole_popup_button::init_ribbon_control_data(tinyxml2::XMLElement* node)
{
	ribbon_bar_data_controlbase::init_ribbon_control_data(node);

	if (!node) return;

	m_all_item_list.clear();

	tinyxml2::XMLElement* control_node = node->FirstChildElement("popup_item");
	if (!control_node) return;

	do
	{
		popup_list_item item;
		//item.nID = itemData->IntAttribute(L"id");
		item.text = get_xml_node_attribute_str(control_node, _T("text"));
		item.icon_id = get_xml_node_attribute_int(control_node, _T("icon_index"));

		CString command_and_parameter_text = get_xml_node_attribute_str(control_node, _T("cmd"));
		vector<CString> cmd_items;
		string_tools::split_string(cmd_items, command_and_parameter_text, _T("|"));
		cmd_items.resize(2);
		item.cmd = cmd_items.front();
		item.cmd_params = cmd_items.back();

		m_all_item_list.push_back(item);

		control_node = control_node->NextSiblingElement("popup_item");
	} while (control_node);
}

ribbon_bar_data_controlbase* ribbon_bar_data_whole_popup_button::clone_control_data() const
{
	ribbon_bar_data_whole_popup_button* control = new ribbon_bar_data_whole_popup_button();
	*control = *this;
	return control;
}

std::vector<ribbon_control_id_and_icon_id> ribbon_bar_data_whole_popup_button::get_control_and_icon_id() const
{
	// ����Ҳ��һ����Ч�����ť
	vector<ribbon_control_id_and_icon_id> vecBase = ribbon_bar_data_controlbase::get_control_and_icon_id();

	// �ټ��ϵ����˵�����Ŀ������������Ч����Ŀ���ָ������㣬��Ϊ����������ID��ע�ⲻ���������Ӳ˵���
	if (!m_all_item_list.empty())
	{
		for (size_t i = 0; i < m_all_item_list.size(); ++i)
		{
			ribbon_control_id_and_icon_id nID;

			nID.id = m_all_item_list[i].id;
			nID.icon_id = m_all_item_list[i].icon_id;

			vecBase.push_back(nID);
		}
	}

	return vecBase;
}

int ribbon_bar_data_whole_popup_button::set_control_id( int start_id )
{
	int nResult = ribbon_bar_data_controlbase::set_control_id(start_id);
	
	if (!m_all_item_list.empty())
	{
		for (size_t i = 0; i < m_all_item_list.size(); ++i)
		{
			if (nResult > IDS_RIBBON_UI_AUTO_MAX)
			{
				nResult = IDS_RIBBON_UI_MIN;
				_ASSERT((_T("Ribbon�п��Զ����õ�����IDֻ������ IDS_RIBBON_UI_MIN ��")
					_T(" IDS_RIBBON_UI_AUTO_MAX ��Χ��"), false));
			}

			m_all_item_list[i].id = nResult;
			++nResult;
		}
	}

	return nResult;
}

bool ribbon_bar_data_whole_popup_button::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (ribbon_bar_data_controlbase::get_command_and_parameter(id, info)) return true;

	for (const popup_list_item& item : m_all_item_list)
	{
		if (item.id != id) continue;

		cmd_container_interface::parse_command(item.cmd, info[0], info[1]);
		info[2] = item.cmd_params;

		return true;
	}

	return false;
}

void ribbon_bar_data_whole_popup_button::get_all_control_id_and_cmd(std::vector<std::pair<int, CString>>& info) const
{
	info.clear();
	ribbon_bar_data_controlbase::get_all_control_id_and_cmd(info);

	for (const popup_list_item& item : m_all_item_list)
		info.push_back(make_pair(item.id, item.cmd));
}


IMPLEMENT_SERIAL(ribbon_bar_data_split_popup_button, ribbon_bar_data_whole_popup_button, 1)

ribbon_bar_data_split_popup_button::ribbon_bar_data_split_popup_button()
{

}

ribbon_bar_data_split_popup_button::~ribbon_bar_data_split_popup_button()
{

}

XTPControlType ribbon_bar_data_split_popup_button::get_ribbon_control_type() const
{
	return xtpControlSplitButtonPopup;
}

ribbon_bar_data_controlbase* ribbon_bar_data_split_popup_button::clone_control_data() const
{
	ribbon_bar_data_split_popup_button* control = new ribbon_bar_data_split_popup_button();
	*control = *this;
	return control;
}

ribbon_bar_data_quick_access_button::ribbon_bar_data_quick_access_button()
{

}

ribbon_bar_data_quick_access_button::~ribbon_bar_data_quick_access_button()
{

}

void ribbon_bar_data_quick_access_button::initlize_config_data(tinyxml2::XMLElement* node) 
{
	if (!node) return;
	m_caption = get_xml_node_attribute_str(node,_T("caption"));
	m_tool_tip = get_xml_node_attribute_str(node, _T("tooltip"));
	m_icon_id = get_xml_node_attribute_int(node,_T("icon_index"));
	m_acc_key = get_xml_node_attribute_str(node,_T("key"));
	m_is_hide = get_xml_node_attribute_bool(node, _T("hide"));

	m_id = IDS_RIBBON_QUICK_ACCESS_MIN + m_icon_id;
	if (m_id > IDS_RIBBON_QUICK_ACCESS_MAX) m_id = IDS_RIBBON_QUICK_ACCESS_MIN;

	CString command_and_parameter_text = get_xml_node_attribute_str(node, _T("cmd"));
	vector<CString> cmd_items;
	string_tools::split_string(cmd_items, command_and_parameter_text, _T("|"));
	cmd_items.resize(2);
	m_cmd = cmd_items.front();
	m_cmd_params = cmd_items.back();
}

bool ribbon_bar_data_quick_access_button::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (m_id != id) return false;

	cmd_container_interface::parse_command(m_cmd, info[0], info[1]);
	info[2] = m_cmd_params;

	return true;

}

ribbon_bar_data_file_menu_item::ribbon_bar_data_file_menu_item()
{

}

ribbon_bar_data_file_menu_item::~ribbon_bar_data_file_menu_item()
{

}

void ribbon_bar_data_file_menu_item::initlize_config_data(tinyxml2::XMLElement* node) 
{
	if (!node) return;
	m_caption = get_xml_node_attribute_str(node, _T("caption"));
	if (_T("-") == m_caption)
	{
		m_acc_key		= _T("");
		m_cmd		= _T("");
		m_cmd_params	= _T("");
		m_id			= 0;
		m_icon_id		= 0;
	}
	else
	{
		m_icon_id = get_xml_node_attribute_int(node, _T("icon_index"));
		m_acc_key = get_xml_node_attribute_str(node, _T("key"));

		m_id = IDS_RIBBON_FILE_MENU_MIN + m_icon_id;
		if (m_id > IDS_RIBBON_FILE_MENU_MAX) m_id = IDS_RIBBON_FILE_MENU_MIN;

		CString command_and_parameter_text = get_xml_node_attribute_str(node, _T("cmd"));
		vector<CString> cmd_items;
		string_tools::split_string(cmd_items, command_and_parameter_text, _T("|"));
		cmd_items.resize(2);
		m_cmd = cmd_items.front();
		m_cmd_params = cmd_items.back();
	}
}

bool ribbon_bar_data_file_menu_item::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (m_id != id) return false;

	cmd_container_interface::parse_command(m_cmd, info[0], info[1]);
	info[2] = m_cmd_params;

	return true;
}

ribbon_quick_access_bar::ribbon_quick_access_bar()
{

}

ribbon_quick_access_bar::~ribbon_quick_access_bar()
{

}

void ribbon_quick_access_bar::initlize_config_data(tinyxml2::XMLElement* node)
{
	if (!node) return;
	m_file_name = get_xml_node_attribute_str(node, _T("icons_file"));
	m_image_has_alpha = node->BoolAttribute(_T("image_has_alpha"));

	tinyxml2::XMLElement* item_node = node->FirstChildElement(_T("item"));
	m_all_buttons.clear();
	do
	{
		if (!item_node) return;
		ribbon_bar_data_quick_access_button quick_access_data;
		quick_access_data.initlize_config_data(item_node);
		m_all_buttons.push_back(quick_access_data);

		item_node = item_node->NextSiblingElement("item");
	} while (item_node);
}

bool ribbon_quick_access_bar::has_this_id(UINT id) const
{
	if (m_all_buttons.empty()) return false;

	for (size_t i = 0; i < m_all_buttons.size(); ++i)
	{
		if (m_all_buttons[i].m_id == id) return true;
	}
	return false;
}

bool ribbon_quick_access_bar::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	if (!has_this_id(id)) return false;

	for (auto &x: m_all_buttons)
	{
		if (x.get_command_and_parameter(id, info))  return true;
	}

	return false;
}




ribbon_file_menu_bar::ribbon_file_menu_bar()
{

}

ribbon_file_menu_bar::~ribbon_file_menu_bar()
{

}

void ribbon_file_menu_bar::initlize_config_data(tinyxml2::XMLElement* node)
{
	if (!node) return;
	m_image_file_name = get_xml_node_attribute_str(node, _T("icons_file"));
	m_image_has_alpha = get_xml_node_attribute_bool(node, _T("image_has_alpha"));

	tinyxml2::XMLElement* item_node = node->FirstChildElement(_T("item"));
	m_menu_items.clear();
	do
	{
		if (!item_node) return;
		ribbon_bar_data_file_menu_item file_menu_data;
		file_menu_data.initlize_config_data(item_node);
		m_menu_items.push_back(file_menu_data);

		item_node = item_node->NextSiblingElement("item");
	} while (item_node);
}

bool ribbon_file_menu_bar::get_command_and_parameter(int id, std::array<CString, 3>& info) const
{
	info[0] = _T("");
	info[1] = _T("");
	info[2] = _T("");

	auto it = std::find_if(m_menu_items.begin(), m_menu_items.end(), [&](const ribbon_bar_data_file_menu_item& item)
	{
		return id == item.m_id;
	});

	if (m_menu_items.end() == it) return false;

	for (auto &x : m_menu_items)
	{
		if (x.get_command_and_parameter(id, info))  return true;
	}

	return false;
}

option_info::option_info()
{

}

option_info::~option_info()
{

}

void option_info::initlize_config_data(tinyxml2::XMLElement* node)
{
	m_about_dialog_class_name = _T("");

	if (!node || node->NoChildren()) return;

	tinyxml2::XMLElement* dialog_about_node = node->FirstChildElement(_T("about_dialog"));
	if (dialog_about_node)
		m_about_dialog_class_name = get_xml_node_attribute_str(dialog_about_node, _T("dialog_class_name"));
}

