#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_FILE_TOOLS_H__
#define __HMGLOBALTOOL_TOOLS_FILE_TOOLS_H__	

class HM_GlobalTool_EXT file_tools
{
private:
	file_tools();
	~file_tools();

public:
	// ��ȡӦ�ó���ĵ�ǰ����·��,������β��б��
	static CString get_app_path();

	// ��ȡӦ�ó���ĵ�ǰ����·��������β��б�ܣ�
	static CString get_app_path_with_xg();

	// ��ȡx_framework���dll����·������ʱ��������Ϳ�ܲ�����ͬһĿ¼�����絥Ԫ���Ի����ļ���,������β��б��
	static CString get_x_framework_path();
	
	// ��ȡx_framework���dll����·������ʱ��������Ϳ�ܲ�����ͬһĿ¼�����絥Ԫ���Ի����ļ���,����β��б��
	static CString get_x_framework_path_with_xg();

	// ��ȡһ���ļ�·���е���Ϣ������Ŀ¼���ļ�������׺���ӿ�֧�����·����·����Ϣ��ȫʱ���ᾡ����ȡ�����ܶ����Ϣ
	// "C:\\test\\1.txt"	���� "C:\\test"  "1"  "txt"
	// "C:\\test\\.txt"		���� "C:\\test"  ""  "txt"
	// "C:\\test\\1"		���� "C:\\test"  "1"  ""
	// "C:\\"				���� "C:"  "1"  "txt"
	// "C:"					���� "C:"  "1"  "txt"
	// "C"					���� ""  "C"  ""
	static void get_file_path_info(CString& path, CString& file_name, CString& suffix, const CString& full_path_name);
	static void get_file_path_info(CString& path, CString& file_name_with_suffix, const CString& full_path_name);
	
	// ���ļ�·���ַ�������ȡ�ļ�����������׺����֧�����·��
	// "C:\\test\\1.txt"����"1"
	static CString get_name_from_file_path_name(const CString& path_name);

	// ���ļ�·���ַ�������ȡ�ļ����������ļ�������׺����֧�����·��
	// "C:\\test\\1.txt"����"1.txt"
	static CString get_full_name_from_file_path_name(const CString& path_name);

	// ���ļ�·���ַ�������ȡ��׺��������ţ�֧�����·��
	// "C:\\test\\1.txt"����"txt"
	static CString get_ext_from_file_path_name(const CString& path_name);

	// ���ļ�·���ַ�������ȡ·������β������б�ܣ�֧�����·��
	// "C:\\test\\1.txt"����"C:\\test"
	// "C:\\test\\www"����"C:\\test"(www��Ϊ���ļ��������޺�׺)
	// "C:\\test\\www\\"����"C:\\test\\www"
	static CString get_path_from_file_path_name(const CString& path_name);

	// ���ļ�·���ַ�������ȡ·������β����б�ܣ�֧�����·��
	// "C:\\test\\1.txt"����"C:\\test\\"
	// "C:\\test\\www"����"C:\\test\\"(www��Ϊ���ļ��������޺�׺)
	// "C:\\test\\www\\"����"C:\\test\\www\\"
	static CString get_path_from_file_path_name_with_xg(const CString& path_name);

	// ��ȡָ��·���µ�����ָ����׺���ļ�ȫ·�������������ļ���
	// ע�⣺�����������files�е�����
	// path�����Ǿ���·����Ҳ���������·������β��б�ܿ��Բ�ҪҲ���Լ��ϣ�·��ֻ�����ļ���·��
	// suffixָ���ļ���׺��������ţ����Ϊ�ձ�ʾ����û�к�׺���ļ���
	// include_sub_dirΪtrue��ʾ�ݹ�������ļ��У�����ֻ��ָ���ļ����µ��ļ�
	static void get_all_files_full_path_in_dir(std::vector<CString>& files, const CString& path, const CString& suffix, bool include_sub_dir = true);

	// �Ƿ������·��
	static bool is_relative_path(const CString& path);

	// ָ��·���Ƿ������Ϊ�ļ�(�ļ�����û�к�׺)
	static bool is_file_path(const CString& path);

	// ָ��·���Ƿ������Ϊ�ļ���
	static bool is_folder_path(const CString& path);

	// ָ����·���Ƿ��Ѿ����ڣ��������ļ���·����Ҳ�������ļ�·����
	static bool is_path_or_file_exist(const CString& path);

	// Ƕ�״����ļ��У�ָ������·������������ڻἶ����������·���е��ļ��У����ɹ�ִ�з���true�����򷵻�false
	static bool create_directory_nested(const CString& dir_path);

	// Ƕ�״����ļ�·�����ļ���ָ������·�����ļ������·�������ڻἶ����������·���е��ļ��У�����ٴ����ļ������ɹ�ִ�з���true�����򷵻�false
	// overrideΪtrue�򸲸�ͬ·���µ�ͬ���ļ������򲻸����ļ�
	static bool create_file_nested(const CString& file_path, bool override = true);

	// ɾ���ļ����ļ��У�·��������Ҳ��Ϊ��ɾ���ɹ�
	// ��������ȫ��ִ�гɹ�����true,���򷵻�false
	// ĳһ�ļ�ɾ��ʧ��ʱ�����ɾ�������ļ����ļ���
	// del_selfΪtrue��ʾ���·��Ϊ�ļ��У�����ͬ����Ҳɾ���������ɾ���ļ����ڲ����ļ������ļ���
	static bool delete_file_or_directory(const CString& dir_path, bool del_self = false);

	// ɾ��ָ�����ļ��У��ļ����ڲ����ļ������ļ���Ҳɾ����
	// del_selfΪtrue��ʾ��ͬ����Ҳɾ���������ɾ���ļ����ڲ����ļ������ļ���
	static bool delete_directory(const CString& dir_path, bool del_self = false);

	// ɾ��ָ�����ļ�
	static bool delete_file(const CString& file_path);

public:
	/////////////////////////////////�ѷ����Ľӿ�/////////////////////////////////////////

	// �ӿ��Ѿ������������delete_file_or_directory()
	// ��������ļ��е�����(�������ļ���), ���������ļ���
	// ·��������Ҳ��Ϊ��ɾ���ɹ�,��������ȫ��ִ�гɹ�����true,���򷵻�false
	static bool delete_files_and_sub_directory(const CString& dir_path);
};

#endif //__HMGLOBALTOOL_TOOLS_FILE_TOOLS_H__
