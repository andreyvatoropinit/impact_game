#include "stdafx.h"
#include <cstdlib>
#include "controller.h"
#include "model.h"
#include "check_touch_something.h"

// �������� ������� �������� ����, �����, ������
CIRCLE CHECK_TOUCH::CheckTouch(RECTANGLE game_field, RECTANGLE board, CIRCLE input_ball)
{
	CIRCLE local_ball = input_ball;

	// ��������� ������� ������ � �������� ����
	input_ball = CHECK_TOUCH::CheckTouchBallAndWall(game_field, input_ball);

	// ���������� �����.
	bool object_eqaul = CIRCLE::CompareCircles(input_ball, local_ball);

	// ���� ����� ������� ����������� - ���������� ���������� ����� � ������
	if (object_eqaul != true) return input_ball;

	// ��������� ������� ����� � ������
	input_ball = CHECK_TOUCH::CheckTouchBallAndBoard(board, input_ball);

	return input_ball;
}

// �������� ������� ������ � ��������
CIRCLE CHECK_TOUCH::CheckTouchBallAndWall(RECTANGLE game_field, CIRCLE ball)
{
	// ����������� ��� � ������� ��� ��������� �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ���� ����� �������� � ������� �����, �� ������ �������� �� ��
	if (square_ball.up_y >= game_field.y[const_left_up_point])
	{
		ball.ball_moving_north = false; ball.ball_moving_south = true;
	}
	// ���� ����� �������� � ���, �� ������� �� ����
	else if (square_ball.down_y <= game_field.y[const_left_down_point])
	{
		exit(0); // ���� ����� ���� ���� ����� - �������
	}
	// ���� ����� �������� �� ���� �� ����, �� ������ ����������� �� ������ ��� �����
	else if (square_ball.left_x <= game_field.x[const_left_down_point])
	{
		ball.ball_moving_west = false; ball.ball_moving_east = true;
	}
	else if (square_ball.right_x >= game_field.x[const_right_down_point])
	{
		ball.ball_moving_west = true; ball.ball_moving_east = false;
	}

	return ball;
}

// �������� ������� ����� � ������
CIRCLE CHECK_TOUCH::CheckTouchBallAndBoard(RECTANGLE board, CIRCLE ball)
{
	// ����������� ��� � �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ����� ����� �� ��� �����
	float x_distance_board_one_part = (board.x[const_right_up_point] - board.x[const_left_up_point]) / const_count_side_board;

	// ���������� ����� � ������ ������� ����� ������ �� ���� ������ �����
	float left_part_left_edge    = board.x[const_left_down_point];
	float left_part_right_edge   = board.x[const_left_down_point] + x_distance_board_one_part;

	float middle_part_left_edge  = board.x[const_left_down_point] + x_distance_board_one_part;
	float middle_part_right_edge = board.x[const_right_up_point ] - x_distance_board_one_part;

	float right_part_left_edge   = board.x[const_right_up_point ] - x_distance_board_one_part;
	float right_part_right_edge  = board.x[const_right_up_point ];

	// ���� ����� ������� �� �����, ������ ����������� �� �����
	if (square_ball.down_y <= board.y[const_left_up_point])
	{
		ball.ball_moving_north = true; 		ball.ball_moving_south = false;

		// ���� ����� �������� � ����� �����.
		if (square_ball.right_x >= left_part_left_edge && square_ball.center_x <= left_part_right_edge)
		{
			ball.ball_moving_west = true;
			ball.ball_moving_east = false;
		}
		// ���� ����� �������� � ������� �����.
		else if (square_ball.center_x >= middle_part_left_edge && square_ball.center_x <= middle_part_right_edge)
		{
			ball.ball_moving_west = false;
			ball.ball_moving_east = false;
		}
		// ���� ����� �������� � ������ �����.
		else if (square_ball.center_x >= left_part_right_edge && square_ball.left_x <= right_part_right_edge)
		{
			ball.ball_moving_west = false;
			ball.ball_moving_east = true;
		}
		else exit(0); // ���� ����� � ����� �� �����
	}
	return ball;
}

// �������� ������� �������� � ������
CIRCLE CHECK_TOUCH::CheckTouchBricks(RECTANGLE* bricks, CIRCLE ball)
{
	CIRCLE local_ball = ball; // ��������� ����� ��� ��������� �������� ������ � ���������
	bool object_eqaul;

	// �������� �� ���� ��������
	for (int i = 0; i < const_count_bricks; i++)
	{
		if (bricks->y[const_left_up_point] != 0) // ���� ������ ����
		{
			// ��������� ������� ������� ����� ������ � ������ ������� �������
			ball = CHECK_TOUCH::CheckTouchUpSideBallAndDownSideBrick(bricks, ball);

			// ���� ������� ����, �� ����� ������� �����������
			object_eqaul = CIRCLE::CompareCircles(local_ball, ball);

			// ���� ����� ��������� ���������� � ������ ���������� �����
			if (object_eqaul != true) 	return ball;

			ball = CHECK_TOUCH::CheckTouchDownSideBallAndUpSideBrick(bricks, ball);
			object_eqaul = CIRCLE::CompareCircles(local_ball, ball);
			if (object_eqaul != true) 	return ball;


			ball = CHECK_TOUCH::CheckTouchLeftSideBallAndRightSideBrick(bricks, ball);
			object_eqaul = CIRCLE::CompareCircles(local_ball, ball);
			if (object_eqaul != true) 	return ball;


			ball = CHECK_TOUCH::CheckTouchRightSideBallAndLeftSideBrick(bricks, ball);
			object_eqaul = CIRCLE::CompareCircles(local_ball, ball);
			if (object_eqaul != true) 	return ball;
		}
		bricks++;
	}
	return ball;		
}

// �������� ������� ������� ����� ������ � ������ ������� �������
CIRCLE CHECK_TOUCH::CheckTouchUpSideBallAndDownSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// ����������� ��� � �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ���� ���� ���� �������� ���� �������
	if (square_ball.up_y >= brick->y[const_left_down_point] && square_ball.up_y < brick->y[const_left_up_point])
	{
		if (square_ball.right_x >= brick->x[const_left_down_point] && square_ball.left_x <= brick->x[const_right_down_point])
		{
			// ������ ����������� ������ � ������� ����� ������
			ball.ball_moving_north = false; ball.ball_moving_south = true;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchDownSideBallAndUpSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// ����������� ��� � �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ���� ��� ���� �������� ����� �������
	if (square_ball.down_y <= brick->y[const_left_up_point] && square_ball.down_y > brick->y[const_left_down_point])
	{
		if (square_ball.right_x >= brick->x[const_left_up_point] && square_ball.left_x <= brick->x[const_right_up_point])
		{
			// ������ ����������� ������ � ������� ����� ������
			ball.ball_moving_north = true; ball.ball_moving_south = false;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchLeftSideBallAndRightSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// ����������� ��� � �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ���� ����� ������� ������ ��������� ������ ������� �������
	if (square_ball.left_x <= brick->x[const_right_up_point] && square_ball.left_x > brick->x[const_left_up_point])
	{
		if (square_ball.up_y >= brick->y[const_right_down_point] && square_ball.down_y <= brick->y[const_right_up_point])
		{
			// ������ ����������� ������ � ������� ����� ������
			ball.ball_moving_east = true; ball.ball_moving_west = false;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchRightSideBallAndLeftSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// ����������� ��� � �������
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// ���� ������ ������� ������ ��������� ����� ������� �������
	if (square_ball.right_x >= brick->x[const_left_down_point] && square_ball.right_x < brick->x[const_right_up_point])
	{
		if (square_ball.up_y >= brick->y[const_left_down_point] && square_ball.down_y <= brick->y[const_left_up_point])
		{
			// ������ ����������� ������ � ������� ����� ������
			ball.ball_moving_east = false; ball.ball_moving_west = true;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

// ������� �������� �������
void RECTANGLE::EraseBrick (RECTANGLE* brick)
{
	for (int i = 0; i < const_count_point_in_rectangle; i++)
	{
		brick->x[i] = 0;
		brick->y[i] = 0;
	}
}

// ������� �������������� ���� � �������
SQUARE BALL_TO_SQUARE::ConvertBallToSquare(CIRCLE ball)
{
	SQUARE square_ball; // ����������� ����� � �������

	square_ball.left_x    = ball.x_pozition_center - ball.radius;
	square_ball.right_x   = ball.x_pozition_center + ball.radius;
	square_ball.up_y      = ball.y_pozition_center + ball.radius;
	square_ball.down_y    = ball.y_pozition_center - ball.radius;
	square_ball.center_x  = ball.x_pozition_center;

	return square_ball;
}

// ��������� ����������� �������� �������� � ��������� ������
bool CIRCLE::CompareCircles(CIRCLE var_one, CIRCLE var_two)
{
	// ���� ����������� �������� �������� � ��������� ������� �� �����
	if (var_one.ball_moving_east  != var_two.ball_moving_east )  return false;
	if (var_one.ball_moving_north != var_two.ball_moving_north) return false;
	if (var_one.ball_moving_south != var_two.ball_moving_south) return false;
	if (var_one.ball_moving_west  != var_two.ball_moving_west )  return false;

	return true;
}




