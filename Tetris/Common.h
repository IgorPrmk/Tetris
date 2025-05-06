#pragma once

class Game;
class Random;
class Board;
class Score;
class Decor;

//id таймерів
enum Timer
{
	DECOR_RAIN = 1, //таймер для декоративного "дощу" з фігур
	GAME_TICK = 2, //таймер для гри (оновлення стану гри)
};

//набір станів гри з точки зору інтерфейсу
enum class UIState
{
	PLAYING,	//гравець грає
	ENTER_NAME,	//гравець вводить ім'я після рекорду
	MENU,		//гравець відкрив головне меню
	CONTROLS,	//гравець відкрив підпункт головного меню "Controls"
	PAUSED,		//гравець встановив гру на паузу
	SETTINGS	//гравець відкрив підпункт головного меню "Settings"
};

