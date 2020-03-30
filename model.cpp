#include "stdafx.h"

#include <gl/glut.h>
#include "controller.h"
#include "model.h"
#include "open_gl.h"
#include "check_touch_something.h"

// интерфейс из контроллера в модель. вызывается по таймеру
void INTERFACE_CONTROLLER_TO_MODEL::SendDataFromControllerToModel(DATA_FROM_CONTROLLER_TO_MODEL 
																		data_from_controller_to_model)
{
	RECTANGLE game_field = RECTANGLE::InitGameField(); // игровое поле - создание и иницализация

	static RECTANGLE board = RECTANGLE::InitBoard(); // доска создание и иницализация

	board = RECTANGLE::MoveBoard(board, data_from_controller_to_model); // движение доски

	static RECTANGLE* bricks = RECTANGLE::InitBricks(); // кирпичи и их инициализация

	static CIRCLE ball = CIRCLE::SetBallParametrs(ball); // мячик и его инициализация

	ball = CHECK_TOUCH::CheckTouch (game_field, board, ball); // проверка касания мячи и стен

	ball = CIRCLE::EventHandle(ball); // обработка изменения направления мяча в следствии касания со стеной или доской

	ball = CHECK_TOUCH::CheckTouchBricks(bricks, ball); // касание кирпичей и мяча

	ball = CIRCLE::EventHandle(ball); // обработка изменения направления мяча в следствии касания с кирпичом

	DRAW::Draw (game_field, board, bricks, ball); // отправляем данные в OPEN Gl
}

// иницализация игрового поля
RECTANGLE RECTANGLE::InitGameField ()
{
	RECTANGLE game_field; 

	int x_screen = DIMENSION_OF_MONITOR::GetWidth(); // параметры монитора высота и ширина
	int y_screen = DIMENSION_OF_MONITOR::GetHeight();

	// задание параметоров прямоугольника 
	// game_part_of_screen это % игровой части от монитора
	game_field.x[zero]                   = zero;                           	game_field.y[zero]                   = zero;
	game_field.x[const_left_up_point]    = zero; 	                        game_field.y[const_left_up_point]    = y_screen;
	game_field.x[const_right_up_point]   = x_screen * game_part_of_screen; 	game_field.y[const_right_up_point]   = y_screen;
	game_field.x[const_right_down_point] = x_screen * game_part_of_screen; 	game_field.y[const_right_down_point] = zero;
	game_field.x[const_left_down_point]  = zero; 	                        game_field.y[const_left_down_point]  = zero;

	return game_field;
};

// инициализация доски
RECTANGLE RECTANGLE::InitBoard()
{
	RECTANGLE board;

	// размеры экрана
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_y_screen = DIMENSION_OF_MONITOR::GetHeight();

	// длина по горизонтали игровой части и середина игровой части
	// game_part_of_screen это % игровой части от монитора
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;
	float x_middle_game_part_of_screen = distance_x_game_part_of_screen * half_of_game_screen;

	// сдвиг влево и вправо от центра доски. выражен в процентах от размеров экрана
	float offset_x = const_x_offset_board_from_center * DIMENSION_OF_MONITOR::GetWidth();
	float offset_y = const_y_offset_board_from_center *  DIMENSION_OF_MONITOR::GetHeight();

	// иниц - ая доски с учетом сдвигов
	board.x [const_left_down_point ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_down_point ] = zero;
	board.x [const_left_up_point   ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_up_point   ] = offset_y;
	board.x [const_right_up_point  ] = x_middle_game_part_of_screen + offset_x; 	board.y [const_right_up_point  ] = offset_y;
	board.x [const_right_down_point] = x_middle_game_part_of_screen + offset_x; 	board.y [const_right_down_point] = zero;
	board.x [const_left_down_point ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_down_point ] = zero;

	return board;
};

// движение доски кнопками
RECTANGLE RECTANGLE::MoveBoard(RECTANGLE board, DATA_FROM_CONTROLLER_TO_MODEL data_from_controller_to_model)
{
	// экран по Х и длина игровой части 
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;

	// середина игровой части 
	float x_middle_game_part_of_screen = distance_x_game_part_of_screen * half_of_game_screen;
	float offset_x_board_from_center = const_x_offset_board_from_center * distance_x_screen;

	// сдвиг края доски по Х от центра 
	float edge_board_x = x_middle_game_part_of_screen - offset_x_board_from_center;

	// количество шагов в сторону от центра. сохраняем между вызовами
	static int move_step = 0; 	
	
	// если нажата клавиша влево
	if (data_from_controller_to_model.GetKeyBoardLeft() == true)
	{
		// если предел движения не превышен
		if (move_step > -constant_count_of_step_board)
		{
			// изменяем координаты доски влево 
			for (int i = 0; i < const_count_point_in_rectangle; i++) board.x[i] = board.x[i] - edge_board_x / constant_count_of_step_board;
			move_step--; // декремент шагов влево
		}
	}
	// если нажата клавиша вправо
	else if (data_from_controller_to_model.GetKeyBoardRight() == true)
	{
		// если лимит шагов не превышен 
		if (move_step < constant_count_of_step_board)
		{
			// изменяем координаты доски вправо 
			for (int i = 0; i < const_count_point_in_rectangle; i++) board.x[i] = board.x[i] + edge_board_x / constant_count_of_step_board;
			move_step++; // инкремент шагов вправо
		}
	}
	return board;
};

// инициализация кирпичей
RECTANGLE* RECTANGLE::InitBricks()
{
	// создаем статический массив кирпичей
	static RECTANGLE bricks [const_count_bricks];
	static bool flag_first_join = true;

	// при первом вызое делаем видимость всем кирпичам и более не заходим
	if (flag_first_join == true) { for (int i = 0; i < const_count_bricks; i++) bricks[i].brick_visible = true; flag_first_join = false; }

	float height = DIMENSION_OF_MONITOR::GetHeight(); // высота
	float zero_level_for_bricks_y = height *  half_of_game_screen; // нулевой уровень для кирпичей это середина экрана по Y

	float size_brick_x = DIMENSION_OF_MONITOR::GetWidth() * game_part_of_screen / const_count_bricks; // размер кирпича по Х
	float size_brick_y = DIMENSION_OF_MONITOR::GetHeight() * const_persent_size_of_brick_y_from_screen; // размер кирпича по Y

	for (int i = 0; i < const_count_bricks; i++) // инициализация видимых кирпичей
	{
		if (bricks[i].brick_visible == false) continue; // если кирпич невидимый, пропускаем

		bricks[i].x[const_left_down_point       ] = size_brick_x * i;                
		bricks[i].y[const_left_down_point       ] = zero_level_for_bricks_y;

		bricks[i].x[const_left_up_point         ] = size_brick_x * i;                
		bricks[i].y[const_left_up_point         ] = zero_level_for_bricks_y + size_brick_y;

		bricks[i].x[const_right_up_point        ] = size_brick_x + size_brick_x * i;  
		bricks[i].y[const_right_up_point        ] = zero_level_for_bricks_y + size_brick_y;

		bricks[i].x[const_right_down_point      ] = size_brick_x + size_brick_x * i;  
		bricks[i].y[const_right_down_point      ] = zero_level_for_bricks_y;

		bricks[i].x[const_left_down_point_repeat] = size_brick_x * i;                 
		bricks[i].y[const_left_down_point_repeat] = zero_level_for_bricks_y;
	}

	return bricks;
}

// устанавливаем параметры шарика
CIRCLE CIRCLE::SetBallParametrs(CIRCLE ball)
{
	static CIRCLE ball_parametrs; 

	// количество сегментов окружности
	ball_parametrs.amount_segments = const_amountSegments;

	// радиус шарика
	ball_parametrs.radius = const_radius_ball *  DIMENSION_OF_MONITOR::GetHeight();

	// определяем исходное положение шарика оно равно центру игровой площади
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;

	ball_parametrs.x_pozition_center = distance_x_game_part_of_screen * half_of_game_screen;

	static bool flag_first_join = true; // один раз иниц - ем исходные параметры шарика
	
	if (flag_first_join == true)
	{
		// центр шарика совпадает с центром доски
		ball_parametrs.y_pozition_center = const_y_offset_board_from_center *  DIMENSION_OF_MONITOR::GetHeight();		
		ball_parametrs.y_pozition_center = ball_parametrs.y_pozition_center + ball_parametrs.radius;
		
		// начально шарик движеться на север
		ball_parametrs.ball_moving_north = true;
		flag_first_join = false;
	}

	return ball_parametrs;
}

// обработчик движения
CIRCLE CIRCLE::EventHandle(CIRCLE ball)
{
	// выполняем инкремент в соот-ии с направление движения шарика
	if (ball.ball_moving_north == true)
	{
		ball.y_pozition_center++;
	}
	else if (ball.ball_moving_south == true)
	{
		ball.y_pozition_center--;
	}

	if (ball.ball_moving_west == true)
	{
		ball.x_pozition_center--;
	}
	else if (ball.ball_moving_east == true)
	{
		ball.x_pozition_center++;
	}

	return ball;
}

// функция получения размеров монитора
int DIMENSION_OF_MONITOR::GetHeight()
{
	int height = glutGet(GLUT_SCREEN_HEIGHT); // высота
	height = height / quarter_of_screen;
	return height;
}

// функция получения размеров монитора
int DIMENSION_OF_MONITOR::GetWidth()
{
	int width = glutGet(GLUT_SCREEN_WIDTH); // ширина
	width = width / quarter_of_screen;
	return width;
};

