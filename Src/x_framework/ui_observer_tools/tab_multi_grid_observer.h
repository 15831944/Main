#pragma once

#include <map>
#include <vector>
#include "x_observer_base.h"


struct NM_THGRID;
class CTHDataBase;


class X_FRAMEWORK_API tab_multi_grid_observer : public x_observer_base
{
	DECLARE_SERIAL(tab_multi_grid_observer)

public:
	tab_multi_grid_observer();
	virtual ~tab_multi_grid_observer();

public:
	virtual void get_table_template_names(std::vector<std::pair<CString, std::vector<CString>>>& all_tables) const;

	// û��ģ�����ı��ʹ�ô˽ӿ��͵����棬ע���������Ʋ�����get_table_template_names�е�ģ��������ͬ
	// observerһ����������Ӧ�ı༭�����ں�CTHDataBase*��ָ���ɿ�ܽӹܣ������ܲ��������ָ��
	virtual void get_table_instances(std::vector<std::pair<CString, std::vector<std::pair<CString, CTHDataBase*>>>>& all_tables) const;

	// ���ڶ�observerӵ�е�ѡ�˳������front��backǰ�淵��true��Ĭ�Ϸ���true
	virtual bool is_tab_name_at_front(const CString& front, const CString& back) const;

	virtual void save_data_from_ui(std::map<CString, CTHDataBase*>& all_dbs);
	virtual void load_data_to_ui(std::map<CString, CTHDataBase*>& all_dbs);
	virtual void cell_button_be_clicked(CTHDataBase* db, NM_THGRID* notify_struct);
	virtual void cell_value_changed(CTHDataBase* db, NM_THGRID* notify_struct);
	virtual void cell_focus_changed(CTHDataBase* db, NM_THGRID* notify_struct);
};
