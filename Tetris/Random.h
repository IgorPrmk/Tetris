#pragma once

#include <random>

class Random
{
public:
	//конструктор
	Random();

	//генерація випадкового цілого числа в діапазоні [MIN, MAX]
	int getRandomInt(const int MIN, const int MAX);

	//генерація випадкового дробового числа в діапазоні [MIN, MAX]
	double getRandomDouble(const double MIN, const double MAX);

private:
	//генератор випадкових чисел: Mersenne Twister 19937
	std::mt19937 m_generator;
};
