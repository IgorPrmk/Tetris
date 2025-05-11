/*****************************************
НТУУ «КПІ ім. ІГОРЯ СІКОРСЬКОГО» 
НН ІНСТИТУТ ПРИКЛАДНОГО СИСТЕМНОГО АНАЛІЗУ 
КАФЕДРА ММСА

Тема: Розробка гри Тетріс
Дата: 2025 рік

Приймак Ігор Володимирович
Студент групи КА-43
*****************************************/

#include "Global.h" //для глобальних змінних
#include "Game.h"	//для роботи з ігровим процесом

//функція, яка обробляє події: рух миші, натискання клавіш, ...
//вікно; код події; додаткова інфомація
LRESULT CALLBACK WindowEvents(const HWND WINDOW_HANDLE, const UINT MESSAGE, const WPARAM WPARAM, const LPARAM LPARAM)
{
	switch (MESSAGE)
	{
	case WM_CREATE: //якщо це перше створення вікна
	{
		//ініціалізація пунктів меню, налаштувань і управління =====
		initMenuItems();
		initSettingsItems();
		initControlsItems();
		//==========================================================

		//ініціалізація декоративного "дощу" з фігур
		initDecorRain();

		//ініціалізація таймерів (як часто буде викликатися WM_TIMER)
		SetTimer(WINDOW_HANDLE, Timer::DECOR_RAIN, 50, 0);
		SetTimer(WINDOW_HANDLE, Timer::GAME_TICK, g_game->getFallSpeed(), 0);
		//==========================================================

		//відкриття всіх звуків гри ================================
		Sound::openMenuMusic();
		Sound::openHappyEndMusic();
		Sound::openSadEndMusic();
		Sound::openMoveSound();
		Sound::openLineClearSound();
		Sound::openSpaceSound();
		//==========================================================

		//програвання музики в меню
		Sound::playMenuMusic(true, true, Sound::getMusicVolume());

	} break; //WM_CREATE

	case WM_TIMER: //якщо був викликаний таймер
	{
		//якщо гра не активна
		if (!g_game)
		{
			//то нічого не робимо
			break;
		}

		//ідедентифікатор таймера
		const int TIMER_ID = WPARAM;

		//якщо йде мова про декоративний "дощ"
		if (TIMER_ID == Timer::DECOR_RAIN)
		{
			//якщо гравець знаходиться в головному меню/налаштуваннях/управлінні
			if
				(
					g_game->getCurrentState() == UIState::MENU ||
					g_game->getCurrentState() == UIState::SETTINGS ||
					g_game->getCurrentState() == UIState::CONTROLS
					)
			{
				//то оновлення позиції декоративних фігур
				updateDecorRain();

				//і оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);

				//і програвання музики в меню
				Sound::playMenuMusic(true, false, Sound::getMusicVolume());
			}
			//якщо гравець не знаходиться в головному меню/налаштуваннях/управлінні
			else
			{
				Sound::stopMenuMusic();
			}
		}
		//якщо йде мова про гру
		else if (TIMER_ID == Timer::GAME_TICK)
		{
			//якщо гравець вводить ім'я
			if (g_game->getCurrentState() == UIState::ENTER_NAME)
			{
				//то миготіння курсора
				g_game->setCursorVisible(!g_game->isCursorVisible());

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}
			//якщо гравець грає
			else if
				(
					g_game->getCurrentState() == UIState::PLAYING &&
					!g_game->isGameOver() &&
					!g_game->isEnteringName()
					)
			{
				//то переміщення фігури вниз
				g_game->moveFigure(0, 1);

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}
		}

	} break; //WM_TIMER

	case WM_MOUSEMOVE: //якщо курсор миші в вікні гри перемістився
	{
		//якщо гра не активна
		if (!g_game)
		{
			//то нічого не робимо
			break;
		}

		//отримуємо координати курсора =============================
		g_game->setMousePosition({ GET_X_LPARAM(LPARAM), GET_Y_LPARAM(LPARAM) }); //координати курсора миші
		//==========================================================

		switch (g_game->getCurrentState())
		{
			//якщо гравець в головному меню
		case UIState::MENU:
		{
			//на який пункт меню навів курсор миші гравець (-1 не навів на пункт меню)
			int newCursorMenuItem = -1;

			//перебір всіх пунктів меню
			for (int i = 0; i < g_menuItems.size(); ++i)
			{
				//якщо гравець навів курсор миші на пункт меню
				if (PtInRect(&g_menuItems[i].itemRectangle, { g_game->getMousePosition().x, g_game->getMousePosition().y}))
				{
					//то запам'ятовуємо номер пункту меню
					newCursorMenuItem = i;

					//вихід з циклу
					break;
				}
			}

			//якщо курсор миші змістився на інший пункт меню або на пусте місце
			if (newCursorMenuItem != g_cursorMenuItem)
			{
				//то запам'ятовуємо новий стан курсора
				g_cursorMenuItem = newCursorMenuItem;

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}

		} break; //UIState::MENU

		//якщо гравець в налаштуваннях
		case UIState::SETTINGS:
		{
			//на який пункт меню навів курсор миші гравець (-1 не навів на пункт меню)
			int newCursorSettingsItem = -1;

			//перебір всіх пунктів налаштувань
			for (int i = 0; i < g_settingsItems.size(); ++i)
			{
				//якщо гравець навів курсор миші на пункт налаштувань
				if (PtInRect(&g_settingsItems[i].itemRectangle, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то запам'ятовуємо номер пункту налаштувань
					newCursorSettingsItem = i;

					//вихід з циклу
					break;
				}
			}

			//якщо курсор миші змістився на інший пункт налаштувань або на пусте місце
			if (newCursorSettingsItem != g_cursorSettingsItem)
			{
				//то запам'ятовуємо новий стан курсора
				g_cursorSettingsItem = newCursorSettingsItem;

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}

		} break; //UIState::SETTINGS

		//якщо гравець в управлінні
		case UIState::CONTROLS:
		{
			//на який пункт меню навів курсор миші гравець (-1 не навів на пункт меню)
			int newCursorControlsItem = -1;

			//перебір всіх пунктів управління
			for (int i = 0; i < g_controlsItems.size(); ++i)
			{
				//якщо гравець навів курсор миші на пункт управління
				if (PtInRect(&g_controlsItems[i].itemRectangle, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то запам'ятовуємо номер пункту налаштувань
					newCursorControlsItem = i;

					//вихід з циклу
					break;
				}
			}

			//якщо курсор миші змістився на інший пункт управління або на пусте місце
			if (newCursorControlsItem != g_cursorControlsItem)
			{
				//то запам'ятовуємо новий стан курсора
				g_cursorControlsItem = newCursorControlsItem;

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}

		} break; //UIState::CONTROLS

		default: break;

		}

	} break; //WM_MOUSEMOVE

	case WM_LBUTTONDOWN: //якщо гравець натиснув ліву кнопку миші
	{
		//якщо гра не активна
		if (!g_game)
		{
			//то нічого не робимо
			break;
		}

		//можливість перетягування вікна ===========================
		ReleaseCapture(); //передача керування миші системі

		//переміщення вікна : вікно, системна команда, переміщення вікна, координати курсора (ігнорується)
		SendMessage(WINDOW_HANDLE, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		//==========================================================

		switch (g_game->getCurrentState())
		{
			//якщо гравець в головному меню
		case UIState::MENU:
		{
			//натискаємо на пункт меню, де знаходиться курсор
			clickOnMenuItem(g_cursorMenuItem);

		} break; //UIState::MENU

		//якщо гравець в налаштуваннях
		case UIState::SETTINGS:
		{
			//якщо курсор знаходиться на гучності музики
			if (g_cursorSettingsItem == (int)SettingsItemID::MUSIC_VOLUME)
			{
				//координати прямокутника пункту налаштувань (разом із стрілками)
				const RECT ITEM_RECTANGLE = g_settingsItems[(int)SettingsItemID::MUSIC_VOLUME].itemRectangle;

				//координати прямокутника лівої стрілки
				const RECT LEFT_ARROW_RECTANGLE =
				{
					ITEM_RECTANGLE.left,
					ITEM_RECTANGLE.top,
					ITEM_RECTANGLE.left + ITEM_RECTANGLE.right / 20,
					ITEM_RECTANGLE.bottom
				};

				//координати прямокутника правої стрілки
				const RECT RIGHT_ARROW_RECTANGLE =
				{
					ITEM_RECTANGLE.right - ITEM_RECTANGLE.right / 20,
					ITEM_RECTANGLE.top,
					ITEM_RECTANGLE.right,
					ITEM_RECTANGLE.bottom
				};

				//якщо курсор знаходиться на стрілці зменшення гучності
				if (PtInRect(&LEFT_ARROW_RECTANGLE, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то зменшуємо гучність музики
					Sound::setMusicVolume(Sound::getMusicVolume() - 10);
				}
				//якщо курсор знаходиться на стрілці збільшення гучності
				else if (PtInRect(&RIGHT_ARROW_RECTANGLE, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то збільшуємо гучність музики
					Sound::setMusicVolume(Sound::getMusicVolume() + 10);
				}

				//оновлення тексту пункту налаштувань ======================
				wchar_t temp[30];
				swprintf(temp, 30, L"Music Volume: %d%%", Sound::getMusicVolume());
				g_settingsItems[(int)SettingsItemID::MUSIC_VOLUME].itemText = temp;
				//==========================================================

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}
			//якщо курсор знаходиться на гучності ефектів
			else if (g_cursorSettingsItem == (int)SettingsItemID::EFFECTS_VOLUME)
			{
				//координати прямокутника пункту налаштувань (разом із стрілками)
				const RECT ITEM_RECTANGLE = g_settingsItems[(int)SettingsItemID::EFFECTS_VOLUME].itemRectangle;

				//координати прямокутника лівої стрілки
				const RECT LEFT_ARROW_RECTANGLE =
				{
					ITEM_RECTANGLE.left,
					ITEM_RECTANGLE.top,
					ITEM_RECTANGLE.left + ITEM_RECTANGLE.right / 20,
					ITEM_RECTANGLE.bottom
				};

				//координати прямокутника правої стрілки
				const RECT RIGHT_ARROW_RECTANGLE =
				{
					ITEM_RECTANGLE.right - ITEM_RECTANGLE.right / 20,
					ITEM_RECTANGLE.top,
					ITEM_RECTANGLE.right,
					ITEM_RECTANGLE.bottom
				};

				//якщо курсор знаходиться на стрілці зменшення гучності
				if (PtInRect(&LEFT_ARROW_RECTANGLE, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то зменшуємо гучність музики
					Sound::setEffectsVolume(Sound::getEffectsVolume() - 10);
				}
				//якщо курсор знаходиться на стрілці збільшення гучності
				else if (PtInRect(&RIGHT_ARROW_RECTANGLE, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то збільшуємо гучність музики
					Sound::setEffectsVolume(Sound::getEffectsVolume() + 10);
				}

				//оновлення тексту пункту налаштувань ======================
				wchar_t temp[30];
				swprintf(temp, 30, L"Effects Volume: %d%%", Sound::getEffectsVolume());
				g_settingsItems[(int)SettingsItemID::EFFECTS_VOLUME].itemText = temp;
				//==========================================================

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}
			else //якщо гравець не натиснув на стрілки гучності
			{
				//то натискаємо на цей пункт налаштувань
				clickOnSettingsItem(g_cursorSettingsItem);
			}

		} break; //UIState::SETTINGS

		//якщо гравець в управлінні
		case UIState::CONTROLS:
		{
			//прохід по всіх пунктах управління
			for (int i = 0; i < g_controlsItems.size(); ++i)
			{
				//якщо гравець навів курсор миші на пункт управління
				if (PtInRect(&g_controlsItems[i].itemRectangle, { g_game->getMousePosition().x, g_game->getMousePosition().y }))
				{
					//то натискаємо на цей пункт управління
					clickOnControlsItem(i);

					//вихід з циклу
					break;
				}
			}

		} break; //UIState::CONTROLS

		default: break;

		}

	} break; //WM_LBUTTONDOWN

	case WM_KEYDOWN: //якщо гравець натиснув клавішу на клавіатурі
	{
		//якщо гра не активна
		if (!g_game)
		{
			//то нічого не робимо
			break;
		}

		//якщо гравець натиснув клавішу ESCAPE
		if (WPARAM == VK_ESCAPE)
		{
			//якщо гравець не в меню/управлінні/налаштуваннях
			if (
				!(
					g_game->getCurrentState() == UIState::MENU ||
					g_game->getCurrentState() == UIState::CONTROLS ||
					g_game->getCurrentState() == UIState::SETTINGS
					)
				)
			{
				//то переходимо в меню
				g_game->setPreviousState(g_game->getCurrentState());
				g_game->setCurrentState(UIState::MENU);

				//ініціалізація пунктів меню
				initMenuItems();

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);

				break;
			}
		}

		switch (g_game->getCurrentState())
		{
			//якщо гравець грає
		case UIState::PLAYING:
		{
			//якщо гравець вводить ім'я
			if (g_game->isEnteringName())
			{
				//то нічого не робимо
				break;
			}

			//якщо гра не закінчена
			if (!g_game->isGameOver())
			{
				switch (WPARAM)
				{
				case VK_LEFT: //якщо гравець натиснув клавішу "вліво"
				{
					//то фігура переміщується вліво
					g_game->moveFigure(-1, 0);

				} break;

				case VK_RIGHT: //якщо гравець натиснув клавішу "вправо"
				{
					//то фігура переміщується вправо
					g_game->moveFigure(1, 0);

				} break;

				case VK_DOWN: //якщо гравець натиснув клавішу "вниз"
				{
					//то фігура переміщується вниз
					g_game->moveFigure(0, 1);

				} break;

				case VK_UP: //якщо гравець натиснув клавішу "вверх"
				{
					//то фігура обертається
					g_game->rotateFigure();

				} break;

				case VK_SPACE: //якщо гравець натиснув клавішу "SPACE"
				{
					//то виконується швидке падіння фігури
					g_game->quickFall();

				} break;

				case 'P': case 'p': //якщо гравець натиснув клавішу "P/p"
				{
					//то гра ставиться на паузу
					g_game->setCurrentState(UIState::PAUSED);

				} break;

				case 'Q': case 'q': //якщо гравець натиснув клавішу "Q/q"
				{
					//вихід із циклу обробки подій вікна
					PostQuitMessage(0);

				} break;

				}

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}

		} break; //UIState::PLAYING

		//якщо гравець вводить ім'я
		case UIState::ENTER_NAME:
		{
			//якщо гравець натиснув клавішу "ESCAPE"
			if (WPARAM == VK_ESCAPE)
			{
				//то перехід в меню
				g_game->setPreviousState(UIState::ENTER_NAME);
				g_game->setCurrentState(UIState::MENU);

				//ініціалізація пунктів меню
				initMenuItems();

				//оновлення вікна
				InvalidateRect(WINDOW_HANDLE, nullptr, false);
			}
		} break;

		//якщо гра на паузі
		case UIState::PAUSED:
		{
			switch (WPARAM)
			{
			case 'P': case 'p': //якщо гравець натиснув клавішу "P/p"
			{
				//перехід до гри
				g_game->setCurrentState(UIState::PLAYING);

			} break;

			case 'Q': case 'q': //якщо гравець натиснув клавішу "Q/q"
			{
				//вихід із циклу обробки подій вікна
				PostQuitMessage(0);

			} break;

			}

			//оновлення вікна
			InvalidateRect(GetActiveWindow(), nullptr, false);

		} break;

		default: break;

		}

	} break; //WM_KEYDOWN

	case WM_CHAR: //якщо гравець вводить символ з клавіатури
	{
		//якщо гра активна і гравець вводить ім'я
		if (g_game && g_game->getCurrentState() == UIState::ENTER_NAME)
		{
			//то обробка введеного символу
			nameInput(WPARAM);
		}

	} break; //WM_CHAR

	case WM_PAINT:
	{
		PAINTSTRUCT paintInfo; //структура для отримання інформації про малювання

		//отримуємо контекст малювання + повідомляє систему про початок малювання
		const HDC DEVICE_CONTEXT = BeginPaint(WINDOW_HANDLE, &paintInfo);

		//розмір вікна для малювання
		RECT drawingWindow;

		//отримання розміру вікна для малювання
		GetClientRect(WINDOW_HANDLE, &drawingWindow);

		//отримання ширини і висоти вікна для малювання ============
		const int DRAWING_WINDOW_WIDTH = drawingWindow.right - drawingWindow.left; //ширина вікна для малювання
		const int DRAWING_WINDOW_HEIGHT = drawingWindow.bottom - drawingWindow.top; //висота вікна для малювання
		//==========================================================

		//для подвійної буферизації (малюємо в пам'яті, а потім переносимо на екран)

		//створюємо контекст малювання в пам'яті
		const HDC MEMORY_DEVICE_CONTEXT = CreateCompatibleDC(DEVICE_CONTEXT);

		//де зберігається те, що було намальовано в пам'яті
		const HBITMAP MEMORY_BITMAP = CreateCompatibleBitmap(DEVICE_CONTEXT, DRAWING_WINDOW_WIDTH, DRAWING_WINDOW_HEIGHT);

		//прив'язка контексту малювання до битмапа
		SelectObject(MEMORY_DEVICE_CONTEXT, MEMORY_BITMAP);

		//==========================================================

		//колір тексту
		COLORREF textColor;

		//колір фону
		HBRUSH backgroundColor;

		if (g_isDarkTheme) //якщо гравець вибрав темну тему
		{
			//то фон чорний, а текст білий
			backgroundColor = CreateSolidBrush(g_BLACK);
			textColor = g_WHITE;
		}
		else //якщо гравець вибрав світлу тему
		{
			//то фон білий, а текст чорний
			backgroundColor = CreateSolidBrush(g_WHITE);
			textColor = g_BLACK;
		}

		//заповнення фону вікна кольором
		FillRect(MEMORY_DEVICE_CONTEXT, &drawingWindow, backgroundColor);

		//видалення пензля
		DeleteObject(backgroundColor);

		//шрифт маленького тексту
		const HFONT TEXT_FONT = CreateFontW
		(
			25,								//висота шрифта
			0,								//ширина шрифта
			0,								//кут повороту шрифта
			0, 								//направлення шрифта
			FW_BOLD,						//жирність шрифта
			0,								//чи курсив
			0, 								//чи підкреслений
			0,								//чи закреслений
			DEFAULT_CHARSET,				//кодування (за замовчування)
			OUT_DEFAULT_PRECIS,				//точність (за замовчування)
			CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
			DEFAULT_QUALITY,				//якість (за замовчування)
			DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
			L"Arial"						//назва шрифта
		);

		//вибір маленького шрифта
		SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

		//шрифт тексту для заголовка
		const HFONT TITLE_FONT = CreateFontW
		(
			80,								//висота шрифта
			0,								//ширина шрифта
			0,								//кут повороту шрифта
			0,								//направлення шрифта
			FW_BOLD,						//жирність шрифта
			0,								//чи курсив
			0,								//чи підкреслений
			0,								//чи закреслений
			DEFAULT_CHARSET,				//кодування (за замовчування)
			OUT_DEFAULT_PRECIS,				//точність (за замовчування)
			CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
			DEFAULT_QUALITY,				//якість (за замовчування)
			DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
			L"Arial"						//назва шрифта
		);

		//встановлення кольору фону тексту
		SetBkMode(MEMORY_DEVICE_CONTEXT, TRANSPARENT);

		//встановлення кольору тексту
		SetTextColor(MEMORY_DEVICE_CONTEXT, textColor);

		//якщо гравець в меню/управлінні/налаштуваннях
		if
			(
				g_game->getCurrentState() == UIState::MENU ||
				g_game->getCurrentState() == UIState::SETTINGS ||
				g_game->getCurrentState() == UIState::CONTROLS
				)
		{
			//то малюємо "дощ" з декоративних фігур
			drawDecorRain(MEMORY_DEVICE_CONTEXT);
		}

		switch (g_game->getCurrentState())
		{
		case UIState::MENU: //якщо гравець в меню
		{
			//вибір шрифта для заголовків
			SelectObject(MEMORY_DEVICE_CONTEXT, TITLE_FONT);

			//текст заголовока меню
			const wchar_t* MENU_TITLE = L"MENU";

			//ширина та висота тексту заголовка
			SIZE titleSize;

			//отримання розміру тексту заголовка в пікселях
			GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, MENU_TITLE, lstrlenW(MENU_TITLE), &titleSize);

			//звідки починати малювати заголовок, аби він був по центру по горизонталі
			const int TITLE_X = (drawingWindow.right - titleSize.cx) / 2; //положення тексту заголовка по осі X

			//положення тексту заголовка по осі Y
			const int TITLE_Y = 100;

			//малювання тексту заголовка
			TextOutW(MEMORY_DEVICE_CONTEXT, TITLE_X, TITLE_Y, MENU_TITLE, lstrlenW(MENU_TITLE));

			//вибір маленького шрифта
			SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

			//видалення шрифта для тексту заголовка
			DeleteObject(TITLE_FONT);

			//кількість пунктів меню
			const int MENU_ITEMS_NUMBER = g_menuItems.size();

			//проміжок між пунктами меню по вертикалі
			const int VERTICAL_ITEMS_SPACE = 30;

			//для зберігання розмірів кожного пункта меню
			std::vector<SIZE> itemSizes(MENU_ITEMS_NUMBER);

			//загальна висота всіх пунктів меню і відступів між ними
			int totalItemsHeight = 0;

			//прохід по кожному пункту меню
			for (int i = 0; i < MENU_ITEMS_NUMBER; ++i)
			{
				//отримання та збереження розміру тексту кожного пункту меню
				SIZE currentItemSize;
				GetTextExtentPoint32W
				(
					MEMORY_DEVICE_CONTEXT,
					g_menuItems[i].itemText.c_str(),
					g_menuItems[i].itemText.size(),
					&currentItemSize
				);
				itemSizes[i] = currentItemSize;
				//==========================================================

				//висота тексту пункту меню + відступ після
				totalItemsHeight += (currentItemSize.cy + VERTICAL_ITEMS_SPACE);
			}

			//якщо є хоча б один пункт меню
			if (MENU_ITEMS_NUMBER > 0)
			{
				//то від загальної висоти віднімається останній відступ (не потрібен)
				totalItemsHeight -= VERTICAL_ITEMS_SPACE;
			}

			//допустима область для малювання пунктів меню =============
			const int AVAILABLE_AREA_TOP = 160;											//обмеження зверху
			const int AVAILABLE_AREA_BOTTOM = drawingWindow.bottom;							//обмеження знизу
			const int AVAILABLE_AREA_HEIGHT = AVAILABLE_AREA_BOTTOM - AVAILABLE_AREA_TOP;	//висота області
			//==========================================================

			//місце де починається малювання пунктів меню по вертикалі
			int menuItem_Y = AVAILABLE_AREA_TOP + (AVAILABLE_AREA_HEIGHT - totalItemsHeight) / 2; //положення тексту по осі Y

			//прохід по кожному пункту меню
			for (int i = 0; i < MENU_ITEMS_NUMBER; ++i)
			{
				//текст поточного пункту меню
				const std::wstring& CURRENT_ITEM_TEXT = g_menuItems[i].itemText;

				//розмір поточного пункту меню
				const SIZE CURRENT_ITEM_SIZE = itemSizes[i];

				//звідки починати малювати текст пункту, аби він був по центру по горизонталі
				const int MENU_ITEM_X = (drawingWindow.right - CURRENT_ITEM_SIZE.cx) / 2; //положення тексту по осі X

				//якщо гравець навів курсор миші на пункт меню
				if (i == g_cursorMenuItem)
				{
					//то малюємо його іншим кольором
					SetTextColor(MEMORY_DEVICE_CONTEXT, g_isDarkTheme ? g_YELLOW : g_ORANGE);
				}
				else
				{
					//інакше малюємо звичайним кольором
					SetTextColor(MEMORY_DEVICE_CONTEXT, textColor);
				}

				//оновлення положення пункту меню
				g_menuItems[i].itemRectangle =
				{
					MENU_ITEM_X,						//ліворуч
					menuItem_Y,							//верх
					MENU_ITEM_X + CURRENT_ITEM_SIZE.cx, //праворуч
					menuItem_Y + CURRENT_ITEM_SIZE.cy	//низ
				};

				//малювання тексту пункту меню
				TextOutW(MEMORY_DEVICE_CONTEXT, MENU_ITEM_X, menuItem_Y, CURRENT_ITEM_TEXT.c_str(), CURRENT_ITEM_TEXT.size());

				//звус координати наступного пункту меню на (висоту тексту + відступ)
				menuItem_Y += (CURRENT_ITEM_SIZE.cy + VERTICAL_ITEMS_SPACE);
			}

		} break; //UIState::MENU

		case UIState::SETTINGS: //якщо гравець в налаштуваннях
		{
			//текст заголовока меню
			const wchar_t* SETTINGS_TITLE = L"SETTINGS";

			//вибір шрифта для заголовків
			SelectObject(MEMORY_DEVICE_CONTEXT, TITLE_FONT);

			//ширина та висота тексту заголовка
			SIZE titleSize;

			//отримання розміру тексту заголовка в пікселях
			GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, SETTINGS_TITLE, lstrlenW(SETTINGS_TITLE), &titleSize);

			//звідки починати малювати заголовок, аби він був по центру по горизонталі
			const int TITLE_X = (drawingWindow.right - titleSize.cx) / 2; //положення тексту заголовка по осі X

			//положення тексту заголовка по осі Y
			const int TITLE_Y = 100;

			//малювання тексту заголовка
			TextOutW(MEMORY_DEVICE_CONTEXT, TITLE_X, TITLE_Y, SETTINGS_TITLE, lstrlenW(SETTINGS_TITLE));

			//вибір маленького шрифта
			SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

			//видалення шрифта для тексту заголовка
			DeleteObject(TITLE_FONT);

			//кількість пунктів налаштувань
			const int SETTINGS_ITEMS_NUMBER = g_settingsItems.size();

			//проміжок між пунктами налаштувань по вертикалі
			const int VERTICAL_ITEMS_SPACE = 30;

			//для зберігання розмірів кожного пункта меню
			std::vector<SIZE> itemSizes(SETTINGS_ITEMS_NUMBER);

			//загальна висота всіх пунктів налаштувань і відступів між ними
			int totalItemsHeight = 0;

			//прохід по кожному пункту налаштувань
			for (int i = 0; i < SETTINGS_ITEMS_NUMBER; ++i)
			{
				SIZE currentItemSize; //розмір поточного пункту налаштувань

				//якщо пункт налаштувань - гучність музики або гучність ефектів
				if (i == (int)SettingsItemID::MUSIC_VOLUME || i == (int)SettingsItemID::EFFECTS_VOLUME)
				{
					//для модифікованого тексту пункту налаштувань
					wchar_t volumeText[30];

					//копіювання модифікованого тексту пункту налаштувань
					swprintf
					(
						volumeText,
						30,
						i == (int)SettingsItemID::MUSIC_VOLUME ? L"Music Volume: %d%%" : L"Effects Volume: %d%%",
						i == (int)SettingsItemID::MUSIC_VOLUME ? Sound::getMusicVolume() : Sound::getEffectsVolume()
					);

					//отримання розміру тексту поточного пункту налаштувань
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, volumeText, lstrlenW(volumeText), &currentItemSize);
				}
				else //якщо пункт налаштувань – не гучність
				{
					//отримання розміру тексту поточного пункту налаштувань
					GetTextExtentPoint32W
					(
						MEMORY_DEVICE_CONTEXT,
						g_settingsItems[i].itemText.c_str(),
						g_settingsItems[i].itemText.size(),
						&currentItemSize
					);
				}

				//збереження розміру тексту поточного пункту налаштувань
				itemSizes[i] = currentItemSize;

				//висота тексту пункту налаштувань + відступ після
				totalItemsHeight += currentItemSize.cy + VERTICAL_ITEMS_SPACE;
			}

			//якщо є хоча б один пункт налаштувань
			if (SETTINGS_ITEMS_NUMBER > 0)
			{
				//то від загальної висоти віднімається останній відступ (не потрібен)
				totalItemsHeight -= VERTICAL_ITEMS_SPACE;
			}

			//допустима область для малювання пунктів налаштувань ======
			const int AVAILABLE_AREA_TOP = 160;												//обмеження зверху
			const int AVAILABLE_AREA_BOTTOM = drawingWindow.bottom;							//обмеження знизу
			const int AVAILABLE_AREA_HEIGHT = AVAILABLE_AREA_BOTTOM - AVAILABLE_AREA_TOP;	//висота області
			//==========================================================

			//місце де починається малювання пунктів налаштувань по вертикалі
			int settingItem_Y = AVAILABLE_AREA_TOP + (AVAILABLE_AREA_HEIGHT - totalItemsHeight) / 2; //положення пункту по осі Y

			//прохід по кожному пункту налаштувань
			for (int i = 0; i < SETTINGS_ITEMS_NUMBER; ++i)
			{
				//скидання кольору тексту
				SetTextColor(MEMORY_DEVICE_CONTEXT, textColor);

				//якщо пункт налаштувань - гучність музики або гучність ефектів
				if (i == (int)SettingsItemID::MUSIC_VOLUME || i == (int)SettingsItemID::EFFECTS_VOLUME)
				{
					const wchar_t* LEFT_ARROW_SYMBOL = L"←"; //символ "стрілка вліво"
					const wchar_t* RIGHT_ARROW_SYMBOL = L"→"; //символ "стрілка вправо"

					//для модифікованого тексту пункту налаштувань
					wchar_t volumeText[30];

					//копіювання модифікованого тексту пункту налаштувань
					swprintf
					(
						volumeText,
						30,
						i == (int)SettingsItemID::MUSIC_VOLUME ? L"Music Volume: %d%%" : L"Effects Volume: %d%%",
						i == (int)SettingsItemID::MUSIC_VOLUME ? Sound::getMusicVolume() : Sound::getEffectsVolume()
					);

					SIZE leftArrowSize;		//розмір стрілки вліво
					SIZE rightArrowSize;	//розмір стрілки вправо
					SIZE volumeTextSize;	//розмір тексту пункту налаштувань (гучність)

					//отримання розміру тексту стрілки вліво
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, LEFT_ARROW_SYMBOL, lstrlenW(LEFT_ARROW_SYMBOL), &leftArrowSize);

					//отримання розміру тексту стрілки вправо
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, volumeText, lstrlenW(volumeText), &volumeTextSize);

					//отримання розміру тексту пункту налаштувань (гучність)
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, RIGHT_ARROW_SYMBOL, lstrlenW(RIGHT_ARROW_SYMBOL), &rightArrowSize);

					const int TEXT_ARROW_SPACING = 10; //проміжок між стрілкою і текстом

					//загальний розмір пункту налаштувань (гучність) = ліва стрілка + проміжок + текст + проміжок + права стрілка
					const int CURRENT_ITEM_WIDTH = leftArrowSize.cx + TEXT_ARROW_SPACING + volumeTextSize.cx + TEXT_ARROW_SPACING + rightArrowSize.cx;

					//звідки починати малювати текст пункту, аби він був по центру по горизонталі
					const int SETTINGS_ITEM_X = (drawingWindow.right - CURRENT_ITEM_WIDTH) / 2; //положення пункту по осі X

					//прямокутник в якому знаходиться стрілка вліво
					const RECT LEFT_ARROW_RECTANGLE =
					{
						SETTINGS_ITEM_X,					//ліворуч
						settingItem_Y,						//верх
						SETTINGS_ITEM_X + leftArrowSize.cx, //праворуч (ширина стрілки)
						settingItem_Y + leftArrowSize.cy	//низ (висота стрілки)
					};

					//прямокутник в якому знаходиться стрілка вправо
					const RECT RIGHT_ARROW_RECTANGLE =
					{
						//ліворуч
						SETTINGS_ITEM_X + leftArrowSize.cx + TEXT_ARROW_SPACING + volumeTextSize.cx + TEXT_ARROW_SPACING,
						settingItem_Y,							//верх
						SETTINGS_ITEM_X + CURRENT_ITEM_WIDTH,	//праворуч (ширина стрілки)
						settingItem_Y + rightArrowSize.cy		//низ (висота стрілки)
					};

					COLORREF leftArrowColor = textColor; //колір стрілки вліво
					COLORREF rightArrowColor = textColor; //колір стрілки вправо

					//якщо курсор миші було наведено на стрілку вліво
					if (PtInRect(&LEFT_ARROW_RECTANGLE, g_game->getMousePosition()))
					{
						if (g_isDarkTheme) //якщо увімкнена темна тема
						{
							//то стрілка вліво буде жовтою
							leftArrowColor = g_YELLOW;
						}
						else //якщо увімкнена світла тема
						{
							//то стрілка вліво буде помаранчевою
							leftArrowColor = g_ORANGE;
						}
					}

					//якщо курсор миші було наведено на стрілку вправо
					if (PtInRect(&RIGHT_ARROW_RECTANGLE, g_game->getMousePosition()))
					{
						if (g_isDarkTheme) //якщо увімкнена темна тема
						{
							//то стрілка вправо буде жовтою
							rightArrowColor = g_YELLOW;
						}
						else //якщо увімкнена світла тема
						{
							//то стрілка вправо буде помаранчевою
							rightArrowColor = g_ORANGE;
						}
					}

					//встановлення кольору стрілки вліво
					SetTextColor(MEMORY_DEVICE_CONTEXT, leftArrowColor);

					//малювання стрілки вліво
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						SETTINGS_ITEM_X,
						settingItem_Y,
						LEFT_ARROW_SYMBOL,
						lstrlenW(LEFT_ARROW_SYMBOL)
					);

					//встановлення кольору тексту
					SetTextColor(MEMORY_DEVICE_CONTEXT, textColor);

					//малювання тексту пункту налаштувань (гучність)
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						SETTINGS_ITEM_X + leftArrowSize.cx + TEXT_ARROW_SPACING,
						settingItem_Y,
						volumeText,
						lstrlenW(volumeText)
					);

					//встановлення кольору стрілки вправо
					SetTextColor(MEMORY_DEVICE_CONTEXT, rightArrowColor);

					//малювання стрілки вправо
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						SETTINGS_ITEM_X + leftArrowSize.cx + TEXT_ARROW_SPACING + volumeTextSize.cx + TEXT_ARROW_SPACING,
						settingItem_Y,
						RIGHT_ARROW_SYMBOL,
						lstrlenW(RIGHT_ARROW_SYMBOL)
					);

					//оновлення положення пункту налаштувань
					g_settingsItems[i].itemRectangle =
					{
						SETTINGS_ITEM_X,
						settingItem_Y,
						SETTINGS_ITEM_X + CURRENT_ITEM_WIDTH,
						settingItem_Y + volumeTextSize.cy
					};

					//оновлення положення пункту налаштувань по осі Y
					settingItem_Y += volumeTextSize.cy + VERTICAL_ITEMS_SPACE;
				}
				else //якщо пункт налаштувань – не гучність
				{
					//текст поточного пункту налаштувань
					const std::wstring& ITEM_TEXT = g_settingsItems[i].itemText;

					//розмір поточного пункту налаштувань
					const SIZE ITEM_SIZE = itemSizes[i];

					//звідки починати малювати текст пункту, аби він був по центру по горизонталі
					const int SETTINGS_ITEM_X = (drawingWindow.right - ITEM_SIZE.cx) / 2; //положення пункту по осі X

					//якщо курсор миші було наведено на пункт налаштувань
					if (i == g_cursorSettingsItem)
					{
						if (g_isDarkTheme) //якщо увімкнена темна тема
						{
							//то пункт налаштувань буде жовтим
							SetTextColor(MEMORY_DEVICE_CONTEXT, g_YELLOW);
						}
						else //якщо увімкнена світла тема
						{
							//то пункт налаштувань буде помаранчевим
							SetTextColor(MEMORY_DEVICE_CONTEXT, g_ORANGE);
						}
					}

					//малювання тексту пункту налаштувань
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						SETTINGS_ITEM_X,
						settingItem_Y,
						ITEM_TEXT.c_str(),
						ITEM_TEXT.size()
					);

					//оновлення положення пункту налаштувань
					g_settingsItems[i].itemRectangle =
					{
						SETTINGS_ITEM_X,
						settingItem_Y,
						SETTINGS_ITEM_X + ITEM_SIZE.cx,
						settingItem_Y + ITEM_SIZE.cy
					};

					//оновлення положення пункту налаштувань по осі Y
					settingItem_Y += ITEM_SIZE.cy + VERTICAL_ITEMS_SPACE;
				}
			}
		} break; // UIState::SETTINGS


		case UIState::CONTROLS: //якщо гравець в налаштуваннях
		{
			//текст заголовока управління
			const wchar_t* CONTROL_TITLE = L"CONTROLS";

			//вибір шрифта для заголовків
			SelectObject(MEMORY_DEVICE_CONTEXT, TITLE_FONT);

			//ширина та висота тексту заголовка
			SIZE titleSize;

			//отримання розміру тексту заголовка в пікселях
			GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, CONTROL_TITLE, lstrlenW(CONTROL_TITLE), &titleSize);

			//звідки починати малювати заголовок, аби він був по центру по горизонталі
			const int TITLE_X = (drawingWindow.right - titleSize.cx) / 2; //положення тексту заголовка по осі X

			//положення тексту заголовка по осі Y
			const int TITLE_Y = 100;

			//малювання тексту заголовка
			TextOutW(MEMORY_DEVICE_CONTEXT, TITLE_X, TITLE_Y, CONTROL_TITLE, lstrlenW(CONTROL_TITLE));

			//вибір маленького шрифта
			SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

			//видалення шрифта для тексту заголовка
			DeleteObject(TITLE_FONT);

			//положення пункту по осі Y
			int controlsItem_Y = TITLE_Y + 200;

			//текст пунктів управління
			const wchar_t* CONTROLS_ITEMS[] =
			{
				L"←  – move left",
				L"→  – move right",
				L"↓  – move down",
				L"↑  – rotate",
				L"Space – quick fall",
				L"P – pause",
				L"Q – quit game"
			};

			//кількість пунктів управління = розмів всього масиву / розміру одного елемента
			const int CONTROLS_ITEMS_NUMBER = sizeof(CONTROLS_ITEMS) / sizeof(CONTROLS_ITEMS[0]);

			//найдовша ліва частини слова (до тире)
			int widestBeforeDash = 0;

			//прохід по кожному пункту управління
			for (int i = 0; i < CONTROLS_ITEMS_NUMBER; ++i)
			{
				//пошук тире в тексті пункту управління
				const wchar_t* DASH_POSITION = wcschr(CONTROLS_ITEMS[i], L'–');

				//якщо тире було знайдено
				if (DASH_POSITION)
				{
					//ліва частина тексту до тире (не включно)
					const std::wstring LEFT_PART(CONTROLS_ITEMS[i], DASH_POSITION);

					//розмір тексту лівої частини тексту до тире в пікселях
					SIZE leftPartSize;

					//отримання розміру тексту лівої частини тексту
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, LEFT_PART.c_str(), LEFT_PART.size(), &leftPartSize);

					//пошук найдовшої лівої частини тексту до тире
					if (leftPartSize.cx > widestBeforeDash)
					{
						widestBeforeDash = leftPartSize.cx;
					}
				}
			}

			//зсув тексту пункту налаштувань вліво від центру вікна
			const int OFFSET_FROM_CENTER = 65;

			//проміжок між пунктами управління по вертикалі
			const int VERTICAL_ITEMS_SPACE = 20;

			//прохід по кожному пункту управління
			for (int i = 0; i < CONTROLS_ITEMS_NUMBER; ++i)
			{
				//пошук тире в тексті пункту управління
				const wchar_t* DASH_POSITION = wcschr(CONTROLS_ITEMS[i], L'–');

				//якщо тире було знайдено
				if (DASH_POSITION)
				{
					//ліва частина тексту до тире (не включно)
					const std::wstring LEFT_PART(CONTROLS_ITEMS[i], DASH_POSITION);

					//права частина тексту від тире (включно)
					const std::wstring RIGHT_PART(DASH_POSITION);

					//розмір тексту лівої частини тексту до тире в пікселях
					SIZE leftPartSize;

					//отримання розміру тексту лівої частини тексту
					GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, LEFT_PART.c_str(), LEFT_PART.size(), &leftPartSize);

					//місце де починається малювання тире в тексті пункту управління по осі X
					const int DASH_X = (drawingWindow.right - widestBeforeDash) / 2 - OFFSET_FROM_CENTER;

					//малювання лівої частини тексту до тире (не включно)
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						DASH_X + (widestBeforeDash - leftPartSize.cx),
						controlsItem_Y,
						LEFT_PART.c_str(),
						LEFT_PART.size()
					);

					//малювання правої частини тексту від тире (включно)
					TextOutW
					(
						MEMORY_DEVICE_CONTEXT,
						DASH_X + widestBeforeDash,
						controlsItem_Y,
						RIGHT_PART.c_str(),
						RIGHT_PART.size()
					);
				}

				//розмір тексту поточного пункту налаштувань
				SIZE itemSize;

				//отримання розміру тексту поточного пункту управління
				GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, CONTROLS_ITEMS[i], lstrlenW(CONTROLS_ITEMS[i]), &itemSize);

				//зсув для наступного пункту управління по осі Y
				controlsItem_Y += itemSize.cy + VERTICAL_ITEMS_SPACE;
			}

			//прохід по кожному пункту управління (для пункту "Back")
			for (int i = 0; i < g_controlsItems.size(); ++i)
			{
				//текст поточного пункту управління
				const std::wstring& CURRENT_ITEM_TEXT = g_controlsItems[i].itemText;

				//розмір поточного пункту управління
				SIZE currentItemSize;

				//отримання розміру тексту поточного пункту управління
				GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, CURRENT_ITEM_TEXT.c_str(), CURRENT_ITEM_TEXT.size(), &currentItemSize);

				//звідки починати малювати текст пункту, аби він був по центру по горизонталі
				const int CONTROLS_ITEM_X = (drawingWindow.right - currentItemSize.cx) / 2; //положення пункту по осі X

				//якщо курсор миші було наведено на пункт управління
				if (i == g_cursorControlsItem)
				{
					if (g_isDarkTheme) //якщо увімкнена темна тема
					{
						//то пункт управління буде жовтим
						SetTextColor(MEMORY_DEVICE_CONTEXT, g_YELLOW);
					}
					else //якщо увімкнена світла тема
					{
						//то пункт управління буде помаранчевим
						SetTextColor(MEMORY_DEVICE_CONTEXT, g_ORANGE);
					}
				}
				else //якщо курсор миші не було наведено на пункт управління
				{
					//то пункт управління буде звичайним кольором
					SetTextColor(MEMORY_DEVICE_CONTEXT, textColor);
				}

				//оновлення положення пункту управління
				g_controlsItems[i].itemRectangle =
				{
					CONTROLS_ITEM_X,
					controlsItem_Y,
					CONTROLS_ITEM_X + currentItemSize.cx,
					controlsItem_Y + currentItemSize.cy
				};

				//малювання тексту пункту управління
				TextOutW
				(
					MEMORY_DEVICE_CONTEXT,
					CONTROLS_ITEM_X,
					controlsItem_Y,
					CURRENT_ITEM_TEXT.c_str(),
					CURRENT_ITEM_TEXT.size()
				);

				//зсув для наступного пункту управління по осі Y
				controlsItem_Y += (currentItemSize.cy + VERTICAL_ITEMS_SPACE);
			}

		} break; // UIState::CONTROLS

		case UIState::PLAYING:		//якщо гравець грає
		case UIState::ENTER_NAME:	//якщо гравець вводить ім'я
		case UIState::PAUSED:		//якщо гра на паузі
		{
			//прохід по всім рядкам ігрого поля
			for (int row = 0; row < g_BOARD_HEIGHT; ++row)
			{
				//прохід по всім стовпцям ігрого поля
				for (int col = 0; col < g_BOARD_WIDTH; ++col)
				{
					//колір поточної клітинки ігрового поля
					COLORREF currentBlockColor = g_game->getBlockColor(row, col);

					//координати поточної блоку ігрового поля в пікселях =======
					const int CURRENT_BLOCK_LEFT = col * g_BLOCK_SIZE;					//координата лівого краю поточного блоку
					const int CURRENT_BLOCK_TOP = row * g_BLOCK_SIZE;					//координата верхнього краю поточного блоку
					const int CURRENT_BLOCK_RIGHT = CURRENT_BLOCK_LEFT + g_BLOCK_SIZE;	//координата правого краю поточного блоку
					const int CURRENT_BLOCK_BOTTOM = CURRENT_BLOCK_TOP + g_BLOCK_SIZE;		//координата нижнього краю поточного блоку
					//==========================================================

					const int FLASHING_DURATION = 6000; //тривалість миготіння блоків фігур в мілісекундах

					//якщо гравець побив рекорд, то блоки фігур будуть мигати до 6 секунд
					if
						(
							g_game->getCurrentState() == UIState::ENTER_NAME &&
							g_game->isNewRecord() &&
							((GetTickCount64() - g_game->getFlashStartTime()) < FLASHING_DURATION)
							)
					{
						//якщо поточний блок це частина фігури
						if (currentBlockColor != g_BLACK)
						{
							//то його колір буде випадковим
							currentBlockColor = getRandomColor();
						}

					}
					else if //якщо блоки промиготіли 6 секунд
						(
							g_game->getCurrentState() == UIState::ENTER_NAME &&
							g_game->isNewRecord() &&
							((GetTickCount64() - g_game->getFlashStartTime()) >= FLASHING_DURATION)
							)
					{
						//то скидаємо прапорець рекорду і час початку миготіння
						//більше не буде миготіння
						g_game->setNewRecord(false);
						g_game->resetFlashStartTime();
					}

					//якщо поточний блок це частина фігури
					if (currentBlockColor != g_BLACK)
					{
						//малювання блоку фігури ===================================
						//створення пензля з кольором блоку
						const HBRUSH BRUSH = CreateSolidBrush(currentBlockColor); //яким кольором заповнити блок

						//що заповнити кольором
						const RECT CURRENT_BLOCK_RECTANGLE =
						{
							CURRENT_BLOCK_LEFT,		//координата лівої сторони
							CURRENT_BLOCK_TOP,		//координата верхньої сторони
							CURRENT_BLOCK_RIGHT,	//координата правої сторони
							CURRENT_BLOCK_BOTTOM	//координата нижньої сторони
						};

						//заповнення блоку кольором
						FillRect(MEMORY_DEVICE_CONTEXT, &CURRENT_BLOCK_RECTANGLE, BRUSH);

						//видалення пензля (вивільнення пам'яті)
						DeleteObject(BRUSH);
						//==========================================================
					}

					//малювання контуру блоку ==================================
					//створення контуру (ручки) : тип, товщина і колір контуру
					const HPEN PEN = CreatePen(PS_SOLID, 1, g_GRAY);

					//вибір контуру
					SelectObject(MEMORY_DEVICE_CONTEXT, PEN);

					//вибір заливки (пензля) : прозорий (без заливки)
					SelectObject(MEMORY_DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

					//малювання контуру блоку
					Rectangle(MEMORY_DEVICE_CONTEXT, CURRENT_BLOCK_LEFT, CURRENT_BLOCK_TOP, CURRENT_BLOCK_RIGHT, CURRENT_BLOCK_BOTTOM);

					//видалення контуру
					DeleteObject(PEN);
					//==========================================================
				}
			}

			//якщо гра не закінчена і не вводиться ім'я
			if (!g_game->isGameOver() && !g_game->isEnteringName())
			{
				//прохід по всіх блоках фігури
				for (const auto& BLOCK : g_game->getCurrentFigure().blocks)
				{
					//абсолютні координати блоку (позиція на ігровому полі) ====
					const int ABSOLUTE_X = g_game->getCurrentFigurePosition().x + BLOCK.x; //абсолютна координата по осі X
					const int ABSOLUTE_Y = g_game->getCurrentFigurePosition().y + BLOCK.y; //абсолютна координата по осі Y
					//==========================================================

					if //якщо координати блоку фігури в межах ігрового поля
						(
							ABSOLUTE_X >= 0 &&
							ABSOLUTE_X < g_BOARD_WIDTH &&
							ABSOLUTE_Y >= 0 &&
							ABSOLUTE_Y < g_BOARD_HEIGHT
							)
					{
						//колір поточного блоку фігури
						const COLORREF CURRENT_BLOCK_COLOR = g_game->getCurrentFigure().color;

						//координати поточної блоку ігрового поля в пікселях =======
						const int CURRENT_BLOCK_LEFT = ABSOLUTE_X * g_BLOCK_SIZE;			//координата лівого краю поточного блоку
						const int CURRENT_BLOCK_TOP = ABSOLUTE_Y * g_BLOCK_SIZE;			//координата верхнього краю поточного блоку
						const int CURRENT_BLOCK_RIGHT = CURRENT_BLOCK_LEFT + g_BLOCK_SIZE;	//координата правого краю поточного блоку
						const int CURRENT_BLOCK_BOTTOM = CURRENT_BLOCK_TOP + g_BLOCK_SIZE;		//координата нижнього краю поточного блоку
						//==========================================================

						//малювання блоку фігури ===================================
						//створення пензля з кольором блоку
						const HBRUSH BRUSH = CreateSolidBrush(CURRENT_BLOCK_COLOR); //яким кольором заповнити блок

						//що заповнити кольором
						const RECT CURRENT_BLOCK_RECTANGLE =
						{
							CURRENT_BLOCK_LEFT,		//координата лівої сторони
							CURRENT_BLOCK_TOP,		//координата верхньої сторони
							CURRENT_BLOCK_RIGHT,	//координата правої сторони
							CURRENT_BLOCK_BOTTOM	//координата нижньої сторони
						};

						//заповнення блоку кольором
						FillRect(MEMORY_DEVICE_CONTEXT, &CURRENT_BLOCK_RECTANGLE, BRUSH);

						//видалення пензля (вивільнення пам'яті)
						DeleteObject(BRUSH);
						//==========================================================

						//малювання контуру блоку ==================================
						//створення контуру (ручки) : тип, товщина і колір контуру
						HPEN pen = CreatePen(PS_SOLID, 1, g_GRAY);

						//вибір контуру
						SelectObject(MEMORY_DEVICE_CONTEXT, pen);

						//вибір заливки (пензля) : прозорий (без заливки)
						SelectObject(MEMORY_DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

						//малювання контуру блоку
						Rectangle(MEMORY_DEVICE_CONTEXT, CURRENT_BLOCK_LEFT, CURRENT_BLOCK_TOP, CURRENT_BLOCK_RIGHT, CURRENT_BLOCK_BOTTOM);

						//видалення контуру
						DeleteObject(pen);
						//==========================================================
					}
				}
			}

			//координати інформаційної панелі праворуч в пікселях ======
			const int INFO_PANEL_X = g_BOARD_WIDTH * g_BLOCK_SIZE + 20; //координати інформаційної панелі по осі X
			const int INFO_PANEL_Y = 50;								//координати інформаційної панелі по осі Y
			//==========================================================

			//текст наступної фігури
			const wchar_t* NEXT_FIGURE_TEXT = L"Next figure:";

			//виведення тексту наступної фігури
			TextOutW
			(
				MEMORY_DEVICE_CONTEXT,
				INFO_PANEL_X,
				INFO_PANEL_Y,
				NEXT_FIGURE_TEXT,
				lstrlenW(NEXT_FIGURE_TEXT)
			);

			//вертикальна позиція області, в якій буде відображатися наступна фігура
			const int NEXT_FIGURE_Y = INFO_PANEL_Y + 30;

			//розмір області в пікселях, в яку буде вписана наступна фігура – 80*80
			const int NEXT_FIGURE_AREA_SIZE = 80;

			//розмір блоку наступної фігури в пікселях – 20*20
			const int NEXT_FIGURE_BLOCK_SIZE = 20;

			//мінімальні і максимальні координати блоку фігури
			int minBlockX = INT_MAX;
			int minBlockY = INT_MAX;
			int maxBlockX = INT_MIN;
			int maxBlockY = INT_MIN;

			//прохід по всіх блоках наступної фігури
			for (const auto& CURRENT_BLOCK : g_game->getNextFigure().blocks)
			{
				//оновлення мінімальной координати блоку фігури по осі X
				if (CURRENT_BLOCK.x < minBlockX)
				{
					minBlockX = CURRENT_BLOCK.x;
				}

				//оновлення максимальной координати блоку фігури по осі X
				if (CURRENT_BLOCK.x > maxBlockX)
				{
					maxBlockX = CURRENT_BLOCK.x;
				}

				//оновлення мінімальной координати блоку фігури по осі Y
				if (CURRENT_BLOCK.y < minBlockY)
				{
					minBlockY = CURRENT_BLOCK.y;
				}

				//оновлення максимальной координати блоку фігури по осі Y
				if (CURRENT_BLOCK.y > maxBlockY)
				{
					maxBlockY = CURRENT_BLOCK.y;
				}
			}

			//ширина наступної фігури в пікселях = кількість блоків по вертикалі * розмір блоку
			const int NEXT_FIGURE_WIDTH = (maxBlockX - minBlockX + 1) * NEXT_FIGURE_BLOCK_SIZE;

			//висота наступної фігури в пікселях = кількість блоків по горизонталі * розмір блоку
			const int NEXT_FIGURE_HEIGHT = (maxBlockY - minBlockY + 1) * NEXT_FIGURE_BLOCK_SIZE;

			//де починати малювати наступну фігуру, щоб вона стояла по центру відведеної області по горизонталі
			const int NEXT_FIGURE_CENTER_X = INFO_PANEL_X + (NEXT_FIGURE_AREA_SIZE - NEXT_FIGURE_WIDTH) / 2;

			//де починати малювати наступну фігуру, щоб вона стояла по центру відведеної області по вертикалі
			const int NEXT_FIGURE_CENTER_Y = NEXT_FIGURE_Y + (NEXT_FIGURE_AREA_SIZE - NEXT_FIGURE_HEIGHT) / 2;

			//колір наступної фігури
			const COLORREF NEXT_FIGURE_COLOR = g_game->getNextFigure().color;

			//прохід по всіх блоках наступної фігури
			for (const auto& CURRENT_BLOCK : g_game->getNextFigure().blocks)
			{
				//де потрібно відмалювати поточний блок наступної фігури по осі X
				const int DRAW_BLOCK_X = NEXT_FIGURE_CENTER_X + (CURRENT_BLOCK.x - minBlockX) * NEXT_FIGURE_BLOCK_SIZE;

				//де потрібно відмалювати поточний блок наступної фігури по осі Y
				const int DRAW_BLOCK_Y = NEXT_FIGURE_CENTER_Y + (CURRENT_BLOCK.y - minBlockY) * NEXT_FIGURE_BLOCK_SIZE;

				//малювання блоку наступної фігури =========================
				//створення пензля з кольором блоку
				const HBRUSH BRUSH = CreateSolidBrush(NEXT_FIGURE_COLOR); //яким кольором заповнити блок

				//що заповнити кольором
				const RECT CURRENT_BLOCK_RECTANGLE =
				{
					DRAW_BLOCK_X,							//координата лівої сторони
					DRAW_BLOCK_Y,							//координата верхньої сторони
					DRAW_BLOCK_X + NEXT_FIGURE_BLOCK_SIZE,	//координата правої сторони
					DRAW_BLOCK_Y + NEXT_FIGURE_BLOCK_SIZE	//координата нижньої сторони
				};

				//заповнення блоку кольором
				FillRect(MEMORY_DEVICE_CONTEXT, &CURRENT_BLOCK_RECTANGLE, BRUSH);

				//видалення пензля (вивільнення пам'яті)
				DeleteObject(BRUSH);
				//==========================================================

				//малювання контуру блоку ================================
				//створення контуру (ручки) : тип, товщина і колір контуру
				const HPEN PEN = CreatePen(PS_SOLID, 1, g_GRAY);

				//вибір контуру
				SelectObject(MEMORY_DEVICE_CONTEXT, PEN);

				//вибір заливки (пензля) : прозорий (без заливки)
				SelectObject(MEMORY_DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

				//малювання контуру блоку
				Rectangle
				(
					MEMORY_DEVICE_CONTEXT,
					DRAW_BLOCK_X,
					DRAW_BLOCK_Y,
					DRAW_BLOCK_X + NEXT_FIGURE_BLOCK_SIZE,
					DRAW_BLOCK_Y + NEXT_FIGURE_BLOCK_SIZE
				);

				//видалення контуру
				DeleteObject(PEN);
				//========================================================
			}

			//де починати малювати текст інформаційної панелі по осі X
			const int TEXT_X = INFO_PANEL_X;

			//де починати малювати текст інформаційної панелі по осі Y
			int text_Y = NEXT_FIGURE_Y + NEXT_FIGURE_AREA_SIZE + 20;

			//відступ між текстом по вертикалі
			const int TEXT_SPACE = 40;

			//массив для тексту на інформаційній панелі
			wchar_t currentText[50];

			//заповнення массиву модифікованим текстом
			wcscpy_s(currentText, L"Score:");

			//вибір шрифта для тексту
			SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

			//виведення тексту
			TextOutW(MEMORY_DEVICE_CONTEXT, TEXT_X, text_Y, currentText, lstrlenW(currentText));

			//відступ
			text_Y += TEXT_SPACE;

			//текст поточного рахунку
			const HFONT CURRENT_SCRORE_FONT = CreateFontW
			(
				50,								//висота шрифта
				0,								//ширина шрифта
				0,								//кут повороту шрифта
				0, 								//направлення шрифта
				FW_BOLD,						//жирність шрифта
				0,								//чи курсив
				0, 								//чи підкреслений
				0,								//чи закреслений
				DEFAULT_CHARSET,				//кодування (за замовчування)
				OUT_DEFAULT_PRECIS,				//точність (за замовчування)
				CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
				DEFAULT_QUALITY,				//якість (за замовчування)
				DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
				L"Arial"						//назва шрифта
			);

			//вибір шрифта для поточного рахунку
			SelectObject(MEMORY_DEVICE_CONTEXT, CURRENT_SCRORE_FONT);

			//текст поточного рахунку
			swprintf_s(currentText, L"%d", g_game->getCurrentScore());

			//виведення тексту
			TextOutW(MEMORY_DEVICE_CONTEXT, TEXT_X, text_Y, currentText, lstrlenW(currentText));

			//вибір шрифта для тексту 
			SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

			//видалення шрифта для поточного рахунку
			DeleteObject(CURRENT_SCRORE_FONT);

			//відступ
			text_Y += TEXT_SPACE * 2;

			//якщо гравець побив рекорд
			if (g_game->getCurrentState() == UIState::ENTER_NAME && g_game->isNewRecord())
			{
				//шрифт тексту для повідомлення про новий рекорд
				const HFONT RECORD_FONT = CreateFontW
				(
					48,								//висота шрифта
					0,								//ширина шрифта
					0,								//кут повороту шрифта
					0,								//направлення шрифта
					FW_BOLD,						//жирність шрифта
					0,								//чи курсив
					0,								//чи підкреслений
					0,								//чи закреслений
					DEFAULT_CHARSET,				//кодування (за замовчування)
					OUT_DEFAULT_PRECIS,				//точність (за замовчування)
					CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
					DEFAULT_QUALITY,				//якість (за замовчування)
					DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
					L"Arial"						//назва шрифта
				);

				//вибір потрібного шрифта
				SelectObject(MEMORY_DEVICE_CONTEXT, RECORD_FONT);

				//текст повідомлення про новий рекорд
				const wchar_t* RECORD_TEXT = L"NEW RECORD!";

				//розмір тексту повідомлення про новий рекорд
				SIZE recordSize;

				//отримання розміру тексту повідомлення про новий рекорд
				GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, RECORD_TEXT, lstrlenW(RECORD_TEXT), &recordSize);

				//де починати малювати текст повідомлення про новий рекорд по осі X, аби він був по центру
				const int RECORD_TEXT_CENTER_X = (g_BOARD_WIDTH * g_BLOCK_SIZE - recordSize.cx) / 2;

				//де починати малювати текст повідомлення про новий рекорд по осі Y, аби він був по центру
				const int RECORD_TEXT_CENTER_Y = (g_BOARD_HEIGHT * g_BLOCK_SIZE) / 2 - recordSize.cy - 20;

				//виведення тексту повідомлення про новий рекорд
				TextOutW
				(
					MEMORY_DEVICE_CONTEXT,
					RECORD_TEXT_CENTER_X,
					RECORD_TEXT_CENTER_Y,
					RECORD_TEXT,
					lstrlenW(RECORD_TEXT)
				);

				//вибір шрифта для звичайного тексту
				SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

				//видалення шрифта
				DeleteObject(RECORD_FONT);
			}

			//якщо гра була закінчена і гравець закінчив вводити ім'я
			if (g_game->isGameOver() && !g_game->isEnteringName())
			{
				//шрифт тексту для повідомлення про завершення гри
				const HFONT GAME_OVER_FONT = CreateFontW
				(
					72,								//висота шрифта
					0,								//ширина шрифта
					0,								//кут повороту шрифта
					0,								//направлення шрифта
					FW_BOLD,						//жирність шрифта
					0,								//чи курсив
					0,								//чи підкреслений
					0,								//чи закреслений
					DEFAULT_CHARSET,				//кодування (за замовчування)
					OUT_DEFAULT_PRECIS,				//точність (за замовчування)
					CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
					DEFAULT_QUALITY,				//якість (за замовчування)
					DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
					L"Arial"						//назва шрифта
				);

				//вибір шрифта для завершнення гри
				SelectObject(MEMORY_DEVICE_CONTEXT, GAME_OVER_FONT);

				//текст завершення гри
				const wchar_t* GAME_OVER_TEXT = L"GAME OVER";

				//розмір тексту завершення гри
				SIZE gameOverSize;

				//отримання розміру тексту завершення гри
				GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, GAME_OVER_TEXT, lstrlenW(GAME_OVER_TEXT), &gameOverSize);

				//де починати малювати текст завершення гри по осі X, аби він був по центру
				const int GAME_OVER_TEXT_CENTER_X = (g_BOARD_WIDTH * g_BLOCK_SIZE - gameOverSize.cx) / 2;

				//де починати малювати текст завершення гри по осі Y, аби він був по центру
				const int GAME_OVER_TEXT_CENTER_Y = (g_BOARD_HEIGHT * g_BLOCK_SIZE - gameOverSize.cy) / 2;

				//виведення тексту завершення гри
				TextOutW
				(
					MEMORY_DEVICE_CONTEXT,
					GAME_OVER_TEXT_CENTER_X,
					GAME_OVER_TEXT_CENTER_Y,
					GAME_OVER_TEXT,
					lstrlenW(GAME_OVER_TEXT)
				);

				//вибір шрифта для звичайного тексту
				SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

				//видалення шрифта для для повідомлення про завершення гри
				DeleteObject(GAME_OVER_FONT);
			}

			//якщо гравець вводить ім'я
			if (g_game->getCurrentState() == UIState::ENTER_NAME)
			{
				//текст для введення імені
				const wchar_t* ENTER_NAME_TEXT = L"Enter your name:";

				//виведення тексту для введення імені
				TextOutW(MEMORY_DEVICE_CONTEXT, TEXT_X, text_Y, ENTER_NAME_TEXT, lstrlenW(ENTER_NAME_TEXT));

				//відступ між текстом по вертикалі
				text_Y += TEXT_SPACE;

				//виведення імені гравця
				TextOutW
				(
					MEMORY_DEVICE_CONTEXT,
					TEXT_X,
					text_Y,
					g_game->getCurrentName().c_str(),
					g_game->getCurrentName().size()
				);

				//розмір імені гравця
				SIZE nameSize;

				//отримання розміру імені гравця
				GetTextExtentPoint32W
				(
					MEMORY_DEVICE_CONTEXT,
					g_game->getCurrentName().c_str(),
					g_game->getCurrentName().size(),
					&nameSize
				);

				//координати блимаючого курсора по осі X
				const int CURSOR_X = TEXT_X + nameSize.cx;

				//координати блимаючого курсора по осі Y
				const int CURSOR_Y = text_Y;

				//висота курсора
				const int CURSOR_HEIGHT = nameSize.cy;

				//якщо курсор видимий
				if (g_game->isCursorVisible())
				{
					//маленький прямокутник, який представляє собою курсор
					const RECT CURSOR =
					{
						CURSOR_X,					//координата лівої сторони
						CURSOR_Y,					//координата верхньої сторони
						CURSOR_X + 2,				//координата правої сторони
						CURSOR_Y + CURSOR_HEIGHT	//координата нижньої сторони
					};

					//колір блимаючого курсора
					COLORREF CURSOR_COLOR = g_WHITE;

					if (g_isDarkTheme) //якщо встановлена темна тема
					{
						//то курсор буде білим
						COLORREF CURSOR_COLOR = g_WHITE;
					}
					else //якщо встановлена світла тема
					{
						//то курсор буде чорним
						COLORREF CURSOR_COLOR = g_BLACK;
					}

					HBRUSH brush = CreateSolidBrush(CURSOR_COLOR);

					FillRect(MEMORY_DEVICE_CONTEXT, &CURSOR, brush);
					DeleteObject(brush);
				}

				//відступ між текстом по вертикалі
				text_Y += TEXT_SPACE;
			}

			//якщо гра на паузі
			if (g_game->getCurrentState() == UIState::PAUSED)
			{
				//шрифт тексту для паузи
				const HFONT PAUSE_FONT = CreateFontW
				(
					72,								//висота шрифта
					0,								//ширина шрифта
					0,								//кут повороту шрифта
					0,								//направлення шрифта
					FW_BOLD,						//жирність шрифта
					0,								//чи курсив
					0,								//чи підкреслений
					0,								//чи закреслений
					DEFAULT_CHARSET,				//кодування (за замовчування)
					OUT_DEFAULT_PRECIS,				//точність (за замовчування)
					CLIP_DEFAULT_PRECIS,			//обрізання (за замовчування)
					DEFAULT_QUALITY,				//якість (за замовчування)
					DEFAULT_PITCH | FF_DONTCARE,	//розташування та стиль (за замовчування)
					L"Arial"						//назва шрифта
				);

				//вибір шрифта для паузи
				SelectObject(MEMORY_DEVICE_CONTEXT, PAUSE_FONT);

				//текст паузи
				const wchar_t* PAUSE_TEXT = L"PAUSE";

				//розмір тексту паузи
				SIZE pauseSize;

				//отримання розміру тексту паузи
				GetTextExtentPoint32W(MEMORY_DEVICE_CONTEXT, PAUSE_TEXT, lstrlenW(PAUSE_TEXT), &pauseSize);

				//координати тексту паузи по осі X, аби він був по центру
				int PAUSE_TEXT_CENTER_X = (g_BOARD_WIDTH * g_BLOCK_SIZE - pauseSize.cx) / 2;

				//координати тексту паузи по осі Y, аби він був по центру
				int PAUSE_TEXT_CENTER_Y = (g_BOARD_HEIGHT * g_BLOCK_SIZE - pauseSize.cy) / 2;

				//виведення тексту паузи
				TextOutW(MEMORY_DEVICE_CONTEXT, PAUSE_TEXT_CENTER_X, PAUSE_TEXT_CENTER_Y, PAUSE_TEXT, lstrlenW(PAUSE_TEXT));

				//вибір шрифта для звичайного тексту
				SelectObject(MEMORY_DEVICE_CONTEXT, TEXT_FONT);

				//видалення шрифта для паузи
				DeleteObject(PAUSE_FONT);
			}

			//відступ між текстом по вертикалі
			text_Y += TEXT_SPACE;

			//заповнення массиву потрібним текстом
			wcscpy_s(currentText, L"High Scores:");

			//виведення тексту
			TextOutW(MEMORY_DEVICE_CONTEXT, TEXT_X, text_Y, currentText, lstrlenW(currentText));

			//відступ між текстом по вертикалі
			text_Y += TEXT_SPACE;

			//прохід по всіх рекордах
			for (int i = 0; i < g_game->getScoreboard().size(); ++i)
			{
				//заповнення массиву модифікованим текстом
				swprintf_s
				(
					currentText,
					L"%d) %s - %d",
					i + 1,
					g_game->getScoreboard()[i].name.c_str(),
					g_game->getScoreboard()[i].score
				);

				//виведення рядка з рекордом
				TextOutW(MEMORY_DEVICE_CONTEXT, TEXT_X, text_Y, currentText, lstrlenW(currentText));

				//менший відступ між текстом по вертикалі
				text_Y += TEXT_SPACE - 10;
			}

		} break; //UIState::PLAYING, UIState::ENTER_NAME, UIState::PAUSED

		}

		//копіювання того, що в пам'яті вікна, на екран
		BitBlt
		(
			DEVICE_CONTEXT,			//куди копіювати
			0, 0,					//координати верхнього лівого кута вікна
			DRAWING_WINDOW_WIDTH,		//ширина вікна, яка буде скопійована
			DRAWING_WINDOW_HEIGHT,	//висота вікна, яка буде скопійована
			MEMORY_DEVICE_CONTEXT,	//звідки копіювати
			0, 0,					//координати верхнього лівого кута вікна в пам'яті
			SRCCOPY					//тип копіювання: просто скопіювати все
		);

		//вивільнення пам'яті
		DeleteObject(TEXT_FONT);
		DeleteObject(MEMORY_BITMAP);
		DeleteDC(MEMORY_DEVICE_CONTEXT);

		//повідомляє систему про кінець малювання
		EndPaint(WINDOW_HANDLE, &paintInfo);

	} break; //WM_PAINT

	case WM_DESTROY: //якщо гравець закрив вікно ігрове вікно
	{
		//зупинка таймерів =======================================
		KillTimer(WINDOW_HANDLE, Timer::DECOR_RAIN);
		KillTimer(WINDOW_HANDLE, Timer::GAME_TICK);
		//========================================================

		//закриття всіх звуків ===================================
		Sound::closeMenuMusic();
		Sound::closeHappyEndMusic();
		Sound::closeSadEndMusic();
		Sound::closeMoveSound();
		Sound::closeLineClearSound();
		Sound::closeSpaceSound();
		//========================================================

		//вивільнення пам'яті ====================================
		if (g_game)
		{
			delete g_game;
			g_game = nullptr;
		}
		if (g_game->m_random)
		{
			delete g_game->m_random;
			g_game->m_random = nullptr;
		}
		//========================================================

		//вихід із циклу обробки подій вікна
		PostQuitMessage(0);

	} break; //WM_DESTROY

	default: //якщо подія не оброблена
	{
		//то викликається стандартна обробка
		return DefWindowProc(WINDOW_HANDLE, MESSAGE, WPARAM, LPARAM);
	}

	}

	return 0;
} //WindowEvents

int WINAPI WinMain(HINSTANCE programHandle, HINSTANCE /*для сумісності*/, LPSTR commandLine, int windowType)
{
	//для свого курсора
	const wchar_t* CUSTOM_CURSOR_PATH = L"tetrisCursor.cur";

	//ім'я вікна для реєстрації
	const wchar_t TETRIS_WINDOW_NAME[] = L"Tetris";

	//налаштування вікна для реєстрації =======================
	WNDCLASSEX windowSettings = {}; //налаштування вікна

	windowSettings.cbSize = sizeof(WNDCLASSEX);						//вказуємо розмір структури (вказується для сумісності)
	windowSettings.lpfnWndProc = WindowEvents;								//вказуємо функцію, яка буде обробляти події ігрового вікна
	windowSettings.hInstance = programHandle;							//вказуємо ідентифікатор програми
	windowSettings.lpszClassName = TETRIS_WINDOW_NAME;						//вказуємо ім'я класу вікна
	windowSettings.hCursor = LoadCursorFromFileW(CUSTOM_CURSOR_PATH);	//вказуємо курсор
	//========================================================

	//реєстрація вікна з вказаними налаштуваннями
	RegisterClassEx(&windowSettings);

	//стиль вікна : вікно без рамки
	const DWORD WINDOW_STYLE = WS_POPUP;

	//розміри ігрового вікна в пікселях
	RECT windowRectangle =
	{
		0,													//координата лівого краю
		0,													//координата верхнього краю
		g_BOARD_WIDTH * g_BLOCK_SIZE + g_SCORE_PANEL_WIDTH, //координата правого краю
		g_BOARD_HEIGHT * g_BLOCK_SIZE						//координата нижнього краю
	};

	//коригування розмірів вікна
	AdjustWindowRect
	(
		&windowRectangle,	//що коригувати
		WINDOW_STYLE,		//стиль вікна
		0					//чи є меню
	);

	const HWND WINDOW_HANDLE = CreateWindowEx
	(
		0,												//додатковий стиль вікна
		TETRIS_WINDOW_NAME,								//зареєстроване ім'я класу вікна
		L"Tetris",										//заголовок вікна
		WINDOW_STYLE,									//стиль вікна
		CW_USEDEFAULT,									//початкова позиція вікна по осі X (за замовчуванням)
		CW_USEDEFAULT,									//початкова позиція вікна по осі Y (за замовчуванням)
		windowRectangle.right - windowRectangle.left,	//ширина вікна
		windowRectangle.bottom - windowRectangle.top,	//висота вікна
		nullptr,										//батьківське вікно (немає)
		nullptr,										//меню (немає)
		programHandle,									//ідентифікатор програми
		nullptr											//додаткові параметри (немає)
	);

	//чи вдалося створити вікно
	if (!WINDOW_HANDLE)
	{
		//якщо ні, то завершуємо програму
		return 0;
	}

	//показати вікно
	ShowWindow(WINDOW_HANDLE, windowType);

	//зберігає інформацію про подію вікна
	MSG msg;

	//цикл обробки подій вікна
	while
		(
			GetMessage //отримання події вікна
			(
				&msg,		//куди зберігати інформацію про подію
				nullptr,	//з якого вікна отримувати подію (nullptr – всі вікна)
				0,			//мінімальний код події (не використовується)
				0			//максимальний код події (не використовується)
			)
			)
	{
		//для WM_CHAR – аналіз натиснутої клавіші
		TranslateMessage(&msg);

		//передача події вікна в обробник
		DispatchMessage(&msg);
	}

	return 0;
}
