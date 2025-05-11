#pragma once

//��� ��������: POINT; PAINTSTRUCT; SIZE; WNDCLASSEX
//��� ����:	COLORREF; ULONGLONG; HWND; RECT; HBRUSH; HDC; LRESULT; HINSTANCE
//��� �������	RGB
//��� �������:	swprinf_s(); GetTickCount64(); GetActiveWindow(); InvalidateRect();
//				CreateSolidBrush(); CreatePen(); SelectObject(); GetStockObject();
//				FillRect(); Rectangle(); WndProc(); SetTimer(); PtInRect(); BeginPaint();
//				DeleteObject(); GetClientRect(); CreateCompatibleDC(); SetBkMode();
//				GetTextExtentPoint32(); SetTextColor(); TextOutW(); lstrlenW();
//				wcschr(); DefWindowProc(); RegisterClassEx();
#include <windows.h>			
#include <algorithm>				//��� ������� sort()
#include <fstream>					//��� ������ � �������
#include <random>					//��� ��������� ���������� �����
#include <windowsx.h>				//��� �������: GET_X_LPARAM(); GET_Y_LPARAM();

#include "Game.h"

Game* g_game = new Game(); //������� ���


//������� ��� ��������� ������ �� 90 ������� times-���� �� ������������ �������
//��������������� ��� ������������� "����" � ����� � ����
void rotateFigure(Figure& figure, const int times)
{
	for (int i = 0; i < times; ++i)
	{
		//�������� ���������� �����
		std::vector<POINT> tempBlocks;

		//��������� �� ��� ������ ������
		for (const auto& block : figure.blocks)
		{
			//�������� ���������� ����� �� 90 ������� � �������� �� �������� ����������
			tempBlocks.push_back({ block.y, -block.x });
		}

		//��������� ���������� ����� ������
		figure.blocks = tempBlocks;
	}
}

//�� ��������������� ����� ���� ����������
bool g_isDarkTheme = true;

//���������� ������ � ���� ("���" �� �����)
struct DecorFigure
{
	Figure figure;			//��� ������
	double x = 0.0;			//������������� ��������� ������ (� �������)
	double y = 0.0;			//����������� ��������� ������ (� �������)
	double speed = 0.0;		//�������� ������ ������ (� ������� �� ��)
};

//���� ������� ��������� �������� ������������ ����� � ���� ("���" �� �����)
const int g_MAX_DECOR_COUNT = 10;

//����� ��� ������������ ����� � ���� ("���" �� �����)
std::vector<DecorFigure> g_decorFigures;

//������� ��� ��������� ����������� �������
COLORREF getRandomColor()
{
	//�� ������� ������� ��� �����
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

	//��������� ����������� ������� �� 0 �� 6
	const int RANDOM_INDEX = g_game->m_random->getRandomInt(0, g_MAX_NUMBER_FIGURES - 1);

	return COLORS[RANDOM_INDEX];
}

//�� ����� ����� ���� ����������� ������
int g_cursorMenuItem = -1;

//�� ����� ����� ����������� ����������� ������
int g_cursorSettingsItem = -1;

//�� ����� ����� ��������� ����������� ������
int g_cursorControlsItem = -1;

//�������, ��� �������� �������� ���� �������
void nameInput(const WPARAM WPARAM)
{
	//���� ��� � ����� ������ �� �������
	if (!g_game)
	{
		//��� ����� �� ������
		return;
	}

	//���� ������� �������� ������ Enter
	if (WPARAM == L'\r')
	{
		//�� ������� ������� �������� ����
		g_game->afterNameInput();

		//��������� ����
		InvalidateRect(GetActiveWindow(), nullptr, false);

		return;
	}

	//���� ������� �������� ������ Backspace
	if (WPARAM == L'\b')
	{
		//�� ��������� ������� � ���� ������
		g_game->deleteCharName();

		//��������� ����
		InvalidateRect(GetActiveWindow(), nullptr, false);

		return;
	}

	//������ ������ �� ���� ������
	g_game->addCharName(WPARAM); //���������� ���� ������ �������� ��������

	//��������� ����
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

//������ ���� ==============================================
struct MenuItem								//����� ����� ���� ����� ����
{
	MenuItemID itemID;						//������������� ������ ����
	std::wstring itemText;					//����� ������ ����
	RECT itemRectangle;						//�����������, � ����� ������������ ����� ����
};
std::vector<MenuItem> g_menuItems;			//������ ��� ������ ����


struct SettingsItem							//����� ����� ���� ����� �����������
{
	SettingsItemID itemID;					//������������� ������ �����������
	std::wstring itemText; 					//����� ������ �����������
	RECT itemRectangle;						//�����������, � ����� ������������ ����� �����������
};
std::vector<SettingsItem> g_settingsItems;	//������ ��� ������ �����������


struct ControlsItem							//����� ����� ���� ����� ���������
{
	ControlsItemID itemID;					//������������� ������ ���������
	std::wstring itemText; 					//����� ������ ���������
	RECT itemRectangle;						//�����������, � ����� ������������ ����� ���������
};
std::vector<ControlsItem> g_controlsItems;	//������ ��� ������ ���������
//==========================================================

const RECT g_ZERO_POSITION = { 0, 0, 0, 0 }; //������� �������

//����������� ������ ����
void initMenuItems()
{
	//������� ������ ������ ����
	g_menuItems.clear();

	//��������� ������ ���� ===================================
	//���� ��� �������
	//���� ������� ������� ��'� (��� �� �������� ������)
	if (g_game && (g_game->inProgress() || g_game->isGameOver() || g_game->isEnteringName()))
	{
		//�� ������ ����� "Continue"
		g_menuItems.push_back({ MenuItemID::CONTINUE, L"Continue", g_ZERO_POSITION });
	}
	g_menuItems.push_back({ MenuItemID::NEW_GAME, L"New Game", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::CONTROLS, L"Controls", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::SETTINGS, L"Settings", g_ZERO_POSITION });
	g_menuItems.push_back({ MenuItemID::EXIT, L"Exit", g_ZERO_POSITION });
	//==========================================================
}

//����������� ������ �����������
void initSettingsItems()
{
	//������� ������ ������ �����������
	g_settingsItems.clear();

	//��������� ������ ����������� ============================
	g_settingsItems.push_back({ SettingsItemID::LIGHT_THEME, L"Light theme", g_ZERO_POSITION });
	g_settingsItems.push_back({ SettingsItemID::DARK_THEME, L"Dark theme", g_ZERO_POSITION });

	const int TEMP_SIZE = 30;
	wchar_t temp[TEMP_SIZE]; //��� ������������� ����� ������ (������� �������)
	swprintf(temp, TEMP_SIZE, L"Music Volume: %d%%", Sound::getMusicVolume());
	g_settingsItems.push_back({ SettingsItemID::MUSIC_VOLUME, temp, g_ZERO_POSITION });
	swprintf(temp, TEMP_SIZE, L"Effects Volume: %d%%", Sound::getEffectsVolume());
	g_settingsItems.push_back({ SettingsItemID::EFFECTS_VOLUME, temp, g_ZERO_POSITION });

	g_settingsItems.push_back({ SettingsItemID::BACK, L"Back", g_ZERO_POSITION });
	//==========================================================
}

//����������� ������ ���������
void initControlsItems()
{
	//������� ������ ������ ���������
	g_controlsItems.clear();

	//��������� ������ ���������
	g_controlsItems.push_back({ ControlsItemID::BACK, L"Back", g_ZERO_POSITION });
}

//�������, ��� �������� ���������� �� ����� ����
void clickOnMenuItem(const int INDEX)
{
	if (!g_game) //���� ��� �� �������
	{
		//�� ����� �� ������
		return;
	}
	else if (INDEX < 0 || INDEX >= g_menuItems.size()) //���� ������ ������ ���� �� � ����� ������
	{
		//�� ����� �� ������
		return;
	}
	else
	{
		//���� ����� ���� �������� �������
		switch (g_menuItems[INDEX].itemID)
		{
		case MenuItemID::CONTINUE:
		{
			//���� ��� �� ����� ���� �� ����
			if (g_game->getPreviousState() == UIState::PAUSED)
			{
				//���������� ����� �����
				g_game->setCurrentState(UIState::PAUSED);
			}
			else //���� �� �� �����
			{
				//������ ���������� �� ��������� �����, �� ��� �������
				g_game->setCurrentState(g_game->getPreviousState());
			}
		} break;

		case MenuItemID::NEW_GAME:
		{
			//�� ���������� ���
			g_game->resetGame();

		} break;

		case MenuItemID::CONTROLS:
		{
			//���� ����� ����������
			g_game->setCurrentState(UIState::CONTROLS);

			//����������� ������ ���������
			initControlsItems();

			//�������� ��������������� �������
			g_cursorControlsItem = -1;

		} break;

		case MenuItemID::SETTINGS:
		{
			//���� ����� ����������
			g_game->setCurrentState(UIState::SETTINGS);

			//����������� ������ �����������
			initSettingsItems();

			//�������� ��������������� �������
			g_cursorSettingsItem = -1;

		} break;

		case MenuItemID::EXIT:
		{
			//����� �� ����� ������� ���� ����
			PostQuitMessage(0);

		} break;

		default: break;
		}

		//��������� ����
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

//�������, ��� �������� ���������� �� ����� �����������
void clickOnSettingsItem(const int INDEX)
{
	if (!g_game) //���� ��� �� �������
	{
		//�� ����� �� ������
		return;
	}
	else if (INDEX < 0 || INDEX >= g_settingsItems.size()) //���� ������ ������ ����������� �� � ����� ������
	{
		//�� ����� �� ������
		return;
	}
	else
	{
		//���� ����� ����������� �������� �������
		switch (g_settingsItems[INDEX].itemID)
		{
		case SettingsItemID::LIGHT_THEME:
		{
			//�� ������������ ����� ���� (�������� ����� ����)
			g_isDarkTheme = false;

		} break;

		case SettingsItemID::DARK_THEME:
		{
			//�� ������������ ����� ����
			g_isDarkTheme = true;

		} break;

		case SettingsItemID::MUSIC_VOLUME:
		case SettingsItemID::EFFECTS_VOLUME:
		{
			return;

		} break;

		case SettingsItemID::BACK:
		{
			//���� ����� ���������� (����)
			g_game->setCurrentState(UIState::MENU);

			//����������� ������ ����
			initMenuItems();

		} break;

		default: break;
		}

		//��������� ����
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

void clickOnControlsItem(const int INDEX)
{
	if (!g_game) //���� ��� �� �������
	{
		//�� ����� �� ������
		return;
	}
	else if (INDEX < 0 || INDEX >= g_controlsItems.size()) //���� ������ ������ ��������� �� � ����� ������
	{
		//�� ����� �� ������
		return;
	}
	else
	{
		//���� ����� ��������� �������� �������
		switch (g_controlsItems[INDEX].itemID)
		{
		case ControlsItemID::BACK:
		{
			//���� ����� ���������� (����)
			g_game->setCurrentState(UIState::MENU);

			//����������� ������ ����
			initMenuItems();

			//��������� ����
			InvalidateRect(GetActiveWindow(), nullptr, false);

		} break;

		default: break;
		}

		//��������� ����
		InvalidateRect(GetActiveWindow(), nullptr, false);
	}
}

//����������� ������������� "����" � �����
void initDecorRain()
{
	//������� ������ ������������ �����
	g_decorFigures.clear();

	//������� ������������ �����
	g_decorFigures.resize(g_MAX_DECOR_COUNT);

	//���� ����������� ������
	DecorFigure decoreFigure;

	//���������� ������ ����������� ������������� ��������
	for (int i = 0; i < g_MAX_DECOR_COUNT; ++i)
	{
		//��������� ������
		decoreFigure.figure = g_game->getRandomFigure();

		//��������� ��������� ������ ===============================
		//��������� ����������� ��������� ������ �� 0 �� 3
		const int RANDOM_ROTATION = g_game->m_random->getRandomInt(0, 3);
		rotateFigure(decoreFigure.figure, RANDOM_ROTATION);
		//==========================================================

		//��������� ������� ���� ������ ============================
		decoreFigure.x = g_game->m_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
		decoreFigure.y = g_game->m_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
		//==========================================================

		//��������� �������� ������
		decoreFigure.speed = g_game->m_random->getRandomDouble(1.5, 2.5);

		//���������� ������ �����
		g_decorFigures[i] = decoreFigure;
	}
}

//�������, ��� ������� ������� ������������ �����
void updateDecorRain()
{
	for (auto& currentDecoreFigure : g_decorFigures)
	{
		//���������� ������ ����
		currentDecoreFigure.y += currentDecoreFigure.speed * 10.0;

		//���� ������ ������ �� ��� ������
		if (currentDecoreFigure.y > g_WINDOW_HEIGHT + 40)
		{
			//��������� ������
			currentDecoreFigure.figure = g_game->getRandomFigure();

			//��������� ��������� ������ ===============================
			//��������� ����������� ��������� ������ �� 0 �� 3
			const int RANDOM_ROTATION = g_game->m_random->getRandomInt(0, 3);
			rotateFigure(currentDecoreFigure.figure, RANDOM_ROTATION);
			//==========================================================

			//��������� ������� ���� ������ ============================
			currentDecoreFigure.x = g_game->m_random->getRandomDouble(0.0, g_WINDOW_WIDTH);
			currentDecoreFigure.y = g_game->m_random->getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
			//==========================================================

			//��������� �������� ������
			currentDecoreFigure.speed = g_game->m_random->getRandomDouble(1.5, 2.5);
		}
	}
}

//�������, ��� ����� ������������ "���" � �����
void drawDecorRain(const HDC DEVICE_CONTEXT)
{
	//����� ����� ����������� ������
	const int BLOCK_SIZE = 20;

	for (const auto& CURRENT_DECORE_FIGURE : g_decorFigures) //������ ��� ������������ �����
	{
		for (const auto& CURRENT_BLOCK : CURRENT_DECORE_FIGURE.figure.blocks) //������ ��� ����� ������
		{
			//�������� ���������� ����� (������� �� �������� ���) ====
			const int ABSOLUTE_X = CURRENT_DECORE_FIGURE.x + CURRENT_BLOCK.x * BLOCK_SIZE; //��������� ���������� �� �� X
			const int ABSOLUTE_Y = CURRENT_DECORE_FIGURE.y + CURRENT_BLOCK.y * BLOCK_SIZE; //��������� ���������� �� �� Y
			//==========================================================

			//��������� ����� ������ ===================================
			//��������� ������ � �������� �����
			const HBRUSH BRUSH = CreateSolidBrush(CURRENT_DECORE_FIGURE.figure.color); //���� �������� ��������� ����

			//�� ��������� ��������
			const RECT BLOCK_RECTANGLE =
			{
				ABSOLUTE_X,					//���������� ��� �������
				ABSOLUTE_Y,					//���������� ������� �������
				ABSOLUTE_X + BLOCK_SIZE,	//���������� ����� �������
				ABSOLUTE_Y + BLOCK_SIZE		//���������� ������ �������
			};

			FillRect(DEVICE_CONTEXT, &BLOCK_RECTANGLE, BRUSH); //���������� ������������ ��������
			DeleteObject(BRUSH); //��������� ������ (���������� ���'��)
			//==========================================================

			//��������� ������� ����� ==================================

			//��������� ������� (�����) : ���, ������� � ���� �������
			const HPEN PEN = CreatePen(PS_SOLID, 1, g_GRAY);

			//���� �������
			SelectObject(DEVICE_CONTEXT, PEN);

			//���� ������� (������) : �������� (��� �������)
			SelectObject(DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

			//��������� ������� �����
			Rectangle
			(
				DEVICE_CONTEXT,
				ABSOLUTE_X,
				ABSOLUTE_Y,
				ABSOLUTE_X + BLOCK_SIZE,
				ABSOLUTE_Y + BLOCK_SIZE
			);

			//��������� �������
			DeleteObject(PEN);
			//==========================================================
		}
	}
}