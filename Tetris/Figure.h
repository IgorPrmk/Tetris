#pragma once

#include "General.h"

enum FigureType
{
	I,
	O,
	T,
	S,
	Z,
	J,
	L
};

//����, ���� ����� ������
class Figure
{
public:
	//�����������
	Figure();

	//������� ��� ��������� ����������� ������� ����� ������
	static COLORREF getRandomColor();
	
	//������� ��� ��������� ��������� ����� ������
	const std::vector<POINT>& getBlocksPosition() const;

	//������� ��� ������������ ��������� ����� ������
	Figure& setBlocksPosition(const std::vector<POINT>& BLOCKS_POSITION);

	//������� ��� ��������� ��������� ������
	POINT getFigurePosition() const;

	//������� ��� ������������ ��������� ������
	Figure& setFigurePosition(const POINT& POSITION);
	
	//������� ��� ���������� ������ �� �������� ����
	void moveFigure(const int X, const int Y);
	
	//�� �������� � ���� ������ (true � ���, ��������)
	bool checkCollision();

	//������� ��� ��������� ������ �� 90 ������� �� ������������ �������
	void rotateFigure();
	
	//������� ��� ��������� ������ �� 90 ������� TIMES-���� �� ������������ �������
	//��������������� ��� ������������� "����" � ����� � ����
	void rotateFigure(const int TIMES);
	
	//������� ����� ������ �� �������� ���, ���� ���� ����� �� ���� �������� ����
	void freezeFigure();
	
	//�������, ��� ��������� ������ ������ �� ������ ����
	void quickFall();
	
	//������� ��� ������������ ������� ������
	Figure& setColor(const COLORREF& COLOR);

	//������� ��� ��������� ������� ������
	const COLORREF& getColor() const;

public:
	//���� ����� ==============================================
	static const COLORREF m_AZURE_I		= RGB(0, 255, 255);	//�������� ������		�����: I
	static const COLORREF m_YELLOW_O	= RGB(255, 255, 0);	//����� ������			�����: O
	static const COLORREF m_PURPLE_T	= RGB(139, 0, 255);	//��������� ������		�����: T
	static const COLORREF m_GREEN_S		= RGB(0, 255, 0);	//������ ������			�����: S
	static const COLORREF m_RED_Z		= RGB(255, 0, 0);	//������� ������		�����: Z
	static const COLORREF m_BLUE_J		= RGB(0, 0, 255);	//���� ������			�����: J
	static const COLORREF m_ORANGE_L	= RGB(255, 128, 0);	//����������� ������	�����: L
	//==========================================================
	
	//����������� ������� ����� � �� (7 �����: I, O, T, S, Z, J, L)
	static const int m_MAX_NUMBER_FIGURES = 7;

	//�� �������� ������� ��� �����
	static const COLORREF m_COLORS[m_MAX_NUMBER_FIGURES];

	//����� ������ ����� ������ (40*40 ������)
	static const int m_BLOCK_SIZE = 40;
	
private:
	std::vector<POINT> m_blocks;	//���������� �������� ����� ������
	COLORREF m_color;				//���� ������
	POINT m_position;				//���������� ������
};

