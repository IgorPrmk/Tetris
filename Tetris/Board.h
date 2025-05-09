#pragma once

#include "General.h"

class Board
{
public:
	//�����������
	Board();

	//������� ��� ��������� ������� ����� � ���
	const COLORREF& getFieldBlockColor(const int X, const int Y) const;

	//������� ��� ������������ ������� ����� � ���
	void setFieldBlockColor(const int X, const int Y, const COLORREF& COLOR);

	//������� ����� ��������� ����� �� �������� ���
	void clearLines();

public:
	//������ �������� ���� (��������� ����� �������� �� ������ ������ ����� ������)
	const int m_BOARD_WIDTH;	//������ �������� ����
	const int m_BOARD_HEIGHT;	//������ �������� ����
	//==========================================================

private:
	std::vector<std::vector<COLORREF>> m_field;		//������ ����
};

