#pragma once

#include "x_framework/macro.h"
#include "x_framework/interface/data_center_interface.h"
#include "x_framework/interface/serialize_interface.h"
#include "graph_data_base.h"
//��ͼ����	
class PROJECT_MANAGER_API GaiLiang_GJ_DrawSet
{
public:
	GaiLiang_GJ_DrawSet();
	~GaiLiang_GJ_DrawSet();
public:
	void serialize_members(member_rw_interface& mt);
public:
	CString m_FZs;	//�Զ��帽ע
	bool m_isMB;//�Ƿ���ͼģ��
	double m_PLMScale;//ƽ�����ͼ����
	double m_DMScale;//�����ͼ����
	double m_gridScale;//������ֽ�������
	double m_sketlScale;//�Ǽܴ�������
	////////
	bool m_is1_2Break;//�Գ�ʱ�Ƿ���1/2���۶�
	double m_forceBreakScope;//�ǶԳ�����£��������ȴ��ڴ�ֵʱ����1/2���۶�
	CString	m_DMAnnoType;//���湿���ŵ�ע�����ͣ�����ע�ͣ�ֱ��ע��
	double	m_FZScale;//��ע��ͼ����

};
//����
class PROJECT_MANAGER_API GaiLiang_GJ_DZ
{
public:
	GaiLiang_GJ_DZ();
	~GaiLiang_GJ_DZ();
public:
	void serialize_members(member_rw_interface& mt);
public:
	CString DZType;//��������:����;Բ��;Բ�����Ǿ���;ֱ�ߵ��Ǿ���
	double DZArrangeVerDis;//�������ĵ��ο��ߵ�Y�����
	CString DZArrangeHorDis;//���������ĵ��ο��ߵ�X����
	CString vecDZwidthH;//������򳤶ȣ�������
	CString vecDZwidthZ;//�������򳤶ȣ�������
	CString vecDzR;//�����뾶
	CString vecDZ_dj;//���ǳߴ�
	double DZdjR;//���ǰ뾶
	bool DZdjInJog;//Բ����ʱ�Ƿ���͹

};
//����
class PROJECT_MANAGER_API GaiLiang_GJ_DK
{
public:
	GaiLiang_GJ_DK();
	~GaiLiang_GJ_DK();
public:
	void serialize_members(member_rw_interface& mt);
public:
	//������Ϣ
	CString m_vecHX;
	CString m_vecZX;
	CString m_Direct;//���鳯������Ϊ0������Ϊ1��

	//�ߴ���Ϣ
	CString m_vecHeight; //����߶ȣ��������
	CString m_vecSBWide;   //�����ϲ����ȣ��������
	CString m_vecXBWide;  //�����²����ȣ��������
	CString m_vecAngleL;  //·�߷���ƽ�棬С׮�Ų���߲࣬���ε�����ڽǣ���׮�Ų��С׮�ŷֽ��߲࣬���ε�����ڽ�
	CString m_vecAngleR;  //·�߷���ƽ�棬С׮�Ų���߲࣬���ε��Ҳ��ڽǣ���׮�Ų��С׮�ŷֽ��߲࣬���ε��Ҳ��ڽ�
};
//��ʯ
class PROJECT_MANAGER_API GaiLiang_GJ_DS
{
public:
	GaiLiang_GJ_DS();
	~GaiLiang_GJ_DS();
public:
	void serialize_members(member_rw_interface& mt);
public:
	bool DS_isRegular;// �Ƿ�����
	CString DS_vecDistY;// ��ʯ����Yֵ
	CString DS_vecDistX;// ��ʯ����Xֵ
	CString DS_Wide_hor;// ����
	CString DS_Wide_ver;// ���
	CString DS_HeightMid;// ���ĸ߶�
	CString DS_Angle;// �Ƕ�

};
class PROJECT_MANAGER_API GaiLiang_GJ_CSHLB
{
public:
	GaiLiang_GJ_CSHLB();
	~GaiLiang_GJ_CSHLB();
public:
	void serialize_members(member_rw_interface& mt);
public:


};

//����������Ϣ
class PROJECT_MANAGER_API GaiLiang_GJ_Base
{
public:
	GaiLiang_GJ_Base();
	~GaiLiang_GJ_Base();
public:
	void serialize_members(member_rw_interface& mt);
public:
	CString vecSlope_ZX;//���������¶�
	double slope_Bot_ZX;//�ײ������¶�
	CString vecWide;//�������
	double Width;//��������
	CString vecHeightMid;//���µ㴦����
	double WideMid_L;//���µ�����������
	bool bSameElevS_B;//��С׮�Ų�߳��Ƿ���ͬ



};
//����������ϸ��Ϣ
class PROJECT_MANAGER_API GaiLiang_GJ_Detail
{
public:
	GaiLiang_GJ_Detail();
	~GaiLiang_GJ_Detail();
public:
	void serialize_members(member_rw_interface& mt);
public:
	//������Ϣ
	CString m_PierType;//�����ӵĶ�����
	bool m_RigidRotation;//������ת
	bool m_bGujie;//�̽�


	//
	CString vecSlope_ZX;//���������¶�
	double slope_Bot_ZX;//�ײ������¶�
	//����ƽ������
	CString vecWide;//�������
	double Width;//��������
	CString vecAngle_LR;//���Ҳ�����������߼н�
	//�������������
	CString vecUpDJ_S_left;//����Ե���ǳߴ�
	CString vecDownDJ_S;//��Ե���ǳߴ�
	//������������
	CString vecHeightMid;//���µ㴦����
	double WideMid_L;//���µ�����������
	CString vecHeight;//�������Ҳ����˸߶�
	CString BGType;//��߶�����,0Ϊֱ�߶Σ�1λԲ��

	CString vecWide_BG;//���Ҳ��߶γ���
	double Radius_BG;//Բ���뾶
	double Radius_DB;//�˲�С����Բ���뾶
	bool isDoubleslope;//�Ƿ���˫��
	CString slope_S;//С׮�Ų���������¶�
	CString slope_B;//��׮�Ų���������¶�
	CString slope_Bot;//�ײ������¶�
	bool bSameElevS_B;//��С׮�Ų�߳��Ƿ���ͬ
	double WideBQ;//��ǽ���



};


class PROJECT_MANAGER_API GaiLiang_GJ_data : public graph_data_base
{
	DECLARE_SERIAL(GaiLiang_GJ_data)
public:
	GaiLiang_GJ_data();
	~GaiLiang_GJ_data();
private:
	virtual void serialize_members(member_rw_interface& mt);
public:

	GaiLiang_GJ_Base	m_Base;	//��������
	GaiLiang_GJ_Detail	m_Detail;	//������ϸ����
	GaiLiang_GJ_DrawSet	m_DrawSet;	//��ͼ����
	GaiLiang_GJ_DZ	m_DZ;	//����
	GaiLiang_GJ_DK	m_DK;	//����
	GaiLiang_GJ_DS	m_DS;	//��ʯ
	GaiLiang_GJ_CSHLB	m_CSHLB;	//��ʯ
private:
	//CTHDataBase;		
};
