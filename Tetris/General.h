#pragma once

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include "Common.h"


extern Game* g_game;		//об'єкт для роботи з грою
extern Random* g_random;	//об'єкт для генерації випадкових чисел
extern Board* g_board;		//об'єкт для роботи з ігровим полем
extern Score* g_score;		//об'єкт для роботи з рекордами
extern Decor* g_decor;		//об'єкт для роботи з декоративним "дощем" з фігур


//використовувані кольори ==================================
extern const COLORREF g_BLACK;			//чорний колір
extern const COLORREF g_WHITE;			//білий колір
extern const COLORREF g_GRAY;			//сірий колір
extern const COLORREF g_YELLOW;			//жовтий колір
extern const COLORREF g_ORANGE;			//помаранчевий колір

extern const COLORREF g_EMPTY_COLOR;	//колір пустого блоку
//==========================================================

//розмір панелі праворуч від ігрового поля (200 пікселів)
extern const int g_SCORE_PANEL_WIDTH;

//ширина вікна = ширина ігрового поля + ширина панелі праворуч від ігрового поля
extern const int g_WINDOW_WIDTH;

//висота вікна = висота ігрового поля
extern const int g_WINDOW_HEIGHT;

//масив всіх декоративних фігур в меню ("дощ" із фігур)
extern std::vector<Decor> g_decorFigures;