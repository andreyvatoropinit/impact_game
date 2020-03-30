#include "stdafx.h"
#include <cstdlib>
#include "controller.h"
#include "model.h"
#include "check_touch_something.h"

// проверка касания игрового поля, доски, мячика
CIRCLE CHECK_TOUCH::CheckTouch(RECTANGLE game_field, RECTANGLE board, CIRCLE input_ball)
{
	CIRCLE local_ball = input_ball;

	// проверяем касание мячика и игрового поля
	input_ball = CHECK_TOUCH::CheckTouchBallAndWall(game_field, input_ball);

	// сравниваем мячик.
	bool object_eqaul = CIRCLE::CompareCircles(input_ball, local_ball);

	// если мячик изменил направление - отправляем измененный мячик в модель
	if (object_eqaul != true) return input_ball;

	// проверяем касание доски и мячика
	input_ball = CHECK_TOUCH::CheckTouchBallAndBoard(board, input_ball);

	return input_ball;
}

// проверка касания мячика и кирпичей
CIRCLE CHECK_TOUCH::CheckTouchBallAndWall(RECTANGLE game_field, CIRCLE ball)
{
	// преобразуем мяч в квадрат для упрощения расчета
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// если мячик ударился о верхнюю стену, то меняем движение на юг
	if (square_ball.up_y >= game_field.y[const_left_up_point])
	{
		ball.ball_moving_north = false; ball.ball_moving_south = true;
	}
	// если мячик ударился о дно, то выходим из игры
	else if (square_ball.down_y <= game_field.y[const_left_down_point])
	{
		exit(0); // если шарик упал ниже доски - выходим
	}
	// если мячик ударился об одну из стен, то меняем направление на восток или запад
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

// проверка касания доски и мячика
CIRCLE CHECK_TOUCH::CheckTouchBallAndBoard(RECTANGLE board, CIRCLE ball)
{
	// преобразуем мяч в квадрат
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// делим доску на три части
	float x_distance_board_one_part = (board.x[const_right_up_point] - board.x[const_left_up_point]) / const_count_side_board;

	// определяем левые и правые крайние точки каждой из трех частей доски
	float left_part_left_edge    = board.x[const_left_down_point];
	float left_part_right_edge   = board.x[const_left_down_point] + x_distance_board_one_part;

	float middle_part_left_edge  = board.x[const_left_down_point] + x_distance_board_one_part;
	float middle_part_right_edge = board.x[const_right_up_point ] - x_distance_board_one_part;

	float right_part_left_edge   = board.x[const_right_up_point ] - x_distance_board_one_part;
	float right_part_right_edge  = board.x[const_right_up_point ];

	// если мячик долетел до доски, меняем направление на север
	if (square_ball.down_y <= board.y[const_left_up_point])
	{
		ball.ball_moving_north = true; 		ball.ball_moving_south = false;

		// если мячик ударился о левую часть.
		if (square_ball.right_x >= left_part_left_edge && square_ball.center_x <= left_part_right_edge)
		{
			ball.ball_moving_west = true;
			ball.ball_moving_east = false;
		}
		// если мячик ударился о среднюю часть.
		else if (square_ball.center_x >= middle_part_left_edge && square_ball.center_x <= middle_part_right_edge)
		{
			ball.ball_moving_west = false;
			ball.ball_moving_east = false;
		}
		// если мячик ударился о правую часть.
		else if (square_ball.center_x >= left_part_right_edge && square_ball.left_x <= right_part_right_edge)
		{
			ball.ball_moving_west = false;
			ball.ball_moving_east = true;
		}
		else exit(0); // если мячки в доску не попал
	}
	return ball;
}

// проверка касания кирпичей и мячика
CIRCLE CHECK_TOUCH::CheckTouchBricks(RECTANGLE* bricks, CIRCLE ball)
{
	CIRCLE local_ball = ball; // локальный шарик для сравнения входного шарика и выходного
	bool object_eqaul;

	// проходим по всем кирпичам
	for (int i = 0; i < const_count_bricks; i++)
	{
		if (bricks->y[const_left_up_point] != 0) // если кирпич есть
		{
			// проверяем касание верхней части мячика и нижней стороны кирпича
			ball = CHECK_TOUCH::CheckTouchUpSideBallAndDownSideBrick(bricks, ball);

			// если касание было, то мячик изменил направление
			object_eqaul = CIRCLE::CompareCircles(local_ball, ball);

			// если мячик изменился возвращаем в модель измененный мячик
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

// проверка касания верхней части мячика и нижней стороны кирпича
CIRCLE CHECK_TOUCH::CheckTouchUpSideBallAndDownSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// преобразуем мяч в квадрат
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// если верх мяча коснулся низа кирпича
	if (square_ball.up_y >= brick->y[const_left_down_point] && square_ball.up_y < brick->y[const_left_up_point])
	{
		if (square_ball.right_x >= brick->x[const_left_down_point] && square_ball.left_x <= brick->x[const_right_down_point])
		{
			// меняем направление шарика и стираем битый кирпич
			ball.ball_moving_north = false; ball.ball_moving_south = true;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchDownSideBallAndUpSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// преобразуем мяч в квадрат
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// если низ мяча коснулся верха кирпича
	if (square_ball.down_y <= brick->y[const_left_up_point] && square_ball.down_y > brick->y[const_left_down_point])
	{
		if (square_ball.right_x >= brick->x[const_left_up_point] && square_ball.left_x <= brick->x[const_right_up_point])
		{
			// меняем направление шарика и стираем битый кирпич
			ball.ball_moving_north = true; ball.ball_moving_south = false;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchLeftSideBallAndRightSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// преобразуем мяч в квадрат
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// если левая сторона мячика коснулась правую сторону кирпича
	if (square_ball.left_x <= brick->x[const_right_up_point] && square_ball.left_x > brick->x[const_left_up_point])
	{
		if (square_ball.up_y >= brick->y[const_right_down_point] && square_ball.down_y <= brick->y[const_right_up_point])
		{
			// меняем направление шарика и стираем битый кирпич
			ball.ball_moving_east = true; ball.ball_moving_west = false;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

CIRCLE CHECK_TOUCH::CheckTouchRightSideBallAndLeftSideBrick(RECTANGLE* brick, CIRCLE ball)
{
	// преобразуем мяч в квадрат
	SQUARE square_ball = BALL_TO_SQUARE::ConvertBallToSquare(ball);

	// если правая сторона мячика коснулась левую сторону кирпича
	if (square_ball.right_x >= brick->x[const_left_down_point] && square_ball.right_x < brick->x[const_right_up_point])
	{
		if (square_ball.up_y >= brick->y[const_left_down_point] && square_ball.down_y <= brick->y[const_left_up_point])
		{
			// меняем направление шарика и стираем битый кирпич
			ball.ball_moving_east = false; ball.ball_moving_west = true;
			RECTANGLE::EraseBrick(brick);
		}
	}
	return ball;
}

// функция стирания кирпича
void RECTANGLE::EraseBrick (RECTANGLE* brick)
{
	for (int i = 0; i < const_count_point_in_rectangle; i++)
	{
		brick->x[i] = 0;
		brick->y[i] = 0;
	}
}

// функция преобразования мяча в квадрат
SQUARE BALL_TO_SQUARE::ConvertBallToSquare(CIRCLE ball)
{
	SQUARE square_ball; // преобразуем шарик в квадрат

	square_ball.left_x    = ball.x_pozition_center - ball.radius;
	square_ball.right_x   = ball.x_pozition_center + ball.radius;
	square_ball.up_y      = ball.y_pozition_center + ball.radius;
	square_ball.down_y    = ball.y_pozition_center - ball.radius;
	square_ball.center_x  = ball.x_pozition_center;

	return square_ball;
}

// сравнение направлений движений входного и выходного шарика
bool CIRCLE::CompareCircles(CIRCLE var_one, CIRCLE var_two)
{
	// если направление движения входного и выходного шариков не равны
	if (var_one.ball_moving_east  != var_two.ball_moving_east )  return false;
	if (var_one.ball_moving_north != var_two.ball_moving_north) return false;
	if (var_one.ball_moving_south != var_two.ball_moving_south) return false;
	if (var_one.ball_moving_west  != var_two.ball_moving_west )  return false;

	return true;
}




