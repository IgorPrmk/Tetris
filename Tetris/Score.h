#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

//���������� ��� ���������� ������
struct ScoreInfo
{
	std::wstring name;	//��'� ������
	int score = 0;		//��������� ��� (������� ����)
};

//���� ��� ������ � ���������
class Score
{
public:
	//�����������
	Score();

	//�������, ��� ��������� ������� ������� � .txt �����
	void loadScoreboard();

	//�������, ��� ������ ������� ������� � .txt ����
	void saveScoreboard();

	//�������, ��� ����� ������� ������� �� ���������� ����
	void sortScoreboard();

	//�������, ��� �������, �� ������� ��������� ����� ������
	bool isNewRecord() const;

	//�������, ��� ����������, �� ������� ��������� ����� ������
	void setNewRecord(const bool IS_NEW_RECORD);

	//�������, ��� ������� �������� ������� ������
	int getCurrentScore() const;

	//�������, ��� ������� ������� ��'� ������
	const std::wstring& getCurrentName() const;

	//�������, ��� ���������� ������� ��'� ������
	void setCurrentName(const std::wstring& NAME);

	//�������, ��� ���������� �������� ������� ������
	void setCurrentScore(const int SCORE);

	//�������, ��� ����� �������� ������
	void clearCurrentRecord();

	//�������, ��� ������� ������� �������
	const std::vector<ScoreInfo>& getScoreboard() const;

	//�������, ��� ���� ����� ������ � ������� �������
	void pushNewRecord();

	//�������, ��� ����� ������� ������� �� ���� ������� ������
	void resizeScoreboard();

	//�������, ��� ���� ������ �� ��������� ���� ������
	void addCharName(const wchar_t CHAR);

	//�������, ��� ������� ������ � ��������� ���� ������
	void deleteCharName();

public:
	const char* m_TETRIS_SCORES_FILE;		//���� � ���������
	const int m_TOP_SIZE;					//���� ������� ������� � ������� �������
	const int m_MAX_NAME_LENGTH; 			//���� ������� ������� � ���� ������

private:
	std::vector<ScoreInfo> m_scoreboard;	//������� �������
	bool m_isNewRecord;						//�� ������� ��������� ����� ������
	ScoreInfo m_currentRecord;				//�������� ������ (�������� ������� � ��'�)
};
