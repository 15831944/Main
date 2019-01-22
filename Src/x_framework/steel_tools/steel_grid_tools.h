#pragma once
#include "steel_data.h"

class steel_grid_conditions;
class CTHDataBase;


class X_FRAMEWORK_API steel_grid_tools
{
private:
	steel_grid_tools(void);
	~steel_grid_tools(void);

public:
	// ��������Ҫָ������ӱ��ģ������Ĭ�ϵ�Ӧ�ò�λ
	static CTHDataBase* create_steel_grid(IN const CString& caption,
										  IN const CString& design_grid,
										  IN const CString& basic_grid,
										  IN const CString& layout_grid,
										  IN const CString& edge_grid,
										  IN const CString& detail_grid,
										  IN bool is_fixed = true, // �ֽ���ǹ̶��ģ����ǿ��ɵ��������ӡ�ɾ���ֽ��
										  IN bool enable_component = false); // Ӧ�ò�λ�Ƿ�����
	
	// ���ݶ�Ӧ�ĸֽ������һ��Ĭ��ֵ
	static void make_default_value_by_grid(const CString& grid_name, steel_data& data);

	// Ϊ���ñ���״̬����������Ҫ�ڵ�Ԫ�����ݱ��ı��Լ�������ڵĴ�����ʾ��ʱ����ã�������������տ�ʼ״̬�ǲ���ȷ�ģ�
	// ˢ�±�����״̬��nDBRow,nDBColΪ���ݷ����仯�ĵ�Ԫ�����к�(���кŵĶ�λ���������Database���������һ��Ϊ-1��
	// ��ʾˢ������������ֻˢ�����к����ڵ��ӱ�һ���ӱ�Ϊ���������˼�롢������Ϣ��5���ֵ������ֽ����ǿ�ҽ���û���������ɲ�Ҫ��-1)
	// �˺����ڲ�Ҫ���б���������������΢��ʱ����õ�������ڵĴ��ڴ�����ʾ״̬���е�Ԫ�����ݱ��޸�ʱ��
	// ���ã���̨ʱ���õ��ã���Ϊ�������ʾ���̨���ݻ�ȡ��ȷ���û�й�ϵ���ر����������ʱ��̨��������
	// �����ݱ��޸ģ�Ӧ�ñ����̨���Ҳ���ñ���������������ִ������û���������ԡ�
	// ����һ����������������ʾ�����룺
	// 	// 1.Ҫ��SDZJ_A���ݵ�0��ֵΪ20 �� SXGJW���ݵ�0�в���ѡ��״̬ʱ������SDZJ_B���ݵ�0��Ϊֻ��������Ϊ�ɶ�д
	// 	steel_grid_conditions cond_01( steel_grid_pos(_T("SDZJ_A"), 0), _T("=="), _T("20"), steel_grid_pos(_T("SDZJ_B"), 0), SGCA_READONLY );
	// 	cond_01.And(steel_grid_pos(_T("SXGJW"), 0), _T("!="), _T("0"));
	// 
	// 	// 2.Ҫ��SXZJJQΪѡ��״̬ʱ������SSGJ_A���ݵ�1��Ϊֻ��������Ϊ�ɶ�д
	// 	steel_grid_conditions cond_02( steel_grid_pos(_T("SXZJJQ"), 0), _T("=="), _T("1"), steel_grid_pos(_T("SSGJ_A"), 1), SGCA_READONLY );
	// 
	// 	vector<steel_grid_conditions> conds;
	// 	conds.push_back(cond_01);
	// 	conds.push_back(cond_02);
	// 	steel_grid_tools::refresh_grid_status(*m_pDB, 10, 5, conds);
	// 	
	// 	//��һ�ַ������Ƽ�����
	// 	steel_grid_conditions cond;
	// 	vector<steel_grid_conditions> conds;
	// 	// ֻҪSXST8��SXST9��SXST10�κ�һ����Ϊ0��ɶ�д������ֻ��
	// 	cond.Set( _T("SXST8"), 0, _T("=="), _T("0"), _T("SJSTJL"), 0, SGCA_READONLY );
	// 	cond.And( _T("SXST9"), 0, _T("=="), _T("0") );
	// 	cond.And( _T("SXST10"), 0, _T("=="), _T("0") );
	// 	conds.push_back(cond);
	// 	�������CTHDataBase��Ҫˢ�£����С��о���-1����ָ��������ӱ����ڵĵ�Ԫ�����к�Ҫ���Ѹ����ʱ�䣩
	// 	���ж������ʱ������֮��Ĺ�ϵΪ���롱�����������֮����Ҫ���򡱹�ϵ����ת��Ϊ���롱������
	static void refresh_grid_status(IN CTHDataBase& db, IN int row, IN int col, const std::vector<steel_grid_conditions>& cond);
	
	// ����Ӧ�ò�λ��������ǩ�ı��ı�������DataBase�б���һ�µ�Ԫ��Ϊֻ��Ҳ�������ã�
	static void set_site_of_injection_label(IN CTHDataBase& db, const CString& text); // ��ǩѡ��ʹ��\n����
	static void set_site_of_injection_label(IN CTHDataBase& db, const std::vector<CString>& all_text); // ÿ��Ԫ��Ϊһ��ѡ��
	
	// ����Ӧ�ò�λ�ĵ�Ԫ���ı��ı�������DataBase�б���һ�µ�Ԫ��Ϊֻ��Ҳ�������ã�
	// indexΪ-1��ʾȫ��Ӧ�ò�λ���޸ģ�������޸�ָ���ĵ�index����0��ʼ����Ӧ�ò��������������������޸�
	static void set_site_of_injection_text(IN CTHDataBase& db, const CString& text, int index = -1); // ��ǩѡ��ʹ��\n����

	// �жϴ���ı��Ƿ��Ǹ����ֽ��(���ж��Ƿ��ǣ�������֤�����ĸ�ʽ�벼���Ƿ�����ȷ)
	static bool is_steel_grid(IN CTHDataBase& db);

	// ���ֽ�����û�����ı��������ݣ�����д���ͱ����ֽ�ľ������ݴ���ʱ���ٱ���
	// strSteel: ��ʾҪ���ĸֽ���������ָ���ŷ����߲��������ȿ���ϸ��λ����Ϣ�������ַ�����Ҫʹ��"[]����"���ţ��Ա���
	//           ʱ��Ϊǰ׺�������ʽΪ���ŷ���-������-�ֽ�����������硰�ŷ�1-�ȿ�ȸ�����33-���۸ֽ��
	// bReport: Ϊtrue��ʾ����汨�������Ϣ�����������鵫������
	// ���ݼ���û�д��󷵻�true�����򷵻�false������ֽ����Ϊ�ձ���0���ӱ�ֻ�б��⣩Ҳ��Ϊ�ǲ��Ϸ�
	// ����������Դ���Ľ������������κεĸ��ĺ������������м��
	static bool check_steel_grid_data(IN const CString& err_text_prefix, IN CTHDataBase& db, bool need_report = true);

	// ��ȡָ���ĵ�Ԫ��
	// Parameter: IN CTHDataBase& db
	//   DataBase
	// Parameter: IN const CString & strID
	//   �����б�ʶ�����ģ���������ָ�����б�ǩ��
	// Parameter: int nValueColIndex
	//   �������е��кţ�0 Ϊ�����еĵ�һ�У�������ָ���ֽ�������������֮����ֵ������
	//   �ֵ��У��������Ϣ�е�ֱ�����߾���Ϣ�еĶ�λ��Ⱦ�Ϊ��0�С�
	// Parameter: OUT std::vector<CTHCell*> vecCell
	//   �洢���صĵ�Ԫ��
	// Return Value:
	//   ���ָ���������������֮���Ҹ��з��ؿյĵ�Ԫ��ָ�롣
	//   ���������ȷ���򷵻�DB��ÿһ���ֽ��ӱ��ж�Ӧ�ĵ�Ԫ��
	// ����˵����
	//   �������ĵ�Ԫ���벻Ҫ�������²�����������ƻ����ṹ���������ݶ�ȡ��д�����
	//   1. ���ĺϲ���Ԫ������
	//   2. ���ĵ�Ԫ���TAG
	//   3. ���ĵ�Ԫ�����ڵ���TAG
	//   3. ���ĵ�Ԫ�����ڵ���TAG
	//   4. ���û���������ֻ���ڱ������������õĻ�����������Ч��
	static void get_steel_cells(IN CTHDataBase& db, IN const CString& id, IN int col_index_of_value, OUT std::vector<CTHCell*>& all_cells);

	// ��������ָ����ʶ�ĵ�Ԫ���ǩ����������ͬGetTHCells
	// Ҫ���õ�ֵ��vecValues�У��������Ҫ��ĵ�Ԫ�����������ַ��������������൥Ԫ��
	// vecValues��ÿ��Ԫ�ض�Ӧһ����Ԫ���еı�ǩ�������ǩ�ж����ʹ��\n�ָ�
	// ȫ�����ó����һ���ַ���ֵ��������ڣ��򲻹ܡ�
	// �����Ҫ�������ݵĵ�Ԫ��϶࣬����ʹ��SetP0DataЧ�ʸ���
	static void set_steel_cells_label(IN CTHDataBase& db, IN const CString& id, IN int col_index_of_value, IN std::vector<CString>& all_values);

	// ��������ָ����ʶ�ĵ�Ԫ���ı�ֵ����������ͬGetTHCells
	// Ҫ���õ�ֵ��vecValues�У��������Ҫ��ĵ�Ԫ�����������ַ��������������൥Ԫ��
	// ȫ�����ó����һ���ַ���ֵ��������ڣ��򲻹ܡ�
	// �����Ҫ�������ݵĵ�Ԫ��϶࣬����ʹ��SetP0DataЧ�ʸ���
	static void set_steel_cells_text(IN CTHDataBase& db, IN const CString& id, IN int col_index_of_value, IN std::vector<CString>& all_values);

	// ����ָ���ĵ�Ԫ���ڸ������е���Ŀ��Ϣ
	// Parameter: IN CTHDataBase & db
	//   DataBase
	// Parameter: IN int nRow
	//   ��DB�е��к�
	// Parameter: IN int nCol
	//   ��DB�е��к�
	// Parameter: OUT CString & strID
	//   �����б�ʶ�����ģ���������ָ�����б�ǩ��
	// Parameter: OUT int nValueColInex
	//   �������е��кţ�0Ϊ�����еĵ�һ�У�������ָ���ֽ�������������֮����ֵ������
	//   �ֵ��У��������Ϣ�е�ֱ�����߾���Ϣ�еĶ�λ��Ⱦ�Ϊ��0�С�
	static void get_steel_cell_info(IN CTHDataBase& db, IN int row, IN int col, OUT CString& id, OUT int col_index_of_value);

	// ���ػ���ʾָ����
	static void hide_row(IN CTHDataBase& db, IN const CString& id);
	static void hide_rows(IN CTHDataBase& db, IN const std::vector<CString>& all_ids); // ���ض��о��������������������DB�ж�η�����λ�н���Ч��
	static void show_row(IN CTHDataBase& db, IN const CString& id);
	static void show_rows(IN CTHDataBase& db, IN const std::vector<CString>& all_ids); // ��ʾ���о��������������������DB�ж�η�����λ�н���Ч��

	// ���ػ���ʾ�ӱ�
	static void hide_sub_grid(IN CTHDataBase& db, IN sub_steel_type type);
	static void show_sub_grid(IN CTHDataBase& db, IN sub_steel_type type);
	
	// ��ȡ��������ݲ�ȫ�����뵽P0���ݽṹ��,bAnalyticAppLoc��ʾ��������Ƿ�Ӧ�ò�λ���н���������ο�ͨ�����˵���ĵ�.docx
	// ������������û�������д�����ݽ��м�飬���۶Դ���ԭ����ȡ�������Ƿ���ȷ��ʹ��P0���˴���
	// bReplaceIDBySteelNameΪtrue��ʾ��ȡ�ĸֽ�������ID����ID����Ϊ�ֽ���Ŀ������䣬����ʹ��ID���
	static void get_steel_data_from_grid(IN CTHDataBase& db, OUT steel_data& data, IN bool parse_by_component = false, IN bool replace_steel_item_id_to_name = false);

	// ���û������P0����ȫ�����뵽�����
	// �ֽ�������ID����ID��Ϊ�ֽ�ID������ʹ����Ŀ�������
	static void set_steel_data_to_grid(OUT CTHDataBase& db, IN const steel_data& data);

	// ����steelP0In���ݣ���Ӧ�ò�λΪstrBW�ĸֽ�����ɾ����Ӧ�ò��ֱ����ϸ������е�һ������Ч��������ı����������steelP0Out�У�Ӧ�ò�λstrBW�����ִ�Сд��Add By WYZ 2012-7-11 13:19:15��
	static void filte_steel_data_by_component(IN const steel_data& data_in, IN const CString component, OUT steel_data& steel_out);
	
	// ��ȡsteelP0In���ݣ���Ӧ�ò�λΪstrBW�ĸֽ����ݱ����������steelP0Out�У�Ӧ�ò�λstrBW�����ִ�Сд��Add By WYZ 2012-7-11 13:19:15��
	// �ֽ���е�Ӧ�ò�λ�ᰴ��ǽ�����ֻҪĳһ�������strBWָ�������������У�����strBWΪ�ջ�Ϊ"All"��ȫ���������С�ʱ�����еĸֽ������Ҫ��
	// strBW�в���������������
	// ����Ҳ����򷵻ص�0���ֽ�����
	static void extract_steel_data_by_component(IN const steel_data& data_in, IN const CString component, OUT steel_data& data_out);
};
