#include "Jugador.h"
#include <iostream>

using namespace std;

Jugador::Jugador(SDL_Renderer* gRend)
{
	gJugadorTexture.gRenderer = gRend;

    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

	//Load media
	if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}
}

bool Jugador::loadMedia()
{
	//Loading success flag
	bool success = true;

	if( !gJugadorTexture.loadFromFile( "sprites/spriteJugador.bmp" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}

	return success;
}
void Jugador::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= JUG_VEL; break;
            case SDLK_DOWN: mVelY += JUG_VEL; break;
            case SDLK_LEFT: mVelX -= JUG_VEL; break;
            case SDLK_RIGHT: mVelX += JUG_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += JUG_VEL; break;
            case SDLK_DOWN: mVelY -= JUG_VEL; break;
            case SDLK_LEFT: mVelX += JUG_VEL; break;
            case SDLK_RIGHT: mVelX -= JUG_VEL; break;
        }
    }
}

void Jugador::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + JUG_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + JUG_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Jugador::render()
{
    //Show the dot
	gJugadorTexture.render( mPosX, mPosY );
}
