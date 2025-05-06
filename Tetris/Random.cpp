#include "Random.h"

Random::Random() : 
	m_generator(std::random_device{}()) //джерело випадковості (ОС)
{

}

int Random::getRandomInt(const int MIN, const int MAX)
{
	//границі для генерації випадкового цілого числа
	std::uniform_int_distribution<int> dist(MIN, MAX);

	//повернення випадкового цілого числа
	return dist(m_generator);
}

double Random::getRandomDouble(const double MIN, const double MAX)
{
	//границі для генерації випадкового дробового числа
	std::uniform_real_distribution<double> dist(MIN, MAX);

	//повернення випадкового дробового числа
	return dist(m_generator);
}
