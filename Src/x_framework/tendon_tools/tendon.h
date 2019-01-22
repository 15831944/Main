#pragma once

#include "tendon.h"

class CTHDataBase;

//////////�������ʦ����ͨ�ý��棬���ݡ��������ʦV1-���1����2011.06.15�нϴ�Ķ��������ڶ��汾////////////
#define TENDON_SC001_ZL			 "����"
#define TENDON_SC001_HL          "����"
#define TENDON_HXYYL             "����ԤӦ��"
#define TENDON_SXYYL             "����ԤӦ��"


X_FRAMEWORK_API void RegisterGWTendonComType();

namespace  THSF
{
	class CTendon;
	class CBeamTypeTable;
	class CBeamLayOutTable;
	class CZLPCTable;
	class CMMTable;
	class CBendingType;
}

//
class X_FRAMEWORK_API CGWTendon
{
public:
	CGWTendon();
	~CGWTendon();
public:
	//�Ż��ľ�̬�ӿ�
	static void PU2P0(const CPUObj* pPU, CGWTendon_P0& P0, const CString& strID = "");
	static void P02PU(CPUObj* pPU, const CGWTendon_P0& P0, const CString& strID = "");
	//��ʼ�������㷨��
	static void InitTendonAlgor(const CGWTendon_P0& P0, THSF::CTendon*& m_pTendonAlgor);
	static void InitTendonAlgor(const CGWTendon_P0& P0, THSF::CTendon& TendonAlgor);
	static void InitTendonAlgor(const CPUObj* pPU, THSF::CTendon*& m_pTendonAlgor, const CString& strID = "");

	//����P0�ϲ�
	static void MergeTendons(const vector<CGWTendon_P0>& vec_tendon_info, 
		const vector<CString>& vecID, OUT CGWTendon_P0& Tendon);

	//���������������,���˸�������������firstλ�ô����All
	static vector<CString> GetName_zlpc(const CPUObj* pPU, const CString& strID = "");
	
	//changetype�ı�����ͣ�1Ϊ�ı�ƽ�����ͱ�2Ϊ�ı��������ͱ�3Ϊ�ı�������ñ�map.firstΪ�滻ǰ
	void ChangeRefName(const std::map<CString,CString>& namemap,int changetype);

	//changetype�ı�����ͣ�1Ϊ�ı�ƽ�����ͱ�2Ϊ�ı��������ͱ�3Ϊ�ı�������ñ�map.firstΪ�滻ǰ
	static void ChangeRefName(const std::map<CString,CString>& namemap,int changetype, CGWTendon_P0& P0);
public:
	//���ø�������ID��
	void SetID(const CString& strID){m_strID = strID;}
	//����һ��ͨ������PU���ݳ�ʼ�������㷨��
	//������database���������㷨����
	void AddDataBase(const CPUObj* pPU);
	//��ʼ�������㷨��
	void InitTendonAlgor(); 
	//��������ͨ������P0���ݳ�ʼ�������㷨��
	void InitTendonAlgor2(const CGWTendon_P0& P0);  

	//�õ���Ҫע��ı������,�����strTableName�������
	static void GetRegisterTableName(CStringArray& strArr, const CString& strID = "");
	static void GetRegisterTableName2(CStringArray& strArr, const CString& strID = "");//�õ�ʵ����

	//PU����Ҫ���õĺ���

	//������д��� InitMyGrid()�����е���
	static void CreateUserDataBase(CPUObj* pPU, OUT vector<CString>& vecTableName, const CString& strID = "");

	//���ñ�񻷾����� InitGridData()�����е���
	static void SetEnvVariable(CPUObj* pPU, const CString& strID = "");

public:
	//��ӹ����ο�������
	void AddComponentLineNames(CString Name);

	//��չ����ο���
	void ClearComponentLineNames(){m_ComponentLines.clear();}

	//pu<->p0
	void PU2P0();
	void P02PU();

	//�ֶ�����ƽ��/������
	//bSW=true����������
	//vecRowNum���������͵�������С������ һ��Ϊ1,3
	//bHXTZ:�Ƿ����κ��������ߺ�Xֵ (Ŀǰ����ԤӦ����ʱ����Ҫ����)
	//bSGF �Ƿ�����ʩ���죨Ŀǰʩ����ֻ���ϲ��ṹ05�����������в��У�
	static CTHDataBase* CreatePSWTypeDataBase(vector<int> vecRowNum, bool bSW= true, bool bHXTZ = false, bool bSGF = false);
	
	/*�ֶ������������ñ�*/
	//vecRowNum���������͵�������С������ һ��Ϊ1,3
	static CTHDataBase* CreateTendonLayDataBase(vector<int> vecRowNum);

	//���ñ���ʼ��ʱ����ʾ����
	static void SetRowNumber(CPUObj* pPU, int nRow = 3, const CString& strID = "");

private:
	//���òο���
	void SetRefCurve();

	////���ø����㷨���ṹ
	void SetTendonData();
	void SetTendonType(const vector<tendon_data_type>& vecP0, const vector<tendon_data_basetype>& vecBase, 
		vector<boost::shared_ptr<THSF::CBeamTypeTable>>& vecSFP0);
	void SetTendonLay(const vector<tendon_data_info> vecP0, vector<boost::shared_ptr<THSF::CBeamLayOutTable>>& vecSFP0);
	void SetTendonZLPC(const vector<tendon_data_tension_batch> vecP0, vector<boost::shared_ptr<THSF::CZLPCTable>>& vecSFP0);
	void SetTendonMM(const vector<tendon_data_anchor_plane> vecP0, vector<boost::shared_ptr<THSF::CMMTable>>& vecSFP0);
	void SetTendonLayPSW(const vector<tendon_data_bend_type> vecP0, vector<boost::shared_ptr<THSF::CBendingType>>& vecSFP0);

public:
	THSF::CTendon* m_pTendonAlgor;//�����㷨��
	CGWTendon_P0 m_P0;//P0����
private:
	std::map<CString,CTHDataBase*> m_pTableData; //������������
	vector<CString> m_ComponentLines;	//�����ο���,��ʱû�����ã��ڶ��ο��߲���Ҫ����

	//֧��һ������ע������������
	CString m_strID;
};


template <class Ty>
bool GetTypeFromVecByName(const vector<Ty>& vecT, const CString& strName,OUT Ty& T)
{
	if(strName == "")
		return false;

	for (int i = 0; i < static_cast<int>(vecT.size()); ++i)
	{
		if (strName == vecT[i].m_name)
		{
			T = vecT[i];
			return true;
		}
	}
	ASSERT(("�������ƶ�Ӧ�����Ͳ�����", false));
	return false;
}


template <class Ty>
void Modify_Index(IN OUT vector<Ty>& vecT, const CString& strName)
{
	if(strName == "")
		return;
	
	for (size_t i = 0; i < vecT.size(); ++i)
		vecT[i].m_name += strName;
}

template <class Ty>
void Modify_Index2(IN OUT vector<Ty>& vecT, const CString& strName)
{
	if(strName == "")
		return;

	for (size_t i = 0; i < vecT.size(); ++i)
	{
		int nPos = vecT[i].m_name.Find("[");
		if (-1 != nPos)
			vecT[i].m_name.Insert(nPos, strName);
	}
}
template <class Ty>
void Add2Vector(IN OUT vector<Ty>& vecRes, const vector<Ty>& vecSrc)
{
	for (size_t i = 0; i < vecSrc.size(); ++i)
		vecRes.push_back(vecSrc[i]);
}