#include "stdafx.h"
#include "test7_5.h"
#include "vector"
#include "functional"
using namespace  std;
void test7_5()
{
	testWrapper();
}
vector<reference_wrapper<CString>>GetNames()
{
	vector<reference_wrapper<CString>>vecNames;
	CString* wb = new CString("����");  //����ɾ��ָ�룬����������Ϊ�� ʹ�������ã�һ�������ڴ˺�������ʱ��������new�����б���ɾ������
	CString*lzh = new CString("������");//����ɾ��ָ�룬����������Ϊ�� ʹ�������ã�һ�������ڴ˺�������ʱ��������new�����б���ɾ������
	CString*cjd = new CString("�ܾ���");//����ɾ��ָ�룬����������Ϊ�� ʹ�������ã�һ�������ڴ˺�������ʱ��������new�����б���ɾ������
	vecNames.push_back(*wb);
	vecNames.push_back(*lzh);
	vecNames.push_back(*cjd);
	vector<int>vecInt{ (int)&wb, (int)&lzh, (int)&cjd };		//ָ������Լ��� ջ��ַ
	vector<int>vecInt1{ *(int*)wb, *(int*)lzh, *(int*)cjd };	//ָ����ָ�ڴ�� �ѵ�ַ��ǰ4���ֽڵ���int����
	vector<int>vecInt2{ (int)wb, (int)lzh, (int)cjd };			//ָ����ָ�ڴ�� �ѵ�ַ
	return vecNames;
}

void testWrapper()
{
	vector<reference_wrapper<CString>> names = GetNames();
	//names[0].get() = CString("����");
	names[0].get() = "����";


}