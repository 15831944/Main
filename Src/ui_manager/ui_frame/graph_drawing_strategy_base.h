#pragma once


// ͼֽ���Ʋ��Ի���
class UI_MANAGER_API graph_drawing_strategy_base : public CObject
{
	DECLARE_SERIAL(graph_drawing_strategy_base)

public:
	graph_drawing_strategy_base();
	virtual ~graph_drawing_strategy_base();

public:
	// ����ͼֽ����ͼֽ��p0�����е��ļ�����ֵ��ͨ��tag�õ������ļ���������׺
	// ��ͬ�Ĺ����ܳ���ͼֽ��Ŀ��һ��������ͼֽ����֯��ʽҲ��һ������˽�ͼֽ�����·�������ƽ��ɻ�ͼ�����ڽ���ʱ
	// ����ֵ�Ǻ����ҿ��е�
	virtual bool draw(const CString& file_path_name);

public:
	// ͼֽ���ݵ�tag�����ڴ�����������ȡͼֽ����
	CString m_graph_data_tag;
};
