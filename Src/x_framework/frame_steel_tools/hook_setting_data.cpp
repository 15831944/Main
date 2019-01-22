#include "StdAfx.h"

#include "..\tools\string_tools.h"

#include "hook_setting_data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

frame_hook_setting::frame_hook_setting()
{
}

frame_hook_setting::~frame_hook_setting()
{
}

void frame_hook_setting::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);

		mt.write(_T("m_hook_name"), m_hook_name);
		mt.write(_T("m_ref_line_name"), m_ref_line_name);
		mt.write(_T("m_is_top_hook"), m_is_top_hook);
		mt.write(_T("m_node_index"), m_node_index);
		mt.write(_T("m_node_begin_layer"), m_node_begin_layer);
		mt.write(_T("m_segment_name"), m_segment_name);
	}
	else
	{
		mt.read(_T("version"), version);
		if (1 == version)
		{
			mt.read(_T("m_hook_name"), m_hook_name);
			mt.read(_T("m_ref_line_name"), m_ref_line_name);
			mt.read(_T("m_is_top_hook"), m_is_top_hook);
			mt.read(_T("m_node_index"), m_node_index);
			mt.read(_T("m_node_begin_layer"), m_node_begin_layer);
			mt.read(_T("m_segment_name"), m_segment_name);
		}
	}
}

void string_to_hook_settings(const CString& setting_text, std::vector<frame_hook_setting>& setting_data)
{
	if (_T("") == setting_text) return;

	vector<CString> setting_rows = string_tools::split_string(setting_text, _T(";"));
	if (setting_rows.empty())
	{
		frame_hook_setting hook_settings;
		setting_data.push_back(hook_settings); // ����һ���յ�������,��ֹ�û�Ԥ�������ݱ�����Ĵ���У�������������
		return;
	}

	for (size_t i = 0; i != setting_rows.size(); ++i)
	{
		frame_hook_setting hook_settings;
		if (_T("") == setting_rows[i]) // ����һ���յ�������,��ֹ�û�Ԥ�������ݱ�����Ĵ���У�������������
		{
			setting_data.push_back(hook_settings);
			continue;
		}

		vector<CString> setting_cells = string_tools::split_string(setting_rows[i], _T(","));
		if (setting_cells.size() != 2) setting_cells.resize(2);
		hook_settings.m_hook_name = setting_cells[0];
		hook_settings.m_hook_name.Trim();

		vector<CString> setting_ref_lines = string_tools::split_string(setting_cells[1], _T("["));
		if (setting_ref_lines.size() != 2) setting_ref_lines.resize(2);
		hook_settings.m_ref_line_name = setting_ref_lines[0];
		hook_settings.m_ref_line_name.Trim();

		vector<CString> setting_segment = string_tools::split_string(setting_ref_lines[1], _T("]"));
		if (setting_segment.size() != 2) setting_segment.resize(2);
		hook_settings.m_segment_name = setting_segment[1];
		hook_settings.m_segment_name.Trim();

		// ������ݲ�ȱ,��Ϊ�ո�,�ٽ���,��������֮ǰȷ�����ݳ����㹻,��ֹ�±�Խ��
		if (setting_segment[0].GetLength() < 3) setting_segment[0] += _T("   "); // �������ո�,���ݳ���һ���㹻��
		// ���ݳ�������Ҫ��
		if (_T("+") == setting_segment[0].Left(1))
			hook_settings.m_is_top_hook = _T("����");
		else if (_T("-") == setting_segment[0].Left(1))
			hook_settings.m_is_top_hook = _T("�ײ�");
		else
			hook_settings.m_is_top_hook = _T("");

		if (_T("S") == setting_segment[0].Right(1))
			hook_settings.m_node_begin_layer = _T("�����");
		else if (_T("E") == setting_segment[0].Right(1))
			hook_settings.m_node_begin_layer = _T("��ֹ��");
		else
			hook_settings.m_node_begin_layer = _T("");

		CString strID = setting_segment[0].Mid(1, setting_segment[0].GetLength() - 2); // ȥ����β���˵������ַ�
		strID.Trim();
		if (_T("") == strID)
			hook_settings.m_node_index = _T("");
		else
			hook_settings.m_node_index.Format(_T("%d"), _ttoi(strID)); // û��ֱ�ӽ����ַ�����ֵ,��Ϊ�˹��˵��������ַ�

		setting_data.push_back(hook_settings);
	}
}

void hook_settings_to_string(const std::vector<frame_hook_setting>& setting_data, CString& setting_text)
{
	CString setting_rows;
	setting_text = _T("");
	vector<frame_hook_setting> hook_settings = setting_data; // ����Ϊconst�ģ�Ҫ���ַ�������һ�������������¶���һ����const��

	for (size_t i = 0; i != hook_settings.size(); ++i)
	{
		// ����ַ������˵Ŀհ��ַ�
		hook_settings[i].m_hook_name.Trim();
		hook_settings[i].m_ref_line_name.Trim();
		hook_settings[i].m_is_top_hook.Trim();
		hook_settings[i].m_node_index.Trim();
		hook_settings[i].m_node_begin_layer.Trim();
		hook_settings[i].m_segment_name.Trim();


		// ������û�Ԥ���Ŀ�����ԭ�������������
		if (_T("") == hook_settings[i].m_hook_name
			&& _T("") == hook_settings[i].m_ref_line_name
			&& _T("") == hook_settings[i].m_is_top_hook
			&& _T("") == hook_settings[i].m_node_index
			&& _T("") == hook_settings[i].m_node_begin_layer
			&& _T("") == hook_settings[i].m_segment_name)
		{
			setting_text += _T(" , [   ] "); // ���һ����������
		}
		else
		{
			// ������if���������ƣ����ﴫ���������ʼ״̬����ȫ��Ϊ��
			// Ϊ��ֹ���ݴ��ң����ĳ���ֶ�û����д��Ϣ�����ÿո����
			setting_rows.Format(_T("%s,%s[%s%s%s]%s"),
				(_T("") == hook_settings[i].m_hook_name ? _T(" ") : hook_settings[i].m_hook_name),
				(_T("") == hook_settings[i].m_ref_line_name ? _T(" ") : hook_settings[i].m_ref_line_name),
				(_T("����") == hook_settings[i].m_is_top_hook ? _T("+") : _T("-")),
				(_T("") == hook_settings[i].m_node_index ? _T("0") : hook_settings[i].m_node_index), // ���û����д��ʾ0
				(_T("�����") == hook_settings[i].m_node_begin_layer ? _T("S") : _T("E")),
				(_T("") == hook_settings[i].m_segment_name ? _T(" ") : hook_settings[i].m_segment_name));

			setting_text += setting_rows;
		}
		if (i < hook_settings.size() - 1) setting_text += _T(";"); // ���һ������_T(";")
	}
}
