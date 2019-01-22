#pragma once


#include "../macro.h"
#include "tendon_data.h"
////////////////////////
#define TENDON_BASETYPE			(_T("����-��������"))
#define TENDON_TYPE				(_T("����-��������"))
#define TENDON_GSMM				(_T("����-����ê��"))
#define TENDON_GSZLPC			(_T("����-��������"))
#define TENDON_REFLINE			(_T("����-�Զ���ο���"))
#define TENDON_USERLINE			(_T("����-ͬ����������"))

//�ֶ�����
#define TENDON_SWLX				(_T("����-��������"))
#define TENDON_PWLX				(_T("����-ƽ������"))
#define TENDON_LAY				(_T("����-��������"))

//tag��
#define TAG_PSWINDEX			(_T("ƽ������������"))
#define TAG_PSWINDEX2			(_T("��������������"))
#define TAG_PSWROW				(_T("ƽ�����б�ǩ"))
#define TAG_SGF					(_T("ʩ�����ǩ"))
#define TAG_YYBW				(_T("Ӧ�ò�λ"))
#define TAG_LAYROW				(_T("�����б�ǩ"))

//�ο�������
#define HXTZX_PW				(_T("����������[ƽ��]"))	//�˴�Ҫ���û��߱���ж���һ��
#define HXTZX_SW				(_T("����������[����]"))
#define ZXTZX					(_T("����������"))
#define GDTZX					(_T("�߶�������"))

// 

class CTHDataBase;
struct NM_THGRID;
class tendon_data;


// ������Ļ���������
class X_FRAMEWORK_API tendon_grid_environment_varible
{
public:
	tendon_grid_environment_varible();
	~tendon_grid_environment_varible();

public:
	std::vector<CString>      m_all_material;       // ��������������
	std::vector<CString>      m_all_anchor;         // ê����������
	std::vector<CString>      m_all_pipe;           // ���ƹ���������
	std::vector<CString>      m_all_linker;         // ��������������
	std::vector<CString>      m_all_chikuai;        // �ݿ�ê����������

	//----
	std::vector<CString>      m_all_swhxtzx;        // �������ͺ���������
	std::vector<CString>      m_all_swgdtzx;        // �������͸߶�������
	std::vector<CString>      m_all_pwhxtzx;        // ƽ�����ͺ���������
	std::vector<CString>      m_all_pwkzw;          // ƽ������ƽ�����λ
	std::vector<CString>      m_all_bzckx;          // �������òο�����
};


// ������񹤾���
class X_FRAMEWORK_API tendon_grid_tools
{
private:
	tendon_grid_tools();
	~tendon_grid_tools();

public:
	// ��ȡ��������ݲ�ȫ�����뵽P0���ݽṹ��
	static void get_data_from_grid(std::map<CString, CTHDataBase*>& all_dbs, tendon_data& data);
	static void get_data_from_grid_basetype(CTHDataBase* pdatabase, std::vector<tendon_data_basetype>& vec_basetype);
	static void get_data_from_grid_bendtype(CTHDataBase* pdatabase, std::vector<tendon_data_bend_type>& vec_bendtype);
	static void get_data_from_grid_anchor_plane(CTHDataBase* pdatabase, std::vector<tendon_data_anchor_plane>& vec_anchor_plane);
	static void get_data_from_grid_refline(CTHDataBase* pdatabase, std::vector<tendon_data_refline>& vec_refline);
	static void get_data_from_grid_tendontype(CTHDataBase* pdatabase, std::vector<tendon_data_type>& vec_refline);
	static void get_data_from_grid_tension_batch(CTHDataBase* pdatabase, std::vector<tendon_data_tension_batch>& vec_refline);
	static void get_data_from_grid_tendon_info(CTHDataBase* pdatabase, std::vector<tendon_data_info>& vec_tendon_info);

	// ���û������P0����ȫ�����뵽�����
	static void set_data_to_grid(std::map<CString, CTHDataBase*>& all_dbs, const tendon_data& data);
	static void set_data_to_grid_basetype(CTHDataBase* pdatabase, const std::vector<tendon_data_basetype>& vec_basetype);
	static void set_data_to_grid_bendtype(CTHDataBase* pdatabase, const std::vector<tendon_data_bend_type>& vec_bendtype, bool bSW, bool bHXTZ = false);
	static void set_data_to_grid_anchor_plane(CTHDataBase* pdatabase, const std::vector<tendon_data_anchor_plane>& vec_anchor_plane);
	static void set_data_to_grid_refline(CTHDataBase* pdatabase, const std::vector<tendon_data_refline>& vec_refline);
	static void set_data_to_grid_tendontype(CTHDataBase* pdatabase, const std::vector<tendon_data_type>& vec_refline);
	static void set_data_to_grid_tension_batch(CTHDataBase* pdatabase, const std::vector<tendon_data_tension_batch>& vec_tension_batch);
	static void set_data_to_grid_tendon_info(CTHDataBase* pdatabase, const std::vector<tendon_data_info>& vec_tendon_info);	

	// �õ����и�������ע������
	static void get_all_tendon_grid_template_names(std::vector<CString>& names);
	static std::vector<CString> get_all_tendon_grid_template_names();

	// ���ñ�񻷾��������ڼ��ر��ʱ���ã��� �贫�����б�����ݼ��Ѿ�ʵ�����ĸ�������������
	static void set_tendon_grid_variable(std::map<CString, CTHDataBase*>& all_dbs, const tendon_grid_environment_varible& src);

	// ����Ĭ��ֵ
	static void make_default_value_by_grid(tendon_data& data);

	// �Ƿ񵯳�������-ͬ���������ߡ��Ķ�����
	static void should_pop_tendon_sub_grid(CTHDataBase* db, NM_THGRID* notify_struct);

	// ���������͡��������͡�ƽ�����͵������б䶯ʱ�����»������
	static void reflesh_temporary_variable(CTHDataBase* db, NM_THGRID* notify_struct);

	// ��������л��������ĵ�Ԫ��ʱ�����ݻ���������뻷������ֵ
	static void set_enviroment_variable(CTHDataBase* db, NM_THGRID* notify_struct);

	// ���ú��������ߺ͸߶������ߣ�ƽ�����λ����������ѡ��
	// ��һ������Ϊ��������������ѡ����ڶ�������Ϊ�߶������ߣ�ƽ�����λ������ѡ�
	static void set_bendtype_droplist(std::vector<CString>& vechxtzx, std::vector<CString>& vecgdtzx, std::map<CString, CTHDataBase*>& all_dbs);

private:
	// ��ʼ��ʵʱ������������Ļ����������ȡ����ʱ���ã�
	static void init_temporary_variable(const tendon_data& data);

private:
	// ��������������ʵʱ���µĻ�����������ʱ�������������������ƣ������������ƣ�ƽ����������
	static std::vector<CString>                      m_basetype_names;			   // ������������
	static std::vector<tendon_data_bend_type>        m_all_ver_bend;			   // ������������
	static std::vector<tendon_data_bend_type>        m_all_hor_bend;			   // ƽ����������
};


// ע������/ƽ��������ͱ�/�������ͱ�
X_FRAMEWORK_API void reigste_tendon_grid_template();
