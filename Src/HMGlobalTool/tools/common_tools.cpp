#include "StdAfx.h"

#include "common_tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


bool app_is_debug_mode()
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

void x_do_ui_events()
{
	DWORD st = GetTickCount();
	MSG msg;
	do
	{
		DWORD en = GetTickCount();
		if (en - st > 3000) return; // ���ֻ��ռ��3�룬�����û��������Ϣʱ����ѧ���²���������Ϣʱ�����ö��ж�����ѭ��������

		if (FALSE == ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) break; // ��Ϣ������û����Ϣʱ����FALSE
		if (FALSE == ::GetMessage(&msg, nullptr, 0, 0)) // // �յ�WM_QUIT��Ϣʱ����FALSE
		{
			AfxPostQuitMessage(0);
			return;
		}

		// ����ʹ��AfxPumpMessageͶ����Ϣ��Ҫ�Լ��ֶ�ʹ��APIת������Ϊ����̨����ʹ��AfxPumpMessage�ѽ�����������
		// AfxPumpMessage() // �յ�WM_QUIT��Ϣʱ����FALSE

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	} while (true);
}
