//  *********************************************************************
//  * Base 64 de- and encoding class                                    *
//  *                                                                   *
//  * ================================================================= *
//  * fixed decode bug while encode include CRLF on 07.10.2017          *
//  * added encode with CRLF flag on 07.10.2017                         *
//  * by Wang You-zhou [wyz5@163.com]                                   *
//  *                                                                   *
//  * ================================================================= *
//  * This class was wrapped on 28.03.2017                              *
//  * by Wang You-zhou [wyz5@163.com]                                   *
//  *                                                                   *
//  * ================================================================= *
//  * This class was written on 28.05.2003                              *
//  * by Jan Raddatz [jan-raddatz@web.de]                               *
//  *                                                                   *
//  * ================================================================= *
//  * Copyright (c) by Jan Raddatz                                      *
//  * This class was published @ codeguru.com                           *
//  * 28.05.2003                                                        *
//  *                                                                   *
//  *********************************************************************

#pragma once  
#ifndef __HMGLOBALTOOL_TOOLS_BASE64_TOOLS_H__
#define __HMGLOBALTOOL_TOOLS_BASE64_TOOLS_H__

#include <vector>

class X_FRAMEWORK_API base64_tools
{
private:
	base64_tools() = delete;
	~base64_tools() = delete;

public:
	// �������뻺�������ݣ����롢��������������ⲿԤ�ȴ����ã���ȷ���ռ��㹻��
	// �����Ƿ��صĻ���������������һ���ֽڲ����Ϊ'\0'
	// base64��������Щ������ͨ�ţ�������Ӧ����ҵ��׼�޶����볬��76���ַ�ʱҪ���У�����"\r\n"�����Ա�֤ÿ�е�
	// �ַ�������76���ַ������������������ͨ��auto_wrapΪ���ã����Ϊtrue��ʾ���Զ����У����򲻲��뻻��
	// ע�⣺����ʱ��ȥ������base64�����ַ�����س������еȣ�����˽����ʵ���õ��Ļ�������ֽ������ܻ�ȵ�
	//      ��calculate_decode_output_buffer_in_byte����������٣����Ҫ����ʵ�ʽ���õ����ֽ���������ʱ��
	//      ��õ��Ŀռ��С����ʵ�ģ���˲���Ҫ�������յĽ����
	static void encode_buffer(const char* input_buffer, unsigned int input_buffer_count_in_byte, char* output_buffer, bool auto_wrap/* = false*/);
	static unsigned int decode_buffer(const char* input_buffer, unsigned int input_buffer_count_in_byte, char* output_buffer);

	// ���������������Ļ��������ڴ��ɽ����߸������(����ʱ��Ҫ�������ݲ��ܼ������軺�����ֽ���)
	// ע�⣬�˴���������Ľ�����ܱ�ʵ�ʽ����ĳ��Ȼ�Ҫ����Ϊ�˴�ֻ�ǰ������㷨���������ĳ��ȣ�ʵ�ʽ���ʱ��Ҫ����ȥ���Ƿ��ַ��Ȳ���
	static unsigned int calculate_encode_output_buffer_in_byte(unsigned int input_buffer_count_in_byte, bool auto_wrap = false);
	static unsigned int calculate_decode_output_buffer_in_byte(const char* input_buffer, unsigned int input_buffer_count_in_byte);

	// �ڴ����ݰ�base64���б��룬��base64�������Ϊ�ڴ�����
	// ����base64���벻���ڶ����ַ���unicode����������⣬��˲�unicodeģʽ��ǿ���Ƽ�ʹ��std::string��Ϊ������
	// ������ͨ���ַ������������Խ�ʡһ����ڴ�
	static void encode_buffer_to_string(const char* input_buffer, unsigned int input_buffer_count_in_byte, CString& output_text, bool auto_wrap = false);
	static void encode_buffer_to_string(const char* input_buffer, unsigned int input_buffer_count_in_byte, std::string& output_text, bool auto_wrap = false);
	static void encode_buffer_to_string(const std::vector<char>& input_buffer, CString& output_text, bool auto_wrap = false);
	static void encode_buffer_to_string(const std::vector<char>& input_buffer, std::string& output_text, bool auto_wrap = false);
	static void decode_string_to_buffer(const CString& input_text, std::vector<char>& output_buffer);
	static void decode_string_to_buffer(const std::string& input_text, std::vector<char>& output_buffer);
};
#endif //__HMGLOBALTOOL_TOOLS_BASE64_TOOLS_H__
