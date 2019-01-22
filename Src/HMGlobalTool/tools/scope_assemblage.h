///////////////////////////////////////////////////////////////////////////////////////////////////
// ������
//     �ṩ�˼��ϡ���������ݽṹ�Լ���Ӧ�����㹦�ܣ����нӿ��ϸ�����ѧ�ϵ���ض�����ʵ�֣�������
//     ��ı�﷽ʽ�Լ����塢���ϵĺ����Լ���Ӧ����ѧ���㹦�ܼ��߼�������֮�⣬����չ��һЩ��ѧѧ��
//     ��û�еĹ��ܣ��Ա����ʹ���߲����ϵı����ԣ���Գ�ʼ�����ַ�������ʶ���У�����Զ�����乹��
//     �ļ������Զ������ص����ֵĺϲ��;����
// 
// ���ߣ�Wang Youzhou
// ά����
//     --V2.2 2015-4-23------------------
//     *ȥ��mfc����
//     *ȥ���ò����Ľӿ�
//     *�����淶��STL�������
//     *�����ⲿ���õ���С���ߺ�����ʹ���벻�����������ļ�
//     *�ӿ���ͬʱ֧��MBCS��Unicode�ַ���
//     *ȥ�����л��ӿڣ����ⲿ�õ��ַ�������������л��Ա�����MFC
//     *����һЩ�ӿ��ϵĵ���ƴд����
//     *scope_value()��Ϊ���츺��������ԭ��Ϊ��ֵ0.0��
//     *scope_value����ʹ�õ����������ֽ��й��������ֵ
//     *assemblage_value����ʹ�õ����������ֽ��й��������ֵ
// 
//     --V2.1 2014-12-4------------------
//     *����һЩע�ʹ���
//     *��������is_in_assemblage�������¹�����⹦�ܽӿ��У������ж��Ƿ�����ָ�����ϣ��൱����ѧ�ϵġʲ�����������ȫ
//     *����������ʱ������ֵ������0��ʾ������һ�������ܴ�򾡿���С������
//     
//     --V2.0----------------------------
//     *����һЩС����
//     *�������л�����
//     *�Ż��ڲ����ϴ����߼�
//     *��ԭ���ӿڽ���������������ȫ������ѧ�ϵĽз�
//     *����������ѧ��û�е�����ӿ��Լ����Ʋ��淶�Ľӿڣ��Ѿ��������ˣ����Խ������ӿڲ����Ϊ������
//     *������ѧ�ϵļ����󽻡��󲢡��Ӽ������жϽӿ�
//     *�¾�number�����䡢���ϵ��߼���������ع��ܣ��Ա���õĴ�����������Ի�
//     *���������и�Ԫ�ص��Ż����ص�����ĺϲ����ܣ������Ч�ʲ����õ�֧�ּ��ϲ���
//     *�������ڹ��켯�ϡ�������ַ���ʹ�������ķ��ŵĹ��ܣ��罫���ĵ����ź������Զ�תΪӢ�ĵ��ַ���
//     
//     --V1.1----------------------------
//     *�ṩ�����ַ���ԭ������ͷ��صĹ���
//     *�����������ݽṹ����Ӧ����
//     *��չ�������ɼ��ϡ�������ַ����ĸ�ʽ��ʹ����ʽ�����Ի�
//     
//     --V1.0----------------------------
//     *�ṩһ�����׵������Ｐ��ϵ�жϵĹ���
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __HMGLOBALTOOL_TOOLS_SCOPE_ASSEMBLAGE_H__
#define __HMGLOBALTOOL_TOOLS_SCOPE_ASSEMBLAGE_H__

#pragma warning(push)

// warning C4251: '***' : class needs to have dll-interface to be used by clients of class '***'
#pragma warning(disable: 4251)


// ��ֵ�࣬���Ա�ʾ��������ڲ�ʹ��double�洢
class HM_GlobalTool_EXT number
{
public:
	number(); // �൱����0.0��ʼ��
	number(short value);
	number(int value);
	number(long value);
	number(unsigned short value);
	number(unsigned int value);
	number(unsigned long value);
	number(float value);
	number(double value);
	number(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������
	number(const number& num);
	~number();

public:
	void set_value(short value);
	void set_value(int value);
	void set_value(long value);
	void set_value(unsigned short value);
	void set_value(unsigned int value);
	void set_value(unsigned long value);
	void set_value(float value);
	void set_value(double value);
	void set_value(const number& num);
	void set_value(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������
	void set_infinitely_p();			// ����ֵΪ������
	void set_infinitely_n();			// ����ֵΪ������
	bool is_infinite() const;			// ��ǰֵ�Ƿ�Ϊ��ֵ�������������ʱ����true��Ϊһ���ض�����ֵʱ����false
	bool is_value() const;				// ��ǰֵ�Ƿ�Ϊһ���ض������֣��൱��IsInfinite()���ȡ�ǣ����������false�����򷵻�true
	bool is_infinite_p() const;			// �Ƿ�Ϊ������
	bool is_infinite_n() const;			// �Ƿ�Ϊ������
	double get_value() const;			// Ϊ������ʱ����һ���㹻С�����֣�Ϊ������ʱ����һ���㹻������֣��벻Ҫ��Ϊ����ֵ����������������������Ϊ������ĸ��������з�Χ���Ƶ�
	std::string to_string_a() const;	// ת��Ϊ�ַ���������6λ��Ч���֣�����ĩβ��0��ɾ�������Ϊ��ֵ�򷵻ؿմ�
	std::wstring to_string_w() const;	// ת��Ϊ�ַ���������6λ��Ч���֣�����ĩβ��0��ɾ�������Ϊ��ֵ�򷵻ؿմ�

public:
	// ֧�ֵ��߼������
	friend bool operator<(double value, const number& num);
	friend bool operator<(const number& num, double value);
	friend bool operator>(double value, const number& num);
	friend bool operator>(const number& num, double value);
	friend bool operator>=(double value, const number& num);
	friend bool operator>=(const number& num, double value);
	friend bool operator<=(double value, const number& num);
	friend bool operator<=(const number& num, double value);
	friend bool operator!=(double value, const number& num);
	friend bool operator!=(const number& num, double value);

	bool operator<(const number& num) const;
	bool operator==(const number& num) const;
	bool operator>(const number& num) const;
	bool operator>=(const number& num) const;
	bool operator<=(const number& num) const;
	bool operator!=(const number& num) const;
	bool operator!() const;

	// CNumΪ��ֵʱ
	number& operator=(double value);
	number& operator=(const number& num);

	// CNumΪ��ֵʱ
	operator double() const; // ��ֵʱ����0

private:
	// ֧�ֵ���ѧ�����
	friend const number operator+(const number& num, double value);
	friend const number operator+(double value, const number& num);
	friend const number operator+(const number& num1, const number& num2);
	friend const number operator-(const number& num, double value);
	friend const number operator-(double value, const number& num);
	friend const number operator-(const number& num1, const number& num2);
	friend const number operator*(const number& num, double value);
	friend const number operator*(double value, const number& num);
	friend const number operator*(const number& num1, const number& num2);
	friend const number operator/(const number& num, double value);
	friend const number operator/(double value, const number& num);
	friend const number operator/(const number& num1, const number& num2);

private:
	double m_value;
	int m_type; // Ϊ0��ʾһ�������ʵ���������ִ洢��m_value�У�Ϊ-1��ʾ����С��Ϊ1��ʾ�����
};


////////////////////////////////////////////////////////////////////////////////////////////////////

// ��ʾ��ѧ�ϵ�һ������������ֵ������һ����ֵ��Ҳ������һ����Χ
// ע��
//   1.��ѧ��������������С���ţ�����������ű�ʾû�����壬�԰�С���Ŵ����������ڲ�����Ϊ��С������
//     �����ź��岻һ��
//   2.��ѧ��������������һ����+�Ż��-�ŵĺ��ŵ�����8��ʾ����������Ϊ������������ݻ����һ��-�ű�
//     ʾ����������ұ������ݻ����һ��+�ű�ʾ������
//   3.�����ʾ������ַ���ȱ��С���Ż������ţ���С���Ŵ�����������
//   4.���š������š�С���ź�����֮��������������ո��Ա�����û��ʵ��ָ��������ݿ�����������
// 
// ���¾�Ϊ�Ϸ����ݣ�
//   35				��ֵ35
//   (11,)			���䣺11�����������11(��������������ѧ����С���ţ�������������ű�ʾ�����壬�԰�С���Ŵ��������ڲ�����ΪС�����������ź��岻һ��)
//   [,11)			���䣺�����11������11
//   (,)��(-,+)		���䣺�����������
//   (11,22)		���䣺11��22������11��Ҳ����22
//   [11,22]		���䣺11��22������11��Ҳ����22
//   (11,22]		���䣺11��22������11��������22
//   [11,22)		���䣺11��22������11��������22
//   Ҫ���������������ֵ��Ϊ�ɱ�ʾ�ĳ���ʵ��ֵ��֧������������������ֵ
//   ���洢��ֵΪһ����ֵʱҲ��һ����Ҫ��
//   ע�����䲻����Ϊ�����䣬����ѧ��һ�������������ַ���������������ʾ�����������
class HM_GlobalTool_EXT scope_value
{
public:
	// �൱�ڸ����������
	scope_value();

	// ����һ�����洢������ֵ��scope_value
	scope_value(short value);
	scope_value(int value);
	scope_value(long value);
	scope_value(unsigned short value);
	scope_value(unsigned int value);
	scope_value(unsigned long value);
	scope_value(float value);
	scope_value(double value);
	scope_value(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������
	
	// ����һ�����洢������ֵ��scope_value
	scope_value(number val);

	// ���Ϊ���ַ�����ʾ�����������
	scope_value(const std::string& text);
	scope_value(const std::wstring& text);
	scope_value(const char* text);
	scope_value(const wchar_t* text);
	
	// leftֻ��Ϊ��ǵ�"("����"["��rightֻ��Ϊ��ǵ�")"����"]"�����򽫰������䴦��
	scope_value(const std::string& left, number l_val, number r_val, const std::string& right);
	scope_value(const std::wstring& left, number l_val, number r_val, const std::wstring& right);
	scope_value(const char* left, number l_val, number r_val, const char* right);
	scope_value(const wchar_t* left, number l_val, number r_val, const wchar_t* right);

	scope_value(bool is_open_left, const number& l_val, const number& r_val, bool is_open_right);

	~scope_value();

public:
	// ǿ���������ò��洢һ����ֵ
	void set_value(const number& val);

	// ǿ���������ò��洢һ����ֵ
	void set_value(short value);
	void set_value(int value);
	void set_value(long value);
	void set_value(unsigned short value);
	void set_value(unsigned int value);
	void set_value(unsigned long value);
	void set_value(float value);
	void set_value(double value);
	void set_value(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������

	// ����ʾ������ַ���תΪһ���������(�ַ�����ʽ�ɲο�����assemblage_value����ʹ���ַ�����ʼ������ʱ���������˵��)
	void set_value(const std::string& strText);
	void set_value(const std::wstring& strText);
	void set_value(const char* text);
	void set_value(const wchar_t* text);

	// ǿ���������ò�����һ������ֵ��leftֻ��Ϊ��ǵ�"("����"["��rightֻ��Ϊ��ǵ�")"����"]"�����򽫰������䴦��
	void set_value(const std::string& left, number l_val, number r_val, const std::string& right);
	void set_value(const std::wstring& left, number l_val, number r_val, const std::wstring& right);
	void set_value(const char* left, number l_val, number r_val, const char* right);
	void set_value(const wchar_t* left, number l_val, number r_val, const wchar_t* right);

	// ����������ֵ,�����䱣�ֲ��䣬strLeftֻ��Ϊ��ǵ�"("����"["�����򽫰������䴦��
	void set_value(const std::string& left, const number& l_val);
	void set_value(const std::wstring& left, const number& l_val);
	void set_value(const char* left, const number& l_val);
	void set_value(const wchar_t* left, const number& l_val);

	// ����������ֵ,�����䱣�ֲ��䣬strRightֻ��Ϊ��ǵ�")"����"]"�����򽫰������䴦��
	void set_value(const number& r_val, const std::string& right);
	void set_value(const number& r_val, const std::wstring& right);
	void set_value(const number& r_val, const char* right);
	void set_value(const number& r_val, const wchar_t* right);

	// ����������ֵ
	void set_value(bool is_open_left, const number& l_val, const number& r_val, bool is_open_right);

	// ����������ֵ,�����䱣�ֲ���
	void set_value(bool is_open_left, const number& l_val);

	// ����������ֵ,�����䱣�ֲ���
	void set_value(const number& r_val, bool is_open_right);

	// ����������ֵ,�����䱣�ֲ��䣬leftֻ��Ϊ��ǵ�"("����"["�����򽫰������䴦��
	void set_left_value(const std::string& left, const number& l_val);
	void set_left_value(const std::wstring& left, const number& l_val);
	void set_left_value(const char* left, const number& l_val);
	void set_left_value(const wchar_t* left, const number& l_val);

	// ����������ֵ,�����䱣�ֲ��䣬rightֻ��Ϊ��ǵ�")"����"]"�����򽫰������䴦��
	void set_right_value(const number& r_val, const std::string& right);
	void set_right_value(const number& r_val, const std::wstring& right);
	void set_right_value(const number& r_val, const char* right);
	void set_right_value(const number& r_val, const wchar_t* right);

	// ����������ֵ,�����䱣�ֲ���
	void set_left_value(bool is_open_left, const number& l_val);

	// ����������ֵ,�����䱣�ֲ���
	void set_right_value(const number& r_val, bool is_open_right);

	// ��ȡ�������ֵ������������䣬������һ�����ݣ��򷵻ظ�����,is_open�����Ƿ�Ϊ������
	void get_left_value(number& l_val, bool& is_open) const;

	// ��ȡ�������ֵ������������䣬������һ�����ݣ��򷵻ظ�����
	const number get_left_value() const;

	// ��ȡ�������ֵ
	void get_right_value(number& r_val, bool& is_open) const;

	// ��ȡ�������ֵ������������䣬������һ�����ݣ��򷵻ظ�����
	const number get_right_value() const;

	// ��������д洢����һ��ֵ��ֱ�ӷ��أ����򷵻��������ֵ
	void get_single_value(number& val) const;

	// ��ȡ�������ֵ������������䣬������һ�����ݣ��򷵻ظ�����
	const number get_single_value() const;

	// �ж���߻����ұ��Ƿ�Ϊ������
	bool is_open(bool is_left = true) const;

	// �ж���߻����ұ��Ƿ�Ϊ������
	bool is_closed(bool is_left = true) const;

	// �жϵ�ǰ�����д洢����һ��ֵ������һ�����䣬���Ϊֵ�򷵻�true�����򷵻�false
	bool is_single_value() const;

	// �ж�ĳ��ֵ�Ƿ����������ڣ���Ϊ����ѧ�ϱ���һ�£�
	bool is_in_scope(const number& val) const;

	// �ж�ָ���������Ƿ���ȫ���ڱ������ڣ���Ϊ����ѧ�ϱ���һ�£�
	bool is_in_scope(const scope_value& scope) const;

	// ��ȡScope��ǰ��״̬������-1��ʾ����״̬�����ҽ��洢һ�����ݣ�����0��ʾ�����������ֵ����
	// ֵ��ȴ���,����1��ʾ����״̬�����Ҵ洢����һ�����䷶Χ��ע��[3, 3]��������Ҳ����0��ʾ����
	int get_scope_status() const;

	// �����䰴������ֵ����ֳ�������������, is_closeΪ���ʾ�𿪵���������ӿڴ���Ϊ�����䣬����Ϊ��
	// ���䣬���������������Ϊ�������䣬�򷵻�true�����򷵻�false�����������¹���
	// 1.������������һ����ֵ������is_closeΪtrueʱ��������ͬ��������ֵ���䲢����true�����򷵻�false
	// 2.�����ֵ���ֵ���ڱ������ڣ��򷵻�false
	// 3.�����ֵ���ֵ�պ��Ƕ˵�ֵ��is_closeΪfalseʱ����ֻ�����������һ�����䣬��������������ʱҲ����false
	// ԭ��ֻ�ܲ�ֺ����������κ�һ������ѧ�Ƕ��������ǷǷ��ģ���ô����ͻ᷵��false,���򷵻�true
	bool split_scope(const number& val, bool is_close, scope_value& out_left, scope_value& out_right) const;

	// ����ǰ������ָ������ϲ�Ϊһ�����䲢����ڱ�������
	// ����޷�����һ���򷵻�false���ɹ�����true
	// �������false�򲻻�Ա��������κθĶ���ʾ�����£�
	// 3 ��(3,5) ���ӳ�Ϊ[3, 5)
	// (3, 5) �� [5, 10]���ӳ�Ϊ(3, 10)
	// (3, 5) �� (5, 10]�޷�����
	// (1, 5) �� (3, 10)���ӳ�Ϊ(1, 10)
	bool merge_scope_value(const number& val);
	bool merge_scope_value(const scope_value& val);

	// ��ȡscope_value��ǰֵ��Ӧ���ַ���,(������ʹ���ַ�����ʼ��ʱԭ�������ַ���)������洢һ��ֵ����ֱ��
	// ���ض�Ӧ�������ַ��������Ϊ���䣬����ѧ�ϵı�ʾ����ķ�ʽ���أ���ֵ�ÿ��ַ�������
	std::string to_string_a() const;
	std::wstring to_string_w() const;

public:
	bool operator==(const scope_value& scope) const;
	bool operator!=(const scope_value& assem) const;

private:
	bool is_old_ui_string() const;	// �ڲ�ʹ��
	std::wstring get_old_ui_string() const;	// �ڲ�ʹ��

private:
	number		m_left;				// �洢������ֵ���ߵ������н��洢һ��ֵʱҲ���浽��
	number		m_right;			// �洢������ֵ
	bool		m_is_single_value;	// �Ƿ�洢����һ��ֵ,true��ʾ�ǣ�false��ʾ��ǰ��һ������
	bool		m_is_left_open;		// �������Ƿ�Ϊ�����䣬��m_is_single_valueΪfalseʱ��Ч
	bool		m_is_right_open;	// �������Ƿ�Ϊ�����䣬��m_is_single_valueΪfalseʱ��Ч

	// ��¼��ֵ�Ƿ񻹱�����void set_value();ʹ�õ����ַ�������ֵʱ���������,�û��κθĶ���
	// �Ĳ������������ֵ��Ϊfalse,��Ҫ���ڻ�������ַ���
	bool		m_is_old_ui_string;

	std::wstring m_old_ui_string;	// ��ʼ��ʱ���洫����ַ���
};


////////////////////////////////////////////////////////////////////////////////////////////////////

// ��ʾ�ɶ��scope_valueԪ����ɵ�һ������
class HM_GlobalTool_EXT assemblage_value
{
public:
	assemblage_value();
	assemblage_value(const assemblage_value& assem);
	assemblage_value(const std::string& text);			// �ַ�������ο�set_value()��Ա����
	assemblage_value(const std::wstring& text);			// �ַ�������ο�set_value()��Ա����
	assemblage_value(const char* text);
	assemblage_value(const wchar_t* text);
	assemblage_value(short value);
	assemblage_value(int value);
	assemblage_value(long value);
	assemblage_value(unsigned short value);
	assemblage_value(unsigned int value);
	assemblage_value(unsigned long value);
	assemblage_value(float value);
	assemblage_value(double value);
	assemblage_value(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������
	assemblage_value(const number& val);				// ����洢һ������,�����ڽ���һ����ֵ
	assemblage_value(const scope_value& val);			// ����洢һ������
	~assemblage_value();

public:
	// ʹ���ַ�����ʼ������,����ʹ�ð�Ƿֺŷָ����Ԫ��,ÿһ��Ԫ�ص��ַ����������ܺϷ��ı�ʾ
	// һ��scope_value,���һ��Ԫ��֮��Ҫ�ֺ�,������������ԣ�����ֺ�ʱ��ɾ��һ���������
	// �����ַ�������,����ַ���Ϊ�������һ����ʾ����������������Ԫ��.
	// �ַ�����ʽ˵�����£�
	//   ��Χ����һ������Ԫ����ɣ�����Ԫ�ؿ�����һ�������һ����ֵ��Ԫ��֮���ð�Ƿֺ����
	//   ����ǰ������ڷָ�scope_value�ķֺ�ǰ��������������ո�ָ����Ա����ݿ�����������
	//   ����Ԫ�ظ�ʽ���£�
	//   �� 123.45  ��ʾ�̶���ֵ123.45
	//   �� (,)  ��ʾ����������Ҳ�ɲ�����ʽ(-,+)
	//   �� (,123)  ��ʾ�����123������123��Ҳ�ɲ���(-,123)
	//   �� [12.3,56.7)  �������ѧ�ϵ��������һ�£������ڵ���12.3��С��56.7
	//   �� ��Ԫ��Ϊ�ձ�ʾ�����������
	//   �� Ԫ��Ϊ����ʱ����������ֵ֮���ð�Ƕ��Ż��ǿո��������(12,34)��ͬ��(12��34)
	//   �� Ԫ��Ϊ����ʱ����ʡ������һ�ߵ����ţ�����ʡ��ʱ��ʾ�����䣬��12��34��ͬ��(12,34)��12,34)
	//   �� �Ƿ����ݻ��ʽʹ��ʱ������
	//   ��Χ�ɶ��Ԫ����ɵ�ʾ����
	//   [12.3,45.6);78;55,66;77  88��ʾ�����Χ������[12.3,45.6)����ֵ78������(55,66)������(77,88)�Ĳ���
	void set_value(const std::string& text);
	void set_value(const std::wstring& text);
	void set_value(const char* text);
	void set_value(const wchar_t* text);
	void set_value(short value);
	void set_value(int value);
	void set_value(long value);
	void set_value(unsigned short value);
	void set_value(unsigned int value);
	void set_value(unsigned long value);
	void set_value(float value);
	void set_value(double value);
	void set_value(bool is_infinitely_great_or_small); // true��ʾֵΪ�����false��ʾֵΪ������
	void set_value(const number& val);								// ǿ�Ƽ������ò��洢һ������,�����ڽ���һ����ֵ
	void set_value(const scope_value& val);							// ǿ�Ƽ������ò��洢һ������
	void set_value(const assemblage_value& value);					// ǿ�Ƽ������ò��洢һ������
	void set_value(const std::vector<scope_value>& scope_items);	// ǿ�Ƽ������ò��洢�ɶ��������ɵļ���

	void set_null();												// �������Ԫ�ز���Ϊ�ռ�
	bool is_null() const;											// �Ƿ��ǿռ�

	// ��������ֻ��Ϊ��ʹ�ö������Ҳ��ķѶ����ڴ棬ֻҪ�������к�������Ա�������Ч����˿��Բ��ý��շ���ֵ�ٴθ�ֵ
	assemblage_value& intersection(const number& num);				// ��ǰ��������һ������Ľ���
	assemblage_value& intersection(const scope_value& scope);		// ��ǰ��������һ������Ľ���
	assemblage_value& intersection(const assemblage_value& scope);	// ��ǰ��������һ������Ľ���

	assemblage_value& union_set(const number& num);					// ��ǰ��������һ������Ĳ���
	assemblage_value& union_set(const scope_value& scope);			// ��ǰ��������һ������Ĳ���
	assemblage_value& union_set(const assemblage_value& scope);		// ��ǰ��������һ������Ĳ���

	bool has_intersection(const number& num) const;					// �жϵ�ǰ��������һ�������Ƿ���ڷǿյĽ���,�����жϣ������޸��ڲ�����
	bool has_intersection(const scope_value& scope) const;			// �жϵ�ǰ��������һ�������Ƿ���ڷǿյĽ���,�����жϣ������޸��ڲ�����
	bool has_intersection(const assemblage_value& assem) const;		// �жϵ�ǰ��������һ�������Ƿ���ڷǿյĽ���,�����жϣ������޸��ڲ�����
	
	bool is_subset(const assemblage_value& assem) const;			// �жϵ�ǰ�����Ƿ���ָ�����ϵ��Ӽ��������ǰ����Ϊ�ջ���ָ���������Ҳ����true��
	bool is_superset(const assemblage_value& assem) const;			// �жϵ�ǰ�����Ƿ���ָ�����ϵĳ��������ָ������Ϊ�ջ��뵱ǰ�������Ҳ����true��
	
	bool is_in_assemblage(const number& num) const;					// ָ���Ķ����Ƿ��ڱ������ڣ��൱���ж��뱾���ϵĽ����Ƿ�Ϊ�գ�
	bool is_in_assemblage(const scope_value& scope) const;			// ָ���Ķ����Ƿ��ڱ������ڣ��൱���ж��뱾���ϵĽ����Ƿ�Ϊ�գ�
	bool is_in_assemblage(const assemblage_value& scope) const;		// ָ���Ķ����Ƿ��ڱ������ڣ��൱���ж��뱾���ϵĽ����Ƿ�Ϊ�գ�

public:	
	const std::string to_string_a() const;							// ������Ԫ��ת���ɰ�Ƿֺŷָ���Ԫ��(������ʹ���ַ�����ʼ��ʱԭ�������ַ���)
	const std::wstring to_string_w() const;							// ������Ԫ��ת���ɰ�Ƿֺŷָ���Ԫ��(������ʹ���ַ�����ʼ��ʱԭ�������ַ���)
	void to_scope_items(std::vector<scope_value>& scope_items) const; // �����ϲ�ɶ������õ����䣬����������ص������ڲ��Ż����ϲ�����ֺ���������乹�ɵļ�����ԭ���ϵȼ�
	const std::vector<scope_value> to_scope_items() const;			// �����ϲ�ɶ������õ����䣬����������ص������ڲ��Ż����ϲ�����ֺ���������乹�ɵļ�����ԭ���ϵȼ�

public:
	bool operator==(const assemblage_value& assem) const;			// ֻҪ�߼����䷶Χ��ȫ�ص�����Ϊ���,������ָԪ��˳��,���ݵ���ȫ��ͬ
	bool operator!=(const assemblage_value& assem) const;			// ֻҪ�߼����䷶Χ��ͬ����Ϊ�����,����Ҫ������Ԫ��˳��,������ȫ��ͬ����Ϊ��false
	assemblage_value& operator=(const assemblage_value& assem);

private:
	void arrange(); // �ڲ�ʹ�ã��������ڲ���Ԫ�أ����ܺϲ��������Ԫ��ȫ���ϲ�����[1,3][2,5]��Ϊ[1,5]�������Ĺ���ʱ����ĳ�ʼ���ַ���

private:
	std::vector<scope_value> m_all_scope;

	// ��¼��ֵ�Ƿ񻹱�����void set_value(const CString& strText);�ӿڴ��������,�û����κθĶ��Ե�
	// �������������ֵ��Ϊfalse,��Ҫ���ڻ�������ַ���
	bool			m_is_old_ui_string;

	std::wstring	m_old_ui_string; // ��ʼ��ʱ���洫����ַ���
};

#pragma warning(push)
#endif //__HMGLOBALTOOL_TOOLS_SCOPE_ASSEMBLAGE_H__
