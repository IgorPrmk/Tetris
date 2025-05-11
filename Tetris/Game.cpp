#include "Game.h"

Game::Game() :
	m_currentState(UIState::MENU),	//���� �������
	m_previousState(UIState::MENU),	//��������� ���� ����
	m_isGameOver(false),			//��� �� ��������
	m_isEnteringName(false), 		//������� �� ������� ��'�
	m_isCursorVisible(false), 		//������ �� ��������
	m_inProgress(false),			//������� �� ���
	m_fallSpeed(500), 				//�������� ��� 500 (���������� �����)
	m_level(0),						//������������ ����������� ����
	m_isEndSoundPlayed(false),		//������ � ���� ��� �� �� ������������
	m_flashStartTime(0),			//�������� ���� ������� �������� ����� ������
	m_score(new Score()),			//��������� ��'���� ��� ������ � ���������
	m_random(new Random()),			//��������� ��'���� ��� ��������� ���������� �����
	m_mousePosition({ 0, 0 })		//���������� ������� ����
{
	//����������� �������� ����
	m_board.resize(g_BOARD_HEIGHT, std::vector<COLORREF>(g_BOARD_WIDTH, g_EMPTY_COLOR));

	//��������� �����
	m_figures.push_back({ { {0,0},{1,0},{2,0},{3,0} }, g_AZURE_I });	// I
	m_figures.push_back({ { {0,0},{1,0},{0,1},{1,1} }, g_YELLOW_O });	// O
	m_figures.push_back({ { {0,0},{1,0},{2,0},{1,1} }, g_PURPLE_T });	// T
	m_figures.push_back({ { {1,0},{2,0},{0,1},{1,1} }, g_GREEN_S });	// S
	m_figures.push_back({ { {0,0},{1,0},{1,1},{2,1} }, g_RED_Z });		// Z
	m_figures.push_back({ { {0,0},{0,1},{0,2},{1,2} }, g_BLUE_J });		// J
	m_figures.push_back({ { {1,0},{1,1},{1,2},{0,2} }, g_ORANGE_L });	// L

	//��������� �������� ��������� ������
	m_nextFigure = getRandomFigure();
}

Figure Game::getRandomFigure()
{
	//��������� ����������� ������� �� 0 �� 6
	const int RANDOM_INDEX = m_random->getRandomInt(0, g_MAX_NUMBER_FIGURES - 1);

	//���������� ��������� ������ � ������
	return m_figures[RANDOM_INDEX];
}

void Game::resetGame()
{
	//��������� �� ��� ������ �������� ���� � ���������� �� "�������" ��������
	for (int y = 0; y < g_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < g_BOARD_WIDTH; ++x)
		{
			m_board[y][x] = g_EMPTY_COLOR;
		}
	}

	m_score->clearCurrentRecord();			//������� ��������� �������
	m_score->setNewRecord(false);			//������� �� ��������� ����� ������
	m_isGameOver = false;			//��� �� ��������
	m_isEnteringName = false;			//������� �� ������� ��'�
	m_currentState = UIState::PLAYING; //���� ����� ����������: ������� ���
	m_inProgress = true;				//������� ���
	m_level = 0;				//������������ ����������� ����
	m_fallSpeed = 500;				//������������ ��������� �������� ���
	m_isEndSoundPlayed = false;			//������ � ���� ��� �� �� ������������
	m_flashStartTime = 0;				//�������� ���� ������� �������� ����� ������
	createFigure();							//������������ ���� ������ �� ������ ����
}

void Game::createFigure()
{
	//������ ���������� ����������� ������
	m_currentFigure = m_nextFigure;

	//�������� ���� �������� ������
	m_nextFigure = getRandomFigure();

	//������������ ������� ������ �� �������� ��� ============
	m_currentFigurePosition.x = g_BOARD_WIDTH / 2 - 2;	//+- �� ������
	m_currentFigurePosition.y = 0;						//������ ������� ����
	//==========================================================

	//���� ������ � ���� ��������, �� ��� ����������
	if (checkCollision(m_currentFigure, m_currentFigurePosition))
	{
		m_isGameOver = true;	//��� ����������
		m_inProgress = false;	//��� ����� �� �����

		checkTop9(); //����������, �� ��������� ������� ������ � ���

		//����������� ������ � ���� ���
		if (!m_isEndSoundPlayed)
		{
			playEndSound();
			m_isEndSoundPlayed = true;
		}
	}
}

bool Game::checkCollision(const Figure& FIGURE, const POINT& POSITION)
{
	//��������� �� ��� ������ ������
	for (const auto& CURRENT_BLOCK : FIGURE.blocks)
	{
		//�������� ���������� ����� (������� �� �������� ���) ====
		const int ABSOLUTE_X = POSITION.x + CURRENT_BLOCK.x; //��������� ���������� �� �� X
		const int ABSOLUTE_Y = POSITION.y + CURRENT_BLOCK.y; //��������� ���������� �� �� Y
		//==========================================================

		if //��������, �� �������� ���� �� ��� ���� ��� �������� � ���� �����
			(
				(ABSOLUTE_X < 0 || ABSOLUTE_X >= g_BOARD_WIDTH) ||
				(ABSOLUTE_Y < 0 || ABSOLUTE_Y >= g_BOARD_HEIGHT) ||
				(m_board[ABSOLUTE_Y][ABSOLUTE_X] != g_EMPTY_COLOR)
				)
		{
			return true; //���, ������ ��������
		}
	}

	return false; //�, ������ �� ��������
}

void Game::freezeFigure()
{
	//��������� �� ��� ������ ������
	for (const auto& CURRENT_BLOCK : m_currentFigure.blocks)
	{
		//�������� ���������� ����� (������� �� �������� ���) ====
		const int ABSOLUTE_X = m_currentFigurePosition.x + CURRENT_BLOCK.x; //��������� ���������� �� �� X
		const int ABSOLUTE_Y = m_currentFigurePosition.y + CURRENT_BLOCK.y; //��������� ���������� �� �� Y
		//==========================================================

		if //���� ���� �� �������� �� ��� ����
			(
				(ABSOLUTE_X >= 0 && ABSOLUTE_X < g_BOARD_WIDTH) &&
				(ABSOLUTE_Y >= 0 && ABSOLUTE_Y < g_BOARD_HEIGHT)
				)
		{
			//�� �����'������� ���� ���� �� �������� ���
			m_board[ABSOLUTE_Y][ABSOLUTE_X] = m_currentFigure.color;
		}

	}

	//�������� ���������� ����� �� �������� ���
	clearLines();

	//��������� ���� ������
	createFigure();
}

void Game::clearLines()
{
	//������� �������� �����
	int linesCleared = 0;

	//������ �� ��� ������ �������� ���� ������ ����
	for (int y = 0; y < g_BOARD_HEIGHT;)
	{
		//�� ����� ������� ����������
		bool isFull = true;

		//������ �� ��� ������ � �����
		for (int x = 0; x < g_BOARD_WIDTH; ++x)
		{
			//���� � ����� � ������ ����
			if (m_board[y][x] == g_EMPTY_COLOR)
			{
				//�� ����� �� ����������
				isFull = false;
				break;
			}
		}

		//���� ����� ������� ����������
		if (isFull)
		{
			//��������� ���������� ����� �� ������� 'y' � �������� ����
			m_board.erase(m_board.begin() + y);

			//������ ����� ����� ������ � ������� �������
			m_board.insert(m_board.begin(), std::vector<COLORREF>(g_BOARD_WIDTH, g_EMPTY_COLOR));

			//�������� ������� �������� �����
			++linesCleared;
		}
		else
		{
			//���� ����� �� ����������, �� ���������� �� ���������� �������� �����
			++y;
		}
	}

	//���� � ������ �����
	if (linesCleared > 0)
	{
		//������ ���� �� ������ ����� (100 ���� �� ����� �������� �����)
		m_score->setCurrentScore(m_score->getCurrentScore() + linesCleared * 100);

		//����������� ����� �������� �����
		Sound::playLineClearSound(false, true, Sound::getEffectsVolume());

		//����� ����� ���
		int newLevel = m_score->getCurrentScore() / 1000;

		//���� ����� ����� ��� ����� ���������
		if (newLevel > m_level)
		{
			//�� ��������� ����� ���
			m_level = newLevel;

			//�������� ��� (�������� �� ������)
			//level = 0 �> 500; level = 1 �> 450; ... ; level = 8 �> 100.
			int newFallSpeed = 500 - m_level * 50;

			//��������� �������� ��� (����� 100)
			if (newFallSpeed < 100)
			{
				//���� �������� ��� ����� 100, �� ��������� 100
				newFallSpeed = 100;
			}

			//���������� ���� �������� ���
			m_fallSpeed = newFallSpeed;

			//��� ���� � ����� ������ �������
			const HWND WINDOW_HANDLE = GetActiveWindow();

			//������� ������������ �������
			KillTimer(WINDOW_HANDLE, Timer::GAME_TICK);

			//������������ ������ ������� (�� ����� ���� ����������� WM_TIMER)
			SetTimer(WINDOW_HANDLE, Timer::GAME_TICK, m_fallSpeed, 0);
		}
	}
}

void Game::rotateFigure()
{
	//���� ���������� ������
	if (m_currentFigure.color == g_YELLOW_O)
	{
		//�� �� �������� ��
		return;
	}

	//��� ����������� ������
	std::vector<POINT> rotated;

	//��������� �� ��� ������ ������� ������
	for (const auto& CURRENT_BLOCK : m_currentFigure.blocks)
	{
		//�������� ���������� ����� �� 90 ������� � �������� � ��� ����������� ������
		rotated.push_back({ CURRENT_BLOCK.y, -CURRENT_BLOCK.x });
	}

	//�� ������� �������� ������ ==============================
	const Figure TEST = { rotated, m_currentFigure.color }; //������ ��� ��������

	//1. ���� ������ ����� ��������
	if (!checkCollision(TEST, m_currentFigurePosition))
	{
		//2. �� ������ �������� ������
		m_currentFigure.blocks = rotated;
		return;
	}
	else //1. ���� ������ �� ������� ��������
	{
		//������� ������ ������
		const POINT OFFSETS[] =
		{
			{ 0, 0 },	//��� ������
			{ -1, 0 },	//����  �� 1
			{ 1, 0 },	//������ �� 1
			{ -2, 0 },	//����  �� 2
			{ 2, 0 },	//������ �� 2
			{ -3, 0 },	//����  �� 3
			{ 3, 0 }	//������ �� 3
		};

		//2. �� �������� �� ������/����
		for (const auto& CURRENT_OFFSET : OFFSETS)
		{
			//���� ������� ������, ���������� �����
			POINT newPosition =
			{
				m_currentFigurePosition.x + CURRENT_OFFSET.x,
				m_currentFigurePosition.y + CURRENT_OFFSET.y
			};

			//3. � ����� ����������, �� ������� �������� ������
			if (!checkCollision(TEST, newPosition))
			{
				//���������� ���� ������� � �������� ======================
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
	//�� ��������� ������ ���� �������
	const POINT NEW_POSITION = { m_currentFigurePosition.x + X, m_currentFigurePosition.y + Y };

	//���� ������ ���� �������� ���
	if (!checkCollision(m_currentFigure, NEW_POSITION))
	{
		m_currentFigurePosition = NEW_POSITION; //�� ������ ��������� ��

		//���� ������ �������� ����
		if (Y == 1)
		{
			//�� ���������� ���� ���������� ������
			Sound::playMoveSound(false, true, Sound::getEffectsVolume());
		}
	}
	else if (Y == 1) //���� ������ �� ���� �������� ����
	{
		freezeFigure(); //�� ������� ��
	}
}

void Game::quickFall()
{
	//���� ������ ���� ������ ����, �� ���������� ��������
	while (!checkCollision(m_currentFigure, { m_currentFigurePosition.x, m_currentFigurePosition.y + 1 }))
	{
		//���������� ��������� ���������� Y
		++m_currentFigurePosition.y;
	}

	//���������� ����� ������ ������
	Sound::playSpaceSound(false, true, Sound::getEffectsVolume());

	//���� ������ ����� �� ���� �������� ����, �� ������� ��
	freezeFigure();
}

void Game::checkTop9()
{
	//���� ������� ��� ������� ��'�, �� ����� �� ������
	if (m_isEnteringName)
	{
		return;
	}

	//�����, �� ���� �������� ������� ����� ��������� � ���:
	//1.������� ������� �������
	//2.������� ������� ������ ����� 9 ������
	//3.�������� ������� ����� ���������� �������
	if
		(
			m_score->getScoreboard().empty() ||
			m_score->getScoreboard().size() < 9 ||
			m_score->getCurrentScore() > m_score->getScoreboard().back().score
			)
	{
		m_score->setNewRecord(true);				//������� ��������� ����� ������
		m_isEnteringName = true;					//������� ������� ��'�
		m_currentState = UIState::ENTER_NAME;	//���� ����� ���������� �� �������� ����

		//��������� ���� ������� �������� ����� ������
		if (m_flashStartTime == 0)
		{
			m_flashStartTime = GetTickCount64();
		}

		return;
	}

	m_score->setNewRecord(false); //������� �� ��������� ����� ������
}

void Game::afterNameInput()
{
	//������ ����� ������ � ������� �������
	m_score->pushNewRecord();

	//������� ������� �������
	m_score->sortScoreboard();

	//�������� ������� ������� �� 9 ������
	m_score->resizeScoreboard();

	//�������� ������� ������� � .txt ����
	m_score->saveScoreboard();

	m_isEnteringName = false;			//������� ����� �� ������� ��'�
	m_currentState = UIState::PLAYING;	//���� ����� ���������� �� "������� ���"
	m_flashStartTime = 0;				//�������� ���� ������� �������� ����� ������
	m_score->setNewRecord(false);			//������� �� ��������� ����� ������
}

void Game::playEndSound()
{
	if (m_score->isNewRecord()) //���� ��� ������������ ������, �� ��������� ������ ������
	{
		Sound::playHappyEndMusic(false, true, Sound::getMusicVolume());
	}
	else //���� ������� �� ���� �����������, �� ��������� ����� ������
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
