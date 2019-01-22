// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#ifndef NO_WARN_MBCS_MFC_DEPRECATION
#define NO_WARN_MBCS_MFC_DEPRECATION
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#ifndef _AFX_NO_OLE_SUPPORT
#	include <afxole.h>         // MFC OLE classes
#	include <afxodlgs.h>       // MFC OLE dialog classes
#	include <afxdisp.h>		   // MFC �Զ�����
#	include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#	include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
#include <gdiplus.h>



//////////////////////////////////////////////////////////////////////////

//#ifndef safe_delete
//#	define safe_delete(p) do{delete(p); (p) = nullptr;}while(false)
//#endif
//
//#ifndef safe_delete_array
//#	define safe_delete_array(p) do{delete[] (p); (p) = nullptr;}while(false)
//#endif

#ifndef safe_delete
#    define safe_delete(p) (delete(p), (p) = nullptr)
#endif
#ifndef safe_delete_array
#    define safe_delete_array(p) (delete[](p), (p) = nullptr)
#endif
//////////////////////////////////////////////////////////////////////////

// ������ѧ���еĳ���
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// MSDN�Ϲ涨���û����Զ������Ϣ��Χ��ע���Ա��ο���
//   WM_USER through 0x7FFF		Integer messages for use by private window classes. 
//   0x7FFF תΪ10����Ϊ 32767
// ����Ϊϵͳ��Ӧ�ó�����
//   0 through WM_USER�C1		Messages reserved for use by the system. 
//   WM_APP through 0xBFFF		Messages available for use by applications. 
//   0xC000 through 0xFFFF		String messages for use by applications. 
//   Greater than 0xFFFF		Reserved by the system. 
//////////////////////////////////////////////////////////////////////////

// ���ú�ϵͳ���������ť���˵�����������ť�Ȼᴥ��WM_COMMAND��Ϣ��Ԫ�ص�id���ص�
// ע��x_framework\tools\app_popup_menu_helper.h�������Ĳ˵��������40000-49999

// ע����ЩidҪ�������ڵĲ˵���ribbon��id��Χ���ֿ���������Ϣ��Ӧ��ӳ���
#define ID_ACCELERATOR_CMD_MIN							10000	// ���ټ��������������С����ID
#define ID_ACCELERATOR_CMD_MAX							19499	// ���ټ������������������ID

// ribbon���
#define WM_RIBBON_UI_COMMAND							(WM_USER + 20003) // ��ϢΪ20003��ʼ��


//ribbon�п��ٷ�������ͼ���ID��Χ
#define IDS_RIBBON_QUICK_ACCESS_MIN						19500
#define IDS_RIBBON_QUICK_ACCESS_MAX						19699

//ribbon���ļ���ͼ���ID��Χ
#define IDS_RIBBON_FILE_MENU_MIN						19700
#define IDS_RIBBON_FILE_MENU_MAX						19899



// ���涨�������ID��Ҫ��������ֵ��Χ��
#define IDS_RIBBON_UI_MIN								20000	// Ribbon�������������С����ID
#define IDS_RIBBON_UI_AUTO_MAX							28999	// �����õ����ID��֮��Ŀ���IDΪ�ض��ģ�������xml�����õ��Զ�����Ŀؼ�ʹ�ã�
#define IDS_RIBBON_UI_MAX								29998	// Ribbon�����������������ID
#define IDS_RIBBON_UI_INVALID							29999	// Ribbon���Զ����õ�ID�з���Ϊ��ֵ��ʾ��Ч������治��ʾ����Ҫ���ڴ����������Чͼ��

//ribbon��Option������
#define IDS_RIBBON_OPTIONS_MIN							30000	 // �������������С����ID
#define IDS_RIBBON_OPTIONS_MINI_MODE					30001	 // ��������С��
#define IDS_RIBBON_OPTIONS_THEMECHANGE					30002	 // �����л�
#define IDS_RIBBON_OPTIONS_ABOUT						30003	 // ���ڶԻ���
#define IDS_RIBBON_OPTIONS_MAX							30099	 // �����������������ID

//////////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <limits>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <utility>
#include <array>
#include <memory>
#include <utility>


#include "XTToolkitPro.h"
#include "x_framework/macro.h"
#include "x_framework/tools/tinyxml2.h"
#include "x_framework/tools/xml_tools.h"
#include "x_framework/tools/string_tools.h"

#include "app_size_config_helper.h"

//////////////////////////////////////////////////////////////////////////

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


