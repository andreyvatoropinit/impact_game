#pragma once

#ifndef check_touch_something
#define check_touch_something


class CHECK_TOUCH
{
	public:

	static CIRCLE CheckTouch              (RECTANGLE game_field, RECTANGLE board, CIRCLE ball);
	static CIRCLE CheckTouchBallAndWall   (RECTANGLE game_field,CIRCLE ball);
	static CIRCLE CheckTouchBallAndBoard  (RECTANGLE board, CIRCLE ball);
	static CIRCLE CheckTouchBricks        (RECTANGLE* bricks, CIRCLE ball);

	static CIRCLE CheckTouchUpSideBallAndDownSideBrick     (RECTANGLE* brick, CIRCLE ball);

	static CIRCLE CheckTouchLeftSideBallAndRightSideBrick  (RECTANGLE* brick, CIRCLE ball);
	static CIRCLE CheckTouchRightSideBallAndLeftSideBrick  (RECTANGLE* brick, CIRCLE ball);
	static CIRCLE CheckTouchDownSideBallAndUpSideBrick     (RECTANGLE* brick, CIRCLE ball);
};


class SQUARE
{
	public:

	int left_x;
	int right_x;
	int up_y;
	int down_y;
	int center_x;
};

class BALL_TO_SQUARE
{
	public:

	static SQUARE ConvertBallToSquare(CIRCLE ball);
};


#endif
