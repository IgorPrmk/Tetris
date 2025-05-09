#include "Decor.h"
#include "Random.h" 	//��� ��������� ���������� �����
#include "Game.h"		//��� ������ � ����

Decor::Decor()
{
	initDecorRain();
}

Decor& Decor::operator=(const Decor& other)
{
	if (this != &other) // �������� �� ����������������
	{
		m_figure = other.m_figure; // �������� ������ Figure
		m_x = other.m_x;           // �������� ���������� X
		m_y = other.m_y;           // �������� ���������� Y
		m_speed = other.m_speed;   // �������� ��������
	}

	return *this;
}

void Decor::initDecorRain()
{
	//������� ������ ������������ �����
	g_decorFigures.clear();

	//������� ������������ �����
	g_decorFigures.resize(m_MAX_DECOR_COUNT);

	//���� ����������� ������
	Decor decore;

	//���������� ������ ����������� ������������� ��������
	for (int i = 0; i < m_MAX_DECOR_COUNT; ++i)
	{
		//��������� ������
		decore.m_figure = g_game.getRandomFigure();

		//��������� ��������� ������ ===============================
		//��������� ����������� ��������� ������ �� 0 �� 3
		const int RANDOM_ROTATION = g_random.getRandomInt(0, 3);
		decore.m_figure.rotateFigure(RANDOM_ROTATION);
		//==========================================================

		//��������� ������� ���� ������ ============================
		decore.m_x = g_random.getRandomDouble(0.0, g_WINDOW_WIDTH);
		decore.m_y = g_random.getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
		//==========================================================

		//��������� �������� ������
		decore.m_speed = g_random.getRandomDouble(1.5, 2.5);

		//���������� ������ �����
		g_decorFigures[i] = decore;
	}
}

void Decor::updateDecorRain()
{
	for (auto& currentDecoreFigure : g_decorFigures)
	{
		//���������� ������ ����
		currentDecoreFigure.m_y += currentDecoreFigure.m_speed * 10.0;

		//���� ������ ������ �� ��� ������
		if (currentDecoreFigure.m_y > g_WINDOW_HEIGHT + 40)
		{
			//��������� ������
			currentDecoreFigure.m_figure = g_game.getRandomFigure();

			//��������� ��������� ������ ===============================
			//��������� ����������� ��������� ������ �� 0 �� 3
			const int RANDOM_ROTATION = g_random.getRandomInt(0, 3);
			currentDecoreFigure.m_figure.rotateFigure(RANDOM_ROTATION);
			//==========================================================

			//��������� ������� ���� ������ ============================
			currentDecoreFigure.m_x = g_random.getRandomDouble(0.0, g_WINDOW_WIDTH);
			currentDecoreFigure.m_y = g_random.getRandomDouble(-g_WINDOW_HEIGHT, 0.0);
			//==========================================================

			//��������� �������� ������
			currentDecoreFigure.m_speed = g_random.getRandomDouble(1.5, 2.5);
		}
	}
}

void Decor::drawDecorRain(const HDC DEVICE_CONTEXT)
{
	//����� ����� ����������� ������
	const int BLOCK_SIZE = 20;

	for (const auto& CURRENT_DECORE_FIGURE : g_decorFigures) //������ ��� ������������ �����
	{
		for (const auto& CURRENT_BLOCK : CURRENT_DECORE_FIGURE.m_figure.getBlocksPosition()) //������ ��� ����� ������
		{
			//��������� ���������� ����� (������� �� �������� ���) ====
			const int ABSOLUTE_X = CURRENT_DECORE_FIGURE.m_x + CURRENT_BLOCK.x * BLOCK_SIZE; //��������� ���������� �� �� X
			const int ABSOLUTE_Y = CURRENT_DECORE_FIGURE.m_y + CURRENT_BLOCK.y * BLOCK_SIZE; //��������� ���������� �� �� Y
			//==========================================================

			//��������� ����� ������ ===================================
			//��������� ������ � �������� �����
			const HBRUSH BRUSH = CreateSolidBrush(CURRENT_DECORE_FIGURE.m_figure.getColor()); //���� �������� ��������� ����

			//�� ��������� ��������
			const RECT BLOCK_RECTANGLE =
			{
				ABSOLUTE_X,				//���������� ��� �������
				ABSOLUTE_Y,				//���������� ������� �������
				ABSOLUTE_X + BLOCK_SIZE,	//���������� ����� �������
				ABSOLUTE_Y + BLOCK_SIZE	//���������� ������ �������
			};

			FillRect(DEVICE_CONTEXT, &BLOCK_RECTANGLE, BRUSH); //���������� ������������ ��������
			DeleteObject(BRUSH); //��������� ������ (���������� ���'��)
			//==========================================================

			//��������� ������� ����� ==================================

			//��������� ������� (�����) : ���, ������� � ���� �������
			const HPEN PEN = CreatePen(PS_SOLID, 1, g_GRAY);

			//���� �������
			SelectObject(DEVICE_CONTEXT, PEN);

			//���� ������� (������) : �������� (��� �������)
			SelectObject(DEVICE_CONTEXT, GetStockObject(NULL_BRUSH));

			//��������� ������� �����
			Rectangle
			(
				DEVICE_CONTEXT,
				ABSOLUTE_X,
				ABSOLUTE_Y,
				ABSOLUTE_X + BLOCK_SIZE,
				ABSOLUTE_Y + BLOCK_SIZE
			);

			//��������� �������
			DeleteObject(PEN);
			//==========================================================
		}
	}
}