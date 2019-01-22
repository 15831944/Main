#pragma once
#include <vector>
#include <utility>
#include <string>
#include "../interface/db_adapter_interface.h"
#include "mysql\mysqlpp\mysql++.h"


// mysql���ݿ����������ע��mysql����Ҫ������ʽָ��Ϊutf8������������
class mysql_db_adapter : public db_adapter_interface
{
public:
	mysql_db_adapter();
	~mysql_db_adapter();

private:
	virtual bool test_connection() const;
	virtual unsigned long long get_query_result_count(const CString& sql);
	virtual std::pair<bool, CString> get_query_result(std::vector<db_record>& result, const CString& sql);
	virtual std::pair<bool, CString> run_as_a_batch(std::vector<const CString>& all_sql);
	virtual std::pair<bool, CString> get_batch_result(std::vector<const CString>& all_sql, std::vector<db_record>& result);
	virtual std::pair<bool, CString> run_alone(const CString& sql);

private:
	// ʹ��ָ�����Ӷ���ֱ�Ӳ�ѯһ�����������
	static std::pair<bool, CString> get_select_result_by_connection(
		mysqlpp::Connection& conn, std::vector<db_record>& result, const CString& sql);

	// ����һ���Ѿ����ӵ����ݿ����Ӷ�������޷��������ӻ������쳣�򷵻�nullptr
	static std::unique_ptr<mysqlpp::Connection> make_db_connection();

	// ����ѯ�����һ���ֶ�תΪC++���������ͣ�ע��mysql����Ҫ������ʽָ��Ϊutf8
	static void convert_to_field_data(db_field& field_data,
		const mysqlpp::String& field_db, const mysqlpp::FieldTypes::value_type& type_db);
};
