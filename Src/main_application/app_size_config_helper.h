#pragma once

class app_size_config_helper
{
private:
	app_size_config_helper();
	~app_size_config_helper();

public:
	// ��ȡ�������ϴα����������λ�ü��ߴ�
	static CRect get_last_main_frame_pos_and_size();
	static void save_last_main_frame_pos_and_size(const CRect pos);

	// ��ȡ�����ô����Ƿ���󻯵�״̬��Ϣ
	static bool get_maximize_flag();
	static void save_maxmize_flag(bool is_maximize);

	// ��ȡ�򱣴�ͣ����嵱ǰ����xml�İ汾��
	static void save_docking_panes_config_version(const CString& ver);
	static CString get_docking_panes_config_version();
};

