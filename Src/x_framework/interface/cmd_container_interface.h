#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_CMD_CONTAINER_INTERFACE_H__	
#define __HMGLOBALTOOL_INTERFACE_CMD_CONTAINER_INTERFACE_H__

#include <utility>
#include <map>
#include <set>

#include "app_ui_interface.h"


// ʹ��post��ʽ�첽ִ��������ö���
class X_FRAMEWORK_API command_post_doing_function : public functor_by_message
{
public:
	command_post_doing_function();
	virtual ~command_post_doing_function();

public:
	CString m_domain;
	CString m_command;
	CString m_parameter;

private:
	virtual void call();
};


// �������һ������������ܴ���һϵ�����������
// һ���������Ҫ�ܱ����ʶ�𣬱�������ע��ʵ���������û����Լ�������ʵ�����ڹ���ʱ�Զ�ע�ᣬ����ʱ��ע�ᣬ��
// �ֳ������ʺ����ڽ��洰���������Զ�ע�ᣬ��ʱ���ڶ����ʵ�����û��Լ������ģ�ֻҪ�����ʵ���Ͳ���Ҫ�ֹ�ע�ᡣ
// ��һ������������ڷǽ����࣬������һ��������Ӧ����崰��û��ֱ�ӹ�������Ϣ������Ŀ����Ϣ������ʱ�û�û����
// ����Ҫȥ�Լ�����һ��������ע�ᣬ����Ҫ�Լ�����������󣬿����ÿ�ܵ�ע��ӿ��Զ�����һ��ʵ��ֱ������ע�Ტ
// �ɿ�ܽӹ��ڴ棬ʾ�����£�
// register_cmd_container_by_class_name(project_commands);
class X_FRAMEWORK_API cmd_container_interface
{
public:
	// �������ʱ����Զ�ע�ᵽ����������У����Զ����շ�ʽ��
	// ���������ע����ȷ���˲�����ֵ����������ڴ�й©
	// Ϊ�����ʹ�ó������ǣ�����Ϊ������ʱ����Ҫ����
	// �ǽ��������ע��˲����Ƿ���Ҫ��Ϊtrue
	cmd_container_interface(bool auto_gc = false);

	// ����ע���ʱ���Ƿ����Զ����ͣ�������з�ע��
	virtual ~cmd_container_interface();

public:
	// ����������Ӧ����ǰ����ִ�к�������Ҫ������·���򷵻�true,���򷵻�false����������enableΪfalse����Ӧֱ�ӷ���false
	// ����������ע��param����ȷ���壨���ʾ�ļ�·�������������壩
	// drive_mode���û򷵻ش������������ʽ��0��ʾ��������������1��ʾ�ɽ���ֱ�ӷ���һ�����ڽ�������ʱ�����Ե��������
	// ע���������벻֧�ֵ�����
	virtual bool fire(const CString& domain, const CString& cmd, const CString& param = _T(""), int drive_mode = 0);

	// ��������״̬��enableΪtrue��ʾ������ã�checkedΪtrue��ʾ����ѡ��״̬���������Ϊ��ť����Ч����is_transparentΪtrueΪ͸������������벻֧�ֵ�����
	// ʵ��Ӧ���У��ܶ�����������������Ҫ�ж������Ƿ���ã�������Ҫ����һЩ��const�ķ����ŵõ�����Ȼ���������Ǹ���֧�֣���˶�Ϊ��const��
	virtual void get_status(const CString& domain, const CString& cmd, bool& enable, bool& checked, bool& is_transparent);

	// �����������Ȥ�����������˵�����Ϊ�գ���ʾ�ܽ��յ�ȫ������
	// ע�⣬����˵��ȫ���������������ȫ����ֻ����Ϊ�ջ���Ϊapp��������ܽ��յ�������app.refresh�����Ӷ����Լ�
	// ����ľֲ������section.new�����Լ�ϵͳ�ڲ������sys.start_page��Ĭ���ǽ��ղ����ģ������ڴ˽ӿ���
	// ��ʽ��ָ�������յ������⣬����ͬһ��������������Ӧʱ˳����ע��˳Ӧ��������һЩ��������籣��ȣ���ܻ�
	// �ʵ�����ִ��˳�������������Ϊ����Ŀ���
	virtual void set_command_filter(std::vector<std::tuple<CString, CString, bool>>& cmds_can_do) const;
	
public:
	// ��ǰ�����Ƿ�֧��ָ��������
	bool is_command_supported(const CString& domain, const CString& cmd) const;

	// �ж�һ�������Ƿ���͸������(���ڳ�������ִ�й�����Ҳ������������������)��Ĭ��Ϊfalse
	// �õ�get_status�����Ϊ��const��
	bool is_transparent_command(const CString& domain, const CString& command);

public:
	// ��������õ���������app.open�õ���Ϊapp������Ϊopen�������Ϊ�գ��򷵻ؿ�
	// ע���͸����ǵ����0Ϊ��͸����1Ϊ͸����ȫдʾ��Ϊ��editor.display_dim.1��ʾ������Ϊ͸������
	static void parse_command(const CString& cmd_full_text, CString& domain, CString& cmd);
	static void parse_command(const CString& cmd_full_text, CString& domain, CString& cmd, bool& is_transparent);
	
	// ��������õ���������app.open�õ���Ϊapp������Ϊopen�������Ϊ�գ����Զ����Ϊ"app"
	static void parse_and_fill_command(const CString& cmd_full_text, CString& domain, CString& cmd);

	// ��������õ�����app.open�õ���Ϊapp�������Ϊ�գ��򷵻ؿ�
	static CString parse_command_domain(const CString& cmd_full_text);
	
	// ��������õ�����app.open�õ���Ϊapp�������Ϊ�գ����Զ����Ϊ"app"
	static CString parse_and_fill_command_domain(const CString& cmd_full_text);

	// �����������ȥ�����������app.open��open
	static CString parse_command_cmd(const CString& cmd_full_text);

	// �����������͸����ǣ�true��ʾ͸��
	static bool parse_transparent_flag(const CString& cmd_full_text);

	// �����������ϳ�һ��������ȫ����,�����Ϊ���԰���������ʱ������������֮��ĵ�ţ�����͸����־
	static CString merge_domain_and_cmd(const CString& domain, const CString& cmd);

	// �����������ϳ�һ��������ȫ����,�����Ϊ�գ����Զ����Ϊ"app"������͸����־
	static CString merge_and_fill_domain(const CString& domain, const CString& cmd);

	// �����������ϳ�һ��������ȫ����,�����Ϊ�գ����Զ����Ϊ"app"�����͸�����Ϊ�������"0"
	static CString merge_full_command(const CString& domain, const CString& cmd, bool is_transparent);

	// ����������м�鲢���ȱʧ����Ϊapp��������ز����д�����ַ������ַ���������͸����ǣ����������������
	static CString& fill_command_domain(CString& cmd);
};


//////////////////////////////////////////////////////////////////////////

// ���������
class X_FRAMEWORK_API cmd_trigger_manager
{
public:
	// ֱ��ִ��ָ����������������ָ���������ִ��ʧ�ܣ��򷵻�false�����ָ��������û������Ĭ�ϵ���Ϊ"app"
	// ���������Ӧ����Ϊ���ڣ����ж��������Ӧ��ͬ������ʱ������ͬ�����Ĵ��ڼ��ز�һ��·�������ݣ������ȴ�����ǰ����
	static bool fire(const CString& domain, const CString& command, const CString& param = _T(""));
	
	// Ͷ�ݵ����������Ϣ�������첽ִ�У�������;������ʼҳ����򿪴��ڣ��ұ����������ʼҳ���ھ���Ҫ�첽ִ�У�
	static void post_fire(const CString& domain, const CString& command, const CString& param = _T(""));

	// ע���������޸��������ͣ��ɹ�����true�����򷵻�false������Ѿ�ע������޸�auto_gc���Բ�����true
	// auto_gcΪtrue��ʾ����Ķ����������������ֹ����ڴ棬��ע���ģ��ж�ص�ʱ��ɾ��
	// auto_gcΪtrue�˹���Ҫ�����������Ƕ��Ϸ���ģ�Ϊfalse���޶�
	static bool register_command_object(cmd_container_interface* cmd_obj, bool auto_gc = false);

	// ȡ��ָ�������ע�ᣨ���ע��ʱָ���Զ����գ�����ڷ�ע��ʱ�����˶���
	// ����true��ʾ���Զ������ˣ�����false��ʾδ�Զ����ն���
	// ע�⣺�������˲�����Ԫ�ر�delete����û�б�������Ҳ���ע��������Ƴ�
	// ע�⣺���һ������ע��Ϊ�Զ������������ⲿ�ֹ������������ķ�ע�ᣬ����[[[��ֹ]]]������������
	//      �ڴ棬��delete�ȣ�����ע�����������
	static bool unregister_command_object(cmd_container_interface* cmd_obj);

	// ��������Ѿ�ע�������Զ�ע��Ķ����������
	static void remove_all_command();

	// ���������ܴ���ָ������Ķ���
	// transparent_typeΪ0��ʾ����ȫ������Ӧ�����������
	// transparent_typeΪ1��ʾ����ȫ����͸����ʽ��Ӧ�����������
	// transparent_typeΪ2��ʾ����ȫ��͸����ʽ��Ӧ�����������
	static std::vector<cmd_container_interface*> get_command_object(
		const CString& domain, const CString& command, int transparent_type = 0);

	// ��ǰ�����Ƿ��ж�����Դ������û���κζ�������Ӧ�򷵻�false,���򷵻�true
	static bool has_any_object_can_do(const CString& domain, const CString& command);

	// ���ش��������Ķ����е�״̬������ʱ����������˳��
	// 1.����������Ӧ���й����������ܴ����������������󼯺�A�������޹����������������󼯺�B
	// 2.�ڼ���A�в���enable��checked��Ϊtrue�Ķ���������򷵻���״̬��������к�̲���
	// 3.�ڼ���B�в���enable��checked��Ϊtrue�Ķ���������򷵻���״̬��������к�̲���
	// 4.�ڼ���A�в���enableΪtrue�Ķ���������򷵻���״̬��������к�̲���
	// 5.�ڼ���B�в���enableΪtrue�Ķ���������򷵻���״̬��������к�̲���
	// 6.�ڼ���A�в���checkedΪtrue�Ķ���������򷵻���״̬��������к�̲���
	// 7.�ڼ���B�в���checkedΪtrue�Ķ���������򷵻���״̬��������к�̲���
	// 8.ֱ����enableΪfalse��checkedΪfalse��������״̬
	// Ϊ��߳���Ч�ʣ��ڲ�����������Σ�����һ�ξͰ�6��״̬�Ķ����ҵ�����1�������ȼ���һ����
	static void get_command_state(const CString& domain, const CString& command,
		bool& enable, bool& checked, bool& is_transparent);

	// ����get_command_state�ж������Ƿ����
	static bool is_command_enable(const CString& domain, const CString& command);

	// ����get_command_state�ж������Ƿ����
	static bool is_command_checked(const CString& domain, const CString& command);

	// �ж�һ�������Ƿ���͸������(���ڳ�������ִ�й�����Ҳ������������������)
	static bool is_transparent_command(const CString& domain, const CString& command);

private:
	static std::map<cmd_container_interface*, bool> m_command_objects;

private:
	cmd_trigger_manager() = delete;
	~cmd_trigger_manager() = delete;
	cmd_trigger_manager(const cmd_trigger_manager&) = delete;
	cmd_trigger_manager(cmd_trigger_manager&&) = delete;
	cmd_trigger_manager& operator=(const cmd_trigger_manager) = delete;
};

// ���Զ����շ�ʽע�ᵽ����������У�ע��ɹ��᷵�ش���Ķ���ʧ�ܻ��Զ���������Ķ��󲢷���nullptr����Ҫ���ڸ��������ע���
X_FRAMEWORK_API cmd_container_interface* register_auto_gc_cmd_and_return_object(cmd_container_interface* obj);

// ���ڸ�������ע��һ���Զ����յ�ʵ��������������в�����ʵ������
// ���������ʵ�����û����ɵĲ���ʹ�ô˽ӿ�ע�ᣬ���ɿ�ܹ������Ӧ������ڹ���ʱ�Զ�ע�ᣬ����ʱ�Զ���ע��
#ifndef register_cmd_container_by_class_name
#define register_cmd_container_by_class_name(class_name) \
	register_auto_gc_cmd_and_return_object(new class_name()) // ��Ҫ��ֺţ����ⲿʹ�õĵط��ֹ��Ӹ�����ʹ��ϰ��
#endif


#endif //__HMGLOBALTOOL_INTERFACE_CMD_CONTAINER_INTERFACE_H__
