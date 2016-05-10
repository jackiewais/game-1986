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
#include "LTexture.h"
#include "Jugador.h"
#include "Elemento/Elemento.h"
#include <SDL2/SDL.h>
#include <map>
#include "BackgroundTemplate.h"	

using namespace std;

struct coordinates
{
	int x;
	int y;
};


struct resolution
{
	int height;
	int width;
};

struct type_Fondo{
	string spritePath;
	int ancho;
	int alto;
};

struct type_Elemento{
	string spritePath;
	string elementoId;
	int posicionX;
	int posicionY;
	int ancho;
	int alto;
};

struct type_Escenario{
	int ancho;
	int alto;
	type_Fondo fondo;
	list<type_Elemento> elementos;
};

enum tipoELemento {FONDO, ELEMENTO, JUGADOR1, JUGADOR2, JUGADOR3, JUGADOR4, JUGADOR5, NO_ELEM};

class Escenario{

	private:

		char *spriteBackground;

		resolution screen;

		// fps
		int scroll ;

		bool pausa = true;

		//list<backgroundObjects> objects;
		list<BackgroundTemplate*> backgroundObjetcs;

		//The surface contained by the window
		SDL_Surface* gScreenSurface = NULL;

		BackgroundTemplate* object;
		LTexture gBGTexture;
	
		SDL_Renderer* dondeRenderiza = NULL;
		//Starts up SDL and creates window
		bool init();

		//Loads media
		bool loadMedia();

		//Frees media and shuts down SDL
		void close();
		int clientId;


		map<int,Jugador*> jugadores;
		void crearJugador(int id, string nombre, int posXIni);
		void updateJugadores();

		//SOCKET
		int socketCliente;
		void interchangeStatus(map<int,Elemento*> &elementos);
		int receiveMsg(char* buffer);
		void processMessages(map<int,Elemento*> &elementos, struct gst** rcvMsgsQty, int msgsQty);

	public:

	Escenario();
	Escenario(int widht, int height);
    bool lunchScreen();
	void insertBackgroundObject(string path, int x, int y, int height, int scrH  );
	void setSize(int width, int height );
	void mostrarVariables();
	void renderBackgroundObjects(int scrollingOffset);	
	type_Elemento parseMsg(string s);


};




#endif /* ESCENARIO_H_ */
