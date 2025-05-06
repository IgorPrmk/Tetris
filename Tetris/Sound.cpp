#include "Sound.h"

//використовувані звуки =====================================
const wchar_t* Sound::m_MENU_MUSIC_PATH			= L"music/tetrisMusic.mp3";	//музика в меню
const wchar_t* Sound::m_HAPPY_END_MUSIC_PATH	= L"music/happyEnd.mp3";	//музика в кінці (новий рекорд)
const wchar_t* Sound::m_SAD_END_MUSIC_PATH		= L"music/sadEnd.mp3";		//музика в кінці (без рекорду)

const wchar_t* Sound::m_MOVE_SOUND_PATH			= L"music/tetrisMove.wav";	//звук переміщення
const wchar_t* Sound::m_LINE_CLEAR_SOUND_PATH	= L"music/raw.wav";			//звук очищення ряду
const wchar_t* Sound::m_SPACE_SOUND_PATH		= L"music/space.wav";		//звук швидкого падіння
//==========================================================

//для роботи зі звуками ====================================
const wchar_t* Sound::m_OPEN_CMD		= L"open \"%ls\" type mpegvideo alias %ls";	//для відкриття файлу зі звуком
const wchar_t* Sound::m_CLOSE_CMD		= L"close %ls";								//для закриття файлу зі звуком
const wchar_t* Sound::m_STOP_CMD		= L"stop %ls";								//для зупинки програвання звуку
const wchar_t* Sound::m_SEEK_CMD		= L"seek %ls to start";						//для перемотування звуку на початок
const wchar_t* Sound::m_PLAY_CMD		= L"play %ls";								//для початку програвання звуку
const wchar_t* Sound::m_PLAY_REPEAT_CMD = L"play %ls repeat";						//для повторного програвання звуку
const wchar_t* Sound::m_SET_VOLUME_CMD	= L"setaudio %ls volume to %d";				//для зміни гучності звуку
//==========================================================

wchar_t Sound::m_command[300]	= { 0 };	//команда для роботи зі звуками

int Sound::m_musicVolume		= 50;		//гучність музики (від 0 до 100)
int Sound::m_effectsVolume		= 50;		//гучність ефектів (від 0 до 100)

//контроль відкриття/програвання звуків ====================
bool Sound::m_isMenuMusicOpen			= false; //чи відкритий файл з музикою в меню

bool Sound::m_isHappyEndMusicOpen		= false; //чи відкритий файл з музикою в кінці (новий рекорд)

bool Sound::m_isSadEndMusicOpen			= false; //чи відкритий файл з музикою в кінці гри (без рекорду)

bool Sound::m_isMoveSoundOpen			= false; //чи відкритий файл зі звуком переміщення фігури вниз

bool Sound::m_isLineClearSoundOpen		= false; //чи відкритий файл зі звуком очищення рядка

bool Sound::m_isSpaceSoundOpen			= false; //чи відкритий файл зі звуком швидкого падіння
//==========================================================

void Sound::open(const wchar_t* PATH, const wchar_t* PSEUDO)
{
	//копіювання модифікованої команди
	swprintf_s(m_command, m_OPEN_CMD, PATH, PSEUDO);

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::seek(const wchar_t* PSEUDO)
{
	//копіювання модифікованої команди
	swprintf_s(m_command, m_SEEK_CMD, PSEUDO);

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::setVolume(const wchar_t* PSEUDO, const int VOLUME)
{
	//копіювання модифікованої команди
	//mciSendString працює від 0 до 1000, тому множимо на 10
	swprintf_s(m_command, m_SET_VOLUME_CMD, PSEUDO, VOLUME * 10);

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::play(const wchar_t* PSEUDO, const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//встановлюємо гучність звуку
	setVolume(PSEUDO, VOLUME); 

	if (SEEK) //якщо потрібно перемотування на початок
	{
		//то перемотування на початок
		seek(PSEUDO);
	}
	
	if (REPEAT) //якщо потрібно тільки повторне програвання
	{
		//копіювання модифікованої команди
		swprintf_s(m_command, m_PLAY_REPEAT_CMD, PSEUDO);
	}
	else //якщо не потрібно повторне програвання
	{
		//копіювання модифікованої команди
		swprintf_s(m_command, m_PLAY_CMD, PSEUDO);
	}

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::stop(const wchar_t* PSEUDO)
{
	//копіювання модифікованої команди
	swprintf_s(m_command, m_STOP_CMD, PSEUDO);

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::close(const wchar_t* PSEUDO)
{
	//копіювання модифікованої команди
	swprintf_s(m_command, m_CLOSE_CMD, PSEUDO);

	//опрацювання команди
	mciSendString(m_command, 0, 0, 0);
}

void Sound::openMenuMusic()
{
	//якщо файл з музикою в меню не відкритий
	if (!m_isMenuMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_MENU_MUSIC_PATH, L"menuMusic"); 

		//і вказуємо, що файл з музикою в меню відкритий
		m_isMenuMusicOpen = true;
	}
}

void Sound::playMenuMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл з музикою в меню не відкритий
	if (!m_isMenuMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_MENU_MUSIC_PATH, L"menuMusic");

		//і вказуємо, що файл з музикою в меню відкритий
		m_isMenuMusicOpen = true;
	}

	play(L"menuMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopMenuMusic()
{
	//якщо файл з музикою в меню відкритий
	if (m_isMenuMusicOpen)
	{
		stop(L"menuMusic");
	}
}

void Sound::closeMenuMusic()
{
	//якщо файл з музикою в меню відкритий
	if (m_isMenuMusicOpen)
	{
		stop(L"menuMusic");

		//то закриваємо цей файл
		close(L"menuMusic");

		//і вказуємо, що файл з музикою в меню закритий
		m_isMenuMusicOpen = false;
	}
}

void Sound::openHappyEndMusic()
{
	//якщо файл з музикою в кінці гри не відкритий
	if (!m_isHappyEndMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_HAPPY_END_MUSIC_PATH, L"happyEndMusic");

		//і вказуємо, що файл з музикою в кінці гри відкритий
		m_isHappyEndMusicOpen = true;
	}
}

void Sound::playHappyEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл з музикою в кінці гри не відкритий
	if (!m_isHappyEndMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_HAPPY_END_MUSIC_PATH, L"happyEndMusic");

		//і вказуємо, що файл з музикою в кінці гри відкритий
		m_isHappyEndMusicOpen = true;
	}

	play(L"happyEndMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopHappyEndMusic()
{
	//якщо файл з музикою в кінці гри відкритий
	if (m_isHappyEndMusicOpen)
	{
		stop(L"happyEndMusic");
	}
}

void Sound::closeHappyEndMusic()
{
	//якщо файл з музикою в кінці гри відкритий
	if (m_isHappyEndMusicOpen)
	{
		stop(L"happyEndMusic");

		//то закриваємо цей файл
		close(L"happyEndMusic");

		//і вказуємо, що файл з музикою в кінці гри закритий
		m_isHappyEndMusicOpen = false;
	}
}

void Sound::openSadEndMusic()
{
	//якщо файл з музикою в кінці гри не відкритий
	if (!m_isSadEndMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_SAD_END_MUSIC_PATH, L"sadEndMusic");

		//і вказуємо, що файл з музикою в кінці гри відкритий
		m_isSadEndMusicOpen = true;
	}
}

void Sound::playSadEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл з музикою в кінці гри не відкритий
	if (!m_isSadEndMusicOpen)
	{
		//то відкриваємо цей файл
		open(m_SAD_END_MUSIC_PATH, L"sadEndMusic");

		//і вказуємо, що файл з музикою в кінці гри відкритий
		m_isSadEndMusicOpen = true;
	}

	play(L"sadEndMusic", REPEAT, SEEK, VOLUME);
}

void Sound::stopSadEndMusic()
{
	//якщо файл з музикою в кінці гри відкритий
	if (m_isSadEndMusicOpen)
	{
		stop(L"sadEndMusic");
	}
}

void Sound::closeSadEndMusic()
{
	//якщо файл з музикою в кінці гри відкритий
	if (m_isSadEndMusicOpen)
	{
		stop(L"sadEndMusic");

		//то закриваємо цей файл
		close(L"sadEndMusic");

		//і вказуємо, що файл з музикою в кінці гри закритий
		m_isSadEndMusicOpen = false;
	}
}

void Sound::openMoveSound()
{
	//якщо файл зі звуком переміщення фігури вниз не відкритий
	if (!m_isMoveSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_MOVE_SOUND_PATH, L"moveSound");

		//і вказуємо, що файл зі звуком переміщення фігури вниз відкритий
		m_isMoveSoundOpen = true;
	}
}

void Sound::playMoveSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл зі звуком переміщення фігури вниз не відкритий
	if (!m_isMoveSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_MOVE_SOUND_PATH, L"moveSound");

		//і вказуємо, що файл зі звуком переміщення фігури вниз відкритий
		m_isMoveSoundOpen = true;
	}

	play(L"moveSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopMoveSound()
{
	//якщо файл зі звуком переміщення фігури вниз відкритий
	if (m_isMoveSoundOpen)
	{
		stop(L"moveSound");
	}
}

void Sound::closeMoveSound()
{
	//якщо файл зі звуком переміщення фігури вниз відкритий
	if (m_isMoveSoundOpen)
	{
		stop(L"moveSound");

		//то закриваємо цей файл
		close(L"moveSound");

		//і вказуємо, що файл зі звуком переміщення фігури вниз закритий
		m_isMoveSoundOpen = false;
	}
}

void Sound::openLineClearSound()
{
	//якщо файл зі звуком очищення рядка не відкритий
	if (!m_isLineClearSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_LINE_CLEAR_SOUND_PATH, L"lineClearSound");

		//і вказуємо, що файл зі звуком очищення рядка відкритий
		m_isLineClearSoundOpen = true;
	}
}

void Sound::playLineClearSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл зі звуком очищення рядка не відкритий
	if (!m_isLineClearSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_LINE_CLEAR_SOUND_PATH, L"lineClearSound");

		//і вказуємо, що файл зі звуком очищення рядка відкритий
		m_isLineClearSoundOpen = true;
	}

	play(L"lineClearSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopLineClearSound()
{
	//якщо файл зі звуком очищення рядка відкритий
	if (m_isLineClearSoundOpen)
	{
		stop(L"lineClearSound");
	}
}

void Sound::closeLineClearSound()
{
	//якщо файл зі звуком очищення рядка відкритий
	if (m_isLineClearSoundOpen)
	{
		stop(L"lineClearSound");

		//то закриваємо цей файл
		close(L"lineClearSound");

		//і вказуємо, що файл зі звуком очищення рядка закритий
		m_isLineClearSoundOpen = false;
	}
}

void Sound::openSpaceSound()
{
	//якщо файл зі звуком швидкого падіння не відкритий
	if (!m_isSpaceSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_SPACE_SOUND_PATH, L"spaceSound");

		//і вказуємо, що файл зі звуком швидкого падіння відкритий
		m_isSpaceSoundOpen = true;
	}
}

void Sound::playSpaceSound(const bool REPEAT, const bool SEEK, const int VOLUME)
{
	//якщо файл зі звуком швидкого падіння не відкритий
	if (!m_isSpaceSoundOpen)
	{
		//то відкриваємо цей файл
		open(m_SPACE_SOUND_PATH, L"spaceSound");

		//і вказуємо, що файл зі звуком швидкого падіння відкритий
		m_isSpaceSoundOpen = true;
	}

	play(L"spaceSound", REPEAT, SEEK, VOLUME);
}

void Sound::stopSpaceSound()
{
	//якщо файл зі звуком швидкого падіння відкритий
	if (m_isSpaceSoundOpen)
	{
		stop(L"spaceSound");
	}
}

void Sound::closeSpaceSound()
{
	//якщо файл зі звуком швидкого падіння відкритий
	if (m_isSpaceSoundOpen)
	{
		stop(L"spaceSound");

		//то закриваємо цей файл
		close(L"spaceSound");

		//і вказуємо, що файл зі звуком швидкого падіння закритий
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
	//якщо вказана гучність в допустимих межах від 0 до 100
	if (VOLUME >= 0 && VOLUME <= 100) 
	{
		//то записуємо нову гучність
		m_musicVolume = VOLUME; 
	}
	//якщо вказана гучність менше 0
	else if (VOLUME < 0)
	{
		//то гучність дорівнює 0
		m_musicVolume = 0;
	}
	//якщо вказана гучність більше 100
	else if (VOLUME > 100)
	{
		//то гучність дорівнює 100
		m_musicVolume = 100;
	}
}

void Sound::setEffectsVolume(const int VOLUME)
{
	//якщо вказана гучність в допустимих межах від 0 до 100
	if (VOLUME >= 0 && VOLUME <= 100)
	{
		//то записуємо нову гучність
		m_effectsVolume = VOLUME;
	}
	//якщо вказана гучність менше 0
	else if (VOLUME < 0)
	{
		//то гучність дорівнює 0
		m_effectsVolume = 0;
	}
	//якщо вказана гучність більше 100
	else if (VOLUME > 100)
	{
		//то гучність дорівнює 100
		m_effectsVolume = 100;
	}
}