#pragma once

#include <windows.h>

//для роботи зі звуками та музикою =========================
#include <mmsystem.h>				//для функції mciSendString()
#pragma comment(lib, "winmm.lib")	//реалізація <mmsystem.h>
//==========================================================

#include <string>	//для роботи з рядками

class Sound
{
public:
	//функція для відкриття файлу з музикою в меню
	static void openMenuMusic();

	//функція для початку програвання музики в меню
	static void playMenuMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання музики в меню
	static void stopMenuMusic();

	//функція для закриття файлу з музикою в кінці
	static void closeMenuMusic();

	//функція для відкриття файлу з музикою в кінці (новий рекорд)
	static void openHappyEndMusic();

	//функція для програвання музики в кінці (новий рекорд)
	static void playHappyEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання музики в кінці гри (новий рекорд)
	static void stopHappyEndMusic();

	//функція для закриття файлу з музикою в кінці гри (новий рекорд)
	static void closeHappyEndMusic();

	//функція для відкриття файлу з музикою в кінці гри (без рекорду)
	static void openSadEndMusic();

	//функція для програвання музики в кінці гри (без рекорду)
	static void playSadEndMusic(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання музики в кінці гри (без рекорду)
	static void stopSadEndMusic();

	//функція для закриття файлу з музикою в кінці гри (без рекорду)
	static void closeSadEndMusic();

	//функція для відкриття файлу зі звуком переміщення фігури вниз
	static void openMoveSound();

	//функція для програвання звуку переміщення фігури вниз
	static void playMoveSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання звуку переміщення фігури вниз
	static void stopMoveSound();

	//функція для закриття файлу зі звуком переміщення фігури вниз
	static void closeMoveSound();

	//функція для відкриття файлу зі звуком очищення рядка
	static void openLineClearSound();

	//функція для програвання звуку очищення рядка
	static void playLineClearSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання звуку очищення рядка
	static void stopLineClearSound();

	//функція для закриття файлу зі звуком очищення рядка
	static void closeLineClearSound();

	//функція для відкриття файлу зі звуком швидкого падіння
	static void openSpaceSound();

	//функція для програвання звуку швидкого падіння
	static void playSpaceSound(const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для завершення програвання звуку швидкого падіння
	static void stopSpaceSound();

	//функція для закриття файлу зі звуком швидкого падіння
	static void closeSpaceSound();

	//функція для отримання гучності музики
	static int getMusicVolume();

	//функція для отримання гучності ефектів
	static int getEffectsVolume();

	//функція для зміни гучності музики
	static void setMusicVolume(const int VOLUME);

	//функція для зміни гучності ефектів
	static void setEffectsVolume(const int VOLUME);

private:
	//використовувані звуки =====================================
	static const wchar_t* m_MENU_MUSIC_PATH;		//музика в меню
	static const wchar_t* m_HAPPY_END_MUSIC_PATH;	//музика в кінці (новий рекорд)
	static const wchar_t* m_SAD_END_MUSIC_PATH;		//музика в кінці (без рекорду)

	static const wchar_t* m_MOVE_SOUND_PATH;		//звук переміщення
	static const wchar_t* m_LINE_CLEAR_SOUND_PATH;	//звук очищення ряду
	static const wchar_t* m_SPACE_SOUND_PATH;		//звук швидкого падіння
	//==========================================================

	//для роботи зі звуками ====================================
	static const wchar_t* m_OPEN_CMD;			//для відкриття файлу зі звуком
	static const wchar_t* m_CLOSE_CMD;			//для закриття файлу зі звуком
	static const wchar_t* m_STOP_CMD;			//для зупинки програвання звуку
	static const wchar_t* m_SEEK_CMD;			//для перемотування звуку на початок
	static const wchar_t* m_PLAY_CMD;			//для початку програвання звуку
	static const wchar_t* m_PLAY_REPEAT_CMD;	//для повторного програвання звуку
	static const wchar_t* m_SET_VOLUME_CMD;		//для зміни гучності звуку

	static wchar_t m_command[300];				//команда для роботи зі звуками
	//==========================================================

	static int m_musicVolume;	//гучність музики (від 0 до 100)
	static int m_effectsVolume;	//гучність ефектів (від 0 до 100)

	//контроль відкриття/програвання звуків =====================
	static bool m_isMenuMusicOpen;			//чи відкритий файл з музикою в меню

	static bool m_isHappyEndMusicOpen;		//чи відкритий файл з музикою в кінці гри (новий рекорд)

	static bool m_isSadEndMusicOpen;		//чи відкритий файл з музикою в кінці гри (без рекорду)

	static bool m_isMoveSoundOpen;			//чи відкритий файл зі звуком переміщення фігури вниз

	static bool m_isLineClearSoundOpen;		//чи відкритий файл зі звуком очищення рядка

	static bool m_isSpaceSoundOpen;			//чи відкритий файл зі звуком швидкого падіння
	//==========================================================

private:
	//функція для відкриття файлу зі звуком
	static void open(const wchar_t* PATH, const wchar_t* PSEUDO);

	//функція для перемотування звуку на початок
	static void seek(const wchar_t* PSEUDO);

	//функція для зміни гучності звуку
	static void setVolume(const wchar_t* PSEUDO, const int VOLUME);

	//функція для програвання звуку
	static void play(const wchar_t* PSEUDO, const bool REPEAT, const bool SEEK, const int VOLUME);

	//функція для зупинки програвання звуку
	static void stop(const wchar_t* PSEUDO);

	//функція для закриття файлу зі звуком
	static void close(const wchar_t* PSEUDO);
};
