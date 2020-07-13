#include <SDL_image.h>
#include <SDL_ttf.h>
using namespace std;
class blocks
{
  public:
    const int Bwidth = 80;
    const int Bheight = 50;
    int x, y, life;
	SDL_Rect brickdst;
	SDL_Rect bricksrc;
	SDL_Texture* bricktex;
	
	void setbrick(string filename, SDL_Renderer* gRenderer, int x, int y, int w,int h) {
		brickdst = { x,y,w,h };
		bricksrc = { 0,0,1000,1000 };
		SDL_Surface* bricksurf = IMG_Load(filename.c_str());
		bricktex = SDL_CreateTextureFromSurface(gRenderer, bricksurf);
	}
	void copybrick( SDL_Renderer* gRenderer, int x, int y, int w, int h) {
		brickdst = { x,y,w,h };
		bricksrc = { 0,0,1000,1000 };
		SDL_RenderCopyEx(gRenderer, bricktex, &bricksrc, &brickdst, 0, NULL, SDL_FLIP_NONE);
	}
	
   
}pause, reset;