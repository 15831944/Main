#include "stdafx.h"
#include "compare_tools.h"
#include "string_tools.h"
#include "math_tools.h"
#include "muparser/include/muParser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
using namespace mu;


double math_tools::linear_interpolation(double start_x, double start_y, double end_x, double end_y, double value_x)
{
	// (x0,y0)��(x1,y1),Ҫ�õ�[x0,x1]������ĳһλ��x��ֱ���ϵ�yֵ
	// б�ʣ��� = (x - x0) / (x1 - x0)
	// �����y = y0 + ��(y1 - y0)

	// �����Ƿ��������ڣ�������������ڣ�Ϊ��֤�����ܵĻ��һ�����õ����ݣ�������㴦��Yֵ
	double dMinX = start_x;
	double dMaxX = end_x;
	if (compare_tools::is_less(dMaxX, dMinX)) swap(dMaxX, dMinX);

	if (compare_tools::is_less(value_x, dMinX)) return start_y;
	if (compare_tools::is_great(value_x, dMaxX)) return end_y;

	if (compare_tools::is_equal(end_x, start_x) || compare_tools::is_equal(end_y, start_y)) return start_y; // ��������Ƿ��˻�Ϊ�㣨��ֹ��ĸΪ0��
	if (compare_tools::is_equal(start_x, value_x)) return start_y; // ����˵㴦��ֵ���������������
	if (compare_tools::is_equal(end_x, value_x)) return end_y; // ����˵㴦��ֵ���������������

	double dA = (value_x - start_x) / (end_x - start_x);
	double dValueY = start_y + dA * (end_y - start_y);

	return dValueY;
}

int math_tools::get_great_integer(double val)
{
	if (compare_tools::is_less_equal(val, 0.0)) return 0;

	double intVal = (double) ((int) val);

	int realVal = 0;
	if (compare_tools::is_less(intVal, val))
		realVal = (int) intVal + 1; // ����ͷʱ�ټ�һ
	else
		realVal = (int) intVal;

	return realVal;
}

double math_tools::pian_da_qu_zheng(double val, double level)
{
	// 0��������������Ϊ0��ʾ��ȡ����ֱ��ԭ������ֵ
	if (compare_tools::is_less_equal(level, 0.0)) return val;

	// ��������������¼�����������⣬���û��Ҫר��дһ����ֹ��������ȡ������������㷨��
	double dDiv = ((long) (val / level)) * level;
	double dDivUp = ((long) (val / level) + 1) * level;

	return (compare_tools::is_less(dDiv, val) ? dDivUp : dDiv); // ���������ԭ������
}

double math_tools::pian_xiao_qu_zheng(double val, double level)
{
	// 0��������������Ϊ0��ʾ��ȡ����ֱ��ԭ������ֵ
	if (compare_tools::is_less_equal(level, 0.0)) return val;

	// ��������������¼�����������⣬���û��Ҫר��дһ����ֹ��������ȡ������������㷨��
	double dDiv = ((long) (val / level)) * level;
	double dDivUp = ((long) (val / level) - 1) * level;

	return (compare_tools::is_great(dDiv, val) ? dDivUp : dDiv); // ���������ԭ������
}

double math_tools::normalize_angle(double val)
{
	if (compare_tools::is_great_equal(val, -M_PI) && compare_tools::is_less_equal(val, M_PI)) return val;

	double abs_val = abs(val);
	int count = (int)(abs_val / (2.0 * M_PI));
	abs_val -= count * 2.0 * M_PI;

	if (compare_tools::is_less(val, 0.0)) abs_val = -abs_val;

	return val;
}

double math_tools::deg_to_rad(double val)
{
	return val / 180.0 * M_PI;
}

double math_tools::rad_to_deg(double val)
{
	return val / M_PI * 180.0;
}

int math_tools::get_integer_nearby_double(double val, double diff)
{
	// ǿ��ȡ���Ľ��
	double val_nearby_zero = (double) ((int) val);

	// ����Զ��0�㴦��ȡ�����
	double val_away_from_zero = val;
	if (compare_tools::is_less(val, 0.0))
		val_away_from_zero = val_nearby_zero - 1.0;
	else
		val_away_from_zero = val_nearby_zero + 1.0;

	// ����Զ��0���ȡ�������ԭ���ݵĲ�ֵ
	double diff_real = abs(val_away_from_zero - val);

	if (compare_tools::is_less_equal(diff_real, abs(diff)))
		return (int) val_away_from_zero;
	else
		return (int)val_nearby_zero;
}

bool math_tools::parse_single_multiply_item(IN const CString& exp_text, OUT vector<CString>& exp_text_items)
{
	exp_text_items.clear();
	int star_index = exp_text.Find(_T('*'));

	if (-1 == star_index)
	{
		exp_text_items.push_back(exp_text);
		return true; // û����д�ɳ˺ű��ʽҲ�ǺϷ���
	}

	CString value = _T(""); // �洢�˺ű��ʽ�е���ֵ����
	int count = 0; // �洢�˺ű��ʽ�еĸ���N����

	int exp_type = 1; // �˺ű��ʽ m*n �Ĵ���ʽ������0��ʾ�ڸ���ǰ��1��ʾ�����ں�
	if (1 == exp_type)
	{
		// 2.�����ں�������ǰ��A*n
		count = _ttoi(exp_text.Mid(star_index + 1)); // ����д��nIndexStar������ȡһ��*��
		if (count <= 0) return false;

		value = exp_text.Mid(0, star_index);
	}
	else
	{
		// 1.������ǰ�������ں�n*A
		count = _ttoi(exp_text.Mid(0, star_index + 1));
		if (count <= 0) return false;

		value = exp_text.Mid(star_index + 1);
	}

	value.Trim();
	exp_text_items.assign(count, value);

	return true;
}

bool math_tools::parse_multiply_expression(IN const CString& text, OUT vector<CString>& result)
{
	bool is_ok = true; // Ĭ��Ϊת����ȷ

	result.clear();

	// Ϊ����������ַ��������Ȱ���*�����ڵ����пո�ȫ��ɾ������Ϊ�е���ϲ��*ǰ���пո񣬶�������һ�ֱȽϳ��������
	CString text_copied = text;
	text_copied.Trim();
	for (int i = 0; i < text_copied.GetLength(); /* Nothing */)
	{
		// �ո񲻿��ܳ������ַ����׻�β����Ϊ֮ǰʹ��Trim()�����ˡ�
		// �����ط��Ŀո�Ҫ����������ܵ���*���ʽ������Ĳ�����𿪡�
		if (_T(' ') != text_copied[i]) { ++i; continue; }

		// ��������ո�Ҳɾ���������ܴ���2      *   300�����������������ѭ�������Ժ���
		// �ս�������ܳ������������ϵĿո����ڵ������
		if (_T('*') == text_copied[i - 1] || _T('*') == text_copied[i + 1]
			|| _T(' ') == text_copied[i - 1] || _T(' ') == text_copied[i + 1])
		{
			// ��ǰ�ո��ַ���ǰ���пո����*����ɾ����ǰ�ո�ɾ����䲻�ᵼ��ѭ��ʧЧ(������vector)
			text_copied.Delete(i);
			continue;
			// �˴����� ++i; ɾ��һ���ַ��൱���±���������һλ
		}

		++i; // �����٣����������ѭ��
	}

	vector<CString> vecExpress = string_tools::split_string(text_copied, _T(" ,;"));
	if (!vecExpress.empty())
	{
		for (size_t i = 0; i != vecExpress.size(); ++i)
		{
			vector<CString> vecOneExpress;
			// �������ʦ�Ŀ⺯���ڲ���ʱ���ִ���,����Բ������Ƶ�ʱԭ�������ַ���,�ҷ���ֵΪtrue
			// �����������֧��3*10 2 5�����ı��ʽ������������ʽ�г��ְ�Ƕ������޷������������
			// ��ʽΪ 100,2,300 �ͻ�ԭ����������ַ����������ڵ����������֮ǰ��Ҫ�������Լ��ĺ�����
			// �ַ���ȫ���ָ���������һ�ĵ����������ʦ�Ŀ⺯����������
			// BWDSGNMOD::ParseBWExpression()��֧������ʹ�ó˺ţ��������ַ�ʹ�ó˺ţ���12*A���������
			// ����ʹ��MessageBox��������Ž��棩��������һ�㣬�ڵ���֮ǰ�ȼ����ʽ���Ƿ������
			// ���֡�С���㡢�Լ��˺ţ������ڴ˴��Ѿ����ַ����ֽ��ˣ���Ϊһ����������һ��������ʽ����
			// ��ֻ��Ҫ����ǲ���һ�����ֳ�����һ�����֣�����������Ҫʵ�ִ˹��ܣ��Ѿ����Լ�д�������
			// û�ж�������ˣ����Է����������ʦ�ĺ���BWDSGNMOD::ParseBWExpression���Լ�дһ��
			// bool flag = BWDSGNMOD::ParseBWExpression(vecExpress[i], vecOneExpress);
			//��ע�����ʽ��2012-3-26 12:21:08��Ϊ�����ں�������ǰ��Ϊ��ֹ����ԭע�Ͳ��ģ����ڴ���ע��
			bool flag = parse_single_multiply_item(vecExpress[i], vecOneExpress);
			if (false == flag)
			{
				_ASSERTE((_T("bool flag = parse_single_multiply_item(vecExpress[i], vecOneExpress);����"), false));

				// �������ⲿר�ŵļ��ģ���Ⲣ�����Ա�ʵ����ϸ����λ���˴�����ע�͵�[WYZ 2014��3��13��20:13:36]
				// CString strErrorInfo;
				// strErrorInfo.Format(_T("�Ǽܻ�ֽ����г˺ű��ʽ��%s����ʽ��������"), vecExpress[i]);
				// //////ReportMessage(2, strErrorInfo);
				is_ok = false; // �����һ��ת�����������󣬶������أ��������Ծ����Ŭ����ת�������ַ���
			}

			if (vecOneExpress.empty()) continue;

			// ��ת������ַ���ȫ����ӵ����������
			result.insert(result.end(), vecOneExpress.begin(), vecOneExpress.end());
		}
	}

	if (result.size() > 100)
	{
		// �������ⲿר�ŵļ��ģ���Ⲣ�����Ա�ʵ����ϸ����λ���˴�����ע�͵�[WYZ 2014��3��13��20:13:36]
		// //////ReportMessage(1, _T("�ֽ����б��ʽ������Ԫ�ظ�������100������"));
		// result.resize(100);
		_ASSERTE((_T("�ֽ����б��ʽ������Ԫ�ظ�������100������"), false));
	}

	return is_ok;
}

double math_tools::calculate_expression(const CString& expression, bool var_case /*= true*/,
	const std::map<CString, double>& var_list /*= std::map<CString, double>()*/, double default_result /*= 0.0*/)
{
	if (expression.IsEmpty()) return 0;

	// muParser�������Ƿ���_UNICODE���������ַ���������unicode����multibyte������CString����ֱ�Ӹ���������ת��
	mu::Parser parser;

	for (auto& x : var_list)
	{
		if (x.first.IsEmpty()) continue;

		if (false == var_case) // �����ִ�Сд
		{
			CString uppercase_var = x.first;
			uppercase_var.MakeUpper();
			
			CString lowercase_var = x.first;
			lowercase_var.MakeLower();

			if (uppercase_var == lowercase_var) // ��������Ӣ����ĸ��������ĳ����ѧ���ţ�
			{
				parser.DefineConst(string_type(x.first), (value_type)(x.second));
			}
			else
			{
				parser.DefineConst(string_type(lowercase_var), (value_type) (x.second));
				parser.DefineConst(string_type(uppercase_var), (value_type) (x.second));
			}
		}
		else
		{
			parser.DefineConst(string_type(x.first), (value_type) (x.second));
		}
	}

	parser.SetExpr(string_type(expression));

	double result = 0.0;

	try
	{
		result = parser.Eval();
	}
	catch (...)
	{
		result = default_result;
	}
	
	return result;
}
