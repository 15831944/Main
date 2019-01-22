#pragma once
#include <array>
#include <vector>
#include <utility>
#include "../resource.h"
#include "app_dialog_base.h"


// ����ͷ�ļ������⣬�ڲ�����lib���Ӻ������ռ������������ͷ�ļ������������ɢlibָ��Ĵ��������ռ���
// Ⱦ���⣬��˷���cpp�а��������⣬���ͷ�ļ����Ѿ������˱��������ռ䣬��ǰ�����cpp��Ӧ��ͷ�ļ�������
// ʹ��namespace��ʽ��������������Ƕ�׵�ͬ�������ռ���ɱ���ʧ�ܣ���Ӧ��ʹ�ò���namespace�ķ�ʽ������
// ����ʹ�ñ���������ʱ����cpp����ֱ�Ӱ���#include "THGridControl/THGridControlInc.h���ɣ���ˣ�
// ���������൱���Ѿ�����ZYH_GRID�����ռ����������

class CTHExcel;
class CTHDataBase;

// CTHCELL.h��ͷ�ļ����������⣬�������˱���THGridControlMac.h�ļ��������Ѿ���������ڱ�������ռ���
// �ˣ���CTHCELL.h��ʹ��һ�£��൱���ڱ�������ռ��ڲ���Ƕ��һ��ͬ���������ռ䣬��˱����ټ�һ�±�������ռ�
namespace ZYH_GRID
{
	class CTHCell;
}

struct NM_THGRID; // �����κ������ռ���



// ��������Ի�����࣬���Ի�����ͼ��ʱ�Զ�����Ϊ�������ͼ��
// �����Ի����ɴ�����������дget_data_from_ui/set_data_to_ui/
class X_FRAMEWORK_API dialog_sub_grid : public app_dialog_base
{
	DECLARE_SERIAL(dialog_sub_grid)
	DECLARE_MESSAGE_MAP()

public:
	dialog_sub_grid(CWnd* parent = nullptr);
	dialog_sub_grid(const CString& template_name, int default_height, int default_width,
		const CString& title = _T(""), CWnd* parent = nullptr);

	virtual ~dialog_sub_grid();

public:
	enum { IDD = IDD_DIALOG_SUB_GRID };

public:
	CString m_text;
	CString m_title;
	CString m_template_name;
	int m_default_left_pos; // Ϊ0���Զ�����
	int m_default_top_pos; // Ϊ0���Զ�����
	int m_default_height;
	int m_default_width;
	
protected:
	virtual void get_data_from_ui();
	virtual void set_data_to_ui();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void adjust_window_layout();
	virtual void set_grid_data_base(); // ��ʼ���ڲ���m_db����Ĭ��ʵ�ִ�m_template_nameʵ�������

	// �ṩ�麯�������������������Ϣ��Ӧ�Ĵ��룬��Ϊ����IDû�зų���
	virtual bool grid_remove_data_in_grid(NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_add_data_in_grid(NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_row_col_size_changed(NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_value_changed(NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_button_clicked(NM_THGRID* notify_struct, LRESULT* result);
	virtual bool grid_cell_focus_changed(NM_THGRID* notify_struct, LRESULT* result);

protected:
	CTHExcel* m_excel;
	CTHDataBase* m_db;
	CButton m_button_ok;
	CButton m_button_cancel;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void on_clicked_ok();

	afx_msg BOOL on_remove_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_add_data_in_grid(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_row_col_size_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_value_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_button_clicked(UINT id, NMHDR* notify_struct, LRESULT* result);
	afx_msg BOOL on_cell_focus_changed(UINT id, NMHDR* notify_struct, LRESULT* result);
};



// �������еĶ����б���Ҫ��������������ʽ��key/value��Ӧ�����ݣ�����𲨡������
// Ĭ�ϵ�Ԫ������Ϊ�ı������������Ҫ�����дset_cell_type()�麯��
class X_FRAMEWORK_API dialog_sub_grid_key_value_array : public dialog_sub_grid
{
public:
	dialog_sub_grid_key_value_array(CWnd* parent = nullptr);
	dialog_sub_grid_key_value_array(const CString& title, int default_height, int default_width,
		const CString& key_title, const CString& value_title, CWnd* parent = nullptr,
		int index_width = 50, int key_width = 70, int value_width = 70);

	virtual ~dialog_sub_grid_key_value_array();

public:
	// ��������
	void get_result_data_pair(std::vector<std::pair<CString, CString>>& data) const;
	void get_result_data_array(std::vector<std::array<CString, 2>>& data) const;
	void get_result_data_vector(std::vector<std::vector<CString>>& data) const;
	void get_result_data_string(CString& data) const;

	// ���ý�������
	void set_result_data_pair(const std::vector<std::pair<CString, CString>>& data);
	void set_result_data_array(const std::vector<std::array<CString, 2>>& data);
	void set_result_data_vector(const std::vector<std::vector<CString>>& data);
	void set_result_data_string(const CString& data, TCHAR row_split_char = _T(';'), TCHAR column_split_char = _T(','));

private:
	TCHAR	m_row_split_char = _T(';');
	TCHAR	m_column_split_char = _T(',');
	CString m_key_title;
	CString m_value_title;
	std::vector<std::pair<CString, CString>> m_matrix_data;

protected:
	virtual void get_data_from_ui();
	virtual void set_data_to_ui();
	virtual void set_grid_data_base();
	virtual void set_cell_type(int col_index, ZYH_GRID::CTHCell* cell); // 1��key,2:value
};

