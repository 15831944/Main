#pragma once
#ifdef   __HMExportTool__
#define  HM_ExportTool_EXT   __declspec(dllexport)
#pragma message("�Ѿ�����__HMExportTool__")
#else
#define  HM_ExportTool_EXT   __declspec(dllimport)
#ifdef _DEBUG
#pragma message("û�ж���__HMExportTool__")
#pragma comment(lib,"HMExportToolD.lib")
#pragma message("Automatically linking with HMExportToolD.dll")
#else
#pragma comment(lib,"HMExportTool.lib")
#pragma message("Automatically linking with HMExportTool.dll")
#endif
#endif
#define _HM_ExportTool_BEGIN   namespace HM_ExportTool{
#define _HM_ExportTool_END    }
#ifdef _DEBUG
#define _HM_ExportToolTEST_EXT HM_ExportTool_EXT
#else
#define _HM_ExportToolTEST_EXT
#endif
