#pragma once

class Game;
class Random;
class Board;
class Score;
class Decor;

//id �������
enum Timer
{
	DECOR_RAIN = 1, //������ ��� ������������� "����" � �����
	GAME_TICK = 2, //������ ��� ��� (��������� ����� ���)
};

//���� ������ ��� � ����� ���� ����������
enum class UIState
{
	PLAYING,	//������� ���
	ENTER_NAME,	//������� ������� ��'� ���� �������
	MENU,		//������� ������ ������� ����
	CONTROLS,	//������� ������ ������� ��������� ���� "Controls"
	PAUSED,		//������� ��������� ��� �� �����
	SETTINGS	//������� ������ ������� ��������� ���� "Settings"
};

