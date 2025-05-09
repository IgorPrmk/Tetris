#pragma once

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include "Common.h"


extern Game g_game;		//��'��� ��� ������ � ����
extern Random g_random;	//��'��� ��� ��������� ���������� �����
extern Board g_board;		//��'��� ��� ������ � ������� �����
extern Score g_score;		//��'��� ��� ������ � ���������
extern Decor g_decor;		//��'��� ��� ������ � ������������ "�����" � �����


//��������������� ������� ==================================
extern const COLORREF g_BLACK;			//������ ����
extern const COLORREF g_WHITE;			//���� ����
extern const COLORREF g_GRAY;			//���� ����
extern const COLORREF g_YELLOW;			//������ ����
extern const COLORREF g_ORANGE;			//������������ ����

extern const COLORREF g_EMPTY_COLOR;	//���� ������� �����
//==========================================================

//����� ����� �������� �� �������� ���� (200 ������)
extern const int g_SCORE_PANEL_WIDTH;

//������ ���� = ������ �������� ���� + ������ ����� �������� �� �������� ����
extern const int g_WINDOW_WIDTH;

//������ ���� = ������ �������� ����
extern const int g_WINDOW_HEIGHT;

//����� ��� ������������ ����� � ���� ("���" �� �����)
extern std::vector<Decor> g_decorFigures;