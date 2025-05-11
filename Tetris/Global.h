#pragma once

//для структур: POINT; PAINTSTRUCT; SIZE; WNDCLASSEX
//для типів:	COLORREF; ULONGLONG; HWND; RECT; HBRUSH; HDC; LRESULT; HINSTANCE
//для макроса	RGB
//для функцій:	swprinf_s(); GetTickCount64(); GetActiveWindow(); InvalidateRect();
//				CreateSolidBrush(); CreatePen(); SelectObject(); GetStockObject();
//				FillRect(); Rectangle(); WndProc(); SetTimer(); PtInRect(); BeginPaint();
//				DeleteObject(); GetClientRect(); CreateCompatibleDC(); SetBkMode();
//				GetTextExtentPoint32(); SetTextColor(); TextOutW(); lstrlenW();
//				wcschr(); DefWindowProc(); RegisterClassEx();
#include <windows.h>			
#include <algorithm>				//для функції sort()
#include <fstream>					//для роботи з файлами
#include <random>					//для генерації випадкових чисел
#include <windowsx.h>				//для макросів: GET_X_LPARAM(); GET_Y_LPARAM();

#include "Game.h"

Game* g_game = new Game(); //поточна гра


//функція для обертання фігури на 90 градусів times-разів за годинниковою стрілкою
//використовується для декоративного "дощу" з фігур в меню
void rotateFigure(Figure& figure, const int times)
{
	for (int i = 0; i < times; ++i)
	{
		//тимчасові координати блоків
		std::vector<POINT> tempBlocks;

		//проходимо по всім блокам фігури
		for (const auto& block : figure.blocks)
		{
			//обертаємо координати блоку на 90 градусів і зберігаємо як тимчасові координати
			tempBlocks.push_back({ block.y, -block.x });
		}

		//оновлюємо координати блоків фігури
		figure.blocks = tempBlocks;
	}
}

//чи використовується темна тема оформлення
bool g_isDarkTheme = true;

//декоративні фігури в меню ("дощ" із фігур)
struct DecorFigure
{
	Figure figure;			//тип фігури
	double x = 0.0;			//горизонтальне положення фігури (в пікселях)
	double y = 0.0;			//вертикальне положення фігури (в пікселях)
	double speed = 0.0;		//швидкість падіння фігури (в пікселях за тік)
};

//макс кількість одночасно падаючих декоративних фігур в меню ("дощ" із фігур)
const int g_MAX_DECOR_COUNT = 10;

//масив всіх декоративних фігур в меню ("дощ" із фігур)
std::vector<DecorFigure> g_decorFigures;

//функція для генерації випадкового кольору
COLORREF getRandomColor()
{
	//всі доступні кольори для фігур
	const COLORREF COLORS[g_MAX_NUMBER_FIGURES] = 
	{ 
		g_AZURE_I, 
		g_YELLOW_O, 
		g_PURPLE_T, 
		g_GREEN_S, 
		g_RED_Z, 
		g_BLUE_J, 
		g_ORANGE_L 
	};

	//генерація випадкового індексу від 0 до 6
	const int RANDOM_INDEX = g_game->m_random->getRandomInt(0, g_MAX_NUMBER_FIGURES - 1);

	return COLORS[RANDOM_INDEX];
}

//на якому пункті меню знаходиться курсор
int g_cursorMenuItem = -1;

//на якому пункті налаштувань знаходиться курсор
int g_cursorSettingsItem = -1;

//на якому пункті управління знаходиться курсор
int g_cursorControlsItem = -1;

//функція, яка обровляє введення імені гравцем
void nameInput(const WPARAM WPARAM)
{
	//якщо гра в даний момент не активна
	if (!g_game)
	{
		//тоді нічого не робимо
		return;
	}

	//якщо гравець натиснув клавішу Enter
	if (WPARAM == L'\r')
	{
		//то гравець закінчив введення імені
		g_game->afterNameInput();

		//оновлення вікна
		InvalidateRect(GetActiveWindow(), nullptr, false);

		return;
	}

	//якщо гравець натиснув клавішу Backspace
	if (WPARAM == L'\b')
	{
		//то видалення символу з імені гравця
		g_game->deleteCharName();

		//оновлення вікна
		InvalidateRect(GetActiveWindow(), nullptr, false);

		return;
	}

	//додаємо символ до імені гравця
	g_game->addCharName(WPARAM); //заповнення імені гравця введеним символом

	//оновлення вікна
	InvalidateRect(GetActiveWindow(), nullptr, false);
}

enum class MenuItemID
{
	CONTINUE,
	NEW_GAME,
	CONTROLS,
	SETTINGS,
	EXIT
};

enum class SettingsItemID
{
	LIGHT_THEME,
	DARK_THEME,
	MUSIC_VOLUME,
	EFFECTS_VOLUME,
	BACK
};

enum class ControlsItemID
{
	BACK
};

//пункти меню ==============================================
struct MenuItem								//являє собою один пункт меню
{
	MenuItemID itemID;						//ідентифікатор пункту меню
	std::wstring itemText;					//текст пункту меню
	RECT itemRectangle;						//прямокутник, в якому розташований пункт меню
};
std::vector<MenuItem> g_menuItems;			//список всіх пунктів меню


struct SettingsItem							//являє собою один пункт налаштувань
{
	SettingsItemID itemID;					//ідентифікатор пункту налаштувань
	std::wstring itemText; 					//текст пункту налаштувань
	RECT itemRectangle;						//прямокутник, в якому розташований пункт налаштувань
};
std::vector<SettingsItem> g_settingsItems;	//список всіх пунктів налаштувань


struct ControlsItem							//являє собою один пункт управління
{
	ControlsItemID itemID;					//ідентифікатор пункту управління
	std::wstring itemText; 					//текст пункту управління
	RECT itemRectangle;						//прямокутник, в якому розташований пункт управління
};
std::vector<ControlsItem> g_controlsItems;	//список всіх пунктів управління
//==========================================================

const RECT g_ZERO_POSITION = { 0, 0, 0, 0 }; //нульова позиція

//ініціалізація пунктів меню
void initMenuItems()
{
	//очистка списку пунктів меню
	g_menuItems.clear();

	//додавання пунктів меню ===================================
	//якщо гра активна
	//якщо гравець вводить ім'я (аби не втратити рекорд)
	if (g_game && (g_game->inProgress() || g_game->isGameOver() || g_game->isEnteringName()))
	{
		//то додаємо пункт "Continue"
		g_menuItems.push_back({ MenuItemID::CONTINUE, L"Continue", g_ZERO_POSITION });
	}
	g_menuItems.push_back({ MenuItemID::NEW_GAME, L"New Game", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::CONTROLS, L"Controls", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::SETTINGS, L"Settings", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::EXIT, L"Exit", g_ZERO_POSITION });
	//==========================================================
}

//ініціалізація пунктів налаштувань
void initSettingsItems()
{
	//очистка списку пунктів налаштувань
	g_settingsItems.clear();

	//додавання пунктів налаштувань ============================
	g_settingsItems.push_back({ SettingsItemID::LIGHT_THEME, L"Light theme", g_ZERO_POSITION });
	g_settingsItems.push_back({ SettingsItemID::DARK_THEME, L"Dark theme", g_ZERO_POSITION });

	const int TEMP_SIZE = 30;
	wchar_t temp[TEMP_SIZE]; //вже модифікований рядок тексту (вказана гучність)
	swprintf(temp, TEMP_SIZE, L"Music Volume: %d%%", Sound::getMusicVolume());
	g_settingsItems.push_back({ SettingsItemID::MUSIC_VOLUME, temp, g_ZERO_POSITION });
	swprintf(temp, TEMP_SIZE, L"Effects Volume: %d%%", Sound::getEffectsVolume());
	g_settingsItems.push_back({ SettingsItemID::EFFECTS_VOLUME, temp, g_ZERO_POSITION });

	g_settingsItems.push_back({ SettingsItemID::BACK, L"Back", g_ZERO_POSITION });
	//==========================================================
}

//ініціалізація пунктів управління
void initControlsItems()
{
	//очистка списку пунктів управління
	g_controlsItems.clear();

	//додавання пунктів управління
	g_controlsItems.push_back({ ControlsItemID::BACK, L"Back", g_ZERO_POSITION });
}

//функція, яка обробляє натискання на пункт меню
void clickOnMenuItem(const int INDEX)
{
	if (!g_game) //якщо гра не активна
	{
		//то нічого не робимо
		return;
	}
	else if (INDEX < 0 || INDEX >= g_menuItems.size()) //якщо індекс пункту меню не в межах списку
	{
		//то нічого не робимо
		return;
	}
	else
	{
		//який пункт меню натиснув гравець
		switch (g_menuItems[INDEX].itemID)
		{
		case MenuItemID::CONTINUE:
		{
			//якщо гра до цього була на паузі
			if (g_game->getPreviousState() == UIState::PAUSED)
			{
				//відновлення стану пауза
				g_game->setCurrentState(UIState::PAUSED);
			}
			else //якщо це не пауза
			{
				//просто повернення на попередній екран, де був гравець
				g_game->setCurrentState(g_game->getPreviousState());
			}
		} break;

		case MenuItemID::NEW_GAME:
		{
			//то перезапуск гри
			g_game->resetGame();

		} break;

		case MenuItemID::CONTROLS:
		{
			//зміна стану інтерфейсу
			g_game->setCurrentState(UIState::CONTROLS);

			//ініціалізація пунктів управління
			initControlsItems();

			//скидання місцязнаходження курсора
			g_cursorControlsItem = -1;

		} break;

		case MenuItemID::SETTINGS:
		{
			//зміна стану інтерфейсу
			g_game->setCurrentState(UIState::SETTINGS);

			//ініціалізація пунктів налаштувань
			initSettingsItems();

			//скидання місцязнаходження курсора
			g_cursorSettingsItem = -1;

		} break;

		case MenuItemID::EXIT:
		{
			//вихід із циклу обробки подій вікна
			PostQuitMessage(0);

		} break;

		default: break;
		}

		//оновлення вікна
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

//функція, яка обробляє натискання на пункт налаштувань
void clickOnSettingsItem(const int INDEX)
{
	if (!g_game) //якщо гра не активна
	{
		//то нічого не робимо
		return;
	}
	else if (INDEX < 0 || INDEX >= g_settingsItems.size()) //якщо індекс пункту налаштувань не в межах списку
	{
		//то нічого не робимо
		return;
	}
	else
	{
		//який пункт налаштувань натиснув гравець
		switch (g_settingsItems[INDEX].itemID)
		{
		case SettingsItemID::LIGHT_THEME:
		{
			//то встановлюємо світлу тему (вимикаємо темну тему)
			g_isDarkTheme = false;

		} break;

		case SettingsItemID::DARK_THEME:
		{
			//то встановлюємо темну тему
			g_isDarkTheme = true;

		} break;

		case SettingsItemID::MUSIC_VOLUME:
		case SettingsItemID::EFFECTS_VOLUME:
		{
			return;

		} break;

		case SettingsItemID::BACK:
		{
			//зміна стану інтерфейсу (меню)
			g_game->setCurrentState(UIState::MENU);

			//ініціалізація пунктів меню
			initMenuItems();

		} break;

		default: break;
		}

		//оновлення вікна
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

void clickOnControlsItem(const int INDEX)
{
	if (!g_game) //якщо гра не активна
	{
		//то нічого не робимо
		return;
	}
	else if (INDEX < 0 || INDEX >= g_controlsItems.size()) //якщо індекс пункту управління не в межах списку
	{
		//то нічого не робимо
		return;
	}
	else
	{
		//який пункт управління натиснув гравець
		switch (g_controlsItems[INDEX].itemID)
		{
		case ControlsItemID::BACK:
		{
			//зміна стану інтерфейсу (меню)
			g_game->setCurrentState(UIState::MENU);

			//ініціалізація пунктів меню
			initMenuItems();

			//оновлення вікна
			InvalidateRect(GetActiveWindow(), nullptr, false);

		} break;

		default: break;
		}

		//оновлення вікна
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

//ініціалізація декоративного "дощу" з фігур
void initDecorRain()
{
	//очистка масиву декоративних фігур
	g_decorFigures.clear();

	//кількість декоративних фігур
	g_decorFigures.resize(g_MAX_DECOR_COUNT);

	//одна декоративна фігура
	DecorFigure decoreFigure;

	//заповнення масиву випадковими декоративними фігурами
	for (int i = 0; i < g_MAX_DECOR_COUNT; ++i)
	{
		//випадкова фігура
		decoreFigure.figure = g_game->getRandomFigure();

		//випадкове обертання фігури ===============================
		//генерація випадкового обертання фігури від 0 до 3
		const int RANDOM_ROTATION = g_game->m_random->getRandomInt(0, 3);
		rotateFigure(decoreFigure.figure, RANDOM_ROTATION);
		//==========================================================

		//випадкова позиція вище екрану ============================
		decoreFigure.x = g_game->m_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
		decoreFigure.y = g_game->m_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
		//==========================================================

		//випадкова швидкість фігури
		decoreFigure.speed = g_game->m_random->getRandomDouble(1.5, 2.5);

		//заповнення масиву фігур
		g_decorFigures[i] = decoreFigure;
	}
}

//функція, яка оновлює позицію декоративних фігур
void updateDecorRain()
{
	for (auto& currentDecoreFigure : g_decorFigures)
	{
		//переміщення фігури вниз
		currentDecoreFigure.y += currentDecoreFigure.speed * 10.0;

		//якщо фігура вийшла за межі екрану
		if (currentDecoreFigure.y > g_WINDOW_HEIGHT + 40)
		{
			//випадкова фігура
			currentDecoreFigure.figure = g_game->getRandomFigure();

			//випадкове обертання фігури ===============================
			//генерація випадкового обертання фігури від 0 до 3
			const int RANDOM_ROTATION = g_game->m_random->getRandomInt(0, 3);
			rotateFigure(currentDecoreFigure.figure, RANDOM_ROTATION);
			//==========================================================

			//випадкова позиція вище екрану ============================
			currentDecoreFigure.x = g_game->m_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
			currentDecoreFigure.y = g_game->m_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
			//==========================================================

			//випадкова швидкість фігури
			currentDecoreFigure.speed = g_game->m_random->getRandomDouble(1.5, 2.5);
		}
	}
}

//функція, яка малює декоративний "дощ" з фігур
void drawDecorRain(const HDC DEVICE_CONTEXT)
{
	//розмір блоку декаративної фігури
	const int BLOCK_SIZE = 20;

	for (const auto& CURRENT_DECORE_FIGURE : g_decorFigures) //перебір всіх декоративних фігур
	{
		for (const auto& CURRENT_BLOCK : CURRENT_DECORE_FIGURE.figure.blocks) //перебір всіх блоків фігури
		{
			//абсолютні координати блоку (позиція на ігровому полі) ====
			const int ABSOLUTE_X = CURRENT_DECORE_FIGURE.x + CURRENT_BLOCK.x * BLOCK_SIZE; //абсолютна координата по осі X
			const int ABSOLUTE_Y = CURRENT_DECORE_FIGURE.y + CURRENT_BLOCK.y * BLOCK_SIZE; //абсолютна координата по осі Y
			//==========================================================

			//малювання блоку фігури ===================================
			//створення пензля з кольором блоку
			const HBRUSH BRUSH = CreateSolidBrush(CURRENT_DECORE_FIGURE.figure.color); //яким кольором заповнити блок

			//що заповнити кольором
			const RECT BLOCK_RECTANGLE =
			{
				ABSOLUTE_X,					//координата лівої сторони
				ABSOLUTE_Y,					//координата верхньої сторони
				ABSOLUTE_X + BLOCK_SIZE,	//координата правої сторони
				ABSOLUTE_Y + BLOCK_SIZE		//координата нижньої сторони
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