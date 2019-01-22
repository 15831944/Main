#include "stdafx.h"

#include <tchar.h>
#include <cfloat>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <string>


#include "scope_assemblage.h"


using namespace std;

//////////////////////////////////////////////////////////////////////////

// ͬʱ֧�������ַ���
typedef std::basic_string<TCHAR> tstring;

//////////////////////////////////////////////////////////////////////////

// �ڲ��õ��ĸ������ȽϹ���
class compare_double
{
private:
	compare_double();
	~compare_double();

public:
	static bool is_equal(double first, double decond);
	static bool is_not_equal(double first, double decond);
	static bool is_great(double first, double decond);
	static bool is_great_equal(double first, double decond);
	static bool is_less(double first, double decond);
	static bool is_less_equal(double first, double decond);

	static double get_precision();
	static void set_precision(double dPrec);
	static void restore_default_precision();

private:
	static double m_precision;
};

double compare_double::m_precision = 1e-8; // ��ʼ��Ϊ10��-8�η�

compare_double::compare_double()
{
}

compare_double::~compare_double()
{
}

// ���������бȽϺ�����ֻ����������Ⱥ�С����ʵ��
bool compare_double::is_equal(double first, double decond)
{
	return (abs(first - decond) <= m_precision);
}

bool compare_double::is_less(double first, double decond)
{
	return (!is_equal(first, decond) && first < decond);
}

bool compare_double::is_not_equal(double first, double decond)
{
	return !is_equal(first, decond);
}

bool compare_double::is_great(double first, double decond)
{
	return !is_equal(first, decond) && !is_less(first, decond);
}

bool compare_double::is_great_equal(double first, double decond)
{
	return !is_less(first, decond);
}

bool compare_double::is_less_equal(double first, double decond)
{
	return is_equal(first, decond) || is_less(first, decond);
}

double compare_double::get_precision()
{
	return m_precision;
}

void compare_double::set_precision(double dPrec)
{
	m_precision = dPrec;
}

void compare_double::restore_default_precision()
{
	m_precision = 1e-8; // �������ʼ����ֵ��ͬ
}

//////////////////////////////////////////////////////////////////////////

// һ���ڲ��õ����ַ���������
class private_string_tool
{
private:
	private_string_tool();
	~private_string_tool();

public:
	// �ж��ַ���str�Ƿ���strBegin��ͷ
	static bool is_begin_with(const tstring& str, const tstring& pre);

	// ���ַ�����ָ�����ַ����е������ַ�����ֲ����ؽ����Ĭ�Ϸָ���Ϊ�ո񣬷ָ�����������ʱ��������ʹ�����ô���
	static void split_string(std::vector<tstring>& result, const tstring& text, const tstring split = _T(" "));

	// ���ַ����е����Ķ��š���š��ֺš�����С���š����������š�ȫ�ǿո�ȫ������Ӣ�İ���ַ�
	static void replace_CHS_to_EN_in_scope_string(tstring& str);

	// �Ѹ�����תΪ�ַ�����Ĭ��ֻ����6λС����������0��ɾ��
	static tstring convert_to_tstring(double val);

	// ɾ����β���˵Ŀհ��ַ����ո��Ʊ�����س���
	static void trim_blank(tstring& str);

	// �ַ���ת��
	static std::string wstring_to_string(const std::wstring& str);
	static std::wstring string_to_wstring(const std::string& str);
	static std::string tstring_to_string(const tstring& str);
	static std::wstring tstring_to_wstring(const tstring& str);
	static tstring wstring_to_tstring(const std::wstring& str);
	static tstring string_to_tstring(const std::string& str);
};

private_string_tool::private_string_tool()
{
}

private_string_tool::~private_string_tool()
{
}

bool private_string_tool::is_begin_with(const tstring& str, const tstring& pre)
{
	if (str.empty() && pre.empty()) return true;
	if (str.empty() || pre.empty()) return false;
	if (str.size() < pre.size()) return false;

	tstring strSub = str.substr(0, pre.size());
	return (strSub == pre);
}

void private_string_tool::split_string(std::vector<tstring>& result, const tstring& text, const tstring split /*= _T(" ")*/)
{
	result.clear();
	if (_T("") == text) return;

	tstring tmp;
	for (const TCHAR& x : text)
	{
		if (tstring::npos == split.find_first_of(x))
		{
			tmp += x;
			continue;
		}

		// ��һ���ַ�Ϊ�ָ�����Ϊ֮ǰ��һ���մ�������Ҳ����
		result.push_back(tmp);
		tmp = _T("");
	}

	// �Ѳ����������һ���ַ����ӵ�����У��п����Ǹ��մ���
	// ���һ���ַ�Ϊ�ָ�����Ϊ������һ���մ���Ҳ��ԭ������һ���մ�
	result.push_back(tmp);
}

std::wstring private_string_tool::string_to_wstring(const std::string& str)
{
	// û��ʹ��ϵͳ��API MultiByteToWideChar��������ֲ�ԽϺá�
	// �ַ����е����Ĳ������룬��Ϊ�Ѿ��л�������롣

	std::string s(str.c_str());

	std::string curLocale = setlocale(LC_ALL, nullptr); // curLocale = "C";
	setlocale(LC_ALL, "chs");

	size_t szDist = 0;
	if (0 != mbstowcs_s(&szDist, nullptr, 0, s.c_str(), s.length())) return L""; // ���صĳ����Ѿ����Ͻ�������
	if (szDist <= 0) return L"";

	wchar_t* pwstr = new wchar_t[szDist];
	memset(pwstr, 0, szDist);
	mbstowcs_s(&szDist, pwstr, szDist, s.c_str(), s.length());
	pwstr[szDist - 1] = '\0';
	wstring result(pwstr);
	safe_delete_array(pwstr);

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::string private_string_tool::wstring_to_string(const std::wstring& str)
{
	// û��ʹ��ϵͳ��API WideCharToMultiByte��������ֲ�ԽϺá�
	// �ַ����е����Ĳ������룬��Ϊ�Ѿ��л�������롣

	std::string curLocale = setlocale(LC_ALL, nullptr); // curLocale = "C";
	setlocale(LC_ALL, "chs");

	size_t szDist = 0;
	wcstombs_s(&szDist, nullptr, 0, str.c_str(), 0);
	if (szDist <= 0) return "";
	szDist += sizeof(char); // �����ټ�һ��������(Ӧ�ÿ��Բ�Ҫ�����ò�MSDN�ˣ���һ�����׵�)

	char* pstr = new char[szDist];
	memset(pstr, 0, szDist);
	size_t szDistRes = 0;
	wcstombs_s(&szDistRes, pstr, szDist, str.c_str(), szDist);
	pstr[szDist - 1] = '\0'; // ���һ���ֽ�Ϊ������
	string result(pstr);
	safe_delete_array(pstr);

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::string private_string_tool::tstring_to_string(const tstring& str)
{
	string result;

#	ifdef _UNICODE
	result = wstring_to_string(str);
#	else
	result = str;
#	endif

	return result;
}

std::wstring private_string_tool::tstring_to_wstring(const tstring& str)
{
	wstring result;

#	ifdef _UNICODE
	result = str;
#	else
	result = string_to_wstring(str);
#	endif

	return result;
}

tstring private_string_tool::wstring_to_tstring(const std::wstring& str)
{
	tstring result;

#	ifdef _UNICODE
	result = str;
#	else
	result = wstring_to_string(str);
#	endif

	return result;

}

tstring private_string_tool::string_to_tstring(const std::string& str)
{
	tstring result;

#	ifdef _UNICODE
	result = string_to_wstring(str);
#	else
	result = str;
#	endif

	return result;
}

void private_string_tool::replace_CHS_to_EN_in_scope_string(tstring& str)
{
	// �����ַ���˫�ֽ�ģʽ��ռ�����ַ���������תΪ���ַ����滻������Ҫ�Ӻܶ��ַ�ʶ�����ϵ��߼�
	wstring str_des = tstring_to_wstring(str);

	for (auto& x : str_des)
	{
		if (L'��' == x) { x = L' '; continue; }
		if (L'��' == x) { x = L','; continue; }
		if (L'��' == x) { x = L';'; continue; }
		if (L'��' == x) { x = L'.'; continue; }
		if (L'��' == x) { x = L'['; continue; }
		if (L'��' == x) { x = L']'; continue; }
		if (L'��' == x) { x = L'('; continue; }
		if (L'��' == x) { x = L')'; continue; }
		if (L'��' == x) { x = L'['; continue; }
		if (L'��' == x) { x = L']'; continue; }
		if (L'��' == x) { x = L'0'; continue; }
		if (L'��' == x) { x = L'1'; continue; }
		if (L'��' == x) { x = L'2'; continue; }
		if (L'��' == x) { x = L'3'; continue; }
		if (L'��' == x) { x = L'4'; continue; }
		if (L'��' == x) { x = L'5'; continue; }
		if (L'��' == x) { x = L'6'; continue; }
		if (L'��' == x) { x = L'7'; continue; }
		if (L'��' == x) { x = L'8'; continue; }
		if (L'��' == x) { x = L'9'; continue; }
	}

	str = wstring_to_tstring(str_des);
}

tstring private_string_tool::convert_to_tstring(double val)
{
	TCHAR buffer[50] = _T("");
	_stprintf_s(buffer, _T("%lf"), val); // Ĭ��6λС��
	buffer[49] = _T('\0'); // Ϊ��ֹ��ʽ������ʱ����û�н���������ĩβǿ������һ��

	tstring str_val(buffer);
	size_t dot_pos = str_val.find_first_of(_T('.'));
	
	if (str_val.empty()) return _T("0");
	if (tstring::npos == dot_pos) return str_val;

	size_t zero_start_pos = str_val.find_last_not_of(_T(".0"), dot_pos); // ĩβ��0ȫ��ȥ�������ֻʣ��һ��С����Ҳȥ��
	if (string::npos == zero_start_pos) return str_val;

	str_val.erase(zero_start_pos + 1, str_val.length() - zero_start_pos);

	return str_val;
}

void private_string_tool::trim_blank(tstring& str)
{
	if (str.empty()) return;

	size_t st = str.find_first_not_of(_T(" \t\n\r"));
	if (tstring::npos != st) str = str.substr(st);

	size_t en = str.find_last_not_of(_T(" \t\n\r"));
	if (tstring::npos != en) str = str.substr(0, en + 1);
}

//////////////////////////////////////////////////////////////////////////
bool operator<(double value, const number& num);
bool operator<(const number& num, double value);
bool operator>(double value, const number& num);
bool operator>(const number& num, double value);
bool operator>=(double value, const number& num);
bool operator>=(const number& num, double value);
bool operator<=(double value, const number& num);
bool operator<=(const number& num, double value);
bool operator!=(double value, const number& num);
bool operator!=(const number& num, double value);


const number operator+(const number& num, double value);
const number operator+(double value, const number& num);
const number operator+(const number& num1, const number& num2);
const number operator-(const number& num, double value);
const number operator-(double value, const number& num);
const number operator-(const number& num1, const number& num2);
const number operator*(const number& num, double value);
const number operator*(double value, const number& num);
const number operator*(const number& num1, const number& num2);
const number operator/(const number& num, double value);
const number operator/(double value, const number& num);
const number operator/(const number& num1, const number& num2);

//////////////////////////////////////////////////////////////////////////

number::number(double value /*= 0.0*/)
{
	set_value(value);
}

number::number(bool is_infinitely_great_or_small)
{
	set_value(is_infinitely_great_or_small);
}

number::number( const number& num )
{
	*this = num;
}

number::number( short value )
{
	set_value(value);
}

number::number( int value )
{
	set_value(value);
}

number::number( long value )
{
	set_value(value);
}

number::number( unsigned short value )
{
	set_value(value);
}

number::number( unsigned int value )
{
	set_value(value);
}

number::number( unsigned long value )
{
	set_value(value);
}

number::number( float value )
{
	set_value(value);
}

number::number()
{
	set_value(0.0);
}

void number::set_value(double value)
{
	m_type = 0;
	m_value = value;
}

void number::set_value(bool is_infinitely_great_or_small)
{
	if (is_infinitely_great_or_small)
		set_infinitely_p();
	else
		set_infinitely_n();
}

void number::set_value( const number& num )
{
	if (num.is_infinite_n())
	{
		m_value = 0.0;
		m_type = -1;
	}
	else if (num.is_infinite_p())
	{
		m_value = 0.0;
		m_type = 1;
	}
	else
	{
		m_value = num.get_value();
		m_type = 0;
	}
}

void number::set_value( short value )
{
	set_value((double)value);
}

void number::set_value( int value )
{
	set_value((double)value);
}

void number::set_value( long value )
{
	set_value((double)value);
}

void number::set_value( unsigned short value )
{
	set_value((double)value);
}

void number::set_value( unsigned int value )
{
	set_value((double)value);
}

void number::set_value( unsigned long value )
{
	set_value((double)value);
}

void number::set_value( float value )
{
	set_value((double)value);
}

void number::set_infinitely_p()
{
	m_value = 0.0;
	m_type = 1;
}

void number::set_infinitely_n()
{
	m_value = 0.0;
	m_type = -1;
}

bool number::is_infinite() const
{
	return is_infinite_n() || is_infinite_p();
}

bool number::is_value() const
{
	return 0 == m_type;
}

bool number::is_infinite_p() const
{
	return 1 == m_type;
}

bool number::is_infinite_n() const
{
	return -1 == m_type;
}

double number::get_value() const
{
	if (is_infinite_p())
		return DBL_MAX;
	else if (is_infinite_n())
		return -DBL_MAX; // ע�ⲻ��DBL_MIN;
	else
		return m_value;
}

bool operator<(double value, const number& num)
{
	return number(value) < num;
}

bool operator<(const number& num, double value)
{
	return num < number(value);
}

bool operator>(double value, const number& num)
{
	return number(value) > num;
}

bool operator>(const number& num, double value)
{
	return num > number(value);
}

bool operator>=(double value, const number& num)
{
	return number(value) >= num;
}

bool operator>=(const number& num, double value)
{
	return num >= number(value);
}

bool operator<=(double value, const number& num)
{
	return number(value) <= num;
}

bool operator<=(const number& num, double value)
{
	return num <= number(value);
}

bool operator!=(double value, const number& num)
{
	return number(value) != num;
}

bool operator!=(const number& num, double value)
{
	return num != number(value);
}

bool number::operator<(const number& num) const
{
	if (is_infinite_n() && num.is_infinite_n())
		return false;
	else if (is_infinite_n() && num.is_infinite_p())
		return true;
	else if (is_infinite_n() && num.is_value())
		return true;
	else if (is_infinite_p() && num.is_infinite_n())
		return false;
	else if (is_infinite_p() && num.is_infinite_p())
		return false;
	else if (is_infinite_p() && num.is_value())
		return false;
	else if (is_value() && num.is_infinite_n())
		return false;
	else if (is_value() && num.is_infinite_p())
		return true;
	else if (is_value() && num.is_value())
		return compare_double::is_less(m_value, num.get_value());
	else
		return false;
}

bool number::operator==(const number& num) const
{
	if (is_infinite_n() && num.is_infinite_n())
		return true;
	else if (is_infinite_n() && num.is_infinite_p())
		return false;
	else if (is_infinite_n() &&  num.is_value())
		return false;
	else if (is_infinite_p() && num.is_infinite_n())
		return false;
	else if (is_infinite_p() && num.is_infinite_p())
		return true;
	else if (is_infinite_p() && num.is_value())
		return false;
	else if (is_value() && num.is_infinite_n())
		return false;
	else if (is_value() && num.is_infinite_p())
		return false;
	else if (is_value() && num.is_value())
		return compare_double::is_equal(m_value, num.get_value());
	else
		return false;
}

bool number::operator>(const number& num) const
{
	return !(*this == num || *this < num);
}

bool number::operator>=(const number& num) const
{
	return !(*this < num);
}

bool number::operator<=(const number& num) const
{
	return (*this < num || *this == num);
}

bool number::operator!=(const number& num) const
{
	return !(*this == num);
}

bool number::operator!() const
{
	return (*this == number(0.0)); // Ϊ0����true�����򷵻�false
}

number& number::operator=(double value)
{
	set_value(value);

	return *this;
}

number& number::operator=(const number& num)
{
	set_value(num);

	return *this;
}

number::operator double() const
{
	return ( is_infinite() ? 0.0 : m_value);
}

number::~number()
{

}

std::string number::to_string_a() const
{
	return private_string_tool::wstring_to_string(to_string_w());
}

std::wstring number::to_string_w() const
{
	tstring text;
	if (is_infinite())
		text = _T("");
	else
		text = private_string_tool::convert_to_tstring(get_value());

	return private_string_tool::tstring_to_wstring(text);
}

const number operator+(const number& num, double value)
{
	return operator+(num, number(value));
}

const number operator+(double value, const number& num)
{
	return operator+(number(value), num);
}

const number operator+( const number& num1, const number& num2 )
{
	number numResutl;

	if (num1.is_infinite_p())
	{
		if (num2.is_infinite_n())
			numResutl.set_value(0.0);
		else
			numResutl.set_value(true);
	}
	else if (num1.is_infinite_n())
	{
		if (num2.is_infinite_p())
			numResutl.set_value(0.0);
		else
			numResutl.set_value(false);
	}
	else
	{
		if (num2.is_infinite_p())
			numResutl.set_value(true);
		else if (num2.is_infinite_n())
			numResutl.set_value(false);
		else
			numResutl.set_value(num1.get_value() + num2.get_value());
	}

	return numResutl;
}

const number operator-(const number& num, double value)
{
	return operator-(num, number(value));
}

const number operator-(double value, const number& num)
{
	return operator-(number(value), num);
}

const number operator-( const number& num1, const number& num2 )
{
	number numResutl;

	if (num1.is_infinite_p())
	{
		if (num2.is_infinite_p())
			numResutl.set_value(0.0);
		else
			numResutl.set_value(true);
	}
	else if (num1.is_infinite_n())
	{
		if (num2.is_infinite_n())
			numResutl.set_value(0.0);
		else
			numResutl.set_value(false);
	}
	else
	{
		if (num2.is_infinite_p())
			numResutl.set_value(false); // ���������ɸ�����
		else if (num2.is_infinite_n())
			numResutl.set_value(true); // ����������������
		else
			numResutl.set_value(num1.get_value() - num2.get_value());
	}

	return numResutl;
}

const number operator*(const number& num, double value)
{
	return operator*(num, number(value));
}

const number operator*(double value, const number& num)
{
	return operator*(number(value), num);
}

const number operator*( const number& num1, const number& num2 )
{
	number num_result;

	if (num1.is_infinite_p())
	{
		if (num2.is_infinite_n())
			num_result.set_value(false); // �������븺����˻�Ϊ������
		else if (num2.is_infinite_p())
			num_result.set_value(true);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else if (compare_double::is_less(num2.get_value(), 0.0))
			num_result.set_value(false); // �������븺���˻�Ϊ������
		else
			num_result.set_value(true);
	}
	else if (num1.is_infinite_n())
	{
		if (num2.is_infinite_n())
			num_result.set_value(true); // �������븺����˻�Ϊ������
		else if (num2.is_infinite_p())
			num_result.set_value(false);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else if (compare_double::is_less(num2.get_value(), 0.0))
			num_result.set_value(true); // �������븺���˻�Ϊ������
		else
			num_result.set_value(false);
	}
	else if (compare_double::is_equal(num1.get_value(), 0.0))
	{
		num_result.set_value(0.0);
	}
	else if (compare_double::is_less(num1.get_value(), 0.0))
	{
		if (num2.is_infinite_n())
			num_result.set_value(true); // �������븺���˻�Ϊ������
		else if (num2.is_infinite_p())
			num_result.set_value(false);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else
			num_result.set_value(num1.get_value() * num2.get_value());
	}
	else
	{
		if (num2.is_infinite_n())
			num_result.set_value(false);
		else if (num2.is_infinite_p())
			num_result.set_value(true);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else
			num_result.set_value(num1.get_value() * num2.get_value());
	}

	return num_result;
}

const number operator/(const number& num, double value)
{
	return operator/(num, number(value));
}

const number operator/(double value, const number& num)
{
	return operator/(number(value), num);
}

const number operator/( const number& num1, const number& num2 )
{
	number num_result;

	if (num1.is_infinite_p())
	{
		if (num2.is_infinite_n())
			num_result.set_value(-1.0);
		else if (num2.is_infinite_p())
			num_result.set_value(1.0);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else if (compare_double::is_less(num2.get_value(), 0.0))
			num_result.set_value(false);
		else
			num_result.set_value(true);
	}
	else if (num1.is_infinite_n())
	{
		if (num2.is_infinite_n())
			num_result.set_value(1.0);
		else if (num2.is_infinite_p())
			num_result.set_value(-1.0);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else if (compare_double::is_less(num2.get_value(), 0.0))
			num_result.set_value(true);
		else
			num_result.set_value(false);
	}
	else if (compare_double::is_equal(num1.get_value(), 0.0))
	{
		num_result.set_value(0.0);
	}
	else if (compare_double::is_less(num1.get_value(), 0.0))
	{
		if (num2.is_infinite_n())
			num_result.set_value(0.0); // �������븺���˻�Ϊ������
		else if (num2.is_infinite_p())
			num_result.set_value(-0.0);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else
			num_result.set_value(num1.get_value() / num2.get_value());
	}
	else
	{
		if (num2.is_infinite_n())
			num_result.set_value(-0.0);
		else if (num2.is_infinite_p())
			num_result.set_value(0.0);
		else if (compare_double::is_equal(num2.get_value(), 0.0))
			num_result.set_value(0.0);
		else
			num_result.set_value(num1.get_value() / num2.get_value());
	}

	return num_result;
}

//////////////////////////////////////////////////////////////////////////

scope_value::scope_value()
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(L"");
}

scope_value::scope_value( number val )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(val);
}

scope_value::scope_value(const std::string& left, number l_val, number r_val, const std::string& right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(left, l_val, r_val, right);
}

scope_value::scope_value(const std::string& text)
{
	set_value(text);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = private_string_tool::string_to_wstring(text);
}

scope_value::scope_value( double value )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(number(value));
}

scope_value::scope_value(bool is_open_left, const number& l_val, const number& r_val, bool is_open_right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(is_open_left, l_val, r_val, is_open_right);
}

scope_value::scope_value(const std::wstring& text)
{
	set_value(text);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = text;
}

scope_value::scope_value(const std::wstring& left, number l_val, number r_val, const std::wstring& right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(left, l_val, r_val, right);
}

scope_value::scope_value(const char* text)
{
	string test_string(text);
	set_value(test_string);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = private_string_tool::string_to_wstring(test_string);
}

scope_value::scope_value(const wchar_t* text)
{
	wstring test_string(text);
	set_value(test_string);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = test_string;
}

scope_value::scope_value(const char* left, number l_val, number r_val, const char* right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(string(left), l_val, r_val, string(right));
}

scope_value::scope_value(const wchar_t* left, number l_val, number r_val, const wchar_t* right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(wstring(left), l_val, r_val, wstring(right));
}

scope_value::scope_value(short value)
{
	set_value(value);
}

scope_value::scope_value(int value)
{
	set_value(value);
}

scope_value::scope_value(long value)
{
	set_value(value);
}

scope_value::scope_value(unsigned short value)
{
	set_value(value);
}

scope_value::scope_value(unsigned int value)
{
	set_value(value);
}

scope_value::scope_value(unsigned long value)
{
	set_value(value);
}

scope_value::scope_value(float value)
{
	set_value(value);
}

scope_value::scope_value(bool is_infinitely_great_or_small)
{
	set_value(is_infinitely_great_or_small);
}

scope_value::~scope_value()
{
}

void scope_value::set_value( const number& val )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	m_left = val;
	m_right = val;
	m_is_single_value = true;
	m_is_left_open = false;
	m_is_right_open = false;
}

void scope_value::set_value(const std::string& left, number l_val, number r_val, const std::string& right)
{
	set_value(private_string_tool::string_to_wstring(left), l_val, r_val, private_string_tool::string_to_wstring(right));
}

void scope_value::set_value(const std::wstring& left, number l_val, number r_val, const std::wstring& right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	tstring new_left = private_string_tool::wstring_to_tstring(left);
	tstring new_right = private_string_tool::wstring_to_tstring(right);
	private_string_tool::replace_CHS_to_EN_in_scope_string(new_left);
	private_string_tool::replace_CHS_to_EN_in_scope_string(new_right);

	if (l_val == r_val)
	{
		set_value(l_val); // ����Ϊ����ֵ
	}
	else
	{
		set_value(new_left, l_val);
		set_value(r_val, new_right);
		m_is_single_value = false;
	}
}

void scope_value::set_value(const std::string& left, const number& l_val)
{
	set_value(private_string_tool::string_to_wstring(left), l_val);
}

void scope_value::set_value(const std::wstring& left, const number& l_val)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	tstring new_left = private_string_tool::wstring_to_tstring(left);
	private_string_tool::replace_CHS_to_EN_in_scope_string(new_left);

	m_left = l_val;
	if (new_left.empty())
		set_value(true, l_val);
	else
		set_value(_T('[') != new_left.front(), l_val);
}

void scope_value::set_value(const number& r_val, const std::string& right)
{
	set_value(r_val, private_string_tool::string_to_wstring(right));
}

void scope_value::set_value(const number& r_val, const std::wstring& right)
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	tstring new_right = private_string_tool::wstring_to_tstring(right);
	private_string_tool::replace_CHS_to_EN_in_scope_string(new_right);

	m_right = r_val;
	if (new_right.empty())
		set_value(r_val, true);
	else
		set_value(r_val, _T(']') != new_right.front());
}

void scope_value::set_value( double value )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	set_value(number(value));
}

void scope_value::set_value( bool is_open_left, const number& l_val, const number& r_val, bool is_open_right )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	if (l_val == r_val)
	{
		set_value(l_val); // ����Ϊ����ֵ
	}
	else
	{
		set_value(is_open_left, l_val);
		set_value(r_val, is_open_right);
		m_is_single_value = false;
	}
}

void scope_value::set_value( bool is_open_left, const number& l_val )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	m_is_left_open = is_open_left;
	m_left = l_val;
}

void scope_value::set_value( const number& r_val, bool is_open_right )
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	m_is_right_open = is_open_right;
	m_right = r_val;
}

void scope_value::set_value(const std::string& strText)
{
	set_value(private_string_tool::string_to_wstring(strText));
}

void scope_value::set_value(const std::wstring& strText)
{
	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = strText;

	tstring text_copy = private_string_tool::wstring_to_tstring(strText);
	private_string_tool::replace_CHS_to_EN_in_scope_string(text_copy);
	private_string_tool::trim_blank(text_copy);

	size_t split_pos = text_copy.find_first_of(_T(" ,")); // �ո�Ͷ��Ŷ�������Ϊ��������ķָ���
	if (tstring::npos == split_pos)
	{
		// û���ҵ�ʱ�����������Ϊ�ձ�ʾ��������Ϊ��ֵ����
		if (text_copy.empty())
		{
			set_value(_T("("), number(false), number(true), _T(")"));
		}
		else
		{
			if (_T("+") == text_copy) set_value(number(true));
			else if (_T("-") == text_copy) set_value(number(false));
			else set_value(number(_tstof(text_copy.c_str())));
		}
	}
	else
	{
		// �����������
		tstring left = text_copy.substr(0, split_pos);
		tstring right = text_copy.substr(split_pos + 1);

		private_string_tool::trim_blank(left);
		private_string_tool::trim_blank(right);

		// ȥ���Ų��������俪���յ����
		tstring left_kh = _T("(");
		tstring right_kh = _T(")");

		if (!left.empty())
		{
			if (_T('[') == left.front() || _T('(') == left.front()
				|| _T(')') == left.front() || _T(']') == left.front())
			{
				if (_T('[') == left.front() || _T(']') == left.front()) left_kh = _T('[');
				left.erase(0, 1);
				private_string_tool::trim_blank(left);
			}
		}

		if (!right.empty())
		{
			if (_T('[') == right.back() || _T('(') == right.back()
				|| _T(')') == right.back() || _T(']') == right.back())
			{
				if (_T('[') == right.back() || _T(']') == right.back()) right_kh = _T(']');
				right.erase(right.length() - 1, 1);
				private_string_tool::trim_blank(right);
			}
		}

		// ������������ֵ
		number numLeft;
		number numRight;

		if (left.empty() || _T("-") == left)
			numLeft.set_value(number(false));
		else
			numLeft.set_value(number(_tstof(left.c_str())));

		if (right.empty() || _T("+") == right)
			numRight.set_value(number(true));
		else
			numRight.set_value(number(_tstof(right.c_str())));

		// ��������
		set_value(left_kh, numLeft, numRight, right_kh);
	}
}

void scope_value::set_value(const char* text)
{
	set_value(string(text));
}

void scope_value::set_value(const wchar_t* text)
{
	set_value(wstring(text));
}

void scope_value::set_value(const number& r_val, const char* right)
{
	set_value(r_val, string(right));
}

void scope_value::set_value(const number& r_val, const wchar_t* right)
{
	set_value(r_val, wstring(right));
}

void scope_value::set_value(const char* left, const number& l_val)
{
	set_value(string(left), l_val);
}

void scope_value::set_value(const wchar_t* left, const number& l_val)
{
	set_value(wstring(left), l_val);
}

void scope_value::set_value(const char* left, number l_val, number r_val, const char* right)
{
	set_value(string(left), l_val, r_val, string(right));
}

void scope_value::set_value(const wchar_t* left, number l_val, number r_val, const wchar_t* right)
{
	set_value(wstring(left), l_val, r_val, wstring(right));
}

void scope_value::set_value(short value)
{
	set_value(number(value));
}

void scope_value::set_value(int value)
{
	set_value(number(value));
}

void scope_value::set_value(long value)
{
	set_value(number(value));
}

void scope_value::set_value(unsigned short value)
{
	set_value(number(value));
}

void scope_value::set_value(unsigned int value)
{
	set_value(number(value));
}

void scope_value::set_value(unsigned long value)
{
	set_value(number(value));
}

void scope_value::set_value(float value)
{
	set_value(number(value));
}

void scope_value::set_value(bool is_infinitely_great_or_small)
{
	set_value(number(is_infinitely_great_or_small));
}

void scope_value::get_left_value(number& l_val, bool& is_open) const
{
	l_val = m_left;
	is_open = m_is_left_open;
}

const number scope_value::get_left_value() const
{
	bool is_open = false;

	number l_val;
	get_left_value(l_val, is_open);

	return l_val;
}

void scope_value::get_right_value( number& r_val, bool& is_open ) const
{
	r_val = m_right;
	is_open = m_is_right_open;
}

const number scope_value::get_right_value() const
{
	bool is_open = false;

	number r_val;
	get_right_value(r_val, is_open);

	return r_val;
}

bool scope_value::is_open( bool is_left /*= true*/ ) const
{
	if (is_left)
	{
		if (m_left.is_infinite())
			return true;
		else
			return m_is_left_open;
	}
	else
	{
		if (m_right.is_infinite())
			return true;
		else
			return m_is_right_open;
	}
}

bool scope_value::is_closed( bool is_left /*= true*/ ) const
{
	return !is_open(is_left);
}

bool scope_value::is_in_scope(const number& val ) const
{
	if (m_is_single_value) return (m_left == val);

	bool is_left = false;
	if (m_is_left_open)
		is_left = (val > m_left);
	else
		is_left = (val >= m_left);

	bool bRight = false;
	if (m_is_right_open)
		bRight = (val < m_right);
	else
		bRight = (val <= m_right);

	return is_left && bRight;	
}

bool scope_value::is_in_scope( const scope_value& scope ) const
{
	if (&scope == this) return true;

	if (is_single_value())
	{
		if (scope.is_single_value())
		{
			return (get_single_value() == scope.get_single_value());
		}
		else
		{
			bool is_open_left = false;
			bool is_open_right = false;
			number numLeft;
			number numRight;

			scope.get_left_value(numLeft, is_open_left);
			scope.get_right_value(numRight, is_open_right);

			// Ϊ����ʱҪ�������Ƿ�����ֵ��ͬ�Ҿ�Ϊ������,���򷵻�true,���򷵻�false(��Ϊ���䲻���ܰ�������ֵ��)
			return (numLeft == numRight && false == is_open_left && false == is_open_right);
		}
	}
	else
	{
		if (scope.is_single_value())
		{
			return is_in_scope(scope.get_single_value());
		}
		else
		{
			// ������������
			number left;
			bool is_open_left = false;
			number right;
			bool is_open_right = false;

			scope.get_left_value(left, is_open_left);
			scope.get_right_value(right, is_open_right);

			// IsInScopeֻ���жϾ������ֵ,�൱�Ǵ�һ���������һ���˵�,����һ��������Ķ˵㴫�����
			// ����,������������Ķ˵�ֵ��ͬ,����Ϊ������,��ʱ�ͻ�������,����Ҫ��ǰ�Ѳ������ڵ�����
			// Ϊ�������������������
			
			bool check_left = false;
			bool check_right = false;

			if (left == get_left_value() && true == is_open_left) // ���ʱ������϶��ᱻ��������,�������߶��ж��Ƿ�Ϊ������
				check_left = true;
			else
				check_left = is_in_scope(left);

			if (right == get_right_value() && true == is_open_right) // ���ʱ������϶��ᱻ��������,�������߶��ж��Ƿ�Ϊ������
				check_right = true;
			else
				check_right = is_in_scope(right);

			return (check_left && check_right);
		}
	}

	// warning C4702: unreachable code������ÿ��·�����з���ֵ�ˣ�Ϊ���ξ�����ע�͵����Ժ�ά������Ĵ���ʱҪС�Ĳ�Ҫ©��ĳ��·���ķ���ֵ
	// return false;
}

int scope_value::get_scope_status() const
{
	if (m_is_single_value)
		return -1;
	else if (m_left < m_right)
		return 1;
	else
		return 0;
}

std::string scope_value::to_string_a() const
{
	return private_string_tool::wstring_to_string(to_string_w());
}

std::wstring scope_value::to_string_w() const
{
	wstring text;

	if (m_is_old_ui_string)
	{
		text = m_old_ui_string;
	}
	else
	{
		if (m_is_single_value)
		{
			text = m_left.to_string_w();
		}
		else
		{
			text += (m_is_left_open ? L"(" : L"[");
			text += m_left.to_string_w() + L"," + m_right.to_string_w();
			text += (m_is_right_open ? L")" : L"]");
		}
	}

	return text;
}

bool scope_value::is_single_value() const
{
	return m_is_single_value;
}

void scope_value::get_single_value( number& val ) const
{
	val = m_left;
}

const number scope_value::get_single_value() const
{
	number num;
	get_single_value(num);
	return num;
}

bool scope_value::operator==( const scope_value& scope ) const
{
	bool bR = false;

	if (m_is_single_value)
	{
		bR = ( scope.is_single_value() && scope.get_left_value() == m_left );
	}
	else
	{
		bR = ( false == scope.is_single_value()
			&& scope.get_left_value() == m_left
			&& scope.get_right_value() == m_right
			&& scope.is_open(true) == m_is_left_open
			&& scope.is_open(false) == m_is_right_open);
	}

	return bR;
}

bool scope_value::operator!=( const scope_value& assem ) const
{
	return !(*this == assem);
}

void scope_value::set_left_value( bool is_open_left, const number& l_val )
{
	set_value(is_open_left, l_val);
}

void scope_value::set_left_value(const std::string& left, const number& l_val)
{
	set_value(left, l_val);
}

void scope_value::set_left_value(const std::wstring& left, const number& l_val)
{
	set_value(left, l_val);
}

void scope_value::set_left_value(const char* left, const number& l_val)
{
	set_left_value(string(left), l_val);
}

void scope_value::set_left_value(const wchar_t* left, const number& l_val)
{
	set_left_value(wstring(left), l_val);
}

void scope_value::set_right_value(const number& r_val, bool is_open_right)
{
	set_value(r_val, is_open_right);
}

void scope_value::set_right_value(const number& r_val, const std::string& right)
{
	set_value(r_val, right);
}

void scope_value::set_right_value(const number& r_val, const std::wstring& right)
{
	set_value(r_val, right);
}

void scope_value::set_right_value(const number& r_val, const char* right)
{
	set_right_value(r_val, string(right));
}

void scope_value::set_right_value(const number& r_val, const wchar_t* right)
{
	set_right_value(r_val, wstring(right));
}

bool scope_value::is_old_ui_string() const
{
	return m_is_old_ui_string;
}

std::wstring scope_value::get_old_ui_string() const
{
	return m_old_ui_string;
}

bool scope_value::split_scope( const number& val, bool is_close, scope_value& out_left, scope_value& out_right ) const
{
	// ��ֵʱ���⴦��
	if (is_single_value())
	{
		number num = get_single_value();
		if ( num != val) return false;

		if (false == is_close) return false;

		out_left.set_value(num);
		out_right.set_value(num);

		return true;
	}

	// �����䴦��

	if (0 == get_scope_status()) return false; // �������
	if (false == is_in_scope(val)) return false; // ����������

	scope_value left = *this;
	left.set_right_value(val, !is_close);
	
	// ����Ƿ���[6, 6]������������䣬������Ϊ��ֵ
	bool is_open_left = false;
	bool is_open_right = false;
	number numLeft;
	number numRight;

	left.get_left_value(numLeft, is_open_left);
	left.get_right_value(numRight, is_open_right);

	if (numLeft == numRight)
	{
		if (false == is_open_left && false == is_open_right)
			left.set_value(numLeft);
		else
			return false;
	}

	// ��������乹���Ƿ����
	if (0 == left.get_scope_status()) return false;


	// ͬ���ķ�ʽ����ڶ�������
	scope_value right = *this;
	right.set_right_value(val, !is_close);

	right.get_left_value(numLeft, is_open_left);
	right.get_right_value(numRight, is_open_right);

	if (numLeft == numRight)
	{
		if (false == is_open_left && false == is_open_right)
			right.set_value(numLeft);
		else
			return false;
	}

	// ���ɵ������޷�������ѧҪ��
	if (0 == right.get_scope_status()) return false;

	
	// ����ִ�����
	out_left = left;
	out_right = right;

	return true;
}

bool scope_value::merge_scope_value( const number& val )
{
	return merge_scope_value(scope_value(val));
}

bool scope_value::merge_scope_value( const scope_value& val )
{
	scope_value item_copy = *this; // ����һ������

	if (0 == val.get_scope_status()) return false;

	if (is_single_value() && val.is_single_value())
	{
		if (*this == val) return true; // ��Ȳ��úϲ�
	}
	else if (is_single_value() && false == val.is_single_value())
	{
		if (val.is_in_scope(*this))
		{
			*this = val;
			return true; // �Ѱ��������úϲ�
		}

		// ����Ƿ���������һ������,��ʱ���������Ϊ������

		number num;
		bool is_open = false;
		val.get_left_value(num, is_open);

		if (true == is_open && num == get_single_value())
		{
			*this = val;
			set_left_value(false, num);
			return true;
		}

		val.get_right_value(num, is_open);
		if (true == is_open && num == get_single_value())
		{
			*this = val;
			set_right_value(num, false);
			return true;
		}
	}
	else if (false == is_single_value() && val.is_single_value())
	{
		if (is_in_scope(val)) return true; // �Ѱ��������úϲ�

		// ����Ƿ���������һ������,��ʱ���������Ϊ������

		number num;
		bool is_open = false;
		get_left_value(num, is_open);

		if (true == is_open && num == val.get_single_value())
		{
			set_left_value(false, num);
			return true;
		}

		get_right_value(num, is_open);
		if (true == is_open && num == val.get_single_value())
		{
			set_right_value(num, false);
			return true;
		}
	}
	else
	{
		if (*this == val) return true; // ���úϲ�
		
		// ��������������Ѿ���ǰ���Ⲣɾ����
		bool is_open_left_01 = false;
		bool is_open_left_02 = false;
		bool is_open_right_01 = false;
		bool is_open_right_02 = false;

		number left01;
		number left02;
		number right01;
		number right02;

		get_left_value(left01, is_open_left_01);
		get_right_value(right01, is_open_right_01);
		val.get_left_value(left02, is_open_left_02);
		val.get_right_value(right02, is_open_right_02);

		// �ж����������Ƿ��й�������
		bool is_ok = false;

		// ����ֻ�жϸ����䣬������Ҫ�����жϣ��Է�һ������ֵΪ����һ��Ϊ��ʱ����©�������������ж�
		// ���������Ƿ��й������ֻ���û�й������ֵ����������������
		if (is_in_scope(left02) || is_in_scope(right02) || val.is_in_scope(left01) || val.is_in_scope(right01))
			is_ok = true;

		if (false == is_ok) return false; // û�й������ֻ���û���������Ķ˵㲻�ܺϲ�

		// ��ȡ�ϲ�������˵�ֵ���������
		bool has_result_left = false;
		number numResultLeft;

		if (left01 < left02)
		{
			has_result_left = is_open_left_01;
			numResultLeft = left01;			
		}
		else if (left01 > left02)
		{
			has_result_left = is_open_left_02;
			numResultLeft = left02;			
		}
		else
		{
			has_result_left = (false == is_open_left_01 || false == is_open_left_02 ? false : true); // ��ֵһ�������ܿ��������һ��������ȡ������
			numResultLeft = left01; // ��ֵһ����ȡһֵ
		}


		bool has_result_right = false;
		number numResultRight;

		if (right01 < right02)
		{
			has_result_right = is_open_right_02;
			numResultRight = right02;			
		}
		else if (right01 > right02)
		{
			has_result_right = is_open_right_01;
			numResultRight = right01;			
		}
		else
		{
			has_result_right = (false == is_open_right_01 || false == is_open_right_02 ? false : true); // ��ֵһ�������ܿ��������һ��������ȡ������
			numResultRight = is_open_right_01; // ��ֵһ����ȡһֵ
		}

		set_value(has_result_left, numResultLeft, numResultRight, has_result_right);
		
		return true; // �ϲ��ɹ�
	}

	// �޷��ϲ�������ԭֵ����
	*this = item_copy; // �ָ�ԭֵ

	return false;
}

assemblage_value::assemblage_value()
{
	set_null();

	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value( const assemblage_value& assem )
{
	*this = assem;
}

assemblage_value::assemblage_value(const std::string& text)
{
	set_value(text);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = private_string_tool::string_to_wstring(text);
}

assemblage_value::assemblage_value(const number& val)
{
	set_value(val);
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value(const std::wstring& text)
{
	set_value(text);

	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse,Ϊ��ֹ���������޸�,�������
	m_old_ui_string = text;
}

assemblage_value::assemblage_value(double value)
{
	set_value(value);
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value(const char* text)
{
	set_value(text);
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value(const scope_value& val)
{
	set_value(val);
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value(const wchar_t* text)
{
	set_value(text);
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

assemblage_value::assemblage_value(short value)
{
	set_value(value);
}

assemblage_value::assemblage_value(int value)
{
	set_value(value);
}

assemblage_value::assemblage_value(long value)
{
	set_value(value);
}

assemblage_value::assemblage_value(unsigned short value)
{
	set_value(value);
}

assemblage_value::assemblage_value(unsigned int value)
{
	set_value(value);
}

assemblage_value::assemblage_value(unsigned long value)
{
	set_value(value);
}

assemblage_value::assemblage_value(float value)
{
	set_value(value);
}

assemblage_value::assemblage_value(bool is_infinitely_great_or_small)
{
	set_value(is_infinitely_great_or_small);
}

void assemblage_value::set_value(const std::string& text)
{
	set_value(private_string_tool::string_to_wstring(text));
}

void assemblage_value::set_value(const std::wstring& text)
{
	m_is_old_ui_string = true; // ÿ��Initialize()�����ַ�������ʱ�Ͱ�����Ϊtrue,һ���Ķ��ͱ�Ϊfalse
	m_old_ui_string = text;

	m_all_scope.clear();
	
	tstring assem_no_const = private_string_tool::wstring_to_tstring(text);
	private_string_tool::replace_CHS_to_EN_in_scope_string(assem_no_const);
	private_string_tool::trim_blank(assem_no_const);

	// ����ĩβ���һ���ֺŻ򶺺ţ�����ֺŻ򶺺�ʱʱ��ɾ��һ��������İ��ָ���������
	if (!assem_no_const.empty())
	{
		if (_T(';') == assem_no_const.back() || _T(',') == assem_no_const.back())
		{
			assem_no_const.erase(assem_no_const.length() - 1, 1);
			private_string_tool::trim_blank(assem_no_const); // ȥ���ֺź���Ҫ�ٳ�һ�οհ��ַ�,�Է��ֺ�ǰ�пո�
		}
	}

	if (assem_no_const.empty())
	{
		m_all_scope.push_back(scope_value(_T(""))); // �ձ�ʾ����ɸ����������
	}
	else
	{
		vector<tstring> all_assem;
		private_string_tool::split_string(all_assem, assem_no_const, _T(";"));

		for (tstring item_copy : all_assem) // ʹ�ö��󣬲������ã�Ŀ�����ڲ�Ҫ�����˵Ŀհ׽����޼�
		{
			private_string_tool::trim_blank(item_copy);
			m_all_scope.push_back(scope_value(private_string_tool::tstring_to_wstring(item_copy)));
		}
	}

	arrange();
}

void assemblage_value::set_value(const char* text)
{
	set_value(string(text));
}

void assemblage_value::set_value(const wchar_t* text)
{
	set_value(wstring(text));
}

const std::string assemblage_value::to_string_a() const
{
	return private_string_tool::wstring_to_string(to_string_w());
}

const std::wstring assemblage_value::to_string_w() const
{
	wstring strResult;

	if (m_is_old_ui_string)
	{
		strResult = m_old_ui_string;
	}
	else
	{
		for (size_t i = 0; i < m_all_scope.size(); ++i)
		{
			strResult += m_all_scope[i].to_string_w();
			if (i < m_all_scope.size() - 1)
				strResult += L";";
		}
	}

	return strResult;
}

void assemblage_value::set_null()
{
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
	m_all_scope.clear();
}

bool assemblage_value::is_null() const
{
	// ���䲻���ܴ��ڿ����䣬�������ʾ��������������ֻҪ��Ԫ�ؾ�
	// ��ʾ�зǿ����䣬ֻ�м��ϲŴ��ڿռ��ϵ�˵��
	return m_all_scope.empty();
}

bool assemblage_value::operator==( const assemblage_value& assem ) const
{
	return is_subset(assem) && assem.is_subset(*this);
}

bool assemblage_value::operator!=( const assemblage_value& assem ) const
{
	return !(*this == assem);
}

assemblage_value::~assemblage_value()
{
}

void assemblage_value::set_value( const number& val )
{
	set_null();
	union_set(val);

	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

void assemblage_value::set_value( double value )
{
	set_value(number(value));
}

void assemblage_value::set_value(short value)
{
	set_value(number(value));
}

void assemblage_value::set_value(int value)
{
	set_value(number(value));
}

void assemblage_value::set_value(long value)
{
	set_value(number(value));
}

void assemblage_value::set_value(unsigned short value)
{
	set_value(number(value));
}

void assemblage_value::set_value(unsigned int value)
{
	set_value(number(value));
}

void assemblage_value::set_value(unsigned long value)
{
	set_value(number(value));
}

void assemblage_value::set_value(float value)
{
	set_value(number(value));
}

void assemblage_value::set_value(bool is_infinitely_great_or_small)
{
	set_value(number(is_infinitely_great_or_small));
}

void assemblage_value::set_value( const scope_value& val )
{
	set_null();
	union_set(val); // �ڲ������Arrange()

	m_is_old_ui_string = false;
	m_old_ui_string = L"";
}

void assemblage_value::set_value( const assemblage_value& value )
{
	*this = value;
	arrange();
}

void assemblage_value::set_value( const std::vector<scope_value>& scope_items )
{
	if (scope_items.empty()) set_null();

	m_all_scope = scope_items;
	m_is_old_ui_string = false;
	m_old_ui_string = L"";
	arrange();
}

assemblage_value& assemblage_value::intersection(const number& num)
{
	for (size_t i = 0; i < m_all_scope.size(); /* Nothing */)
	{
		if (m_all_scope[i].is_in_scope(num))
		{
			m_all_scope.clear();
			m_all_scope.push_back(scope_value(num));
			break; // ֻ����һ���Ϳ�����
		}
		else
		{
			// û�н�����ɾ����
			m_all_scope.erase(m_all_scope.begin() + i);
		}
	}

	m_is_old_ui_string = false;
	m_old_ui_string = L"";
	
	return *this;
}

// ������������Ĺ������䣬���û�й������䣬�򷵻�false�����򷵻�true
bool GetIntersectionOfTwoScope(const scope_value& first, const scope_value& second, scope_value& result)
{
	if (0 == first.get_scope_status() || 0 == second.get_scope_status()) return false;

	// Ϊ��Ԫ����ֵʱҪ���⴦��,��Ϊ��������Ե�ֵ��Ч,������Ӱ���жϽ��
	// Ϊ����ʱ,�����κ����,�������ȡ�ϴ�,�Ҷ�ȡ��С��ԭ��
	
	// ����ֵ���
	if (first.is_single_value())
	{
		if (second.is_in_scope(first.get_single_value()))
		{
			result.set_value(first.get_single_value());
			return true;
		}
		else
		{
			return false;
		}
	}

	if (second.is_single_value())
	{
		if (first.is_in_scope(second.get_single_value()))
		{
			result.set_value(second.get_single_value());
			return true;
		}
		else
		{
			return false;
		}
	}

	// �����������

	number left01;
	bool is_left_open_01 = false;
	first.get_left_value(left01, is_left_open_01);

	number right01;
	bool is_right_open_01 = false;
	first.get_right_value(right01, is_right_open_01);

	number left02;
	bool is_left_open_02 = false;
	second.get_left_value(left02, is_left_open_02);

	number right02;
	bool is_right_open_02 = false;
	second.get_right_value(right02, is_right_open_02);

	number left_result;
	bool has_left_open_result = false;

	number rightResult;
	bool has_right_open_result = false;

	
	// ������
	if (left01 < left02)
	{
		left_result = left02;
		has_left_open_result = is_left_open_02;
	}
	else if (left01 > left02)
	{
		left_result = left01;
		has_left_open_result = is_left_open_01;
	}
	else
	{
		left_result = left01;

		if (is_left_open_01 == is_left_open_02)
			has_left_open_result = is_left_open_01;
		else
			has_left_open_result = true; // �󽻼�Ҫ���Ȱ������䴦���������˵㣩(�󲢼������Ȱ������䴦��)
	}

	// ������
	if (right01 < right02)
	{
		rightResult = right01;
		has_right_open_result = is_right_open_01;
	}
	else if (right01 > right02)
	{
		rightResult = right02;
		has_right_open_result = is_right_open_02;
	}
	else
	{
		rightResult = right01;

		if (is_right_open_01 == is_right_open_02)
			has_right_open_result = is_right_open_01;
		else
			has_right_open_result = true; // �󽻼�Ҫ���Ȱ������䴦���������˵㣩(�󲢼������Ȱ������䴦��
	}

	scope_value sc(has_left_open_result, left_result, rightResult, has_right_open_result);
	if (0 == sc.get_scope_status()) return false;

	// ��Ϊǰ�������ȡ��,��ȡС��ԭ������������, ���п��ܱ������������û�н���,�ǽ���ʹ���,����
	// �˴���Ҫ��֤�Խ��������֤����ȷ���ٸ�ֵ���Է��Ƿ��������ɴ���ṹ������
	if (false == first.is_in_scope(sc) || false == second.is_in_scope(sc)) return false;

	result = sc;

	return true;
}

assemblage_value& assemblage_value::intersection(const scope_value& scope)
{
	// ȫ���Ľ������ҵ�����������Ҫ�ҵ�һ���ͷ��أ���Ϊ����ÿ������Ľ����Ĵ�С��һ��
	for (size_t i = 0; i < m_all_scope.size(); /* Nothing */)
	{
		scope_value tmp;
		if (GetIntersectionOfTwoScope(m_all_scope[i], scope, tmp))
		{
			m_all_scope[i] = tmp; // �д����Ϊ����
			++i;
		}
		else
		{
			// û�н�����ɾ����
			m_all_scope.erase(m_all_scope.begin() + i);
		}
	}

	arrange();

	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	return *this;
}

assemblage_value& assemblage_value::intersection(const assemblage_value& scope)
{
	if (this == &scope) return *this;

	vector<scope_value> vecResult;
	for (size_t i = 0; i < m_all_scope.size(); ++i)
	{
		// ������ѭ���е��ò���Ϊ������󽻼������ذ汾,��Ϊ���ڲ���ɾ��Ԫ��,����ɴ˴�Խ��
		for (size_t j = 0; j < scope.m_all_scope.size(); ++j)
		{
			scope_value tmp;
			if (GetIntersectionOfTwoScope(m_all_scope[i], scope.m_all_scope[j], tmp))
				vecResult.push_back(tmp);
		}
	}

	m_all_scope = vecResult;

	arrange();

	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	return *this;
}

assemblage_value& assemblage_value::union_set(const number& num)
{
	scope_value sc(num);
	m_all_scope.push_back(sc);

	arrange();
	
	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	return *this;
}

assemblage_value& assemblage_value::union_set(const scope_value& scope)
{
	m_all_scope.push_back(scope);

	arrange();

	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	return *this;
}

assemblage_value& assemblage_value::union_set(const assemblage_value& scope)
{
	if (this == &scope) return *this;

	for (size_t i = 0; i < scope.m_all_scope.size(); ++i)
		m_all_scope.push_back(scope.m_all_scope[i]);

	arrange();

	m_is_old_ui_string = false;
	m_old_ui_string = L"";

	return *this;
}

bool assemblage_value::has_intersection(const number& num) const
{
	assemblage_value tmp_assem = *this;
	tmp_assem.intersection(num);

	return (false == tmp_assem.is_null());
}

bool assemblage_value::has_intersection(const scope_value& scope) const
{
	assemblage_value tmp_assem = *this;
	tmp_assem.intersection(scope);

	return (false == tmp_assem.is_null());
}

bool assemblage_value::has_intersection(const assemblage_value& assem) const
{
	assemblage_value tmp_assem = *this;
	tmp_assem.intersection(assem);

	return (false == tmp_assem.is_null());
}

bool assemblage_value::is_subset(const assemblage_value& assem) const
{
	if (this == &assem) return true;

	assemblage_value tmp_assem_01 = *this;
	assemblage_value tmp_assem_02 = assem;

	// ȷ���ڲ�����������û���ص���û�м�ϣ�������һ����������ϲ���������˵�������䶼û�м�ϣ�
	tmp_assem_01.arrange();
	tmp_assem_02.arrange();

	for (size_t i = 0; i < tmp_assem_01.m_all_scope.size(); ++i)
	{
		bool bIn = false;
		for (size_t j = 0; j < tmp_assem_02.m_all_scope.size(); ++j)
		{
			if (tmp_assem_02.m_all_scope[i].is_in_scope(tmp_assem_01.m_all_scope[j])) // 02����01
			{
				bIn = true;
				break;
			}
		}

		if (false == bIn) return false;
	}

	return true;
}

bool assemblage_value::is_superset(const assemblage_value& assem) const
{
	if (this == &assem) return true;

	return (assem.is_subset(*this)); // ����д��false == IsSubset(assem); ����������Ӽ���Ҳ���ܲ����Ӽ������н���
}

void assemblage_value::arrange()
{
	// ����һ��֮�󲢲���֪���Ƿ��Ѿ���ɣ���Ϊ�п��ܵ�һ�ε���ʱ��������û�й�ϵ������
	// ������������һ�������ԭ��û�й�ϵ�������������й�ϵ�ˣ����Ա���Ҫ���ϵ�����ֱ
	// ��û���κ����䷢�����ϲ�Ϊֹ��
	
	bool be_arranged = false; // ִ�й�������Ԫ�ط������仯�򷵻�false��û��Ԫ�ط����仯˵���Ѿ������
	
	while(false == be_arranged)
	{
		be_arranged = true; // ����Ϊ���β����Ѿ�����ã������б䶯�ٸ�Ϊfalse

		for (size_t i = 0; i < m_all_scope.size(); /* Nothing */)
		{
			scope_value& itemNew = m_all_scope[i];

			// ����������Ƴ�
			if (0 == itemNew.get_scope_status())
			{
				m_all_scope.erase(m_all_scope.begin() + i);
				be_arranged = false;
				continue;
			}

			for (size_t j = i + 1; j < m_all_scope.size(); /* Nothing */)
			{
				if (true == itemNew.merge_scope_value(m_all_scope[j]))
				{
					m_all_scope.erase(m_all_scope.begin() + j);
					be_arranged = false;
					continue;
				}

				++j;
			}

			++i;
		}

	} // end of while

	// ��Ҫ��ԭʼ�ַ�������������ֻ���Ż��ṹ�������ı�����ķ�Χ�����Բ�Ҫ��ԭʼ�ַ���
	//m_is_old_ui_string = false;
	//m_old_ui_string = L"";

}

assemblage_value& assemblage_value::operator=(const assemblage_value& assem)
{
	if (this == &assem) return *this;

	m_is_old_ui_string = assem.m_is_old_ui_string;
	m_old_ui_string = assem.m_old_ui_string;
	m_all_scope = assem.m_all_scope;

	return *this;
}

void assemblage_value::to_scope_items( std::vector<scope_value>& scope_items ) const
{
	scope_items.clear();
	scope_items = m_all_scope;
}

const std::vector<scope_value> assemblage_value::to_scope_items() const
{
	vector<scope_value> scope_items = m_all_scope;
	return scope_items;
}

bool assemblage_value::is_in_assemblage( const number& num ) const
{
	return has_intersection(num);
}

bool assemblage_value::is_in_assemblage( const scope_value& scope ) const
{
	return has_intersection(scope);
}

bool assemblage_value::is_in_assemblage( const assemblage_value& scope ) const
{
	return has_intersection(scope);
}


//////////////////////////////////////////////////////////////////////////

// ���Դ���,����
//class test_scope
//{
//public:
//	test_scope()
//	{
//		// ���ʹ�����ֹ�����û�ж�����
//		number nm_1(1);
//		number nm_2(1.0);
//
//		scope_value sc_1(1);
//		scope_value sc_2(1.0);
//
//		assemblage_value as_1(1);
//		assemblage_value as_2(1.0);
//
//		assemblage_value sa;
//		sa.union_set(scope_value(_T("(��,5)")));
//		sa.union_set(number(2));						// saΪ(0, 5)
//		sa.union_set(number(0));						// saΪ[0, 5)
//		sa.union_set(number(5));						// saΪ[0, 5]
//		sa.union_set(scope_value(_T("[0, 3)")));		// saΪ[0, 5]
//		sa.union_set(scope_value(_T("(-10, 0)")));		// saΪ(-10, 5]
//		sa.union_set(scope_value(_T("[-20, -10)")));	// saΪ[-20, 10) (-10, 5]
//		sa.union_set(scope_value(_T("(5, 10)")));		// saΪ[-20, 10) (-10, 10)
//		sa.union_set(scope_value(_T("[5, 20)")));		// saΪ[-20, 10) (-10, 20)
//		sa.union_set(scope_value(_T("(20, 30)")));		// saΪ[-20, 10) (-10, 20) (20, 30)
//
//		assemblage_value saNew = sa;
//		sa.union_set(saNew);							// saΪ[-20, 10) (-10, 20) (20, 30)
//		sa.intersection(scope_value(_T("(5, 15)")));	// saΪ(5, 15)
//
//	}
//};
//
//
//test_scope k;
