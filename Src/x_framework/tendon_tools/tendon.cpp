#include "StdAfx.h"
#include "THDataWareHouse/THDataWareHouseInc.h"
#include "THDWAccessory/THDWAccessoryInc.h"

#include "HCurveAlgor/HCurveAlgorInc.h"
#include "BridgeRoad/BridgeRoadinc.h"
#include "../ExchangeUI/GWTendon_ExchangeUI.h"
#include "../FormView/GWTendon_Form.h"
#include "tendon.h"
#include "GWP1manager/log.h"

using namespace THSF;


CGWTendon::CGWTendon()
{
	m_pTendonAlgor = NULL;
}

CGWTendon::~CGWTendon()
{
	delete m_pTendonAlgor;
	m_pTendonAlgor = NULL;
}


void CGWTendon::InitTendonAlgor()
{
	delete m_pTendonAlgor;
	m_pTendonAlgor = new CTendon(m_P0.m_CompName);
	//��database����P0����
	PU2P0();
	//���òο���
	SetRefCurve();
	//�����㷨����ֵ
	SetTendonData();
}

void CGWTendon::InitTendonAlgor2(const CGWTendon_P0& P0)
{
	m_P0 = P0;
	delete m_pTendonAlgor;
	m_pTendonAlgor = new CTendon(m_P0.m_CompName);
	//���òο���
	SetRefCurve();
	//�����㷨����ֵ
	SetTendonData();
}

void CGWTendon::SetRefCurve()
{
	//��ӹ����ο���
	for (int i = 0; i < static_cast<int>(m_ComponentLines.size()); ++i)
	{
		CRefCurve Curve;
		Curve.m_Note = m_ComponentLines[i];
		m_pTendonAlgor->AddRefCurve(Curve);
	}

	//����û��Զ���ο���
	for (int i = 0; i < static_cast<int>(m_P0.m_vecRefLine.size()); ++i)
	{
		const tendon_data_refline& RefLine = m_P0.m_vecRefLine[i];
		CRefCurve RefCurve;
		if (RefLine.m_userline.empty())
		{
			RefCurve.m_Note.Format("%s+%s+offset+%d", RefLine.m_name, RefLine.m_refline, RefLine.m_dist);
		}
		else
		{
			RefCurve.m_Note = RefLine.m_name;
			vector<HCURVE_ALGOR::CCurveItem> vecItem;
			for (int j = 0; j < static_cast<int>(RefLine.m_userline.size()); ++j)
			{
				HCURVE_ALGOR::CCurveItem item;
				item.	m_x = RefLine.m_userline[j].m_x;
				item.	m_y = RefLine.m_userline[j].m_y;
				item.	m_r = RefLine.m_userline[j].m_r;
				vecItem.push_back(item);
			}
			CTHCurve Curve;
			HCURVE_ALGOR::GetTHCurve(Curve, vecItem, 1e-3);
			RefCurve.m_RefCurve.reset(new CTHCurve(Curve));
			m_pTendonAlgor->AddRefCurve(RefCurve);
		}
	}
}

void CGWTendon::AddComponentLineNames(CString Name)
{
	m_ComponentLines.push_back(Name);
}

void CGWTendon::GetRegisterTableName(CStringArray& strArr, const CString& strID)
{

	strArr.Add(TENDON_BASETYPE+strID);//��������
	strArr.Add(TENDON_TYPE+strID);//��������
	strArr.Add(TENDON_GSMM+strID);//ê��
	strArr.Add(TENDON_GSZLPC+strID);//��������
	strArr.Add(TENDON_REFLINE+strID);//�û���
	strArr.Add(TENDON_USERLINE+strID);//�û���
	//�ֶ���Ҳע�ᣬ�������л�
	strArr.Add(TENDON_SWLX+strID);//��������
	strArr.Add(TENDON_PWLX+strID);//ƽ������
	strArr.Add(TENDON_LAY+strID);//��������	
}


CTHDataBase* CGWTendon::CreatePSWTypeDataBase(vector<int> vecRowNum, bool bSW, bool bHXTZ, bool bSGF)
{
	CString strTitle = bSW?"��������":"ƽ������";
	CString strColumn = bSW?"�߶�������":"ƽ�����λ";

	CTHDataBase* pdatabase = GWCreateDatabase();
	pdatabase->SetExtendData("bSGF", bSGF?"1":"0");
	//����ģ����
	CString strTemplateName = bSW?TENDON_SWLX:TENDON_PWLX;
	//pdatabase->SetAsTemplate(TRUE);
	pdatabase->SetID(strTemplateName);

	pdatabase->SetRowCount(9);
	pdatabase->SetColCount(13);
	pdatabase->SetFixedRowCount(1);
	pdatabase->SetFixedColCount(1);

	pdatabase->EnableRowAutoSerialNumber(TRUE);
	pdatabase->EnableColAutoSerialNumber(TRUE);
	pdatabase->EnableMergeMenu(TRUE);
	pdatabase->EnableColMenu(FALSE);
	pdatabase->EnableRowGroup(TRUE);
	
	//Tag����
	pdatabase->SetRowTag(3, TAG_PSWINDEX);
	pdatabase->SetRowTag(4, TAG_PSWINDEX2);
	pdatabase->SetRowTag(5, TAG_SGF);
	pdatabase->SetRowTag(8, TAG_PSWROW);
	//////////////////////////////////////////////////////////////////////////
	pdatabase->MergeCells(1,1,1,12);
	pdatabase->SetTitleFormat(1, 1, RT_TABLETITLE);
	CTHCell *pTempCell =pdatabase->GetCell(1,1);
	pTempCell->SetText(strTitle);
	pTempCell->SetReadOnly(TRUE);

	pdatabase->MergeCells(2,2,2,12);
	pTempCell =pdatabase->GetCell(2,2);
	pTempCell->SetText("��������");
	pTempCell->SetAlignment(DT_CENTER|DT_VCENTER|DT_HOR);
	pTempCell->SetReadOnly(TRUE);
	pTempCell->SetHintText("�����������");
	pdatabase->SetTitleFormat(2, 2, RT_SUBTITLE);

	pTempCell=pdatabase->GetCell(3,2);
	pTempCell->SetText("��������");
	pTempCell->SetReadOnly(TRUE);

	pdatabase->MergeCells(3,3,3,12);
	pTempCell=pdatabase->GetCell(3,3);
	CString strType = bSW?"��������":"ƽ������";
	pTempCell->SetHintText(strType + "��������/n��ʽΪ:��������[����]/n����������,���Ÿ���/n��:������[A]��������[A,B,C,D]");

	pTempCell=pdatabase->GetCell(4,2);
	pTempCell->SetText("����������");
	pTempCell->SetReadOnly(TRUE);

	pdatabase->MergeCells(4,3,4,12);
	pTempCell=pdatabase->GetCell(4,3);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetHintText("������������Чʱ��ʾ�������������߷�����͸���,\n��Чʱ��ʾ������������i�ľ�����ͬ");


	pTempCell=pdatabase->GetCell(5,2);
	pTempCell->SetText("ʩ����");
	pTempCell->SetReadOnly(TRUE);

	pdatabase->MergeCells(5,3,5,12);
	pTempCell=pdatabase->GetCell(5,3);
	pTempCell->SetCellType(CT_CHECKDROPLIST);
	//pTempCell->SetLabelText("@SGF");
	pTempCell->SetHintText("ʩ��������");

	//��������༭��guidssss
	//pdatabase->MergeCells(5,2,5,12);
	//////////////////////////////////////////////////////////////////////////

	pdatabase->MergeCells(6,2, 6,12);
	pTempCell =pdatabase->GetCell(6,2);
	pTempCell->SetText("��������");
	pTempCell->SetAlignment(DT_CENTER|DT_VCENTER|DT_HOR);
	pTempCell->SetReadOnly(TRUE);
	pdatabase->SetTitleFormat(6, 2, RT_SUBTITLE);

	pTempCell=pdatabase->GetCell(7,2);
	pTempCell->SetText("���");

	pTempCell=pdatabase->GetCell(7,3);
	pTempCell->SetText("����������");

	pTempCell=pdatabase->GetCell(7,4);
	pTempCell->SetText("X����(mm)");
	pTempCell->SetHintText("������������ߵľ���");

	pTempCell=pdatabase->GetCell(7,5);
	pTempCell->SetText(strColumn);

	pTempCell=pdatabase->GetCell(7,6);
	pTempCell->SetText("Y����(mm)");
	pTempCell->SetHintText("����" + strColumn + "�ľ���");

	pTempCell=pdatabase->GetCell(7,7);
	pTempCell->SetText("���ɰ뾶R(m)");
	pTempCell->SetHintText("���ɰ뾶");

	pTempCell=pdatabase->GetCell(7,8);
	pTempCell->SetText("��ģʽ");	

	pTempCell=pdatabase->GetCell(7,9);
	pTempCell->SetText("����");

	pTempCell=pdatabase->GetCell(7,10);
	pTempCell->SetText("����ֱ�߳�(mm)");

	pTempCell=pdatabase->GetCell(7,11);
	pTempCell->SetText("��ƫ��");
	pTempCell->SetHintText("��ƫ��");
	
	//pTempCell=pdatabase->GetCell(8,2);

	pTempCell=pdatabase->GetCell(8,3);
	pTempCell->SetCellType(CT_DROPLIST);
	if (bHXTZ)
		pTempCell->SetReadOnly(TRUE);

	pTempCell=pdatabase->GetCell(8,4);
	pTempCell->SetHintText("������������ߵľ���");
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	if (bHXTZ)
		pTempCell->SetReadOnly(TRUE);

	pTempCell=pdatabase->GetCell(8,5);
	pTempCell->SetCellType(CT_DROPLIST);

	pTempCell=pdatabase->GetCell(8,6);
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	pTempCell->SetHintText("����"+ strColumn + "�ľ���");

	pTempCell=pdatabase->GetCell(8,7);
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	pTempCell->SetHintText("���ɰ뾶R:�������¶���ӹ��ɰ뾶,\n��LL/LA/RRģʽ����Ч\
\nR>=0:RΪ�̶�ֵ\nR<0: |R|Ϊ����ֵ,���ɱ�");
	pTempCell->SetMaskText("����-��ģʽR&&RR#12385");

	pTempCell=pdatabase->GetCell(8,8);
	pTempCell->SetCellType(CT_DROPDOWN);
	pTempCell->SetLabelText("LL\nR\nRR");
	pTempCell->SetHintText("��LA/LL/R/RR����ģʽ\nLA:���ʽ\nLL:����ʽ\
\nR:�ο�ƽ��ʽ\nRR:ָ��λ�ô�Բ����\n����LAģʽ,���ڴ˵�Ԫ��ֱ������Ƕ�ֵ");

	pTempCell=pdatabase->GetCell(8,9);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetLabelText("����\nˮƽ");
	pTempCell->SetHintText("��������ֱ�߶γ���ʱ�Ĳ�෽��\nˮƽ:���뷽��Ϊֱ�߶ζ˵���ˮ\
\nƽ���ϵ�ͶӰ��֮��ľ���\
\n����:���뷽��Ϊ���ε����߷���\nע:��������LA/R/RRģʽ����Ч");

	pTempCell=pdatabase->GetCell(8,10);
	pTempCell->SetDataType(CELLDATA_STRING);
	pTempCell->SetHintText("���ο��Ƶ�����X/Y������ֱ�߶γ���(S2,E2)\n������ֵ:����ֵΪ�̶�ֵ\
\n���븺ֵ:����ֵ�ľ���ֵΪ��Сֵ\nע:��������LA/R/RRģʽ����Ч");
	pTempCell->SetSketchMapName("����-��ģʽR&&RR#99999");

	pTempCell=pdatabase->GetCell(8,11);
	pTempCell->SetDataType(CELLDATA_STRING);
	pTempCell->SetHintText("��������Լ��:X/Y/R/A/S/E\
\n����Լ������:\
\n���Կɱ�ʽ:�������޹�ϵ,ͼ���п����ɲ���\
\n��Կɱ�ʽ:��һ��Ϊ��׼,�������,ͼ���в�����׼ʱ��������,\
\n��������ʱ���ı��ֵ\
\n��Բ���ʽ:��һ�����������ж���,���λ�ò���\
\nԼ�����巽ʽ:��XΪ��\
\n���Կɱ�ʽ:X=100,@,300\
\n��Կɱ�ʽ:@X=100,@,300 \
\n��Բ���ʽ:@@X = @,200,300\
\nע:=�Ҳ���ʽ��@��ʾ��׼λ,��=����ֵ\
\n���ͱ��ʱԼ����ʽ���б𷽷�:\
\n���Կɱ�ʽ:=���ΪX\
\n��Կɱ�ʽ:=���Ϊ@X\
\n��Բ���ʽ:=���Ϊ@@X����X���ʽ\
\n������Ϣ:\
\n==:��ʾ�Գ�,�Գ���ȫ�ṹͳһ,�����û�����,��Ҫ����ʱ��Ӷ�\
\n��������(Ԥ��ȷ����)\
\n!2-3:��ʾ2��3����������Ч");
	
	//����m_semodifier,m_rmodifier
	//pTempCell=pdatabase->GetCell(8,12);
	//////////////////////////////////////////////////////////////////////////

	//�иߡ��п�
	pdatabase->SetRowHeight(0,0);
	if (!bSW)
	{
		pdatabase->SetRowHeight(4,0);
	}

	if (!bSW || !bSGF)
	{//ʩ����ֻ�������ʱ�����
		pdatabase->SetRowHeight(5,0);
	}

	pdatabase->SetColWidth(0,0);
	pdatabase->SetColWidth(1, 30);
	pdatabase->SetColWidth(2, 80);
	pdatabase->SetColWidth(3, 80);
	pdatabase->SetColWidth(4, 80);
	pdatabase->SetColWidth(5, 80);
	pdatabase->SetColWidth(6, 80);
	pdatabase->SetColWidth(7, 80);
	pdatabase->SetColWidth(8, 75);
	pdatabase->SetColWidth(9, 75);
	pdatabase->SetColWidth(10, 90);
	pdatabase->SetColWidth(11, 80);
	pdatabase->SetColWidth(12, 0);
	//��������Ϣ
	CString strBigRow = bSW?CString("��������"):CString("ƽ������");
	CString strSmallRow = bSW?CString("���䶨��"):CString("ƽ�䶨��");
	pdatabase->SetAsRowGroup(8,8,2, strSmallRow,TRUE,7);
	pdatabase->SetAsRowGroup(2,8,1, strBigRow, TRUE, 1);

	int nGroupNum = static_cast<int>(vecRowNum.size());
	pdatabase->InsertRowPre(2, nGroupNum-1, strBigRow);
	
	int count = pdatabase->GetSubDataBaseCount();
	for (int i=0;i<count;++i)
	{
		CGridDataBase subDataBase;//������
		pdatabase->GetSubDataBase(i,subDataBase);
		int count1=subDataBase.GetSubDataBaseCount();
		for (int j=0;j<count1;j++)
		{
			CGridDataBase subDataBase1;//����
			subDataBase.GetSubDataBase(j,subDataBase1);
			int count2= subDataBase1.GetSubDataBaseCount();
			for (int m= 0; m < count2; m++)
			{
				if (m==1)
				{
					CGridDataBase tempDataBase;//������
					subDataBase1.GetSubDataBase(m,tempDataBase);
					int rowCount = tempDataBase.GetRowCount()-1;
					CString startRowTag =tempDataBase.FindStartRowTag(rowCount);
					CString rowTag = tempDataBase.GetRowTag(rowCount);
					int rowNo= pdatabase->FindRow(rowTag,startRowTag);
					int subCount = 0;
					if (j < static_cast<int>(vecRowNum.size()))
						subCount = vecRowNum[j] - 1;
					pdatabase->InsertRowPost(rowNo,subCount, strSmallRow);
				}
			}
		}
	}

	//���ñ���
	pdatabase->SetTitleFormat(1, 1, RT_TABLETITLE);

	return pdatabase;
}

//���ú��������뵥Ԫ�����η�
void SetXiuShiFu(CTHFormatCellInfo& cif)
{
	//��ʽ����
	cif.m_strFormatIDName = "����-Ӧ�ò�λ";

	//Ĭ������
	cif.m_nDefaultRowNum = 1;

	//�Ƿ��й̶�
	cif.m_nRowNumFixed = false;

	//����
	cif.m_strRowName = "Ӧ�ò�λ";

	//��ֵԭ��
	//m_strValueAddType = strValueAddType;  //Modify By RYC 2010.12.14 PM Bug Fixed!

	//����
	size_t i = 0;

	cif.m_ColList.clear();
	//����Ϣ��ȡ
	//for (i=0;i<nColNum;i++)ֻ��1��
	//{
	CTHOneCellColumnInfo ci;
	//������
	ci.m_strTitle = "���";

	//�������� 
	ci.m_strVarName = "m_xh";

	//2��������
	ci.m_strVarType = "STRING";

	//��������

	//���η��ķ�������
	ci.m_strQualiferNameList = "Z/B";

	//Ĭ�����η�
	ci.m_strDefaultQualifer = "Z";

	//���η��ķ������
	ci.m_strQualiferNoteList = "����";

	//��ϸ����
	ci.m_strQualiferDetailNoteList = "Z(�к���);B(�ߺ���)";

	//ʾ��ͼ
	//ci.m_strGraphicNameList = strValue;

	//�Ƿ���������
	ci.m_strPermitOrder = "��";       

	//��Ĭ��ֵ
	ci.m_strDefaultValue = "";       

	cif.m_ColList.push_back(ci);
	//}
}



/*�ֶ������������ñ�*/
CTHDataBase*  CGWTendon::CreateTendonLayDataBase(vector<int> vecRowNum)
{
	CTHDataBase* pdatabase = GWCreateDatabase();
	//����ģ����
	pdatabase->SetID(TENDON_LAY);	 

	pdatabase->SetRowCount(5);
	pdatabase->SetColCount(8);
	pdatabase->SetFixedRowCount(1);
	pdatabase->SetFixedColCount(1);

	//pdatabase->EnableRowAutoSerialNumber(TRUE);
	//pdatabase->EnableColAutoSerialNumber(TRUE);
	//pdatabase->EnableMergeMenu(TRUE);
	//pdatabase->EnableColMenu(FALSE);

	pdatabase->EnableRowGroup(TRUE);
	
	//����TAG
	pdatabase->SetRowTag(2, TAG_YYBW);
	pdatabase->SetRowTag(4, TAG_LAYROW);
	//////////////////////////////////////////////////////////////////////////
	pdatabase->MergeCells(1,1,1,7);
	CTHCell *pTempCell =pdatabase->GetCell(1,1);
	pTempCell->SetText("��������");
	pTempCell->SetReadOnly(TRUE);

	pTempCell=pdatabase->GetCell(2,2);
	pTempCell->SetText("Ӧ�ò�λ");
	pTempCell->SetReadOnly(TRUE);

	pdatabase->MergeCells(2,3,2,7);
	pTempCell=pdatabase->GetCell(2,3);
	pTempCell->SetCellType(CT_CHECKDROPLIST);
	//pTempCell->SetCellType(CT_TASKEDIT);
	//Ӧ�ò�λformatedit
	//CTHFormatCellInfo cif;
	//SetXiuShiFu(cif);
	////AddQualiferInfo(pData->GetTemplateName(),cif);
	//pTempCell->SetFormatorName(cif.m_strFormatIDName);
	//pdatabase->AddFormat(cif);

	///////////////////////////////////////////////////////////////////////////
	pTempCell=pdatabase->GetCell(3,2);
	pTempCell->SetText("���");

	pTempCell=pdatabase->GetCell(3,3);
	pTempCell->SetText("��������");
	pTempCell->SetHintText("������,��ʽ:��������/��������");
	
	pTempCell=pdatabase->GetCell(3,4);
	pTempCell->SetText("�ο�����");

	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetHintText("hint");

	pTempCell=pdatabase->GetCell(3,5);
	pTempCell->SetText("������(mm)");
	pTempCell->SetHintText("���������������\
\n�����ʽΪ���1,���2,���3...,������������\
\n������дΪn*s�ĸ�ʽ,n��ʾ����,s��ʾ���\
\n��������������֧��+��,��F1+F2,��ʾF1��F2�������\
\n��������������֧��-��,��-F1,��ʾF1���͵ĸ���ƽ��\
\n���갴��X�᾵��(Y����ȡ��)\
\n���Ϊ0ʱ��ʾ�ο��ߵ�λ��,�Լ��ο��ߴ����ø���\
\n���Ϊ0,0��ʾ�ο��ߴ��Լ��ο��ߴ������ø���\
\n��1:100,200,0,200,100,F1\
\n��ʾ����ο���-300,-200,0,200,300λ�ô�\
\n��������ΪF1�ĸ���\
\n��1:100,200,0,0,200,100,F1+F2\
\n��ʾ����ο���-300,-200,200,300λ�ô�\
\n��������ΪF1��F2�ĸ���");
	pTempCell=pdatabase->GetCell(3,6);
	pTempCell->SetText("������");
	pTempCell->SetHintText("���뷽ʽ�������һ��");

	//Ϊ�����༭�������guid
	//GetCell(3,7);	

	pTempCell=pdatabase->GetCell(4,3);
	pTempCell->SetHintText("������,��ʽ:��������/��������");

	pTempCell=pdatabase->GetCell(4,4);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetHintText("hint");

	pTempCell=pdatabase->GetCell(4,5);
	pTempCell->SetHintText("���������������\
\n�����ʽΪ���1,���2,���3...,������������\
\n������дΪs*n�ĸ�ʽ,s��ʾ���,n��ʾ����,\
\n��������������֧��+��,��F1+F2,��ʾF1��F2�������\
\n��������������֧��-��,��-F1,��ʾF1���͵ĸ���ƽ��\
\n���갴��X�᾵��(Y����ȡ��)\
\n���Ϊ0ʱ��ʾ�ο��ߵ�λ��,�Լ��ο��ߴ����ø���\
\n���Ϊ0,0��ʾ�ο��ߴ��Լ��ο��ߴ������ø���\
\n��1:100,200,0,200,100,F1\
\n��ʾ����ο���-300,-200,0,200,300λ�ô�\
\n��������ΪF1�ĸ���\
\n��1:100,200,0,0,200,100,F1+F2\
\n��ʾ����ο���-300,-200,200,300λ�ô�\
\n��������ΪF1��F2�ĸ���");

	pTempCell=pdatabase->GetCell(4,6);
	pTempCell->SetHintText("�ο���Ϊ��һT��0�ſ�������\
\n���뷽ʽ�������һ��\
\nֻ�貼�õ�һT����,������Զ��ԳƵ�����T");
	
	//�иߡ��п�
	pdatabase->SetRowHeight(0,0);

	pdatabase->SetColWidth(0,0);
	pdatabase->SetColWidth(1, 30);
	pdatabase->SetColWidth(2, 80);
	pdatabase->SetColWidth(3, 180);
	pdatabase->SetColWidth(4, 180);
	pdatabase->SetColWidth(5, 180);
	pdatabase->SetColWidth(6, 180);
	pdatabase->SetColWidth(7, 0);

	//��������Ϣ
	CString strSmallRow = "С����";
	CString strBigRow = "������";
	pdatabase->SetAsRowGroup(4,4,2, strSmallRow,TRUE, 3);
	pdatabase->SetAsRowGroup(2,4,1, strBigRow, TRUE);

	int nGroupNum = static_cast<int>(vecRowNum.size());
	pdatabase->InsertRowPre(2, nGroupNum-1, strBigRow);

	int count = pdatabase->GetSubDataBaseCount();
	for (int i=0;i<count;++i)
	{
		CGridDataBase subDataBase;//������
		pdatabase->GetSubDataBase(i,subDataBase);
		int count1=subDataBase.GetSubDataBaseCount();
		for (int j=0;j<count1;j++)
		{
			CGridDataBase subDataBase1;//����
			subDataBase.GetSubDataBase(j,subDataBase1);
			int count2= subDataBase1.GetSubDataBaseCount();
			for (int m= 0; m < count2; m++)
			{
				if (m==1)
				{
					CGridDataBase tempDataBase;//������
					subDataBase1.GetSubDataBase(m,tempDataBase);
					int rowCount = tempDataBase.GetRowCount()-1;
					CString startRowTag =tempDataBase.FindStartRowTag(rowCount);
					CString rowTag = tempDataBase.GetRowTag(rowCount);
					int rowNo= pdatabase->FindRow(rowTag,startRowTag);
					int subCount = 0;
					if (j < static_cast<int>(vecRowNum.size()))
						subCount = vecRowNum[j] - 1;
					pdatabase->InsertRowPost(rowNo,subCount, strSmallRow);
				}
			}
		}
	}

	pdatabase->SetTitleFormat(1, 1, RT_TABLETITLE);

	return pdatabase;
}

void CGWTendon::AddDataBase(const CPUObj* pPU)
{
	m_pTableData.clear();
	if (!pPU)
	{
		ASSERT(FALSE);
		return;
	}
	
	//���ù�������
	//m_P0.m_CompName = const_cast<CPUObj*>(pPU)->GetObjName();

	//new changed on 2011.06.15
	m_pTableData[TENDON_BASETYPE] = pPU->FindUserData(TENDON_BASETYPE+m_strID);
	m_pTableData[TENDON_TYPE]           = pPU->FindUserData(TENDON_TYPE+m_strID);
	m_pTableData[TENDON_GSMM]       = pPU->FindUserData(TENDON_GSMM+m_strID);
	m_pTableData[TENDON_GSZLPC]     = pPU->FindUserData(TENDON_GSZLPC+m_strID);
	m_pTableData[TENDON_REFLINE]     = pPU->FindUserData(TENDON_REFLINE+m_strID);
	m_pTableData[TENDON_PWLX]         = pPU->FindUserData(TENDON_PWLX+m_strID);
	m_pTableData[TENDON_SWLX]         = pPU->FindUserData(TENDON_SWLX+m_strID);
	m_pTableData[TENDON_LAY]            = pPU->FindUserData(TENDON_LAY+m_strID);
};

void CGWTendon::PU2P0()
{
	tendon_exchange_ui::pu_to_data_basetype(m_pTableData[TENDON_BASETYPE],  m_P0.m_vecBaseType);
	tendon_exchange_ui::pu_to_data_tendontype(m_pTableData[TENDON_TYPE],     m_P0.m_vecTendonType);
	tendon_exchange_ui::pu_to_data_anchor_plane(m_pTableData[TENDON_GSMM],       m_P0.m_vecAnchorP);         
	tendon_exchange_ui::pu_to_data_tension_batch(m_pTableData[TENDON_GSZLPC],               m_P0.m_veczlpc);
	tendon_exchange_ui::pu_to_data_refline(m_pTableData[TENDON_REFLINE],          m_P0.m_vecRefLine);

	tendon_exchange_ui::pu_to_data_bendtype(m_pTableData[TENDON_PWLX],         m_P0.m_vecPWType);
	tendon_exchange_ui::pu_to_data_bendtype(m_pTableData[TENDON_SWLX],         m_P0.m_vecSWType);    
	tendon_exchange_ui::pu_to_data_tendon_info(m_pTableData[TENDON_LAY],               m_P0.m_vecTendon);
}

void CGWTendon::P02PU()
{
	DWORD t1 = GetTickCount();
	tendon_exchange_ui::data_to_pu_basetype(m_pTableData[TENDON_BASETYPE],   m_P0.m_vecBaseType);

	DWORD t2 = GetTickCount();
	tendon_exchange_ui::data_to_pu_tendontype(m_pTableData[TENDON_TYPE],      m_P0.m_vecTendonType);
	DWORD t3 = GetTickCount();

	tendon_exchange_ui::data_to_pu_anchor_plane(m_pTableData[TENDON_GSMM],       m_P0.m_vecAnchorP);    
	DWORD t4 = GetTickCount();

	tendon_exchange_ui::data_to_pu_tension_batch(m_pTableData[TENDON_GSZLPC],               m_P0.m_veczlpc);
	DWORD t5 = GetTickCount();

	tendon_exchange_ui::data_to_pu_refline(m_pTableData[TENDON_REFLINE],          m_P0.m_vecRefLine);
	DWORD t6 = GetTickCount();


	tendon_exchange_ui::data_to_pu_bendtype(m_pTableData[TENDON_PWLX],         m_P0.m_vecPWType,false);
	DWORD t7 = GetTickCount();

	tendon_exchange_ui::data_to_pu_bendtype(m_pTableData[TENDON_SWLX],         m_P0.m_vecSWType,true, TENDON_SXYYL==m_strID);
	DWORD t8 = GetTickCount();

	tendon_exchange_ui::data_to_pu_tendon_info(m_pTableData[TENDON_LAY],                m_P0.m_vecTendon);    
	DWORD t9 = GetTickCount();

	CLOG::GetLog()->Write(LOG_PLAINTEXT,  "��������������%s%d����-%s%d����-%s%d����-%s%d����-%s%d����-%s%d����-%s%d����-%s%d����\n" , 
		                                                             TENDON_BASETYPE,t2-t1 ,
																	 TENDON_TYPE,t3-t2 ,
																	 TENDON_GSMM,t4-t3 ,
																	 TENDON_GSZLPC,t5-t4 ,
																	 TENDON_REFLINE,t6-t5 ,
																	 TENDON_PWLX,t7-t6 ,
																	 TENDON_SWLX,t8-t7 , 
																	 TENDON_LAY,t9-t8 );

}

void CGWTendon::SetTendonData()
{
	if (!m_pTendonAlgor)
	{
		_ASSERTE(("�����㷨��δ��ʼ��", false));
		return;
	}
	CTendonData TendonData;
	TendonData.m_name = m_P0.m_CompName;
	SetTendonType(m_P0.m_vecTendonType, m_P0.m_vecBaseType, TendonData.m_TypeTable);
	SetTendonLay(m_P0.m_vecTendon, TendonData.m_LayOut);
	SetTendonZLPC(m_P0.m_veczlpc, TendonData.m_ZLTable);
	SetTendonMM(m_P0.m_vecAnchorP, TendonData.m_MMTable);
	SetTendonLayPSW(m_P0.m_vecPWType, TendonData.m_HSecType);
	SetTendonLayPSW(m_P0.m_vecSWType, TendonData.m_VSecType);
	m_pTendonAlgor->Set(TendonData);
}

void CGWTendon::SetTendonType(const vector<tendon_data_type>& vecP0,
							  const vector<tendon_data_basetype>& vecBase,
							  vector<boost::shared_ptr<THSF::CBeamTypeTable>>& vecSFP0)
{
	vecSFP0.clear();
	for (size_t i = 0; i < vecP0.size(); ++i)
	{
		const tendon_data_type& P0 = vecP0[i];
		boost::shared_ptr<CBeamTypeTable> pSFP0;
		pSFP0.reset(new CBeamTypeTable);
		//todo:
		pSFP0->m_TypeName = P0.m_strName;
		//ͨ���������������ҵ�������������	
		tendon_data_basetype Base;
		bool bFind = GetTypeFromVecByName(vecBase, P0.m_base_type, Base);
		if (bFind)
		{
			CSteelBaseData& BaseData =pSFP0->m_BaseData;
			BaseData.m_numbers = Base.m_numbers;
			BaseData.m_start_anchor = Base.m_start_anchor;
			BaseData.m_end_anchor = Base.m_end_anchor;
			BaseData.m_corrugated_pipe = Base.m_corrugated_pipe;
			BaseData.m_materail_type = Base.m_materail_type;
			BaseData.m_tension_stress = Base.m_tension_stress;
			BaseData.m_tension_concrete_strength = Base.m_tension_concrete_strength;
			BaseData.m_CZLXS = Base.m_tension;
			BaseData.m_linker = Base.m_linker;
		}
		pSFP0->m_HType = P0.m_horizon_bend_type;
		pSFP0->m_VType = P0.m_vertical_bend_type;
		pSFP0->m_SteelbeamNature = ("��ʵ����" == P0.m_tendon_property || "����" == P0.m_tendon_property)?0:1;
		pSFP0->m_NameInPaper = P0.m_drawing_name;
		pSFP0->m_FJM = P0.m_drawing_name;
		vecSFP0.push_back(pSFP0);
	}
}

void CGWTendon::SetTendonLay(const vector<tendon_data_info> vecP0, 
							 vector<boost::shared_ptr<THSF::CBeamLayOutTable>>& vecSFP0)
{
	vecSFP0.clear();
	for (size_t i = 0; i < vecP0.size(); ++i)
	{
		const tendon_data_info& P0 = vecP0[i];
		boost::shared_ptr<CBeamLayOutTable> pSFP0;
		pSFP0.reset(new CBeamLayOutTable);
		//todo:
		pSFP0->m_PartsName = P0.m_application_pos;
		pSFP0->m_LayOut.clear();
		for (size_t j = 0; j < P0.m_all_layout.size(); ++j)
		{
			const tendon_data_layout& Lay = P0.m_all_layout[j];
			CSingleLayOut LayOut;
			LayOut.m_name = Lay.m_strName;
			LayOut.m_RefCurveName = Lay.m_refline_name;
			LayOut.m_cross_layout.m_cross_layout = Lay.m_cross_layout;
			LayOut.m_VLayOut.m_VLayOut.push_back(Lay.m_portrait_layout);

	/*		if (Lay.m_vecDist.size() != Lay.m_vecType.size())
			{
				_ASSERTE(("������P0��������!"));
				continue;
			}
			LayOut.m_cross_layout.m_cross_layout = "";
			for (int k = 0; k < Lay.m_vecDist.size(); ++k)
			{
				CString strAcrLay;
				strAcrLay.Format("%g,%s", Lay.m_vecDist, Lay.m_vecType);
				LayOut.m_cross_layout.m_cross_layout += strAcrLay;
				if (Lay.m_vecDist.size() - 1 != k)
					LayOut.m_cross_layout.m_cross_layout += ";";
			}
			SplitString(Lay.m_portrait_layout, ',', LayOut.m_VLayOut.m_VLayOut);*/
			pSFP0->m_LayOut.push_back(LayOut);
		}
		vecSFP0.push_back(pSFP0);
	}
}

void CGWTendon::SetTendonZLPC(const vector<tendon_data_tension_batch> vecP0, 
							  vector<boost::shared_ptr<THSF::CZLPCTable>>& vecSFP0)
{
	vecSFP0.clear();
	for (size_t i = 0; i < vecP0.size(); ++i)
	{
		const tendon_data_tension_batch& P0 = vecP0[i];
		boost::shared_ptr<CZLPCTable> pSFP0;
		pSFP0.reset(new CZLPCTable);
		//todo:
		pSFP0->m_ZLPCName = P0.m_batch_name;
		pSFP0->m_GSName = P0.m_tension_tendon;
		vecSFP0.push_back(pSFP0);
	}
}

void CGWTendon::SetTendonMM(const vector<tendon_data_anchor_plane> vecP0, 
							vector<boost::shared_ptr<THSF::CMMTable>>& vecSFP0)
{
	vecSFP0.clear();
	for (size_t i = 0; i < vecP0.size(); ++i)
	{
		const tendon_data_anchor_plane& P0 = vecP0[i];
		boost::shared_ptr<CMMTable> pSFP0;
		pSFP0.reset(new CMMTable);
		//todo:
		pSFP0->m_name = P0.m_strName;
		pSFP0->m_MMGS = P0.m_anchor_tendon;
		
		//�㷨���޸�
		//BrdRoad::AngleToRadian(P0.m_ha, pSFP0->m_LMSetTop.a);
		//BrdRoad::AngleToRadian(P0.m_ha2, pSFP0->m_LMSetTop.a2);
		//pSFP0->m_LMSetTop.Lmin = P0.m_hLmin;
		//pSFP0->m_LMSetTop.Hmin = P0.m_hHmin;

		//BrdRoad::AngleToRadian(P0.m_ha, pSFP0->m_LMSetBot.a);
		//BrdRoad::AngleToRadian(P0.m_ha2, pSFP0->m_LMSetBot.a2);
		//pSFP0->m_LMSetBot.Lmin = P0.m_hLmin;
		//pSFP0->m_LMSetBot.Hmin = P0.m_hHmin;

		//BrdRoad::AngleToRadian(P0.m_va, pSFP0->m_DMSetLeft.a);
		//BrdRoad::AngleToRadian(P0.m_va2, pSFP0->m_DMSetLeft.a2);
		//pSFP0->m_DMSetLeft.Lmin = P0.m_vLmin;
		//pSFP0->m_DMSetLeft.Hmin = P0.m_vHmin;

		//BrdRoad::AngleToRadian(P0.m_va, pSFP0->m_DMSetRight.a);
		//BrdRoad::AngleToRadian(P0.m_va2, pSFP0->m_DMSetRight.a2);
		//pSFP0->m_DMSetRight.Lmin = P0.m_vLmin;
		//pSFP0->m_DMSetRight.Hmin = P0.m_vHmin;

		pSFP0->m_MMPieceIndex = P0.m_gear_anchor_name;
		
		pSFP0->m_RefFace = P0.m_reference_surface;
		vecSFP0.push_back(pSFP0);
	}
}

void CGWTendon::SetTendonLayPSW(const vector<tendon_data_bend_type> vecP0, 
								vector<boost::shared_ptr<THSF::CBendingType>>& vecSFP0)
{
	vecSFP0.clear();
	for (size_t i = 0; i < vecP0.size(); ++i)
	{
		const tendon_data_bend_type& P0 = vecP0[i];
		boost::shared_ptr<CBendingType> pSFP0;
		pSFP0.reset(new CBendingType);
		//todo:
		pSFP0->m_SYName = P0.m_name;
		pSFP0->m_MirrorAxis = P0.m_strObjectID.Right(P0.m_strObjectID.GetLength()-P0.m_strObjectID.Find('?')-1);/*�Գ�����ʱδ֧��*/
		pSFP0->m_VTZX = P0.m_porlines;
		SplitString(P0.m_joint_list, ',', pSFP0->m_JointLineNames);
		pSFP0->m_Secs.clear();
		for (size_t j = 0; j < P0.m_sections.size(); ++j)
		{
			const tendon_data_section& BSection = P0.m_sections[j];
			CBendingSection Sec;
			Sec.m_x.second = BSection.m_x;
			Sec.m_x_curvename = BSection.m_x_curvename;
			Sec.m_y.second = BSection.m_y;
			Sec.m_y_curvename = BSection.m_y_curvename;
			Sec.m_r.second = BSection.m_r;
			Sec.m_type = BSection.m_type;
			Sec.m_angle.second = BSection.m_angle;
			Sec.m_s.second = BSection.m_s;
			Sec.m_e.second = BSection.m_e;
			Sec.m_line_dir = BSection.m_line_dir;
			Sec.m_semodifier = 0;
			Sec.m_rmodifier = 0;
			Sec.m_IsOrth = FALSE;
			Sec.m_InPutZuData = BSection.m_zu;
			pSFP0->m_Secs.push_back(Sec);
		}
		vecSFP0.push_back(pSFP0);
	}

	//ƽ����� Ĭ��[ֱ��]
	boost::shared_ptr<CBendingType> pSFP02;
	pSFP02.reset(new CBendingType);
	pSFP02->m_SYName = "Ĭ��[ֱ��]";
	//pSFP0->m_MirrorAxis �Գ�����ʱδ֧��
	pSFP02->m_VTZX = "";
	pSFP02->m_Secs.clear();
	for (size_t j = 0; j < 2; ++j)
	{
		CBendingSection Sec;
		Sec.m_x.second = 0;
		Sec.m_x_curvename = 0==j?"TL":"TR";
		Sec.m_y.second = 0;
		Sec.m_y_curvename = "����";
		Sec.m_r.second = 0;
		Sec.m_type = "LL";
		Sec.m_angle.second = 0;
		Sec.m_s.second = 0;
		Sec.m_e.second = 0;
		Sec.m_line_dir = "F";
		Sec.m_semodifier = 0;
		Sec.m_rmodifier = 0;
		Sec.m_IsOrth = FALSE;
		Sec.m_InPutZuData = "";
		pSFP02->m_Secs.push_back(Sec);
	}
	vecSFP0.push_back(pSFP02);
}

void CGWTendon::SetRowNumber(CPUObj* pPU,  int nRow, const CString& strID)
{
	if (!pPU)
	{
		ASSERT(FALSE);
		return;
	}
	vector<CString> vecTableName;
	vecTableName.push_back(TENDON_BASETYPE+strID);
	vecTableName.push_back(TENDON_TYPE+strID);
	vecTableName.push_back(TENDON_GSMM+strID);
	vecTableName.push_back(TENDON_GSZLPC+strID);
	vecTableName.push_back(TENDON_REFLINE+strID);
	for (size_t i = 0; i < vecTableName.size(); ++i)
	{
		CTHDataBase* pDataBase = pPU->FindUserData(vecTableName[i]);
		if(pDataBase)
		{
			pDataBase->SetRowCount(nRow + 2);
			pDataBase->EnableRowAutoSerialNumber(TRUE);
		}
	}

	//���ñ������

	//����bug20738�޸� 2012.0816
	//CTHDataBase* pdatabase = pPU->FindUserData(TENDON_GSMM+strID);
	//if (pdatabase)
	//	pdatabase->SetValue(0, 0, "ê��");
	
	CTHDataBase* pdatabase = pPU->FindUserData(TENDON_BASETYPE+strID);
	if (pdatabase)
		pdatabase->SetValue(0, 0, "��������");

	pdatabase = pPU->FindUserData(TENDON_GSZLPC+strID);
	if (pdatabase)
		pdatabase->SetValue(0, 0, "��������");

	pdatabase = pPU->FindUserData(TENDON_REFLINE+strID);
	if (pdatabase)
		pdatabase->SetValue(0, 0, " �Զ���ο���");

	pdatabase = pPU->FindUserData(TENDON_USERLINE+strID);
	if (pdatabase)
		pdatabase->SetValue(0, 0, " ͬ����������");

}

void CGWTendon::SetEnvVariable(CPUObj* pPU, const CString& strID)
{
	if (!pPU)
	{
		ASSERT(FALSE);
		return;
	}

	CTHDWAccessoryManager *pDWAMgr =  GetAccessoryManager();
	std::vector<CString> strList;
	CStringArray szArr;
	
	//�������ͱ��
	CTHDataBase* pData = pPU->FindUserData(TENDON_BASETYPE+strID);
	if (NULL == pData)
	{
		_ASSERT(FALSE);
		return;
	}
	pDWAMgr->GetMateriaNamesByType_WD(szArr , MT_PRESTEEL_COM);
	for (INT_PTR i=0; i<szArr.GetSize(); i++)
	{
		strList.push_back(szArr[i]);
	}
	pData->SetEnvVariable("strMaterial", strList);

	szArr.RemoveAll();
	strList.clear();
	pDWAMgr->GetMateriaNamesByType_WD(szArr , MT_ANCHOR);
	for (INT_PTR i=0; i<szArr.GetSize(); i++)
	{
		strList.push_back(szArr[i]);
	}
	pData->SetEnvVariable("strAF", strList);

	szArr.RemoveAll();
	strList.clear();
	pDWAMgr->GetMateriaNamesByType_WD(szArr , MT_CORRUGATEDPIPE);
	for (INT_PTR i=0; i<szArr.GetSize(); i++)
	{
		strList.push_back(szArr[i]);
	}
	pData->SetEnvVariable("strB", strList);

	szArr.RemoveAll();
	strList.clear();
	pDWAMgr->GetMateriaNamesByType_WD(szArr, MT_CONNECTOR);
	for (INT_PTR i=0; i<szArr.GetSize(); i++)
	{
		strList.push_back(szArr[i]);
	}
	pData->SetEnvVariable("Linker", strList);
}

bool IsHasSGF(CPUObj* pPU, const CString& strID)
{
	if (pPU->GetCompType() == CT_BOXGIRDER && TENDON_SC001_ZL==strID)
	{
		return true;
	}
	return false;
}

void CGWTendon::CreateUserDataBase(CPUObj* pPU, vector<CString>& vecTableName, const CString& strID)
{
	//����ע��ı�������������ر��
	vecTableName.clear();
	CP1Manager* g_Data = GetP1Manager();
	
	vector<int> vecNum;
	vecNum.push_back(3);
	std::map<GWCOMPONENTTYPE,std::vector<ComponentPUDefine>>::iterator it2 = g_Data->m_componentPUTables.find( pPU->GetCompType());
	if( it2 != g_Data->m_componentPUTables.end())
	{
		std::vector<ComponentPUDefine>::iterator iit = it2->second.begin();
		for(; iit != it2->second.end(); iit++)
		{
			//��������/ƽ��������ͱ�/�������ͱ�
			if(iit->strInstance  ==TENDON_SWLX+strID)
			{
				bool bSGF = false;
				CTHDataBase* pdatabase = CreatePSWTypeDataBase(vecNum,true, true, IsHasSGF(pPU, strID));
				pPU->AddUserDataBase(TENDON_SWLX+strID, pdatabase);
				vecTableName.push_back(TENDON_SWLX+strID);
			}
			else if (iit->strInstance ==TENDON_PWLX+strID)
			{
				CTHDataBase* pdatabase = CreatePSWTypeDataBase(vecNum, false);
				pPU->AddUserDataBase(TENDON_PWLX+strID,pdatabase);
				vecTableName.push_back(TENDON_PWLX+strID);
			}
			else if (iit->strInstance ==TENDON_LAY+strID)
			{
				CTHDataBase* pdatabase = CreateTendonLayDataBase(vecNum);
				pPU->AddUserDataBase(TENDON_LAY+strID,pdatabase);
				vecTableName.push_back(TENDON_LAY+strID);
			}
		}
	}
}


//�Ż��ľ�̬�ӿ�
void CGWTendon::PU2P0(const CPUObj* pPU, CGWTendon_P0& P0, const CString& strID)
{
	CGWTendon td;
	td.SetID(strID);
	td.AddDataBase(pPU);
	td.PU2P0();

	CViewSetting* pSet = pPU->FindTendonSet( strID);
	if(pSet)
	{
		td.m_P0.m_pSetting->Copy( pSet);
	}
	else
	{
	//�ȰѸ����༭�����ݱ������������ⱻ�������ݳ��
		if(P0.m_pSetting)
			td.m_P0.m_pSetting->Copy( P0.m_pSetting);	
	}


	P0 = td.m_P0;
}
void CGWTendon::P02PU(CPUObj* pPU, const CGWTendon_P0& P0, const CString& strID)
{
	CGWTendon td;
	td.SetID(strID);
	td.AddDataBase(pPU);
	td.m_P0 = P0;
	td.P02PU();

	//
	if(P0.m_pSetting)
	{
		CViewSetting* pSet = pPU->FindTendonSet( strID);
		if( !pSet)
		{
			pSet = new CGTEditorAssistDataInfo;
			pPU->m_tendonSetMap[strID]=pSet;
		}
		pSet->Copy( P0.m_pSetting);	

		int i = 0;
	}
}



void CGWTendon::InitTendonAlgor(const CPUObj* pPU, THSF::CTendon*& pTendonAlgor, const CString& strID)
{
	CGWTendon td;
	td.SetID(strID);
	td.AddDataBase( pPU);

	td.InitTendonAlgor();

	delete pTendonAlgor;
	ASSERT(td.m_pTendonAlgor);
	if (td.m_pTendonAlgor)
		pTendonAlgor = td.m_pTendonAlgor->Copy();
}

//��ʼ�������㷨��
void CGWTendon::InitTendonAlgor(const CGWTendon_P0& P0, THSF::CTendon& TendonAlgor)
{
	CGWTendon td;
	td.m_P0 = P0;
	td.InitTendonAlgor2(P0);
	if (td.m_pTendonAlgor)
		TendonAlgor  = *td.m_pTendonAlgor;
}
void CGWTendon::InitTendonAlgor(const CGWTendon_P0& P0, THSF::CTendon*& pTendonAlgor)
{
	CGWTendon td;
	td.m_P0 = P0;
	td.InitTendonAlgor2(P0);
	delete pTendonAlgor;
	ASSERT(td.m_pTendonAlgor);
	if (td.m_pTendonAlgor)
		pTendonAlgor = td.m_pTendonAlgor->Copy();
}

vector<CString> CGWTendon::GetName_zlpc(const CPUObj* pPU, const CString& strID)
{
	vector<CString> vecName;
	vecName.push_back("all");
	CTHDataBase* pdatabase = pPU->FindUserData(TENDON_GSZLPC + strID);
	if (!pdatabase)
	{
		ASSERT(FALSE);
		return vecName;
	}

	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		CString strName;
		pdatabase->Get_Prop(i, "zlpc", strName);
		//BUG24877 ɾ���ظ�����
		bool bSame = false;
		for (size_t j = 0; j < vecName.size(); ++j)
		{
			if (strName == vecName[j])
			{
				bSame = true;
				break;
			}
		}

		if (!bSame)
			vecName.push_back(strName);
	}
	return vecName;
}


void CGWTendon::MergeTendons(const vector<CGWTendon_P0>& vec_tendon_info, 
						 const vector<CString>& vecID, OUT CGWTendon_P0& MergeTendon)
{
	if (vec_tendon_info.size() != vecID.size())
	{
		_ASSERTE(("MergeTendons����������������ID��ʾ������һ��", false));
		return;
	}
	MergeTendon.ClearAll();	
	for (size_t i = 0; i < vec_tendon_info.size(); ++i)
	{
		CGWTendon_P0 Tendon = vec_tendon_info[i];
		const CString& strID = vecID[i];

		Modify_Index(Tendon.m_vecBaseType, strID);	//��������
		Modify_Index2(Tendon.m_vecPWType, strID);		//ƽ������
		Modify_Index2(Tendon.m_vecSWType, strID);		//��������
		Modify_Index(Tendon.m_vecRefLine, strID);		//�û���
		
		//��������
		//for (size_t j = 0; j < Tendon.m_veczlpc.size(); ++j)
		//	Tendon.m_veczlpc[j].m_batch_name+=strID;
		
		//ê��
		for (size_t j = 0; j < Tendon.m_vecAnchorP.size(); ++j)
			Tendon.m_vecAnchorP[j].m_strName+=strID;

		//��������
		for (size_t j = 0; j < Tendon.m_vecTendonType.size(); ++j)
		{
			Tendon.m_vecTendonType[j].m_strName+=strID;
			Tendon.m_vecTendonType[j].m_base_type+=strID;
			//����
			int nPos = Tendon.m_vecTendonType[j].m_vertical_bend_type.Find("[");
			if (-1 != nPos)
				Tendon.m_vecTendonType[j].m_vertical_bend_type.Insert(nPos, strID);

			//ƽ��
			if (Tendon.m_vecTendonType[j].m_horizon_bend_type.Compare("Ĭ��[ֱ��]") != 0)
			{
				nPos =Tendon.m_vecTendonType[j].m_horizon_bend_type.Find("[");
				if (-1 != nPos)
					Tendon.m_vecTendonType[j].m_horizon_bend_type.Insert(nPos, strID);
			}
		}
		
		//����ʵ��
		for (size_t j = 0; j < Tendon.m_vecTendon.size(); ++j)
		{
			for (size_t n = 0; n < Tendon.m_vecTendon[j].m_all_layout.size(); ++n)
			{
				tendon_data_layout& tendonlay = Tendon.m_vecTendon[j].m_all_layout[n];
				//tendonlay.m_strName += strID;
				//�޸ĺ�����
				vector<CString> vecZu;
				SplitString(tendonlay.m_cross_layout, ';', vecZu);
				tendonlay.m_cross_layout = "";
				for (int s = 0; s < vecZu.size(); ++s)
				{
					CString& strInst = vecZu[s];
					vector<CString> vecInst;
					SplitString(strInst, ';', vecInst);
					strInst = "";
					if (!vecInst.empty())
					{
						CString& strTypes = vecInst.back();
						vector<CString> vecType;
						SplitString(strTypes, '+', vecType);

						strTypes = "";
						for (int k = 0; k < vecType.size(); ++k)
						{
							vecType[k] += strID;
							//��ԭtypes
							if (0 != k)
								strTypes += "+";
							strTypes += vecType[k];
						}
					}
					//��ԭʵ��
					for (int d = 0; d < vecInst.size(); ++d)
					{
						if (0 != d)
							strInst += ",";
						strInst += vecInst[d];
					}
					//��ԭ��
					if (0 != s)
						tendonlay.m_cross_layout += ";";
					tendonlay.m_cross_layout += strInst;
				}
			}
		}
		
		//���뵽MergeTendon��
		Add2Vector(MergeTendon.m_vecBaseType, Tendon.m_vecBaseType);
		Add2Vector(MergeTendon.m_vecPWType, Tendon.m_vecPWType);
		Add2Vector(MergeTendon.m_vecSWType, Tendon.m_vecSWType);
		Add2Vector(MergeTendon.m_vecRefLine, Tendon.m_vecRefLine);
		Add2Vector(MergeTendon.m_veczlpc, Tendon.m_veczlpc);
		Add2Vector(MergeTendon.m_vecAnchorP, Tendon.m_vecAnchorP);
		Add2Vector(MergeTendon.m_vecTendonType, Tendon.m_vecTendonType);
		Add2Vector(MergeTendon.m_vecTendon, Tendon.m_vecTendon);
	}
}



void ChangePSWRefName(vector<tendon_data_bend_type>& vecPWType, const std::map<CString,CString>& namemap)
{
	for (size_t i = 0; i < vecPWType.size(); ++i)
	{
		tendon_data_bend_type& PSW = vecPWType[i];
		
		map<CString,CString>::const_iterator item = namemap.find(PSW.m_porlines);
		if (item != namemap.end())
			PSW.m_porlines = item->second;
	
		vector<CString> vecID;
		SplitString(PSW.m_strObjectID, ';', vecID);
		for (size_t n = 1; n < vecID.size(); ++n)
		{
			for (map<CString, CString>::const_iterator iter = namemap.begin(); iter != namemap.end(); ++iter)
				vecID[i].Replace(iter->first, iter->second);
		}

		PSW.m_strObjectID = "";
		for (int n = 0; n < vecID.size(); ++i)
		{
			if (0 != i)
				PSW.m_strObjectID += ";";
			PSW.m_strObjectID += vecID[i];
		}

		for (size_t j = 0; j < PSW.m_sections.size(); ++j)
		{
			tendon_data_section& section = PSW.m_sections[j];

			item = namemap.find(section.m_x_curvename);
			if (item != namemap.end())
				section.m_x_curvename = item->second;

			item = namemap.find(section.m_y_curvename);
			if (item != namemap.end())
				section.m_y_curvename = item->second;

			for (map<CString, CString>::const_iterator iter = namemap.begin(); iter != namemap.end(); ++iter)
				section.m_zu.Replace(iter->first, iter->second);
		}
	}
}

void ChangeLayRefName(vector<tendon_data_info>& vecInfo, const std::map<CString,CString>& namemap)
{
	for (size_t i = 0; i < vecInfo.size(); ++i)
	{
		tendon_data_info& info = vecInfo[i];
		for (size_t j = 0; j < info.m_all_layout.size(); ++j)
		{
			tendon_data_layout& Lay =  info.m_all_layout[j];
			map<CString,CString>::const_iterator item = namemap.find(Lay.m_refline_name);
			if (item != namemap.end())
				Lay.m_refline_name = item->second;
		}
	}
}

void CGWTendon::ChangeRefName(const std::map<CString,CString>& namemap,int changetype)
{
	switch (changetype)
	{
	case 1:
			ChangePSWRefName(m_P0.m_vecPWType, namemap);
		break;
	case 2:
			ChangePSWRefName(m_P0.m_vecSWType, namemap);
		break;
	case 3:
			ChangeLayRefName(m_P0.m_vecTendon, namemap);
		break;
	default:
		_ASSERTE(("ChangeRefNameʱ����changetype����ȷ",false));
		break;
	}
}

void CGWTendon::ChangeRefName(const std::map<CString,CString>& namemap,int changetype, CGWTendon_P0& P0)
{
	switch (changetype)
	{
	case 1:
		ChangePSWRefName(P0.m_vecPWType, namemap);
		break;
	case 2:
		ChangePSWRefName(P0.m_vecSWType, namemap);
		break;
	case 3:
		ChangeLayRefName(P0.m_vecTendon, namemap);
		break;
	default:
		_ASSERTE(("ChangeRefNameʱ����changetype����ȷ",false));
		break;
	}
}
