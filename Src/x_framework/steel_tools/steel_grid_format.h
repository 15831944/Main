#pragma once


class format_cell
{
public:
	format_cell();
	~format_cell();

public:
	CString				m_text;				// ��Ԫ���е��ı�
	bool				m_is_read_only;		// �Ƿ�Ϊֻ��
	CString				m_hint_text;		// ��ʾ����
	long				m_data_type;		// ��������
	short				m_alignment;		// ���뷽ʽ
	CString				m_label_text;		// �����б��е�����(�����@��ͷ����Ϊ������������ֹ��ȡ�ⲿ���Ļ������������������ڲ�ʹ�õĻ�������)
	int					m_cell_type;		// ��Ԫ������
	CString				m_sketch_map_name;	// ʾ��ͼ���ʽ
};

class format_caption
{
public:
	CString				m_main_title;		// ������ı���
	format_cell			m_component;		// ģ�����ӱ��Ӧ�ò���				
};

class format_design
{
public:
	CString				m_id;				// ��ʶ
	CString				m_item_name;		// ��Ŀ����
	format_cell			m_value;			// ����ֵ
	bool				m_using_string_value; // �����Ԫ��Ϊ�����б�ʱ�����ݰ�����ֵ������������Ŀ��������
};

class format_basic_grid
{
public:
	CString				m_id;				// ��ʶ
	CString				m_steel_name;		// �ֽ�����
	format_cell			m_diameter;			// ֱ��
	format_cell			m_steel_type;		// ���ֺ�
	format_cell			m_hook_type;		// �乳���ͣ�����ֻ��Ϊ��Щֵ�е����ɸ���"/", "���乳", "��׼�乳/90��", "��׼�乳/135��", "��׼�乳/180��", "�����乳/90��", "�����乳/135��", "�����乳/180��", "�����乳/90��", "�����乳/135��", "�����乳/180��"
	format_cell			m_distance;			// �˾�
	format_cell		 	m_force_type;		// �������ͣ�����ֻ��Ϊ��Щֵ�е����ɸ���"CA", "CB", "CC", "CD"
};

class format_layout_grid
{
public:
	CString				m_id;				// ��ʶ
	CString				m_sub_id;			// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	CString				m_steel_name;		// �ֽ�����
	CString				m_area;				// ��������
	format_cell			m_spaces_distance;	// ���ü��
	format_cell			m_spaces_count;		// ���� ����� �� ����
};

class format_edge_grid
{
public:
	CString				m_id;				// ������ʶ
	CString				m_sub_id;			// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	CString				m_steel_name;		// �ֽ�����
	CString				m_edge_name;		// �߾�����
	format_cell			m_location;			// λ�ã������ĵ���Ϊ����λ�㣩����ֻ����Ϊ, �����ġ��͡���Ե���е����ɸ�������ת�����ݻ����
	format_cell			m_logic_type;		// ��ϵ���ͣ������ĵ���Ϊ����λ��ʽ������ֻ��Ϊ����С�ڡ��������ڡ�������С��ȡ�����е����ɸ�
	format_cell			m_edge_distance;	// �߾�ֵ
	format_cell			m_round_level;		// ȡ����������ֻ��Ϊ��Щֵ�е����ɸ�����/������һ�������ַ��������ڱ�ʾȡ������
	format_cell			m_round_type;		// ȡ����ʽ������ֻ��Ϊ��Щֵ�е����ɸ���"/", "ƫ��", "ƫС", "��������"
	format_cell			m_expect_edge;		// �����߾ࣨ�����ĵ���Ϊ��������ࣩ
};

class format_edge_detail
{
public:
	CString				m_id;				// ������ʶ
	CString				m_sub_id;			// �ӱ�ʶ���������� ��ʶ����Ӧ��һ����Ŀ��һ��Ϊһ����д��ĸ
	CString				m_steel_name;		// �ֽ�����
	CString				m_item_name;		// ��Ŀ����
	format_cell			m_value;			// ֵ
	bool				m_using_string_value; // �����Ԫ��Ϊ�����б�ʱ�����ݰ�����ֵ������������Ŀ��������
};

class steel_grid_format
{
public:
	steel_grid_format(void);
	~steel_grid_format(void);

public:
	format_caption get_caption_format();
	format_design get_design_format(const CString& id);								// ����ʶ�õ���Ŀ��Ϣ
	format_basic_grid get_basic_format(const CString& id);							// ����ʶ�õ���Ŀ��Ϣ
	format_layout_grid get_layout_format(const CString& id, const CString& sub_id); // ����ʶ�õ���Ŀ��Ϣ
	format_edge_grid get_edge_format(const CString& id, const CString& sub_id);		// ����ʶ�õ���Ŀ��Ϣ
	format_edge_detail get_detail_format(const CString& id, const CString& sub_id);	// ����ʶ�õ���Ŀ��Ϣ

public:
	format_caption						m_caption_format;	// ��ͷ
	std::vector<format_design>			m_design_format;	// ���˼��
	std::vector<format_basic_grid>		m_basic_format;		// ������Ϣ
	std::vector<format_layout_grid>		m_layout_format;	// ������Ϣ
	std::vector<format_edge_grid>		m_edge_format;		// �߾���Ϣ
	std::vector<format_edge_detail>		m_detail_format;	// ������Ϣ

public:
	std::vector<CString>			m_vecTemplateName; // �˱��������ڲ�ʹ�ã����ڼ�¼ģ�����Ա����ʱ���ٱ���Ϣ
};
