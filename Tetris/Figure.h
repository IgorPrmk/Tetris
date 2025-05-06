#pragma once

#include "General.h"

enum FigureType
{
	I,
	O,
	T,
	S,
	Z,
	J,
	L
};

//клас, який описує фігуру
class Figure
{
public:
	//конструктор
	Figure();

	//функція для генерації випадкового кольору блоку фігури
	static COLORREF getRandomColor();
	
	//функція для отримання координат блоків фігури
	const std::vector<POINT>& getBlocksPosition() const;

	//функція для встановлення координат блоків фігури
	Figure& setBlocksPosition(const std::vector<POINT>& BLOCKS_POSITION);

	//функція для отримання координат фігури
	POINT getFigurePosition() const;

	//функція для встановлення координат фігури
	Figure& setFigurePosition(const POINT& POSITION);
	
	//функція для переміщення фігури по ігровому полю
	void moveFigure(const int X, const int Y);
	
	//чи врізається в щось фігура (true – так, врізається)
	bool checkCollision();

	//функція для обертання фігури на 90 градусів за годинниковою стрілкою
	void rotateFigure();
	
	//функція для обертання фігури на 90 градусів TIMES-разів за годинниковою стрілкою
	//використовується для декоративного "дощу" з фігур в меню
	void rotateFigure(const int TIMES);
	
	//функція фіксує фігуру на ігровому полі, коли вона більше не може рухатися вниз
	void freezeFigure();
	
	//функція, яка прискорює падіння фігури до самого низу
	void quickFall();
	
	//функція для встановлення кольору фігури
	Figure& setColor(const COLORREF& COLOR);

	//функція для отримання кольору фігури
	const COLORREF& getColor() const;

public:
	//колір фігур ==============================================
	static const COLORREF m_AZURE_I		= RGB(0, 255, 255);	//блакитна фігура		форма: I
	static const COLORREF m_YELLOW_O	= RGB(255, 255, 0);	//жовта фігура			форма: O
	static const COLORREF m_PURPLE_T	= RGB(139, 0, 255);	//фіолетова фігура		форма: T
	static const COLORREF m_GREEN_S		= RGB(0, 255, 0);	//зелена фігура			форма: S
	static const COLORREF m_RED_Z		= RGB(255, 0, 0);	//червона фігура		форма: Z
	static const COLORREF m_BLUE_J		= RGB(0, 0, 255);	//синя фігура			форма: J
	static const COLORREF m_ORANGE_L	= RGB(255, 128, 0);	//помаранчева фігура	форма: L
	//==========================================================
	
	//максимальна кількість фігур в грі (7 фігур: I, O, T, S, Z, J, L)
	static const int m_MAX_NUMBER_FIGURES = 7;

	//всі доступні кольори для фігур
	static const COLORREF m_COLORS[m_MAX_NUMBER_FIGURES];

	//розмір одного блоку фігури (40*40 пікселів)
	static const int m_BLOCK_SIZE = 40;
	
private:
	std::vector<POINT> m_blocks;	//координати чотирьох блоків фігури
	COLORREF m_color;				//колір фігури
	POINT m_position;				//координати фігури
};

