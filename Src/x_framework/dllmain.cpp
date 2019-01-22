// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#include "tools/table_templates.h"
#include "tools/auto_load_module.h"
#include "tools/file_tools.h"
#include "interface/cmd_container_interface.h"
#include "interface/data_center_interface.h"
#include "tendon_tools/tendon_grid_tools.h"
#include "private_detail/mysql_db_adapter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE main_framework_dll = { NULL, NULL };
HINSTANCE g_resource_handle = nullptr;

static mysql_db_adapter* mysql_adapter = nullptr;


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("x_frame.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(main_framework_dll, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(main_framework_dll);
		g_resource_handle = hInstance;

		// �����Զ���������ģ��ǰע�ᣬ����ȷ������ģ���л�����������Ĭ��ע���������ݿ�ʵ��
		mysql_adapter = new mysql_db_adapter();
		regist_database_adapter(mysql_adapter);

		table_templates::load_table_template_name_from_dir(file_tools::get_app_path_with_xg() + _T("gui_tables"));
		auto_load_module::load_model_by_xml();
		reigste_tendon_grid_template();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("x_frame.DLL Terminating!\n");

		// ����Ϊnullptr�����
		regist_database_adapter(nullptr);
		safe_delete(mysql_adapter);

		// ������������
		data_center::destroy_data();

		// �������е�ע������
		cmd_trigger_manager::remove_all_command();

		// Terminate the library before destructors are called
		AfxTermExtensionModule(main_framework_dll);
	}
	return 1;   // ok
}
