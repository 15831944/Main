#pragma once

#ifndef __HMGLOBALTOOL_INTERFACE_SERIALIZE_INTERFACE_H__
#define __HMGLOBALTOOL_INTERFACE_SERIALIZE_INTERFACE_H__



#include <array>
#include <vector>
#include <list>
#include <stack>
#include <map>

#include "../tools/string_tools.h"
#include "../tools/xml_tools.h"
#include "../tools/json.h"
//#include "../macro.h"


class member_rw_interface;


//////////////////////////////////////////////////////////////////////////


// ��Ҫ֧�����л������ݽṹ����Ӵ˽ӿ�����
// û�����������������������ע�᷽ʽ�����ģ�����д���л��꣩�������඼Ҫд���л��꣬�����޷���ȷ�����л�
class HM_GlobalTool_EXT serialize_interface
{
public:
	serialize_interface();
	virtual ~serialize_interface();

public:
	virtual void serialize_members(member_rw_interface& mt);
};


//////////////////////////////////////////////////////////////////////////


// ���л����ݱ�ǣ������ȷ������ֵ���Լ��Ժ���չ������˳�������л�ʧ�ܣ����л�ʱǿתΪint�ͣ�ע�ⲻҪ����������Χ��
enum class serialize_flag
{
	sub_obj_bg = 0,		// �ӽڵ㿪ʼ
	sub_obj_ed = 1,		// �ӽڵ����
	array_bg = 2,		// ���鿪ʼ
	array_ed = 3		// �������
};


//////////////////////////////////////////////////////////////////////////


// ������������װ��һ���������Ա���&��������ʹ�õ�ʱ��ֱ�ӿ��Ը������������͹����һ������ʹ���߲��ù�ϵ�������װ
// ע�⣬���ڱ�����Ҫͬʱ���������ݶ�ȡ��д�룬��ˣ���������ݶ��󲻿�������ֵ����������ֵ��������Ϊ��
// ֵ�����ȷʵ����Ҫ����ֱ��ʹ�����л���д�ӿ�
template <typename T>
class x_data_wrapper
{
public:
	// ��ֹ��ʽת����ֹ�����л������ӿ���ɶ�����
	// Ϊ�����x_wrapper_macroʹ�ã���ֹ����������������ڲ������ʱ����ָ����Ч�����������ã���name��
	// ����Ϊ�Զ����ɣ�����Ϳ�������ʱ������˲��ܴ�Ϊ���ã�����Ҳ����һ��������
	explicit x_data_wrapper(CString name, T& data)
	{
		m_name = name;
		m_data = &data;
	}

	~x_data_wrapper() = default;

public:
	CString get_name() const { return m_name; }
	T* get_data() const { return m_data; }

private:
	CString m_name;
	T* m_data = nullptr;
};


// �μ���x_wrapper_macro_named_enum
template <typename T>
class x_data_wrapper_enum
{
public:
	explicit x_data_wrapper_enum(CString name, T& data)
	{
		m_name = name;
		m_data = &data;
	}

public:
	CString get_name() const { return m_name; }
	T* get_data() const { return m_data; }

private:
	CString m_name;
	T* m_data = nullptr;
};

// �μ���x_wrapper_macro_named_buffer
class HM_GlobalTool_EXT x_data_wrapper_buffer
{
public:
	// ͬʱ���ڶ�д�����Ȳ�֧������ֵ��д��ʱ��Ҫ��ȷ���ú�byte_count������ʱ��ֵ���ڲ�����buffer��ȡ�ĳ���
	explicit x_data_wrapper_buffer(CString name, char*& buffer, long& byte_count);

public:
	CString get_name() const;
	char*& get_buffer() const;
	long& get_byte_count() const;

private:
	CString m_name;
	char*& m_buffer;
	long& m_byte_count;
};


// ������ʱд��serialize_flag::sub_obj_bg��ǣ�����ʱд��serialize_flag::sub_obj_ed���
// һ������������л�������ʼ��������Ϊ���л�����ʱʹ�õı�ʶ����ͬһ����������ͬ�����ڶ�������Ϊmember_rw_interface����mt
class HM_GlobalTool_EXT x_object_flag_serialization_guard
{
public:
	explicit x_object_flag_serialization_guard(CString name, member_rw_interface& mt);
	~x_object_flag_serialization_guard();
	x_object_flag_serialization_guard(const x_object_flag_serialization_guard&) = delete;
	void operator=(const x_object_flag_serialization_guard&) const = delete;

private:
	CString m_name;
	member_rw_interface* m_mt = nullptr;
	bool   is_storing;
};


// ������ʱд��serialize_flag::array_bg��ǣ�����ʱд��serialize_flag::array_ed���
// ����Ϊ���л�����ʱʹ�õı�ʶ����ͬһ����������ͬ�����ڶ�������Ϊmember_rw_interface����mt
class HM_GlobalTool_EXT x_array_flag_serialization_guard
{
public:
	explicit x_array_flag_serialization_guard(CString name, member_rw_interface& mt);
	~x_array_flag_serialization_guard();
	x_array_flag_serialization_guard(const x_array_flag_serialization_guard&) = delete;
	void operator=(const x_array_flag_serialization_guard&) const = delete;

private:
	CString m_name;
	member_rw_interface* m_mt = nullptr;
};



// �ṩ�����Եĺ꣬�򻯳������ݵĶ�д�Լ���д���뱣��һ�£�ǿ�ҽ���ʹ�ô˷���ʵ��ҵ�������л�����
// ����ʹ��read,write�ӿڣ�����ʾ�����£�
// mt & x_wrapper_macro(a) & x_data_wrapper<int>("name", b); // ʹ��&�����������л�
// pair<int, CString> test_pair;
// mt.read(x_wrapper_macro_named(_T("pair"), test_pair)); // Ҳ����ʹ�ö�д�ӿ�
// ע�⣬��������������Ҫͬʱ���������ݶ�ȡ��д�룬��ˣ���������ݶ��󲻿�������ֵ����������ֵ��������Ϊ��
// ֵ�����ȷʵ����Ҫ����ֱ��ʹ�����л���д�ӿ�
#define x_wrapper_macro_named(n, x) x_data_wrapper<decltype(x)>((n),(x))
#define x_wrapper_macro(x) x_wrapper_macro_named(#x, x)
#define x_wrapper_macro_named_enum(n, x) x_data_wrapper_enum<decltype(x)>((n),(x))
#define x_wrapper_macro_enum(x) x_wrapper_macro_named_enum(#x, x)
#define x_wrapper_macro_named_buffer(n, x, c) x_data_wrapper_buffer((n),(x),(c))
#define x_wrapper_macro_buffer(x, c) x_wrapper_macro_named_buffer(#x,x,c)


//////////////////////////////////////////////////////////////////////////


// ��Ա��д�Ľӿڣ���ͬ�Ĵ�ȡ��ʽ����������ʹ��json�洢Ҫ�Ӵ˽ӿ�������ͬ�������ƴ洢��mfc��CArchive��
// ��xml��ʽ�Ⱦ�Ҫ�����ڴ˽ӿ�
// ע�⣬�˽ӿڲ����������ļ���ʽ����������Ϊ�ڴ������ڴ����ת��Ϊ�ļ�����ʹ�ô˽ӿ�
// ǿ�ҽ���ʹ��ǰ���ṩ�ļ��װ�װ��ʵ��ҵ�������л���������ʹ��read,write�ӿڣ�����
// ʾ�����£�
// mt & x_wrapper_macro(123.456) & x_data_wrapper<int>("name", 001); // ʹ��&�����������л�
// pair<int, CString> test_pair;
// mt.read(x_wrapper_macro_named(_T("pair"), test_pair)); // Ҳ����ʹ�ö�д�ӿ�
class HM_GlobalTool_EXT member_rw_interface
{
public:
	virtual bool is_storing() = 0;
	inline bool is_loading() { return !is_storing(); } // �˽ӿڲ���Ҫ����Ϊvirtual��

	// ����&��������Ա�����boost�����л��Ĵ��������ֲ���ۣ��Լ����л��������������
	// �÷���Ҫ�����֣�ʾ�����£�
	// mt & x_wrapper_macro(aaaaa) & x_wrapper_macro(123.456) & x_data_wrapper<int>("name", 001);
	template <typename T>
	member_rw_interface& operator&(x_data_wrapper<T>& data)
	{
		if (is_storing())
			write(data);
		else
			read(data);

		return *this;
	}

	template <typename T>
	member_rw_interface& operator&(x_data_wrapper_enum<T>& data)
	{
		if (is_storing())
		{
			write(data.get_name(), (long long) (*data.get_data())); // ��ȡֵ��תΪ���ͣ���Ҫ��ת������ȡֵ
		}
		else
		{
			long long tmp = 0;
			read(data.get_name(), tmp);
			*data.get_data() = (std::remove_reference<T>::type)tmp;
		}

		return *this;
	}

	member_rw_interface& operator&(x_data_wrapper_buffer& data)
	{
		if (is_storing())
			write(data.get_name(), data.get_buffer(), data.get_byte_count());
		else
			read(data.get_name(), data.get_buffer(), data.get_byte_count());

		return *this;
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// ���溯���汾

	// ����json��ʽ�����ƣ���ȡʱ����ָ���ڵ�����xml�Ͷ����Ʒ�ʽ����Բ��ã���Ϊ��ͳһ��ǿ��Ҫ������
	// json������֮һ�Ǵ󲿷ֿ�д��Ľڵ��ڲ�ʹ��map�洢���������л����ļ�����һ������˳���Լ������ܱ��������ϻ��ݵ�����

	void read(const CString& name, const serialize_flag& data); // ��־�����޸ģ����Ϊconst
	void read(const CString& name, bool& data);
	void read(const CString& name, char& data);
	void read(const CString& name, unsigned char& data);
	void read(const CString& name, wchar_t& data);
	void read(const CString& name, short& data);
	void read(const CString& name, unsigned short& data);
	void read(const CString& name, int& data);
	void read(const CString& name, unsigned int& data);
	void read(const CString& name, long& data);
	void read(const CString& name, unsigned long& data);
	void read(const CString& name, long long& data);
	void read(const CString& name, unsigned long long& data);
	void read(const CString& name, float& data);
	void read(const CString& name, double& data);
	void read(const CString& name, long double& data);
	void read(const CString& name, CString& data);
	void read(const CString& name, std::string& data);
	void read(const CString& name, std::wstring& data);
	
	// buffer���ڲ�ʹ��new���䲢���ⲿʹ�õ��˸�����������ڴ棩��length���ؽ�����buffer�ֽ���
	void read(const CString& name, char*& buffer, long& byte_count);

	void write(const CString& name, const serialize_flag& data);
	void write(const CString& name, const bool& data);
	void write(const CString& name, const char& data);
	void write(const CString& name, const unsigned char& data);
	void write(const CString& name, const wchar_t& data);
	void write(const CString& name, const short& data);
	void write(const CString& name, const unsigned short& data);
	void write(const CString& name, const int& data);
	void write(const CString& name, const unsigned int& data);
	void write(const CString& name, const long& data);
	void write(const CString& name, const unsigned long& data);
	void write(const CString& name, const long long& data);
	void write(const CString& name, const unsigned long long& data);
	void write(const CString& name, const float& data);
	void write(const CString& name, const double& data);
	void write(const CString& name, const long double& data);
	void write(const CString& name, const CString& data);
	void write(const CString& name, const std::string& data);
	void write(const CString& name, const std::wstring& data);

	// д��ʱҪ֧���ַ�������ֵ����ȡʱ���ã�ͨͨתΪCString����д�����
	void write(const CString& name, const char* data);
	void write(const CString& name, const wchar_t* data);
	void write(const CString& name, char* buffer, long byte_count); // buffer���ⲿ�����


	//////////////////////////////////////////////////////////////////////////
	// �����汾
	// ���ڶ����������δ�ڱ��ӿ���ö�ٳ�����Ҫ֧�ֶ�д����Ҫ��serialize_interface�ӿ�������ʵ����ӿ�

	template <typename T>
	inline void read(const CString& name, T& data)
	{
		read_impl(name, data);
	}

	template <typename T>
	inline void write(const CString& name, const T& data)
	{
		write_impl(name, data);
	}

	template <typename T>
	inline void read(x_data_wrapper<T>& data)
	{
		if (!data.get_name().IsEmpty() && data.get_data())
			read(data.get_name(), *data.get_data());
	}

	template <typename T>
	inline void write(x_data_wrapper<T>& data)
	{
		if (!data.get_name().IsEmpty() && data.get_data())
			write(data.get_name(), *data.get_data());
	}


protected:
	// ��Ҫ����json\xml�ȸ�ʽ�����л���ֵ�����ڲ���ǣ���������ݴ����ǣ�Ϊ�����ͽӿ�ƥ�䷶Χ����const����
	virtual inline void read_impl(const CString& name, const serialize_flag& data) = 0;

	virtual inline void read_impl(const CString& name, bool& data) = 0;
	virtual inline void read_impl(const CString& name, char& data) = 0;
	virtual inline void read_impl(const CString& name, unsigned char& data) = 0;
	virtual inline void read_impl(const CString& name, wchar_t& data) = 0;
	virtual inline void read_impl(const CString& name, short& data) = 0;
	virtual inline void read_impl(const CString& name, unsigned short& data) = 0;
	virtual inline void read_impl(const CString& name, int& data) = 0;
	virtual inline void read_impl(const CString& name, unsigned int& data) = 0;
	virtual inline void read_impl(const CString& name, long& data) = 0;
	virtual inline void read_impl(const CString& name, unsigned long& data) = 0;
	virtual inline void read_impl(const CString& name, long long& data) = 0;
	virtual inline void read_impl(const CString& name, unsigned long long& data) = 0;
	virtual inline void read_impl(const CString& name, float& data) = 0;
	virtual inline void read_impl(const CString& name, double& data) = 0;
	virtual inline void read_impl(const CString& name, long double& data) = 0;
	virtual inline void read_impl(const CString& name, CString& data) = 0;

	// buffer���ڲ�ʹ��new���䲢���ⲿʹ�õ��˸�����������ڴ棩��length���ؽ�����buffer�ֽ���
	virtual inline void read_impl(const CString& name, char*& buffer, long& byte_count) = 0;


	// ��Ҫ����json\xml�ȸ�ʽ�����л���ֵ�����ڲ���ǣ�����û��ʹ�ü�ֵ��Ϊ�����ͽӿ�ƥ�䷶Χ����const����
	virtual inline void write_impl(const CString& name, const serialize_flag& data) = 0;

	virtual inline void write_impl(const CString& name, const bool& data) = 0;
	virtual inline void write_impl(const CString& name, const char& data) = 0;
	virtual inline void write_impl(const CString& name, const unsigned char& data) = 0;
	virtual inline void write_impl(const CString& name, const wchar_t& data) = 0;
	virtual inline void write_impl(const CString& name, const short& data) = 0;
	virtual inline void write_impl(const CString& name, const unsigned short& data) = 0;
	virtual inline void write_impl(const CString& name, const int& data) = 0;
	virtual inline void write_impl(const CString& name, const unsigned int& data) = 0;
	virtual inline void write_impl(const CString& name, const long& data) = 0;
	virtual inline void write_impl(const CString& name, const unsigned long& data) = 0;
	virtual inline void write_impl(const CString& name, const long long& data) = 0;
	virtual inline void write_impl(const CString& name, const unsigned long long& data) = 0;
	virtual inline void write_impl(const CString& name, const float& data) = 0;
	virtual inline void write_impl(const CString& name, const double& data) = 0;
	virtual inline void write_impl(const CString& name, const long double& data) = 0;
	virtual inline void write_impl(const CString& name, const CString& data) = 0;
	virtual inline void write_impl(const CString& name, char* buffer, long byte_count) = 0; // buffer���ⲿ�����

protected:
	//////////////////////////////////////////////////////////////////////////
	// �����Ѿ���serialize_members������Ҳ����ֱ�����л�
	
	template <typename T>
	inline void read_impl(const CString& name, T& data) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		data.serialize_members(*this);
	}

	template <typename T>
	inline void write_impl(const CString& name, const T& data) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		const_cast<T&>(data).serialize_members(*this); // ��Ա��������const�ģ�����ͨ��const�������
	}

	//////////////////////////////////////////////////////////////////////////
	// �������������(���Ԫ����ָ�룬�򲻴���֧�֣������Ҫ��Ԫ�ش�CObject����������ָ��������л����û��Լ�ʵ��
	// �����л�ʱ������ͨ��Runtime�����ģ����������new�ģ�����������������ã���֧�֣��ۺ�������ָ������л�
	// ���ܻ������𲻵�̫��ʵ��Ч�������Բ��ṩ)
	template<typename T, size_t N>
	inline void read_impl(const CString& name, std::array<T, N>& val) 
	{
		read_impl(name, serialize_flag::array_bg);

		int count = 0;
		read_impl(_T("count"), count); // nameû�����壬ֻ���ڱ���ԭ��ͳһ

		//val.clear();
		for (int i = 0; i < count && i < (int) val.size(); ++i)
			read_impl(_T("item_") + string_tools::num_to_string(i), val[i]); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		read_impl(name, serialize_flag::array_ed);
	}

	template<typename T, size_t N>
	inline void write_impl(const CString& name, const std::array<T, N>& val)
	{
		write_impl(name, serialize_flag::array_bg);

		write_impl(_T("count"), (int) (val.size())); // nameû�����壬ֻ���ڱ���ԭ��ͳһ
		for (size_t i = 0; i < val.size(); ++i)
			write_impl(_T("item_") + string_tools::num_to_string(i), val[i]); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		write_impl(name, serialize_flag::array_ed);
	}

	// vector<bool>�������������һ��������ѹ���洢�����Ҫʹ���ػ��汾
	void read_impl(const CString& name, std::vector<bool>& val);
	void write_impl(const CString& name, const std::vector<bool>& val);

	template<typename T>
	inline void read_impl(const CString& name, std::vector<T, std::allocator<T>>& val)
	{
		read_impl(name, serialize_flag::array_bg);

		int count = 0;
		read_impl(_T("count"), count); // nameû�����壬ֻ���ڱ���ԭ��ͳһ

		val.resize(count);
		for (int i = 0; i < count; ++i)
			read_impl(_T("item_") + string_tools::num_to_string(i), val[i]); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		read_impl(name, serialize_flag::array_ed);
	}

	template<typename T>
	inline void write_impl(const CString& name, const std::vector<T, std::allocator<T>>& val)
	{
		write_impl(name, serialize_flag::array_bg);

		write_impl(_T("count"), (int) (val.size())); // nameû�����壬ֻ���ڱ���ԭ��ͳһ
		for (size_t i = 0; i < val.size(); ++i)
			write_impl(_T("item_") + string_tools::num_to_string(i), val[i]); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		write_impl(name, serialize_flag::array_ed);
	}

	template<typename T>
	inline void read_impl(const CString& name, std::list<T, std::allocator<T>>& val)
	{
		read_impl(name, serialize_flag::array_bg);

		int count = 0;
		read_impl(_T("count"), count); // nameû�����壬ֻ���ڱ���ԭ��ͳһ

		val.clear();
		for (int i = 0; i < count; ++i)
		{
			T item;
			read_impl(_T("item_") + string_tools::num_to_string(i), item); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������
			val.push_back(item);
		}

		read_impl(name, serialize_flag::array_ed);
	}

	template<typename T>
	inline void write_impl(const CString& name, const std::list<T, std::allocator<T>>& val) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		write_impl(name, serialize_flag::array_bg);

		write_impl(_T("count"), (int) (val.size()));
		size_t index = 0;
		for (const auto& x : val)
			write_impl(_T("item_") + string_tools::num_to_string(index++), x); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		write_impl(name, serialize_flag::array_ed);
	}

	template<typename T1, typename T2>
	inline void read_impl(const CString& name, std::pair<T1, T2>& val) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		// tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������
		T1 key;
		read_impl(name + _T("_key"), key);
		
		T2 value;
		read_impl(name + _T("_value"), value);

		val = std::make_pair(key, value);
	}

	template<typename T1, typename T2>
	inline void write_impl(const CString& name, const std::pair<T1, T2>& val) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		// tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������
		write_impl(name + _T("_key"), val.first);
		write_impl(name + _T("_value"), val.second);
	}

	template<typename TK, typename TV>
	inline void read_impl(const CString& name,
		std::map<TK, TV, std::less<TK>, std::allocator<std::pair<const TK, TV>>>& val) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		read_impl(name, serialize_flag::array_bg);

		int count = 0;
		read_impl(_T("count"), count);

		val.clear();
		for (int i = 0; i < count; ++i)
		{
			std::pair<TK, TV> item;
			read_impl(name + _T("_item_") + string_tools::num_to_string(i), item); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������
			val.insert(item);
		}

		read_impl(name, serialize_flag::array_ed);
	}

	template<typename TK, typename TV>
	inline void write_impl(const CString& name,
		const std::map<TK, TV, std::less<TK>, std::allocator<std::pair<const TK, TV>>>& val) // nameû�����壬ֻ���ڱ���ԭ��ͳһ
	{
		write_impl(name, serialize_flag::array_bg);

		write_impl(_T("count"), (int) (val.size()));
		size_t index = 0;
		for (const auto& x : val)
			write_impl(name + _T("_item_") + string_tools::num_to_string(index++), x); // tinyxml2����ʱ��֧�ֽڵ���Ϊ���ֿ�ͷ,����������

		write_impl(name, serialize_flag::array_ed);
	}

protected:
	member_rw_interface(); // ֻ���ɿ�ܻ��������������������
	member_rw_interface(const member_rw_interface& src);
	virtual ~member_rw_interface();

private:
	friend class serialize_interface;
};


//////////////////////////////////////////////////////////////////////////


class HM_GlobalTool_EXT bin_archive : public member_rw_interface
{
public:
	bin_archive(CArchive& ar);
	virtual ~bin_archive();

public:
	virtual bool is_storing();

protected:
	virtual inline void read_impl(const CString& name, const serialize_flag& data);
	virtual inline void read_impl(const CString& name, bool& data);
	virtual inline void read_impl(const CString& name, char& data);
	virtual inline void read_impl(const CString& name, unsigned char& data);
	virtual inline void read_impl(const CString& name, wchar_t& data);
	virtual inline void read_impl(const CString& name, short& data);
	virtual inline void read_impl(const CString& name, unsigned short& data);
	virtual inline void read_impl(const CString& name, int& data);
	virtual inline void read_impl(const CString& name, unsigned int& data);
	virtual inline void read_impl(const CString& name, long& data);
	virtual inline void read_impl(const CString& name, unsigned long& data);
	virtual inline void read_impl(const CString& name, long long& data);
	virtual inline void read_impl(const CString& name, unsigned long long& data);
	virtual inline void read_impl(const CString& name, float& data);
	virtual inline void read_impl(const CString& name, double& data);
	virtual inline void read_impl(const CString& name, long double& data);
	virtual inline void read_impl(const CString& name, CString& data);
	virtual inline void read_impl(const CString& name, char*& buffer, long& byte_count);

	virtual inline void write_impl(const CString& name, const serialize_flag& data);
	virtual inline void write_impl(const CString& name, const bool& data);
	virtual inline void write_impl(const CString& name, const char& data);
	virtual inline void write_impl(const CString& name, const unsigned char& data);
	virtual inline void write_impl(const CString& name, const wchar_t& data);
	virtual inline void write_impl(const CString& name, const short& data);
	virtual inline void write_impl(const CString& name, const unsigned short& data);
	virtual inline void write_impl(const CString& name, const int& data);
	virtual inline void write_impl(const CString& name, const unsigned int& data);
	virtual inline void write_impl(const CString& name, const long& data);
	virtual inline void write_impl(const CString& name, const unsigned long& data);
	virtual inline void write_impl(const CString& name, const long long& data);
	virtual inline void write_impl(const CString& name, const unsigned long long& data);
	virtual inline void write_impl(const CString& name, const float& data);
	virtual inline void write_impl(const CString& name, const double& data);
	virtual inline void write_impl(const CString& name, const long double& data);
	virtual inline void write_impl(const CString& name, const CString& data);
	virtual inline void write_impl(const CString& name, char* buffer, long byte_count);

private:
	CArchive& m_ar;
};


//////////////////////////////////////////////////////////////////////////


class HM_GlobalTool_EXT json_archive : public member_rw_interface
{
public:
	json_archive(Json::Value* json, bool is_storing = false);
	virtual ~json_archive();

public:
	virtual bool is_storing();

public:
	void clear_parent_nodes_cache();

protected:
	virtual inline void read_impl(const CString& name, const serialize_flag& data);
	virtual inline void read_impl(const CString& name, bool& data);
	virtual inline void read_impl(const CString& name, char& data);
	virtual inline void read_impl(const CString& name, unsigned char& data);
	virtual inline void read_impl(const CString& name, wchar_t& data);
	virtual inline void read_impl(const CString& name, short& data);
	virtual inline void read_impl(const CString& name, unsigned short& data);
	virtual inline void read_impl(const CString& name, int& data);
	virtual inline void read_impl(const CString& name, unsigned int& data);
	virtual inline void read_impl(const CString& name, long& data);
	virtual inline void read_impl(const CString& name, unsigned long& data);
	virtual inline void read_impl(const CString& name, long long& data);
	virtual inline void read_impl(const CString& name, unsigned long long& data);
	virtual inline void read_impl(const CString& name, float& data);
	virtual inline void read_impl(const CString& name, double& data);
	virtual inline void read_impl(const CString& name, long double& data);
	virtual inline void read_impl(const CString& name, CString& data);
	virtual inline void read_impl(const CString& name, char*& buffer, long& byte_count);

	virtual inline void write_impl(const CString& name, const serialize_flag& data);
	virtual inline void write_impl(const CString& name, const bool& data);
	virtual inline void write_impl(const CString& name, const char& data);
	virtual inline void write_impl(const CString& name, const unsigned char& data);
	virtual inline void write_impl(const CString& name, const wchar_t& data);
	virtual inline void write_impl(const CString& name, const short& data);
	virtual inline void write_impl(const CString& name, const unsigned short& data);
	virtual inline void write_impl(const CString& name, const int& data);
	virtual inline void write_impl(const CString& name, const unsigned int& data);
	virtual inline void write_impl(const CString& name, const long& data);
	virtual inline void write_impl(const CString& name, const unsigned long& data);
	virtual inline void write_impl(const CString& name, const long long& data);
	virtual inline void write_impl(const CString& name, const unsigned long long& data);
	virtual inline void write_impl(const CString& name, const float& data);
	virtual inline void write_impl(const CString& name, const double& data);
	virtual inline void write_impl(const CString& name, const long double& data);
	virtual inline void write_impl(const CString& name, const CString& data);
	virtual inline void write_impl(const CString& name, char* buffer, long byte_count);

private:
	Json::Value* m_json = nullptr; // ���ܴ�Ϊ���ã�jsoncpp�⸳ֵ��ʱ���ڲ�ʹ����swap���޷�������סԭ����
	int m_json_array_reading_index = 0; // Ϊjson�����ʱ�����ڼ�ס��ǰ��ȡ��Ԫ���±�
	bool m_is_storing = false;
	
	// ����ʱ��Ƕ����ڴ����ⲿ����firstΪ��json����secondΪ��json������ֳ�array_index������Ϊ�����������ã�
	std::stack<std::pair<Json::Value*, int>> m_parent_nodes;
};


//////////////////////////////////////////////////////////////////////////


class HM_GlobalTool_EXT xml_archive : public member_rw_interface
{
public:
	xml_archive(tinyxml2::XMLElement* xml, bool is_storing = false);
	virtual ~xml_archive();

public:
	virtual bool is_storing();

protected:
	virtual inline void read_impl(const CString& name, const serialize_flag& data);
	virtual inline void read_impl(const CString& name, bool& data);
	virtual inline void read_impl(const CString& name, char& data);
	virtual inline void read_impl(const CString& name, unsigned char& data);
	virtual inline void read_impl(const CString& name, wchar_t& data);
	virtual inline void read_impl(const CString& name, short& data);
	virtual inline void read_impl(const CString& name, unsigned short& data);
	virtual inline void read_impl(const CString& name, int& data);
	virtual inline void read_impl(const CString& name, unsigned int& data);
	virtual inline void read_impl(const CString& name, long& data);
	virtual inline void read_impl(const CString& name, unsigned long& data);
	virtual inline void read_impl(const CString& name, long long& data);
	virtual inline void read_impl(const CString& name, unsigned long long& data);
	virtual inline void read_impl(const CString& name, float& data);
	virtual inline void read_impl(const CString& name, double& data);
	virtual inline void read_impl(const CString& name, long double& data);
	virtual inline void read_impl(const CString& name, CString& data);
	virtual inline void read_impl(const CString& name, char*& buffer, long& byte_count);

	virtual inline void write_impl(const CString& name, const serialize_flag& data);
	virtual inline void write_impl(const CString& name, const bool& data);
	virtual inline void write_impl(const CString& name, const char& data);
	virtual inline void write_impl(const CString& name, const unsigned char& data);
	virtual inline void write_impl(const CString& name, const wchar_t& data);
	virtual inline void write_impl(const CString& name, const short& data);
	virtual inline void write_impl(const CString& name, const unsigned short& data);
	virtual inline void write_impl(const CString& name, const int& data);
	virtual inline void write_impl(const CString& name, const unsigned int& data);
	virtual inline void write_impl(const CString& name, const long& data);
	virtual inline void write_impl(const CString& name, const unsigned long& data);
	virtual inline void write_impl(const CString& name, const long long& data);
	virtual inline void write_impl(const CString& name, const unsigned long long& data);
	virtual inline void write_impl(const CString& name, const float& data);
	virtual inline void write_impl(const CString& name, const double& data);
	virtual inline void write_impl(const CString& name, const long double& data);
	virtual inline void write_impl(const CString& name, const CString& data);
	virtual inline void write_impl(const CString& name, char* buffer, long byte_count);

private:
	tinyxml2::XMLElement* m_xml = nullptr;
	bool m_is_array_reading_mode = false;
	tinyxml2::XMLElement* m_last_child_xml = nullptr;
	CString m_last_child_name;
	bool m_is_storing = false;

	// ����ʱ��Ƕ����ڴ����ⲿ����
	// <0>Ϊ������
	// <1>Ϊ�������Ƿ�������ģʽ��ȡ״̬
	// <2>Ϊ���������һ�ζ�ȡʱʹ�õĽڵ����
	// <3>Ϊ���������һ�ζ�ȡʱʹ�õĽڵ��������������ģʽ��ʼʱ����Ϊ�գ�����ʱ���ջ�лָ�
	// ע����Ϊ����ģʽ��Ҳ��һ���ŵ��ǽڵ�����ȫ��ͬ�Ķ����������������Ԫ�أ����Ҫ��ס��ǰ�������һ�ζ�ȡʱ�Ľ�
	// ��������ȷ����һ������������ģʽ���Ƿ�Ҫ�Ƶ��ֵܽڵ��ϣ��ڵ�����ͬʱ����Ҫ�ƶ�
	std::stack<std::tuple<tinyxml2::XMLElement*, bool, tinyxml2::XMLElement*, CString>> m_parent_nodes;
};


#endif //__HMGLOBALTOOL_INTERFACE_SERIALIZE_INTERFACE_H__

