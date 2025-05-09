#include "Figure.h"
#include "Board.h"
#include "Game.h"
#include "Sound.h"

Figure::Figure() :
	m_color(g_GRAY)				//���� ������
{
	//������������ ������� ������ �� �������� ��� ============
	m_position.x = g_board.m_BOARD_WIDTH / 2 - 2;	//+- �� ������
	m_position.y = 0;								//������ ������� ����
	//==========================================================
}

const COLORREF Figure::m_COLORS[m_MAX_NUMBER_FIGURES] =
{
	Figure::m_AZURE_I,
	Figure::m_YELLOW_O,
	Figure::m_PURPLE_T,
	Figure::m_GREEN_S,
	Figure::m_RED_Z,
	Figure::m_BLUE_J,
	Figure::m_ORANGE_L
};

COLORREF Figure::getRandomColor()
{
	//��������� ����������� ������� �� 0 �� 6
	const int RANDOM_INDEX = g_random.getRandomInt(0, m_MAX_NUMBER_FIGURES - 1);

	return m_COLORS[RANDOM_INDEX];
}

const std::vector<POINT>& Figure::getBlocksPosition() const
{
	//��������� ���������� ������
	return m_blocks;
}

Figure& Figure::setBlocksPosition(const std::vector<POINT>& BLOCKS_POSITION)
{
	if (BLOCKS_POSITION.size() != 4) //���� ������ �� ���������� � 4 �����
	{
		//�� ������ �� ������
		return *this;
	}

	//���������� ���� ���������� ����� ������
	m_blocks = BLOCKS_POSITION;

	return *this;
}

POINT Figure::getFigurePosition() const
{
	return m_position;
}

Figure& Figure::setFigurePosition(const POINT& POSITION)
{
	m_position = POSITION;

	return *this;
}

void Figure::moveFigure(const int X, const int Y)
{
	//�� ��������� ������ ���� �������
	const POINT NEW_POSITION = { m_position.x + X, m_position.y + Y };

	Figure testFigure = *this; //������� ������ ��� ��������

	//������ ������� ��� ������� ������
	testFigure.setFigurePosition(NEW_POSITION); //������ ���������� ����� ������

	//���� ������ ���� �������� ���
	if (!testFigure.checkCollision())
	{
		m_position = NEW_POSITION; //�� ������ ��������� ��

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

bool Figure::checkCollision()
{
	//��������� �� ��� ������ ������
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//��������� ���������� ����� (������� �� �������� ���) ====
		const int ABSOLUTE_X = m_position.x + CURRENT_BLOCK.x; //��������� ���������� �� �� X
		const int ABSOLUTE_Y = m_position.y + CURRENT_BLOCK.y; //��������� ���������� �� �� Y
		//==========================================================

		if //��������, �� �������� ���� �� ��� ���� ��� �������� � ���� �����
		(
			(ABSOLUTE_X < 0 || ABSOLUTE_X >= g_board.m_BOARD_WIDTH)	||
			(ABSOLUTE_Y < 0 || ABSOLUTE_Y >= g_board.m_BOARD_HEIGHT)	||
			(g_board.getFieldBlockColor(ABSOLUTE_X, ABSOLUTE_Y) != g_EMPTY_COLOR)
		)
		{
			return true; //���, ������ ��������
		}
	}

	return false; //��, ������ �� ��������
}

void Figure::rotateFigure()
{
	//���� ���������� ������
	if (m_color == m_YELLOW_O)
	{
		//�� �� �������� ��
		return;
	}

	//���������� ����� ��� ����������� ������
	std::vector<POINT> rotated;

	//��������� �� ��� ������ ������� ������
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//�������� ���������� ����� �� 90 ������� � �������� � ��� ����������� ������
		rotated.push_back({ CURRENT_BLOCK.y, -CURRENT_BLOCK.x });
	}

	//������� ������ ��� ��������
	Figure testFigure = *this; 

	//������ ������� ��� ������� ������
	testFigure.setBlocksPosition(rotated); 

	//1. ���� ������� ������ ����� ��������
	if (!testFigure.checkCollision())
	{
		//2. �� ������ �������� ������
		m_blocks = rotated;
		return;
	}
	else //1. ���� ������� ������ �� ������� ��������
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
				m_position.x + CURRENT_OFFSET.x,
				m_position.y + CURRENT_OFFSET.y
			};

			//������ ���� ������� ������
			testFigure.setFigurePosition(newPosition); 

			//3. � ����� ����������, �� ������� �������� ������� ������
			if (!testFigure.checkCollision())
			{
				//���� ������� ������ ����� ��������
				//�� ���������� ����������� ����� ���� ������� � ��������
				m_position	= newPosition;
				m_blocks	= rotated;
				//==========================================================

				return;
			}
		}
	}
	//==========================================================
}

void Figure::rotateFigure(const int TIMES)
{
	for (int i = 0; i < TIMES; ++i)
	{
		//�������� ���������� �����
		std::vector<POINT> tempBlocks;

		//��������� �� ��� ������ ������
		for (const auto& block : m_blocks)
		{
			//�������� ���������� ����� �� 90 ������� � �������� �� �������� ����������
			tempBlocks.push_back({ block.y, -block.x });
		}

		//��������� ���������� ����� ������
		m_blocks = tempBlocks;
	}
}

void Figure::freezeFigure()
{
	//��������� �� ��� ������ ������
	for (const auto& CURRENT_BLOCK : m_blocks)
	{
		//��������� ���������� ����� (������� �� �������� ���) ====
		const int ABSOLUTE_X = m_position.x + CURRENT_BLOCK.x; //��������� ���������� �� �� X
		const int ABSOLUTE_Y = m_position.y + CURRENT_BLOCK.y; //��������� ���������� �� �� Y
		//==========================================================

		if //���� ���� �� �������� �� ��� ����
			(
				(ABSOLUTE_X >= 0 && ABSOLUTE_X < g_board.m_BOARD_WIDTH) &&
				(ABSOLUTE_Y >= 0 && ABSOLUTE_Y < g_board.m_BOARD_HEIGHT)
				)
		{
			//�� �����'������� ���� ���� �� �������� ���
			g_board.setFieldBlockColor(ABSOLUTE_X, ABSOLUTE_Y, m_color);
		}

	}

	//�������� ���������� ����� �� �������� ���
	g_board.clearLines();

	//��������� ���� ������
	g_game.createFigure();
}

void Figure::quickFall()
{
	//������� ������ ��� ��������
	Figure testFigure = *this;

	//���� ������ ���� ������ ����, �� ���������� ��������
	while (!testFigure.checkCollision())
	{
		++testFigure.m_position.y; //��������� ������� ������ ����
	}

	//���������� ���� ������� ����������� �����
	*this = testFigure;

	//���������� ����� ������ ������
	Sound::playSpaceSound(false, true, Sound::getEffectsVolume());

	//���� ������ ����� �� ���� �������� ����, �� ������� ��
	freezeFigure();
}

Figure& Figure::setColor(const COLORREF& COLOR)
{
	m_color = COLOR;

	return *this;
}

const COLORREF& Figure::getColor() const
{
	return m_color;
}
