#include "stdafx.h"

#include <vector>

#include "csv_tools.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

// �����л���������
class raii_change_location
{
public:
	raii_change_location(int category, const TCHAR* locale)
	{
		m_category = category;
		m_locale = _tsetlocale(category, locale);
	}

	~raii_change_location()
	{
		_tsetlocale(m_category, m_locale);
	}

private:
	raii_change_location() = delete;
	raii_change_location(const raii_change_location&) = delete;
	void operator=(const raii_change_location&) = delete;

private:
	int m_category;
	CString m_locale;
};

//////////////////////////////////////////////////////////////////////////

void csv_tools::export_to_csv_file(const CString& strFileName, const vector<vector<CString>>& data)
{
	// if (data.empty()) return; // ����Ϊ��Ҳ����һ��csv�ļ���ֻ��������д����

	// vc++��unicode�ַ����ļ�д��ʱ��������Ļ���bug�޷���ȷд�룬����Ҫ�л�����
	raii_change_location raii_loc(LC_ALL, _T("chs"));

	CStdioFile file(strFileName, CFile::modeCreate | CFile::modeWrite);
	try
	{
		CString strFileText;
		CString strCell;

		// ������תΪ���ŷָ�ÿһ��
		for (size_t i = 0; i < data.size(); ++i)
		{
			const vector<CString>& dataRow = data[i];

			for (size_t j = 0; j < dataRow.size(); ++j)
			{
				strCell = dataRow[j];
				strCell.Replace(_T("\""), _T("\"\"")); // ��Ԫ������˫���ţ�д���ļ�Ҫ��Ϊ��������˫����
				
				// ��Ԫ���а����˶��ź�˫�����Լ����У�����������Ҫ��˫����������
				if (-1 != strCell.FindOneOf(_T(",\"\r\n"))) strCell = _T("\"") + strCell + _T("\"");

				strFileText += strCell;
				if (j < dataRow.size() - 1) strFileText += _T(","); // ���һ�����治Ҫ����
			}

			if (i < data.size() - 1) strFileText += _T("\n"); // ���һ�в�Ҫ���з�
		}

		file.WriteString(strFileText);
	}
	catch (...)
	{
		// Nothing;
	}

	file.Close();
}

std::vector<std::vector<CString>> csv_tools::import_data_from_csv(const CString& strFileName, bool make_same_colum_count/* = true*/)
{
	vector<vector<CString>> data;
	import_data_from_csv(strFileName, data, make_same_colum_count);
	return data;
}

void put_one_string_to_row_item(vector<CString>& row_items, const CString& item, bool link_to_last_row)
{
	if (link_to_last_row)
	{
		if (row_items.empty())
			row_items.push_back(item);
		else
			// �����������ĵ�Ԫ���������л��У�Ҫ����(����windowsϵͳĬ�ϲ�Ϊ\r\n��ͨ���Ժã�������excel�в��Թ�������)
			row_items.back().Append(_T('\n') + item);
	}
	else
	{
		row_items.push_back(item);
	}
}

// ����һ�����ݣ�����������к��л��У������Ѿ���ȷ�����ķŵ�result�У����һ��û�����������������result�����
// �棬����һ�ν���ʱ��������ӵ����һ���ַ����ĺ���
// ����������ݾ�����ȷ�����꣬����true���������ݴ���ʱ���Զ������Ա�֤�������ܵõ������ܶ�����ݣ������δ������
// ��������Ҫ����һ�е����ݼ��������򷵻�false
// link_to_last_rowΪtrue��ʾ��ǰ����result�е����һ��Ԫ����csv�е�ͬһ�����ݣ�һ������£���������ֵΪfalse
// ʱ����ֵ��true������false
// ע�⴫����ַ����в��ܺ��лس����з�
bool prive_split_csv_row_ex(vector<CString>& result, const CString& row, bool link_to_last_row)
{
	TCHAR current_char = _T('\0'), next_char = _T('\0');
	bool current_pos_in_cell = (link_to_last_row ? true : false); // ��ǰ�ַ����Ƿ���˫�������������ַ�����
	bool linked_to_last_row_has_been_done = false; // �����ǰ��Ҫ���ӵ���һ�У�����Ҫ���ӵĵ�һ����Ԫ������ɺ�ͻ���Ϊtrue������Ĳ������ӵ���һ�У�������������
	CString cell_text = _T("");
	
	int row_count = row.GetLength();
	for (int i = 0; i < row_count; /* nothing */)
	{
		current_char = row[i];
		next_char = (i == row_count - 1 ? _T('\0') : row[i + 1]);

		if (_T(',') == current_char && !current_pos_in_cell)
		{
			if (current_pos_in_cell)
			{
				// �������Ҫ��������һ����˵��������˫�����У����Ե�Ԫ���е��������ݣ������зָ���
				cell_text += current_char;
				
				// ����Ѿ������һ���ַ�����ֱ�Ӵ����
				if (i == row_count - 1)
				{
					put_one_string_to_row_item(result, cell_text, link_to_last_row && false == linked_to_last_row_has_been_done);
					return current_pos_in_cell;
				}
			}
			else
			{
				put_one_string_to_row_item(result, cell_text, link_to_last_row && false == linked_to_last_row_has_been_done);
				linked_to_last_row_has_been_done = true; // ��ǰ���Ų��������ڣ�����Ч���зָ�������ʱ�����費��Ҫ���ӵ�ǰһ�ж�����Ѵ˱����Ϊtrue��ʾ������в���Ҫ�ٿ������ӵ���һ�е����
				cell_text.Empty();
			}

			++i;
			continue;
		}
		else if (_T('\"') == current_char && _T('\"') == next_char)
		{
			cell_text += _T('\"'); // ��ʹ��_T("\"")���д���С

			// ������������������ڵ�ǰ�е���ĩβ��ֱ�Ӵ����
			if (i == row_count - 2) // ע�������2�����Ǽ�1
			{
				put_one_string_to_row_item(result, cell_text, link_to_last_row && false == linked_to_last_row_has_been_done);
				return current_pos_in_cell;
			}

			i += 2; // һ��������������
			continue;
		}
		else if (_T('\"') == current_char && _T('\"') != next_char)
		{
			current_pos_in_cell = !current_pos_in_cell; // return ǰҪ������˱�־

			// ����Ѿ������һ���ַ�����ֱ�Ӵ����
			if (i == row_count - 1)
			{
				put_one_string_to_row_item(result, cell_text, link_to_last_row && false == linked_to_last_row_has_been_done);
				return current_pos_in_cell;
			}

			++i;
			continue;
		}
		else
		{
			cell_text += current_char;
			
			if (i == row_count - 1)
			{
				put_one_string_to_row_item(result, cell_text, link_to_last_row && false == linked_to_last_row_has_been_done);
				return current_pos_in_cell;
			}
			
			++i;
		}
	}

	return current_pos_in_cell;
}

void csv_tools::import_data_from_csv(const CString& strFileName, std::vector<std::vector<CString>>& data, bool make_same_colum_count/* = true*/)
{
	data.clear();

	CStdioFile file(strFileName, CFile::modeRead);
	try
	{
		CString csv_row;
		vector<CString> result;
		bool row_finished = true;
		// The CString version of ReadString() removes the '\n' if present; the LPTSTR version does not.
		while (TRUE == file.ReadString(csv_row))
		{
			row_finished = (false == prive_split_csv_row_ex(result, csv_row, !row_finished));
			if (row_finished)
			{
				data.push_back(result);
				result.clear();
			}
		}

		if (!row_finished)
		{
			data.push_back(result);
			result.clear();
		}
	}
	catch (...)
	{
		// nothing;
	}
	file.Close();

	// ȷ�������е�������һ���࣬�а�����м��㣬����������Կո����
	if (make_same_colum_count)
	{
		size_t szMax = 0;
		for (auto& x : data)
			if (x.size() > szMax) szMax = x.size();

		if (0 == szMax)
			data.clear();
		else
			for (auto& x : data) x.resize(szMax, _T(""));
	}
}