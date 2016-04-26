/*
 * Escenario.h
 *
 *  Created on: 22/04/2016
 *      Author: nahue
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <stdio.h>
#include <iostream>
#include <list>
#include <SDL2/SDL.h>
#include "Jugador.h"

using namespace std;

struct coordinates
{
	int x;
	int y;
};

struct backgroundObjects{
		 coordinates position;
		 int id;
};

struct resolution
{
	int height;
	int width;
};

class Escenario{

	private:

		char *spriteBackground;

		resolution screen;

		// fps
		int scroll ;

		list<backgroundObjects> objects;

		//The window we'll be rendering to
		SDL_Window* gWindow = NULL;

		//The surface contained by the window
		SDL_Surface* gScreenSurface = NULL;

		//The image we will load and show on the screen
		SDL_Surface* gHelloWorld = NULL;

		//Starts up SDL and creates window
		bool init();

		//Loads media
		bool loadMedia();

		//Frees media and shuts down SDL
		void close();

	public:

	Escenario(int widht, int height);
    void lunchScreen();
	void insertBackgroundObject(backgroundObjects object);
	void setSize(int width, int height );
	void mostrarVariables();

};




#endif /* ESCENARIO_H_ */
