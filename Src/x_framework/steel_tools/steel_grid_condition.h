// ���ļ����������ڿؼ�������״̬��һЩ��

#pragma once

enum steel_grid_condition_action
{
	SGCA_HIDE,		// �����������أ�����������ʾ��ĳһ����ֻҪ��һ����Ԫ������Ҫ���أ��ͻ�������ж���������
	SGCA_READONLY	// ����������Ϊֻ��������������Ϊ�ɶ�д
};


// ��λһ���ֽ���е���������Ԫ��
class X_FRAMEWORK_API steel_grid_pos
{
public:
	steel_grid_pos();
	steel_grid_pos(IN const CString& id, IN int index); // indexȡֵֻ�ܴ�0��5
	~steel_grid_pos();

public:
	void set(IN const CString& id, IN int index); // indexȡֵֻ�ܴ�0��5

public:
	CString		m_id;				// ��������õı�ʶ������������ʶ���ӱ�ʶ�������Ǹ�����ĵ��еı�ʶ
	int			m_value_index;		// ��Ч�����еĵڼ��У�0 Ϊ�����еĵ�һ�У�ע�ⲻ�Ǳ��ĵ�һ�У�
};


// ������ʱ��steel_grid_condition_action����Ϊ���в�����������ʱ��steel_grid_condition_action���෴��Ϊ���в���
// ���ж������ʱ������֮��Ĺ�ϵΪ���롱�����������֮����Ҫ���򡱹�ϵ����ת��Ϊ���롱������
// ��Ԫ������������ı������ǿؼ�����������ʱ���ַ���ֵΪ��Ӧ��Ԫ���text�����磺��ѡ��ѡ��
// ʱΪ��1����û��ѡ��ʱΪ��0����������ͬ���������ѯ���ؼ��ĸ����ˣ����ܰ��ո�����ĵ��е�ֵ��������
// �����еĵ�Ԫ���ַ������ִ�Сд������
// ע��:
//   1.�����ݽṹ������ʵ�ּ򵥵Ľ���״̬���ã��벻Ҫ���ø��ӹ�ϵ����Ƕ���޸�״̬��������ݹ��޸�״̬��������
//   2.�����ͬһ����Ԫ�������˶�������������������Ḳ��֮ǰ������
//   3.���ͬһ����Ԫ��Ȼ�Ӱ��������Ԫ���״̬��ͬʱҲ��Ҫ����ʵ��Ԫ������Ӱ�죬��ôִ�е��Ⱥ�˳���ܷ���vectorʱ���Ⱥ�˳��Ӱ�죬ԭ�������ȷ�����ִ�С�
class X_FRAMEWORK_API steel_grid_conditions
{
public:
	steel_grid_conditions();
	steel_grid_conditions(IN const CString& cond_id, IN int cond_index, IN const CString& cond_action, IN const CString& cond_value, IN const CString& des_id, IN int des_index, IN steel_grid_condition_action des_action); // cond_actionֻ�ܴ���_T("==")��_T("!=")
	steel_grid_conditions(IN const steel_grid_pos& cond_pos, IN const CString& cond_action, IN const CString& cond_value, IN const steel_grid_pos& des_pos, IN steel_grid_condition_action des_action); // cond_actionֻ�ܴ���_T("==")��_T("!=")
	steel_grid_conditions(IN bool is_ok, IN const steel_grid_pos& des_pos, IN steel_grid_condition_action des_action); // is_okΪtrue��ʾ�������㣬�����ʾ������(�����������ʹ�����Լ����ƣ����������������������is_ok����)
	~steel_grid_conditions();

public:
	void set(IN const CString& cond_id, IN int cond_index, IN const CString& cond_action, IN const CString& cond_value, IN const CString& des_id, IN int des_index, IN steel_grid_condition_action des_action); // cond_actionֻ�ܴ���_T("==")��_T("!=")
	void set(IN const steel_grid_pos& cond_pos, IN const CString& cond_action, IN const CString& cond_value, IN const steel_grid_pos& des_pos, IN steel_grid_condition_action des_action); // cond_actionֻ�ܴ���_T("==")��_T("!=")
	void set(IN bool is_ok, IN const steel_grid_pos& des_pos, IN steel_grid_condition_action des_action); // is_okΪtrue��ʾ�������㣬�����ʾ������(�����������ʹ�����Լ����ƣ����������������������is_ok����)
	void and(IN const CString& cond_id, IN int cond_index, IN const CString& cond_action, IN const CString& cond_value); // ����And������cond_actionֻ�ܴ���_T("==")��_T("!=")
	void and(IN const steel_grid_pos& cond_pos, IN const CString& cond_action , IN const CString& cond_value); // ����And������cond_actionֻ�ܴ���_T("==")��_T("!=")
	void clear_all_conditions();
	int get_condition_count() const;
	const steel_grid_pos get_condition_pos_at(int index) const;
	const CString get_condition_type_at(int index) const; // ����δ���÷���_T("")������ʱ�򷵻�_T("==")����_T("!=")
	const CString get_condition_value_at(int index) const;
	const steel_grid_pos get_des_pos() const;
	enum steel_grid_condition_action get_condition_action() const;
	bool is_force_condition() const; // Ϊtrue��ʾ�˲�������Ҫ�κ��������ƣ�����ִ��ָ���Ĳ���
	bool is_condition_exist_and_true() const; // �����Ƿ�����

private:
	// һ������
	typedef struct
	{
		steel_grid_pos	cell_condition;
		CString			condition_action_text; // Ϊ_T("==")ʱ������Ԫ����strVAlue��ȵ����棬�������٣���֮����Ϊ_T("!=")ʱ��ͬԭ��
		CString			value;
	}single_condition;

	bool							m_is_force_condition;			// Ϊtrue��ʾ���޶�Դ��Ԫ��ȡֵ������������ָ����Ŀ�굥Ԫ�����ǰ�������ָ���Ĳ������д���
	bool							m_is_condition_exist_and_true;	// �����Ƿ����㣨m_is_force_conditionΪtrueʱʹ�ã�

	std::vector<single_condition>	m_conditions;					// ���յ�����
	steel_grid_pos					m_des_cell;						// Ҫ�����ĵ�Ԫ��
	steel_grid_condition_action		m_action;						// Ҫ��������Ϊ
};

