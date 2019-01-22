#pragma once
//#include <utility>
#include "../interface/serialize_interface.h"
#include "../interface/data_center_interface.h"


class CViewSetting;
class CGTEditorAssistDataInfo;

enum refline_type
{
	TENDON_Acr_PW,            //���������ߣ�ƽ�䣩
	TENDON_Acr_SW,            //���������ߣ����䣩
	TENDON_Por,               //����������
	TENDON_Ver,               //�߶�������
	TENDON_UnKnow             //δѡ��
};

//  �νṹ(ƽ�������͵ļ�������)
class X_FRAMEWORK_API tendon_data_section
{
public:
	tendon_data_section();
	~tendon_data_section();

public:
	void serialize_members(member_rw_interface& mt);

public:
	//�νṹ���벿��
	CString		    m_x_curvename;               //����������
	double		    m_x;                         //X����(mm)
	CString		    m_y_curvename;               //�߶�������
	double		    m_y;                         //Y����(mm)
	double		    m_r;                         //���ɰ뾶m
	CString		    m_type;                      //��ģʽ��"LL"��ʾ���߶Σ�"LA"��ʾֱ����ǣ�"R"����ʾ�ο��߶Σ�"RR"��ʾ�ο��߶��е�Բ�����ɵ�
	double		    m_angle;                     //��ǣ�����m_typeΪ"LA"ʱ��Ч����λ���ǻ���
	CString		    m_line_dir;                  //m_s,m_eֱ�߷���:"F"��ʾ����"X"��ʾˮƽ��
	double		    m_s;
	double		    m_e;
	int			    m_semodifier;                //S,E��ȡ�����η�
	int			    m_rmodifier;                 //R��ȡ�����η�
	CString		    m_zu;		                 //�嶨��
};

// ����/ƽ������
class X_FRAMEWORK_API tendon_data_bend_type
{
public:
	tendon_data_bend_type();
	~tendon_data_bend_type();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString                             m_name;	               //��������
	CString                             m_porlines;            //���������ߣ�(������������Ч)
	std::vector<tendon_data_section>    m_sections;            //�νṹ
	CString                             m_joint_list;          //ʩ�����б�

	//�༭�����������
	CString	                            m_strObjectID;
};

//  ͬ����������
class X_FRAMEWORK_API tendon_data_userline
{
public:
	tendon_data_userline();
	~tendon_data_userline();

public:
	void serialize_members(member_rw_interface& mt);

	// �ַ�����vector<tendon_data_userline>���໥ת������
	static void string_to_vecUserline(const CString& text, std::vector<tendon_data_userline>& result);
	static void vecUserline_to_string(CString& text, const std::vector<tendon_data_userline>& vecUserline);


public:
	double       m_x;          // ����x(m)
	double       m_y;          // ����y(m)
	double       m_r;          // �뾶R(m)
};

// ��������P0����
class X_FRAMEWORK_API tendon_data_basetype
{
public:
	tendon_data_basetype();
	~tendon_data_basetype();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString       	m_name;                            //��������
	CString       	m_materail_type;                   //��������
	int		      	m_numbers;                         //����
	CString       	m_start_anchor;                    //��ʼê��
	CString       	m_end_anchor;                      //��ֹê��
	CString       	m_corrugated_pipe;                 //���ƹ�
	double	      	m_tension_stress;                  //��������Ӧ��MPa
	double	      	m_tension_concrete_strength;       //����ʱ������ǿ�ȱ�
	double	      	m_tension;                         //������ϵ��(%)
	CString	      	m_linker;                          //����������
};


// ��������P0����
class X_FRAMEWORK_API tendon_data_type
{
public:
	tendon_data_type();
	~tendon_data_type();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString	        m_name;		                	//������������
	CString         m_base_type;			    	//��������
	CString	        m_vertical_bend_type;	    	//��������
	CString	        m_horizon_bend_type;	    	//ƽ������
	CString	        m_tendon_property;		        //��������
	CString	        m_drawing_name;               	//ͼֽ������
	CString         m_sub_type;				    	//������
};

// ����ê��P0����
class X_FRAMEWORK_API tendon_data_anchor_plane
{
public:
	tendon_data_anchor_plane();
	~tendon_data_anchor_plane();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString				m_anchor_name;		        //ê������
	CString				m_anchor_tendon;         	//ê�̸���
	CString             m_gear_anchor_name;		    //�ݿ�ê������
	CString				m_reference_surface;		//�ο���
};

// �������α�P0����
class X_FRAMEWORK_API tendon_data_tension_batch
{
public:
	tendon_data_tension_batch();
	~tendon_data_tension_batch();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString		    	m_batch_name;			        //������������
	CString		    	m_tension_tendon;				//��������
};

// �Զ���ο���P0����
class X_FRAMEWORK_API tendon_data_refline
{
public:
	tendon_data_refline();
	~tendon_data_refline();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString                            m_name;             //����
	refline_type	                   m_type;             //�ο�������
	CString                            m_refline;          //�ο�������
	double                             m_dist;             //ƫ��ֵ
	std::vector<tendon_data_userline>  m_userline;         //Ϊ�ձ�ʾʹ�òο���ƫ��
	CString                            m_note;		       //��ע

	//�༭�����������
	CString                            m_guid;
};


// ��������P0����
class X_FRAMEWORK_API tendon_data_layout
{
public:
	tendon_data_layout();
	~tendon_data_layout();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString				m_tendon_name;		     	//��������
	CString				m_refline_name;				//�ο�������
	CString				m_portrait_layout;	     	//������		
	CString             m_cross_layout;             //������

	//�༭�����������
	CString		        m_guid;
};

// ����P0����
class X_FRAMEWORK_API tendon_data_info
{
public:
	tendon_data_info();
	~tendon_data_info();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString								m_application_pos;		//Ӧ�ò�λ
	std::vector<tendon_data_layout>	    m_all_layout;		    //��������
};




// �����ȡ�����и����������
class X_FRAMEWORK_API tendon_data : public data_node_base
{
	DECLARE_SERIAL(tendon_data)
public:
	tendon_data();
	~tendon_data();

public:
	void serialize_members(member_rw_interface& mt);

public:
	//tendon_data(const tendon_data& Other);
	//tendon_data& operator=(const tendon_data& Other);

	// �������vector����
	void clearall();

	//����Ӧ�ò�λ���������µĸ���
	tendon_data get_tendon_data_by_name(const CString& name)const;

public:
	CString                                    m_component_name;          //��������

	std::vector<tendon_data_basetype>          m_all_basetype;	          //��������
	std::vector<tendon_data_type>	           m_all_tendon_type;	      //��������
	std::vector<tendon_data_anchor_plane>      m_all_anchor_plane;        //����ê��
	std::vector<tendon_data_tension_batch>     m_all_tension_batch;       //��������
	std::vector<tendon_data_refline>           m_all_refline;             //�Զ���ο���
	std::vector<tendon_data_bend_type>         m_all_ver_bend_type;	      //��������
	std::vector<tendon_data_bend_type>         m_all_hor_bend_type;	      //ƽ������
	std::vector<tendon_data_info>              m_all_tendon_info;	      //����ʵ��

	////�༭����������� 2013.03.26 �������ǻ��������
	//CGTEditorAssistDataInfo*                   m_pSetting;

};
