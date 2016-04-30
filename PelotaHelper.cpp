#include "PelotaHelper.h"
#include <iostream>

using namespace std;

void PelotaHelper::initTexture(SDL_Renderer* gRend)
{
	gPelotaTexture.gRenderer = gRend;

	//Load media
	if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}
}

bool PelotaHelper::loadMedia()
{
	//Loading success flag
	bool success = true;

	if( !gPelotaTexture.loadFromFile  ( "sprites/pelota.png" ) )// ( "sprites/spriteJugador.bmp" ) )
	{
		printf( "Failed to load ball texture!\n" );
		success = false;
	}

	return success;
}


PelotaHelper::~PelotaHelper()
{
	gPelotaTexture.free();
}
