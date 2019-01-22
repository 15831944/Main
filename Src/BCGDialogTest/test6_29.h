#ifndef _TEST6_29_
#define  _TEST6_29_
#include "type_traits"
void test6_29();
void testTrait();
template <class T>
CString foo(const T&val)
{
	CString str;
	using namespace  std;
	if (is_pointer<T>::value)
		str = "���õ���ָ��";
	else
		str = "���õ��Ƕ���";
	return str;
}
CString&& testMove(CString&& str);
CString testMove(CString& str);
CString testMove(const CString& str);
//
struct IntId
{
	IntId(int val){ set(val); }
	IntId():id(nullptr){ }
	void set(int val)
	{
		if (id != nullptr)
			id = new int;
		*id = val;
	}
	int * id;
	void operator =(int val){ *id = val; }
};
IntId& GetId(int value, vector< IntId*>&vecId);
int& GetId(int value,int& id);
void testGetID();

void testPair();
class person
{
public:
	//person(){}
	person(CString name="Jonh", int age=20,bool isMan=true,CString school="Сѧ") :m_name(name), m_age(age),m_isMan(isMan),m_school(school){}
	CString m_name;
	int m_age;

	CString m_school;
	bool m_isMan;
};
class clerk
{
public:
	CString m_name;
	int m_age;
	char m_index;

};
//�������͵�ǿ��ת�������뱣֤������� ��Ա����˳��һ�£�����ͻ��������
void testChange();
void testID();

#endif