#include "Score.h"

Score::Score() :
	m_isNewRecord(false),						//�� ������� ��������� ����� ������
	m_TETRIS_SCORES_FILE("tetris_top9.txt"),	//���� � ���������
	m_TOP_SIZE(9),								//���� ������� ������� � ������� �������
	m_MAX_NAME_LENGTH(5)						//���� ������� ������� � ���� ������
{
	//������������ ������� ������� � �����
	loadScoreboard();

	//�������� ��������� �������
	clearCurrentRecord();
}

void Score::loadScoreboard()
{
	//�������� ������� �������
	m_scoreboard.clear();

	//�������� ����� � ���������
	std::ifstream file(m_TETRIS_SCORES_FILE);

	//��������, �� ���� ��������
	if (!file.is_open())
	{
		return;
	}

	//���������� ���� ��� ��������� �������
	ScoreInfo scoreInfo;

	//�������� ���� ��� ���������� � �����
	std::string name;
	int score;

	//���� ������� ������� ��'� �� ������� � �����
	while (file >> name >> score)
	{
		scoreInfo.name = std::wstring(name.begin(), name.end());
		scoreInfo.score = score;

		//������ ����� �� ������� �������
		m_scoreboard.push_back(scoreInfo);
	}

	//�������� ����� ���� ����������
	file.close();

	//���������� ������� ������� �� ���������� ����
	sortScoreboard();

	//��������� ������� ������� �� ���� ������� ������
	resizeScoreboard();
}

void Score::saveScoreboard()
{
	//�������� ����� � ���������
	std::ofstream file(m_TETRIS_SCORES_FILE);

	//��������, �� ���� ��������
	if (!file.is_open())
	{
		return;
	}

	//������ �� ��� ������� � ������� �������
	for (const auto& CURRENT_SCORE : m_scoreboard)
	{
		//������������ ����� � wstring � string
		std::string name(CURRENT_SCORE.name.begin(), CURRENT_SCORE.name.end());

		//����� � ����: ��'� ������ �� ���� �������
		file << name << " " << CURRENT_SCORE.score << std::endl;
	}

	//�������� ����� ���� ����������
	file.close();
}

void Score::sortScoreboard()
{
	sort
	(
		m_scoreboard.begin(), m_scoreboard.end(),
		[](const ScoreInfo& a, const ScoreInfo& b)
		{
			return a.score > b.score;
		}
	);
}

bool Score::isNewRecord() const
{
	return m_isNewRecord;
}

void Score::setNewRecord(const bool IS_NEW_RECORD)
{
	m_isNewRecord = IS_NEW_RECORD;
}

int Score::getCurrentScore() const
{
	return m_currentRecord.score;
}

const std::wstring& Score::getCurrentName() const
{
	return m_currentRecord.name;
}

void Score::setCurrentName(const std::wstring& NAME)
{
	m_currentRecord.name = NAME;
}

void Score::setCurrentScore(const int SCORE)
{
	m_currentRecord.score = SCORE;
}

void Score::clearCurrentRecord()
{
	m_currentRecord.name.clear();
	m_currentRecord.score = 0;
}

const std::vector<ScoreInfo>& Score::getScoreboard() const
{
	return m_scoreboard;
}

void Score::pushNewRecord()
{
	m_scoreboard.push_back(m_currentRecord);
}

void Score::resizeScoreboard()
{
	//���� � ������� ������� ����� ���� ������� ������
	if (m_scoreboard.size() > m_TOP_SIZE)
	{
		//�� ������� �� �� ���� ������� ������
		m_scoreboard.resize(m_TOP_SIZE);
	}
}

void Score::addCharName(const wchar_t CHAR)
{
	//���� � ������� ������� ������� ���� ����� ���� ������� �������
	if (m_currentRecord.name.size() < m_MAX_NAME_LENGTH)
	{
		//�� ������ ������ �� ��������� ���� ������
		m_currentRecord.name.push_back(CHAR);
	}
}

void Score::deleteCharName()
{
	//���� � ������� ������� ��'� �� �����
	if (!m_currentRecord.name.empty())
	{
		//�� ��������� ������� ������ � ����
		m_currentRecord.name.pop_back();
	}
}
