#pragma once

class CTHDataBase;

// �����ڸ���xml�������ݼ��س���ȫ�����ò���
class app_configs
{
private:
	app_configs() = delete;
	~app_configs() = delete;

public:
	//�˽ӿ��ɿ�ܵ���
	static void load_settings_from_xml();

	// �Ƿ�����Ƥ��
	static bool is_skin_enable();

public:
	static bool m_enable_splash; // �Ƿ���ʾ��������
	static bool m_enable_skin; // �Ƿ�����Ƥ��
	static CString m_image_name; //��������ͼƬ����
	static CString m_application_name; // Ӧ�ó�������
	static bool m_show_start_page; // ��������ʱ�Ƿ���ʾ��ʼҳ���������������򲻴��ļ����������������˫��ֱ��������
	static bool m_only_one_instance; // �Ƿ�ֻ����һ������ʵ��
};
