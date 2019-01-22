#pragma once

// �ߵ��ӳ���ʽ
enum class x3d_extend_type{none, both, first, second};

// �ߵĽ��㷽ʽ
// ������ֱ�߶���Ϊself��������ӳ�����Ϊstart�����յ��ӳ�����Ϊend,�޽���Ϊnone
enum class x3d_intersection_type{none, self, start, end};

// ͼԪ���Ʒ�ʽ
enum class x3d_drawing_type{none, dots, lines, line_strip, line_loop, triangles, triangles_trip, triangles_fan};

// ����ķ��������㷽ʽ
// plane:ƽ�淨������ʽ����֮����������
// vertex:���㷨������ʽ����֮���������ԣ����ù⻬����
enum class x3d_normal_type{none, plane, vertex };

// ��άƽ��ķ�ʽ��oxyƽ�棬oxzƽ�棬oyzƽ��
enum class x3d_plane_type{none, oxy, oxz, oyz};