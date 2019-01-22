#pragma once
#ifndef __HMGLOBALTOOL_INTERFACE_DB_ADAPTER_INTERFACE_H__	
#define __HMGLOBALTOOL_INTERFACE_DB_ADAPTER_INTERFACE_H__	

#include <vector>
#include <utility>


// ���ݿ��һ���ֶ���Ϣ
class HM_GlobalTool_EXT db_field
{
public:
	// ����ת������
	// ��ֵ������boolת��ʱ����0Ϊtrue������Ϊtrue����������8λ������λ���ǣ��Ⱥ����������������ת�������Եͣ�ʹbool�жϸ����ϳ���
	// tp_integer֮���С����ת������int->long���ڲ�ֱ��ǿ��ת��
	// tp_real֮���С����ת������double->float���ڲ�ֱ��ǿ��ת��
	// tp_real��tp_integer֮���ת������double->long���ڲ�ֱ��ǿ��ת��
	// tp_string��tp_integer֮���ת��ʹ��_ttoll()��������Ҫ���и�λ�ض�
	// tp_string��tp_integer֮���ת��ʹ��_tcstold()��������Ҫ������Чλ�ضϣ�����ת��Ҳ֧��
	// tp_string��tp_real֮���ת��ʹ��_tcstold()��������Ҫ������Чλ�ضϣ�����ת��Ĭ�ϱ���12λС��
	// tp_buffer����ֵ����ת�����Ϊ0,���ַ���ת�����Ϊ���ַ�������boolת�����Ϊfalse������ת�����õ��յ�buffer
	// ע�⣺ʱ�ڡ����ڵĴ����ڲ�ͬ�����ݿ�����������������ʽ��С�仯���޷�ʹ��ͨ�õķ�ʽ������֮����SQL���ʱ
	//      Ҳд���ַ�����ʽ����where fd_test='2000-1-1'������˱��ӿڻὫʱ�䡢���ڰ��ַ�����ʽ���������һ
	//      �ӹ�������Ͽ�ܵ�string_tools.h�е�ʱ�����ת���ӿڿ�ʵ���������Ĵ�����
	enum class type
	{
		tp_integer,			// �����������ͣ���bool��int��char��unsigned long��
		tp_real,			// ���ָ���������float��double��long double
		tp_string,			// �ַ�������
		tp_buffer,			// �����洢�Ķ������ڴ��
	};

public:
	db_field(type tp = type::tp_integer);
	db_field(const db_field& src);
	db_field(db_field&& src);
	db_field& operator=(const db_field& src);
	db_field& operator=(db_field&& src);
	bool operator==(const db_field& src) const; // �ַ�������ʱ�Դ�С����
	bool operator!=(const db_field& src) const;
	~db_field();

public:
	type get_type() const;

	void get_value(bool& val) const;
	void get_value(char& val) const;
	void get_value(unsigned char& val) const;
	void get_value(wchar_t& val) const;
	void get_value(short& val) const;
	void get_value(unsigned short& val) const;
	void get_value(int& val) const;
	void get_value(unsigned int& val) const;
	void get_value(long& val) const;
	void get_value(unsigned long& val) const;
	void get_value(long long& val) const;
	void get_value(unsigned long long& val) const;
	void get_value(float& val) const;
	void get_value(double& val) const;
	void get_value(long double& val) const;
	void get_value(CString& val) const;
	
	// ����һ���ض����ȵ�buffer���ڴ�ռ����ɱ��������Ҫ�����������п���
	void get_value(std::pair<size_t, char*>& val) const;

	// �ṩһ������ֱֵ�ӷ��ؽ���İ汾���ڼ��û�����
	template<typename T>
	T get_value() const
	{
		T tmp;
		get_value(tmp);
		return tmp;
	}

	void set_value(bool val);
	void set_value(char val);
	void set_value(unsigned char val);
	void set_value(wchar_t val);
	void set_value(short val);
	void set_value(unsigned short val);
	void set_value(int val);
	void set_value(unsigned int val);
	void set_value(long val);
	void set_value(unsigned long val);
	void set_value(long long val);
	void set_value(unsigned long long val);
	void set_value(float val);
	void set_value(double val);
	void set_value(long double val);
	void set_value(const CString& val);
	
	// ����Ϊ�ض����ȵ�buffer
	// clone_bufferΪfalse��ʾֱ�ӽӹܴ�����ڴ�ռ�
	// Ϊtrue��ʾ���ӹܴ�����ڴ�ռ䣬����ԭ������һ�ݳ������ڲ�����
	// ��������Ϊ���ڴ�飬����make_pair(0, nullptr)
	void set_value(const std::pair<size_t, const char*>& val, bool clone_buffer = true);

private:
	void destory_value();

private:
	type m_value_type;
	void* m_value;
};


// ��ʾ���ݿ��еĵ�һ����¼����һ�����ݣ�
class HM_GlobalTool_EXT db_record
{
public:
	db_record();
	~db_record();

public:
	// �������ƻ�ȡ�ֶζ����ָ�룬�ֶ����Ʋ����ִ�Сд
	db_field* field(const CString& name);
	db_field* field(unsigned int index);

public:
	std::vector<std::pair<CString, db_field>> m_row; // firstΪ�ֶ�����secondΪ�ֶ�����
};


//////////////////////////////////////////////////////////////////////////


// ���ݿ�����Ľӿ��࣬���ڷ�װ��ͬ���ݿ�Կ�ܵ�Ӱ��
class HM_GlobalTool_EXT db_adapter_interface
{
public:
	db_adapter_interface();
	db_adapter_interface(const db_adapter_interface&) = delete;
	db_adapter_interface(db_adapter_interface&&) = delete;
	void operator=(const db_adapter_interface&) = delete;
	void operator=(db_adapter_interface&&) = delete;
	virtual ~db_adapter_interface();

public:
	// ִ��һ��select��䲢������û��ƥ��Ľ�������򷵻�true�����򷵻�false
	// �˽ӿھ��������û���¼��Ȩ�޲�ѯ����������Ϣ�Ĳ�ѯ����Ϊ������Ϣй©�ķ��գ��ڲ�ʵ�ֱ���ﵽָ���İ�ȫ
	// ������Ҫ���ڲ�ʵ��ʹ��get_query_result_count��ɣ���ϸҪ��μ���ע�͡�
	bool has_query_result(const CString& sql);

public:
	// �ж�һ��sql����Ƿ�Ϊ��ѯ��䣨��䲻���Կհ��ַ���ͷ��
	static bool is_select_sql(const CString& sql);

public:
	// ���Ե�ǰʱ���ܲ�����ȷ���ӵ����ݿ⣬�ɹ�����true�����򷵻�false
	// 1.�ӿ��ڲ����벶�������ݿ�����쳣�������ݿ��쳣���Բ���������ȷ���ⲿʹ����Ա����Ҫ�������ݿ�������쳣
	// 2.�˽ӿڱ���������̰߳�ȫ
	virtual bool test_connection() const = 0;

	// ִ��һ��select��䲢���ز�ѯ����ļ�¼����
	// �˽ӿھ��������û���¼��Ȩ�޲�ѯ����������Ϣ�Ĳ�ѯ����Ϊ������Ϣй©�ķ��գ��˽ӿڵ�������ʵ�ֱ�����������Ҫ��
	// 1.����ʹ���ܷ�������������Ϊ����Ĳ�ѯ��䣬�粻�÷���ƥ����û����룬��Ӧ��ֱ�Ӳ�ѯƥ��ļ�¼���Ƿ���ڵ���1
	// 2.���ݿ��е�������Ϣ�������Ĵ洢���˽ӿڰ�����Ҫ��Բ�ѯ��Ϣ���ܺ󰴼����������ɲ�ѯ����ٲ�ѯ��������������
	//   �Ƿ�ʹ�ð�ȫ������������涨��������������¾���ʹ�ð�ȫ���ӣ���Ҫ����֤�飩
	// 3.�˽ӿڱ����㹻��Ч����Ҫ���غͽ��մ��������ʵ����ݣ����ص�����ֻҪ���жϳ��Ƿ���ƥ�������򷵻�����Խ��Խ��
	// 4.����䲻�ý��ճ�select֮�����䣨������ɾ���ݵ���䣩������������Ƕ�׵��Ӿ�
	// 5.�ӿ��ڲ����к��������ݿ�����ӺͶϿ���������ʱ���ӡ��Ͽ�������ʹ�����Ӷ�������������Լ�������
	// 6.�ӿ��ڲ����벶�������ݿ�����쳣�������ݿ��쳣���Բ���������ȷ���ⲿʹ����Ա����Ҫ�������ݿ�������쳣
	// 7.�˽ӿڱ���������̰߳�ȫ
	// 8.Ϊ��������ȡЧ�ʺͱ�����д���뵼������Դ���Ķ��������������ʱ��ʹ��ֻ��Ȩ�޵����ӽ��в���
	// 9.�ڲ�ʵ����sql���ĸ�����Ӱ���һ��������û�ʹ�ô˽ӿ�ʱ�뾡��ʹ��count�ۺϺ������ܿ����볣���ѯ���
	//   ���ɽӿ��ڲ�����������Ϣ���ؽ��������Ч����ģ�����������Ӧ��ʹ�ã�
	//   select count(*) as fd_count from tb_test
	//   �����ǣ�
	//   select * as fd_count from tb_test
	//   ��Ȼ�õ�������һ��������ȫ�Ժʹ������������ȫ����һ������
	// 10.����ӿ�has_query_result�ڲ�ʵ�������ڴ˽ӿڣ�����has_query_result�İ�ȫ�Լ�����Ҫ��Ҳ��������
	virtual unsigned long long get_query_result_count(const CString& sql) = 0;

	// ��ѯָ���ļ�¼
	// ����ֶ�Ϊ���������ݻ��ļ��������������ڴ�鷵�أ����Ϊ���ַ��������ַ������ͷ��أ���ȡʱ��ע��
	// ע�⣺
	//   ������ص����ݹ����п��ܵ����ڴ����ʧ����ɳ����쳣��ֹ�������ҵ���������ʹ��һ���Է��ػ��ǲ��ַ��أ���
	//   ���Ƿ�ҳ���ز���
	// �������ʵ������������Ҫ��
	// 1.�˽ӿڽ�֧��select���������������Ƕ�׵��Ӿ䣬����֧������ɾ���ݵȷǲ�ѯ���ʵ����
	// 2.�ӿ��ڲ����к��������ݿ�����ӺͶϿ���������ʱ���ӡ��Ͽ�������ʹ�����Ӷ�������������Լ�������
	// 3.�ӿ��ڲ����벶�������ݿ�����쳣�������ݿ��쳣���Բ���������ȷ���ⲿʹ����Ա����Ҫ�������ݿ�������쳣
	// 4.�˽ӿڱ���������̰߳�ȫ
	// 5.Ϊ��������ȡЧ�ʺͱ�����д���뵼������Դ���Ķ��������������ʱ��ʹ��ֻ��Ȩ�޵����ӽ��в���
	// 6.��ѯʧ�ܱ��뱣֤�������Ϊ��
	virtual std::pair<bool, CString> get_query_result(std::vector<db_record>& result, const CString& sql) = 0;

	// �Կ�������ķ�ʽһ��������ִ����ָ����ȫ��sql��䣬�������ͺ͹��ܲ��ޣ�ֻҪ���ݿ�֧�ּ��ɣ��������������
	// ��ع���ִ�е�ȫ���Ķ��Բ���
	// ���ص�first��ʾִ���Ƿ���ȫ�ɹ����ɹ�ִ����ɷ���true�����򷵻�false
	// ���ص�second��ʾ����ʱ�Ĵ�����Ϣ������ɹ���Ϊ��
	// �˽ӿڵ�������ʵ�ֱ�����������Ҫ��
	// 1.�ӿ��ڲ����벶�������ݿ�����쳣�������ݿ��쳣���Բ���������ȷ���ⲿʹ����Ա����Ҫ�������ݿ�������쳣
	// 2.�˽ӿڱ���������̰߳�ȫ
	virtual std::pair<bool, CString> run_as_a_batch(std::vector<const CString>& all_sql) = 0;

	// �Կ�������ķ�ʽһ����ִ��ָ����ȫ��sql��䣬���������һ����ѯ���Ľ��
	// �˽ӿ�ǿ��Ҫ�����һ��������Ϊselect��䣬������ִ�У�����ִ�й����е��м���һ�ɲ����أ����������һ
	// ����ѯ���Ľ���������������ͺ͹��ܲ��ޣ�ֻҪ���ݿ�֧�ּ���
	// ִ�й�������������������ع���ִ�е�ȫ���Ķ��Բ���
	// ���ӿ�һ������õ��������Ҫ��֤����ѯ���������ѯ�Ĺ����в�����;�����Ự�����ݸĶ���Ӱ���������ݲ�ͬ
	// ������Ҫ��ʱ�����ʹ�ñ��ӿڣ�һ�����ϵͳ�����д�Ҫ��
	// ��������£��˽ӿڵ�������Ҫ����run_as_a_batchһ��
	virtual std::pair<bool, CString> get_batch_result(std::vector<const CString>& all_sql, std::vector<db_record>& result) = 0;

	// �Բ���������ķ�ʽִ��һ�������sql��䣬�������ͺ͹��ܲ��ޣ�ֻҪ���ݿ�֧�ּ���
	// ���ص�first��ʾִ���Ƿ���ȫ�ɹ����ɹ�ִ����ɷ���true�����򷵻�false
	// ���ص�second��ʾ����ʱ�Ĵ�����Ϣ������ɹ���Ϊ��
	// �˽ӿڵ�������ʵ�ֱ�����������Ҫ��
	// 1.�ӿ��ڲ����벶�������ݿ�����쳣�������ݿ��쳣���Բ���������ȷ���ⲿʹ����Ա����Ҫ�������ݿ�������쳣
	// 2.�˽ӿڱ���������̰߳�ȫ
	virtual std::pair<bool, CString> run_alone(const CString& all_sql) = 0;
};


// Ҫ�ÿ���������������ݿ⣬��Ҫʵ����x_database_interface��������ע�ᣬע��Ķ������ⲿ������ܲ�ά������
// �˲���Ҫ��ע�ᣬ���Դ�nullptr��ʾͣ���Ѿ�ע���ʵ������Ĭ�ϵ�ʵ���ӹܣ�Ĭ��ʵ��ʲôҲ������ֻ��֤����һ��ʵ�����ѣ�
// ע�⣺
//   �������ݿ���ԣ����������Ǽҳ��㷹��ԭ���ϴ˽ӿ�ҲҪ֧�ֲ��������������ǵ�ע�����ݲ���ʵ��һ��ֻ���ڳ������ʱ
//   ���Ѿ���ɣ�����һ�����ر�ǰ�����ᶯ̬��ʵ������һ����ڷ����ݿ�����ҵ�������˵��Ϊ���ԣ�Ϊ�򻯳����߼���
//   �˺����ڲ������������������ҵ��������ڳ�������ʱ����ȷע����ʵ�ʵ������;��Ҫ�޸ģ�����Ժ󲢷��������
//   ǿ���ٿ���֧�֣��˴�����ע���Լ�֮��
HM_GlobalTool_EXT void regist_database_adapter(db_adapter_interface* adapter);

// ����ע������ݿ����ʵ�������û��ע����򷵻��ڲ�Ĭ�����ɵ�ʵ����Ĭ��ʵ��ʲôҲ������ֻ��֤����һ��ʵ�����ѣ�
// �˽ӿ�֧�ֲ�������
HM_GlobalTool_EXT db_adapter_interface* get_database_adapter();

#endif //__HMGLOBALTOOL_INTERFACE_DB_ADAPTER_INTERFACE_H__
