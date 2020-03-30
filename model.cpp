#include "stdafx.h"

#include <gl/glut.h>
#include "controller.h"
#include "model.h"
#include "open_gl.h"
#include "check_touch_something.h"

// ��������� �� ����������� � ������. ���������� �� �������
void INTERFACE_CONTROLLER_TO_MODEL::SendDataFromControllerToModel(DATA_FROM_CONTROLLER_TO_MODEL 
																		data_from_controller_to_model)
{
	RECTANGLE game_field = RECTANGLE::InitGameField(); // ������� ���� - �������� � ������������

	static RECTANGLE board = RECTANGLE::InitBoard(); // ����� �������� � ������������

	board = RECTANGLE::MoveBoard(board, data_from_controller_to_model); // �������� �����

	static RECTANGLE* bricks = RECTANGLE::InitBricks(); // ������� � �� �������������

	static CIRCLE ball = CIRCLE::SetBallParametrs(ball); // ����� � ��� �������������

	ball = CHECK_TOUCH::CheckTouch (game_field, board, ball); // �������� ������� ���� � ����

	ball = CIRCLE::EventHandle(ball); // ��������� ��������� ����������� ���� � ��������� ������� �� ������ ��� ������

	ball = CHECK_TOUCH::CheckTouchBricks(bricks, ball); // ������� �������� � ����

	ball = CIRCLE::EventHandle(ball); // ��������� ��������� ����������� ���� � ��������� ������� � ��������

	DRAW::Draw (game_field, board, bricks, ball); // ���������� ������ � OPEN Gl
}

// ������������ �������� ����
RECTANGLE RECTANGLE::InitGameField ()
{
	RECTANGLE game_field; 

	int x_screen = DIMENSION_OF_MONITOR::GetWidth(); // ��������� �������� ������ � ������
	int y_screen = DIMENSION_OF_MONITOR::GetHeight();

	// ������� ����������� �������������� 
	// game_part_of_screen ��� % ������� ����� �� ��������
	game_field.x[zero]                   = zero;                           	game_field.y[zero]                   = zero;
	game_field.x[const_left_up_point]    = zero; 	                        game_field.y[const_left_up_point]    = y_screen;
	game_field.x[const_right_up_point]   = x_screen * game_part_of_screen; 	game_field.y[const_right_up_point]   = y_screen;
	game_field.x[const_right_down_point] = x_screen * game_part_of_screen; 	game_field.y[const_right_down_point] = zero;
	game_field.x[const_left_down_point]  = zero; 	                        game_field.y[const_left_down_point]  = zero;

	return game_field;
};

// ������������� �����
RECTANGLE RECTANGLE::InitBoard()
{
	RECTANGLE board;

	// ������� ������
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_y_screen = DIMENSION_OF_MONITOR::GetHeight();

	// ����� �� ����������� ������� ����� � �������� ������� �����
	// game_part_of_screen ��� % ������� ����� �� ��������
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;
	float x_middle_game_part_of_screen = distance_x_game_part_of_screen * half_of_game_screen;

	// ����� ����� � ������ �� ������ �����. ������� � ��������� �� �������� ������
	float offset_x = const_x_offset_board_from_center * DIMENSION_OF_MONITOR::GetWidth();
	float offset_y = const_y_offset_board_from_center *  DIMENSION_OF_MONITOR::GetHeight();

	// ���� - �� ����� � ������ �������
	board.x [const_left_down_point ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_down_point ] = zero;
	board.x [const_left_up_point   ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_up_point   ] = offset_y;
	board.x [const_right_up_point  ] = x_middle_game_part_of_screen + offset_x; 	board.y [const_right_up_point  ] = offset_y;
	board.x [const_right_down_point] = x_middle_game_part_of_screen + offset_x; 	board.y [const_right_down_point] = zero;
	board.x [const_left_down_point ] = x_middle_game_part_of_screen - offset_x; 	board.y [const_left_down_point ] = zero;

	return board;
};

// �������� ����� ��������
RECTANGLE RECTANGLE::MoveBoard(RECTANGLE board, DATA_FROM_CONTROLLER_TO_MODEL data_from_controller_to_model)
{
	// ����� �� � � ����� ������� ����� 
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;

	// �������� ������� ����� 
	float x_middle_game_part_of_screen = distance_x_game_part_of_screen * half_of_game_screen;
	float offset_x_board_from_center = const_x_offset_board_from_center * distance_x_screen;

	// ����� ���� ����� �� � �� ������ 
	float edge_board_x = x_middle_game_part_of_screen - offset_x_board_from_center;

	// ���������� ����� � ������� �� ������. ��������� ����� ��������
	static int move_step = 0; 	
	
	// ���� ������ ������� �����
	if (data_from_controller_to_model.GetKeyBoardLeft() == true)
	{
		// ���� ������ �������� �� ��������
		if (move_step > -constant_count_of_step_board)
		{
			// �������� ���������� ����� ����� 
			for (int i = 0; i < const_count_point_in_rectangle; i++) board.x[i] = board.x[i] - edge_board_x / constant_count_of_step_board;
			move_step--; // ��������� ����� �����
		}
	}
	// ���� ������ ������� ������
	else if (data_from_controller_to_model.GetKeyBoardRight() == true)
	{
		// ���� ����� ����� �� �������� 
		if (move_step < constant_count_of_step_board)
		{
			// �������� ���������� ����� ������ 
			for (int i = 0; i < const_count_point_in_rectangle; i++) board.x[i] = board.x[i] + edge_board_x / constant_count_of_step_board;
			move_step++; // ��������� ����� ������
		}
	}
	return board;
};

// ������������� ��������
RECTANGLE* RECTANGLE::InitBricks()
{
	// ������� ����������� ������ ��������
	static RECTANGLE bricks [const_count_bricks];
	static bool flag_first_join = true;

	// ��� ������ ����� ������ ��������� ���� �������� � ����� �� �������
	if (flag_first_join == true) { for (int i = 0; i < const_count_bricks; i++) bricks[i].brick_visible = true; flag_first_join = false; }

	float height = DIMENSION_OF_MONITOR::GetHeight(); // ������
	float zero_level_for_bricks_y = height *  half_of_game_screen; // ������� ������� ��� �������� ��� �������� ������ �� Y

	float size_brick_x = DIMENSION_OF_MONITOR::GetWidth() * game_part_of_screen / const_count_bricks; // ������ ������� �� �
	float size_brick_y = DIMENSION_OF_MONITOR::GetHeight() * const_persent_size_of_brick_y_from_screen; // ������ ������� �� Y

	for (int i = 0; i < const_count_bricks; i++) // ������������� ������� ��������
	{
		if (bricks[i].brick_visible == false) continue; // ���� ������ ���������, ����������

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

// ������������� ��������� ������
CIRCLE CIRCLE::SetBallParametrs(CIRCLE ball)
{
	static CIRCLE ball_parametrs; 

	// ���������� ��������� ����������
	ball_parametrs.amount_segments = const_amountSegments;

	// ������ ������
	ball_parametrs.radius = const_radius_ball *  DIMENSION_OF_MONITOR::GetHeight();

	// ���������� �������� ��������� ������ ��� ����� ������ ������� �������
	float distance_x_screen = DIMENSION_OF_MONITOR::GetWidth();
	float distance_x_game_part_of_screen = distance_x_screen * game_part_of_screen;

	ball_parametrs.x_pozition_center = distance_x_game_part_of_screen * half_of_game_screen;

	static bool flag_first_join = true; // ���� ��� ���� - �� �������� ��������� ������
	
	if (flag_first_join == true)
	{
		// ����� ������ ��������� � ������� �����
		ball_parametrs.y_pozition_center = const_y_offset_board_from_center *  DIMENSION_OF_MONITOR::GetHeight();		
		ball_parametrs.y_pozition_center = ball_parametrs.y_pozition_center + ball_parametrs.radius;
		
		// �������� ����� ��������� �� �����
		ball_parametrs.ball_moving_north = true;
		flag_first_join = false;
	}

	return ball_parametrs;
}

// ���������� ��������
CIRCLE CIRCLE::EventHandle(CIRCLE ball)
{
	// ��������� ��������� � ����-�� � ����������� �������� ������
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

// ������� ��������� �������� ��������
int DIMENSION_OF_MONITOR::GetHeight()
{
	int height = glutGet(GLUT_SCREEN_HEIGHT); // ������
	height = height / quarter_of_screen;
	return height;
}

// ������� ��������� �������� ��������
int DIMENSION_OF_MONITOR::GetWidth()
{
	int width = glutGet(GLUT_SCREEN_WIDTH); // ������
	width = width / quarter_of_screen;
	return width;
};

