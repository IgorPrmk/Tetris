#pragma once

#include "General.h"

//клас для зберігання даних про гру
class Game
{
public:
	UIState m_currentState;							//поточний стан інтерфейсу
	UIState m_previousState;						//попередній стан інтерфейсу

	Figure m_currentFigure;							//поточна падаюча фігура	
	Figure m_nextFigure;							//наступна фігура, яка з'явиться в грі
	std::vector<Figure> m_figures;					//масив усіх можливих фігур

	bool m_isGameOver;								//чи гра закінчена
	bool m_isEnteringName; 							//чи гравець в даний момент вводить своє ім'я
	bool m_isCursorVisible;							//чи видимий курсор (миготіння під час введення імені)
	bool m_inProgress;								//чи є розпочата сесія гри
	
	bool m_isEndSoundPlayed;						//чи вже програлася музика в кінці гри
	ULONGLONG m_flashStartTime;						//коли блоки фігури почали миготіти випадковими кольорами

	//керування швидкістю гри ==================================
	int m_fallSpeed;	//швидкість падіння фігури
	int m_level;		//рівень гри (залежить від кількості очок) – від 0 до 8
	//==========================================================

	Game() :
		m_currentState(UIState::MENU),	//меню відкрито
		m_previousState(UIState::MENU),	//попередній стан меню
		m_isGameOver(false),			//гра не закінчена
		m_isEnteringName(false), 		//гравець не вводить ім'я
		m_isCursorVisible(false), 		//курсор не миготить
		m_inProgress(false),			//гравець не грає
		m_fallSpeed(500), 				//швидкість гри 500 (початковий рівень)
		m_level(0),						//встановлення початкового рівня
		m_isEndSoundPlayed(false),		//музика в кінці гри ще не програвалася
		m_flashStartTime(0)				//скидання часу початку миготіння блоків фігури
	{
		//створення фігур
		m_figures[FigureType::I].setColor(Figure::m_AZURE_I).setBlocksPosition(	{ { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } });
		m_figures[FigureType::O].setColor(Figure::m_YELLOW_O).setBlocksPosition({ { 0, 0 },{ 1, 0 },{ 0, 1 },{ 1, 1 } });
		m_figures[FigureType::T].setColor(Figure::m_PURPLE_T).setBlocksPosition({ { 0, 0 },{ 1, 0 },{ 2, 0 },{ 1, 1 } });
		m_figures[FigureType::S].setColor(Figure::m_GREEN_S).setBlocksPosition(	{ { 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 } });
		m_figures[FigureType::Z].setColor(Figure::m_RED_Z).setBlocksPosition(	{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 2, 1 } });
		m_figures[FigureType::J].setColor(Figure::m_BLUE_J).setBlocksPosition(	{ { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 2 } });
		m_figures[FigureType::L].setColor(Figure::m_ORANGE_L).setBlocksPosition({ { 1, 0 },{ 1, 1 },{ 1, 2 },{ 0, 2 } });

		//генерація наступної випадкової фігури
		m_nextFigure = getRandomFigure();
	}

	//функція для генерації випадкової фігури
	Figure getRandomFigure()
	{
		//генерація випадкового індексу від 0 до 6
		const int RANDOM_INDEX = g_random->getRandomInt(0, Figure::m_MAX_NUMBER_FIGURES - 1);

		//повернення випадкової фігури з масиву
		return m_figures[RANDOM_INDEX];
	}

	//функція для повного перезапуску гри
	void resetGame()
	{
		//проходимо по всім блокам ігрового поля і заповнюємо їх "порожнім" кольором
		for (int y = 0; y < g_board->m_BOARD_HEIGHT; ++y)
		{
			for (int x = 0; x < g_board->m_BOARD_WIDTH; ++x)
			{
				g_board->setFieldBlockColor(x, y, g_EMPTY_COLOR);
			}
		}

		g_score->clearCurrentRecord();			//очистка поточного рахунку
		g_score->setNewRecord(false);			//гравець не встановив новий рекорд
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

	//функція створює нову фігуру на ігровому полі і перевіряє, чи не настав кінець гри
	void createFigure()
	{
		//беремо заздалегідь згенеровану фігуру
		m_currentFigure = m_nextFigure;

		//генеруємо нову наступну фігуру
		m_nextFigure = getRandomFigure();

		//якщо фігура в щось врізається, то гра закінчується
		if (m_currentFigure.checkCollision())
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

	//функція перевіряє, чи входить рахунок гравця в топ найкращих результатів
	void checkTop9()
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
				g_score->getScoreboard().empty() ||
				g_score->getScoreboard().size() < 9 ||
				g_score->getCurrentScore() > g_score->getScoreboard().back().score
				)
		{
			g_score->setNewRecord(true);				//гравець встановив новий рекорд
			m_isEnteringName = true;					//гравець вводить ім'я
			m_currentState = UIState::ENTER_NAME;	//зміна стану інтерфейсу на введення імені

			//встановка часу початку миготіння блоків фігури
			if (m_flashStartTime == 0)
			{
				m_flashStartTime = GetTickCount64();
			}

			return;
		}

		g_score->setNewRecord(false); //гравець не встановив новий рекорд
	}

	//функція для обробки введенного гравцем імені
	void afterNameInput()
	{
		//додаємо новий рекорд в таблицю рекордів
		g_score->pushNewRecord();

		//сортуємо таблицю рекордів
		g_score->sortScoreboard();

		//зменшуємо таблицю рекордів до 9 записів
		g_score->resizeScoreboard();

		//зберігаємо таблицю рекордів в .txt файл
		g_score->saveScoreboard();

		m_isEnteringName = false;			//гравець більше не вводить ім'я
		m_currentState = UIState::PLAYING;	//зміна стану інтерфейсу на "гравець грає"
		m_flashStartTime = 0;				//скидання часу початку миготіння блоків фігури
		g_score->setNewRecord(false);			//гравець не встановив новий рекорд
	}

	//функція для відтворення звуку закінчення гри (сумна/весела музика)
	void playEndSound()
	{
		if (g_score->isNewRecord()) //якщо був встановлений рекорд, то відкриваємо веселу музику
		{
			Sound::playHappyEndMusic(false, true, Sound::getMusicVolume());
		}
		else //якщо рекорду не було встановлено, то відкриваємо сумну музику
		{
			Sound::playSadEndMusic(false, true, Sound::getMusicVolume());
		}
	}
};

