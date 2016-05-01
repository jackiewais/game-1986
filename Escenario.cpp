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
//The window renderer		
SDL_Renderer* gRenderer = NULL;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

Escenario::Escenario(int height, int width) {
this->setSize(width,height);
//Start up SDL and create window
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
void Escenario::lunchScreen(){

//Main loop flag
bool quit = false;
//Event handler
SDL_Event e;
int scrollingOffset = 0;
Jugador jugador (gRenderer,screen.width,screen.height, 0.3);

Jugador otroJugador (gRenderer,screen.width,screen.height, 0.6);
//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT ){
				quit = true;
			}
			jugador.handleEvent(e);

			//TODO ESTO SE REEMPLAZA POR LOS MENSAJES DEL SERVIDOR
			if (e.key.keysym.sym == SDLK_0 && e.type == SDL_KEYUP && e.key.repeat == 0){
				otroJugador.patear();
			}
			if (e.key.keysym.sym == SDLK_1 && e.type == SDL_KEYUP && e.key.repeat == 0){
				otroJugador.forzarPosicion(15,70);
			}
			if (e.key.keysym.sym == SDLK_2 && e.type == SDL_KEYUP && e.key.repeat == 0){
				otroJugador.forzarPosicion(230,400);
			}
			//-------------------------------------------------------------
		}
				jugador.move();
				otroJugador.moverPelotas();

				//Scroll background
				++scrollingOffset;
				if( scrollingOffset >gBGTexture.getHeight() )
				{
					scrollingOffset = 0;
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				gBGTexture.render( 0, scrollingOffset);
				gBGTexture.render(0,  scrollingOffset - gBGTexture.getHeight() );
				//---------------------
				renderBackgroundObjects(scrollingOffset);
				
				//pruebaOb.render(pruebaOb.x,scrollingOffset-pruebaOb.y);
				otroJugador.render();
				jugador.render();

				//Update screen
				SDL_RenderPresent( gRenderer );
	}
	
			

//Free resources and close SDL
close();

}

void Escenario::insertBackgroundObject(string path , int x , int y, int height, int scrH ){

	cout << path << endl;
	cout << x << endl;
	cout << y << endl;
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
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "1986", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen.width, this->screen.height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
			}
		}
	}

	return success;
}

bool Escenario::loadMedia()
{
	//Loading success flag
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
	//Deallocate surface
	gBGTexture.free();
	

	//Destroy window
	SDL_DestroyWindow( gWindow );
	

	//Quit SDL subsystems
	//Free loaded images

		//Destroy window
		SDL_DestroyRenderer( gRenderer );
		gRenderer = NULL;

		//Quit SDL subsystems
		
		SDL_Quit();

}





