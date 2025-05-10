#include "Figure.h"
#include "Board.h"
#include "Game.h"
#include "Sound.h"

Figure::Figure() :
	m_color(g_GRAY)				//колір фігури
{
	//встановлюємо позицію фігури на ігровому полі ============
	m_position.x = g_board.m_BOARD_WIDTH / 2 - 2;	//+- по центру
	m_position.y = 0;								//верхня частина поля
	//==========================================================
}

const COLORREF Figure::m_COLORS[m_MAX_NUMBER_FIGURES] =
{
	Figure::m_AZURE_I,
	Figure::m_YELLOW_O,
	Figure::m_PURPLE_T,
	Figure::m_GREEN_S,
	Figure::m_RED_Z,
	Figure::m_BLUE_J,
	Figure::m_ORANGE_L
};

COLORREF Figure::getRandomColor()
{
	//генерація випадкового індексу від 0 до 6
	const int RANDOM_INDEX = g_random.getRandomInt(0, m_MAX_NUMBER_FIGURES - 1);

	return m_COLORS[RANDOM_INDEX];
}

const std::vector<POINT>& Figure::getBlocksPosition() const
{
	//повертаємо координати фігури
	return m_blocks;
}

Figure& Figure::setBlocksPosition(const std::vector<POINT>& BLOCKS_POSITION)
{
	if (BLOCKS_POSITION.size() != 4) //якщо фігура не складається з 4 блоків
	{
		//то нічого не робимо
		return *this;
	}

	//присвоюємо нову координату блоків фігури
	m_blocks = BLOCKS_POSITION;

	return *this;
}

POINT Figure::getFigurePosition() const
{
	return m_position;
}

Figure& Figure::setFigurePosition(const POINT& POSITION)
{
	m_position = POSITION;

	return *this;
}

void Figure::moveFigure(const int X, const int Y)
{
	//де опиниться фігура після зміщення
	const POINT NEW_POSITION = { m_position.x + X, m_position.y + Y };

	Figure testFigure = *this; //тестова фігура для перевірки

	//задаємо позицію для тестової фігури
	testFigure.setFigurePosition(NEW_POSITION); //задаємо координати блоків фігури

	//якщо фігура може рухатися далі
	if (!testFigure.checkCollision())
	{
		m_position = NEW_POSITION; //то просто переміщаємо її

		//якщо фігура рухається вниз
		if (Y == 1)
		{
			//то відтворюємо звук переміщення фігури
			Sound::playMoveSound(false, true, Sound::getEffectsVolume());
		}
	}
	else if (Y == 1) //якщо фігура не може рухатися вниз
	{
		freezeFigure(); //то фіксуємо її
	}
}

bool Figure::checkCollision()
{
	//проходимо по всім блокам фігури
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//абсолютні координати блоку (позиція на ігровому полі) ====
		const int ABSOLUTE_X = m_position.x + CURRENT_BLOCK.x; //абсолютна координата по осі X
		const int ABSOLUTE_Y = m_position.y + CURRENT_BLOCK.y; //абсолютна координата по осі Y
		//==========================================================

		if //перевірка, чи виходить блок за межі поля або врізається в інші блоки
		(
			(ABSOLUTE_X < 0 || ABSOLUTE_X >= g_board.m_BOARD_WIDTH)	||
			(ABSOLUTE_Y < 0 || ABSOLUTE_Y >= g_board.m_BOARD_HEIGHT)	||
			(g_board.getFieldBlockColor(ABSOLUTE_X, ABSOLUTE_Y) != g_EMPTY_COLOR)
		)
		{
			return true; //так, фігура врізається
		}
	}

	return false; //ні, фігура не врізається
}

void Figure::rotateFigure()
{
	//якщо квадратная фігура
	if (m_color == m_YELLOW_O)
	{
		//то не обертаємо її
		return;
	}

	//координати блоків вже перевернутої фігури
	std::vector<POINT> rotated;

	//проходимо по всім блокам поточної фігури
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//обертаємо координати блоку на 90 градусів і зберігаємо у вже перевернуту фігуру
		rotated.push_back({ CURRENT_BLOCK.y, -CURRENT_BLOCK.x });
	}

	//тестова фігура для перевірки
	Figure testFigure = *this; 

	//задаємо позицію для тестової фігури
	testFigure.setBlocksPosition(rotated); 

	//1. якщо тестову фігуру можна обернути
	if (!testFigure.checkCollision())
	{
		//2. то просто обертаємо фігуру
		m_blocks = rotated;
		return;
	}
	else //1. якщо тестову фігуру не вдалося обернути
	{
		//варіанти здвигів фігури
		const POINT OFFSETS[] =
		{
			{ 0, 0 },	//без здвигу
			{ -1, 0 },	//вліво  на 1
			{ 1, 0 },	//вправо на 1
			{ -2, 0 },	//вліво  на 2
			{ 2, 0 },	//вправо на 2
			{ -3, 0 },	//вліво  на 3
			{ 3, 0 }	//вправо на 3
		};

		//2. то здвигаємо її вправо/вліво
		for (const auto& CURRENT_OFFSET : OFFSETS)
		{
			//нова позиція фігури, враховуючи здвиг
			POINT newPosition =
			{
				m_position.x + CURRENT_OFFSET.x,
				m_position.y + CURRENT_OFFSET.y
			};

			//задаємо нову позицію фігури
			testFigure.setFigurePosition(newPosition); 

			//3. і знову перевіряємо, чи вдається обернути тестову фігуру
			if (!testFigure.checkCollision())
			{
				//якщо тестову фігуру можна обернути
				//то присвоюємо оригінальній фігурі нову позицію і обертаємо
				m_position	= newPosition;
				m_blocks	= rotated;
				//==========================================================

				return;
			}
		}
	}
	//==========================================================
}

void Figure::rotateFigure(const int TIMES)
{
	for (int i = 0; i < TIMES; ++i)
	{
		//тимчасові координати блоків
		std::vector<POINT> tempBlocks;

		//проходимо по всім блокам фігури
		for (const auto& block : m_blocks)
		{
			//обертаємо координати блоку на 90 градусів і зберігаємо як тимчасові координати
			tempBlocks.push_back({ block.y, -block.x });
		}

		//оновлюємо координати блоків фігури
		m_blocks = tempBlocks;
	}
}

void Figure::freezeFigure()
{
	//проходимо по всім блокам фігури
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//абсолютні координати блоку (позиція на ігровому полі) ====
		const int ABSOLUTE_X = m_position.x + CURRENT_BLOCK.x; //абсолютна координата по осі X
		const int ABSOLUTE_Y = m_position.y + CURRENT_BLOCK.y; //абсолютна координата по осі Y
		//==========================================================

		if //якщо блок не виходить за межі поля
			(
				(ABSOLUTE_X >= 0 && ABSOLUTE_X < g_board.m_BOARD_WIDTH) &&
				(ABSOLUTE_Y >= 0 && ABSOLUTE_Y < g_board.m_BOARD_HEIGHT)
				)
		{
			//то запам'ятовуємо його колір на ігровому полі
			g_board.setFieldBlockColor(ABSOLUTE_X, ABSOLUTE_Y, m_color);
		}

	}

	//очищення заповнених рядків на ігровому полі
	g_board.clearLines();

	//створення нової фігури
	g_game.createFigure();
}

void Figure::quickFall()
{
	//тестова фігура для перевірки
	Figure testFigure = *this;

	//поки фігура може падати вниз, не зустрічаючи перешкод
	while (!testFigure.checkCollision())
	{
		++testFigure.m_position.y; //переміщуємо тестову фігуру вниз
	}

	//присвоюємо нову позицію оригінальній фігурі
	*this = testFigure;

	//відтворення звуку падіння фігури
	Sound::playSpaceSound(false, true, Sound::getEffectsVolume());

	//якщо фігура більше не може рухатися вниз, то фіксуємо її
	freezeFigure();
}

Figure& Figure::setColor(const COLORREF& COLOR)
{
	m_color = COLOR;

	return *this;
}

const COLORREF& Figure::getColor() const
{
	return m_color;
}
