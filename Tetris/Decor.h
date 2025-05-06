#pragma once

#include "General.h"
#include "Figure.h"

//клас, який описує декоративний "дощ" з фігур
class Decor
{
public:
	//конструктор
	Decor();

	//конструктор копіювання  
	Decor(const Decor& other) = default;

	//оператор присвоєння  
	Decor& operator=(const Decor& other);

	//функція, яка ініціалізує декоративний "дощ" з фігур
	void initDecorRain();

	//функція, яка оновлює позицію декоративних фігур
	void updateDecorRain();
	
	//функція, яка малює декоративний "дощ" з фігур
	void drawDecorRain(const HDC DEVICE_CONTEXT);
	
	
public:
	//макс кількість одночасно падаючих декоративних фігур в меню ("дощ" із фігур)
	const int m_MAX_DECOR_COUNT = 10;

private:
	Figure m_figure;		//тип фігури
	double m_x		= 0.0;	//горизонтальне положення фігури (в пікселях)
	double m_y		= 0.0;	//вертикальне положення фігури (в пікселях)
	double m_speed	= 0.0;	//швидкість падіння фігури (в пікселях за тік)
};