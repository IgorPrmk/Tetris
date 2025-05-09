#pragma once

#include "General.h"
#include "Figure.h"

//����, ���� ����� ������������ "���" � �����
class Decor
{
public:
	//�����������
	Decor();

	//����������� ���������  
	Decor(const Decor& other) = default;

	//�������� ���������  
	Decor& operator=(const Decor& other);

	//�������, ��� ��������� ������������ "���" � �����
	void initDecorRain();

	//�������, ��� ������� ������� ������������ �����
	void updateDecorRain();
	
	//�������, ��� ����� ������������ "���" � �����
	void drawDecorRain(const HDC DEVICE_CONTEXT);
	
	
public:
	//���� ������� ��������� �������� ������������ ����� � ���� ("���" �� �����)
	const int m_MAX_DECOR_COUNT = 10;

private:
	Figure m_figure;		//��� ������
	double m_x		= 0.0;	//������������� ��������� ������ (� �������)
	double m_y		= 0.0;	//����������� ��������� ������ (� �������)
	double m_speed	= 0.0;	//�������� ������ ������ (� ������� �� ��)
};