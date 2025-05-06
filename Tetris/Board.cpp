#include "Board.h"

Board::Board() :
	m_BOARD_WIDTH(10),		//ширина ігрового поля
	m_BOARD_HEIGHT(20)		//висота ігрового поля
{
	m_field.resize
	(
		m_BOARD_HEIGHT, 
		std::vector<COLORREF>(m_BOARD_WIDTH, g_EMPTY_COLOR)
	);
}

const COLORREF& Board::getFieldBlockColor(const int X, const int Y) const
{
	return m_field[Y][X];
}

void Board::setFieldBlockColor(const int X, const int Y, const COLORREF& COLOR)
{
	//встановлюємо колір блоку в полі
	m_field[Y][X] = COLOR;
}

void Board::clearLines()
{
	//кількість очищених рядків
	int linesCleared = 0;

	//прохід по всім рядкам ігрового поля зверху вниз
	for (int y = 0; y < m_BOARD_HEIGHT;)
	{
		//чи рядок повністю заповнений
		bool isFull = true;

		//прохід по всім блокам в рядку
		for (int x = 0; x < m_BOARD_WIDTH; ++x)
		{
			//якщо в рядку є пустий блок
			if (m_field[y][x] == g_EMPTY_COLOR)
			{
				//то рядок не заповнений
				isFull = false;
				break;
			}
		}

		//якщо рядок повністю заповнений
		if (isFull)
		{
			//видаляємо заповнений рядок під номером 'y' з ігрового поля
			m_field.erase(m_field.begin() + y);

			//додаємо новий рядок зверху з пустими блоками
			m_field.insert(m_field.begin(), std::vector<COLORREF>(m_BOARD_WIDTH, g_EMPTY_COLOR));

			//збільшуємо кількість очищених рядків
			++linesCleared;
		}
		else
		{
			//якщо рядок не заповнений, то переходимо до наступного нижнього рядка
			++y;
		}
	}

	//якщо є очищені рядки
	if (linesCleared > 0)
	{
		//додаємо очки за очищені рядки (100 очок за кожен очищений рядок)
		g_score->setCurrentScore(g_score->getCurrentScore() + linesCleared * 100);

		//програвання звуку очищення рядка
		Sound::playLineClearSound(false, true, Sound::getEffectsVolume());

		//новий рівень гри
		int newLevel = g_score->getCurrentScore() / 1000;

		//якщо новий рівень гри більше поточного
		if (newLevel > g_game->m_level)
		{
			//то оновлюємо рівень гри
			g_game->m_level = newLevel;

			//швидкість гри (інтервал між тиками)
			//level = 0 –> 500; level = 1 –> 450; ... ; level = 8 –> 100.
			int newFallSpeed = 500 - g_game->m_level * 50;

			//обмеження швидкості гри (мінімум 100)
			if (newFallSpeed < 100)
			{
				//якщо швидкість гри менше 100, то повертаємо 100
				newFallSpeed = 100;
			}

			//збереження нової швидкості гри
			g_game->m_fallSpeed = newFallSpeed;

			//яке вікно в даний момент активне
			const HWND WINDOW_HANDLE = GetActiveWindow();

			//зупинка попереднього таймера
			KillTimer(WINDOW_HANDLE, Timer::GAME_TICK);

			//встановлення нового таймера (як часто буде викликатися WM_TIMER)
			SetTimer(WINDOW_HANDLE, Timer::GAME_TICK, g_game->m_fallSpeed, 0);
		}
	}
}
