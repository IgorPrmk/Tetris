#include "Sound.h"

//�������������� ����� =====================================
const wchar_t* Sound::m_MENU_MUSIC_PATH			= L"music/tetrisMusic.mp3";	//������ � ����
const wchar_t* Sound::m_HAPPY_END_MUSIC_PATH	= L"music/happyEnd.mp3";	//������ � ���� (����� ������)
const wchar_t* Sound::m_SAD_END_MUSIC_PATH		= L"music/sadEnd.mp3";		//������ � ���� (��� �������)

const wchar_t* Sound::m_MOVE_SOUND_PATH			= L"music/tetrisMove.wav";	//���� ����������
const wchar_t* Sound::m_LINE_CLEAR_SOUND_PATH	= L"music/raw.wav";			//���� �������� ����
const wchar_t* Sound::m_SPACE_SOUND_PATH		= L"music/space.wav";		//���� �������� ������
//==========================================================

//��� ������ � ������� ====================================
const wchar_t* Sound::m_OPEN_CMD		= L"open \"%ls\" type mpegvideo alias %ls";	//��� �������� ����� � ������
const wchar_t* Sound::m_CLOSE_CMD		= L"close %ls";								//��� �������� ����� � ������
const wchar_t* Sound::m_STOP_CMD		= L"stop %ls";								//��� ������� ����������� �����
const wchar_t* Sound::m_SEEK_CMD		= L"seek %ls to start";						//��� ������������� ����� �� �������
const wchar_t* Sound::m_PLAY_CMD		= L"play %ls";								//��� ������� ����������� �����
const wchar_t* Sound::m_PLAY_REPEAT_CMD = L"play %ls repeat";						//��� ���������� ����������� �����
const wchar_t* Sound::m_SET_VOLUME_CMD	= L"setaudio %ls volume to %d";				//��� ���� ������� �����
//==========================================================

wchar_t Sound::m_command[300]	= { 0 };	//������� ��� ������ � �������

int Sound::m_musicVolume		= 50;		//������� ������ (�� 0 �� 100)
int Sound::m_effectsVolume		= 50;		//������� ������ (�� 0 �� 100)

//�������� ��������/����������� ����� ====================
bool Sound::m_isMenuMusicOpen			= false; //�� �������� ���� � ������� � ����

bool Sound::m_isHappyEndMusicOpen		= false; //�� �������� ���� � ������� � ���� (����� ������)

bool Sound::m_isSadEndMusicOpen			= false; //�� �������� ���� � ������� � ���� ��� (��� �������)

bool Sound::m_isMoveSoundOpen			= false; //�� �������� ���� � ������ ���������� ������ ����

bool Sound::m_isLineClearSoundOpen		= false; //�� �������� ���� � ������ �������� �����

bool Sound::m_isSpaceSoundOpen			= false; //�� �������� ���� � ������ �������� ������
//==========================================================

void Sound::open(const wchar_t* PATH, const wchar_t* PSEUDO)
{
	//��������� ������������ �������
	swprintf_s(m_command, m_OPEN_CMD, PATH, PSEUDO);

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::seek(const wchar_t* PSEUDO)
{
	//��������� ������������ �������
	swprintf_s(m_command, m_SEEK_CMD, PSEUDO);

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::setVolume(const wchar_t* PSEUDO, const int VOLUME)
{
	//��������� ������������ �������
	//mciSendString ������ �� 0 �� 1000, ���� ������� �� 10
	swprintf_s(m_command, m_SET_VOLUME_CMD, PSEUDO, VOLUME * 10);

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::play(const wchar_t* PSEUDO, const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//������������ ������� �����
	setVolume(PSEUDO, VOLUME); 

	if (SEEK) //���� ������� ������������� �� �������
	{
		//�� ������������� �� �������
		seek(PSEUDO);
	}
	
	if (REPEAT) //���� ������� ����� �������� �����������
	{
		//��������� ������������ �������
		swprintf_s(m_command, m_PLAY_REPEAT_CMD, PSEUDO);
	}
	else //���� �� ������� �������� �����������
	{
		//��������� ������������ �������
		swprintf_s(m_command, m_PLAY_CMD, PSEUDO);
	}

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::stop(const wchar_t* PSEUDO)
{
	//��������� ������������ �������
	swprintf_s(m_command, m_STOP_CMD, PSEUDO);

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::close(const wchar_t* PSEUDO)
{
	//��������� ������������ �������
	swprintf_s(m_command, m_CLOSE_CMD, PSEUDO);

	//����������� �������
	mciSendString(m_command, 0, 0, 0);
}

void Sound::openMenuMusic()
{
	//���� ���� � ������� � ���� �� ��������
	if (!m_isMenuMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_MENU_MUSIC_PATH, L"menuMusic"); 

		//� �������, �� ���� � ������� � ���� ��������
		m_isMenuMusicOpen = true;
	}
}

void Sound::playMenuMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������� � ���� �� ��������
	if (!m_isMenuMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_MENU_MUSIC_PATH, L"menuMusic");

		//� �������, �� ���� � ������� � ���� ��������
		m_isMenuMusicOpen = true;
	}

	play(L"menuMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopMenuMusic()
{
	//���� ���� � ������� � ���� ��������
	if (m_isMenuMusicOpen)
	{
		stop(L"menuMusic");
	}
}

void Sound::closeMenuMusic()
{
	//���� ���� � ������� � ���� ��������
	if (m_isMenuMusicOpen)
	{
		stop(L"menuMusic");

		//�� ��������� ��� ����
		close(L"menuMusic");

		//� �������, �� ���� � ������� � ���� ��������
		m_isMenuMusicOpen = false;
	}
}

void Sound::openHappyEndMusic()
{
	//���� ���� � ������� � ���� ��� �� ��������
	if (!m_isHappyEndMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_HAPPY_END_MUSIC_PATH, L"happyEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isHappyEndMusicOpen = true;
	}
}

void Sound::playHappyEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������� � ���� ��� �� ��������
	if (!m_isHappyEndMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_HAPPY_END_MUSIC_PATH, L"happyEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isHappyEndMusicOpen = true;
	}

	play(L"happyEndMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopHappyEndMusic()
{
	//���� ���� � ������� � ���� ��� ��������
	if (m_isHappyEndMusicOpen)
	{
		stop(L"happyEndMusic");
	}
}

void Sound::closeHappyEndMusic()
{
	//���� ���� � ������� � ���� ��� ��������
	if (m_isHappyEndMusicOpen)
	{
		stop(L"happyEndMusic");

		//�� ��������� ��� ����
		close(L"happyEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isHappyEndMusicOpen = false;
	}
}

void Sound::openSadEndMusic()
{
	//���� ���� � ������� � ���� ��� �� ��������
	if (!m_isSadEndMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_SAD_END_MUSIC_PATH, L"sadEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isSadEndMusicOpen = true;
	}
}

void Sound::playSadEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������� � ���� ��� �� ��������
	if (!m_isSadEndMusicOpen)
	{
		//�� ��������� ��� ����
		open(m_SAD_END_MUSIC_PATH, L"sadEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isSadEndMusicOpen = true;
	}

	play(L"sadEndMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopSadEndMusic()
{
	//���� ���� � ������� � ���� ��� ��������
	if (m_isSadEndMusicOpen)
	{
		stop(L"sadEndMusic");
	}
}

void Sound::closeSadEndMusic()
{
	//���� ���� � ������� � ���� ��� ��������
	if (m_isSadEndMusicOpen)
	{
		stop(L"sadEndMusic");

		//�� ��������� ��� ����
		close(L"sadEndMusic");

		//� �������, �� ���� � ������� � ���� ��� ��������
		m_isSadEndMusicOpen = false;
	}
}

void Sound::openMoveSound()
{
	//���� ���� � ������ ���������� ������ ���� �� ��������
	if (!m_isMoveSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_MOVE_SOUND_PATH, L"moveSound");

		//� �������, �� ���� � ������ ���������� ������ ���� ��������
		m_isMoveSoundOpen = true;
	}
}

void Sound::playMoveSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������ ���������� ������ ���� �� ��������
	if (!m_isMoveSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_MOVE_SOUND_PATH, L"moveSound");

		//� �������, �� ���� � ������ ���������� ������ ���� ��������
		m_isMoveSoundOpen = true;
	}

	play(L"moveSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopMoveSound()
{
	//���� ���� � ������ ���������� ������ ���� ��������
	if (m_isMoveSoundOpen)
	{
		stop(L"moveSound");
	}
}

void Sound::closeMoveSound()
{
	//���� ���� � ������ ���������� ������ ���� ��������
	if (m_isMoveSoundOpen)
	{
		stop(L"moveSound");

		//�� ��������� ��� ����
		close(L"moveSound");

		//� �������, �� ���� � ������ ���������� ������ ���� ��������
		m_isMoveSoundOpen = false;
	}
}

void Sound::openLineClearSound()
{
	//���� ���� � ������ �������� ����� �� ��������
	if (!m_isLineClearSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_LINE_CLEAR_SOUND_PATH, L"lineClearSound");

		//� �������, �� ���� � ������ �������� ����� ��������
		m_isLineClearSoundOpen = true;
	}
}

void Sound::playLineClearSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������ �������� ����� �� ��������
	if (!m_isLineClearSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_LINE_CLEAR_SOUND_PATH, L"lineClearSound");

		//� �������, �� ���� � ������ �������� ����� ��������
		m_isLineClearSoundOpen = true;
	}

	play(L"lineClearSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopLineClearSound()
{
	//���� ���� � ������ �������� ����� ��������
	if (m_isLineClearSoundOpen)
	{
		stop(L"lineClearSound");
	}
}

void Sound::closeLineClearSound()
{
	//���� ���� � ������ �������� ����� ��������
	if (m_isLineClearSoundOpen)
	{
		stop(L"lineClearSound");

		//�� ��������� ��� ����
		close(L"lineClearSound");

		//� �������, �� ���� � ������ �������� ����� ��������
		m_isLineClearSoundOpen = false;
	}
}

void Sound::openSpaceSound()
{
	//���� ���� � ������ �������� ������ �� ��������
	if (!m_isSpaceSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_SPACE_SOUND_PATH, L"spaceSound");

		//� �������, �� ���� � ������ �������� ������ ��������
		m_isSpaceSoundOpen = true;
	}
}

void Sound::playSpaceSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//���� ���� � ������ �������� ������ �� ��������
	if (!m_isSpaceSoundOpen)
	{
		//�� ��������� ��� ����
		open(m_SPACE_SOUND_PATH, L"spaceSound");

		//� �������, �� ���� � ������ �������� ������ ��������
		m_isSpaceSoundOpen = true;
	}

	play(L"spaceSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopSpaceSound()
{
	//���� ���� � ������ �������� ������ ��������
	if (m_isSpaceSoundOpen)
	{
		stop(L"spaceSound");
	}
}

void Sound::closeSpaceSound()
{
	//���� ���� � ������ �������� ������ ��������
	if (m_isSpaceSoundOpen)
	{
		stop(L"spaceSound");

		//�� ��������� ��� ����
		close(L"spaceSound");

		//� �������, �� ���� � ������ �������� ������ ��������
		m_isSpaceSoundOpen = false;
	}
}

int Sound::getMusicVolume()
{
	return m_musicVolume;
}

int Sound::getEffectsVolume()
{
	return m_effectsVolume;
}

void Sound::setMusicVolume(const int VOLUME)
{
	//���� ������� ������� � ���������� ����� �� 0 �� 100
	if (VOLUME >= 0 && VOLUME <= 100) 
	{
		//�� �������� ���� �������
		m_musicVolume = VOLUME; 
	}
	//���� ������� ������� ����� 0
	else if (VOLUME < 0)
	{
		//�� ������� ������� 0
		m_musicVolume = 0;
	}
	//���� ������� ������� ����� 100
	else if (VOLUME > 100)
	{
		//�� ������� ������� 100
		m_musicVolume = 100;
	}
}

void Sound::setEffectsVolume(const int VOLUME)
{
	//���� ������� ������� � ���������� ����� �� 0 �� 100
	if (VOLUME >= 0 && VOLUME <= 100)
	{
		//�� �������� ���� �������
		m_effectsVolume = VOLUME;
	}
	//���� ������� ������� ����� 0
	else if (VOLUME < 0)
	{
		//�� ������� ������� 0
		m_effectsVolume = 0;
	}
	//���� ������� ������� ����� 100
	else if (VOLUME > 100)
	{
		//�� ������� ������� 100
		m_effectsVolume = 100;
	}
}