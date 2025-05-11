#pragma once

#include <windows.h>

//��� ������ � ������� �� ������� =========================
#include <mmsystem.h>				//��� ������� mciSendString()
#pragma comment(lib, "winmm.lib")	//��������� <mmsystem.h>
//==========================================================

#include <string>	//��� ������ � �������

class Sound
{
public:
	//������� ��� �������� ����� � ������� � ����
	static void openMenuMusic();

	//������� ��� ������� ����������� ������ � ����
	static void playMenuMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ������ � ����
	static void stopMenuMusic();

	//������� ��� �������� ����� � ������� � ����
	static void closeMenuMusic();

	//������� ��� �������� ����� � ������� � ���� (����� ������)
	static void openHappyEndMusic();

	//������� ��� ����������� ������ � ���� (����� ������)
	static void playHappyEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ������ � ���� ��� (����� ������)
	static void stopHappyEndMusic();

	//������� ��� �������� ����� � ������� � ���� ��� (����� ������)
	static void closeHappyEndMusic();

	//������� ��� �������� ����� � ������� � ���� ��� (��� �������)
	static void openSadEndMusic();

	//������� ��� ����������� ������ � ���� ��� (��� �������)
	static void playSadEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ������ � ���� ��� (��� �������)
	static void stopSadEndMusic();

	//������� ��� �������� ����� � ������� � ���� ��� (��� �������)
	static void closeSadEndMusic();

	//������� ��� �������� ����� � ������ ���������� ������ ����
	static void openMoveSound();

	//������� ��� ����������� ����� ���������� ������ ����
	static void playMoveSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ����� ���������� ������ ����
	static void stopMoveSound();

	//������� ��� �������� ����� � ������ ���������� ������ ����
	static void closeMoveSound();

	//������� ��� �������� ����� � ������ �������� �����
	static void openLineClearSound();

	//������� ��� ����������� ����� �������� �����
	static void playLineClearSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ����� �������� �����
	static void stopLineClearSound();

	//������� ��� �������� ����� � ������ �������� �����
	static void closeLineClearSound();

	//������� ��� �������� ����� � ������ �������� ������
	static void openSpaceSound();

	//������� ��� ����������� ����� �������� ������
	static void playSpaceSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ���������� ����������� ����� �������� ������
	static void stopSpaceSound();

	//������� ��� �������� ����� � ������ �������� ������
	static void closeSpaceSound();

	//������� ��� ��������� ������� ������
	static int getMusicVolume();

	//������� ��� ��������� ������� ������
	static int getEffectsVolume();

	//������� ��� ���� ������� ������
	static void setMusicVolume(const int VOLUME);

	//������� ��� ���� ������� ������
	static void setEffectsVolume(const int VOLUME);

private:
	//�������������� ����� =====================================
	static const wchar_t* m_MENU_MUSIC_PATH;		//������ � ����
	static const wchar_t* m_HAPPY_END_MUSIC_PATH;	//������ � ���� (����� ������)
	static const wchar_t* m_SAD_END_MUSIC_PATH;		//������ � ���� (��� �������)

	static const wchar_t* m_MOVE_SOUND_PATH;		//���� ����������
	static const wchar_t* m_LINE_CLEAR_SOUND_PATH;	//���� �������� ����
	static const wchar_t* m_SPACE_SOUND_PATH;		//���� �������� ������
	//==========================================================

	//��� ������ � ������� ====================================
	static const wchar_t* m_OPEN_CMD;			//��� �������� ����� � ������
	static const wchar_t* m_CLOSE_CMD;			//��� �������� ����� � ������
	static const wchar_t* m_STOP_CMD;			//��� ������� ����������� �����
	static const wchar_t* m_SEEK_CMD;			//��� ������������� ����� �� �������
	static const wchar_t* m_PLAY_CMD;			//��� ������� ����������� �����
	static const wchar_t* m_PLAY_REPEAT_CMD;	//��� ���������� ����������� �����
	static const wchar_t* m_SET_VOLUME_CMD;		//��� ���� ������� �����

	static wchar_t m_command[300];				//������� ��� ������ � �������
	//==========================================================

	static int m_musicVolume;	//������� ������ (�� 0 �� 100)
	static int m_effectsVolume;	//������� ������ (�� 0 �� 100)

	//�������� ��������/����������� ����� =====================
	static bool m_isMenuMusicOpen;			//�� �������� ���� � ������� � ����

	static bool m_isHappyEndMusicOpen;		//�� �������� ���� � ������� � ���� ��� (����� ������)

	static bool m_isSadEndMusicOpen;		//�� �������� ���� � ������� � ���� ��� (��� �������)

	static bool m_isMoveSoundOpen;			//�� �������� ���� � ������ ���������� ������ ����

	static bool m_isLineClearSoundOpen;		//�� �������� ���� � ������ �������� �����

	static bool m_isSpaceSoundOpen;			//�� �������� ���� � ������ �������� ������
	//==========================================================

private:
	//������� ��� �������� ����� � ������
	static void open(const wchar_t* PATH, const wchar_t* PSEUDO);

	//������� ��� ������������� ����� �� �������
	static void seek(const wchar_t* PSEUDO);

	//������� ��� ���� ������� �����
	static void setVolume(const wchar_t* PSEUDO, const int VOLUME);

	//������� ��� ����������� �����
	static void play(const wchar_t* PSEUDO, const bool REPEAT, const bool SEEK, const int VOLUME);

	//������� ��� ������� ����������� �����
	static void stop(const wchar_t* PSEUDO);

	//������� ��� �������� ����� � ������
	static void close(const wchar_t* PSEUDO);
};
