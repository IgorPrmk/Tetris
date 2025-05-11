#pragma once

#include "Score.h"
#include "Sound.h"
#include "Random.h"

//колір фігур ==============================================
const COLORREF g_AZURE_I = RGB(0, 255, 255);	//блакитна фігура		форма: I
const COLORREF g_YELLOW_O = RGB(255, 255, 0);	//жовта фігура			форма: O
const COLORREF g_PURPLE_T = RGB(139, 0, 255);	//фіолетова фігура		форма: T
const COLORREF g_GREEN_S = RGB(0, 255, 0);		//зелена фігура			форма: S
const COLORREF g_RED_Z = RGB(255, 0, 0);		//червона фігура		форма: Z
const COLORREF g_BLUE_J = RGB(0, 0, 255);		//синя фігура			форма: J
const COLORREF g_ORANGE_L = RGB(255, 128, 0);	//помаранчева фігура	форма: L
//==========================================================

//використовувані кольори ==================================
const COLORREF g_BLACK = RGB(0, 0, 0);			//чорний колір
const COLORREF g_WHITE = RGB(255, 255, 255);	//білий колір
const COLORREF g_GRAY = RGB(50, 50, 50);		//сірий колір
const COLORREF g_YELLOW = RGB(255, 255, 0);		//жовтий колір
const COLORREF g_ORANGE = RGB(255, 128, 0);		//помаранчевий колір

const COLORREF g_EMPTY_COLOR = RGB(0, 0, 0);	//колір пустого блоку
//==========================================================

//розміри ігрового поля (фактичний розмір залежить від розміру одного блоку фігури)
const int g_BOARD_WIDTH = 10;	//ширина ігрового поля
const int g_BOARD_HEIGHT = 20;	//висота ігрового поля
//==========================================================

//розмір одного блоку фігури (40*40 пікселів)
const int g_BLOCK_SIZE = 40;

//розмір панелі праворуч від ігрового поля (200 пікселів)
const int g_SCORE_PANEL_WIDTH = 200;

//ширина вікна = ширина ігрового поля + ширина панелі праворуч від ігрового поля
const int g_WINDOW_WIDTH = g_BLOCK_SIZE * g_BOARD_WIDTH + g_SCORE_PANEL_WIDTH;

//висота вікна = висота ігрового поля
const int g_WINDOW_HEIGHT = g_BLOCK_SIZE * g_BOARD_HEIGHT;

//максимальна кількість фігур в грі (7 фігур: I, O, T, S, Z, J, L)
const int g_MAX_NUMBER_FIGURES = 7;

//id таймерів
enum Timer
{
	DECOR_RAIN = 1, //таймер для декоративного "дощу" з фігур
	GAME_TICK = 2,	//таймер для гри (оновлення стану гри)
};

//набір станів гри з точки зору інтерфейсу
enum class UIState
{
	PLAYING,	//гравець грає
	ENTER_NAME,	//гравець вводить ім'я після рекорду
	MENU,		//гравець відкрив головне меню
	CONTROLS,	//гравець відкрив підпункт головного меню "Controls"
	PAUSED,		//гравець встановив гру на паузу
	SETTINGS	//гравець відкрив підпункт головного меню "Settings"
};

//описує фігуру
struct Figure
{
	std::vector<POINT> blocks;	//координати чотирьох блоків фігури
	COLORREF color = g_GRAY;	//колір фігури
};

//клас для зберігання даних про гру
class Game
{
public:
	//конструктор
	Game();

	//для генерації випадкової фігури
	Figure getRandomFigure();
	
	//для повного перезапуску гри
	void resetGame();
	
	//створює нову фігуру на ігровому полі і перевіряє, чи не настав кінець гри
	void createFigure();
	
	//чи врізається в щось фігура (true – так, врізається)
	bool checkCollision(const Figure& FIGURE, const POINT& POSITION);
	
	//фіксує фігуру на ігровому полі, коли вона більше не може рухатися вниз
	void freezeFigure();
	
	//очищає заповнені рядки на ігровому полі
	void clearLines();
	
	//для обертання фігури на 90 градусів за годинниковою стрілкою
	//використовується для грального процесу
	void rotateFigure();

	//для переміщення фігури по ігровому полю
	void moveFigure(const int X, const int Y);

	//прискорює падіння фігури до самого низу
	void quickFall();

	//перевіряє, чи входить рахунок гравця в топ найкращих результатів
	void checkTop9();
	
	//для обробки введенного гравцем імені
	void afterNameInput();
	
	//для відтворення звуку закінчення гри (сумна/весела музика)
	void playEndSound();
	
	//для отримання швидкості падіння фігури
	int getFallSpeed() const;

	//для отримання поточного стану інтерфейсу
	UIState getCurrentState() const;

	//для отримання попереднього стану інтерфейсу
	UIState getPreviousState() const;

	//встановлює поточний стан інтерфейсу
	void setCurrentState(const UIState STATE);

	//встановлює попередній стан інтерфейсу
	void setPreviousState(const UIState STATE);

	//чи видно курсор
	bool isCursorVisible() const;

	//встановлює видимість курсора
	void setCursorVisible(const bool VISIBLE);

	//чи гра була закінчена
	bool isGameOver() const;

	//чи гравець вводить ім'я
	bool isEnteringName() const;

	//чи вставлено новий рекорд
	bool isNewRecord() const;

	//чи гра в процесі (чи вона триває)
	bool inProgress() const;

	//встановлює стан рекорду
	void setNewRecord(const bool NEW_RECORD);

	//повертає час початку миготіння блоків фігури
	ULONGLONG getFlashStartTime() const;

	//встановлює час початку миготіння блоків фігури
	void resetFlashStartTime();

	//повертає колір блоку ігрового поля
	const COLORREF& getBlockColor(const int X, const int Y) const;

	//повертає координати поточної фігури
	const POINT& getCurrentFigurePosition() const;

	//повертає поточну фігуру
	const Figure& getCurrentFigure() const;

	//повертає наступну фігуру
	const Figure& getNextFigure() const;

	//повертає поточну кількість очок
	int getCurrentScore() const;

	//повертає поточне ім'я гравця
	const std::wstring& getCurrentName() const;

	//повертає таблицю рекордів
	const std::vector<ScoreInfo>& getScoreboard() const;

	//додає новий символ в кінець поточного імені гравця
	void addCharName(const wchar_t CHAR);

	//видаляє символ в кінці поточного імені гравця
	void deleteCharName();

	//повертає відповідну фігуру з масиву всіх фігур
	const Figure& getFigure(const int FIGURE_TYPE) const;

	//повертає координати курсора миші
	const POINT& getMousePosition() const { return m_mousePosition; }

	//встановлює координати курсора миші
	void setMousePosition(const POINT& MOUSE_POSITION) { m_mousePosition = MOUSE_POSITION; }	//встановити координати курсора миші

public:
	Random* m_random;							//об'єкт для генерації випадкових чисел

private:
	UIState m_currentState;						//поточний стан інтерфейсу
	UIState m_previousState;					//попередній стан інтерфейсу
	std::vector<std::vector<COLORREF>> m_board;	//ігрове поле
	Figure m_currentFigure;						//поточна падаюча фігура
	Figure m_nextFigure;						//наступна фігура, яка з'явиться в грі
	POINT m_currentFigurePosition;				//координати поточно падаючої фігури
	std::vector<Figure> m_figures;				//масив усіх можливих фігур
	ULONGLONG m_flashStartTime;					//коли блоки фігури почали миготіти випадковими кольорами
	POINT m_mousePosition;						//зберігає координати курсору { long, long }
	int m_fallSpeed;							//швидкість падіння фігури
	int m_level;								//рівень гри (залежить від кількості очок) – від 0 до 8

	bool m_isEnteringName; 						//чи гравець в даний момент вводить своє ім'я
	bool m_isCursorVisible;						//чи видимий курсор (миготіння під час введення імені)
	bool m_inProgress;							//чи є розпочата сесія гри
	bool m_isEndSoundPlayed;					//чи вже програлася музика в кінці гри
	bool m_isGameOver;							//чи гра закінчена

	Score* m_score;								//об'єкт для роботи з рекордами
};