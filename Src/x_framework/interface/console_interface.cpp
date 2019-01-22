#include "stdafx.h"

#include <memory>

#include "../tools/file_tools.h"
#include "../tools/string_tools.h"

#include "app_ui_interface.h"
#include "cmd_container_interface.h"

#include "console_interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;



console_session_interface::console_session_interface()
{
}

console_session_interface::~console_session_interface()
{
	end_console_session();
}

void console_session_interface::session_be_finished()
{
}

void console_session_interface::on_console_action(int action_type)
{
}

void console_session_interface::notify_console_char_input(UINT char_value, UINT repeat_count, UINT flags)
{
	console_interface* con = get_console_interface();
	if (!con) return;

	if (VK_ESCAPE == char_value)
	{
		con->notify_console_action(0);
		return;
	}
	else if (VK_RETURN == char_value || VK_SPACE == char_value)
	{
		// ��������ִ�й����У�ֱ��ת��������������������
		if (is_doing_command())
		{
			con->notify_console_action(VK_RETURN == char_value ? 1 : 2);
		}
		else
		{
			CString input_text = con->get_current_input();
			if (!input_text.IsEmpty())
			{
				CString domain, cmd;
				cmd_container_interface::parse_and_fill_command(input_text, domain, cmd);
				if (cmd.IsEmpty()) return;
				cmd_trigger_manager::fire(domain, cmd);
			}
		}

		// ����ǿո���ո��ݲ���Ϊ���룬����̨֪ͨ������ֱ�ӷ��أ���������������벻�˿ո�
		return;
	}
	else
	{
		; // nothing
	}

	if (con->m_current_session_obj == this)
		con->on_char_input(char_value, repeat_count, flags);
}

void console_session_interface::request_console_session()
{
	console_interface* con = get_console_interface();
	if (!con) return;
	
	if (con->m_current_session_obj == this) return;

	if (con->m_current_session_obj)
		con->m_current_session_obj->session_be_finished();

	con->m_current_session_obj = this;
}

void console_session_interface::end_console_session()
{
	console_interface* con = get_console_interface();
	if (!con) return;
	if (con->m_current_session_obj != this) return; // û�лỰ��ϵʱ��Ȩ�����Ự

	con->m_current_session_obj = nullptr;
}


//////////////////////////////////////////////////////////////////////////


console_interface::console_interface()
{
	m_total_error_count = 0;
	m_error_count = 0;
	m_warning_count = 0;
	m_serious_warning_count = 0;
	m_default_prompt = _T("���");
	m_current_session_obj = nullptr;

	reset_log_file_default_file_path_name();
}

console_interface::~console_interface()
{

}

CString console_interface::get_current_input() const
{
	return _T("");
}

void console_interface::clear_current_input()
{

}

void console_interface::add_prompt(const CString& prompt, bool new_line /*= true*/, bool end_line/* = false*/)
{

}

void console_interface::add_input(const CString& str)
{

}

void console_interface::clear_all_output()
{
	// m_total_error_count = 0;��ֵ���ⲿ�������ô���ֻ��ʹ���ֶ����ýӿ����
	m_error_count = 0;
	m_warning_count = 0;
	m_serious_warning_count = 0;
}

void console_interface::notify_console_action(int action_type)
{
	bool will_do_command = false;
	if (nullptr == m_current_session_obj) will_do_command = true;
	if (m_current_session_obj && !m_current_session_obj->is_doing_command()) will_do_command = true;

	if (m_current_session_obj && (m_current_session_obj->is_doing_command() || 0 == action_type))
		m_current_session_obj->on_console_action(action_type);

	CString input_text = get_current_input();
	input_text.Trim();

	CString domain, command, param;

	// �������������Ͳ���֮����|�����������в���������ո񣬿ո����Ƕ�������
	vector<CString> cmd_and_param = string_tools::split_string(input_text, _T("|"));
	cmd_and_param.resize(2);
	param = cmd_and_param.back();

	cmd_container_interface::parse_and_fill_command(cmd_and_param.front(), domain, command);
	if (command.IsEmpty()) return;

	// ���������������Ŀǰ�Դ�������ִ�й����У��ǲ����ٴ�������ģ���������͸���������������ִ�й����д���
	if (will_do_command)
	{
		// 0��ʾ����ESC��
		if (input_text.IsEmpty() || 0 == action_type)
		{
			add_prompt(get_default_prompt(),false, true);
			return;
		}

		if (cmd_trigger_manager::has_any_object_can_do(domain, command))
			cmd_trigger_manager::fire(domain, command, param);
		else
			add_prompt(get_default_prompt() + _T("�Ƿ�����"), true, true);
	}
	else
	{
		// ����ǲ���͸���������ֱ�Ӵ�������������ԣ���Ϊ���������ǲ�����Ƕ�����е�
		if (cmd_trigger_manager::is_transparent_command(domain, command))
			cmd_trigger_manager::fire(domain, command, param);
		else if (!m_current_session_obj->is_doing_command())
			add_prompt(_T("��Ч����"), true, true); // ���������в�����ǰ��������Ự������Ʋ�����
		else
			; // ��ʱ����������ִ��Ҳ����͸�������ֻ�ܵ����ǳ��������ˣ����������������жϲ�����
	}
}

void console_interface::get_last_command(CString& domain, CString& command)
{
	domain = m_last_command.first;
	command = m_last_command.second;
}

void console_interface::clear_last_command()
{
	m_last_command.first = _T("");
	m_last_command.second = _T("");
}

void console_interface::set_default_prompt(const CString& prompt)
{
	m_default_prompt = prompt;
}

CString console_interface::get_default_prompt() const
{
	return m_default_prompt;
}

void console_interface::print_message_count_report()
{
	CString report_text;
	report_text.Format("�ܼƴ�������%ld�����ؾ�������%ld����������%ld .",
		m_error_count, m_serious_warning_count, m_warning_count);

	print_info_implement(OO_UI_LOG, MT_INFO, report_text);
}

void console_interface::save_text_to_log_file(const CString& text)
{
	m_log_file.output_text(text);
}

void console_interface::reset_log_file_path_name(const CString& text)
{
	m_log_file.set_file_path(text);
}

void console_interface::reset_log_file_default_file_path_name()
{
	// ����ʹ��app��·����Ҫʹ��x_framework��·������Ϊ�������п����ǵ�Ԫ���Ի�������������ʹ��app��·����
	// ��������������ļ���������������·���£���˸�Ϊ���ģ���·����ͨ��
	CString default_path_name = file_tools::get_x_framework_path() + _T("\\main_application_log.txt");
	m_log_file.set_file_path(default_path_name);
}

void console_interface::print_info(const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	print_info_implement(OO_UI_LOG, MT_INFO, result);
}

void console_interface::print_info(message_type type, const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	print_info_implement(OO_UI_LOG, type, result);
}

void console_interface::print_info(output_object out, message_type type, const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	print_info_implement(out, type, result);
}

void console_interface::print_info_implement(output_object out, message_type type, const CString& text)
{
	// ��Ҫֱ��++����ֹ���߳̾�̬
	if (MT_ERROR == type)
	{
		::InterlockedIncrement(&m_error_count);
		::InterlockedIncrement(&m_total_error_count);
	}
	else if (MT_WARNING == type)
	{
		::InterlockedIncrement(&m_warning_count);
	}
	else if (MT_SERIOUS_WARNING == type)
	{
		::InterlockedIncrement(&m_serious_warning_count);
	}
	else
	{
		// nothing;
	}

	// ���û���κ�һ�������Ŀ���̨����ע�ᵽ����У���ֱ���������־��ע����Ͱ����������Ϊ��Ҫ�������
	if (nullptr == get_console_interface() && OO_UI_LOG == out || OO_LOG == out)
		save_text_to_log_file(text);
}

long console_interface::get_total_error_count() const
{
	return m_total_error_count;
}

void console_interface::reset_total_error_count()
{
	m_total_error_count = 0;
}

//////////////////////////////////////////////////////////////////////////

static console_interface* g_console_obj = nullptr;


class null_base_console : public console_interface
{
	virtual void on_char_input(UINT char_value, UINT repeat_count, UINT flags) {}
	virtual void print_info_implement(output_object out, message_type type, const CString& text) {}
};

console_interface* get_console_interface()
{
	static auto base_console = make_unique<null_base_console>();

	// ����̨�����⣬����������ܵ�������������������Ϣ�������һ���н����������Ϊ��ȷ��ҵ����벻�������䲻ͬUI��
	// ����ȡ���ն�����ɱ����򵽴�������жϴ��룬���ⲿ��û��ע���������ʱ�Զ����ɻ���Ķ��󷵻�
	if (g_console_obj)
		return g_console_obj;
	else
		return base_console.get();
}

void set_console_interface(console_interface* console_obj)
{
	g_console_obj = console_obj;
}

//////////////////////////////////////////////////////////////////////////


void app_print_info(const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	app_print_info(OO_UI_LOG, MT_INFO, result);
}

void app_print_info(message_type type, const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	app_print_info(OO_UI_LOG, type, result);
}

void app_print_info(output_object out, message_type type, const TCHAR* format, ...)
{
	CString result;

	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	console_interface* cmd = get_console_interface();
	if (cmd)
	{
		cmd->print_info(out, type, result);
		return;
	}
}
