/*
 * Escenario.cpp
 *
 *  Created on: 22/04/2016
 *      Author: nahue
 */

#include "Escenario.h"
#include <stdio.h>
#include <SDL2/SDL.h>

using namespace std;

SDL_Renderer* gRenderer = NULL;

Escenario::Escenario(int height, int width) {
	this->setSize(width,height);
	//Start up SDL and create window
			if( !init() )
			{
				printf( "Failed to initialize!\n" );
			}
			else
			{
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

Jugador jugador (gRenderer,screen.width,screen.height);

//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			 if( e.type == SDL_QUIT )
			{
        		quit = true;
			}
			 jugador.handleEvent(e);
		}
		jugador.move();
	

	 SDL_Rect stretchRect;
	 stretchRect.x = 0;
	 stretchRect.y = 0;
	 stretchRect.w = this->screen.width;
	 stretchRect.h = this->screen.height;

         SDL_BlitScaled( this->gHelloWorld, NULL, gScreenSurface, &stretchRect);



			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			jugador.render();

			//Update screen
			SDL_RenderPresent( gRenderer );

	 //Apply the image
	 //SDL_BlitSurface( this->gHelloWorld, NULL, gScreenSurface, NULL );

	 //Update the surface
	 //SDL_UpdateWindowSurface( gWindow );
	}
	
			

//Free resources and close SDL
close();

}

void Escenario::insertBackgroundObject(backgroundObjects object){

	this->objects.push_back(object);

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
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
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
		this->gWindow = SDL_CreateWindow( "1986 ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen.width,this->screen.height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
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

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}


			//Get window surface
			this->gScreenSurface = SDL_GetWindowSurface( gWindow );
		}

	}

	return success;
}

bool Escenario::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	this->gHelloWorld = SDL_LoadBMP( "background.bmp" );
	if( this->gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "background.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}
void Escenario::close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	this->gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	this->gWindow = NULL;

	//Quit SDL subsystems
	//Free loaded images

		//Destroy window
		SDL_DestroyRenderer( gRenderer );
		gRenderer = NULL;

		//Quit SDL subsystems
		IMG_Quit();
		SDL_Quit();

}



