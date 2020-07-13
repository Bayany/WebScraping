#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include "sstream"
#undef main
using namespace std;
const int SCREEN_WIDTH = 494;
const int SCREEN_HEIGHT = 690;
#include "blocks.h"
#include "balls.h"
blocks brick[8][6];
SDL_Window* window = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Surface* winsurf = NULL;
SDL_Rect balldst[100];
SDL_Rect ballsrc[100];
SDL_Texture* balltex[100];
bool pause1 = true;
int red, green, blue;
int ballcount = 1;
float teta = 1.51;
int click;
int stage = 1;
int flag1 = 0;
int count = 0, count1 = 0;
float xstart = 247, ystart = 579;
//float teta = 1.51;
int first = 0, firstobject = 0;
bool quit = false;
int status = 0;
int mousex = NULL, mousey = NULL;
int max = 0;
string whichbird;
int birdnum = 0;
Mix_Music* music = NULL;
TTF_Font* font = NULL;
TTF_Font* font1 = NULL;
SDL_Rect textdst;
SDL_Texture* texy;
SDL_Surface* textsurf = NULL;
balls birds[100];
static int score = 0;

void savinghighscore() {
	int high_score;
	ifstream input("high_score.txt");
	input >> high_score;
	ofstream output("high_score.txt");
	if (stage > high_score) {
		output << stage;
	}
	else{
		output << high_score;
	}
}
void getallscore() {
	for (int i = 0; i < ballcount; i++) {
		score += birds[i].getscore;
}
}
void resetgame() {


	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 6; j++)
			brick[i][j].life = 0;

	ballcount = 1;
	stage = 1;


}
void setBall(string filename, SDL_Renderer* gRenderer, int x, int y, int w, int i) {
	balldst[i] = { x,y,w,w };
	ballsrc[i] = { 0,0,1000,1000 };
	SDL_Surface* surf = IMG_Load(filename.c_str());
	balltex[i] = SDL_CreateTextureFromSurface(gRenderer, surf);
	//SDL_RenderCopyEx(gRenderer, balltex[i], &ballsrc[i], &balldst[i], 0, NULL, SDL_FLIP_NONE);
}
void copyball(SDL_Renderer* gRenderer, int x, int y, int w, int i) {
	balldst[i] = { x,y,w,w };
	ballsrc[i] = { 0,0,1000,1000 };
	SDL_RenderCopyEx(gRenderer, balltex[i], &ballsrc[i], &balldst[i], 0, NULL, SDL_FLIP_NONE);
}
void bricknum(blocks brick[8][6], int i, int j, int x, int y)
{
	int w = 80;
	int h = 80;
	if (brick[i][j].life > max) max = brick[i][j].life;
	string str;
	str = to_string(brick[i][j].life);
	SDL_Color color = { 1,0,0 };
	textsurf = TTF_RenderText_Solid(font, str.c_str(), color);
	texy = SDL_CreateTextureFromSurface(gRenderer, textsurf);
	SDL_QueryTexture(texy, NULL, NULL, &w, &h);
	textdst = { x, y, w, h };

	SDL_RenderCopyEx(gRenderer, texy, NULL, &textdst, 0.0, NULL, SDL_FLIP_NONE);
	// SDL_DestroyTexture(texy);
	 //SDL_FreeSurface(textsurf);
}
SDL_Texture* texy1;
SDL_Rect  textdst1;
void ballnum(int x, int y)
{
	int w1 = 150;
	int h1 = 200;

	string str1;
	str1 = to_string(ballcount);
	SDL_Color color1 = { red,green,blue };
	textsurf = TTF_RenderText_Solid(font1, str1.c_str(), color1);
	texy1 = SDL_CreateTextureFromSurface(gRenderer, textsurf);
	SDL_QueryTexture(texy1, NULL, NULL, &w1, &h1);
	textdst1 = { x, y, w1, h1 };

	SDL_RenderCopyEx(gRenderer, texy1, NULL, &textdst1, 0.0, NULL, SDL_FLIP_NONE);

}
SDL_Surface* a;
SDL_Texture* b;
SDL_Rect textdst2;
void scoretext(int x, int y)
{
	int w2 = 150;
	int h2 = 200;

	string str2="SCORE : ";
	str2 += to_string(stage);
	SDL_Color color2 = { red,green,blue };
	a = TTF_RenderText_Solid(font1, str2.c_str(), color2);
	b = SDL_CreateTextureFromSurface(gRenderer, a);
	SDL_QueryTexture(b, NULL, NULL, &w2, &h2);
	textdst2 = { x, y, w2, h2 };

	SDL_RenderCopyEx(gRenderer, b, NULL, &textdst2, 0.0, NULL, SDL_FLIP_NONE);

}

void showbestscore(int x,int y) {
	int w2 = 150;
	int h2 = 200;
	int high;
	fstream input("high_score.txt");
	input >> high;
	string str2 = "Best Score : ";
	str2 += to_string(high);
	SDL_Color color2 = { red,green,blue };
	a = TTF_RenderText_Solid(font1, str2.c_str(), color2);
	b = SDL_CreateTextureFromSurface(gRenderer, a);
	SDL_QueryTexture(b, NULL, NULL, &w2, &h2);
	textdst2 = { x, y, w2, h2 };

	SDL_RenderCopyEx(gRenderer, b, NULL, &textdst2, 0.0, NULL, SDL_FLIP_NONE);
}
void init_brick() {

	int w, h = 152;
	brick[8][6].life;

	for (int i = 0; i < 8; i++)
	{
		w = 0;
		for (int j = 0; j < 6; j++)
		{
			brick[i][j].x = w;
			brick[i][j].y = h;
			w += 82;
		}
		h += 52;
	}
}

void createRandom(blocks brick[8][6], int stage)
{
	int bricknum;
	if (stage <= 12)
		bricknum = rand() % 2 + 1;

	else if (stage > 12)
		bricknum = rand() % 5 + 1;

	int random[6];
	fill_n(random, 6, 0);
	//init the egg
	int r = rand() % 6;
	random[r] = -1;
	brick[0][r].life = -1;
	for (int i = 1; i <= bricknum; i++)
	{
		do { r = rand() % 6; } while (random[r] == -1 || brick[0][r].life > 0);

		brick[0][r].life = stage;

	}
}
void shift(blocks brick[8][6], int stage) {

	for (int i = 6; i >= 0; i--)
	{

		for (int j = 0; j < 6; j++)
		{
			brick[i + 1][j].life = brick[i][j].life;
			brick[i][j].life = 0;
		}
	}


}
void setAllBricks(blocks brick[8][6]) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (brick[i][j].life == -1) {
				brick[i][j].setbrick("stuff/egg.png", gRenderer, brick[i][j].x + 40, brick[i][j].y + 25, 7, 7);
			}
			else if (brick[i][j].life > 0 && brick[i][j].life <= 5)
				brick[i][j].setbrick("stuff/brick.png", gRenderer, brick[i][j].x, brick[i][j].y, brick[i][j].Bwidth, brick[i][j].Bheight);

			else if (brick[i][j].life > 5)
				brick[i][j].setbrick("stuff/brick2.png", gRenderer, brick[i][j].x, brick[i][j].y, brick[i][j].Bwidth, brick[i][j].Bheight);
		}
	}

}



void ShowBlock(blocks brick[8][6])
{
	for (int i = 0; i <8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (brick[i][j].life > 0) {
				brick[i][j].copybrick(gRenderer, brick[i][j].x, brick[i][j].y, brick[i][j].Bwidth, brick[i][j].Bheight);
				bricknum(brick, i, j, brick[i][j].x + 38, brick[i][j].y + 10);
			}

			else if (brick[i][j].life == -1)
				brick[i][j].copybrick(gRenderer, brick[i][j].x + 40, brick[i][j].y + 5, 40, 40);


		}
	}
}
void drawline()
{
	SDL_SetRenderDrawColor(gRenderer, 1, 1, 1, 255);
	for (int i = 1; i <= 5; i++)
	{
		SDL_RenderDrawLine(gRenderer, 0, 95 + i, 494, 95 + i);
		SDL_RenderDrawLine(gRenderer, 0, 585 + i, 494, 585 + i);
	}
}




bool isshot(int ballcount, balls birds[100])
{
	bool shot = false;
	for (int i = 0; i < ballcount; i++)
	{
		if (birds[i].exist) {
			shot = true;

		}
	}
	return shot;
}



void dashedline(float x, float y)
{


	int space = 0;
	SDL_SetRenderDrawColor(gRenderer, red, green, blue, 255);

	while ((SCREEN_WIDTH >= x && x >= 0) && y >= 102)
	{
		if (space > 3) {
			SDL_RenderDrawPoint(gRenderer, x, y);
			SDL_RenderDrawPoint(gRenderer, x + 1, y);
		}
		space++;

		if (space > 10)
			space -= 10;

		x += cos(teta);
		y -= sin(teta);
	}

}

bool gameover(blocks brick[8][6])
{

	for (int i = 0; i < 6; i++)
	{
		if (brick[7][i].life > 0)
			return true;
		if (brick[7][i].life == -1)
			ballcount++;
	}
	return false;
}

void setButtons() {
	reset.setbrick("stuff/reset.png", gRenderer, 10, 10, 100, 100);
	pause.setbrick("stuff/pause.png", gRenderer, 10, 10, 100, 100);
}

void gamecontrol() {
	SDL_Event e;
	setAllBricks(brick);
	setButtons();
	for (int i = 0; i < 1; i++) {
		setBall(whichbird, gRenderer, birds[i].x, birds[i].y, 30, i);
	}
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
			SDL_Quit();
		}
		else if (e.type == SDL_KEYDOWN)
		{

			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			{
				if (teta < 0.1)teta = 0.1;
				teta += 0.01;
			}
			break;
			case SDLK_RIGHT:
			{
				if (teta < 0.1)teta = 0.1;
				teta -= 0.01;
			}
			break;
			case SDLK_SPACE:
			{
				for (int i = 0; i < 1; i++)
				{
					birds[i].exist = 1;
					birds[i].vx = 0.25 * cos(teta);
					birds[i].vy = 0.25 * sin(teta);
				}
			}
			break;
			}
		}
		//cout << "x= " << mousex << " y= " << mousey <<1<< endl;
			//SDL_GetMouseState(&mousex, &mousey);
			//cout << "x= " << mousex << " y= " << mousey << endl;
		 if (e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&mousex, &mousey);
			
			if ((455 > mousex && mousex > 405) && (80 > mousey && mousey > 20)) {
				pause1 = false;
				status = 0;
				return;
			}
			if ((380 > mousex && mousex > 320) && (80 > mousey && mousey > 20)) {
				click++;
				if (click % 2)
					pause1 = false;
				else pause1 = true;

			}


		}
	}
}
SDL_Texture* tex1;

void loadbackground() {
	SDL_Surface* background1 = IMG_Load("stuff/background.jpg");
	tex1 = SDL_CreateTextureFromSurface(gRenderer, background1);
	SDL_RenderCopy(gRenderer, tex1, NULL, NULL);
	SDL_RenderPresent(gRenderer);

}
			

void gameloop() {
	createRandom(brick, stage);

	while ((!quit) && pause1)
	{
		//cout << "yes" << endl;
			//	cout << "x= " << mousex << " y= " << mousey << endl;

			SDL_RenderCopy(gRenderer, tex1, NULL, NULL);
			while ((!(isshot(ballcount, birds)) && (!quit))&& pause1)
			{
				//readhighscore();
				SDL_RenderCopy(gRenderer, tex1, NULL, NULL);
				drawline();
				copyball(gRenderer, 10, 600, 70, 0);
				ballnum(100, 620);
				showbestscore(30, 45);
				reset.copybrick(gRenderer, 400, 20, 60, 60);
				pause.copybrick(gRenderer, 320, 20, 60, 60);
				scoretext(30, 12);
				ShowBlock(brick);
				dashedline(xstart, ystart);
				gamecontrol();

				for (int i = 0; i < ballcount; i++) {

					copyball(gRenderer, birds[i].x - 15, birds[i].y - 20, 30, i);
				}
				SDL_RenderPresent(gRenderer);
			}




			int count = 1;
			flag1 = true;
			while (((isshot(ballcount, birds)) && (!quit)) && pause1)
			{

				SDL_RenderCopy(gRenderer, tex1, NULL, NULL);
				reset.copybrick(gRenderer, 400, 20, 60, 60);
				pause.copybrick(gRenderer, 320, 20, 60, 60);
				copyball(gRenderer, 10, 600, 70, 0);
				ballnum(100, 620);
				scoretext(30, 12);
				showbestscore(30, 45);
				drawline();
				ShowBlock(brick);
				//ballcount
				for (int p = 0; p < 1; p++)
				{

					copyball(gRenderer, birds[p].x + p * 20, birds[p].y + p * 20, 30, p);
					birds[p].moveball();

				}

				SDL_RenderPresent(gRenderer);
			}

			int temp = ballcount;
			for (int i = 0; i < ballcount; i++)
			{
				temp += birds[i].increaseball;
				if (birds[i].increaseball) birds[i].exist = 1;
			}

			ballcount = temp;
			for (int i = 0; i < ballcount; i++)
				birds[i].increaseball = 0;
			stage++;
			for (int i = 0; i < ballcount; i++)
			{
				if (i != firstobject)
					birds[i].life = 0;
				birds[i].x = birds[firstobject].x2;
				birds[i].y = birds[firstobject].y2;
			}
			xstart = birds[firstobject].x2;
			ystart = birds[firstobject].y2;
			teta = 1.57;
			drawline();
			shift(brick, stage);
			createRandom(brick, stage);
			SDL_RenderPresent(gRenderer);
			if (gameover(brick)) {
				getallscore();
				savinghighscore();
				/*readhighscore();
				if (stage > score) {
					highfile.open("stuff/highscore", ofstream::out | ofstream::trunc);
					highfile << stage << endl;
					highfile.close();
				}*/
				cout << score << endl;
				status = 3;
				return;
			}

		
			
	}
	
}
void birdname() {

	switch (birdnum) {
	case 0: cout << "Red" << endl; break;
	case 1:  cout << "Chuck " << endl; break;
	case 2:  cout << "The Blues" << endl; break;
	case 3:  cout << "Matilda" << endl; break;
	case 4:  cout << "Bomb" << endl; break;
	default: SDL_Quit();
	}

}
void linecolor() {

	switch (birdnum) {
	case 0: red = 255; green = 0; blue = 0; break;
	case 1:  red = 254; green = 220; blue = 86; break;
	case 2:  red = 101; green = 147; blue = 255; break;
	case 3:  red = 255; green = 255; blue = 255; break;
	case 4:  red = 0; green = 0; blue = 0; break;
	default: SDL_Quit();
	}

}


string birdpost;

SDL_Texture* texbird1;
SDL_Texture* texbird2;
SDL_Texture* texbird3;
SDL_Texture* texbird4;
SDL_Texture* texbird5;

void birdposters() {
	switch (birdnum) {
	case 0: SDL_RenderCopy(gRenderer, texbird1, NULL, NULL); whichbird = "stuff/bird1.png"; break;
	case 1: SDL_RenderCopy(gRenderer, texbird2, NULL, NULL); whichbird = "stuff/bird2.png"; break;
	case 2: SDL_RenderCopy(gRenderer, texbird3, NULL, NULL); whichbird = "stuff/bird3.png"; break;
	case 3: SDL_RenderCopy(gRenderer, texbird4, NULL, NULL); whichbird = "stuff/bird4.png"; break;
	case 4: SDL_RenderCopy(gRenderer, texbird5, NULL, NULL); whichbird = "stuff/bird5.png"; break;
	default: SDL_Quit();
	}
}

void loadbirdpost() {
	SDL_Surface* birdwall1 = IMG_Load("stuff/bird1poster.jpg");
	texbird1 = SDL_CreateTextureFromSurface(gRenderer, birdwall1);
	SDL_Surface* birdwall2 = IMG_Load("stuff/bird2poster.jpg");
	texbird2 = SDL_CreateTextureFromSurface(gRenderer, birdwall2);
	SDL_Surface* birdwall3 = IMG_Load("stuff/bird3poster.jpg");
	texbird3 = SDL_CreateTextureFromSurface(gRenderer, birdwall3);
	SDL_Surface* birdwall4 = IMG_Load("stuff/bird4poster.jpg");
	texbird4 = SDL_CreateTextureFromSurface(gRenderer, birdwall4);
	SDL_RenderCopy(gRenderer, texbird4, NULL, NULL);
	SDL_Surface* birdwall5 = IMG_Load("stuff/bird5poster.jpg");
	texbird5 = SDL_CreateTextureFromSurface(gRenderer, birdwall5);


}

void choosebird() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
			SDL_Quit();
			return;
		}
		SDL_GetMouseState(&mousex, &mousey);
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			//it is ok		
			if ((591 > mousey && mousey > 460) && (334 > mousex && mousex > 178)) {
				linecolor();
				status = 2;
				return;
			}

			//it is right arrow
			if ((330 > mousey && mousey > 290) && (470 > mousex && mousex > 360)) {
				if (birdnum == 4)  birdnum = 0;
				else birdnum++;
				birdname();
				choosebird();
			}

			//it is left arrow
			if ((325 > mousey && mousey > 290) && (140 > mousex && mousex > 14)) {
				//cout << "x= " << mousex << " y= " << mousey << endl;
				if (birdnum == 0) birdnum = 4;
				else birdnum--;
				birdname();
				choosebird();
			}
			birdposters();
			SDL_RenderPresent(gRenderer);
		}
	}
}
void startmenu() {
	SDL_Event e;
	SDL_Texture* tex;
	SDL_Surface* background = IMG_Load("stuff/menu.jpg");
	tex = SDL_CreateTextureFromSurface(gRenderer, background);
	SDL_RenderCopy(gRenderer, tex, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	SDL_Surface* screen;
	while (SDL_PollEvent(&e) != 0) {
		SDL_GetMouseState(&mousex, &mousey);

		if (e.type == SDL_QUIT) {
			quit = true;
			SDL_Quit();
			return;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if ((224 > mousey && mousey > 79) && (395 > mousex && mousex > 96)) {
				pause1 = true;
				resetgame();
				status = 1;
				return;
			}
		}
	}
}
SDL_Texture* tex2;
void loadgameover() {
	SDL_Event e;
	SDL_Surface* gameover = IMG_Load("stuff/gameover.jpg");
	tex2 = SDL_CreateTextureFromSurface(gRenderer, gameover);
	showbestscore(30, 10);
	SDL_RenderCopy(gRenderer, tex2, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	while (SDL_PollEvent(&e) != 0) {
		SDL_GetMouseState(&mousex, &mousey);

		if (e.type == SDL_QUIT) {
			quit = true;
			SDL_Quit();
			return;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if ((580 > mousey && mousey > 480) && (380 > mousex && mousex > 270)) {
				resetgame();
				status = 0;
				return;
			}
		}
	}
}


int main()
{
	SDL_Event e;

	SDL_GL_SetSwapInterval(0);
	window = SDL_CreateWindow("ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	srand(time(0));
	init_brick();
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS("stuff/music.wav");
	if (music == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayMusic(music, -1);
	loadbirdpost();
	font = TTF_OpenFont("stuff/font.ttf", 20);
	font1 = TTF_OpenFont("stuff/font.ttf", 30);
	//TTF_CgameoverFont(font);
	//TTF_Quit();
	while (!quit) {
		switch (status) {
		case 0: startmenu();  break;
		case 1: choosebird(); break;
		case 2: loadbackground(); gameloop(); break;
		case 3:loadgameover(); break;
		default: SDL_Quit();
		}

	}


	return 0;
}


// cout<<"x= "<<mousex<<" y= "<<mousey<<endl;