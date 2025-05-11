#pragma once

#include <random>

class Random
{
public:
	//�����������
	Random();

	//��������� ����������� ������ ����� � ������� [MIN, MAX]
	int getRandomInt(const int MIN, const int MAX);

	//��������� ����������� ��������� ����� � ������� [MIN, MAX]
	double getRandomDouble(const double MIN, const double MAX);

private:
	//��������� ���������� �����: Mersenne Twister 19937
	std::mt19937 m_generator;
};
