#include "Score.h"

Score::Score() :
	m_isNewRecord(false),						//чи гравець встановив новий рекорд
	m_TETRIS_SCORES_FILE("tetris_top9.txt"),	//файл з рекордами
	m_TOP_SIZE(9),								//макс кількість рекордів в таблиці рекордів
	m_MAX_NAME_LENGTH(5)						//макс кількість символів в імені гравця
{
	//завантаження таблиці рекордів з файлу
	loadScoreboard();

	//очищення поточного рекорду
	clearCurrentRecord();
}

void Score::loadScoreboard()
{
	//очищення таблиці рекордів
	m_scoreboard.clear();

	//відкриття файлу з рекордами
	std::ifstream file(m_TETRIS_SCORES_FILE);

	//перевірка, чи файл відкрився
	if (!file.is_open())
	{
		return;
	}

	//тимчасовий зміна для зберігання рекорду
	ScoreInfo scoreInfo;

	//тимчасові змінні для зчитування з файлу
	std::string name;
	int score;

	//поки вдається зчитати ім'я та рахунок з файлу
	while (file >> name >> score)
	{
		scoreInfo.name = std::wstring(name.begin(), name.end());
		scoreInfo.score = score;

		//додаємо запис до таблиці рекордів
		m_scoreboard.push_back(scoreInfo);
	}

	//закриття файлу після зчитування
	file.close();

	//сортування таблиці рекордів за зменшенням очок
	sortScoreboard();

	//зменшення таблиці рекордів до макс кількості записів
	resizeScoreboard();
}

void Score::saveScoreboard()
{
	//відкриття файлу з рекордами
	std::ofstream file(m_TETRIS_SCORES_FILE);

	//перевірка, чи файл відкрився
	if (!file.is_open())
	{
		return;
	}

	//прохід по всім записам в таблиці рекордів
	for (const auto& CURRENT_SCORE : m_scoreboard)
	{
		//перетворення рядка з wstring в string
		std::string name(CURRENT_SCORE.name.begin(), CURRENT_SCORE.name.end());

		//запис в файл: ім'я гравця та його рахунок
		file << name << " " << CURRENT_SCORE.score << std::endl;
	}

	//закриття файлу після збереження
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
	//якщо в таблиці рекордів більше макс кількості записів
	if (m_scoreboard.size() > m_TOP_SIZE)
	{
		//то обрізаємо її до макс кількості записів
		m_scoreboard.resize(m_TOP_SIZE);
	}
}

void Score::addCharName(const wchar_t CHAR)
{
	//якщо в таблиці рекордів довжина імені менше макс кількості символів
	if (m_currentRecord.name.size() < m_MAX_NAME_LENGTH)
	{
		//то додаємо символ до поточного імені гравця
		m_currentRecord.name.push_back(CHAR);
	}
}

void Score::deleteCharName()
{
	//якщо в таблиці рекордів ім'я не пусте
	if (!m_currentRecord.name.empty())
	{
		//то видаляємо останній символ з імені
		m_currentRecord.name.pop_back();
	}
}
