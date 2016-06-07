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
#include "connectionManager/ConnectionManager.h"
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


enum tipoELemento {FO, VE, EL, NO_ELEM};

class Escenario{

	private:

		char *spriteBackground;
		//WindowSize
		resolution screen;
		//EscenarioSize = tamaño del nivel
		resolution escenarioSize;
		// fps
		int scroll, escenarioHeight;
		int velocidadDesplazamiento, velocidadDisparo;
		bool pausa = true;
		bool reset = false;
		bool started;
		bool desconectado = false;

		//list<backgroundObjects> objects;
		list<BackgroundTemplate*> backgroundObjetcs;

		list<Parser::type_Sprite*> sprites;

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
		ConnectionManager* conManager;
		//void interchangeStatus(map<int,Elemento*> &elementos);
		void sendStatus();
		void receiveStatus();
		int receiveMsg(char* buffer);
		void processMessages(struct gst** rcvMsgsQty, int msgsQty);
		void getSpritePath(char *&spritePathPelota, Parser::spriteType id);


	public:

	Escenario();
	Escenario(ConnectionManager* connectionManager,int scroll);
	Escenario(ConnectionManager* connectionManager,int scroll, int offset, bool playing);
	Escenario(int widht, int height, ConnectionManager* connectionManager, int scroll);
	void setCantJugadores(int cantJugadores);
    bool lunchScreen(struct gst* position, bool forcePos);
	void insertBackgroundObject(string path, int x, int y, int height, int scrH  );
	void setSize(int width, int height );
	void setVelocidades(int velocidadDesplazamiento, int velocidadDisparo);
	void mostrarVariables();
	void renderBackgroundObjects(int scrollingOffset);	
	type_Elemento parseMsg(struct gst* msg);
	void generarEscenario();
	void setEscenarioSize(int height);
	void deleteBackgroundObjetcs();
	void addSprites(list<Parser::type_Sprite*> sprites);

	int cantJugadores;

};

#endif /* ESCENARIO_H_ */
