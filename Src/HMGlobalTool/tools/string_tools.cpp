#include "stdafx.h"
#include <string>
#include "md5_tools.h"
#include "../private_detail/x_string_to_time.h"

#include "string_tools.h"

 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


CString string_tools::format(const TCHAR* format, ...)
{
	CString result;

	// ���´���������MFC�е�Format��Ա������ʵ��
	va_list argList;
	va_start(argList, format);
	result.FormatV(format, argList);
	va_end(argList);

	return result;
}

CString string_tools::time_format(const TCHAR* format, int y, int m, int d, int hr /*= 0*/, int mi /*= 0*/, int se /*= 0*/)
{
	return time_format(format, convert_time(y, m, d, hr, mi, se));
}

CString string_tools::time_format(const TCHAR* format, const tm& t)
{
	TCHAR buff[1028] = ""; // �Զ༸���ֽ���������������أ�ѧMFC������ʵ����1K byte������1K ���ַ���unicodeΪ2K��
	_tcsftime(buff, 1024 * sizeof(TCHAR), format, &t); // ������С�����ֽڣ�ע��˺�����mbcs��Ϊ�ֽ�������unicode��Ϊ�ַ������൱��ͳһΪ�ַ���
	return CString(buff);
}

CString string_tools::time_format(const TCHAR* format, time_t t)
{
	return time_format(format, convert_time(t));
}

CString string_tools::time_format(const TCHAR* format)
{
	return time_format(format, time(nullptr)); // ����Ϊnullptr,�������ڵ�����ʱ��
}

time_t string_tools::convert_time_stamp(const TCHAR* format, const TCHAR* t)
{
	return convert_time_stamp(convert_time(format, t));
}

time_t string_tools::convert_time_stamp(const tm& t)
{
	return mktime((tm*)&t); // ǿ��ȥ��const����
}

time_t string_tools::convert_time_stamp(int y, int m, int d, int hr /*= 0*/, int mi /*= 0*/, int se /*= 0*/)
{
	tm t;
	t.tm_year = y;
	t.tm_mon = m;
	t.tm_mday = d;
	t.tm_hour = hr;
	t.tm_min = mi;
	t.tm_sec = se;
	t.tm_isdst = 0; // ����ʱ��ʶ����ʵ������ʱ��ʱ��tm_isdstΪ������ʵ������ʱ�Ľ���tm_isdstΪ0�����˽����ʱ��tm_isdst()Ϊ�����й�һ�㲻ʵ������ʱ
	return convert_time_stamp(t);
}

time_t string_tools::convert_time_stamp()
{
	return time(nullptr); // ����Ϊnullptr,�������ڵ�����ʱ��
}

tm string_tools::convert_time(const TCHAR* format, const TCHAR* t)
{
	// GCC��strptime�������ڴ˹��ܣ���VC��֧�֣��Ƿ�Ϊ��׼Ҫ��ĺ�����û�в���C++��׼���ݲ���������Ϻܶ෽
	// ������ʹ��sscanf��ֱ��ȡ����������̫�࣬������ã�����һ�����׵ķ�����ʹ��boost�е�ת�����ߣ����ǵ���
	// �ܿⲻ����boost��������󲻲�ȡ�˷�����
	tm loc_tm;
	string fmt_str = CString_to_string(format);
	string tm_str = CString_to_string(t);
	x_private_strptime(fmt_str.c_str(), tm_str.c_str(), &loc_tm);
	return loc_tm;
}

tm string_tools::convert_time(time_t t)
{
	tm* loc_tm_pt = localtime(&t);
	tm loc_tm = *loc_tm_pt;
	safe_delete(loc_tm_pt);
	return loc_tm;
}

tm string_tools::convert_time(int y, int m, int d, int hr /*= 0*/, int mi /*= 0*/, int se /*= 0*/)
{
	return convert_time(convert_time_stamp(y, m, d, hr, mi, se));
}

tm string_tools::convert_time()
{
	return convert_time(convert_time_stamp());
}

inline std::string string_tools::wstring_to_string(const std::wstring& str)
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

inline std::wstring string_tools::string_to_wstring(const std::string& str)
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

CString string_tools::wstring_to_CString(const std::wstring& str)
{
	return string_to_CString(wstring_to_string(str));
}

std::wstring string_tools::CString_to_wstring(const CString& str)
{
	return string_to_wstring(CString_to_string(str));
}

CString string_tools::string_to_CString(const std::string& str)
{
#	ifdef _UNICODE
	return CString(string_to_wstring(str).c_str());
#	else
	return CString (str.c_str());
#	endif
}

void wstring_to_utf8_string(const std::wstring& in, std::string& out)
{
	out = "";

	int out_len = ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.length(), nullptr, 0, nullptr, nullptr);
	if (out_len <= 0) return;

	char* pResult = new char[out_len + 1];
	if (nullptr == pResult) return;
	memset(pResult, 0, (out_len + 1) * sizeof(char));

	::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), out_len, pResult, out_len * sizeof(char), nullptr, nullptr);
	pResult[out_len] = '\0';
	out.assign(pResult, out_len);

	safe_delete_array(pResult);
	return;
}

void utf8_string_to_wstring(const std::string& in, std::wstring& out)
{
	int out_len = ::MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length() * sizeof(char), nullptr, 0);
	if (out_len <= 0) return;

	wchar_t* pResult = new wchar_t[out_len + 1];
	memset(pResult, 0, (out_len + 1) * sizeof(wchar_t));

	::MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length() * sizeof(char), pResult, out_len);
	pResult[out_len] = L'\0';

	out.assign(pResult, out_len);
	safe_delete_array(pResult);
}

std::string string_tools::utf8_to_string(const std::string& str)
{
	wstring ws;
	utf8_string_to_wstring(str, ws);
	return wstring_to_string(ws);
}

std::wstring string_tools::utf8_to_wstring(const std::string& str)
{
	wstring ws;
	utf8_string_to_wstring(str, ws);
	return ws;
}

CString string_tools::utf8_to_CString(const std::string& str)
{
	wstring ws;
	utf8_string_to_wstring(str, ws);
	return wstring_to_CString(ws);
}

std::string string_tools::string_to_utf8(const std::string& str)
{
	wstring ws = string_to_wstring(str);
	string st;
	wstring_to_utf8_string(ws, st);
	return st;
}

std::string string_tools::wstring_to_utf8(const std::wstring& str)
{
	string st;
	wstring_to_utf8_string(str, st);
	return st;
}

std::string string_tools::CString_to_utf8(const CString& str)
{
	wstring ws = CString_to_wstring(str);
	string st;
	wstring_to_utf8_string(ws, st);
	return st;
}

CString string_tools::num_to_string(long num)
{
	return num_to_string((long long) num);
}

CString string_tools::num_to_string(short num)
{
	return num_to_string((long long) num);
}

CString string_tools::num_to_string(int num)
{
	return num_to_string((long long) num);
}

CString string_tools::num_to_string(unsigned short num)
{
	return num_to_string((unsigned long long) num);
}

CString string_tools::num_to_string(unsigned int num)
{
	return num_to_string((unsigned long long) num);
}

CString string_tools::num_to_string(unsigned long num)
{
	return num_to_string((unsigned long long) num);
}

CString string_tools::num_to_string(long long num)
{
	CString strResult;
	strResult.Format(_T("%lld"), num);
	return strResult;
}

CString string_tools::num_to_string(unsigned long long num)
{
	CString strResult;
	strResult.Format(_T("%llu"), num);
	return strResult;
}

CString string_tools::num_to_string(float num, int nozero_count_after_point /*= 6*/)
{
	return num_to_string((double)num, nozero_count_after_point);
}

CString string_tools::num_to_string(double num, int nozero_count_after_point /*= 6*/)
{
	return num_to_string((long double)num, nozero_count_after_point);

}


CString string_tools::num_to_string(long double num, int nozero_count_after_point /*= 6*/)
{
	// ����long double��Ȼ��ʾ��Χ��󣬵�����
	if (nozero_count_after_point < 0 || nozero_count_after_point > 20) return _T("");

	if (0 == nozero_count_after_point)
	{
		CString zero_length_result;
		zero_length_result.Format(_T("%d"), (int) num);
		return zero_length_result;
	}

	CString fmt;
	fmt.Format(_T("%%.%dLf"), nozero_count_after_point); // ע�⣬��ʽ����LΪlong double�����Сд��Ϊdouble
	CString result;
	result.Format(fmt, num);

	result.Trim(_T('0'));

	if (result.IsEmpty()) result = _T("0");
	if (_T(".") == result.Left((int) _tcslen(_T(".")))) result = _T("0") + result;
	if (_T(".") == result.Right((int) _tcslen(_T(".")))) result.Delete(result.GetLength() - 1);
	if (result.IsEmpty()) result = _T("0"); // ֮ǰ��ɾ��������������Ҫ�ж��Ƿ�Ϊ��

	return result;
}

void string_tools::replace_chs_punctuation_marks_to_en(CString& str)
{
	// �����ַ���˫�ֽ�ģʽ��ռ�����ַ���������תΪ���ַ����滻������Ҫ�Ӻܶ��ַ�ʶ�����ϵ��߼�
	wstring str_des = CString_to_wstring(str);

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

	str = wstring_to_CString(str_des);
}

const CString string_tools::rearrange_string_blank_chars(const CString& text, const CString& split)
{
	CString result;
	rearrange_string_blank_chars(result, split);
	return result;
}

void string_tools::rearrange_string_blank_chars(CString& text, const CString& split)
{
	text.Trim(); // ɾ���ַ�����β���˵Ŀո��ַ������Ǳ���ģ����������߼��������������Խ��

	if (_T("") == text) return;
	if (_T("") == split) return;

	// ��֤�ַ����м������Ŀո�ֻ����һ��
	for (int i = 0; i < text.GetLength(); /* Nothing */)
	{
		if (_T(' ') == text[i])
		{
			// ע:ǰ���Ѿ�Trim��,���Դ�ʱ�ո���Զ��������ڵ�һ��,Ҳ������������һ��,����i-1��i+1����Խ��

			if (_T(' ') == text[i + 1]  // ��һ��Ҳ�ǿո�����Ƿָ���,��ǰ�ո�Ҫɾ��
				|| split.Find(text[i + 1]) >= 0)
			{
				text.Delete(i);
				continue; // ���������һ����䣬���ܵ�����Щ�������ַ�������ȥ�ˣ�û��ɾ���ɾ�
			}
			else if (_T(' ') == text[i - 1] // ǰһ���ǿո�(��������,������д��ȥ)����ǰһ���Ƿָ���
				|| split.Find(text[i - 1]) >= 0)
			{
				text.Delete(i);
				continue;
			}
			// ����û��else��֧�ˣ������������������������++i��
		}

		// û���κ��ַ���ɾ��ʱ����ִ����һ����䣬�����ַ���ɾ��ʱ�����൱��������
		// ��һ���ַ��������Ѿ�continue��������++i�ˡ����û����һ����䣬ѭ������
		// ������ѭ����
		++i;

	} // end of for()
}

std::vector<CString> string_tools::split_string(const CString& text, const CString& split/* = _T(" ,;") */)
{
	std::vector<CString> result;
	split_string(result, text, split);
	return result;
}

void string_tools::split_string(std::vector<CString>& result, const CString& text, const CString& split)
{
	result.clear();
	if (_T("") == text) return;

	bool need_split_by_space = (split.Find(_T(' ')) >= 0);

	CString text_copied = text;
	if (need_split_by_space)
	{
		text_copied.Trim(_T(' ')); // Ĭ��ȥ���հ��ַ����ո񣬻س����Ʊ�������˴���ȥ���ո��ַ�
		for (int i = 0; i < text_copied.GetLength(); ++i)
		{
			if (_T(' ') != text_copied[i]) continue;

			// �ѵ�ǰ�ո��ַ�����������ո��ַ�ȫ��ȥ��
			int del_count = 0;
			for (int j = i + 1; j < text_copied.GetLength(); ++j)
			{
				if (_T(' ') == text_copied[j])
					++del_count;
				else
					break;
			}

			if (del_count > 0) text_copied.Delete(i + 1, del_count);
		}
	}

	CString tmp_str;
	for (int i = 0; i < text_copied.GetLength(); ++i)
	{
		if (-1 == split.Find(text_copied[i])) // �����i���ַ����Ƿָ���
		{
			tmp_str += text_copied[i];
		}
		else
		{
			// ��һ���ַ�Ϊ�ָ�����Ϊ֮ǰ��һ���մ�������Ҳ����
			result.push_back(tmp_str);
			tmp_str = _T("");
		}
	}
	// �Ѳ����������һ���ַ����ӵ�����У��п����Ǹ��մ���
	// ���һ���ַ�Ϊ�ָ�����Ϊ������һ���մ���Ҳ�����һ���մ�
	result.push_back(tmp_str);
}

void string_tools::split_matrix_string(std::vector<std::vector<CString>>& result,
	const CString& text, TCHAR row_split /*= _T('\n')*/, TCHAR column_split /*= _T('\t')*/)
{
	result.clear();
	CString r_split_text;
	r_split_text.AppendChar(row_split);
	CString c_split_text;
	c_split_text.AppendChar(column_split);

	vector<CString> rows;
	split_string(rows, text, r_split_text);
	for (CString& row : rows)
	{
		vector<CString> columns;
		split_string(columns, row, c_split_text);
		result.push_back(columns);
	}
}

std::string wstringTostring(const std::wstring& ws)
{
	// û��ʹ��ϵͳ��API WideCharToMultiByte��������ֲ�ԽϺá�
	// �ַ����е����Ĳ������룬��Ϊ�Ѿ��л�������롣
	// ע�⣬���������㱸�쳣��ȫ�ԣ���ʱ���ٸ���֧��

	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");

	size_t szDist = 0;

	wcstombs_s(&szDist, NULL, 0, ws.c_str(), 0);
	szDist += sizeof(char); // �����ټ�һ��������(Ӧ�ÿ��Բ�Ҫ�����ò�MSDN�ˣ���һ�����׵�)

	char* pStr = new char[szDist];
	memset(pStr, 0, szDist);

	size_t szDistRes = 0;
	wcstombs_s(&szDistRes, pStr, szDist, ws.c_str(), szDist);

	pStr[szDist - 1] = '\0'; // ���һ���ֽ�Ϊ������
	std::string strResult(pStr);
	delete [] pStr;

	setlocale(LC_ALL, curLocale.c_str());

	return strResult;
}

string string_tools::CString_to_string(const CString& str)
{
#	ifdef _UNICODE
	return wstring_to_string(wstring((const wchar_t*) str));
#	else
	return string((const char*) str);
#	endif
}
 
void string_tools::merge_string(CString& result, const std::vector<CString>& all_strings,
	const CString& split /*= _T(",")*/, bool end_with_split/* = false*/)
{
	result = _T("");

	if (all_strings.empty()) return;
	if (split.IsEmpty()) return;

	result = all_strings[0];
	for (size_t i = 1; i < all_strings.size(); ++i)
		result = result + split + all_strings[i];

	if (!result.IsEmpty() && end_with_split) result += split;
}

CString string_tools::merge_string(const std::vector<CString>& all_strings,
	const CString& split /*= _T(",")*/, bool end_with_split/* = false*/)
{
	CString result;
	merge_string(result, all_strings, split, end_with_split);
	return result;
}

void string_tools::merge_matrix_string(CString& result, const std::vector<std::vector<CString>>& all_strings,
	TCHAR row_split /*= _T('\n')*/, TCHAR column_split /*= _T('\t')*/, bool end_with_split/* = false*/)
{
	result = _T("");

	for (size_t row = 0; row < all_strings.size(); ++row)
	{
		for (size_t col = 0; col < all_strings[row].size(); ++col)
		{
			result += all_strings[row][col];
			if (col + 1 < all_strings[row].size()) result.AppendChar(column_split);
		}
		
		if (row + 1 < all_strings.size()) result.AppendChar(row_split);
	}

	if (!result.IsEmpty() && end_with_split) result.AppendChar(row_split);
}

int string_tools::get_text_width_in_px(const CString& text, CDC* dc)
{
	if (!dc) return text.GetLength() * 8; // ��8�����ع�һ�����أ���Ҫֱ�ӷ���0

	CSize sz = dc->GetTextExtent(text);
	return (int) sz.cx;
}

void string_tools::generate_guid(CString& guid_text, bool need_split_char/* = true*/)
{
	guid_text = _T("");

	GUID guid; // ����guid��Ϊ�ļ���
	if (S_OK == ::CoCreateGuid(&guid))
	{
		CString guid_format;
		if (need_split_char)
			guid_format = _T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"); // 0��ʾʹ��0���
		else
			guid_format = _T("%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"); // 0��ʾʹ��0���

		TCHAR guid_char_array[40] = { _T('\0') }; // ���ٷ����ڴ澡���ٷ���
		_sntprintf_s(guid_char_array, _countof(guid_char_array) - 1, guid_format, 
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

		guid_text = guid_char_array;
	}
}

CString string_tools::generate_guid(bool need_split_char/* = true*/)
{
	CString guid;
	generate_guid(guid, need_split_char);
	return guid;
}

void string_tools::make_md5_value(const CString& text, CString& result, int count/* = 1*/)
{
	md5_value::make_md5_value(text, result, count);
}

CString string_tools::make_md5_value(const CString& text, int count/* = 1*/)
{
	CString result;
	md5_value::make_md5_value(text, result, count);
	return result;
}

bool string_tools::are_trimed_cstring_equal_no_case(const CString& first, const CString& second)
{
	CString tmp_first = first;
	tmp_first.Trim();

	CString tmp_second = second;
	tmp_second.Trim();

	return (0 == tmp_first.CompareNoCase(tmp_second));
}

bool string_tools::are_trimed_cstring_equal(const CString& first, const CString& second)
{
	CString tmp_first = first;
	tmp_first.Trim();

	CString tmp_second = second;
	tmp_second.Trim();

	return tmp_first == tmp_second;
}

CString string_tools::generate_unique_name(const std::vector<CString>& exist_names, const CString& prefix)
{
	bool is_text_exist = (exist_names.end() != find_if(exist_names.begin(), exist_names.end(), [&](const CString& item)
	{
		return !item.IsEmpty() && 0 == item.CompareNoCase(prefix);
	}));

	if (!prefix.IsEmpty() && !is_text_exist) return prefix;

	CString result_name;
	int num_suffix = 1;
	while (true)
	{
		result_name.Format("%s%d", prefix, num_suffix);

		is_text_exist = (exist_names.end() != find_if(exist_names.begin(), exist_names.end(), [&](const CString& item)
		{
			return 0 == item.CompareNoCase(result_name);
		}));

		if (!is_text_exist) return result_name;

		++num_suffix;
	}
}

