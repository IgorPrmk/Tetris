#include "Decor.h"
#include "Random.h" 	//для генерації випадкових чисел
#include "Game.h"		//для роботи з грою

Decor::Decor()
{
	initDecorRain();
}

Decor& Decor::operator=(const Decor& other)
{
	if (this != &other) // Проверка на самоприсваивание
	{
		m_figure = other.m_figure; // Копируем объект Figure
		m_x = other.m_x;           // Копируем координату X
		m_y = other.m_y;           // Копируем координату Y
		m_speed = other.m_speed;   // Копируем скорость
	}

	return *this;
}

void Decor::initDecorRain()
{
	//очистка масиву декоративних фігур
	g_decorFigures.clear();

	//кількість декоративних фігур
	g_decorFigures.resize(m_MAX_DECOR_COUNT);

	//одна декоративна фігура
	Decor decore;

	//заповнення масиву випадковими декоративними фігурами
	for (int i = 0; i < m_MAX_DECOR_COUNT; ++i)
	{
		//випадкова фігура
		decore.m_figure = g_game->getRandomFigure();

		//випадкове обертання фігури ===============================
		//генерація випадкового обертання фігури від 0 до 3
		const int RANDOM_ROTATION = g_random->getRandomInt(0, 3);
		decore.m_figure.rotateFigure(RANDOM_ROTATION);
		//==========================================================

		//випадкова позиція вище екрану ============================
		decore.m_x = g_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
		decore.m_y = g_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
		//==========================================================

		//випадкова швидкість фігури
		decore.m_speed = g_random->getRandomDouble(1.5, 2.5);

		//заповнення масиву фігур
		g_decorFigures[i] = decore;
	}
}

void Decor::updateDecorRain()
{
	for (auto& currentDecoreFigure : g_decorFigures)
	{
		//переміщення фігури вниз
		currentDecoreFigure.m_y += currentDecoreFigure.m_speed * 10.0;

		//якщо фігура вийшла за межі екрану
		if (currentDecoreFigure.m_y > g_WINDOW_HEIGHT + 40)
		{
			//випадкова фігура
			currentDecoreFigure.m_figure = g_game->getRandomFigure();

			//випадкове обертання фігури ===============================
			//генерація випадкового обертання фігури від 0 до 3
			const int RANDOM_ROTATION = g_random->getRandomInt(0, 3);
			currentDecoreFigure.m_figure.rotateFigure(RANDOM_ROTATION);
			//==========================================================

			//випадкова позиція вище екрану ============================
			currentDecoreFigure.m_x = g_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
			currentDecoreFigure.m_y = g_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
			//==========================================================

			//випадкова швидкість фігури
			currentDecoreFigure.m_speed = g_random->getRandomDouble(1.5, 2.5);
		}
	}
}

void Decor::drawDecorRain(const HDC DEVICE_CONTEXT)
{
	//розмір блоку декаративної фігури
	const int BLOCK_SIZE = 20;

	for (const auto& CURRENT_DECORE_FIGURE : g_decorFigures) //перебір всіх декоративних фігур
	{
		for (const auto& CURRENT_BLOCK : CURRENT_DECORE_FIGURE.m_figure.getBlocksPosition()) //перебір всіх блоків фігури
		{
			//абсолютні координати блоку (позиція на ігровому полі) ====
			const int ABSOLUTE_X = CURRENT_DECORE_FIGURE.m_x + CURRENT_BLOCK.x * BLOCK_SIZE; //абсолютна координата по осі X
			const int ABSOLUTE_Y = CURRENT_DECORE_FIGURE.m_y + CURRENT_BLOCK.y * BLOCK_SIZE; //абсолютна координата по осі Y
			//==========================================================

			//малювання блоку фігури ===================================
			//створення пензля з кольором блоку
			const HBRUSH BRUSH = CreateSolidBrush(CURRENT_DECORE_FIGURE.m_figure.getColor()); //яким кольором заповнити блок

			//що заповнити кольором
			const RECT BLOCK_RECTANGLE =
			{
				ABSOLUTE_X,				//координата лівої сторони
				ABSOLUTE_Y,				//координата верхньої сторони
				ABSOLUTE_X + BLOCK_SIZE,	//координата правої сторони
				ABSOLUTE_Y + BLOCK_SIZE	//координата нижньої сторони
			};

			FillRect(DEVICE_CONTEXT, &BLOCK_RECTANGLE, BRUSH); //заповнення прямокутника кольором
			DeleteObject(BRUSH); //видалення пензля (вивільнення пам'яті)
			//==========================================================

			//малювання контуру блоку ==================================

			//створення контуру (ручки) : тип, товщина і колір контуру
			const HPEN PEN = CreatePen(PS_SOLID, 1, g_GRAY);

			//вибір контуру
			SelectObject(DEVICE_CONTEXT, PEN);

			//вибір заливки (пензля) : прозорий (без заливки)
			SelectObject(DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

			//малювання контуру блоку
			Rectangle
			(
				DEVICE_CONTEXT,
				ABSOLUTE_X,
				ABSOLUTE_Y,
				ABSOLUTE_X + BLOCK_SIZE,
				ABSOLUTE_Y + BLOCK_SIZE
			);

			//видалення контуру
			DeleteObject(PEN);
			//==========================================================
		}
	}
}