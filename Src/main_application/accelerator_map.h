#pragma once


// �洢 ����ݼ�-��� ��ӳ���ϵ
class accelerator_map
{
public:
	accelerator_map();
	~accelerator_map();

public:
	void create_accelerator_table_by_xml();
	bool is_accelerator_table_empty() const;
	HACCEL get_accelerator_handle() const;
	CString get_accelerator_command_by_id(DWORD id);

private:
	HACCEL m_accelerators_handle; // �����ļ��ټ�����
	std::map<int, CString> m_db_accelerator_map; // �������ټ�����id�������ӳ���

private:
	accelerator_map(const accelerator_map& mp) = delete;
};

