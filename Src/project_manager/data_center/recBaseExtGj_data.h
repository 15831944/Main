#pragma once

#include "x_framework/macro.h"
#include "x_framework/interface/data_center_interface.h"
#include "x_framework/interface/serialize_interface.h"
#include "graph_data_base.h"






//ԭʼ����	
class PROJECT_MANAGER_API rectBaseGJ_org
{
public:
	rectBaseGJ_org();
	~rectBaseGJ_org();
public:
	void serialize_members(member_rw_interface& mt);
public:
	CString gz_enSZA;//���������ϡ��ع�����		
	bool gz_fApart;//����ʽ����		
	bool gzr_fPDsn;//�Ƿ�ƽ���ڵ�·�����		
	//__________________________________________________________
	double gzr_bKs;//���������bKs		
	double gzr_lKs;//����������lKs		
	double m_hKs_ADD;//�����������߶�hKs(mm)	
	// 
	CString gzr_hK;//���hK
	CString gzr_bcl;//���������bcl
	CString gzr_lcq;//ǰ��������lcq

	//
	bool m_bPG001Reinforce;//����������øֽ�		
	bool m_bPG001GouJin;//����������ù���
	//__________________________________________________________ 
	CString m_JiChuMat;//������������		
	double m_lkd;//����������������������		
	double m_DY;//�������������߾��߰��Ե������		
	bool m_fbS;//�����Ƿ����ó�����		


};
class PROJECT_MANAGER_API rectBaseGJ_gen
{
public:
	rectBaseGJ_gen();
	~rectBaseGJ_gen();
public:
	void serialize_members(member_rw_interface& mt);
public:
	CString gz_enPGTYPE;//�ı����������\U����̨�ı����������
	CString gz_zBs;//������������out
	CString gz_bBs;//����������ľ�out
	double gzr_aA;//б����out
	bool gz_fSplit;//�Ƿ��г�����out:���ϲ��ṹ����������
	double gz_bS;//��������out:
	bool gz_fSect;//�Ƿ��ͼΪ��������(����Ϊ�ڲ��߲�����)out
	double gz_zKs;//����������out:���ڶ�Ӧ���ϲ��ṹ�ĵױ��
	double gz_zKsr;//�����������Ҳ�out

};
class PROJECT_MANAGER_API rectBaseGJ_align
{
public:
	rectBaseGJ_align();
	~rectBaseGJ_align();
public:
	void serialize_members(member_rw_interface& mt);
public:
	bool m_fKJ;//�Ƿ������������		
	bool m_fGJ;//��������Ƿ����		
	double m_bKzx;//�����������ײ�ĺ���б��		
	double m_lKzx;//�����������ײ������б��		
	bool m_kDg;//��������Ƿ����ù���		

};
class PROJECT_MANAGER_API rectBaseGJ_Common
{
public:
	rectBaseGJ_Common();
	~rectBaseGJ_Common();
public:
	void serialize_members(member_rw_interface& mt);



public:
	double gzr_bKs;//���������bKs		
	double gzr_lKs;//����������lKs		
	double m_hKs_ADD;//�����������߶�hKs(mm)		
	CString gzr_hK;//���hK		
	CString gzr_bcl;//���������bcl		
	CString gzr_lcq;//ǰ��������lcq		
	bool m_bPG001Reinforce;//����������øֽ�		
	bool m_bPG001GouJin;//����������ù���		
	CString m_JiChuMat;//������������		

	//__________________________________________________________ 
};
class PROJECT_MANAGER_API RectBaseGJ_Detail
{
public:
	RectBaseGJ_Detail();
	~RectBaseGJ_Detail();
public:
	void serialize_members(member_rw_interface& mt);
public:
};


class PROJECT_MANAGER_API rectBaseGJ_data : public graph_data_base
{
	DECLARE_SERIAL(rectBaseGJ_data)
public:
	rectBaseGJ_data();
	~rectBaseGJ_data();
private:
	virtual void serialize_members(member_rw_interface& mt);
public:
	//������Ϣ
	//rectBaseGJ_Common	m_Common;
	//��ϸ��Ϣ
	rectBaseGJ_org	m_org;
	rectBaseGJ_gen	m_gen;
	rectBaseGJ_align	m_align;



private:
	//CTHDataBase;		
};


