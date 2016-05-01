#include "BackgroundTemplate.h"	

using namespace std;
BackgroundTemplate::BackgroundTemplate(SDL_Renderer* gRender, int x, int y, string path)
{
	textura.gRenderer=gRender;
	if( !textura.loadFromFile  ( path ) )// ( "sprites/spriteJugador.bmp" ) )
	{
		printf( "Failed to load ball texture!\n" );
		
	}
	this->posX=x;
	this->posY=y;

}
void BackgroundTemplate::render(int scrollingOffset)
{
	textura.render(this->posX,scrollingOffset-this->posY);
}
	