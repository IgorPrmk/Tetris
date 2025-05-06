#include "General.h"
#include "Game.h"
#include "Random.h"
#include "Board.h"
#include "Score.h"
#include "Sound.h"
#include "Decor.h"
#include "Figure.h"

Game* g_game		= nullptr;	//об'єкт для роботи з грою
Random* g_random	= nullptr;	//об'єкт для генерації випадкових чисел
Board* g_board		= nullptr;	//об'єкт для роботи з ігровим полем
Score* g_score		= nullptr;	//об'єкт для роботи з рекордами
Decor* g_decor		= nullptr;	//об'єкт для роботи з декоративним "дощем" з фігур

//використовувані кольори ==================================
const COLORREF g_BLACK			= RGB(0, 0, 0);			//чорний колір
const COLORREF g_WHITE			= RGB(255, 255, 255);	//білий колір
const COLORREF g_GRAY			= RGB(50, 50, 50);		//сірий колір
const COLORREF g_YELLOW			= RGB(255, 255, 0);		//жовтий колір
const COLORREF g_ORANGE			= RGB(255, 128, 0);		//помаранчевий колір

const COLORREF g_EMPTY_COLOR	= RGB(0, 0, 0);			//колір пустого блоку
//==========================================================

//розмір панелі праворуч від ігрового поля (200 пікселів)
const int g_SCORE_PANEL_WIDTH = 200;

const int g_WINDOW_WIDTH = Figure::m_BLOCK_SIZE * g_board->m_BOARD_WIDTH + g_SCORE_PANEL_WIDTH;

const int g_WINDOW_HEIGHT = Figure::m_BLOCK_SIZE * g_board->m_BOARD_HEIGHT;

std::vector<Decor> g_decorFigures(g_decor->m_MAX_DECOR_COUNT);