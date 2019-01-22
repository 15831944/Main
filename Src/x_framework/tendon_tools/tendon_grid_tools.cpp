#include "stdafx.h"

using namespace std;
#include "THGridControl/THGridControlMac.h"
#include "THGridControl/THDataBase.h"
#include "THGridControl/THBaseExcel.h"
#include "THGridControl/RowGroupDataTool.h"
#include "THGridControl/StringParserTools.h"
#include "THGridControl/GridDatabase.h"

#include "../interface/console_interface.h"
#include "../tools/table_templates.h"
#include "tendon_data.h"
#include "dialog_tendon_random_curves.h"

#include "tendon_grid_tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



tendon_grid_tools::tendon_grid_tools()
{
}

tendon_grid_tools::~tendon_grid_tools()
{
}


//�ֶ�����ƽ��/������
//bSW=true����������
//vecRowNum���������͵�������С������ һ��Ϊ1,3
//bHXTZ:�Ƿ����κ��������ߺ�Xֵ (Ŀǰ����ԤӦ����ʱ����Ҫ����)
//bSGF �Ƿ�����ʩ���죨Ŀǰʩ����ֻ���ϲ��ṹ05�����������в��У�
CTHDataBase* create_bendtype_database(vector<int> vecRowNum, bool bSW, bool bHXTZ, bool bSGF)
{
	CString strTitle = bSW ? "��������" : "ƽ������";
	CString strColumn = bSW ? "�߶�������" : "ƽ�����λ";

	CTHDataBase* pDatabase = new CTHDataBase();
	pDatabase->SetExtendData("bSGF", bSGF ? "1" : "0");
	//����ģ����
	CString strTemplateName = bSW ? TENDON_SWLX : TENDON_PWLX;
	pDatabase->SetID(strTemplateName);

	pDatabase->SetRowCount(9);
	pDatabase->SetColCount(13);
	pDatabase->SetFixedRowCount(1);
	pDatabase->SetFixedColCount(1);

	pDatabase->EnableRowAutoSerialNumber(TRUE);
	pDatabase->EnableColAutoSerialNumber(TRUE);
	pDatabase->EnableMergeMenu(TRUE);
	pDatabase->EnableColMenu(FALSE);
	pDatabase->EnableRowGroup(TRUE);

	//Tag����
	pDatabase->SetRowTag(3, TAG_PSWINDEX);
	pDatabase->SetRowTag(4, TAG_PSWINDEX2);
	//pDatabase->SetColTag(3, TAG_PSWINDEX);
	pDatabase->SetRowTag(5, TAG_SGF);
	pDatabase->SetRowTag(8, TAG_PSWROW);
	//////////////////////////////////////////////////////////////////////////
	pDatabase->MergeCells(1, 1, 1, 12);
	pDatabase->SetTitleFormat(1, 1, RT_TABLETITLE);
	CTHCell *pTempCell = pDatabase->GetCell(1, 1);
	pTempCell->SetText(strTitle);
	pTempCell->SetReadOnly(TRUE);

	pDatabase->MergeCells(2, 2, 2, 12);
	pTempCell = pDatabase->GetCell(2, 2);
	pTempCell->SetText("��������");
	pTempCell->SetAlignment(DT_CENTER | DT_VCENTER | DT_HOR);
	pTempCell->SetReadOnly(TRUE);
	pTempCell->SetHintText("�����������");
	pDatabase->SetTitleFormat(2, 2, RT_SUBTITLE);

	pTempCell = pDatabase->GetCell(3, 2);
	pTempCell->SetText("��������");
	pTempCell->SetReadOnly(TRUE);

	pDatabase->MergeCells(3, 3, 3, 12);
	pTempCell = pDatabase->GetCell(3, 3);
	CString strType = bSW ? _T("��������") : _T("ƽ������");
	pTempCell->SetHintText(strType + _T("��������/n��ʽΪ:��������[����]/n����������,���Ÿ���/n��:������[A]��������[A,B,C,D]"));
	//pTempCell->SetTag(_T("liuwenbin"));

	pTempCell = pDatabase->GetCell(4, 2);
	pTempCell->SetText("����������");
	pTempCell->SetReadOnly(TRUE);

	pDatabase->MergeCells(4, 3, 4, 12);
	pTempCell = pDatabase->GetCell(4, 3);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetHintText("������������Чʱ��ʾ�������������߷�����͸���,\n��Чʱ��ʾ������������i�ľ�����ͬ");


	pTempCell = pDatabase->GetCell(5, 2);
	pTempCell->SetText("ʩ����");
	pTempCell->SetReadOnly(TRUE);

	pDatabase->MergeCells(5, 3, 5, 12);
	pTempCell = pDatabase->GetCell(5, 3);
	pTempCell->SetCellType(CT_CHECKDROPLIST);
	//pTempCell->SetLabelText("@SGF");
	pTempCell->SetHintText("ʩ��������");

	//��������༭��guidssss
	//pDatabase->MergeCells(5,2,5,12);
	//////////////////////////////////////////////////////////////////////////

	pDatabase->MergeCells(6, 2, 6, 12);
	pTempCell = pDatabase->GetCell(6, 2);
	pTempCell->SetText("��������");
	pTempCell->SetAlignment(DT_CENTER | DT_VCENTER | DT_HOR);
	pTempCell->SetReadOnly(TRUE);
	pDatabase->SetTitleFormat(6, 2, RT_SUBTITLE);

	pTempCell = pDatabase->GetCell(7, 2);
	pTempCell->SetText("���");

	pTempCell = pDatabase->GetCell(7, 3);
	pTempCell->SetText("����������");

	pTempCell = pDatabase->GetCell(7, 4);
	pTempCell->SetText("X����(mm)");
	pTempCell->SetHintText("������������ߵľ���");

	pTempCell = pDatabase->GetCell(7, 5);
	pTempCell->SetText(strColumn);

	pTempCell = pDatabase->GetCell(7, 6);
	pTempCell->SetText("Y����(mm)");
	pTempCell->SetHintText("����" + strColumn + "�ľ���");

	pTempCell = pDatabase->GetCell(7, 7);
	pTempCell->SetText("���ɰ뾶R(m)");
	pTempCell->SetHintText("���ɰ뾶");

	pTempCell = pDatabase->GetCell(7, 8);
	pTempCell->SetText("��ģʽ");

	pTempCell = pDatabase->GetCell(7, 9);
	pTempCell->SetText("����");

	pTempCell = pDatabase->GetCell(7, 10);
	pTempCell->SetText("����ֱ�߳�(mm)");

	pTempCell = pDatabase->GetCell(7, 11);
	pTempCell->SetText("��ƫ��");
	pTempCell->SetHintText("��ƫ��");

	//pTempCell=pDatabase->GetCell(8,2);

	pTempCell = pDatabase->GetCell(8, 3);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetLabelText("@strHXTZX");
	if (bHXTZ)
		pTempCell->SetReadOnly(TRUE);

	pTempCell = pDatabase->GetCell(8, 4);
	pTempCell->SetHintText("������������ߵľ���");
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	if (bHXTZ)
		pTempCell->SetReadOnly(TRUE);

	pTempCell = pDatabase->GetCell(8, 5);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetLabelText("@strGDTZX");

	pTempCell = pDatabase->GetCell(8, 6);
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	pTempCell->SetHintText("����" + strColumn + "�ľ���");

	pTempCell = pDatabase->GetCell(8, 7);
	pTempCell->SetDataType(CELLDATA_DOUBLE);
	pTempCell->SetHintText("���ɰ뾶R:�������¶���ӹ��ɰ뾶,\n��LL/LA/RRģʽ����Ч\
						   						   \nR>=0:RΪ�̶�ֵ\nR<0: |R|Ϊ����ֵ,���ɱ�");
	pTempCell->SetMaskText("����-��ģʽR&&RR#12385");

	pTempCell = pDatabase->GetCell(8, 8);
	pTempCell->SetCellType(CT_DROPDOWN);
	pTempCell->SetLabelText("LL\nR\nRR");
	pTempCell->SetHintText("��LA/LL/R/RR����ģʽ\nLA:���ʽ\nLL:����ʽ\
						   						   \nR:�ο�ƽ��ʽ\nRR:ָ��λ�ô�Բ����\n����LAģʽ,���ڴ˵�Ԫ��ֱ������Ƕ�ֵ");

	pTempCell = pDatabase->GetCell(8, 9);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetLabelText("����\nˮƽ");
	pTempCell->SetHintText("��������ֱ�߶γ���ʱ�Ĳ�෽��\nˮƽ:���뷽��Ϊֱ�߶ζ˵���ˮ\
						   						   \nƽ���ϵ�ͶӰ��֮��ľ���\
												   						   \n����:���뷽��Ϊ���ε����߷���\nע:��������LA/R/RRģʽ����Ч");

	pTempCell = pDatabase->GetCell(8, 10);
	pTempCell->SetDataType(CELLDATA_STRING);
	pTempCell->SetHintText("���ο��Ƶ�����X/Y������ֱ�߶γ���(S2,E2)\n������ֵ:����ֵΪ�̶�ֵ\
						   						   \n���븺ֵ:����ֵ�ľ���ֵΪ��Сֵ\nע:��������LA/R/RRģʽ����Ч");
	pTempCell->SetSketchMapName("����-��ģʽR&&RR#99999");

	pTempCell = pDatabase->GetCell(8, 11);
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
	
	//����m_SEModifier,m_RModifier
	//pTempCell=pDatabase->GetCell(8,12);
	//////////////////////////////////////////////////////////////////////////

	//�иߡ��п�
	pDatabase->SetRowHeight(0, 0);
	if (!bSW)
	{
		pDatabase->SetRowHeight(4, 0);
	}

	if (!bSW || !bSGF)
	{//ʩ����ֻ�������ʱ�����
		pDatabase->SetRowHeight(5, 0);
	}

	pDatabase->SetColWidth(0, 0);
	pDatabase->SetColWidth(1, 30);
	pDatabase->SetColWidth(2, 80);
	pDatabase->SetColWidth(3, 80);
	pDatabase->SetColWidth(4, 80);
	pDatabase->SetColWidth(5, 80);
	pDatabase->SetColWidth(6, 80);
	pDatabase->SetColWidth(7, 80);
	pDatabase->SetColWidth(8, 75);
	pDatabase->SetColWidth(9, 75);
	pDatabase->SetColWidth(10, 90);
	pDatabase->SetColWidth(11, 80);
	pDatabase->SetColWidth(12, 0);
	//��������Ϣ
	CString strBigRow = bSW ? CString("��������") : CString("ƽ������");
	CString strSmallRow = bSW ? CString("���䶨��") : CString("ƽ�䶨��");
	pDatabase->SetAsRowGroup(8, 8, 2, strSmallRow, TRUE, 7);
	pDatabase->SetAsRowGroup(2, 8, 1, strBigRow, TRUE, 1);

	size_t nGroupNum = vecRowNum.size();
	pDatabase->InsertRowPre(2, nGroupNum - 1, strBigRow);

	int count = pDatabase->GetSubDataBaseCount();
	for (int i = 0; i < count; ++i)
	{
		CGridDataBase subDataBase;//������
		pDatabase->GetSubDataBase(i, subDataBase);
		int count1 = subDataBase.GetSubDataBaseCount();
		for (int j = 0; j < count1; j++)
		{
			CGridDataBase subDataBase1;//����
			subDataBase.GetSubDataBase(j, subDataBase1);
			int count2 = subDataBase1.GetSubDataBaseCount();
			for (int m = 0; m < count2; m++)
			{
				if (m == 1)
				{
					CGridDataBase tempDataBase;//������
					subDataBase1.GetSubDataBase(m, tempDataBase);
					int rowCount = tempDataBase.GetRowCount() - 1;
					CString startRowTag = tempDataBase.FindStartRowTag(rowCount);
					CString rowTag = tempDataBase.GetRowTag(rowCount);
					int rowNo = pDatabase->FindRow(rowTag, startRowTag);
					int subCount = 0;
					if (j < static_cast<int>(vecRowNum.size()))
						subCount = vecRowNum[j] - 1;
					pDatabase->InsertRowPost(rowNo, subCount, strSmallRow);
				}
			}
		}
	}

	//���ñ���
	pDatabase->SetTitleFormat(1, 1, RT_TABLETITLE);

	return pDatabase;
}

CTHDataBase*  create_tendon_layout_database(vector<int> vecRowNum)
{
	//CTHDataBase* pDatabase = GWCreateDatabase();
	CTHDataBase* pDatabase = new CTHDataBase();
//����ģ����
	pDatabase->SetID(TENDON_LAY);

	pDatabase->SetRowCount(5);
	pDatabase->SetColCount(8);
	pDatabase->SetFixedRowCount(1);
	pDatabase->SetFixedColCount(1);

	//pDatabase->EnableRowAutoSerialNumber(TRUE);
	//pDatabase->EnableColAutoSerialNumber(TRUE);
	//pDatabase->EnableMergeMenu(TRUE);
	//pDatabase->EnableColMenu(FALSE);

	pDatabase->EnableRowGroup(TRUE);

	//����TAG
	pDatabase->SetRowTag(2, TAG_YYBW);
	pDatabase->SetRowTag(4, TAG_LAYROW);
	//////////////////////////////////////////////////////////////////////////
	pDatabase->MergeCells(1, 1, 1, 7);
	CTHCell *pTempCell = pDatabase->GetCell(1, 1);
	pTempCell->SetText("��������");
	pTempCell->SetReadOnly(TRUE);

	pTempCell = pDatabase->GetCell(2, 2);
	pTempCell->SetText("Ӧ�ò�λ");
	pTempCell->SetReadOnly(TRUE);

	pDatabase->MergeCells(2, 3, 2, 7);
	pTempCell = pDatabase->GetCell(2, 3);
	pTempCell->SetCellType(CT_CHECKDROPLIST);
	//pTempCell->SetCellType(CT_TASKEDIT);
	//Ӧ�ò�λformatedit
	//CTHFormatCellInfo cif;
	//SetXiuShiFu(cif);
	////AddQualiferInfo(pData->GetTemplateName(),cif);
	//pTempCell->SetFormatorName(cif.m_strFormatIDName);
	//pDatabase->AddFormat(cif);

	///////////////////////////////////////////////////////////////////////////
	pTempCell = pDatabase->GetCell(3, 2);
	pTempCell->SetText("���");

	pTempCell = pDatabase->GetCell(3, 3);
	pTempCell->SetText("��������");
	pTempCell->SetHintText("������,��ʽ:��������/��������");

	pTempCell = pDatabase->GetCell(3, 4);
	pTempCell->SetText("�ο�����");

	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetHintText("hint");

	pTempCell = pDatabase->GetCell(3, 5);
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
	pTempCell = pDatabase->GetCell(3, 6);
	pTempCell->SetText("������");
	pTempCell->SetHintText("���뷽ʽ�������һ��");

	//Ϊ�����༭�������guid
	//GetCell(3,7);	

	pTempCell = pDatabase->GetCell(4, 3);
	pTempCell->SetHintText("������,��ʽ:��������/��������");

	pTempCell = pDatabase->GetCell(4, 4);
	pTempCell->SetCellType(CT_DROPLIST);
	pTempCell->SetLabelText("@strCKXM");
	pTempCell->SetHintText("hint");

	pTempCell = pDatabase->GetCell(4, 5);
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

	pTempCell = pDatabase->GetCell(4, 6);
	pTempCell->SetHintText("�ο���Ϊ��һT��0�ſ�������\
\n���뷽ʽ�������һ��\
\nֻ�貼�õ�һT����,������Զ��ԳƵ�����T");

	//�иߡ��п�
	pDatabase->SetRowHeight(0, 0);

	pDatabase->SetColWidth(0, 0);
	pDatabase->SetColWidth(1, 30);
	pDatabase->SetColWidth(2, 80);
	pDatabase->SetColWidth(3, 180);
	pDatabase->SetColWidth(4, 180);
	pDatabase->SetColWidth(5, 180);
	pDatabase->SetColWidth(6, 180);
	pDatabase->SetColWidth(7, 0);

	//��������Ϣ
	CString strSmallRow = "С����";
	CString strBigRow = "������";
	pDatabase->SetAsRowGroup(4, 4, 2, strSmallRow, TRUE, 3);
	pDatabase->SetAsRowGroup(2, 4, 1, strBigRow, TRUE);

	int nGroupNum = static_cast<int>(vecRowNum.size());
	pDatabase->InsertRowPre(2, nGroupNum - 1, strBigRow);

	int count = pDatabase->GetSubDataBaseCount();
	for (int i = 0; i < count; ++i)
	{
		CGridDataBase subDataBase;//������
		pDatabase->GetSubDataBase(i, subDataBase);
		int count1 = subDataBase.GetSubDataBaseCount();
		for (int j = 0; j < count1; j++)
		{
			CGridDataBase subDataBase1;//����
			subDataBase.GetSubDataBase(j, subDataBase1);
			int count2 = subDataBase1.GetSubDataBaseCount();
			for (int m = 0; m < count2; m++)
			{
				if (m == 1)
				{
					CGridDataBase tempDataBase;//������
					subDataBase1.GetSubDataBase(m, tempDataBase);
					int rowCount = tempDataBase.GetRowCount() - 1;
					CString startRowTag = tempDataBase.FindStartRowTag(rowCount);
					CString rowTag = tempDataBase.GetRowTag(rowCount);
					int rowNo = pDatabase->FindRow(rowTag, startRowTag);
					int subCount = 0;
					if (j < static_cast<int>(vecRowNum.size()))
						subCount = vecRowNum[j] - 1;
					pDatabase->InsertRowPost(rowNo, subCount, strSmallRow);
				}
			}
		}
	}

	pDatabase->SetTitleFormat(1, 1, RT_TABLETITLE);

	return pDatabase;
}





void reigste_tendon_grid_template()
{
	vector<int> vecnum;
	vecnum.push_back(3);
	CTHDataBase* db = create_bendtype_database(vecnum, true, true, true);
	table_templates::add_database_sample(_T("tendon-swbendtype"), db);

	db = create_bendtype_database(vecnum, false, false, false);
	table_templates::add_database_sample(_T("tendon-pwbendtype"), db);

	db = create_tendon_layout_database(vecnum);
	table_templates::add_database_sample(_T("tendon-tendoninfo"), db);
}

void tendon_grid_tools::get_data_from_grid(std::map<CString, CTHDataBase*>& all_dbs, tendon_data& data)
{
	// �������������
	data.clearall();

	CTHDataBase* db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	get_data_from_grid_basetype(db, data.m_all_basetype);

	db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	get_data_from_grid_tendontype(db, data.m_all_tendon_type);

	db = all_dbs.at(_T("����-����ê��"));
	if (!db) return;
	get_data_from_grid_anchor_plane(db, data.m_all_anchor_plane);

	db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	get_data_from_grid_tension_batch(db, data.m_all_tension_batch);

	db = all_dbs.at(_T("����-�Զ���ο���"));
	if (!db) return;
	get_data_from_grid_refline(db, data.m_all_refline);

	db = all_dbs.at(_T("tendon-swbendtype"));
	if (!db) return;
	get_data_from_grid_bendtype(db, data.m_all_ver_bend_type);

	db = all_dbs.at(_T("tendon-pwbendtype"));
	if (!db) return;
	get_data_from_grid_bendtype(db, data.m_all_hor_bend_type);

	db = all_dbs.at(_T("tendon-tendoninfo"));
	if (!db) return;
	get_data_from_grid_tendon_info(db, data.m_all_tendon_info);
}

void tendon_grid_tools::get_data_from_grid_basetype(CTHDataBase* pdatabase, vector<tendon_data_basetype>& vec_basetype)
{
	vec_basetype.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�������ͱ�"));
		return;
	}
	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		tendon_data_basetype basetype;
		pdatabase->Get_Prop(i, "m_StrName", basetype.m_name);
		if(!basetype.m_name.IsEmpty())
		{
			pdatabase->Get_Prop(i, "m_MaterName", basetype.m_materail_type);	
			pdatabase->Get_Prop(i, "m_Num", basetype.m_numbers);	
			pdatabase->Get_Prop(i, "m_StartAnchorFlat", basetype.m_start_anchor);	
			pdatabase->Get_Prop(i, "m_EndAnchorFlat", basetype.m_end_anchor);	
			pdatabase->Get_Prop(i, "m_StrB", basetype.m_corrugated_pipe);	
			pdatabase->Get_Prop(i, "m_Stress", basetype.m_tension_stress);	
			pdatabase->Get_Prop(i, "m_CInfo", basetype.m_tension_concrete_strength);
			pdatabase->Get_Prop(i, "m_Tension", basetype.m_tension);
			pdatabase->Get_Prop(i, "m_Linker", basetype.m_linker);
			vec_basetype.push_back(basetype);
		}
	}		
}

void tendon_grid_tools::get_data_from_grid_bendtype(CTHDataBase* pdatabase, vector<tendon_data_bend_type>& vec_bendtype)
{
	vec_bendtype.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-ƽ�����"));
		return;
	}
	std::vector<RowGroupInfo> giList;
	CRowGroupDataTool gdi(pdatabase);
	pdatabase->QueryGroupStructs(giList);

	size_t i = 0;
	std::vector<RowGroupInfo> giListEx;
	for (i = 0; i != giList.size(); ++i)
	{
		RowGroupInfo rgi = giList[i];
		if (rgi.nEndRowNum == rgi.nStartRowNum)
		{
			continue;
		}
		giListEx.push_back(rgi);
	}

	for (i = 0; i != giListEx.size(); ++i)
	{
		tendon_data_bend_type PSWType;
		int nStart = giListEx[i].nStartRowNum;
		int nEnd = giListEx[i].nEndRowNum;
		//
		CTHCell *pCell = pdatabase->GetCell(nStart + 1, 3);
		PSWType.m_name = pCell->GetText();
		PSWType.m_name.Trim();
		//
		pCell = pdatabase->GetCell(nStart + 2, 3);
		PSWType.m_porlines = pCell->GetText();

		pCell = pdatabase->GetCell(nStart + 3, 3);
		PSWType.m_joint_list = pCell->GetText();

		CString strID;
		strID.Format("PSWID%d", i);
		pdatabase->GetExtendData(strID, PSWType.m_strObjectID);

		int j = 0;
		for (j = nStart + 6; j <= nEnd; j++)
		{
			//�Ƿ�Ϊ��Ч��
			bool bAvailability = false;

			tendon_data_section BeingSection;
			//
			pCell = pdatabase->GetCell(j, 3);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;				
			BeingSection.m_x_curvename = pCell->GetText();
			//
			pCell = pdatabase->GetCell(j, 4);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			BeingSection.m_x = atof(pCell->GetText());
			//
			pCell = pdatabase->GetCell(j, 5);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			BeingSection.m_y_curvename = pCell->GetText();
			//
			pCell = pdatabase->GetCell(j, 6);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			BeingSection.m_y = atof(pCell->GetText());
			//
			pCell = pdatabase->GetCell(j, 7);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			BeingSection.m_r = atof(pCell->GetText()) * 1000;
			//
			pCell = pdatabase->GetCell(j, 8);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			CString strType = pCell->GetText();
			if ("LL" == strType || "R" == strType || "RR" == strType)
			{
				BeingSection.m_type = strType;
			}
			else
			{
				BeingSection.m_type = "LA";
				BeingSection.m_angle = atof(strType)*M_PI / 180;
				//BrdRoad::AngleToRadian(atof(strType), BeingSection.m_angle);
			}

			//
			pCell = pdatabase->GetCell(j, 9);
			if ("" != pCell->GetText())
				bAvailability = true;
			BeingSection.m_line_dir = pCell->GetText() == "ˮƽ" ? "X" : "F";
			//
			CString strSE;
			vector<CString> strSElist;
			pCell = pdatabase->GetCell(j, 10);
			if ("" != pCell->GetText())
				bAvailability = true;
			strSE = pCell->GetText();
			VectorSplit(strSE, ',', strSElist);
			strSElist.resize(2, "");
			BeingSection.m_s = atof(strSElist[0]);
			BeingSection.m_e = atof(strSElist[1]);

			pCell = pdatabase->GetCell(j, 11);
			BeingSection.m_zu = pCell->GetText();

			pCell = pdatabase->GetCell(j, 12);
			if (pCell)
			{
				CString strModifier = pCell->GetText();
				vector<CString> vecModifier;
				SplitString(strModifier, ',', vecModifier);
				vecModifier.resize(2, "");
				BeingSection.m_semodifier = atoi(vecModifier[0]);
				BeingSection.m_rmodifier = atoi(vecModifier[1]);
			}
			if (bAvailability)
				PSWType.m_sections.push_back(BeingSection);
		}
		vec_bendtype.push_back(PSWType);
	}
}

void tendon_grid_tools::get_data_from_grid_anchor_plane(CTHDataBase* pdatabase, vector<tendon_data_anchor_plane>& vec_anchor_plane)
{
	vec_anchor_plane.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-ê���"));
		return;
	}
	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		tendon_data_anchor_plane Plane;
		pdatabase->Get_Prop(i, "mmmc", Plane.m_anchor_name);
		if (!Plane.m_anchor_name.IsEmpty())
		{
			pdatabase->Get_Prop(i, "mggs", Plane.m_anchor_tendon);

			pdatabase->Get_Prop(i, "ckmc", Plane.m_gear_anchor_name);

			pdatabase->Get_Prop(i, "ckm", Plane.m_reference_surface);

			vec_anchor_plane.push_back(Plane);
		}
	}
}
void tendon_grid_tools::get_data_from_grid_refline(CTHDataBase* pdatabase, vector<tendon_data_refline>& vec_refline)
{
	vec_refline.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�ο��߱�"));
		return;
	}
	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		tendon_data_refline RefLine;
		pdatabase->Get_Prop(i, "m_Name", RefLine.m_name);
		//if(!RefLine.m_name.IsEmpty()  )
		//{
		CString strType = "";
		pdatabase->Get_Prop(i, "m_Type", strType);
		if ("����������[ƽ��]" == strType)
			RefLine.m_type = TENDON_Acr_PW;
		else if ("����������[����]" == strType)
			RefLine.m_type = TENDON_Acr_SW;
		else if ("����������" == strType)
			RefLine.m_type = TENDON_Por;
		else if ("�߶�������" == strType)
			RefLine.m_type = TENDON_Ver;
		else
			RefLine.m_type = TENDON_UnKnow;
		pdatabase->Get_Prop(i, "m_CKXName", RefLine.m_refline);	
		CString strProperty;
		pdatabase->Get_Prop(i, "m_Line", strProperty);	
		tendon_data_userline::string_to_vecUserline(strProperty, RefLine.m_userline);
		pdatabase->Get_Prop(i, "m_Note", RefLine.m_note);
		pdatabase->Get_Prop(i, "m_guid", RefLine.m_guid);
		vec_refline.push_back(RefLine);
		//}
	}
}


void tendon_grid_tools::get_data_from_grid_tendontype(CTHDataBase* pdatabase, vector<tendon_data_type>& vecType)
{
	vecType.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�������ͱ�"));
		return;
	}
	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		tendon_data_type tp;
		pdatabase->Get_Prop(i, "lxmc", tp.m_name);
		//if(!tp.m_name.IsEmpty() )
		//{
			pdatabase->Get_Prop(i, "jblx", tp.m_base_type);
			pdatabase->Get_Prop(i, "swlx", tp.m_vertical_bend_type);
			pdatabase->Get_Prop(i, "pwlx", tp.m_horizon_bend_type);
			pdatabase->Get_Prop(i, "gsxz", tp.m_tendon_property);
			pdatabase->Get_Prop(i, "tzmc", tp.m_drawing_name);
			pdatabase->Get_Prop(i, "zlx", tp.m_sub_type);
			vecType.push_back(tp);
		//}
	}
}
void tendon_grid_tools::get_data_from_grid_tension_batch(CTHDataBase* pdatabase, vector<tendon_data_tension_batch>& vec_refline)
{
	vec_refline.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�������α�"));
		return;
	}
	for (int i = 0; i < pdatabase->GetNumberRows(); ++i)
	{
		tendon_data_tension_batch pc;
		pdatabase->Get_Prop(i, "zlpc", pc.m_batch_name);
		//if(!pc.m_batch_name.IsEmpty())
		//{
			pdatabase->Get_Prop(i, "mggs", pc.m_tension_tendon);
			vec_refline.push_back(pc);
		//}
	}
}
void tendon_grid_tools::get_data_from_grid_tendon_info(CTHDataBase* pdatabase, vector<tendon_data_info>& vec_tendon_info)
{
	vec_tendon_info.clear();
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-���ñ�"));
		return;
	}
	std::vector<RowGroupInfo> giList;
	CRowGroupDataTool gdi(pdatabase);
	pdatabase->QueryGroupStructs(giList);

	size_t i = 0;
	std::vector<RowGroupInfo> giListEx;
	for (i = 0; i != giList.size(); ++i)
	{
		RowGroupInfo rgi = giList[i];
		if (rgi.nEndRowNum == rgi.nStartRowNum)
		{
			continue;
		}
		giListEx.push_back(rgi);
	}

	for (i = 0; i != giListEx.size(); ++i)
	{
		tendon_data_info info;
		int nStart = giListEx[i].nStartRowNum;
		int nEnd  = giListEx[i].nEndRowNum;
		//
		CTHCell *pCell = pdatabase->GetCell(nStart,3);
		info.m_application_pos = pCell->GetText();

		int j = 0;
		for (j = nStart + 2; j <= nEnd; j++)
		{
			//�Ƿ�Ϊ��Ч��
			bool bAvailability = false;

			tendon_data_layout lay;
			//
			pCell = pdatabase->GetCell(j,3);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			lay.m_tendon_name = pCell->GetText();
			//
			pCell = pdatabase->GetCell(j,4);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			lay.m_refline_name = pCell->GetText();
			//
			pCell = pdatabase->GetCell(j,5);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			lay.m_cross_layout = pCell->GetText();

			pCell = pdatabase->GetCell(j,6);
			if (!pCell->GetText().IsEmpty()) bAvailability = true;
			lay.m_portrait_layout = pCell->GetText();
			//
			pCell = pdatabase->GetCell(j,7);
			if(pCell)
				lay.m_guid = pCell->GetText();

			if (bAvailability)
				info.m_all_layout.push_back(lay);
		}
			vec_tendon_info.push_back(info);
	}
}

void tendon_grid_tools::set_data_to_grid(std::map<CString, CTHDataBase*>& all_dbs, const tendon_data& data)
{
	CTHDataBase* db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	set_data_to_grid_basetype(db, data.m_all_basetype);

	db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	set_data_to_grid_tendontype(db, data.m_all_tendon_type);

	db = all_dbs.at(_T("����-����ê��"));
	if (!db) return;
	set_data_to_grid_anchor_plane(db, data.m_all_anchor_plane);

	db = all_dbs.at(_T("����-��������"));
	if (!db) return;
	set_data_to_grid_tension_batch(db, data.m_all_tension_batch);

	db = all_dbs.at(_T("����-�Զ���ο���"));
	if (!db) return;
	set_data_to_grid_refline(db, data.m_all_refline);

	db = all_dbs.at(_T("tendon-swbendtype"));
	if (!db) return;
	set_data_to_grid_bendtype(db, data.m_all_ver_bend_type, true, false);

	db = all_dbs.at(_T("tendon-pwbendtype"));
	if (!db) return;
	set_data_to_grid_bendtype(db, data.m_all_hor_bend_type, false, false);

	db = all_dbs.at(_T("tendon-tendoninfo"));
	if (!db) return;
	set_data_to_grid_tendon_info(db, data.m_all_tendon_info);

	// ��ȡ�����ݺ���¸����������
	init_temporary_variable(data);
}

void tendon_grid_tools::set_data_to_grid_basetype(CTHDataBase* pdatabase, const vector<tendon_data_basetype>& vec_basetype)
{
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�������ͱ�"));
		return;
	}
	pdatabase->SetRowCount(2);
	int nRow = vec_basetype.size() <= 3?5:vec_basetype.size() + 2;
	pdatabase->SetRowCount(nRow);
	for (size_t i = 0; i < vec_basetype.size(); ++i)
	{
		const tendon_data_basetype& basetype = vec_basetype[i];
		pdatabase->Set_Prop(i, "m_StrName", basetype.m_name);
		pdatabase->Set_Prop(i, "m_MaterName", basetype.m_materail_type);	
		pdatabase->Set_Prop(i, "m_Num", basetype.m_numbers);	
		pdatabase->Set_Prop(i, "m_StartAnchorFlat", basetype.m_start_anchor);	
		pdatabase->Set_Prop(i, "m_EndAnchorFlat", basetype.m_end_anchor);	
		pdatabase->Set_Prop(i, "m_StrB", basetype.m_corrugated_pipe);	
		pdatabase->Set_Prop(i, "m_Stress", basetype.m_tension_stress);	
		pdatabase->Set_Prop(i, "m_CInfo", basetype.m_tension_concrete_strength);
		pdatabase->Set_Prop(i, "m_Tension", basetype.m_tension);
		pdatabase->Set_Prop(i, "m_Linker", basetype.m_linker);
	}		
}

void tendon_grid_tools::set_data_to_grid_bendtype(CTHDataBase* pDatabase, const vector<tendon_data_bend_type>& vec_bendtype, bool bSW, bool bHXTZ)
{
	if (!pDatabase)
	{
		ASSERT(FALSE);
		return;
	}
	//�������кŴ����±�
	vector<int> nRowNum;
	for (size_t i  = 0; i < vec_bendtype.size(); ++i)
	{
		const tendon_data_bend_type& PSWType= vec_bendtype[i];
		nRowNum.push_back(PSWType.m_sections.size());
	}

	if (vec_bendtype.empty())
	{//databaseʼ�ն���һ�У���ֹvec_bendtypeΪ�յ�ʱ��database�����������ݲ���Ӧ
		nRowNum.push_back(3);
	}

	CString strHasSGF;
	pDatabase->GetExtendData("bSGF", strHasSGF);
	CTHDataBase* pdatabase = create_bendtype_database(nRowNum, bSW, bHXTZ, strHasSGF == "1");
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("��������ƽ�����ʧ��!"));
		return;
	}

	if (!vec_bendtype.empty())
	{
		//p02pu
		std::vector<RowGroupInfo> giList;
		CRowGroupDataTool gdi(pdatabase);
		pdatabase->QueryGroupStructs(giList);

		std::vector<RowGroupInfo> giListEx;
		for (size_t i = 0; i != giList.size(); ++i)
		{
			RowGroupInfo rgi = giList[i];
			if (rgi.nEndRowNum == rgi.nStartRowNum)
			{
				continue;
			}
			giListEx.push_back(rgi);
		}

		for (size_t i = 0; i != giListEx.size(); ++i)
		{
			const tendon_data_bend_type& PSWType = vec_bendtype[i];
			int nStart = giListEx[i].nStartRowNum;
			int nEnd  = giListEx[i].nEndRowNum;
			//
			CTHCell *pCell = pdatabase->GetCell(nStart+1,3);
			pCell->SetText(PSWType.m_name);
			//
			pCell = pdatabase->GetCell(nStart+2,3);
			pCell->SetText(PSWType.m_porlines);
			//
			pCell = pdatabase->GetCell(nStart+3,3);
			pCell->SetText(PSWType.m_joint_list);

			CString strID;
			strID.Format("PSWID%d", i);
			pdatabase->SetExtendData(strID, PSWType.m_strObjectID);

			if (PSWType.m_sections.size() > 0)
			{
				for (int j = nStart + 6; j <= nEnd; j++)
				{
					const tendon_data_section& BeingSection = PSWType.m_sections[j - nStart - 6];
					//
					pCell = pdatabase->GetCell(j, 3);
					pCell->SetText(BeingSection.m_x_curvename);
					//
					pCell = pdatabase->GetCell(j,4);
					CString x;
					x.Format("%f", BeingSection.m_x);
					TrimZeroForDoubleStr(x);
					pCell->SetText(x);
					//
					pCell = pdatabase->GetCell(j,5);
					pCell->SetText(BeingSection.m_y_curvename);
					//
					pCell = pdatabase->GetCell(j,6);
					CString y;
					y.Format("%f", BeingSection.m_y);
					TrimZeroForDoubleStr(y);
					pCell->SetText(y);
					//
					pCell = pdatabase->GetCell(j,7);
					CString r;
					r.Format("%f", BeingSection.m_r / 1000);
					TrimZeroForDoubleStr(r);
					pCell->SetText(r);
					//
					pCell = pdatabase->GetCell(j,8);
					if ("LL" == BeingSection.m_type || "R" == BeingSection.m_type || "RR" == BeingSection.m_type)
					{
						pCell->SetText(BeingSection.m_type);
					}
					else
					{
						CString strLA;
						double dAngle = 0;
						//BrdRoad::RadianToAngle(BeingSection.m_angle, dAngle);
						dAngle = BeingSection.m_angle * 180 / M_PI;
						strLA.Format("%f", dAngle);
						TrimZeroForDoubleStr(strLA);
						pCell->SetText(strLA);
					}

					//
					pCell = pdatabase->GetCell(j, 9);
					pCell->SetText(BeingSection.m_line_dir == "X" ? "ˮƽ" : "����");
					//
					CString strS, strE, strSE;
					strS.Format("%f", BeingSection.m_s);
					strE.Format("%f", BeingSection.m_e);
					TrimZeroForDoubleStr(strS);
					TrimZeroForDoubleStr(strE);
					strSE = strS + "," + strE;
					pCell = pdatabase->GetCell(j, 10);
					pCell->SetText(strSE);
					//
					pCell = pdatabase->GetCell(j, 11);
					pCell->SetText(BeingSection.m_zu);

					CString strModifier;
					strModifier.Format("%d,%d", BeingSection.m_semodifier, BeingSection.m_rmodifier);
					pCell = pdatabase->GetCell(j, 12);
					if (pCell)
						pCell->SetText(strModifier);
				}
			}
		}
	}

	//copy
	pDatabase->Copy(*pdatabase);
	delete pdatabase;
}

void tendon_grid_tools::set_data_to_grid_anchor_plane(CTHDataBase* pdatabase, const vector<tendon_data_anchor_plane>& vec_anchor_plane)
{
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-ê���"));
		return;
	}
	pdatabase->SetRowCount(2);
	int nRow = vec_anchor_plane.size() <= 3 ? 5 : vec_anchor_plane.size() + 2;
	pdatabase->SetRowCount(nRow);
	for (size_t i = 0; i != vec_anchor_plane.size(); ++i)
	{
		const tendon_data_anchor_plane& MiaoMian = vec_anchor_plane[i];
		pdatabase->Set_Prop(i, "mmmc", MiaoMian.m_anchor_name);
		pdatabase->Set_Prop(i, "mggs", MiaoMian.m_anchor_tendon);
		pdatabase->Set_Prop(i, "ckmc", MiaoMian.m_gear_anchor_name);
		pdatabase->Set_Prop(i, "ckm", MiaoMian.m_reference_surface);
	}
}

void tendon_grid_tools::set_data_to_grid_refline(CTHDataBase* pdatabase, const vector<tendon_data_refline>& vec_refline)
{
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�ο��߱�"));
		return;
	}
	pdatabase->SetRowCount(2);
	int nRow = vec_refline.size() <= 3?5:vec_refline.size() + 2;
	pdatabase->SetRowCount(nRow);
	for (size_t i = 0; i != vec_refline.size(); ++i)
	{
		const tendon_data_refline& RefLine = vec_refline[i];
		pdatabase->Set_Prop(i, "m_Name", RefLine.m_name);
		CString strType = "";
		if (TENDON_Acr_PW == RefLine.m_type)
			strType = "����������[ƽ��]";
		else if (TENDON_Acr_SW == RefLine.m_type)
			strType = "����������[����]";
		else if (TENDON_Por == RefLine.m_type)
			strType = "����������";
		else if (TENDON_Ver == RefLine.m_type)
			strType = "�߶�������";
		pdatabase->Set_Prop(i, "m_Type", strType);
		pdatabase->Set_Prop(i, "m_CKXName", RefLine.m_refline);
		CString strProperty;
		tendon_data_userline::vecUserline_to_string(strProperty,RefLine.m_userline);
		pdatabase->Set_Prop(i, "m_Line", strProperty);		
		pdatabase->Set_Prop(i, "m_Note", RefLine.m_note);
		pdatabase->Set_Prop(i, "m_guid", RefLine.m_guid);
	}		
}

void tendon_grid_tools::set_data_to_grid_tendontype(CTHDataBase* pdatabase, const vector<tendon_data_type>& vecType)
{
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ������ͱ�"));
		return;
	}
	pdatabase->SetRowCount(2);
	int nRow = vecType.size() <= 3?5:vecType.size() +2;
	pdatabase->SetRowCount(nRow);
	for (size_t i = 0; i != vecType.size(); ++i)
	{
		const tendon_data_type& tp = vecType[i];
		pdatabase->Set_Prop(i, "lxmc", tp.m_name);
		pdatabase->Set_Prop(i, "jblx", tp.m_base_type);
		pdatabase->Set_Prop(i, "swlx", tp.m_vertical_bend_type);
		pdatabase->Set_Prop(i, "pwlx", tp.m_horizon_bend_type);
		pdatabase->Set_Prop(i, "gsxz", tp.m_tendon_property);
		pdatabase->Set_Prop(i, "tzmc", tp.m_drawing_name);
		pdatabase->Set_Prop(i, "zlx", tp.m_sub_type);
	}
}
void tendon_grid_tools::set_data_to_grid_tension_batch(CTHDataBase* pdatabase, const vector<tendon_data_tension_batch>& vec_tension_batch)
{
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("ȱ�ٸ���-�������α�"));
		return;
	}
	pdatabase->SetRowCount(2);
	int nRow = vec_tension_batch.size() <= 3 ? 5 : vec_tension_batch.size() + 2;
	pdatabase->SetRowCount(nRow);
	for (size_t i = 0; i < vec_tension_batch.size(); ++i)
	{
		const tendon_data_tension_batch& pc = vec_tension_batch[i];
		pdatabase->Set_Prop(i, "zlpc", pc.m_batch_name);
		pdatabase->Set_Prop(i, "mggs", pc.m_tension_tendon);
	}
}
void tendon_grid_tools::set_data_to_grid_tendon_info(CTHDataBase* pDatabase, const vector<tendon_data_info>& vec_tendon_info)
{
	if (!pDatabase)
	{
		ASSERT(FALSE);
		return;
	}
	//�������кŴ����±�
	vector<int> nRowNum;
	for (size_t i  = 0; i != vec_tendon_info.size(); ++i)
	{
		const tendon_data_info& info= vec_tendon_info[i];
		nRowNum.push_back(info.m_all_layout.size());
	}

	if (vec_tendon_info.empty())
	{//databaseʼ�ն���һ�У���ֹvec_bendtypeΪ�յ�ʱ��database�����������ݲ���Ӧ
		nRowNum.push_back(3);
	}

	CTHDataBase* pdatabase = create_tendon_layout_database(nRowNum);
	if (!pdatabase)
	{
		app_print_info(MT_WARNING, _T("�����������ñ��ʧ�ܣ�"));
		return;
	}

	if (!vec_tendon_info.empty())
	{
		//p02pu
		std::vector<RowGroupInfo> giList;
		CRowGroupDataTool gdi(pdatabase);
		pdatabase->QueryGroupStructs(giList);

		size_t i = 0;
		std::vector<RowGroupInfo> giListEx;
		for (i = 0; i != giList.size(); ++i)
		{
			RowGroupInfo rgi = giList[i];
			if (rgi.nEndRowNum == rgi.nStartRowNum)
			{
				continue;
			}
			giListEx.push_back(rgi);
		}

		for (i = 0; i != giListEx.size(); ++i)
		{
			const tendon_data_info& info = vec_tendon_info[i];
			int nStart = giListEx[i].nStartRowNum;
			int nEnd = giListEx[i].nEndRowNum;
			//
			CTHCell *pCell = pdatabase->GetCell(nStart, 3);
			pCell->SetText(info.m_application_pos);
			//

			int j = 0;
			for (j = nStart + 2; j <= nEnd; j++)
			{
				int nIndex = j - nStart - 2;
				if (j - nStart - 2 >= (int)info.m_all_layout.size())
					break;

				const tendon_data_layout& lay = info.m_all_layout[nIndex];
				//
				pCell = pdatabase->GetCell(j, 3);
				pCell->SetText(lay.m_tendon_name);
				//
				pCell = pdatabase->GetCell(j, 4);
				pCell->SetText(lay.m_refline_name);
				//
				pCell = pdatabase->GetCell(j, 5);
				pCell->SetText(lay.m_cross_layout);
				//
				pCell = pdatabase->GetCell(j, 6);
				pCell->SetText(lay.m_portrait_layout);

				pCell = pdatabase->GetCell(j, 7);
				if (pCell)	pCell->SetText(lay.m_guid);
			}
		}
	}
	//copy
	pDatabase->Copy(*pdatabase);
	delete pdatabase;
}

void tendon_grid_tools::get_all_tendon_grid_template_names(std::vector<CString>& names)
{
	names = get_all_tendon_grid_template_names();
}

std::vector<CString> tendon_grid_tools::get_all_tendon_grid_template_names()
{
	vector<CString> names = 
	{
		_T("����-��������"),
		_T("����-��������"),
		_T("����-����ê��"),
		_T("����-��������"),
		_T("����-�Զ���ο���"),
		_T("tendon-swbendtype"),
		_T("tendon-pwbendtype"),
		_T("tendon-tendoninfo"), 
	};
	return names;
}

void tendon_grid_tools::set_tendon_grid_variable(std::map<CString, CTHDataBase*>& all_dbs, const tendon_grid_environment_varible& src)
{
	CTHDataBase* db = all_dbs.at(_T("����-��������"));
	if (!db) return;

	db->SetEnvVariable("strMaterial", src.m_all_material);
	db->SetEnvVariable("strAnchor", src.m_all_anchor);
	db->SetEnvVariable("strPipe", src.m_all_pipe);
	db->SetEnvVariable("strLinker", src.m_all_linker);

	db = all_dbs.at(_T("����-����ê��"));
	if (!db) return;
	db->SetEnvVariable("ckmc", src.m_all_chikuai);

	db = all_dbs.at(_T("tendon-swbendtype"));
	if (!db) return;
	db->SetEnvVariable(_T("strHXTZX"), src.m_all_swhxtzx);
	db->SetEnvVariable(_T("strGDTZX"), src.m_all_swgdtzx);

	db = all_dbs.at(_T("tendon-pwbendtype"));
	if (!db) return;
	db->SetEnvVariable(_T("strHXTZX"), src.m_all_pwhxtzx);
	db->SetEnvVariable(_T("strGDTZX"), src.m_all_pwkzw);

	db = all_dbs.at(_T("tendon-tendoninfo"));
	if (!db) return;
	db->SetEnvVariable(_T("strCKXM"), src.m_all_bzckx);
}

void tendon_grid_tools::make_default_value_by_grid(tendon_data& data)
{
	data.clearall();

}

void tendon_grid_tools::should_pop_tendon_sub_grid(CTHDataBase* db, NM_THGRID* notify_struct)
{
	if (!db) return;
	if (!notify_struct) return;

	int clicked_row = notify_struct->rowNo;
	int clicked_col = notify_struct->colNo;

	CString col_tag = db->GetColTag(clicked_col);
	if (_T("m_Line") == col_tag)
	{
		dialog_tendon_random_curves dlg(_T("����-ͬ����������"), 290, 430, _T("����-ͬ����������"));
		CTHCell* cell = db->GetCell(clicked_row, clicked_col);
		dlg.m_text = (cell ? cell->GetText() : _T(""));
		if (IDOK == dlg.DoModal())
		{
			db->GetExcel()->GetActiveCellType()->GetInpalceWnd()->SetWindowText(dlg.m_text);
			db->GetExcel()->DeActiveCell();
		}
	}
}



void tendon_grid_tools::init_temporary_variable(const tendon_data& data)
{
	for (size_t i = 0; i != data.m_all_basetype.size(); ++i)
		m_basetype_names.push_back(data.m_all_basetype[i].m_name);
	m_all_ver_bend = data.m_all_ver_bend_type;
	m_all_hor_bend = data.m_all_hor_bend_type;
}

vector<CString> tendon_grid_tools::m_basetype_names = {};
vector<tendon_data_bend_type> tendon_grid_tools::m_all_ver_bend = {};
vector<tendon_data_bend_type> tendon_grid_tools::m_all_hor_bend = {};

void tendon_grid_tools::reflesh_temporary_variable(CTHDataBase* db, NM_THGRID* notify_struct)
{
	if (!db) return;
	if (!notify_struct) return;

	int clicked_row = notify_struct->rowNo;
	int clicked_col = notify_struct->colNo;

	CString col_tag = db->GetColTag(clicked_col);
	CString row_tag = db->GetRowTag(clicked_row);
	if (_T("m_StrName") == col_tag)
	{
		m_basetype_names.clear();
		for (int i = 0; i < db->GetRowCount() - db->GetFixedRowCount(); ++i)
		{
			CString tmp_name;
			db->Get_Prop(i, _T("m_StrName"), tmp_name);
			m_basetype_names.push_back(tmp_name);
		}
	}
	else if (_T("ƽ������������") == row_tag && _T("tendon-swbendtype") == db->GetID())
	{
		m_all_ver_bend.clear();
		get_data_from_grid_bendtype(db, m_all_ver_bend);
	}
	else if (_T("ƽ������������") == row_tag && _T("tendon-pwbendtype") == db->GetID())
	{
		m_all_hor_bend.clear();
		get_data_from_grid_bendtype(db, m_all_hor_bend);
	}
	else
	{
		// nothing
	}
}

void tendon_grid_tools::set_enviroment_variable(CTHDataBase* db, NM_THGRID* notify_struct)
{
	if (!db) return;
	if (!notify_struct) return;

	int clicked_row = notify_struct->rowNo;
	int clicked_col = notify_struct->colNo;

	CString col_tag = db->GetColTag(clicked_col);
	if (_T("jblx") == col_tag)
	{
		db->SetEnvVariable(_T("jblx"), m_basetype_names);
	}
	else if (_T("swlx") == col_tag)
	{
		vector<CString> ver_bend_names;

		for (size_t i = 0; i != m_all_ver_bend.size(); ++i)
			ver_bend_names.push_back(m_all_ver_bend[i].m_name);

		db->SetEnvVariable(_T("swlx"), ver_bend_names);
	}
	else if (_T("pwlx") == col_tag)
	{
		vector<CString> hor_bend_names;

		for (size_t i = 0; i != m_all_hor_bend.size(); ++i)
			hor_bend_names.push_back(m_all_hor_bend[i].m_name);

		db->SetEnvVariable(_T("pwlx"), hor_bend_names);
	}
}

void tendon_grid_tools::set_bendtype_droplist(std::vector<CString>& vechxtzx, std::vector<CString>& vecgdtzx, std::map<CString, CTHDataBase*>& all_dbs)
{
	vector<CString>names{ _T("tendon-swbendtype"), _T("tendon-pwbendtype") };
	for (CString name : names)
	{
		CTHDataBase* db = all_dbs.at(name);
		db->SetEnvVariable(_T("strHXTZX"), vechxtzx);
		db->SetEnvVariable(_T("strGDTZX"), vecgdtzx);
	}

}

tendon_grid_environment_varible::tendon_grid_environment_varible()
{

}

tendon_grid_environment_varible::~tendon_grid_environment_varible()
{

}
