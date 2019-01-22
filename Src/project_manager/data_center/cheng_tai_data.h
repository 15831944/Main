#ifndef cheng_tai_data_h__
#define cheng_tai_data_h__

#pragma once

#include "boost/shared_ptr.hpp"
#include "x_framework/macro.h"
#include "x_framework/interface/data_center_interface.h"
#include "x_framework/interface/serialize_interface.h"

#include "graph_data_base.h"


typedef struct PROJECT_MANAGER_API SDianCeng
{
	SDianCeng();
	operator bool() const{ return m_set; }
	void serialize_members(member_rw_interface& mt);

	bool m_set;			//ϵ�������õ��
	double m_height;	//�����
	double m_width;		//����������
} SXiLiang;

class PROJECT_MANAGER_API CCTXiLiang
{
public:
	CCTXiLiang();
	void serialize_members(member_rw_interface& mt);
	operator bool() const{ return m_xl; }

	SXiLiang m_xl;		//����ϵ��
	SDianCeng m_dc;		//����ϵ�����
	std::pair<double, double> m_daoJiao;//������������
};

struct PROJECT_MANAGER_API SZhuangJi
{
	SZhuangJi();
	void serialize_members(member_rw_interface& mt);

	std::vector<std::vector<double>> m_x;
	std::vector<double> m_y;
};
class PROJECT_MANAGER_API CChengTaiJL//��̨����
{
public:
	CChengTaiJL();
	virtual void serialize_members(member_rw_interface& mt);

	double m_height;	//��̨���
	int m_buZhi;		//������ʽ---0:�������� 2:����б��
	double m_jiaodu;	//��б�Ƕ�-��λ��

	bool m_dingDaoJiao;	//���ö�����
	double m_dLenTopDJ;	//�����ǳߴ�

	double m_topLever;	//���߳�
	SDianCeng m_dc;		//��̨���
	SZhuangJi m_zj;		//׮��
	CCTXiLiang m_xl;
};

class PROJECT_MANAGER_API CJuXingCT : public CChengTaiJL
{
public:
	CJuXingCT();
	virtual void serialize_members(member_rw_interface& mt);
	double m_length;	//�������
	double m_width;		//�������
	std::pair<double, double> m_daoJiao;//������������
};

class PROJECT_MANAGER_API CYuanDuanCT : public CChengTaiJL
{
public:
	CYuanDuanCT();
	virtual void serialize_members(member_rw_interface& mt);
	double m_xianGao;		//�Ҹ�
	double m_banJing;		//�뾶
};

class PROJECT_MANAGER_API CYuanXingCT : public CChengTaiJL
{
public:
	CYuanXingCT();
	virtual void serialize_members(member_rw_interface& mt);
	double m_banJing;	//�뾶
};

class PROJECT_MANAGER_API cheng_tai_GZ
{
public:
	cheng_tai_GZ();
	void serialize_members(member_rw_interface& mt);
	int m_leiXing;		//��̨�˲�����---1:����  2:Բ���� 3:�н��� 4:Բ�������� 5��������
	std::vector<double> m_ctJianJu;//��̨��࣬��ϵ������
	std::vector<boost::shared_ptr<CChengTaiJL>> m_pCT; //�����̨,�����γ�̨ʱ����Ϊ����
};

// 
class PROJECT_MANAGER_API cheng_tai_data : public graph_data_base
{
	DECLARE_SERIAL(cheng_tai_data)
public:
	cheng_tai_data();
	~cheng_tai_data();

	virtual void serialize_members(member_rw_interface& mt);
public:
	cheng_tai_GZ m_gz;
};

#endif // cheng_tai_data_h__
