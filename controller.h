#pragma once

#ifndef contoller
#define contoller

// класс обработки нажатий клавиатуры
class KEYBOARD
{
	public:

	void static Keyboard(unsigned char, int, int);
};

class DATA_FROM_CONTROLLER_TO_MODEL 
{

	bool keyboard_left_pressed;
	bool keyboard_right_pressed;

	public:

	void KeyBoardLeftPressed(bool);
	void KeyBoardRightPressed(bool);

	static bool GetKeyBoardLeft();
	static bool GetKeyBoardRight();

};

// интерфейс из контроллера в модель.
class INTERFACE_CONTROLLER_TO_MODEL
{
	public:

	void static SendDataFromControllerToModel (DATA_FROM_CONTROLLER_TO_MODEL);
};

// таймер который обновляет картинку
class TIMER
{
	public:

	void static Timer(int);
};

// коды нажатий клавиш z and x
const int const_button_z = 255;
const int const_button_x = 247;

#endif

