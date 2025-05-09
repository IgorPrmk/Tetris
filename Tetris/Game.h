#pragma once

#include "Common.h"    // ����� ������ UIState
#include "Board.h"
#include "Figure.h"
#include "Score.h"
#include "Random.h"
#include "Sound.h"

//���� ��� ��������� ����� ��� ���
class Game
{
public:
	UIState m_currentState;							//�������� ���� ����������
	UIState m_previousState;						//���������� ���� ����������

	Figure m_currentFigure;							//������� ������� ������	
	Figure m_nextFigure;							//�������� ������, ��� �'������� � ��
	std::vector<Figure> m_figures;					//����� ��� �������� �����

	bool m_isGameOver;								//�� ��� ��������
	bool m_isEnteringName; 							//�� ������� � ����� ������ ������� ��� ��'�
	bool m_isCursorVisible;							//�� ������� ������ (�������� �� ��� �������� �����)
	bool m_inProgress;								//�� � ��������� ���� ���
	
	bool m_isEndSoundPlayed;						//�� ��� ���������� ������ � ���� ���
	ULONGLONG m_flashStartTime;						//���� ����� ������ ������ ������� ����������� ���������

	//��������� �������� ��� ==================================
	int m_fallSpeed;	//�������� ������ ������
	int m_level;		//����� ��� (�������� �� ������� ����) � �� 0 �� 8
	//==========================================================

	Game() :
		m_currentState(UIState::MENU),	//���� �������
		m_previousState(UIState::MENU),	//���������� ���� ����
		m_isGameOver(false),			//��� �� ��������
		m_isEnteringName(false), 		//������� �� ������� ��'�
		m_isCursorVisible(false), 		//������ �� ��������
		m_inProgress(false),			//������� �� ���
		m_fallSpeed(500), 				//�������� ��� 500 (���������� �����)
		m_level(0),						//������������ ����������� ����
		m_isEndSoundPlayed(false),		//������ � ���� ��� �� �� ������������
		m_flashStartTime(0)				//�������� ���� ������� �������� ����� ������
	{
		//��������� �����
		m_figures[FigureType::I].setColor(Figure::m_AZURE_I).setBlocksPosition(	{ { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } });
		m_figures[FigureType::O].setColor(Figure::m_YELLOW_O).setBlocksPosition({ { 0, 0 },{ 1, 0 },{ 0, 1 },{ 1, 1 } });
		m_figures[FigureType::T].setColor(Figure::m_PURPLE_T).setBlocksPosition({ { 0, 0 },{ 1, 0 },{ 2, 0 },{ 1, 1 } });
		m_figures[FigureType::S].setColor(Figure::m_GREEN_S).setBlocksPosition(	{ { 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 } });
		m_figures[FigureType::Z].setColor(Figure::m_RED_Z).setBlocksPosition(	{ { 0, 0 },{ 1, 0 },{ 1, 1 },{ 2, 1 } });
		m_figures[FigureType::J].setColor(Figure::m_BLUE_J).setBlocksPosition(	{ { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 2 } });
		m_figures[FigureType::L].setColor(Figure::m_ORANGE_L).setBlocksPosition({ { 1, 0 },{ 1, 1 },{ 1, 2 },{ 0, 2 } });

		//��������� �������� ��������� ������
		m_nextFigure = getRandomFigure();
	}

	//������� ��� ��������� ��������� ������
	Figure getRandomFigure()
	{
		//��������� ����������� ������� �� 0 �� 6
		const int RANDOM_INDEX = g_random.getRandomInt(0, Figure::m_MAX_NUMBER_FIGURES - 1);

		//���������� ��������� ������ � ������
		return m_figures[RANDOM_INDEX];
	}

	//������� ��� ������� ����������� ���
	void resetGame()
	{
		//��������� �� ��� ������ �������� ���� � ���������� �� "��������" ��������
		for (int y = 0; y < g_board.m_BOARD_HEIGHT; ++y)
		{
			for (int x = 0; x < g_board.m_BOARD_WIDTH; ++x)
			{
				g_board.setFieldBlockColor(x, y, g_EMPTY_COLOR);
			}
		}

		g_score.clearCurrentRecord();			//������� ��������� �������
		g_score.setNewRecord(false);			//������� �� ��������� ����� ������
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

	//������� ������� ���� ������ �� �������� ��� � ��������, �� �� ������ ����� ���
	void createFigure()
	{
		//������ ���������� ����������� ������
		m_currentFigure = m_nextFigure;

		//�������� ���� �������� ������
		m_nextFigure = getRandomFigure();

		//���� ������ � ���� ��������, �� ��� ����������
		if (m_currentFigure.checkCollision())
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

	//������� ��������, �� ������� ������� ������ � ��� ��������� ����������
	void checkTop9()
	{
		//���� ������� ��� ������� ��'�, �� ������ �� ������
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
				g_score.getScoreboard().empty() ||
				g_score.getScoreboard().size() < 9 ||
				g_score.getCurrentScore() > g_score.getScoreboard().back().score
				)
		{
			g_score.setNewRecord(true);				//������� ��������� ����� ������
			m_isEnteringName = true;					//������� ������� ��'�
			m_currentState = UIState::ENTER_NAME;	//���� ����� ���������� �� �������� �����

			//��������� ���� ������� �������� ����� ������
			if (m_flashStartTime == 0)
			{
				m_flashStartTime = GetTickCount64();
			}

			return;
		}

		g_score.setNewRecord(false); //������� �� ��������� ����� ������
	}

	//������� ��� ������� ���������� ������� �����
	void afterNameInput()
	{
		//������ ����� ������ � ������� �������
		g_score.pushNewRecord();

		//������� ������� �������
		g_score.sortScoreboard();

		//�������� ������� ������� �� 9 ������
		g_score.resizeScoreboard();

		//�������� ������� ������� � .txt ����
		g_score.saveScoreboard();

		m_isEnteringName = false;			//������� ����� �� ������� ��'�
		m_currentState = UIState::PLAYING;	//���� ����� ���������� �� "������� ���"
		m_flashStartTime = 0;				//�������� ���� ������� �������� ����� ������
		g_score.setNewRecord(false);			//������� �� ��������� ����� ������
	}

	//������� ��� ���������� ����� ��������� ��� (�����/������ ������)
	void playEndSound()
	{
		if (g_score.isNewRecord()) //���� ��� ������������ ������, �� ��������� ������ ������
		{
			Sound::playHappyEndMusic(false, true, Sound::getMusicVolume());
		}
		else //���� ������� �� ���� �����������, �� ��������� ����� ������
		{
			Sound::playSadEndMusic(false, true, Sound::getMusicVolume());
		}
	}
};

