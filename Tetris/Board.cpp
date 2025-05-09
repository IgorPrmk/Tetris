#include "Board.h"
#include "Sound.h"
#include "Score.h"
#include "Game.h"

Board::Board() :
	m_BOARD_WIDTH(10),		//������ �������� ����
	m_BOARD_HEIGHT(20)		//������ �������� ����
{
	m_field.resize
	(
		m_BOARD_HEIGHT, 
		std::vector<COLORREF>(m_BOARD_WIDTH, g_EMPTY_COLOR)
	);
}

const COLORREF& Board::getFieldBlockColor(const int X, const int Y) const
{
	return m_field[Y][X];
}

void Board::setFieldBlockColor(const int X, const int Y, const COLORREF& COLOR)
{
	//������������ ���� ����� � ���
	m_field[Y][X] = COLOR;
}

void Board::clearLines()
{
	//������� �������� �����
	int linesCleared = 0;

	//������ �� ��� ������ �������� ���� ������ ����
	for (int y = 0; y < m_BOARD_HEIGHT;)
	{
		//�� ����� �������� ����������
		bool isFull = true;

		//������ �� ��� ������ � �����
		for (int x = 0; x < m_BOARD_WIDTH; ++x)
		{
			//���� � ����� � ������ ����
			if (m_field[y][x] == g_EMPTY_COLOR)
			{
				//�� ����� �� ����������
				isFull = false;
				break;
			}
		}

		//���� ����� �������� ����������
		if (isFull)
		{
			//��������� ���������� ����� �� ������� 'y' � �������� ����
			m_field.erase(m_field.begin() + y);

			//������ ����� ����� ������ � ������� �������
			m_field.insert(m_field.begin(), std::vector<COLORREF>(m_BOARD_WIDTH, g_EMPTY_COLOR));

			//�������� ������� �������� �����
			++linesCleared;
		}
		else
		{
			//���� ����� �� ����������, �� ���������� �� ���������� �������� �����
			++y;
		}
	}

	//���� � ������� �����
	if (linesCleared > 0)
	{
		//������ ���� �� ������� ����� (100 ���� �� ����� �������� �����)
		g_score.setCurrentScore(g_score.getCurrentScore() + linesCleared * 100);

		//����������� ����� �������� �����
		Sound::playLineClearSound(false, true, Sound::getEffectsVolume());

		//����� ����� ���
		int newLevel = g_score.getCurrentScore() / 1000;

		//���� ����� ����� ��� ����� ���������
		if (newLevel > g_game.m_level)
		{
			//�� ��������� ����� ���
			g_game.m_level = newLevel;

			//�������� ��� (�������� �� ������)
			//level = 0 �> 500; level = 1 �> 450; ... ; level = 8 �> 100.
			int newFallSpeed = 500 - g_game.m_level * 50;

			//��������� �������� ��� (������ 100)
			if (newFallSpeed < 100)
			{
				//���� �������� ��� ����� 100, �� ��������� 100
				newFallSpeed = 100;
			}

			//���������� ���� �������� ���
			g_game.m_fallSpeed = newFallSpeed;

			//��� ���� � ����� ������ �������
			const HWND WINDOW_HANDLE = GetActiveWindow();

			//������� ������������ �������
			KillTimer(WINDOW_HANDLE, Timer::GAME_TICK);

			//������������ ������ ������� (�� ����� ���� ����������� WM_TIMER)
			SetTimer(WINDOW_HANDLE, Timer::GAME_TICK, g_game.m_fallSpeed, 0);
		}
	}
}
