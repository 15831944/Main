#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_MATH_TOOLS_H__
#define __HMGLOBALTOOL_TOOLS_MATH_TOOLS_H__

#include <map>
#include <vector>


class X_FRAMEWORK_API math_tools
{
private:
	math_tools() = delete;
	~math_tools() = delete;

public:
	// �������Բ�ֵ����
	// dStartX	�������X
	// dStartY	�������Y
	// dEndX	�յ�����X
	// dEndY	�յ�����Y
	// dValueX	Ҫ��ѯ�ĵ�����X����������Сֵ��СʱȡСֵ�˵㴦��ֵ�������ֵ����ʱȡ��ֵ�˵㴦��ֵ�����ڶ˵�ʱֱ��ȡ�˵㴦ֵ
	static double linear_interpolation(double start_x, double start_y, double end_x, double end_y, double value_x);

	// ����һ������������һ����С�������������������
	// ���������С�ڵ���0�򷵻�0
	static int get_great_integer(double val);

	// ��һ����������ȡ���������ƫ��ȡ��,����պ������ͷ���ԭֵ�����ȡ�������Ǹ�С����ȡ������Ҳ������С��
	// ȡ������Ϊ0��ʾ��ȡ������ֻ�ܴ��ڵ���0
	// �����������Ǹ��������������������ȡ����һ�����ڳߴ��ȡ��
	static double pian_da_qu_zheng(double val, double level);

	// ��һ����������ȡ���������ƫСȡ��,����պ������ͷ���ԭֵ�����ȡ�������Ǹ�С����ȡ������Ҳ������С��
	// ȡ������Ϊ0��ʾ��ȡ������ֻ�ܴ��ڵ���0
	// �����������Ǹ�����������С�ĸ�������ȡ����һ�����ڱ��ȡ�����������͵ķ���ȡ��
	static double pian_xiao_qu_zheng(double val, double level);

	// �Ի��Ƚ��й�񻯣�����ת����-Pi��Pi��Χ��
	static double normalize_angle(double val);

	// �Ƕ�ת����
	static double deg_to_rad(double val);
	
	// ����ת�Ƕ�
	static double rad_to_deg(double val);

	// ��ָ������������ǿ��ȡ�������������ȡ��ǰ�����������������С�ڵ���diff�����λ��ȡ��
	// ��ָ��diffΪ0.1����2.1�õ�2��2.9�õ�3��2.8�õ�2��-2.2�õ�-2��-2.9�õ�-3
	// ��������ҪΪ�˽������һЩʵ�ʹ����о��������ɵĸ�����1����Ϊ
	static int get_integer_nearby_double(double val, double diff);

	// �����˺ű��ʽ���˺������ܽ���һ���������������һ�����������������磺
	// 3*2 �� 6.5*3 �� 8.0*5.5 �� -3*8
	// �˺�����Գ˺�ǰ������ݽ�������ת������ǿ��Ҫ��ֻ��ת�����������˺ź�����������
	// ��ʽ���⣬����������Trim��ԭ�������������ж���˺ŵ���
	// ����ת����ʱ���ʶ���һ���˺š�������ֵΪfalse��ʱ���Ǿ�������ת��������е���
	// �������ܲ���һ������Ҫ��ʹ���߾͵�ע�⡣
	// ���*ǰ�������ǿ��ת����Ϊ0������vector�н����������ַ�����Ϊ����
	// ���*���������Trim��Ϊ���ַ���������vector�д���ָ����Ŀ�Ŀ��ַ���
	// ����Ϊ��ȷʾ������Ӧ����������
	// 100.2*3		->	100.2    100.2    100.2
	// A*5			->	A    A    A    A    A
	//��ע�����ʽ��2012-3-26 12:21:08��Ϊ�����ں�������ǰ��Ϊ��ֹ����ԭע�Ͳ��ģ����ڴ���ע��
	static bool parse_single_multiply_item(IN const CString& exp_text, OUT std::vector<CString>& exp_text_items);

	// �����ַ������ʽ�������ַ������տո񡢶��š��ֺ�����ֿ�������һ�������ʽ
	// Ҫ�����ı��ʽ��ֻ���������ַ����ո񡢶��š��ֺš��˺š�С���㣬������ǳ˺ű��ʽ��
	// ��ʾ���ݵĿ������ַ������ַ�����ʾ��������ֻ��������������ᱻǿ������ʾ����
	// 100 0 �����100 0
	// 100,0 �����100 0
	// 100*3 �����100 100 100
	// A*3	 �����A A A
	// AB*3	 �����AB AB AB
	// 10 2*10 5 �����10 10 10 5
	// ����ո���һ��ʹ�ã���β���˵Ŀո񱻺���
	// �ո�ͷָ������ڣ�����Կո�
	// �ո�ͳ˺����ڣ�����Կո�
	// һ��Ҫʹ��֮ǰ�����GWFormatString��ʽ��һ���ַ���������һЩ��ʽ������ַ������Ա�������ȷ
	//��ע�����ʽ��2012-3-26 12:21:08��Ϊ�����ں�������ǰ��Ϊ��ֹ����ԭע�Ͳ��ģ����ڴ���ע��
	// ����ֻ��Ϊ��������ֵ�������С�����������ݸ�ʽ�ڲ�Ҳ�������ܽ�����ȷ�����򲻻��쳣��������֤�����ȷ
	static bool parse_multiply_expression(IN const CString& text, OUT std::vector<CString>& result);

	// �������ݱ��ʽ��ֵ
	// �����������򷵻�default_result
	// ������ʽ�в�����������var_list���Բ��ô�,��3+(5-6)*2
	// ������������ִ�Сд����var_caseΪfalse��������Ϊtrue
	static double calculate_expression(	const CString& expression, bool var_case = true,
		const std::map<CString, double>& var_list = std::map<CString, double>(), double default_result = 0.0);
};

#endif //__HMGLOBALTOOL_TOOLS_MATH_TOOLS_H__
