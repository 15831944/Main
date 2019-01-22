#include "stdafx.h"
#include <string>
#include <vector>
#include <io.h>

#include "file_tools.h"

 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


file_tools::file_tools()
{
}


file_tools::~file_tools()
{
}

CString file_tools::get_app_path()
{
	TCHAR path_buffer[MAX_PATH];
	::GetModuleFileName(NULL, path_buffer, MAX_PATH);

	CString path, name;
	get_file_path_info(path, name, path_buffer);
	return path;
}

CString file_tools::get_app_path_with_xg()
{
	CString path = get_app_path();
	if (!path.IsEmpty()) path += _T('\\');
	return path;
}

CString file_tools::get_x_framework_path()
{
	extern HINSTANCE g_resource_handle;

	TCHAR path_buffer[MAX_PATH];
	::GetModuleFileName((HMODULE)g_resource_handle, path_buffer, MAX_PATH);

	CString path, name;
	get_file_path_info(path, name, path_buffer);
	return path;
}

CString file_tools::get_x_framework_path_with_xg()
{
	CString path = get_x_framework_path();
	if (!path.IsEmpty()) path += _T('\\');
	return path;
}

void file_tools::get_file_path_info(CString& path, CString& file_name, CString& suffix, const CString& full_path_name)
{
	path = _T("");
	file_name = _T("");
	suffix = _T("");

	// �ȷֽ�·�����ļ���
	CString only_name_with_sufix;
	get_file_path_info(path, only_name_with_sufix, full_path_name);

	// �ٷֽ��ļ����ͺ�׺
	if (only_name_with_sufix.IsEmpty()) return;

	int pos = only_name_with_sufix.ReverseFind(_T('.'));
	if (pos >= 0)
	{
		file_name = only_name_with_sufix.Left(pos); // ��Ҫ'.'
		suffix = only_name_with_sufix.Mid(pos + 1);
	}
	else
	{
		file_name = only_name_with_sufix;
		suffix = _T("");
	}
}

void file_tools::get_file_path_info(CString& path, CString& file_name_with_suffix, const CString& full_path_name)
{
	path = _T("");
	file_name_with_suffix = _T("");

	int tmp_pos_01 = full_path_name.ReverseFind(_T('\\'));
	int tmp_pos_02 = full_path_name.ReverseFind(_T('/'));
	int pos = max(tmp_pos_01, tmp_pos_02);
	if (pos >= 0)
	{
		file_name_with_suffix = full_path_name.Mid(pos + 1);
		path = full_path_name.Left(pos); // ��Ҫ'\\'
		return;
	}

	pos = full_path_name.ReverseFind(_T(':'));
	if (pos >= 0)
	{
		file_name_with_suffix = full_path_name.Mid(pos + 1);
		path = full_path_name.Left(pos + 1); // �����������ð�ű���Ҫ���ǺϷ���·��
	}
	else
	{
		file_name_with_suffix = full_path_name;
		path = _T("");
	}
}

CString file_tools::get_name_from_file_path_name(const CString& path_name)
{
	CString path, name, suffix;
	get_file_path_info(path, name, suffix, path_name);
	return name;
}

CString file_tools::get_full_name_from_file_path_name(const CString& path_name)
{
	CString path, name;
	get_file_path_info(path, name, path_name);
	return name;
}

CString file_tools::get_ext_from_file_path_name(const CString& path_name)
{
	CString path, name, suffix;
	get_file_path_info(path, name, suffix, path_name);
	return suffix;
}

CString file_tools::get_path_from_file_path_name(const CString& path_name)
{
	CString path, name;
	get_file_path_info(path, name, path_name);
	return path;
}

CString file_tools::get_path_from_file_path_name_with_xg(const CString& path_name)
{
	CString path = get_path_from_file_path_name(path_name);
	if (!path.IsEmpty()) path += _T("\\");
	return path;
}

void file_tools::get_all_files_full_path_in_dir(vector<CString>& files, const CString& path, const CString& suffix, bool include_sub_dir /*= true*/)
{
	CString full_path = path;
	if (full_path.Right(1) == _T("\\") || full_path.Right(1) == _T("/"))
		full_path.Delete(full_path.GetLength() - 1);

	if (full_path.IsEmpty()) full_path = get_app_path();

	// ʹ��C++�⹦�ܱ����ļ���������ϵͳAPI����ǿ�������ֲ��

	struct _tfinddata_t fileFinder;
	intptr_t handle = _tfindfirst(path + _T("\\*.*"), &fileFinder); // ����ָ��Ϊ*.txt���ִ���׺����ʽ�������޷�����
	if (-1 == handle) return;

	// ��һ���ļ����ô�����Ϊ��һ���϶���"."������ʾ".\"��ǰĿ¼
	// �ڶ�Ҳ���ô����϶���".."����"..\"��һ��Ŀ¼��ֻҪ��������
	do
	{
		CString curr_file_name(fileFinder.name);
		if (_T(".") == curr_file_name) continue;
		if (_T("..") == curr_file_name) continue;

		// ��Ŀ¼��ݹ������Ŀ¼��������д��_A_SUBDIR == fileFinder.attrib
		if ((_A_SUBDIR & fileFinder.attrib) && include_sub_dir)
			get_all_files_full_path_in_dir(files, path + _T("\\") + curr_file_name, suffix, include_sub_dir);

		int dot_pos = curr_file_name.ReverseFind(_T('.'));
		if (-1 == dot_pos && !suffix.IsEmpty()) continue;

		if (suffix.IsEmpty())
		{
			files.push_back(path + _T("\\") + curr_file_name);
		}
		else
		{
			CString curr_file_suffix = curr_file_name.Mid(dot_pos + 1); // ��Ҫ.��
			if (0 == curr_file_suffix.CompareNoCase(suffix) && curr_file_name.GetLength() > suffix.GetLength()) // ��ֹ�ļ���Ϊ�գ�ֻ�к�׺
				files.push_back(path + _T("\\") + curr_file_name);
		}

	} while (0 == _tfindnext(handle, &fileFinder));

	_findclose(handle);
}

bool file_tools::is_relative_path(const CString& path)
{
	return (-1 == path.Find(_T(':'))); // û��:����Ϊ�����·��
}

bool file_tools::is_file_path(const CString& path)
{
	if (FALSE == PathFileExists(path)) return false;
	
	return (false == is_folder_path(path));
}

bool file_tools::is_folder_path(const CString& path)
{
	if (FALSE == PathFileExists(path)) return false;

	// ��API����PathIsDirectory()Ҳ��һ����ԭ��
	return (0 != (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(path)));
}

bool file_tools::is_path_or_file_exist(const CString& path)
{
	// �ܶ��˲�֪�����������ʵֱ�Ӿ���API�ӿڿ��Ե����е������ļ����Ƿ�ɹ����жϣ��е����ļ�������������
	// ��ʽ���жϣ����������廨���ţ�����������Ч�ʺ������ԣ���˿�ܰ�װһ�·��ڹ��ߺ�����
	return TRUE == ::PathFileExists(path);
}

// ֻ�Ǵ�����ȫ·�����ļ�Ŀ¼��������ļ���û�д���
// ע�⣬�����·���������ļ���·�����������ļ�·�������ұ����Ǿ���·����������ܴ���
bool implement_create_directory_nested(const CString& full_path)
{
	// �Ȱ�·��ͳһΪ����·���ұ���·������ʽ����β����û��\\�ַ��Ϳհ��ַ�
	CString str = full_path;
	str.Trim();
	str.Replace(_T("//"), _T("\\")); // ����д������//��Ϊ��ȷ�ĵ���'\'
	str.Replace(_T("/"), _T("\\")); // ������·����ʽ��Ϊ����·����ʽ

	str.Trim(_T("\\ \t\r\n"));// ȥ�����˵�'\\'���հ��ַ�
	if (file_tools::is_relative_path(str)) str = file_tools::get_app_path_with_xg() + str;

	// �õ�ȫ���ļ��е�����·������C:\\AAA\\BBB\\CCC���õ������ĸ��ַ�����
	// [0] C:
	// [1] C:\\AAA
	// [2] C:\\AAA\\BBB
	// [3] C:\\AAA\\BBB\\CCC
	// ע�⣺Խ����·��Խ�ں���
	vector<CString> all_folder_path;
	int cur_pos = str.Find(_T('\\'));
	do 
	{
		if (-1 == cur_pos)
		{
			all_folder_path.push_back(str); // û�ҵ�ԭ������
			break;
		}

		all_folder_path.push_back(str.Left(cur_pos));
		cur_pos = str.Find(_T('\\'), cur_pos + 1); // ǰ���Ѿ�ȷ����β����û��\\�ַ��Ϳհ��ַ�
	} while (true);

	// ��0��Ԫ��ɾ��Ϊ�����̷���Ҫɾ��
	if (!all_folder_path.empty()) all_folder_path.erase(all_folder_path.begin());

	// �Ӷ̵�·����ʼ����
	for (auto& x : all_folder_path)
	{
		if (x.IsEmpty()) continue; // ���������\\\\ֻ��һ�����������Ŀ����ƺ���
		if (::PathFileExists(x)) continue;

		if (!::CreateDirectory(x, nullptr)) return false;
	}

	return true;
}

// Ƕ�״����ļ��У�ָ������·������������ڻἶ����������·���е��ļ���
bool file_tools::create_directory_nested(const CString& dir_path)
{
	// �Ȱ�·��ͳһΪ����·���ұ���·������ʽ����β����û��\\�ַ��Ϳհ��ַ�
	CString str = dir_path;
	str.Trim();
	str.Replace(_T("//"), _T("\\")); // ����д������//��Ϊ��ȷ�ĵ���'\'
	str.Replace(_T("/"), _T("\\")); // ������·����ʽ��Ϊ����·����ʽ

	str.Trim(_T("\\ \t\r\n"));// ȥ�����˵�'\\'���հ��ַ�
	if (file_tools::is_relative_path(str)) str = file_tools::get_app_path_with_xg() + str;

	if (::PathIsDirectory(str)) return true; // ·���Ѿ�����
	if (!::PathIsDirectory(str)) implement_create_directory_nested(str); //���Ŀ¼������,��������Ŀ¼
	return (TRUE == ::PathIsDirectory(str)); // ��ɺ��ⴴ����·���Ƿ����
}

bool file_tools::create_file_nested(const CString& file_path, bool override/* = true*/)
{
	// �Ȱ�·��ͳһΪ����·���ұ���·������ʽ����β����û��\\�ַ��Ϳհ��ַ�
	CString str = file_path;
	str.Trim();
	str.Replace(_T("//"), _T("\\")); // ����д������//��Ϊ��ȷ�ĵ���'\'
	str.Replace(_T("/"), _T("\\")); // ������·����ʽ��Ϊ����·����ʽ

	str.Trim(_T("\\ \t\r\n"));// ȥ�����˵�'\\'���հ��ַ�
	if (file_tools::is_relative_path(str)) str = file_tools::get_app_path_with_xg() + str;

	if (::PathFileExists(str)) return true;

	if (str.GetLength() >= MAX_PATH) return false;
	TCHAR parent_dir[MAX_PATH] = _T("");
	_tcscpy_s(parent_dir, str);

	//��ȡ�ϼ�Ŀ¼
	if (!::PathRemoveFileSpec(parent_dir)) return false;

	//�����ļ�����·����ȫ��Ŀ¼
	if (!implement_create_directory_nested(parent_dir)) return false;

	DWORD create_flag = (override ? CREATE_ALWAYS : OPEN_ALWAYS);
	HANDLE file_handle = ::CreateFile(str, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr, create_flag, FILE_FLAG_BACKUP_SEMANTICS, nullptr);

	if (file_handle == INVALID_HANDLE_VALUE) return false;
	::CloseHandle(file_handle);
	
	return true;
}

// ����ɾ�����ڵĿ��ļ���
bool implement_delete_single_empty_folder(const CString& path)
{
	// �ļ��е��������ú��ļ��Ĳ���һ��
	if (FILE_ATTRIBUTE_NORMAL & GetFileAttributes(path))
		SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);

	return (TRUE == RemoveDirectory(path)); // ֻ���Ƴ����ļ���
}

bool file_tools::delete_file_or_directory(const CString& dir_path, bool del_self /*= false*/)
{
	if (FALSE == ::PathFileExists(dir_path)) return true; // ·��������Ҳ��Ϊ��ɾ���ɹ�
	if (is_file_path(dir_path))
		return delete_file(dir_path);
	else
		return delete_directory(dir_path, del_self);
}

// ����������ļ�������ɾ��
bool implement_delete_all_files_and_sub_directories_in_folder(const CString& cur_path)
{
	if (FALSE == PathFileExists(cur_path)) return true; // ·��������Ҳ��Ϊ��ɾ���ɹ�
	if (file_tools::is_file_path(cur_path)) return file_tools::delete_file(cur_path);

	// �������ļ��м��ڲ������ļ�
	bool all_ok = true;
	CFileFind file_find;
	BOOL has_found = file_find.FindFile(cur_path + _T("\\*.*"));
	while (has_found)
	{
		has_found = file_find.FindNextFile();
		CString cur_file_path = file_find.GetFilePath();

		if (file_find.IsDirectory())
		{
			if (file_find.IsDots()) continue;

			bool flag = implement_delete_all_files_and_sub_directories_in_folder(cur_file_path);
			if (false == flag) all_ok = false;

			if (false == implement_delete_single_empty_folder(cur_file_path)) all_ok = false;
		}
		else
		{
			if (false == file_tools::delete_file(cur_file_path)) all_ok = false;
		}
	}

	file_find.Close();

	return all_ok;
}

bool file_tools::delete_directory(const CString& dir_path, bool del_self /*= false*/)
{
	if (!is_folder_path(dir_path) || FALSE == ::PathFileExists(dir_path)) return false;

	bool res = implement_delete_all_files_and_sub_directories_in_folder(dir_path);
	
	if (del_self)
	{
		bool flag = implement_delete_single_empty_folder(dir_path);
		if (!flag) res = false;
		return res;
	}
	
	return res;
}

bool file_tools::delete_file(const CString& file_path)
{
	if (FALSE == ::PathFileExists(file_path)) return true; // ·��������Ҳ��Ϊ��ɾ���ɹ�
	if (!is_file_path(file_path)) return false;

	if (FILE_ATTRIBUTE_NORMAL & GetFileAttributes(file_path))
		SetFileAttributes(file_path, FILE_ATTRIBUTE_NORMAL);

	return (TRUE == DeleteFile(file_path));
}

bool file_tools::delete_files_and_sub_directory(const CString& dir_path)
{
	return delete_file_or_directory(dir_path, false);
}

