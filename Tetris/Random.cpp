#include "Random.h"

Random::Random() : 
	m_generator(std::random_device{}()) //������� ����������� (��)
{

}

int Random::getRandomInt(const int MIN, const int MAX)
{
	//������� ��� ��������� ����������� ������ �����
	std::uniform_int_distribution<int> dist(MIN, MAX);

	//���������� ����������� ������ �����
	return dist(m_generator);
}

double Random::getRandomDouble(const double MIN, const double MAX)
{
	//������� ��� ��������� ����������� ��������� �����
	std::uniform_real_distribution<double> dist(MIN, MAX);

	//���������� ����������� ��������� �����
	return dist(m_generator);
}
