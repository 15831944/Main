#pragma once

#include "x_framework/macro.h"
#include "x_framework/interface/data_center_interface.h"
#include "x_framework/interface/serialize_interface.h"

#include "graph_data_base.h"

//��ͼ����
class PROJECT_MANAGER_API eqgj_drawset	// ��ͼ����
{
public:
	eqgj_drawset();
	~eqgj_drawset();
public:
	void serialize_members(member_rw_interface& mt);
	// 
	bool fbq;	// �Ƿ����ñ�ǽ
	bool fnt;	// �Ƿ�����ţ��
	bool fck;	// �Ƿ����òۿ�
	bool fTM;	// �Ƿ�����̨ñ
	bool fQQ;	// �Ƿ�����ǰǽ
	bool fCQ;	// �Ƿ����ò�ǽ
	bool fDTB;	// �Ƿ����õ�����
	//
	double dScale;	// ��ͼ����
};

//��������
class PROJECT_MANAGER_API eqgj_nt
{
public:
	eqgj_nt();
	~eqgj_nt();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double m_hToBQ;	// ����ǽ������
	double m_bT;	// ����
	double m_hVer;	// ��ֱ�θ߶�
	double m_hX;	// ��б�θ߶�
};

class PROJECT_MANAGER_API eqgj_ck	// ��ǽ�ۿ�
{
public:
	eqgj_ck();
	~eqgj_ck();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double m_hToBQ;	// ����ǽ������
	double m_bT;	// �ۿڿ��
};

class PROJECT_MANAGER_API eqgj_ssf	// ��ǽ������Ԥ����
{
public:
	eqgj_ssf();
	~eqgj_ssf();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double m_h;		// �߶�
	double m_w;		// ���
};

class PROJECT_MANAGER_API eqgj_bq	// ��ǽ
{
public:
	eqgj_bq();
	~eqgj_bq();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double m_bT;	// ��ǽ����
	double m_bB;	// ��ǽ�׿�
	double m_H;		// ��ǽ���ƽ���߶�
	//double m_VerH;	// ��ǽ��ֱ�θ߶�
	double m_iSlope;// �������¶�

	eqgj_nt nt;		// ţ��
	eqgj_ck ck;		// �ۿ�
	eqgj_ssf ssf;	// ������Ԥ����
};

class PROJECT_MANAGER_API eqgj_tm	// ̨ñ
{
public:
	eqgj_tm();
	~eqgj_tm();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double	m_dH;	// �߶�
	double	m_dW;	// ���,������ǽ�׿�
	double	m_dDJ;	// ���ǳߴ�,��ֱ�ߴ���ͬ
};

class PROJECT_MANAGER_API eqgj_qq	// ǰǽ
{
public:
	eqgj_qq();
	~eqgj_qq();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double  m_dPosZ;// ������뱳ǽǰ�����
	double	m_dH;	// �߶�lunkuo
	double	m_dW;	// ���,������ǽ�׿�
	double	m_dDJ;	// ���ǳߴ�,��ֱ�ߴ���ͬ
};

class PROJECT_MANAGER_API eqgj_cq	// ��ǽ
{
public:
	eqgj_cq();
	~eqgj_cq();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double	m_dH;		// �߶�
	double	m_dW;		// ���
	double	m_dDisToBQ;	// �߳���ǽ��Ե�߶�
};

class PROJECT_MANAGER_API eqgj_dtb	// ������
{
public:
	eqgj_dtb();
	~eqgj_dtb();
public:
	void serialize_members(member_rw_interface& mt);

public:
	double	m_dH;	// �߶�
	double	m_dL;	// ���
	double	m_dW;	// ���
};

class PROJECT_MANAGER_API eqgj_eq	// ��ǽ
{
public:
	eqgj_eq();
	~eqgj_eq();
public:
	void serialize_members(member_rw_interface& mt);

public:
	bool	m_bLeft;	// �Ƿ�Ϊ����ǽ
	double	m_dTopLen;	// ���涥��
	double	m_dBotLen;	// ����׿�
	double	m_dWide;	// ���
	double	m_dDisToBQ;	// �߳���ǽ��Ե�߶�
	double	m_dHVer;	// ��ֱ�θ߶�
	double	m_dhX;		// ��б�θ߶�
	double  m_dSlope;	// ��Ե�¶�
	bool    m_bSetDJ;	// �Ƿ����õ���
	double  m_dDJWide;	// ���ǿ��
	double  m_dDJHei;	// ���Ǹ߶�
	bool	m_bHorToMLBot; // �Ƿ���ñ����ˮƽ
	double	m_dAnglePM;	// ƽ����б�Ƕ�-����

};


// 
class PROJECT_MANAGER_API eq_gj_data : public graph_data_base
{
	DECLARE_SERIAL(eq_gj_data)

public:
	eq_gj_data();
	~eq_gj_data();

private:
	virtual void serialize_members(member_rw_interface& mt);
public:
	CString	m_strName;	 // ��̨����
	eqgj_eq m_EQ;		 // ��ǽ
	eqgj_bq m_BQ;		 // ��ǽ
	eqgj_tm m_TM;		 // ̨ñ
	eqgj_qq m_QQ;		 // ǰǽ
	eqgj_cq m_CQ;		 // ��ǽ
	eqgj_dtb m_DTB;		 // ������

	eqgj_drawset drawset;// ��ͼ����
};

