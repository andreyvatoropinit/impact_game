#include "stdafx.h"
#include <gl/glut.h>
#include <math.h>
#include "controller.h"
#include "open_gl.h"
#include "model.h"

// класс рисования прямоугольника
class DRAW_RECTANGLE
{
	public:

	static void DrawRectangle(RECTANGLE rectangle)
	{
		// опен жль функции 
		glLineWidth(LINEWIDTH);
		glColor3f(WHITE);
		glBegin(GL_LINE_STRIP);

		for (int i = 0; i < const_count_point_in_rectangle; i++)
		{
			glVertex2i(rectangle.x[i], rectangle.y[i]); // вершины
		}
		glEnd();
	}
};


// класс рисования шарика 
class DRAW_CIRCLE
{
	public:
	static void drawCircle(CIRCLE ball)
	{
		glBegin(GL_LINE_LOOP);

		float angle, dx, dy;

		for (int i = zero; i < ball.amount_segments; i++)
		{
			angle = 2.0 * pi * float(i) / float(ball.amount_segments);

			dx = ball.radius * cosf(angle);
			dy = ball.radius * sinf(angle);

			glVertex2f(ball.x_pozition_center + dx, ball.y_pozition_center + dy);
		}

		glEnd(); 
	}
};

// функция рисования опен жль. вызывается из модели
void DRAW::Draw (RECTANGLE game_field, RECTANGLE board, RECTANGLE* bricks, CIRCLE ball)
{
	// очищаем экран
	glClear(GL_COLOR_BUFFER_BIT);

	// рисуем игровое поле
	DRAW_RECTANGLE::DrawRectangle(game_field);

	// рисуем доску
	DRAW_RECTANGLE::DrawRectangle(board);

	// рисуем кирпичи
	for (int i = 0; i < const_count_bricks; i++)
	{
		RECTANGLE rectangle = *(bricks + i);
		DRAW_RECTANGLE::DrawRectangle(rectangle);
	}

	// рисуем шарик
	DRAW_CIRCLE::drawCircle(ball);

	glutSwapBuffers();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	unsigned int width = DIMENSION_OF_MONITOR::GetWidth();
	unsigned int height = DIMENSION_OF_MONITOR::GetHeight();

	// создаем окно с размерами
	glutInitWindowSize(width,height); 
	glutInitWindowPosition(zero, zero); // позиция окна от левого верхнего угла
	glutCreateWindow("Impact"); // создаем окно и его название
		
	// функция обработки нажатий клавиш
	glutKeyboardFunc(KEYBOARD::Keyboard);

	// функция обновления изображения
	glutTimerFunc(refresh_timer, TIMER::Timer, number_of_timer); // функция обновления содержимого экрана
	
	glOrtho(zero, width, zero, height, zero, zero + 1); // начало координат в левом нижнем углу

	glClearColor(zero, zero, zero, zero); // цвет окна
	
	glutMainLoop();
}