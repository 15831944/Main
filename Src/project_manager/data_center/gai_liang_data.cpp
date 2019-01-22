#include "stdafx.h"
#include "gai_liang_data.h"


GaiLiang_GJ_Base::GaiLiang_GJ_Base()
{
	vecSlope_ZX = "0,0";//���������¶�
	slope_Bot_ZX = 0;//�ײ������¶�
	vecWide = "2140,1860";//�������
	Width = 10900;//��������
	vecHeightMid = "2199.75,2800";//���µ㴦����
	WideMid_L = 5450;//���µ�����������
	bSameElevS_B = false;//��С׮�Ų�߳��Ƿ���ͬ

}
GaiLiang_GJ_Base::~GaiLiang_GJ_Base()
{
}
void GaiLiang_GJ_Base::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//����
		mt.write(_T("vecSlope_ZX"), vecSlope_ZX);
		mt.write(_T("slope_Bot_ZX"), slope_Bot_ZX);
		mt.write(_T("vecWide"), vecWide);
		mt.write(_T("Width"), Width);
		mt.write(_T("vecHeightMid"), vecHeightMid);
		mt.write(_T("WideMid_L"), WideMid_L);
		mt.write(_T("bSameElevS_B"), bSameElevS_B);

	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			//����
			mt.read(vecSlope_ZX);
			mt.read(slope_Bot_ZX);
			mt.read(vecWide);
			mt.read(Width);
			mt.read(vecHeightMid);
			mt.read(WideMid_L);
			mt.read(bSameElevS_B);

		}
	}
}


GaiLiang_GJ_DrawSet::GaiLiang_GJ_DrawSet()
{
	m_FZs = "";//�Զ��帽ע
	m_isMB = false;//�Ƿ���ͼģ��
	m_PLMScale = 50;//ƽ�����ͼ����
	m_DMScale = 50;//�����ͼ����
	m_gridScale = 50;//������ֽ�������
	m_sketlScale = 50;//�Ǽܴ�������
	m_is1_2Break = false;//�Գ�ʱ�Ƿ���1/2���۶�
	m_forceBreakScope = 3000;//�ǶԳ�����£��������ȴ��ڴ�ֵʱ����1/2���۶�
	m_DMAnnoType = "����ע��";//���湿���ŵ�ע�����ͣ�����ע�ͣ�ֱ��ע��
	m_FZScale = 50;


}
GaiLiang_GJ_DrawSet::~GaiLiang_GJ_DrawSet()
{
}
void GaiLiang_GJ_DrawSet::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//
		mt.write(_T("m_FZs"), m_FZs);//�Զ��帽ע
		mt.write(_T("m_isMB"), m_isMB);//�Ƿ���ͼģ��
		mt.write(_T("m_PLMScale"), m_PLMScale);//ƽ�����ͼ����
		mt.write(_T("m_DMScale"), m_DMScale);//�����ͼ����
		mt.write(_T("m_gridScale"), m_gridScale);//������ֽ�������
		mt.write(_T("m_sketlScale"), m_sketlScale);//�Ǽܴ�������
		mt.write(_T("m_is1_2Break"), m_is1_2Break);//�Գ�ʱ�Ƿ���1/2���۶�
		mt.write(_T("m_forceBreakScope"), m_forceBreakScope);//�ǶԳ�����£��������ȴ��ڴ�ֵʱ����1/2���۶�
		mt.write(_T("m_DMAnnoType"), m_DMAnnoType);//���湿���ŵ�ע�����ͣ�����ע�ͣ�ֱ��ע��
		mt.write(_T("m_FZScale"), m_FZScale);//�Զ��帽ע

	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			mt.read(m_FZs);//�Զ��帽ע
			mt.read(m_isMB);//�Ƿ���ͼģ��
			mt.read(m_PLMScale);//ƽ�����ͼ����
			mt.read(m_DMScale);//�����ͼ����
			mt.read(m_gridScale);//������ֽ�������
			mt.read(m_sketlScale);//�Ǽܴ�������
			mt.read(m_is1_2Break);//�Գ�ʱ�Ƿ���1/2���۶�
			mt.read(m_forceBreakScope);//�ǶԳ�����£��������ȴ��ڴ�ֵʱ����1/2���۶�
			mt.read(m_DMAnnoType);//���湿���ŵ�ע�����ͣ�����ע�ͣ�ֱ��ע��
			mt.read(m_FZScale);//�Զ��帽ע

		}
	}
}
//����
GaiLiang_GJ_DZ::GaiLiang_GJ_DZ()
{
	DZType = "Բ��";
	DZArrangeVerDis = -150;
	DZArrangeHorDis = "2500,9300";
	vecDZwidthH = "1600,1800";
	vecDZwidthZ = "1600,1800";
	vecDzR = "1500,1600";
	vecDZ_dj = "20,20";
	DZdjR = 100;
	DZdjInJog = true;

}
GaiLiang_GJ_DZ::~GaiLiang_GJ_DZ()
{
}
void GaiLiang_GJ_DZ::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//
		mt.write(_T("DZType"), DZType);
		mt.write(_T("DZArrangeVerDis"), DZArrangeVerDis);
		mt.write(_T("DZArrangeHorDis"), DZArrangeHorDis);
		mt.write(_T("vecDZwidthH"), vecDZwidthH);
		mt.write(_T("vecDZwidthZ"), vecDZwidthZ);
		mt.write(_T("vecDzR"), vecDzR);
		mt.write(_T("vecDZ_dj"), vecDZ_dj);
		mt.write(_T("DZdjR"), DZdjR);
		mt.write(_T("DZdjInJog"), DZdjInJog);
	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			mt.read(DZType);
			mt.read(DZArrangeVerDis);
			mt.read(DZArrangeHorDis);
			mt.read(vecDZwidthH);
			mt.read(vecDZwidthZ);
			mt.read(vecDzR);
			mt.read(vecDZ_dj);
			mt.read(DZdjR);
			mt.read(DZdjInJog);

		}
	}
}
//����
GaiLiang_GJ_DK::GaiLiang_GJ_DK()
{
	m_vecHX = "150,11500";
	m_vecZX = "0,30";
	m_Direct = "����";
	m_vecHeight = "750,750";
	m_vecSBWide = "450,450";
	m_vecXBWide = "600,600";
	m_vecAngleL = "90,90";
	m_vecAngleR = "90,90";


}
GaiLiang_GJ_DK::~GaiLiang_GJ_DK()
{
}
void GaiLiang_GJ_DK::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		mt.write(_T("m_vecHX"), m_vecHX);
		mt.write(_T("m_vecZX"), m_vecZX);
		mt.write(_T("m_Direct"), m_Direct);
		mt.write(_T("m_vecHeight"), m_vecHeight);
		mt.write(_T("m_vecSBWide"), m_vecSBWide);
		mt.write(_T("m_vecXBWide"), m_vecXBWide);
		mt.write(_T("m_vecAngleL"), m_vecAngleL);
		mt.write(_T("m_vecAngleR"), m_vecAngleR);

	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			mt.read(m_vecHX);
			mt.read(m_vecZX);
			mt.read(m_Direct);
			mt.read(m_vecHeight);
			mt.read(m_vecSBWide);
			mt.read(m_vecXBWide);
			mt.read(m_vecAngleL);
			mt.read(m_vecAngleR);

		}
	}
}
//��ʯ
GaiLiang_GJ_DS::GaiLiang_GJ_DS()
{
	DS_isRegular = true;// �Ƿ�����
	DS_vecDistY = "455,110";// ��ʯ����Yֵ
	DS_vecDistX = "180,540,900";// ��ʯ����Xֵ
	DS_Wide_hor = "500,500";// ����
	DS_Wide_ver = "500,500";// ���
	DS_HeightMid = "200,200";// ���ĸ߶�
	DS_Angle = "90,90";// �Ƕ�

}
GaiLiang_GJ_DS::~GaiLiang_GJ_DS()
{
}
void GaiLiang_GJ_DS::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//
		mt.write(_T("DS_isRegular"), DS_isRegular);
		mt.write(_T("DS_vecDistY"), DS_vecDistY);
		mt.write(_T("DS_vecDistX"), DS_vecDistX);
		mt.write(_T("DS_Wide_hor"), DS_Wide_hor);
		mt.write(_T("DS_Wide_ver"), DS_Wide_ver);
		mt.write(_T("DS_HeightMid"), DS_HeightMid);
		mt.write(_T("DS_Angle"), DS_Angle);

	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			mt.read(DS_isRegular);
			mt.read(DS_vecDistY);
			mt.read(DS_vecDistX);
			mt.read(DS_Wide_hor);
			mt.read(DS_Wide_ver);
			mt.read(DS_HeightMid);
			mt.read(DS_Angle);

		}
	}
}

GaiLiang_GJ_CSHLB::GaiLiang_GJ_CSHLB()
{


}
GaiLiang_GJ_CSHLB::~GaiLiang_GJ_CSHLB()
{
}
void GaiLiang_GJ_CSHLB::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//


	}
	else
	{
		mt.read(version);
		if (1 == version)
		{


		}
	}
}
//����������ϸ��Ϣ
GaiLiang_GJ_Detail::GaiLiang_GJ_Detail()
{
	m_PierType = "������";//�����ӵĶ�����
	m_RigidRotation = false;//������ת
	m_bGujie = false;//�̽�
	//
	vecSlope_ZX = "0,0";//���������¶�
	slope_Bot_ZX = 0;//�ײ������¶�
	//����ƽ������
	vecWide = "1000,1000";//�������
	Width = 11800;//��������
	vecAngle_LR = "90,90";//���Ҳ�����������߼н�
	//�������������
	vecUpDJ_S_left = "0,0,0,0";//����Ե���ǳߴ�
	vecDownDJ_S = "0,800,0,800";//��Ե���ǳߴ�
	//������������
	vecHeightMid = "1600,1820";//���µ㴦����
	WideMid_L = 5900;//���µ�����������
	vecHeight = "1020,1020";//�������Ҳ����˸߶�
	BGType = "ֱ��";//��߶�����
	vecWide_BG = "1350,1350";//���Ҳ��߶γ���
	Radius_BG = 10000;//Բ���뾶
	Radius_DB = 100;//�˲�С����Բ���뾶
	isDoubleslope = false;//�Ƿ���˫��
	slope_S = "0,0";//С׮�Ų���������¶�
	slope_B = "0,0";//��׮�Ų���������¶�
	slope_Bot = "0,0";//�ײ������¶�
	bSameElevS_B = 0;//��С׮�Ų�߳��Ƿ���ͬ
	WideBQ = 0;//��ǽ���

}
GaiLiang_GJ_Detail::~GaiLiang_GJ_Detail()
{
}
void GaiLiang_GJ_Detail::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
//
		mt.write(_T("m_PierType"), m_PierType);
		mt.write(_T("m_RigidRotation"), m_RigidRotation);
		mt.write(_T("m_bGujie"), m_bGujie);
//
		mt.write(_T("vecSlope_ZX"), vecSlope_ZX);
		mt.write(_T("slope_Bot_ZX"), slope_Bot_ZX);
		mt.write(_T("vecWide"), vecWide);
		mt.write(_T("Width"), Width);
		mt.write(_T("vecAngle_LR"), vecAngle_LR);
		mt.write(_T("vecUpDJ_S_left"), vecUpDJ_S_left);
		mt.write(_T("vecDownDJ_S"), vecDownDJ_S);
		mt.write(_T("vecHeightMid"), vecHeightMid);
		mt.write(_T("WideMid_L"), WideMid_L);
		mt.write(_T("vecHeight"), vecHeight);
		mt.write(_T("BGType"), BGType);
		mt.write(_T("vecWide_BG"), vecWide_BG);
		mt.write(_T("Radius_BG"), Radius_BG);
		mt.write(_T("Radius_DB"), Radius_DB);
		mt.write(_T("isDoubleslope"), isDoubleslope);
		mt.write(_T("slope_S"), slope_S);
		mt.write(_T("slope_B"), slope_B);
		mt.write(_T("slope_Bot"), slope_Bot);
		mt.write(_T("bSameElevS_B"), bSameElevS_B);
		mt.write(_T("WideBQ"), WideBQ);



	}
	else
	{
		mt.read(version);
		if (1 == version)
		{
			mt.read(m_PierType);
			mt.read(m_RigidRotation);
			mt.read(m_bGujie);
//
			mt.read(vecSlope_ZX);
			mt.read(slope_Bot_ZX);
			mt.read(vecWide);
			mt.read(Width);
			mt.read(vecAngle_LR);
			mt.read(vecUpDJ_S_left);
			mt.read(vecDownDJ_S);
			mt.read(vecHeightMid);
			mt.read(WideMid_L);
			mt.read(vecHeight);
			mt.read(BGType);
			mt.read(vecWide_BG);
			mt.read(Radius_BG);
			mt.read(Radius_DB);
			mt.read(isDoubleslope);
			mt.read(slope_S);
			mt.read(slope_B);
			mt.read(slope_Bot);
			mt.read(bSameElevS_B);
			mt.read(WideBQ);

		}
	}
}


IMPLEMENT_SERIAL(GaiLiang_GJ_data, graph_data_base, 1)
GaiLiang_GJ_data::GaiLiang_GJ_data()
{
}
GaiLiang_GJ_data::~GaiLiang_GJ_data()
{
}
void GaiLiang_GJ_data::serialize_members(member_rw_interface& mt)
{
	int version = 1;
	if (mt.is_storing())
	{
		mt.write(_T("version"), version);
		//
		mt.write(_T("m_Base"), m_Base);
		mt.write(_T("m_Detail"), m_Detail);
		mt.write(_T("m_DrawSet"), m_DrawSet);
		mt.write(_T("m_DZ"), m_DZ);
		mt.write(_T("m_DK"), m_DK);
		mt.write(_T("m_DS"), m_DS);


	}
	else
	{
		mt.read(version);
		if (1 == version)
		{

			mt.read(m_Base);
			mt.read(m_Detail);
			mt.read(m_DrawSet);
			mt.read(m_DZ);
			mt.read(m_DK);
			mt.read(m_DS);
		}
	}
	graph_data_base::serialize_members(mt);
}
