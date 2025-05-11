#include "Game.h"

Game::Game() :
	m_currentState(UIState::MENU),	//меню відкрито
	m_previousState(UIState::MENU),	//попередній стан меню
	m_isGameOver(false),			//гра не закінчена
	m_isEnteringName(false), 		//гравець не вводить ім'я
	m_isCursorVisible(false), 		//курсор не миготить
	m_inProgress(false),			//гравець не грає
	m_fallSpeed(500), 				//швидкість гри 500 (початковий рівень)
	m_level(0),						//встановлення початкового рівня
	m_isEndSoundPlayed(false),		//музика в кінці гри ще не програвалася
	m_flashStartTime(0),			//скидання часу початку миготіння блоків фігури
	m_score(new Score()),			//створення об'єкта для роботи з рекордами
	m_random(new Random()),			//створення об'єкта для генерації випадкових чисел
	m_mousePosition({ 0, 0 })		//координати курсора миші
{
	//ініціалізація ігрового поля
	m_board.resize(g_BOARD_HEIGHT, std::vector<COLORREF>(g_BOARD_WIDTH, g_EMPTY_COLOR));

	//створення фігур
	m_figures.push_back({ { {0,0},{1,0},{2,0},{3,0} }, g_AZURE_I });	// I
	m_figures.push_back({ { {0,0},{1,0},{0,1},{1,1} }, g_YELLOW_O });	// O
	m_figures.push_back({ { {0,0},{1,0},{2,0},{1,1} }, g_PURPLE_T });	// T
	m_figures.push_back({ { {1,0},{2,0},{0,1},{1,1} }, g_GREEN_S });	// S
	m_figures.push_back({ { {0,0},{1,0},{1,1},{2,1} }, g_RED_Z });		// Z
	m_figures.push_back({ { {0,0},{0,1},{0,2},{1,2} }, g_BLUE_J });		// J
	m_figures.push_back({ { {1,0},{1,1},{1,2},{0,2} }, g_ORANGE_L });	// L

	//генерація наступної випадкової фігури
	m_nextFigure = getRandomFigure();
}

Figure Game::getRandomFigure()
{
	//генерація випадкового індексу від 0 до 6
	const int RANDOM_INDEX = m_random->getRandomInt(0, g_MAX_NUMBER_FIGURES - 1);

	//повернення випадкової фігури з масиву
	return m_figures[RANDOM_INDEX];
}

void Game::resetGame()
{
	//проходимо по всім блокам ігрового поля і заповнюємо їх "порожнім" кольором
	for (int y = 0; y < g_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < g_BOARD_WIDTH; ++x)
		{
			m_board[y][x] = g_EMPTY_COLOR;
		}
	}

	m_score->clearCurrentRecord();			//очистка поточного рахунку
	m_score->setNewRecord(false);			//гравець не встановив новий рекорд
	m_isGameOver = false;			//гра не закінчена
	m_isEnteringName = false;			//гравець не вводить ім'я
	m_currentState = UIState::PLAYING; //зміна стану інтерфейсу: гравець грає
	m_inProgress = true;				//гравець грає
	m_level = 0;				//встановлення початкового рівня
	m_fallSpeed = 500;				//встановлення початкову швидкість гри
	m_isEndSoundPlayed = false;			//музика в кінці гри ще не програвалася
	m_flashStartTime = 0;				//скидання часу початку миготіння блоків фігури
	createFigure();							//встановлення нової фігури на ігрове поле
}

void Game::createFigure()
{
	//беремо заздалегідь згенеровану фігуру
	m_currentFigure = m_nextFigure;

	//генеруємо нову наступну фігуру
	m_nextFigure = getRandomFigure();

	//встановлюємо позицію фігури на ігровому полі ============
	m_currentFigurePosition.x = g_BOARD_WIDTH / 2 - 2;	//+- по центру
	m_currentFigurePosition.y = 0;						//верхня частина поля
	//==========================================================

	//якщо фігура в щось врізається, то гра закінчується
	if (checkCollision(m_currentFigure, m_currentFigurePosition))
	{
		m_isGameOver = true;	//гра закінчується
		m_inProgress = false;	//гра більше не триває

		checkTop9(); //перевіряємо, чи потрапляє рахунок гравця в топ

		//програвання музики в кінці гри
		if (!m_isEndSoundPlayed)
		{
			playEndSound();
			m_isEndSoundPlayed = true;
		}
	}
}

bool Game::checkCollision(const Figure& FIGURE, const POINT& POSITION)
{
	//проходимо по всім блокам фігури
	for (const auto& CURRENT_BLOCK : FIGURE.blocks)
	{
		//абсолютні координати блоку (позиція на ігровому полі) ====
		const int ABSOLUTE_X = POSITION.x + CURRENT_BLOCK.x; //абсолютна координата по осі X
		const int ABSOLUTE_Y = POSITION.y + CURRENT_BLOCK.y; //абсолютна координата по осі Y
		//==========================================================

		if //перевірка, чи виходить блок за межі поля або врізається в інші блоки
			(
				(ABSOLUTE_X < 0 || ABSOLUTE_X >= g_BOARD_WIDTH) ||
				(ABSOLUTE_Y < 0 || ABSOLUTE_Y >= g_BOARD_HEIGHT) ||
				(m_board[ABSOLUTE_Y][ABSOLUTE_X] != g_EMPTY_COLOR)
				)
		{
			return true; //так, фігура врізається
		}
	}

	return false; //ні, фігура не врізається
}

void Game::freezeFigure()
{
	//проходимо по всім блокам фігури
	for (const auto& CURRENT_BLOCK : m_currentFigure.blocks)
	{
		//абсолютні координати блоку (позиція на ігровому полі) ====
		const int ABSOLUTE_X = m_currentFigurePosition.x + CURRENT_BLOCK.x; //абсолютна координата по осі X
		const int ABSOLUTE_Y = m_currentFigurePosition.y + CURRENT_BLOCK.y; //абсолютна координата по осі Y
		//==========================================================

		if //якщо блок не виходить за межі поля
			(
				(ABSOLUTE_X >= 0 && ABSOLUTE_X < g_BOARD_WIDTH) &&
				(ABSOLUTE_Y >= 0 && ABSOLUTE_Y < g_BOARD_HEIGHT)
				)
		{
			//то запам'ятовуємо його колір на ігровому полі
			m_board[ABSOLUTE_Y][ABSOLUTE_X] = m_currentFigure.color;
		}

	}

	//очищення заповнених рядків на ігровому полі
	clearLines();

	//створення нової фігури
	createFigure();
}

void Game::clearLines()
{
	//кількість очищених рядків
	int linesCleared = 0;

	//прохід по всім рядкам ігрового поля зверху вниз
	for (int y = 0; y < g_BOARD_HEIGHT;)
	{
		//чи рядок повністю заповнений
		bool isFull = true;

		//прохід по всім блокам в рядку
		for (int x = 0; x < g_BOARD_WIDTH; ++x)
		{
			//якщо в рядку є пустий блок
			if (m_board[y][x] == g_EMPTY_COLOR)
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
			m_board.erase(m_board.begin() + y);

			//додаємо новий рядок зверху з пустими блоками
			m_board.insert(m_board.begin(), std::vector<COLORREF>(g_BOARD_WIDTH, g_EMPTY_COLOR));

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
		m_score->setCurrentScore(m_score->getCurrentScore() + linesCleared * 100);

		//програвання звуку очищення рядка
		Sound::playLineClearSound(false, true, Sound::getEffectsVolume());

		//новий рівень гри
		int newLevel = m_score->getCurrentScore() / 1000;

		//якщо новий рівень гри більше поточного
		if (newLevel > m_level)
		{
			//то оновлюємо рівень гри
			m_level = newLevel;

			//швидкість гри (інтервал між тиками)
			//level = 0 –> 500; level = 1 –> 450; ... ; level = 8 –> 100.
			int newFallSpeed = 500 - m_level * 50;

			//обмеження швидкості гри (мінімум 100)
			if (newFallSpeed < 100)
			{
				//якщо швидкість гри менше 100, то повертаємо 100
				newFallSpeed = 100;
			}

			//збереження нової швидкості гри
			m_fallSpeed = newFallSpeed;

			//яке вікно в даний момент активне
			const HWND WINDOW_HANDLE = GetActiveWindow();

			//зупинка попереднього таймера
			KillTimer(WINDOW_HANDLE, Timer::GAME_TICK);

			//встановлення нового таймера (як часто буде викликатися WM_TIMER)
			SetTimer(WINDOW_HANDLE, Timer::GAME_TICK, m_fallSpeed, 0);
		}
	}
}

void Game::rotateFigure()
{
	//якщо квадратная фігура
	if (m_currentFigure.color == g_YELLOW_O)
	{
		//то не обертаємо її
		return;
	}

	//вже перевернута фігура
	std::vector<POINT> rotated;

	//проходимо по всім блокам поточної фігури
	for (const auto& CURRENT_BLOCK : m_currentFigure.blocks)
	{
		//обертаємо координати блоку на 90 градусів і зберігаємо у вже перевернуту фігуру
		rotated.push_back({ CURRENT_BLOCK.y, -CURRENT_BLOCK.x });
	}

	//чи вдається обернути фігуру ==============================
	const Figure TEST = { rotated, m_currentFigure.color }; //фігура для перевірки

	//1. якщо фігуру можна обернути
	if (!checkCollision(TEST, m_currentFigurePosition))
	{
		//2. то просто обертаємо фігуру
		m_currentFigure.blocks = rotated;
		return;
	}
	else //1. якщо фігуру не вдалося обернути
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
				m_currentFigurePosition.x + CURRENT_OFFSET.x,
				m_currentFigurePosition.y + CURRENT_OFFSET.y
			};

			//3. і знову перевіряємо, чи вдається обернути фігуру
			if (!checkCollision(TEST, newPosition))
			{
				//присвоюємо нову позицію і обертаємо ======================
				m_currentFigurePosition = newPosition;
				m_currentFigure.blocks = rotated;
				//==========================================================

				return;
			}
		}
	}
	//==========================================================
}

void Game::moveFigure(const int X, const int Y)
{
	//де опиниться фігура після зміщення
	const POINT NEW_POSITION = { m_currentFigurePosition.x + X, m_currentFigurePosition.y + Y };

	//якщо фігура може рухатися далі
	if (!checkCollision(m_currentFigure, NEW_POSITION))
	{
		m_currentFigurePosition = NEW_POSITION; //то просто переміщаємо її

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

void Game::quickFall()
{
	//поки фігура може падати вниз, не зустрічаючи перешкод
	while (!checkCollision(m_currentFigure, { m_currentFigurePosition.x, m_currentFigurePosition.y + 1 }))
	{
		//відбувається збільшення координати Y
		++m_currentFigurePosition.y;
	}

	//відтворення звуку падіння фігури
	Sound::playSpaceSound(false, true, Sound::getEffectsVolume());

	//якщо фігура більше не може рухатися вниз, то фіксуємо її
	freezeFigure();
}

void Game::checkTop9()
{
	//якщо гравець вже вводить ім'я, то нічого не робимо
	if (m_isEnteringName)
	{
		return;
	}

	//умови, за яких поточний рахунок гідний потрапити в топ:
	//1.таблиця рекордів порожня
	//2.таблиця рекордів містить менше 9 записів
	//3.поточний рахунок більше останнього рекорду
	if
		(
			m_score->getScoreboard().empty() ||
			m_score->getScoreboard().size() < 9 ||
			m_score->getCurrentScore() > m_score->getScoreboard().back().score
			)
	{
		m_score->setNewRecord(true);				//гравець встановив новий рекорд
		m_isEnteringName = true;					//гравець вводить ім'я
		m_currentState = UIState::ENTER_NAME;	//зміна стану інтерфейсу на введення імені

		//встановка часу початку миготіння блоків фігури
		if (m_flashStartTime == 0)
		{
			m_flashStartTime = GetTickCount64();
		}

		return;
	}

	m_score->setNewRecord(false); //гравець не встановив новий рекорд
}

void Game::afterNameInput()
{
	//додаємо новий рекорд в таблицю рекордів
	m_score->pushNewRecord();

	//сортуємо таблицю рекордів
	m_score->sortScoreboard();

	//зменшуємо таблицю рекордів до 9 записів
	m_score->resizeScoreboard();

	//зберігаємо таблицю рекордів в .txt файл
	m_score->saveScoreboard();

	m_isEnteringName = false;			//гравець більше не вводить ім'я
	m_currentState = UIState::PLAYING;	//зміна стану інтерфейсу на "гравець грає"
	m_flashStartTime = 0;				//скидання часу початку миготіння блоків фігури
	m_score->setNewRecord(false);			//гравець не встановив новий рекорд
}

void Game::playEndSound()
{
	if (m_score->isNewRecord()) //якщо був встановлений рекорд, то відкриваємо веселу музику
	{
		Sound::playHappyEndMusic(false, true, Sound::getMusicVolume());
	}
	else //якщо рекорду не було встановлено, то відкриваємо сумну музику
	{
		Sound::playSadEndMusic(false, true, Sound::getMusicVolume());
	}
}

int Game::getFallSpeed() const
{
	return m_fallSpeed;
}

UIState Game::getCurrentState() const
{
	return m_currentState;
}

UIState Game::getPreviousState() const
{
	return m_previousState;
}

void Game::setCurrentState(const UIState STATE)
{
	m_currentState = STATE;
}

void Game::setPreviousState(const UIState STATE)
{
	m_previousState = STATE;
}

bool Game::isCursorVisible() const
{
	return m_isCursorVisible;
}

void Game::setCursorVisible(const bool VISIBLE)
{
	m_isCursorVisible = VISIBLE;
}

bool Game::isGameOver() const
{
	return m_isGameOver;
}

bool Game::isEnteringName() const
{
	return m_isEnteringName;
}

bool Game::isNewRecord() const
{
	return m_score->isNewRecord();
}

bool Game::inProgress() const
{
	return m_inProgress;
}

void Game::setNewRecord(const bool NEW_RECORD)
{
	m_score->setNewRecord(NEW_RECORD);
}

ULONGLONG Game::getFlashStartTime() const
{
	return m_flashStartTime;
}

void Game::resetFlashStartTime()
{
	m_flashStartTime = 0;
}

const COLORREF& Game::getBlockColor(const int X, const int Y) const
{
	return m_board[X][Y];
}

const POINT& Game::getCurrentFigurePosition() const
{
	return m_currentFigurePosition;
}

const Figure& Game::getCurrentFigure() const
{
	return m_currentFigure;
}

const Figure& Game::getNextFigure() const
{
	return m_nextFigure;
}

int Game::getCurrentScore() const
{
	return m_score->getCurrentScore();
}

const std::wstring& Game::getCurrentName() const
{
	return m_score->getCurrentName();
}

const std::vector<ScoreInfo>& Game::getScoreboard() const
{
	return m_score->getScoreboard();
}

void Game::addCharName(const wchar_t CHAR)
{
	m_score->addCharName(CHAR);
}

void Game::deleteCharName()
{
	m_score->deleteCharName();
}

const Figure& Game::getFigure(const int FIGURE_TYPE) const
{
	return m_figures[FIGURE_TYPE];
}
