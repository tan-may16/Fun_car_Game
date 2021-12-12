#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include<math.h>
#include "ysglfontdata.h"

void DrawCar(double x, double y)
{
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex2d(x+25, y+25);
	glVertex2d(x - 25, y+25);
	glVertex2d(x - 25, y - 25);
	glVertex2d(x+25, y - 25);
	glEnd();
}

void DrawTarget(double tx, double ty, double tw, double th)
{
	glColor3ub(0, 255, 0);
	glBegin(GL_QUADS);
	glVertex2d(tx- tw / 2, ty+ th / 2);
	glVertex2d(tx + tw/2, ty + th / 2);
	glVertex2d(tx + tw/2, ty - th/2);
	glVertex2d(tx - tw / 2, ty - th/2);
	glEnd();
}

void DrawObstacle(double ox, double oy, double ow, double oh)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(ox - ow / 2, oy + oh / 2);
	glVertex2d(ox + ow / 2, oy + oh / 2);
	glVertex2d(ox + ow / 2, oy - oh / 2);
	glVertex2d(ox - ow / 2, oy - oh / 2);
	glEnd();
}
double CheckHitObstacle(
	double CarX, double CarY,
	double ObstacleX, double ObstacleY, double ObstacleW, double ObstacleH)
{
	double relX = abs(CarX - ObstacleX);
	double relY = ObstacleY - CarY - 25;

	if (0 <= relX && relX < ObstacleW && -25 - ObstacleH / 2.0 <= relY && relY < ObstacleH / 2)
	{
		return 1;
	}
	return 0;
}
double CheckHitTarget(
	double CarX, double CarY,
	double targetX, double targetY, double targetW, double targetH)
{
	double relX = abs(CarX - targetX);
	double relY = targetY - CarY-25;

	if (0 <= relX && relX < targetW && -25-targetH <= relY && relY < targetH/2)
	{
		return 1;
	}
	return 0;
}

int main(void)
{
	double x, y, tx[3], ty[3], tw=80.0, th[3], tv[3], tstate[3],dt, ox[3], oy[3], ow = 80.0, oh[3], ov[3], ostate[3];
	int count,End, time_count;
	char count_s;
	srand(time(nullptr));
	
	FsOpenWindow(0, 0, 600, 750, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslated(0, 750, 0);
	glRotated(180, 1, 0, 0);

	x = 300; y = 75; dt = 0.025;
	count = 0;
	End = 0;
	time_count = 0;
	for (int i = 0; i < 3; ++i)
	{
		tstate[i] = 1;
		tx[i] = 100.0 + 200.0 * i;
		ty[i] = 100.0 +rand() % 600;
		th[i] = 10.0 + rand() % 40;
		tv[i] = 200.0 + rand() % 500;
		ostate[i] = 1;
		ox[i] = 100.0 + 200.0 * i;
		oy[i] = 750;
		oh[i] = 10.0 + rand() % 40;
		ov[i] =  40.0+ rand() % 400;
	}
	
	for (;;)
	{
		FsPollDevice();
		glBegin(GL_LINES);
		glVertex2d(300, 0);
		glVertex2d(300, 750);
		glEnd();
		auto  key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}
		
		//Moving

		if (FSKEY_LEFT == key)
		{
			if (x == 300 || x == 500)
			{
				x -= 200;
			}
		}
		else if (FSKEY_RIGHT == key)
		{
			if (x == 100 || x == 300)
			{
				x += 200;
			}
		}

		else if (FSKEY_UP == key)
		{
			if (y <= 700.0)
			{
				y += 50.0;
			}

		}
		else if (FSKEY_DOWN == key)
		{
			if (y >= 75.0)
			{
				y -= 50.0;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (0 != tstate[i])
			{

				if (ty[i] - th[i] / 2.0 <= 0)
				{
					ty[i] = 750;
					tv[i] = 100.0 + rand() % 500;
					th[i] = 10.0 + rand() % 40;
				}
				ty[i] = ty[i] - tv[i] * dt;
			}
			if (0 != ostate[i])
			{

				if (oy[i] - oh[i] / 2.0 <= 0)
				{
					oy[i] = 750;
					ov[i] = 200.0 + rand() % 500;
					oh[i] = 10.0 + rand() % 40;
				}
				oy[i] = oy[i] - ov[i] * dt;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (0 == tstate[i])
			{
				count += 1;
				ty[i] = 750;
				tv[i] = 150.0 + rand() % 500;
				th[i] = 10.0 + rand() % 40;
				tstate[i] = 1;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (0 == ostate[i])
			{
				glColor3ub(0, 0, 255);
				glRasterPos2d(200, 400);
				YsGlDrawFontBitmap20x28("Game Over!!");
				End = 1;
			}
			

		}
		if (End == 1)
		{
			time_count += 1;
			if (time_count >= 20)
			{
				break;
			}
			
		}
		//Checking
		for (int i = 0; i < 3; i++)
		{
			if (0 != CheckHitTarget(x, y, tx[i], ty[i], tw, th[i]))
			{
				
				tstate[i] = 0;
			}
			if (0 != CheckHitObstacle(x, y, ox[i], oy[i], ow, oh[i]))
			{
				
				ostate[i] = 0;
			}
		}
		


		// Rendering >>

		
		for (int i = 0; i < 3; i++)
		{
			if (0 != tstate[i])
			{
				DrawTarget(tx[i], ty[i], tw, th[i]);
			}
			if (0 != ostate[i])
			{
				DrawObstacle(ox[i], oy[i], ow, oh[i]);
			}
		}
		

		DrawCar(x, y);
		
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0xff00);
		glColor3ub(255, 255, 0);
		glBegin(GL_LINES);

		glVertex2d(10, 0);
		glVertex2d(10, 750);
		
		glVertex2d(160, 0);
		glVertex2d(160, 750);

		glVertex2d(220, 0);
		glVertex2d(220, 750);

		glVertex2d(360, 0);
		glVertex2d(360, 750);

		glVertex2d(420, 0);
		glVertex2d(420, 750);

		glVertex2d(590, 0);
		glVertex2d(590, 750);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		
		glColor3ub(255, 255, 0);
		for (int i = 0; i < 12; i++)
		{
			
			glBegin(GL_LINES);
			glVertex2d(160, 60.0 * i);
			glVertex2d(220, 60.0 * (i + 1.0));

			glVertex2d(360, 60.0 * i);
			glVertex2d(420, 60.0 * (i + 1.0));
			glEnd();
		}
		FsSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FsSleep(25);  
		
		glShadeModel(GL_SMOOTH);
		glBegin(GL_QUADS);
		glColor3ub(128, 128, 128);
		glVertex2i(0, 0);
		glColor3ub(128, 128, 128);
		glVertex2i(600, 0);
		glColor3ub(100, 100, 100);
		glVertex2i(600, 750);
		glColor3ub(100, 100, 100);
		glVertex2i(0, 750);
		glEnd();
	}
	
	printf(" Score - %i", count);
	return 0;
}