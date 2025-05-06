#pragma once

#include <windows.h>	//для роботи з вікнами
#include <windowsx.h>	//для макросів: GET_X_LPARAM; GET_Y_LPARAM;
#include <fstream>		//для роботи з файлами	
#include <algorithm>	//для функції sort
#include <vector>

#include "Game.h" 	//для роботи з даними про гру
#include "Random.h"	//для генерації випадкових чисел
#include "Score.h"	//для роботи з рекордами
#include "Board.h"	//для роботи з ігровим полем
#include "Figure.h"	//для роботи з фігурами
#include "Sound.h"	//для роботи зі звуками
#include "Decor.h"	//для роботи з декоративним "дощем" з фігур


extern Game* g_game = new Game();	//об'єкт для роботи з грою
extern Random* g_random = new Random();	//об'єкт для генерації випадкових чисел
extern Board* g_board = new Board();	//об'єкт для роботи з ігровим полем
extern Score* g_score = new Score();	//об'єкт для роботи з рекордами
extern Decor* g_decor = new Decor();	//об'єкт для роботи з декоративним "дощем" з фігур


//використовувані кольори ==================================
extern const COLORREF g_BLACK;			//чорний колір
extern const COLORREF g_WHITE;			//білий колір
extern const COLORREF g_GRAY;			//сірий колір
extern const COLORREF g_YELLOW;			//жовтий колір
extern const COLORREF g_ORANGE;			//помаранчевий колір

extern const COLORREF g_EMPTY_COLOR;	//колір пустого блоку
//==========================================================

//id таймерів
enum Timer
{
	DECOR_RAIN	= 1, //таймер для декоративного "дощу" з фігур
	GAME_TICK	= 2, //таймер для гри (оновлення стану гри)
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

//розмір панелі праворуч від ігрового поля (200 пікселів)
extern const int g_SCORE_PANEL_WIDTH;

//ширина вікна = ширина ігрового поля + ширина панелі праворуч від ігрового поля
extern const int g_WINDOW_WIDTH;

//висота вікна = висота ігрового поля
extern const int g_WINDOW_HEIGHT;

//масив всіх декоративних фігур в меню ("дощ" із фігур)
extern std::vector<Decor> g_decorFigures;