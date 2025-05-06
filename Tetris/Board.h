#pragma once

#include "General.h"

class Board
{
public:
	//конструктор
	Board();

	//функція для отримання кольору блоку в полі
	const COLORREF& getFieldBlockColor(const int X, const int Y) const;

	//функція для встановлення кольору блоку в полі
	void setFieldBlockColor(const int X, const int Y, const COLORREF& COLOR);

	//функція очищає заповнені рядки на ігровому полі
	void clearLines();

public:
	//розміри ігрового поля (фактичний розмір залежить від розміру одного блоку фігури)
	const int m_BOARD_WIDTH;	//ширина ігрового поля
	const int m_BOARD_HEIGHT;	//висота ігрового поля
	//==========================================================

private:
	std::vector<std::vector<COLORREF>> m_field;		//ігрове поле
};

