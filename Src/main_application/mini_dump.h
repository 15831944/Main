#ifndef DB_MINI_DUMP_H_
#define DB_MINI_DUMP_H_


typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS;

class mini_dump_helper 
{
private:
	mini_dump_helper();
	~mini_dump_helper();

public: 
	static bool m_need_popup_report_window; // �Ƿ񵯳���Ϣ��,Ĭ�ϲ�����������ʱ���д���ֱ�Ӳ���ʾ���˳���ӦС����ʦҪ�󣩣�������WinMain�����Ƶĵط���Ϊtrue
	static void enable_dump(bool popup_report_window = false); // �ڳ����ʼ��ʱ����һ�μ���
	
private:
	static LONG WINAPI exception_top_level_filter(EXCEPTION_POINTERS* ex_point);
};

#endif // DB_MINI_DUMP_H_