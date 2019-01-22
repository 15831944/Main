//////////////////////////////////////////////////////////////////////////
// �ļ��������ܴӿ�Դ��ĿeMule����ֲ����������΢�Ķ��Ա����ñ��������ڼ���ڴ����
// ʧ�ܼ������쳣���������ɵ����õ�ת���ļ�
// ��ģ�鹦�ܱȽ�С�ɣ���û��ʹ��google breakpad���Ƚ���ʽ����ҵ������ڶ���������
// ���ӵķ�ʽ�����һЩ�Ƚ�����ı������쳣�˳�����һ���ܼ�⵽��Ŀǰ���ֵ�ֻ��ջ���
// �޷�������׽��ջ���һ�����ɵݹ����ѭ�����£����������ڿ����׶λ�ʹ�ý׶κ����׾�
// �ܷ��֣���ʽ��Ʒ��̫���ܳ��֣�����׽Ҳû̫�����⣬�ؼ���Ҫ��׽�������쳣ֻ������һ��
// �쳣����Ľ��̲���ʵ�֣�������Ҫ����һЩ���������ȥʵ�ֲ�ֵ�ã��п�Դ�Ŀ��ã�������
// �������鷳����google breakpad����������ע�ͣ���
// ��Ŀ��վ http://www.emule-project.net
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <tchar.h>
#include <cassert>
#include <ctime>

#include "windows.h"
#include "dbghelp.h"
#include "rpc.h"

#include "mini_dump.h"


// �����ĳ�ʼ��������ǰ��
bool mini_dump_helper::m_need_popup_report_window = false;

mini_dump_helper::mini_dump_helper()
{
}

mini_dump_helper::~mini_dump_helper()
{
}

int memory_malloc_callback_handler(size_t sz)
{
	static bool has_report = false;

	// ��������ڽṹ���쳣����Ĺ��ܣ����Կ���ʹ��MFC�Ķ��󣬶�ջû��̫��Ӱ�죬����ڴ���ȫ�Ĺ⣬
	// ����ֶԻ��������޷����У�ջ��һ��ռ䶼���䲻���ˣ����������������Σ����ǵ��ڴ����ʧ�ܿ����ɳ���Ա
	// �Լ�������ʱ�����Ա����Ҫ����һ���㹻��Ŀռ��Ƿ�����ȷ���䣬���Գ�����������ֻҪ������Ϣ��Ϳ��ԣ���
	// Ҫǿ�ƽ������򣬺��������������������ڳ���Ա�Ƿ������ڴ����ķ��ص�ַ�Ƿ�Ϊnull��������Ӧ����
	if (!has_report && mini_dump_helper::m_need_popup_report_window)
	{
		// ���ڴ��Ѿ������ʱ�򣬹���CString�ֻᵼ���ڴ���䣬��ɱ������ݹ�������Ӵ˱�Ǵ���һ�£��������Format֮ǰ��
		has_report = true;
		CString str;
		str.Format(_T("���������ڴ����ʱʧ��(size: %u byte)"), sz);
		AfxMessageBox(str, MB_OK | MB_ICONWARNING | MB_APPLMODAL | MB_TOPMOST);
	}

	has_report = false;

	return 0;
}

// ��ֹ�����쳣������
bool disable_set_unhandled_exception_filter()
{
	HMODULE hKernel32 = ::LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return false;

	void* org_entry = (void*) ::GetProcAddress(hKernel32, _T("SetUnhandledExceptionFilter"));
	if (!org_entry) return false;

#ifdef _M_IX86

	// [WYZע] Code for x86:
	// 33 C0        xor   eax,eax
	// C2 04 00     ret   4
	unsigned char hook_api_code [] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };

#elif _M_X64

	// [WYZע] Code for x64
	// 33 C0        xor   eax,eax
	// C3           ret
	unsigned char hook_api_code [] = { 0x33, 0xC0, 0xC3 };

#else

#	error "�쳣����Ĵ���ֻ�������� x86 �� x64 ��CPU!"

#endif

	SIZE_T bytes_written = 0;
	BOOL is_ok = ::WriteProcessMemory(::GetCurrentProcess(), org_entry, hook_api_code, sizeof(hook_api_code), &bytes_written);
	return (TRUE == is_ok);
}

inline void get_app_path_with_xg(CString& path)
{
	path.Empty();

	TCHAR dump_file_path[MAX_PATH] = { _T('\0') };

	// ����0Ϊʧ�ܣ���ȷʱ��������ģ���������磺"D:\***\***\***.exe"
	::GetModuleFileName(NULL, dump_file_path, _countof(dump_file_path) - 1);
	TCHAR* pszFileName = _tcsrchr(dump_file_path, _T('\\'));
	if (pszFileName) pszFileName[1] = _T('\0'); // �����һ��'\\'��ض�(����\\)

	path = dump_file_path;
}

inline void generate_guid(CString& guid_text)
{
	GUID guid; // ����guid��Ϊ�ļ���
	if (S_OK == ::CoCreateGuid(&guid))
	{
		// GUID ��ʽ��66C78769-C996-488a-AA8F-3CB806782FE6������Ϊ36
		TCHAR guid_char_array[40] = { _T('\0') }; // �����Ѿ��쳣�����ٷ����ڴ澡���ٷ���
		_sntprintf_s(guid_char_array, _countof(guid_char_array) - 1,
			_T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"), // 0��ʾʹ��0���
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

		guid_text = guid_char_array;
	}
}

void mini_dump_helper::enable_dump(bool popup_report_window/* = false*/)
{
	m_need_popup_report_window = popup_report_window;

	// 1.��������δ�����쳣ʱ������ʱ������ת���ļ�
	::SetUnhandledExceptionFilter(exception_top_level_filter);
	disable_set_unhandled_exception_filter();

	// 2.�������ڴ����ʧ��ʱ������ʾ
	_set_new_mode(1);
	_set_new_handler(memory_malloc_callback_handler);
}

// ����dump�ļ���
void generate_dump_file_name(CString& strDump)
{
	CString path;
	CString guid;
	get_app_path_with_xg(path);
	generate_guid(guid);
	strDump = path + guid + _T(".dmp");
}

LONG mini_dump_helper::exception_top_level_filter(EXCEPTION_POINTERS* ex_point)
{
	CString dump_file_and_error_id;
	dump_file_and_error_id.Format(_T("\"%u"), (unsigned int)::GetLastError());

	if (ex_point)
	{
		CString file_name;
		generate_dump_file_name(file_name);

		// �����쳣�����ˣ�����ǰ�����쳣�����У�������쳣�����ٴα���׽��
		// HANDLE file_handle = ::CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING | OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE file_handle = ::CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// pExceptionInfo����Ϊ��MiniDumpWriteDumpҲ�������������ʾϵͳû���ڱ���ʱ��⵽���õ�ת����Ϣ
		if (file_handle != INVALID_HANDLE_VALUE)
		{
			_MINIDUMP_EXCEPTION_INFORMATION ex_info = { 0 };
			ex_info.ThreadId = ::GetCurrentThreadId();
			ex_info.ExceptionPointers = ex_point;
			ex_info.ClientPointers = NULL;

			// dbghelp.dll V5.1�����ϰ汾����MiniDumpWriteDump������XP����Ϊ6.1.xxx
			MINIDUMP_TYPE mdt = (MINIDUMP_TYPE) (MiniDumpWithPrivateReadWriteMemory
				| MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo
				| MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);
			BOOL dmp_result = ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), file_handle, mdt, &ex_info, nullptr, nullptr);
			if (!dmp_result)
			{
				file_name.Format(_T("dmp�ļ�����ʧ��(error id [%u])"), (unsigned int)::GetLastError());
			}
		}
		else
		{
			file_name.Format(_T("dmp�ļ�����ʧ��(error id [%u])"), (unsigned int)::GetLastError());
		}

		::CloseHandle(file_handle); // �����ļ��Ƿ�ɹ��������ر�

		dump_file_and_error_id += _T(" ") + file_name; // ��һ���ո���������ָ���,�����ĵط����˹������
		dump_file_and_error_id.AppendChar(_T('\"'));
	}

	if (m_need_popup_report_window)
	{
		CString file_name;
		get_app_path_with_xg(file_name);
		file_name += _T("error_report.exe");
		::ShellExecute(nullptr, _T("open"), file_name, dump_file_and_error_id, nullptr, SW_SHOW);
	}

	// ����ϵͳ��ʱ���ǵ�����������ʾ(win8/8.1ϵͳ�ڲ����ƸĶ����)���˴�ǿ�ƽ���ȷ�����治�����κγ����ڣ�����
	// �����Զ����Ե����ⳡ��ʹ�ã��˳�����Ϊ0��ʾ���������˳������Ա���ϵͳ����(win8/8.1֮ǰ��ϵͳ���Բ���Ҳ���ᱨ��)
	::TerminateProcess(::GetCurrentProcess(), 0);
	
	// ��Ҫ��Ϊ�˳������ܻᵼ��ѭ�����������Ƿ�����ȷ�ı�־���ɲ���ϵͳ�Լ�����
	// ��������Ѿ�ת�����ļ�����ô����EXCEPTION_EXECUTE_HANDLER��ʾ�쳣�Ѵ�������ϵͳ�����ٸ������������
	// Ϊ���������ԣ��ط���EXCEPTION_CONTINUE_SEARCH��ʾ���쳣ת��ϵͳ������������ϵͳ�ı��������ܣ��Զ�
	// ��ʾ���������Դ��ڣ����ܲ�������VSʵ����ɽӹܵ�����������⣬����������������
	return EXCEPTION_EXECUTE_HANDLER;
}
