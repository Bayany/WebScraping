#include <SDL_image.h>

class balls
{
public:
	
	float x = 247, y = 579, life = 0, x2 = 247, y2 = 579;
	bool exist = 0;
	float vx, vy;
	int increaseball = 0;
	int getscore = 0;



	bool touchcorner(int i, int j) {
		if ((x < brick[i][j].x) && (pow((x - brick[i][j].x), 2) + pow(y - brick[i][j].y, 2) <= pow(7, 2)))
			return 1;
		if ((x > brick[i][j].x + 80) && (pow((x - brick[i][j].x - 80), 2) + pow(y - brick[i][j].y - 50, 2) <= pow(7, 2)))
			return 1;
		return 0;
	}
	bool touchsidey(int i, int j) {
		if ((y < brick[i][j].y) && (x >= brick[i][j].x && x <= (brick[i][j].x + 80)) && (brick[i][j].y - y <= 7))
			return 1;
		if ((y > brick[i][j].y + 50) && (x >= brick[i][j].x && x <= (brick[i][j].x + 80)) && ((brick[i][j].y + 50 - y) * (-1) <= 7))
			return 1;
		return 0;

	}
	bool touchsidex(int i, int j) {
		if ((x < brick[i][j].x) && (y >= brick[i][j].y && y <= (brick[i][j].y + 50)) && (brick[i][j].x - x <= 7))
			return 1;
		if ((x > brick[i][j].x + 80) && (y >= brick[i][j].y && y <= (brick[i][j].y + 50)) && ((-1) * (brick[i][j].x + 80 - x) <= 7))
			return 1;
		return 0;

	}
	void moveball()
	{
		if (exist) {
			x += vx;
			y -= vy;
		}

			if (vy <0.01 && vy>-0.01)
				vy = 0.01;
			if (x <= 7 || x>=477)
				vx *= -1;
			 if (y <= 110)
				vy *= -1;
			 if (y >= 580)
			{

				x2 = x;
				y2 = y;
				exist = 0;
			}
		

		bool r = true;
		for (int i = 0; i < 7; i++)
		{
			if (r) {
				for (int j = 0; j < 6; j++)
				{
					if (r) {
						if (brick[i][j].life > 0)
						{
							if (touchcorner(i, j)) {
								swap(vx, vy);
								brick[i][j].life--;
	                               getscore++;

							}

							if (touchsidex(i, j)) {
								vx *= -1;
								brick[i][j].life--;
								getscore++;
								//r = false;
							}
							if (touchsidey(i, j)) {
								vy *= -1;
								brick[i][j].life--;
								getscore++;
								//r = false;
							}
						}

						else if (brick[i][j].life == -1)
						{
							if (((x - brick[i][j].x - 40) * (x - brick[i][j].x - 40)) + ((y - brick[i][j].y - 25) * (y - brick[i][j].y - 4)) <= 400)
							{
								brick[i][j].life = 0;
								increaseball++;
								r = false;
							}
						}
					}
					else break;
				}
			}else break;
			}
		}
	

};