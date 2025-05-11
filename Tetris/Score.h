#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

//інформація про збережений рекорд
struct ScoreInfo
{
	std::wstring name;	//ім'я гравця
	int score = 0;		//результат гри (кількість очок)
};

//клас для роботи з рекордами
class Score
{
public:
	//конструктор
	Score();

	//функція, яка завантажує таблицю рекордів з .txt файлу
	void loadScoreboard();

	//функція, яка зберігає таблицю рекордів в .txt файл
	void saveScoreboard();

	//функція, яка сортує таблицю рекордів за зменшенням очок
	void sortScoreboard();

	//функція, яка повертає, чи гравець встановив новий рекорд
	bool isNewRecord() const;

	//функція, яка встановлює, чи гравець встановив новий рекорд
	void setNewRecord(const bool IS_NEW_RECORD);

	//функція, яка повертає поточний рахунок гравця
	int getCurrentScore() const;

	//функція, яка повертає поточне ім'я гравця
	const std::wstring& getCurrentName() const;

	//функція, яка встановлює поточне ім'я гравця
	void setCurrentName(const std::wstring& NAME);

	//функція, яка встановлює поточний рахунок гравця
	void setCurrentScore(const int SCORE);

	//функція, яка очищає поточний рекорд
	void clearCurrentRecord();

	//функція, яка повертає таблицю рекордів
	const std::vector<ScoreInfo>& getScoreboard() const;

	//функція, яка додає новий рекорд в таблицю рекордів
	void pushNewRecord();

	//функція, яка обрізає таблицю рекордів до макс кількості записів
	void resizeScoreboard();

	//функція, яка додає символ до поточного імені гравця
	void addCharName(const wchar_t CHAR);

	//функція, яка видаляє символ з поточного імені гравця
	void deleteCharName();

public:
	const char* m_TETRIS_SCORES_FILE;		//файл з рекордами
	const int m_TOP_SIZE;					//макс кількість рекордів в таблиці рекордів
	const int m_MAX_NAME_LENGTH; 			//макс кількість символів в імені гравця

private:
	std::vector<ScoreInfo> m_scoreboard;	//таблиця рекордів
	bool m_isNewRecord;						//чи гравець встановив новий рекорд
	ScoreInfo m_currentRecord;				//поточний рекорд (поточний рахунок і ім'я)
};
