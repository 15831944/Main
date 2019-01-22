#include "stdafx.h"
#include "../tools/string_tools.h"
#include "mysql/mysqlpp/mysql++.h"
#include "mysql/mysqlpp/dbdriver.h"

#include "mysql_db_adapter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
using namespace mysqlpp;


mysql_db_adapter::mysql_db_adapter()
{
}

mysql_db_adapter::~mysql_db_adapter()
{

}

bool mysql_db_adapter::test_connection() const
{
	return (nullptr != make_db_connection());
}

unsigned long long mysql_db_adapter::get_query_result_count(const CString& sql)
{
	if (!is_select_sql(sql)) return 0ull;

	try
	{
		auto conn = make_db_connection();
		if (!conn) return 0ull;

		// StoreQueryResult�᷵�ز�ѯ�����ݣ�Ϊ��֤�������ݰ�ȫ�ԣ���Ҫʹ�ô����ݷ��ص�sql��䣬Ӧ����ʹ�þۺ�
		// ��������Ӧ��ʹ�ã�
		// select count(*) as fd_count from tb_test
		// �����ǣ�
		// select * as fd_count from tb_test
		// ��Ȼ�õ�������һ��������ȫ�Ժʹ������������ȫ����һ������
		StoreQueryResult res = conn->query(string_tools::CString_to_utf8(sql)).store();
		return res.size();
	}
	catch (const Exception&)
	{
		// nothing;
	}

	return 0ull;
}

std::pair<bool, CString> mysql_db_adapter::get_query_result(std::vector<db_record>& result, const CString& sql)
{
	result.clear();
	if (!is_select_sql(sql)) return make_pair(false, _T("û�п���ִ�е�sql���"));

	bool is_ok = false;
	CString error_info;

	try
	{
		auto conn = make_db_connection();
		if (!conn) return make_pair(false, _T("�޷��������ݿ�"));

		pair<bool, CString> tmp_states = get_select_result_by_connection(*conn.get(), result, sql);
		is_ok = tmp_states.first;
		error_info = tmp_states.second;
	}
	catch (const Exception& ex)
	{
		is_ok = false;
		error_info = string_tools::utf8_to_CString(ex.what());
	}

	return make_pair(is_ok, error_info);
}

std::pair<bool, CString> mysql_db_adapter::run_as_a_batch(std::vector<const CString>& all_sql)
{
	if (all_sql.empty()) return make_pair(false, _T("û�п���ִ�е�sql���"));

	bool is_ok = false;
	CString error_info;

	try
	{
		auto conn = make_db_connection();
		if (!conn) return make_pair(false, _T("�޷��������ݿ�"));

		Query qu = conn->query();
		
		// �ȹ����conn����ָ�룬���쳣ʱ�϶�����������Ự����ʱ���Զ��ع�������conn��ʱ��û����������˲��������
		Transaction trans(*conn.get(), Transaction::serializable, Transaction::session);
		
		for (const CString& sql : all_sql)
		{
			if (sql.IsEmpty()) continue;
			qu.exec(string_tools::CString_to_utf8(sql));
		}

		trans.commit();
		is_ok = true;
		error_info = _T("");
	}
	catch (const Exception& ex)
	{
		is_ok = false;
		error_info = string_tools::utf8_to_CString(ex.what());
	}

	return make_pair(is_ok, error_info);
}


std::pair<bool, CString> mysql_db_adapter::get_batch_result(std::vector<const CString>& all_sql, std::vector<db_record>& result)
{
	result.clear();

	if (all_sql.empty()) return make_pair(false, _T("û�п���ִ�е�sql���"));

	const CString& last_sql = all_sql.back();
	if (!is_select_sql(last_sql)) return make_pair(false, _T("���һ����������select���"));

	bool is_ok = false;
	CString error_info;

	try
	{
		auto conn = make_db_connection();
		if (!conn) return make_pair(false, _T("�޷��������ݿ�"));

		Query qu = conn->query();
		
		// �ȹ����conn����ָ�룬���쳣ʱ�϶�����������Ự����ʱ���Զ��ع�������conn��ʱ��û����������˲��������
		Transaction trans(*conn.get(), Transaction::serializable, Transaction::session);
		
		for (size_t i = 0; i < all_sql.size() - 1; ++i) // size() - 1�������������ѭ������Ϊǰ�汣֤Ϊ������Ϊ��
		{
			if (all_sql[i].IsEmpty()) continue;
			qu.exec(string_tools::CString_to_utf8(all_sql[i]));
		}

		pair<bool, CString> tmp_states = get_select_result_by_connection(*conn.get(), result, last_sql);
		is_ok = tmp_states.first;
		error_info = tmp_states.second;

		trans.commit();

		is_ok = true;
		error_info = _T("");
	}
	catch (const Exception& ex)
	{
		is_ok = false;
		error_info = string_tools::utf8_to_CString(ex.what());
	}

	return make_pair(is_ok, error_info);
}

std::pair<bool, CString> mysql_db_adapter::run_alone(const CString& sql)
{
	if (sql.IsEmpty()) return make_pair(false, _T("sql���Ϊ�գ��޷�ִ��"));

	bool is_ok = false;
	CString err_info;
	try
	{
		auto conn = make_db_connection();
		if (!conn) return make_pair(false, _T("���ݿ�����ʧ��"));

		conn->query(string_tools::CString_to_utf8(sql)).execute();
		is_ok = true;
		err_info = _T("");
	}
	catch (const Exception& er)
	{
		is_ok = false;
		string u8_err_info(er.what());
		err_info = string_tools::utf8_to_CString(u8_err_info);
	}

	return make_pair(is_ok, err_info);
}

std::pair<bool, CString> mysql_db_adapter::get_select_result_by_connection(
	mysqlpp::Connection& conn, std::vector<db_record>& result, const CString& sql)
{
	result.clear();
	bool is_ok = false;
	CString err_info;

	// UseQueryResult res_use = conn->query().use();
	StoreQueryResult res_store = conn.query(string_tools::CString_to_utf8(sql)).store();

	size_t row_count = res_store.num_rows();
	size_t column_count = res_store.fields().size();
	if (row_count <= 0 || column_count <= 0) return make_pair(true, _T(""));

	result.resize(row_count);
	for (auto& row : result)
		row.m_row.resize(column_count);

	for (size_t i = 0; i < row_count; ++i)
	{
		db_record& rec = result[i];

		for (size_t j = 0; j < column_count; ++j)
		{
			rec.m_row[j].first = string_tools::utf8_to_CString(res_store.field_name((int) j));
			convert_to_field_data(rec.m_row[j].second, res_store[i][j], res_store.field_type((int) j));
		}
	}

	return make_pair(true, _T(""));
}

std::unique_ptr<mysqlpp::Connection> mysql_db_adapter::make_db_connection()
{
	// ���������룬����Ҫ��Ϊ�����ý������ò������������Ӵ��İ�ȫ��
	unique_ptr<Connection> conn = make_unique<Connection>();
	conn->set_option(new mysqlpp::SetCharsetNameOption("utf8"));
	bool is_ok = conn->connect("bridge_bim", "rm-uf6kw1r6nv7u5rpovo.mysql.rds.aliyuncs.com",
		"bridge_bim_user", "bridge", 3306);
	//bool is_ok = conn->connect("xiajianguang", "127.0.0.1",
	//	"root", "tonghao123.", 3306);

	if (is_ok && conn->connected()) return conn;

	return nullptr;
}

void mysql_db_adapter::convert_to_field_data(db_field& field_data,
	const mysqlpp::String& field_db, const mysqlpp::FieldTypes::value_type& type_db)
{
	// mysql֧�ֵ���������Ŀǰȫ��������Ŀ��
	// sql_tinyint									 // sql_tinyint_null
	// sql_bool										 // sql_bool_null
	// sql_boolean									 // sql_boolean_null
	// sql_tinyint_unsigned							 // sql_tinyint_unsigned_null
	// sql_smallint									 // sql_smallint_null
	// sql_smallint_unsigned						 // sql_smallint_unsigned_null
	// sql_int										 // sql_int_null
	// sql_int_unsigned								 // sql_int_unsigned_null
	// sql_mediumint								 // sql_mediumint_null
	// sql_mediumint_unsigned						 // sql_mediumint_unsigned_null
	// sql_bigint									 // sql_bigint_null
	// sql_bigint_unsigned							 // sql_bigint_unsigned_null
	// sql_int1										 // sql_int1_null
	// sql_int2										 // sql_int2_null
	// sql_int3										 // sql_int3_null
	// sql_int4										 // sql_int4_null
	// sql_int8										 // sql_int8_null
	// sql_middleint								 // sql_middleint_null

	// sql_float									 // sql_float_null
	// sql_double									 // sql_double_null
	// sql_decimal									 // sql_decimal_null
	// sql_fixed									 // sql_fixed_null
	// sql_float4									 // sql_float4_null
	// sql_float8									 // sql_float8_null
	// sql_numeric									 // sql_numeric_null

	// sql_enum										 // sql_enum_null
	// sql_char										 // sql_char_null
	// sql_varchar									 // sql_varchar_null
	// sql_character_varying						 // sql_character_varying_null
	// sql_tinytext									 // sql_tinytext_null
	// sql_text										 // sql_text_null
	// sql_mediumtext								 // sql_mediumtext_null
	// sql_longtext									 // sql_longtext_null
	// sql_long_varchar								 // sql_long_varchar_null
	// sql_long										 // sql_long_null
	// sql_blob										 // sql_blob_null
	// sql_tinyblob									 // sql_tinyblob_null
	// sql_mediumblob								 // sql_mediumblob_null
	// sql_long_varbinary							 // sql_long_varbinary_null
	// sql_longblob									 // sql_longblob_null

	// sql_date										 // sql_date_null
	// sql_datetime									 // sql_datetime_null
	// sql_time										 // sql_time_null
	// sql_timestamp								 // sql_timestamp_null

	// sql_set										 // sql_set_null

	//////////////////////////////////////////////////////////////////////////

	if (typeid(sql_tinyint) == type_db)						{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_bool) == type_db)						{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_boolean) == type_db)						{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_tinyint_unsigned) == type_db)			{ field_data.set_value(unsigned short(field_db)); return; }
	if (typeid(sql_smallint) == type_db)					{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_smallint_unsigned) == type_db)			{ field_data.set_value(unsigned short(field_db)); return; }
	if (typeid(sql_int) == type_db)							{ field_data.set_value(int(field_db)); return; }
	if (typeid(sql_int_unsigned) == type_db)				{ field_data.set_value(unsigned int(field_db)); return; }
	if (typeid(sql_mediumint) == type_db)					{ field_data.set_value(int(field_db)); return; }
	if (typeid(sql_mediumint_unsigned) == type_db)			{ field_data.set_value(unsigned int(field_db)); return; }
	if (typeid(sql_bigint) == type_db)						{ field_data.set_value(long long(field_db)); return; }
	if (typeid(sql_bigint_unsigned) == type_db)				{ field_data.set_value(unsigned long long(field_db)); return; }
	if (typeid(sql_int1) == type_db)						{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_int2) == type_db)						{ field_data.set_value(short(field_db)); return; }
	if (typeid(sql_int3) == type_db)						{ field_data.set_value(int(field_db)); return; }
	if (typeid(sql_int4) == type_db)						{ field_data.set_value(int(field_db)); return; }
	if (typeid(sql_int8) == type_db)						{ field_data.set_value(long long(field_db)); return; }
	if (typeid(sql_middleint) == type_db)					{ field_data.set_value(int(field_db)); return; }

	if (typeid(sql_float) == type_db)						{ field_data.set_value(float(field_db)); return; }
	if (typeid(sql_double) == type_db)						{ field_data.set_value(double(field_db)); return; }
	if (typeid(sql_decimal) == type_db)						{ field_data.set_value(double(field_db)); return; }
	if (typeid(sql_fixed) == type_db)						{ field_data.set_value(double(field_db)); return; }
	if (typeid(sql_float4) == type_db)						{ field_data.set_value(float(field_db)); return; }
	if (typeid(sql_float8) == type_db)						{ field_data.set_value(double(field_db)); return; }
	if (typeid(sql_numeric) == type_db)						{ field_data.set_value(double(field_db)); return; }

	if (typeid(sql_enum) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_char) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_varchar) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_character_varying) == type_db)			{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_tinytext) == type_db)					{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_text) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_mediumtext) == type_db)					{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_longtext) == type_db)					{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_long_varchar) == type_db)				{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_long) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_blob) == type_db)						{ field_data.set_value(make_pair(field_db.size(), field_db.data()), true); return; }
	if (typeid(sql_tinyblob) == type_db)					{ field_data.set_value(make_pair(field_db.size(), field_db.data()), true); return; }
	if (typeid(sql_mediumblob) == type_db)					{ field_data.set_value(make_pair(field_db.size(), field_db.data()), true); return; }
	if (typeid(sql_long_varbinary) == type_db)				{ field_data.set_value(make_pair(field_db.size(), field_db.data()), true); return; }
	if (typeid(sql_longblob) == type_db)					{ field_data.set_value(make_pair(field_db.size(), field_db.data()), true); return; }

	if (typeid(sql_date) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_datetime) == type_db)					{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_time) == type_db)						{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_timestamp) == type_db)					{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }

	if (typeid(sql_set) == type_db)							{ field_data.set_value(string_tools::utf8_to_CString(field_db.c_str())); return; }

	//////////////////////////////////////////////////////////////////////////
	// ǧ��ע�⣬��null��β�����Ͳ��ǿ��������ͣ����Ǳ�ʾ����Ϊ�յ����ͣ����ݿ�Ŀ�NULL������C++����Ϊ�գ������
	// Ϊ��������ʱҲ�п��������ݣ�������ԭ�򰴲���null�Ĵ�����ʹΪNULL���ݿ�Ҳ������������Ӧ���ݣ�������Ҫ�ֹ�
	// �ж��Ƿ����ݳ��ȴ���0��0��ʾΪNULL������Щ��������ΪNULLʱ�����Ƿ���0ֵ���ݣ����Ƿ���һЩ��������ݣ�����
	// �����ͣ�ΪNULLʱ���ء�NULL���ַ������Ⲣ�������������ģ�����Ҫ����0ֵ���أ������ַ����������Ҫ���⴦��

	if (typeid(sql_tinyint_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_bool_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_boolean_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_tinyint_unsigned_null) == type_db)		{ field_data.set_value(field_db.is_null() ? (unsigned short)0          : unsigned short(field_db)); return; }
	if (typeid(sql_smallint_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_smallint_unsigned_null) == type_db)		{ field_data.set_value(field_db.is_null() ? (unsigned short)0          : unsigned short(field_db)); return; }
	if (typeid(sql_int_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (int)0                     : int(field_db)); return; }
	if (typeid(sql_int_unsigned_null) == type_db)			{ field_data.set_value(field_db.is_null() ? (unsigned int)0            : unsigned int(field_db)); return; }
	if (typeid(sql_mediumint_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (int)0                     : int(field_db)); return; }
	if (typeid(sql_mediumint_unsigned_null) == type_db)		{ field_data.set_value(field_db.is_null() ? (unsigned int)0            : unsigned int(field_db)); return; }
	if (typeid(sql_bigint_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (long long)0               : long long(field_db)); return; }
	if (typeid(sql_bigint_unsigned_null) == type_db)		{ field_data.set_value(field_db.is_null() ? (unsigned long long)0      : unsigned long long(field_db)); return; }
	if (typeid(sql_int1_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_int2_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (short)0                   : short(field_db)); return; }
	if (typeid(sql_int3_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (int)0                     : int(field_db)); return; }
	if (typeid(sql_int4_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (int)0                     : int(field_db)); return; }
	if (typeid(sql_int8_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (long long)0               : long long(field_db)); return; }
	if (typeid(sql_middleint_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (int)0                     : int(field_db)); return; }

	if (typeid(sql_float_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (float)0                   : float(field_db)); return; }
	if (typeid(sql_double_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (double)0                  : double(field_db)); return; }
	if (typeid(sql_decimal_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (double)0                  : double(field_db)); return; }
	if (typeid(sql_fixed_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (double)0                  : double(field_db)); return; }
	if (typeid(sql_float4_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (float)0                   : float(field_db)); return; }
	if (typeid(sql_float8_null) == type_db)					{ field_data.set_value(field_db.is_null() ? (double)0                  : double(field_db)); return; }
	if (typeid(sql_numeric_null) == type_db)				{ field_data.set_value(field_db.is_null() ? (double)0                  : double(field_db)); return; }

	if (typeid(sql_enum_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_char_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_varchar_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_character_varying_null) == type_db)		{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_tinytext_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_text_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_mediumtext_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_longtext_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_long_varchar_null) == type_db)			{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_long_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_blob_null) == type_db)					{ field_data.set_value(field_db.is_null() ? make_pair(0, nullptr)      : make_pair(field_db.size(), field_db.data()),      field_db.is_null() ? false : true); return; }
	if (typeid(sql_tinyblob_null) == type_db)				{ field_data.set_value(field_db.is_null() ? make_pair(0, nullptr)      : make_pair(field_db.size(), field_db.data()),      field_db.is_null() ? false : true); return; }
	if (typeid(sql_mediumblob_null) == type_db)				{ field_data.set_value(field_db.is_null() ? make_pair(0, nullptr)      : make_pair(field_db.size(), field_db.data()),      field_db.is_null() ? false : true); return; }
	if (typeid(sql_long_varbinary_null) == type_db)			{ field_data.set_value(field_db.is_null() ? make_pair(0, nullptr)      : make_pair(field_db.size(), field_db.data()),      field_db.is_null() ? false : true); return; }
	if (typeid(sql_longblob_null) == type_db)				{ field_data.set_value(field_db.is_null() ? make_pair(0, nullptr)      : make_pair(field_db.size(), field_db.data()),      field_db.is_null() ? false : true); return; }

	if (typeid(sql_date_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_datetime_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_time_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
	if (typeid(sql_timestamp_null) == type_db)				{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }

	if (typeid(sql_set_null) == type_db)					{ field_data.set_value(field_db.is_null() ? CString(_T(""))            : string_tools::utf8_to_CString(field_db.c_str())); return; }
}
