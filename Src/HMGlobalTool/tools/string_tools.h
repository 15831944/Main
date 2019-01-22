#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_STRING_TOOLS_H__	
#define __HMGLOBALTOOL_TOOLS_STRING_TOOLS_H__

#include <time.h>
#include <string>
#include <vector>
#include <tchar.h>

class HM_GlobalTool_EXT string_tools
{
private:
	string_tools() = delete;
	~string_tools() = delete;

public:
	// ��ʽ��������һ��CString������Ϊ��CString��Format����һ�£���ʽ����ϸ����printf���ʽ���Ʒ�(��ʽ��������ó���1K byte)
	static CString format(const TCHAR* format, ...);

	// ��ʽ����ת��ʱ�����֧�ֵĸ�ʽ���о����£�ע�ⲻҪ���ַ���ת���ĸ�ʽ������ˣ���
	// %a  ���ڼ��ļ�д
	// %A  ���ڼ���ȫ��
	// %b  �·ֵļ�д
	// %B  �·ݵ�ȫ��
	// %c  ��׼�����ڵ�ʱ�䴮
	// %C  ��ݵĺ���λ����
	// %d  ʮ���Ʊ�ʾ��ÿ�µĵڼ���
	// %D  �� / �� / ��
	// %e  �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
	// %F  �� - �� - ��
	// %g  ��ݵĺ���λ���֣�ʹ�û����ܵ���
	// %G  ��֣�ʹ�û����ܵ���
	// %h  ��д���·���
	// %H  24Сʱ�Ƶ�Сʱ
	// %I  12Сʱ�Ƶ�Сʱ
	// %j  ʮ���Ʊ�ʾ��ÿ��ĵڼ���
	// %m  ʮ���Ʊ�ʾ���·�
	// %M  ʮʱ�Ʊ�ʾ�ķ�����
	// %n  ���з�
	// %p  ���ص�AM��PM�ĵȼ���ʾ
	// %r  12Сʱ��ʱ��
	// %R  ��ʾСʱ�ͷ��ӣ�hh : mm
	// %S  ʮ���Ƶ�����
	// %t  ˮƽ�Ʊ��
	// %T  ��ʾʱ���룺hh : mm : ss
	// %u  ÿ�ܵĵڼ��죬����һΪ��һ�죨ֵ��0��6������һΪ0��
	// %U  ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
	// %V  ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
	// %w  ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
	// %W  ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
	// %x  ��׼�����ڴ�
	// %X  ��׼��ʱ�䴮
	// %y  �������͵�ʮ������ݣ�ֵ��0��99��
	// %Y  �����Ͳ��ֵ�ʮ�����
	// %z  ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
	// %Z  ͬ%z
	// %%  �ٷֺ�
	// ע�⣺�������ʹ�õ������յģ�����0��ʼ������
	//      ��ʽ��������ó���1K byte
	//      ��ʽ����������������Ǹ�ʽ�ַ���ԭ���������"%Y��%m��%d��"
	static CString time_format(const TCHAR* format, int y, int m, int d, int hr = 0, int mi = 0, int se = 0); // ��ʵ������ʱ
	static CString time_format(const TCHAR* format, const tm& t);
	static CString time_format(const TCHAR* format, time_t t); // ʹ��ʱ�����ʽ��
	static CString time_format(const TCHAR* format); // ʹ�õ�ǰϵͳʱ��ı���ʱ����и�ʽ�������Ǹ������α�׼ʱ�䣩
	static time_t convert_time_stamp(const TCHAR* format, const TCHAR* t);
	static time_t convert_time_stamp(const tm& t);
	static time_t convert_time_stamp(int y, int m, int d, int hr = 0, int mi = 0, int se = 0); // ��ʵ������ʱ
	static time_t convert_time_stamp(); // ʹ�õ�ǰϵͳʱ�����ת�������Ǹ������α�׼ʱ�䣩
	static tm convert_time(const TCHAR* format, const TCHAR* t);
	static tm convert_time(time_t t);
	static tm convert_time(int y, int m, int d, int hr = 0, int mi = 0, int se = 0); // ��ʵ������ʱ
	static tm convert_time(); // ʹ�õ�ǰϵͳʱ�����ת�������Ǹ������α�׼ʱ�䣩

	// �ַ���ת��
	// ע�⣺
	//   ���Եĺ������е� string��Ϊmulti-byte �ַ�����utf8��Ϊ utf-8 ��ʽ��string��������������һ�������ڲ����벻һ��
	//   wstring ��Ϊunicode�ַ���������Ϊ utf-16 ��ʽ
	//   CString ����Ŀ���õ��ַ�������أ��൱�� multi-byte �� string �� utf-16 �� wstring
	static std::string wstring_to_string(const std::wstring& str);
	static std::string CString_to_string(const CString& str);
	static std::string utf8_to_string(const std::string& str);
	static std::wstring string_to_wstring(const std::string& str);
	static std::wstring CString_to_wstring(const CString& str);
	static std::wstring utf8_to_wstring(const std::string& str);
	static CString wstring_to_CString(const std::wstring& str);
	static CString string_to_CString(const std::string& str);
	static CString utf8_to_CString(const std::string& str);
	static std::string string_to_utf8(const std::string& str);
	static std::string wstring_to_utf8(const std::wstring& str);
	static std::string CString_to_utf8(const CString& str);

	// ������תΪ�ַ����������С�����Ტ��β�������0ȥ��,����6С��
	static CString num_to_string(short num);
	static CString num_to_string(int num);
	static CString num_to_string(long num);
	static CString num_to_string(long long num);
	static CString num_to_string(unsigned short num);
	static CString num_to_string(unsigned int num);
	static CString num_to_string(unsigned long num);
	static CString num_to_string(unsigned long long num);
	static CString num_to_string(float num, int nozero_count_after_point = 6); // �����С������ĩβ��0ȥ����nozero_count_after_point��ʾС���������������󳤶ȣ����20λ���������봦��
	static CString num_to_string(double num, int nozero_count_after_point = 6); // �����С������ĩβ��0ȥ����nozero_count_after_point��ʾС���������������󳤶ȣ����20λ���������봦��
	static CString num_to_string(long double num, int nozero_count_after_point = 6); // �����С������ĩβ��0ȥ����nozero_count_after_point��ʾС���������������󳤶ȣ����20λ���������봦��

	// ���ַ����е����Ķ��š���š��ֺš�����С���š����������š�ȫ�ǿո�ȫ������Ӣ�İ���ַ�
	static void replace_chs_punctuation_marks_to_en(CString& str);

	// ��ʽ���ַ�����������¹���:
	// 1.���ַ�������β���˵İ�ǿհ��ַ����ո񡢻س����Ʊ����ȫ��ɾ��
	// 2.�ַ����м�����������ڵİ�ǿո������һ��
	// 3.�������������������ո��ַ���ָ��ַ�����ʱɾ����Щ�ո�
	// 4.�ָ��ַ���split���κ��ַ�����һ����Ч�ķָ���
	// 5.�ָ���split�г��ֿո��㷨���߼�Ҳ��Ȼ����ȷ��
	static const CString rearrange_string_blank_chars(const CString& text, const CString& split);
	static void rearrange_string_blank_chars(CString& text, const CString& split);

	// ���ַ�����ָ�����ַ����е������ַ�����ֲ����ؽ����Ĭ�Ϸָ���Ϊ�ո񶺺ŷֺţ��ָ�����������ʱ��������ʹ�����ô���
	// ע�⣺��������Ŀո������Ϊ�ָ������ֻ����һ����������β�Ŀո�����ȥ���������ָ�����ԭ��������ָ���ո���
	// Ϊ�ָ���ʱ����ȥ���κοո��ַ�
	static std::vector<CString> split_string(const CString& text, const CString& split = _T(" ,;"));
	static void split_string(std::vector<CString>& result, const CString& text, const CString& split = _T(" ,;"));
	static void split_matrix_string(std::vector<std::vector<CString>>& result, const CString& text,
		TCHAR row_split = _T('\n'), TCHAR column_split = _T('\t'));
	 
	// �ϲ��ַ���
	// all_strings ���ϲ����ַ�����split �ָ�����result �ϲ��õ��ַ���
	// end_with_split��ʾβ���Զ��Ӵ���һ��split�ָ���������Ҫ���ϲ�Ϊ���б�ʱ���������һ�е����һ��ĩβ�Ƿ�����зָ�����
	// �ϲ�������Ϊ�ղ����ڽ�β��ӷָ���
	static void merge_string(CString& result, const std::vector<CString>& all_strings, const CString& split = _T(","), bool end_with_split = false);
	static CString merge_string(const std::vector<CString>& all_strings, const CString& split = _T(","), bool end_with_split = false);
	static void merge_matrix_string(CString& result, const std::vector<std::vector<CString>>& all_strings,
		TCHAR row_split = _T('\n'), TCHAR column_split = _T('\t'), bool end_with_split = false);

	// ����dc�����ַ����Ļ��ƿ�ȣ�������Ϊ��λ��
	static int get_text_width_in_px(const CString& text, CDC* dc);

	// ����guid�ַ�����need_split_charΪtrue��ʾ���ָ�����ֻ��Ϊ��-���ţ���������£�
	// ���ָ�����ʽΪ��66C78769-C996-488A-AA8F-3CB806782FE6������Ϊ36
	// �޷ָ�����ʽΪ��66C78769C996488AAA8F3CB806782FE6������Ϊ32
	static void generate_guid(CString& guid_text, bool need_split_char = true);
	static CString generate_guid(bool need_split_char = true);

	// ��һ���ַ�������MD5���ܣ�����޷�������ɼ��ܣ��򷵻ؿ��ַ�����count Ϊ���ܴ���
	// ������ԭ�ַ�����Ϊ��ֹ����ⷵ��ֵ���������������й©
	static void make_md5_value(const CString& text, CString& result, int count = 1);
	static CString make_md5_value(const CString& text, int count = 1);

	// �Ƚ�����ȥ�հ��ַ����ֵ�Ƿ���ȣ������ִ�Сд
	static bool are_trimed_cstring_equal_no_case(const CString& first, const CString& second);

	// �Ƚ�����ȥ�հ��ַ����ֵ�Ƿ���ȣ����ִ�Сд
	static bool are_trimed_cstring_equal(const CString& first, const CString& second);

	// ����ָ�����Ѿ����ڵ��ַ����б���ָ��������Ϊǰ׺���ں�������»��ߺ����֣�����һ�������������Ҳ��ظ����ַ�������
	static CString generate_unique_name(const std::vector<CString>& exist_names, const CString& prefix);
};

#endif //__HMGLOBALTOOL_TOOLS_STRING_TOOLS_H__
