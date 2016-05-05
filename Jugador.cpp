#include "Jugador.h"
#include <iostream>

using namespace std;

Jugador::Jugador(SDL_Renderer* gRend, int scr_width, int scr_height, double iWidth)
{
	gJugadorTexture.gRenderer = gRend;

    //Initialize the offsets
    mPosX = scr_width*iWidth;
    mPosY = scr_height-JUG_HEIGHT;

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

	pelotaHelper.initTexture(gJugadorTexture.gRenderer);
}

bool Jugador::loadMedia()
{
	//Loading success flag
	bool success = true;

	if( !gJugadorTexture.loadFromFile  ( "sprites/spriteJugador.png" ) )// ( "sprites/spriteJugador.bmp" ) )
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
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY -= JUG_VEL; break;
            case SDLK_DOWN: mVelY += JUG_VEL; break;
            case SDLK_LEFT: mVelX -= JUG_VEL; break;
            case SDLK_RIGHT: mVelX += JUG_VEL; break;

        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY += JUG_VEL; break;
            case SDLK_DOWN: mVelY -= JUG_VEL; break;
            case SDLK_LEFT: mVelX += JUG_VEL; break;
            case SDLK_RIGHT: mVelX -= JUG_VEL; break;
            case SDLK_SPACE:
            		patear();
                 	break;
        }
    }
}

void Jugador::patear(){
	Pelota* pelota = new Pelota(&pelotaHelper.gPelotaTexture,mPosX+(JUG_WIDTH/2),mPosY);
	lista_pelotas.push_back(pelota);
}

void Jugador::moverPelotas(){
    Pelota* pelToDelete;
    for (list<Pelota *>::iterator it=lista_pelotas.begin(); it != lista_pelotas.end(); ++it){
    	(*it)->move();
    	if ((*it)->ballIsOut())
    		pelToDelete = (*it);
    }

    lista_pelotas.remove(pelToDelete);
}

void Jugador::move()
{
    mPosX += mVelX;
    if( ( mPosX < 0 ) || ( mPosX + JUG_WIDTH > screen_width ) ){
        //Move back
        mPosX -= mVelX;
    }

    mPosY += mVelY;
    if( ( mPosY < 0 ) || ( mPosY + JUG_HEIGHT > screen_height ) ){
        mPosY -= mVelY;
    }

    moverPelotas();
}

void Jugador::forzarPosicion(int x, int y){
	mPosX = x;
	mPosY = y;
}

void Jugador::render()
{
	if (!pausa){
		++frame;
		if( frame / 6 >= JUGADOR_ANIMATION_FRAMES ){
			frame = 0;
		}
	}

	for (list<Pelota *>::iterator it=lista_pelotas.begin(); it != lista_pelotas.end(); ++it)
		(*it)->render();

	SDL_Rect* currentClip = &gSpriteClips[ frame / 6 ];
	gJugadorTexture.render( mPosX, mPosY, currentClip );
}

void Jugador::manageAlpha(){
	int alpha = (pausa || desconectado)?128:255;
		gJugadorTexture.setAlpha(alpha);
}

void Jugador::managePausa(bool p){
	pausa = p;
	manageAlpha();

}

void Jugador::manageDesconexion(bool d){
	desconectado = d;
	manageAlpha();
}

Jugador::~Jugador()
{
	gJugadorTexture.free();
}
