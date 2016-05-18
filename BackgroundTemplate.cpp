#include "BackgroundTemplate.h"	

using namespace std;
BackgroundTemplate::BackgroundTemplate(SDL_Renderer* gRender, string path, int x, int y, int height, int scrH )
{
	textura.gRenderer=gRender;
	if( !textura.loadFromFile  ( path ) )// ( "sprites/spriteJugador.bmp" ) )
	{
		printf( "Failed to load ball texture!\n" );
		textura.loadFromFile  ( "vacio.bmp" )	;
	}
	
	this->posX=x;
	this->posY=y;
	this->height = height;
	this->scrH = scrH;

}
void BackgroundTemplate::render(int scrollingOffset)
{
	if (scrollingOffset-this->posY + height > 0 && scrollingOffset-this->posY - height < scrH){
		textura.render(this->posX,scrollingOffset-this->posY);
	}
}

BackgroundTemplate::~BackgroundTemplate()
{
	textura.free();
}
	
