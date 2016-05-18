#include "Pelota.h"
#include <iostream>

using namespace std;


Pelota::Pelota(LTexture* gText, int posIX, int posIY, int velDisparo)
{
	gPelotaTexture = *gText;
	mPosX = posIX-(PEL_WIDTH/2);
	mPosY = posIY;
	isOut = false;
	PEL_VEL = velDisparo;
}

/*void Pelota::setData(SDL_Renderer* gRend)
{
	gPelotaTexture.gRenderer = gRend;

	//Load media
	if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}
}

bool Pelota::loadMedia()
{
	//Loading success flag
	bool success = true;

	if( !gPelotaTexture.loadFromFile  ( "sprites/pelota.png" ) )// ( "sprites/spriteJugador.bmp" ) )
	{
		printf( "Failed to load ball texture!\n" );
		success = false;
	}

	return success;
}*/



void Pelota::patear( int posIX, int posIY){
    mPosX = posIX-(PEL_WIDTH/2);
    mPosY = posIY;
    isOut = false;
}


bool Pelota::ballIsOut(){
	return isOut;
}

void Pelota::move()
{
    //Move the dot up or down
    mPosY -= PEL_VEL;

    //If the dot went too far up or down
    if( mPosY + PEL_HEIGHT < 0 )
        isOut = true;
}

void Pelota::render()
{
	gPelotaTexture.render( mPosX, mPosY, 0 );

}

Pelota::~Pelota()
{
	gPelotaTexture.free();
}

