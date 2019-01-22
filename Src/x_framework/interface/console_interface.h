#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_CONSOLE_INTERFACE_H__	
#define __HMGLOBALTOOL_INTERFACE_CONSOLE_INTERFACE_H__

#include <utility>

#include "../tools/mini_file_log.h"


enum  message_type
{
	MT_ERROR = 0,			// ����
	MT_WARNING = 1,			// ����
	MT_SERIOUS_WARNING = 2,	// ���ؾ���
	MT_INFO = 3,			// һ����Ϣ
	MT_DEBUG = 4			// ������Ϣ
};

enum  output_object
{
	OO_NONE = 0,			// ��������κ�ý��
	OO_UI_LOG = 1,			// ͬʱ��������洰�ں�Ĭ����־�ļ�
	OO_UI = 2,				// ����������洰��
	OO_LOG = 3				// �������ϵͳĬ����־�ļ�
};


// ��������Ӧ�ӿڣ��κζ���Ҫ�ܽ��յ������еĽ���������Ϣ�����ܺ������н���ʵʱ���������Ӵ˽ӿ�����
class X_FRAMEWORK_API console_session_interface
{
public:
	console_session_interface();
	virtual ~console_session_interface();

public:
	// �������̨��֮�����������лỰ����ֹ�����������û�г��лỰ���򲻻ᱻ֪ͨ��
	// �Ự����ʱ����ᱻ�ض���������ڣ���������������Ự�󱾶���ʧȥ�ỰȨ
	// �˽ӿ��ɿ�ܵ��ã����ӵ�лỰ�Ķ�����Ҫ�����ͷŻỰȨ���Ե���end_console_session()���
	virtual void session_be_finished() = 0;

	// �������̨�򱾶���������������0��ʾ����ESC����1��ʾ���˻س�����2��ʾ���˿ո��
	// �������е�����Ҫ�������Լ�ȡ���������������е���Ϣ�Ƶ�����б�����Ҳ�ɱ��������
	virtual void on_console_action(int action_type) = 0;

	// �Ƿ������������̨�Ľ���ִ�й����У��˽ӿں���Ҫ������ȷʵ�֣����������������̨��Ϊ
	// ע�⣬�˽ӿڲ����Ƿ����Ƿ��лỰ��ϵ�����Ǳ�ʾ���лỰ������¿��ܻ���������ж���ִ����Ӧ�Ľ������ܣ���
	// һ����������û�н���ʱ����true�����򷵻�false���������������ڻ���һ���ߵĹ����У��û�ֻ��������㣬����
	// û�������յ�ʱ���������Ǵ�������ִ�й�����
	virtual bool is_doing_command() = 0;

public:
	// ����ת���ӿ� ������ǰ������ַ�����ת�����������̨����ʵ����������ͬ�������Ч��
	// һ������ת��key_char�¼�����������ת��key_down�¼���key_down�¼��ɱ��������������д����������Ҫ����
	// ֻ�б�������лỰȨʱ����Ч
	void notify_console_char_input(UINT char_value, UINT repeat_count, UINT flags);

	// �����������н����Ự��ֻ���������Ự֮���������̨�ŻὫʵ�ֽ�������Ϣ����������
	// ��ע�⣬�ڻỰ�����У��������������Ҳ����Ự���򱾶�����յ��Ự����ֹ����Ϣ���һỰת����������Ķ�����
	// ����������Ѿ��лỰȨ�����ٴ����󽫺������󣬱��ֻỰ��ϵ����
	// ����������ʱ���Զ������Ự
	void request_console_session();

	// �����Ự��ֻ�б�����ӵ�лỰȨ�˽ӿڲ���Ч��������Ч�������Ựʱ������ñ��ӿڵ�session_be_finished����
	void end_console_session();
};


// �����нӿڣ�ͬʱ֧��Ҳ֧����Ϣ���
class X_FRAMEWORK_API console_interface
{
public:
	console_interface();
	console_interface(const console_interface&) = delete;
	console_interface(console_interface&&) = delete;
	console_interface& operator=(const console_interface&) = delete;
	virtual ~console_interface();

public:
	// ��ȡ��ǰ���������룬����ǰ��������
	virtual CString get_current_input() const;

	// ��յ�ǰ�������������ǰ�������������
	virtual void clear_current_input();

	// ���������ݣ��˽ӿ�����������ʵ�֣�һ��������ս�����ʾ�����������಻Ҫ�ڴ˽ӿ�������ڲ��������
	virtual void clear_all_output();

	// �����������ʾ������ϴ���ʾ��������Ϣ
	// new_lineΪtrue���Ƚ��༭���е���Ч�����ϵ��б�Ȼ�󽫴�������д��༭�򣬷���ֱ�ӽ���������׷�ӵ��༭������ĩβ
	// end_lineΪtrue��ʾ������Ӻ��Զ�ˢ���б�Ϊfalse��ʾ�¼ӵ��������ڱ༭����,ֻҪend_line��Ϊtrue����;�����������Ϣ�������������ǰ׺
	// ���������ϢpromptΪ�գ����ʾ�������б༭���е�ǰ�����Լ�����ˢ���б��У�����յ�ǰ�༭�����ݲ�����ʾϵͳĬ��ǰ����
	virtual void add_prompt(const CString& prompt, bool new_line = true, bool end_line = false);

	// �ڱ༭�������ĩβ�����µ��������������ӵ����ݲ��ᵱ����ʾ��������
	// ��ӵ����ݲ��ᵼ�±༭���е�����ˢ���б���
	virtual void add_input(const CString& str);

public:
	// �������̨�������¶���ʱ���ã�0��ʾ����ESC����1��ʾ���˻س�����2��ʾ���˿ո��
	void notify_console_action(int action_type);

	// ����Ҫ��ס�����һ���û�����������ִ�е�����
	void set_last_command(const CString& domain, const CString& command);
	void get_last_command(CString& domain, CString& command);
	void clear_last_command(); // ������������Ϊ���ַ���

	// ����Ĭ�ϵ�����ǰ����ʾ������û��������ִ��ʱ�����л��Զ���ʾǰ����
	void set_default_prompt(const CString& prompt);
	CString get_default_prompt() const;

	// ���������ڵĵľ��桢���������ͳ����Ϣ����յĲ������ڣ�
	void print_message_count_report();

	// ͳ�ƴ����һ�μ�����Ϊ0�Ժ�����д�����������̨���ܴ�����������������ڵĴ�������������ڵ���ղ�������û��Ӱ�죩
	long get_total_error_count() const;
	
	// �ô�������Ϊ0
	void reset_total_error_count();

	// �������������־�ļ���
	void save_text_to_log_file(const CString& text);

	// ����������־�ļ������λ�ã��˲����������ر���־�ļ����ٴ���λ�ô��������־�����ṩ�Ѵ��ڵ���ȷ·�����ļ����Բ����ڣ�
	void reset_log_file_path_name(const CString& path_name);
	
	// ����־����Ϊ���Ĭ�ϵ�·�������ƣ�Ĭ��Ϊbin\\main_application_log.txt��
	void reset_log_file_default_file_path_name();

	// ��Ϣ��������࣬���ֱ༭���е����ݲ��䣬ֱ�ӽ��������뵽�б�ĩβ
	// ֧��ֱ�����CString���ַ����飬֧�ִ���ʽ�������ʹ��ʾ����
	// db_output::write(_T("output text"));
	// db_output::write(_("output a integer : %d, a double %lf"), 123, 456.789);
	// CString info = _T("text in CString object"); db_output::write(info);
	// �ڲ���ͨ��print_info_implement��ʵ�֣�������ʵ��print_info_implement����
	void print_info(const TCHAR* format, ...); // �� MT_INFO ��������� OO_UI_LOG
	void print_info(message_type type, const TCHAR* format, ...); // ��ָ��������� OO_UI_LOG
	void print_info(output_object out, message_type type, const TCHAR* format, ...); // ��ָ�����������ָ��Ŀ��

protected:
	// ����ת���ӿ� ���������������̨�����Ķ����key_char����ת����������̨,һ������ʵ�ָö���ļ��������Զ�ͬ��������̨
	// ֻ�г��лỰȨ�Ķ���Ż��Զ��������ض��򵽿���̨������Ҫ�лỰȨ��Ҫ��console_session_interface��������������ӿ�
	virtual void on_char_input(UINT char_value, UINT repeat_count, UINT flags) = 0;
	
	// �˽ӿ�Ϊ������Ϣ��ʵ�ֽӿڣ�������ʵ��ʱ�벻Ҫ��Ϊpublic����Ȩ�ޣ���Ϊ���������ӿ���print_info
	// ������ʵ��ʱ�����Ҫ���û���Ĵ˺����������������ͳ�Ʋ���ȷ��������ʵ�ֽ�ͳ�ƴ��󡢾�������
	virtual void print_info_implement(output_object out, message_type type, const CString& text) = 0;

private:
	// ��̨���ܴ�����������������ڵĴ�������ͳ�ƴ����һ�μ�����Ϊ0�Ժ�����д���������������ڵ���ղ�������û��Ӱ�죩
	long	m_total_error_count;

	// ͳ�����һ������֮��������Ϣ
	long	m_error_count;
	long	m_warning_count;
	long	m_serious_warning_count;

	CString m_default_prompt;
	std::pair<CString, CString> m_last_command;

	// ��ǰ�Ự���󣬴˶�����ڴ����ⲿ����
	console_session_interface* m_current_session_obj;

	// ������־������ļ�
	mini_file_log m_log_file;

private:
	friend class console_session_interface;
};


//////////////////////////////////////////////////////////////////////////


// ��ȡ������������ʵ��(ʵ��������ڴ����ⲿ�����ӿ��ڲ�����)
X_FRAMEWORK_API console_interface* get_console_interface();
X_FRAMEWORK_API void set_console_interface(console_interface* console_obj);

// ֱ���� MT_INFO �����������־��û�н���Ҳ������������
X_FRAMEWORK_API void app_print_info(const TCHAR* format, ...);

// ��ָ������ֱ���������־��û�н���Ҳ������������
X_FRAMEWORK_API void app_print_info(message_type type, const TCHAR* format, ...);

// ��ָ������ֱ���������־��û�н���Ҳ������������
X_FRAMEWORK_API void app_print_info(output_object out, message_type type, const TCHAR* format, ...);
#endif //__HMGLOBALTOOL_INTERFACE_CONSOLE_INTERFACE_H__
