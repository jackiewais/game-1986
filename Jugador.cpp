#include "Jugador.h"
#include <iostream>

using namespace std;

Jugador::Jugador(SDL_Renderer* gRend, int scr_width, int scr_height)
{
	gJugadorTexture.gRenderer = gRend;

    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    frame = 0;

    screen_width = scr_width;
    screen_height = scr_height;

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
	}else
	{
		for (unsigned int i=0;i < JUGADOR_ANIMATION_FRAMES ;i++){
			gSpriteClips[ i ].x =   JUG_WIDTH * i;
			gSpriteClips[ i ].y =   0;
			gSpriteClips[ i ].w =  JUG_WIDTH;
			gSpriteClips[ i ].h =  JUG_HEIGHT;
		}
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
    if( ( mPosX < 0 ) || ( mPosX + JUG_WIDTH > screen_width ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + JUG_HEIGHT > screen_height ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Jugador::render()
{
	//Render current frame
	SDL_Rect* currentClip = &gSpriteClips[ frame / 6 ];
	gJugadorTexture.render( mPosX, mPosY, currentClip );
	++frame;
	if( frame / 6 >= JUGADOR_ANIMATION_FRAMES ){
		frame = 0;
	}

}

Jugador::~Jugador()
{
	gJugadorTexture.free();
}
