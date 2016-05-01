#include "LTexture.h"
#include <iostream>
#include <list>

using namespace std;

class BackgroundTemplate
{
    public:

		LTexture textura;
		BackgroundTemplate(SDL_Renderer* gRender, string path, int x,int y, int height, int scrH );
		void render(int scrollingOffset);

		~BackgroundTemplate();
    private:
		int posX;
		int posY;
		
		int height,scrH;


};
