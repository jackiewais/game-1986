#include "LTexture.h"
#include <iostream>
#include <list>

using namespace std;

class BackgroundTemplate
{
    public:

		LTexture textura;
		BackgroundTemplate(SDL_Renderer* gRender, int x,int y,string path);
		void render(int scrollingOffset);
    private:
		int posX;
		int posY;
		
};
