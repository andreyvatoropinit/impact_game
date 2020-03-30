#include "stdafx.h"
#include <gl/glut.h>
#include "controller.h"
#include "open_gl.h"

// глобальная переменная необходима т.к. работа идет в двух потоках
// один поток считывает данные о нажатии клавиш, второй отправляет их в модель.
static DATA_FROM_CONTROLLER_TO_MODEL data_from_controller_to_model;

void DATA_FROM_CONTROLLER_TO_MODEL::KeyBoardLeftPressed (bool button_pressed)
{
	data_from_controller_to_model.keyboard_left_pressed = button_pressed;
}

void DATA_FROM_CONTROLLER_TO_MODEL::KeyBoardRightPressed (bool button_pressed)
{
	data_from_controller_to_model.keyboard_right_pressed = button_pressed;
}

bool DATA_FROM_CONTROLLER_TO_MODEL::GetKeyBoardLeft()
{
	return data_from_controller_to_model.keyboard_left_pressed;
}

bool DATA_FROM_CONTROLLER_TO_MODEL::GetKeyBoardRight()
{
	return data_from_controller_to_model.keyboard_right_pressed;
}

void KEYBOARD::Keyboard(unsigned char key, int x, int y)
{ 
	// если нажали клавишу Z
	if (key == const_button_z)
	{
 		data_from_controller_to_model.KeyBoardLeftPressed(true);
	}
	else if (key == const_button_x)
	{
		data_from_controller_to_model.KeyBoardRightPressed(true);
	}
}

void TIMER::Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(refresh_timer, Timer, number_of_timer);

	// шлем данные в модель
	INTERFACE_CONTROLLER_TO_MODEL::SendDataFromControllerToModel(data_from_controller_to_model);
	
	// сбрасываем факт нажатия кнопок клавиатуры
	data_from_controller_to_model.KeyBoardLeftPressed(false);
	data_from_controller_to_model.KeyBoardRightPressed(false);
}