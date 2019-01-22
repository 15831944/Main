#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_MINI_FILE_LOG_H__	
#define __HMGLOBALTOOL_TOOLS_MINI_FILE_LOG_H__

#include <fstream>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
// û�����������벻Ҫʹ�ô��͵���־���߿⣬��log4cpp��boost����־���ߵ�,�򵥵Ķ���
// ��ά���Ժ����������Ǳ�����õ���ʽ[wyz]
//////////////////////////////////////////////////////////////////////////

// һ�����Ⱦ���Ķ��̰߳�ȫ�ļ���־��¼�࣬������֧���ı���ʽ
// ��־���Ƿ�ʹ�õ���ģʽ��ʹ�������о������������಻��ǿ���Ե�Ҫ��
// �ļ��ڹ�����Զ��򿪣�ֱ������ʱ�Źرգ��Ա�����־���ʱÿ�δ򿪽����ļ�Ѱַ�����������ٶ�
// Ϊȷ���ļ��ڳ����쳣ʱ�ܼ�¼�����µ���־�ұ�֤�ļ����𻵣�ÿ�����ʱ���Զ�ǿ�ƽ�������������ˢ���ļ���
// ���������������ڽ�����־��¼�������ڲ�˽�õ�С���ߣ�������
class HM_GlobalTool_EXT mini_file_log
{
public:
	mini_file_log(); // ����Ҫ�ٵ���set_file_path
	mini_file_log(const CString& file_full_path); // �ڲ��Ѿ�������set_file_path
	~mini_file_log();
	mini_file_log(const mini_file_log&) = delete;
	mini_file_log(mini_file_log&&) = delete;
	mini_file_log& operator=(const mini_file_log&) = delete;

public:
	// ��־�ļ����ȹر����µ�·���򿪣������ڳ���ִ�й����е��ô˽ӿ���ʱ�ı����λ��
	// ע�⣺·���б�������ļ���
	void set_file_path(const CString& file_full_path_name);

	// ���ļ�βʵʱ׷���ı���¼���ҵ�ǰ�ı����Զ����������ʱ��
	// Ϊȷ�����ʱ��Ч�ʣ������ݴ��Լ�飬�����ļ��Ƿ�򿪵ȣ�ʹ��ʱ��ȷ���ⲿ�����Ѿ��㱸
	void output_text(const CString& text);

private:
	std::ofstream m_output_fs;
	CRITICAL_SECTION m_cs_lock; // ʹ���ٽ�����Ϊ���̷߳��ʿ��Ʊ�mutexҪʡ��Դ����־�⾡����Ч
};

#endif //__HMGLOBALTOOL_TOOLS_MINI_FILE_LOG_H__
