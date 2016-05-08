/*
 * Escenario.cpp
 *
 *  Created on: 22/04/2016
 *      Author: nahue
 */
#include <SDL2/SDL.h>
#include "Escenario.h"	
#include <stdio.h>
#include <map>


using namespace std;
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

Escenario::Escenario(int height, int width) {
	this->setSize(width,height);
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		this->gBGTexture.gRenderer=gRenderer;
		//Load media

		if( !loadMedia() )
		{
		printf( "Failed to load media!\n" );
		}
	}
}
bool Escenario::lunchScreen(){

bool quit = false;
bool started = false;

SDL_Event e;
int scrollingOffset = 0;
Jugador jugador (gRenderer,screen.width,screen.height, 1, string("Juan"));
Jugador otroJugador (gRenderer,screen.width,screen.height,2,string("Roman"));

Label lpausa;
lpausa.setData(gRenderer, string("Pause"),screen.width/2,screen.height/2,72);
Label lesperando;
lesperando.setData(gRenderer, string("Esperando Jugadores"),screen.width/2,screen.height/2+36,24);

bool reset = false;

	while( !quit && !reset)
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT ){
				quit = true;
			}

			if ( e.type == SDL_KEYUP && e.key.repeat == 0){
				switch(e.key.keysym.sym){
					case SDLK_p:
						if (started){
							pausa = !pausa;
							jugador.managePausa(pausa);
							otroJugador.managePausa(pausa);
						}
						break;

					case SDLK_r:
						reset = true;
						break;
				}
			}

			if (!pausa){
				jugador.handleEvent(e);
			}

			//TODO ESTO SE REEMPLAZA POR LOS MENSAJES DEL SERVIDOR
			if (e.type == SDL_KEYUP && e.key.repeat == 0){
				switch (e.key.keysym.sym){
					case SDLK_0:
						otroJugador.patear();
						break;
					case SDLK_9:
						otroJugador.hacerTruco();
						break;
					case SDLK_1:
						otroJugador.forzarPosicion(15,70);
						break;
					case SDLK_2:
						otroJugador.forzarPosicion(230,400);
						break;
					case SDLK_q:
						otroJugador.manageDesconexion(true);
						break;
					case SDLK_w:
						otroJugador.manageDesconexion(false);
						break;

					case SDLK_a:
						if (!started){
							pausa = false;
							started = true;
							jugador.managePausa(pausa);
							jugador.hacerTruco();
							otroJugador.managePausa(pausa);
							otroJugador.hacerTruco();
							break;
						}
				}
			}
			//-------------------------------------------------------------
		}


		if (!pausa){
			jugador.move();
			otroJugador.moverPelotas();

			//Scroll background
			++scrollingOffset;
			if( scrollingOffset >gBGTexture.getHeight() )
			{
				scrollingOffset = 0;
			}
		}


		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		gBGTexture.render( 0, scrollingOffset);
		gBGTexture.render(0,  scrollingOffset - gBGTexture.getHeight() );

		renderBackgroundObjects(scrollingOffset);

		otroJugador.render();
		jugador.render();
		if (pausa)
			lpausa.render();
		if (!started)
			lesperando.render();

		SDL_RenderPresent( gRenderer );
	}
			
	close();

	return quit;
}

void Escenario::insertBackgroundObject(string path , int x , int y, int height, int scrH ){

	BackgroundTemplate* object = new BackgroundTemplate(gRenderer,path, x,y,height, scrH);
	
	backgroundObjetcs.push_back(object);
}

void Escenario::renderBackgroundObjects(int scrollingOffset)
{
	for (list<BackgroundTemplate *>::iterator it=backgroundObjetcs.begin(); it != backgroundObjetcs.end(); ++it)
		(*it)->render(scrollingOffset);
}

void Escenario::setSize(int width, int height) {
	this->screen.height=height;
	this->screen.width=width;
}

void Escenario::mostrarVariables()
{
	cout << this->screen.height << endl;
	cout << this->screen.width << endl;

}

bool Escenario::init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "1986", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen.width, this->screen.height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
			}
		}
	}

	return success;
}

bool Escenario::loadMedia()
{
	bool success = true;

	if( !gBGTexture.loadFromFile( "background.bmp") )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	return success;
}
void Escenario::close()
{
	gBGTexture.free();

	SDL_DestroyWindow( gWindow );
	SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;
	
	SDL_Quit();

}





