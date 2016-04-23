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
				else
				{
					//Main loop flag
								bool quit = false;

								//Event handler
								SDL_Event e;

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
									}

									//Apply the image
									SDL_BlitSurface( this->gHelloWorld, NULL, gScreenSurface, NULL );

									//Update the surface
									SDL_UpdateWindowSurface( gWindow );
								}
				}
			}

			//Free resources and close SDL
			//close();
			SDL_Quit();


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
		//Create window
		this->gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen.width,this->screen.height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
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
	this->gHelloWorld = SDL_LoadBMP( "lu.bmp" );
	if( this->gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
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

}


void Escenario::lunchScreen()
{
	//
}
