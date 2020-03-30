#pragma once

#ifndef model
#define model

const float game_part_of_screen = 0.75f;
const float half_of_game_screen = 0.5f;

const float const_persent_size_of_brick_y_from_screen = 0.06f;

const float const_x_offset_board_from_center = 0.06f;
const float const_y_offset_board_from_center = 0.03f;

const float const_radius_ball = const_y_offset_board_from_center / 2; // радиус равен половине высоты доски

const int const_count_point_in_rectangle = 5;

const int const_count_bricks = 10;

const int const_amountSegments = 50;	

const int const_left_down_point  = 0;
const int const_left_up_point    = 1;
const int const_right_down_point = 3;
const int const_right_up_point   = 2;
const int const_left_down_point_repeat = 4;

const int const_count_side_board = 3;

const signed int constant_count_of_step_board = 8;

class RECTANGLE
{
	public:

	int x[const_count_point_in_rectangle];
	int y[const_count_point_in_rectangle];

	bool brick_visible;

	static RECTANGLE InitGameField();
	
	static RECTANGLE InitBoard();
	static RECTANGLE MoveBoard(RECTANGLE board, DATA_FROM_CONTROLLER_TO_MODEL data_from_controller_to_model);
	
	static RECTANGLE* InitBricks();
	static void EraseBrick(RECTANGLE* bricks);
};

class DIMENSION_OF_MONITOR
{
	public:

	static int GetWidth();
	static int GetHeight();
};

class CIRCLE
{
	public:

	int amount_segments;
	float radius;
	int x_pozition_center;
	int y_pozition_center;

	bool ball_moving_north;
	bool ball_moving_south;
	bool ball_moving_west;
	bool ball_moving_east;

	static CIRCLE SetBallParametrs(CIRCLE ball);
	static CIRCLE EventHandle(CIRCLE ball);

	static bool CompareCircles(CIRCLE var_one, CIRCLE var_two);
};

class DRAW
{
	public:

	void static Draw(RECTANGLE game_field, RECTANGLE board, RECTANGLE* bricks, CIRCLE ball);
};

#endif

