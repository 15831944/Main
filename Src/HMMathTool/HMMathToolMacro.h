#pragma once
#ifdef   __HMMathTool__
#define  HM_MathTool_EXT   __declspec(dllexport)
#pragma message("�Ѿ�����__HM_MathTool__")
#else
#define  HM_MathTool_EXT   __declspec(dllimport)
#pragma message("û�ж���__HM_MathTool__")
#ifdef _DEBUG
#pragma comment(lib,"HMMathToolD.lib")
#pragma message("Automatically linking with HMMathToolD.dll")
#else
#pragma comment(lib,"HMMathTool.lib")
#pragma message("Automatically linking with HMMathTool.dll")
#endif
#endif
#define _HM_MathTool_BEGIN   namespace HM_MathTool{
#define _HM_MathTool_END    }
#ifdef _DEBUG
#define _HM_MathToolTEST_EXT HM_MathTool_EXT
#else
#define _HM_MathToolTEST_EXT
#endif
