#pragma once

#include "Score.h"
#include "Sound.h"
#include "Random.h"

//���� ����� ==============================================
const COLORREF g_AZURE_I = RGB(0, 255, 255);	//�������� ������		�����: I
const COLORREF g_YELLOW_O = RGB(255, 255, 0);	//����� ������			�����: O
const COLORREF g_PURPLE_T = RGB(139, 0, 255);	//��������� ������		�����: T
const COLORREF g_GREEN_S = RGB(0, 255, 0);		//������ ������			�����: S
const COLORREF g_RED_Z = RGB(255, 0, 0);		//������� ������		�����: Z
const COLORREF g_BLUE_J = RGB(0, 0, 255);		//���� ������			�����: J
const COLORREF g_ORANGE_L = RGB(255, 128, 0);	//����������� ������	�����: L
//==========================================================

//�������������� ������� ==================================
const COLORREF g_BLACK = RGB(0, 0, 0);			//������ ����
const COLORREF g_WHITE = RGB(255, 255, 255);	//���� ����
const COLORREF g_GRAY = RGB(50, 50, 50);		//���� ����
const COLORREF g_YELLOW = RGB(255, 255, 0);		//������ ����
const COLORREF g_ORANGE = RGB(255, 128, 0);		//������������ ����

const COLORREF g_EMPTY_COLOR = RGB(0, 0, 0);	//���� ������� �����
//==========================================================

//������ �������� ���� (��������� ����� �������� �� ������ ������ ����� ������)
const int g_BOARD_WIDTH = 10;	//������ �������� ����
const int g_BOARD_HEIGHT = 20;	//������ �������� ����
//==========================================================

//����� ������ ����� ������ (40*40 ������)
const int g_BLOCK_SIZE = 40;

//����� ����� �������� �� �������� ���� (200 ������)
const int g_SCORE_PANEL_WIDTH = 200;

//������ ���� = ������ �������� ���� + ������ ����� �������� �� �������� ����
const int g_WINDOW_WIDTH = g_BLOCK_SIZE * g_BOARD_WIDTH + g_SCORE_PANEL_WIDTH;

//������ ���� = ������ �������� ����
const int g_WINDOW_HEIGHT = g_BLOCK_SIZE * g_BOARD_HEIGHT;

//����������� ������� ����� � �� (7 �����: I, O, T, S, Z, J, L)
const int g_MAX_NUMBER_FIGURES = 7;

//id �������
enum Timer
{
	DECOR_RAIN = 1, //������ ��� ������������� "����" � �����
	GAME_TICK = 2,	//������ ��� ��� (��������� ����� ���)
};

//���� ����� ��� � ����� ���� ����������
enum class UIState
{
	PLAYING,	//������� ���
	ENTER_NAME,	//������� ������� ��'� ���� �������
	MENU,		//������� ������ ������� ����
	CONTROLS,	//������� ������ ������� ��������� ���� "Controls"
	PAUSED,		//������� ��������� ��� �� �����
	SETTINGS	//������� ������ ������� ��������� ���� "Settings"
};

//����� ������
struct Figure
{
	std::vector<POINT> blocks;	//���������� �������� ����� ������
	COLORREF color = g_GRAY;	//���� ������
};

//���� ��� ��������� ����� ��� ���
class Game
{
public:
	//�����������
	Game();

	//��� ��������� ��������� ������
	Figure getRandomFigure();
	
	//��� ������� ����������� ���
	void resetGame();
	
	//������� ���� ������ �� �������� ��� � ��������, �� �� ������ ����� ���
	void createFigure();
	
	//�� �������� � ���� ������ (true � ���, ��������)
	bool checkCollision(const Figure& FIGURE, const POINT& POSITION);
	
	//����� ������ �� �������� ���, ���� ���� ����� �� ���� �������� ����
	void freezeFigure();
	
	//����� �������� ����� �� �������� ���
	void clearLines();
	
	//��� ��������� ������ �� 90 ������� �� ������������ �������
	//��������������� ��� ��������� �������
	void rotateFigure();

	//��� ���������� ������ �� �������� ����
	void moveFigure(const int X, const int Y);

	//��������� ������ ������ �� ������ ����
	void quickFall();

	//��������, �� ������� ������� ������ � ��� ��������� ����������
	void checkTop9();
	
	//��� ������� ���������� ������� ����
	void afterNameInput();
	
	//��� ���������� ����� ��������� ��� (�����/������ ������)
	void playEndSound();
	
	//��� ��������� �������� ������ ������
	int getFallSpeed() const;

	//��� ��������� ��������� ����� ����������
	UIState getCurrentState() const;

	//��� ��������� ������������ ����� ����������
	UIState getPreviousState() const;

	//���������� �������� ���� ����������
	void setCurrentState(const UIState STATE);

	//���������� ��������� ���� ����������
	void setPreviousState(const UIState STATE);

	//�� ����� ������
	bool isCursorVisible() const;

	//���������� �������� �������
	void setCursorVisible(const bool VISIBLE);

	//�� ��� ���� ��������
	bool isGameOver() const;

	//�� ������� ������� ��'�
	bool isEnteringName() const;

	//�� ��������� ����� ������
	bool isNewRecord() const;

	//�� ��� � ������ (�� ���� �����)
	bool inProgress() const;

	//���������� ���� �������
	void setNewRecord(const bool NEW_RECORD);

	//������� ��� ������� �������� ����� ������
	ULONGLONG getFlashStartTime() const;

	//���������� ��� ������� �������� ����� ������
	void resetFlashStartTime();

	//������� ���� ����� �������� ����
	const COLORREF& getBlockColor(const int X, const int Y) const;

	//������� ���������� ������� ������
	const POINT& getCurrentFigurePosition() const;

	//������� ������� ������
	const Figure& getCurrentFigure() const;

	//������� �������� ������
	const Figure& getNextFigure() const;

	//������� ������� ������� ����
	int getCurrentScore() const;

	//������� ������� ��'� ������
	const std::wstring& getCurrentName() const;

	//������� ������� �������
	const std::vector<ScoreInfo>& getScoreboard() const;

	//���� ����� ������ � ����� ��������� ���� ������
	void addCharName(const wchar_t CHAR);

	//������� ������ � ���� ��������� ���� ������
	void deleteCharName();

	//������� �������� ������ � ������ ��� �����
	const Figure& getFigure(const int FIGURE_TYPE) const;

	//������� ���������� ������� ����
	const POINT& getMousePosition() const { return m_mousePosition; }

	//���������� ���������� ������� ����
	void setMousePosition(const POINT& MOUSE_POSITION) { m_mousePosition = MOUSE_POSITION; }	//���������� ���������� ������� ����

public:
	Random* m_random;							//��'��� ��� ��������� ���������� �����

private:
	UIState m_currentState;						//�������� ���� ����������
	UIState m_previousState;					//��������� ���� ����������
	std::vector<std::vector<COLORREF>> m_board;	//������ ����
	Figure m_currentFigure;						//������� ������� ������
	Figure m_nextFigure;						//�������� ������, ��� �'������� � ��
	POINT m_currentFigurePosition;				//���������� ������� ������� ������
	std::vector<Figure> m_figures;				//����� ��� �������� �����
	ULONGLONG m_flashStartTime;					//���� ����� ������ ������ ������� ����������� ���������
	POINT m_mousePosition;						//������ ���������� ������� { long, long }
	int m_fallSpeed;							//�������� ������ ������
	int m_level;								//����� ��� (�������� �� ������� ����) � �� 0 �� 8

	bool m_isEnteringName; 						//�� ������� � ����� ������ ������� ��� ��'�
	bool m_isCursorVisible;						//�� ������� ������ (�������� �� ��� �������� ����)
	bool m_inProgress;							//�� � ��������� ���� ���
	bool m_isEndSoundPlayed;					//�� ��� ���������� ������ � ���� ���
	bool m_isGameOver;							//�� ��� ��������

	Score* m_score;								//��'��� ��� ������ � ���������
};