#pragma once
#include <utility>
#include "../interface/serialize_interface.h"
#include "../interface/data_center_interface.h"

// �Ǽֽܸ�Ŀ��Ƶ�(P0����)//��Ӧ"���Ƶ��"��һ��
class X_FRAMEWORK_API frame_data_node
{
public:
	frame_data_node();
	~frame_data_node();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString			m_ref_line;				// �ο������ƣ����������ţ�
	CString			m_dist_to_ref_line;		// ���Ƶ㵽�ο��ߵľ���
	CString			m_begin_layer;			// �����(�ֽ�˵�Ĳ�����Ϊ0)
	CString			m_end_layer;			// ��ֹ��(�ֽ�˵�Ĳ�����Ϊ0)
	
	// ���Ƶ����䷽���ã����ο��߰������ţ�1Ϊ���ã�0Ϊ������),�˱������㷨�������������߼��г�ͻ������֮��
	// ������ܶ໭ͼ�������Ϊ�����Ը������ǽ��ô˱����ģ�֮���Էſ�����Ϊ�ϲ��ṹ��������ʱ�����÷���һ�£�
	// �ֽ�ͼ���ܳ���ȷ���˸����ã��㷨����ȫ�ɲ�Ҫ������εı��������Ĺ��������ˡ�
	CString				m_transposed;

	// ���Ƶ����ڸֽ��ֱ������ֵ�Ϲ�����֧�֣�ֻ���¹�����֧�֣�Ϊ0��հ���1����Ϊ����ʾ��ֱ���ľ���ֵ��
	// �иֽ�ã�Ϊ����ʾ�ֽ�ֱ��������֮ǰ���ˡ�\�����ʾ�Ǽ���Ϣ�������ֱ������ţ���1��ʼ�����޷�����ʱ��
	// Ĭ��ȡ��1����
	CString				m_diameter;
};


// �Ǽֽܸ���乳��������P0����
class X_FRAMEWORK_API frame_data_hook
{
public:
	frame_data_hook();
	~frame_data_hook();

public:
	void serialize_members(member_rw_interface& mt);

public:
	static CString get_general_hook_name(); // ����ͨ���乳������

public:
	CString			m_index_name;			// ��������
	CString			m_hook_type;			// �乳��ʽ,0,ֱ���乳;1,Բ���乳;2,�����乳;
	CString			m_hook_length;			// �乳ֱ�߶γ���
	CString			m_arc_radius;			// Բ���뾶
	CString			m_hook_end_type;		// �˲��ضϵ���乳�ķ�������[0,�乳Ϊ��ֱ����;1,�乳Ϊ��ֱ����;2,ָ���Ƕ�dAngle]
	CString			m_hook_end_angle;		// �˲��ضϵ���乳��ֽ��ߵļнǣ��Ƕȣ�
	CString			m_hook_direction;		// �乳����true���ڡ�false���⣩
	CString			m_binded_segment;		// �乳�������ĸֽ����
};


// һƬ�Ǽֽܸ���P0����
class X_FRAMEWORK_API frame_data_piece
{
public:
	frame_data_piece();
	~frame_data_piece();

public:
	void serialize_members(member_rw_interface& mt);

public:
	CString							m_index_name;					// �Ǽֽܸ��������������
	bool							m_need_weld;					// �Ƿ񺸳ɹǼ�[TRUE,�Ǽֽܸ���;FALSE,�ǹǼֽܸ���]
	bool							m_is_symmetric;					// �Ƿ�Գ�
	CString							m_steel_type;					// �ֽ�ĸ��ֺ�(ȱʡΪ2,�����Ƹ�)
	CString							m_diameter;						// �ֽ�ֱ��(mm)����������ֱ��������֮���ð�ǿո񡢶��Ż�ֺ���������ֱ��Ϊ�����øֽ�
	CString							m_hook_weld_length;				// �乳�ĺ��쳤��
	CString							m_top_space;					// �û�ָ���Ķ����������(0-based,ֵΪ0��ʾȡ�ֽ��⾶)
	CString							m_bottom_space;					// �û�ָ���ĵײ��������(0-based,ֵΪ0��ʾȡ�ֽ��⾶)
	CString							m_top_flag;						// ���������Ƿ�˳�����ο���(0-based)
	CString							m_bottom_flag;					// �ײ������Ƿ�˳�ײ��ο���(0-based)
	CString							m_bent_angle;					// ����Ƕ�(�Ƕ�)
	CString							m_arc_steel_radius;				// Բ������ɰ뾶
	CString							m_mid_bent_radius;				// ���۵㴦�Ĺ���Բ���뾶
	CString							m_mid_weld_length;				// �м����۵�ĺ��쳤��
	bool							m_has_top_perforation_steel;	// �Ƿ��ж���ͨ����
	bool							m_has_bottom_perforation_steel;	// �Ƿ��еײ�ͨ����
	CString							m_hook_settings;				// ָ���乳��Ϣ
	std::vector<frame_data_node>	m_top_nodes;					// �ϲ����Ƶ�
	std::vector<frame_data_node>	m_bottom_nodes;					// �²����Ƶ�
	CString							m_laction;						// Ӧ�ò�λ��Ŀǰ�����ڹǼܱ༭�������治��ʾ��
};


// �����ȡ�����йǼ��������
class X_FRAMEWORK_API frame_data : public data_node_base
{
public:
	frame_data();
	~frame_data();

public:
	std::vector<frame_data_piece>	m_all_frames;			// ȫ���ĹǼ���Ϣ
	frame_data_hook					m_general_hook;			// ͨ���乳��Ϣ����һ���ұ�����һ�������乳��������Ϊ��ͨ���乳��
	std::vector<frame_data_hook>	m_all_hooks;			// ���е��乳������Ϣ

public:
	virtual void serialize_members(member_rw_interface& mt);

public:
	// ����ָ���Ǽ���
	frame_data_piece* get_sepcial_frame(const CString& name);
	
	// �����Ǽ����ݴ������õ��ĳ˺ű��ʽ����ʽ���� 100, 200,300*5 50 89 , 90, A B, C*3, Str*5, 100.123 *  5
	//��ע�����ʽ��2012-3-26 12:21:08��Ϊ�����ں�������ǰ��Ϊ��ֹ����ԭע�Ͳ��ģ����ڴ���ע��
	// һ�����ڽ����������йǼܵ����Ͷ�����ñ��ʽ
	// �����ɹ�����true��ʧ�ܷ���false
	static bool parse_frame_expression(IN const CString& text, OUT std::vector<CString>& result);
	
	//��һ���ο����ַ�����ȡ����ż��ο��ߵ��������ƣ���"KD1"���õ�"KD"��1,�ο���û����ŵ�ʱ����KM֮�ࣩ����-1
	static std::pair<int, CString> get_ref_line_info(const CString& ref_line_full_name);

	//����KD��KZ��������ֵ
	static int find_max_kd_kz_index(const CString& strRefString, const std::vector<frame_data_node>& vecNodes);
};
