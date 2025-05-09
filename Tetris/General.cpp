#include "General.h"
#include "Game.h"
#include "Random.h"
#include "Board.h"
#include "Score.h"
#include "Sound.h"
#include "Decor.h"
#include "Figure.h"

Game g_game;	//��'��� ��� ������ � ����
Random g_random;	//��'��� ��� ��������� ���������� �����
Board g_board;	//��'��� ��� ������ � ������� �����
Score g_score;	//��'��� ��� ������ � ���������
Decor g_decor;	//��'��� ��� ������ � ������������ "�����" � �����

//��������������� ������� ==================================
const COLORREF g_BLACK			= RGB(0, 0, 0);			//������ ����
const COLORREF g_WHITE			= RGB(255, 255, 255);	//���� ����
const COLORREF g_GRAY			= RGB(50, 50, 50);		//���� ����
const COLORREF g_YELLOW			= RGB(255, 255, 0);		//������ ����
const COLORREF g_ORANGE			= RGB(255, 128, 0);		//������������ ����

const COLORREF g_EMPTY_COLOR	= RGB(0, 0, 0);			//���� ������� �����
//==========================================================

//����� ����� �������� �� �������� ���� (200 ������)
const int g_SCORE_PANEL_WIDTH = 200;

const int g_WINDOW_WIDTH = Figure::m_BLOCK_SIZE * g_board.m_BOARD_WIDTH + g_SCORE_PANEL_WIDTH;

const int g_WINDOW_HEIGHT = Figure::m_BLOCK_SIZE * g_board.m_BOARD_HEIGHT;

std::vector<Decor> g_decorFigures(g_decor.m_MAX_DECOR_COUNT);