
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <list>
#include "Logger/Log.h"
#include "Parser/Parser.h"
#include "Utils/messages.h"
#include "Elemento/Elemento.h"
#include "connectionManager/ConnectionManager.h"
#include "Escenario.h"
#include "Sound.h"
#include "Login.h"
#include <SDL2/SDL_mixer.h>

using namespace std;

#define BUFLEN 1000

unsigned short logLevel;
unsigned short clientId;

Log glog;
Sound gsound;
Login glogin;
int msgsQty, velocidadDesplazamiento, velocidadDisparo;
char userName[50];

map<int, type_Elemento> elements;
Escenario* mapa;

enum messageType {CHAR, INT, DOUBLE, STRING, ERROR};


void playGame(ConnectionManager* connectionManager, struct gst* position){
	bool quit = false;
	type_Elemento elem;
	/*descomentar esto cuando este listo el server
	 * ya no sería necesaria la parte del for con los elementos del escenario (si con los jugadores)*/
	int height;
	while (!quit){
		int velDisp = velocidadDesplazamiento + velocidadDisparo;
		mapa->setVelocidades(velocidadDesplazamiento, velDisp);
		for (map<int,type_Elemento>::iterator it=elements.begin(); it!=elements.end(); ++it) {
			elem = it->second;
			//if (elem.elementoId == "VE"){
			//	mapa->setSize(elem.ancho, elem.alto);
			//}
			if (elem.elementoId == "FO"){
				height = elem.alto;
		    	mapa->setEscenarioSize(height);
		    } else {
		    	elem = it->second;
			cout << elem.spritePath << endl;
		    	mapa->insertBackgroundObject(elem.spritePath, elem.posicionX, elem.posicionY,
		    			elem.alto, height);
		    }

			cout << "Debug: paso playGame" << endl;

		}

		quit = mapa->lunchScreen(position);
	}

}




// MÉTODO QUE DEBE CERRAR TODAS LAS OPERACIONES INCONCLUSAS Y CERRAR LA APLICACIÓN PROLIJAMENTE.
int finish(ConnectionManager *connectionManager)
{
	cout << "-----" << endl;
	cout << "Terminamos la ejecución del programa." << endl;
	cout << "-----" << endl;

	if (connectionManager -> isConnected())
		glog.writeLine("antes de terminar nos desconectamos del servidor.");
		connectionManager -> disconnect();

	return 0;
}



void mostrarLogin(char* ipChar, int& portNumber, string& name)
{
	glogin.ventanaLogin(ipChar,portNumber,name);
}

Parser::spriteType formatearTipoSprite(char * tipo) {
	if (strcmp(tipo,"02")==0)
		return Parser::PE;
	if (strcmp(tipo,"00")==0)
		return Parser::DI;
	if (strcmp(tipo,"01")==0)
		return Parser::VU;
	if (strcmp(tipo, "04")==0)
		return Parser::J1;
	if (strcmp(tipo, "05")==0)
		return Parser::J2;
	if (strcmp(tipo, "06")==0)
		return Parser::J3;
	if (strcmp(tipo, "03")==0)
		return Parser::EN;
	if (strcmp(tipo, "07")==0)
		return Parser::FO;
	return Parser::BL;
}

void loadScenario(ConnectionManager* connectionManager) {
	char *bufferSnd, *bufferSndS, bufferRcv[BUFLEN], bufferRcvS[BUFLEN];
	struct gst* sndMsg, * sndMsgS;
	struct gst** msgs, **msgsS;
	int bufferSndLen, bufferSndLenS;
	mapa = new Escenario(connectionManager,1);

	sndMsg = genAdminGst(0,command::REQ_SCENARIO);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	connectionManager -> sendMsg(bufferSnd,bufferSndLen);
	memset(bufferRcv,0,BUFLEN);
	if (connectionManager -> receive(bufferRcv) == 0) {
		msgsQty = decodeMessages(&msgs, bufferRcv);
		mapa->setCantJugadores(stoi(msgs[0]->id));
		velocidadDesplazamiento = stoi(msgs[1]->ancho);
		velocidadDisparo = stoi(msgs[1]->alto);
		for (int i = 2; i < msgsQty; i++) {
			type_Elemento element = mapa->parseMsg(msgs[i]);
			elements.insert(pair<int,type_Elemento>(i,element));
		}
	}
	//se piden los sprites
	list<Parser::type_Sprite*> sprites;
	sndMsgS = genAdminGst(0,command::REQ_SPRITES);
	bufferSndLenS = encodeMessages(&bufferSndS, &sndMsgS, 1);
	connectionManager -> sendMsg(bufferSndS,bufferSndLenS);
	memset(bufferRcvS,0,BUFLEN);
	if (connectionManager -> receive(bufferRcvS) == 0) {
		msgsQty = decodeMessages(&msgsS, bufferRcvS);
		for (int i = 0; i < msgsQty; i++) {
			Parser::type_Sprite *sprite = new Parser::type_Sprite();
			sprite->id = formatearTipoSprite(msgsS[i]->id);
			char * first_token = strtok(msgsS[i]->path, "=");
			ifstream fondoSprite (first_token);
			if (!fondoSprite.good()) {
				// El archivo imagen que queremos usar no existe, usamos el default.
				if (sprite->id == Parser::PE)
					sprite->path = "vacioPelota.png";
				if (sprite->id == Parser::J1 || sprite->id == Parser::J2 || sprite->id == Parser::J3)
					sprite->path = "vacioJugador.png";
				if (sprite->id == Parser::VU)
					sprite->path = "vacioTruco.png";
				if (sprite->id == Parser::DI)
					sprite->path = "vacioDisparo.png";
			}
			else{
				// El path de la imagen es correcto y la podemos recuperar.
				sprite->path = first_token;
			}
			sprites.push_back(sprite);
		}
	}
	mapa->addSprites(sprites);
	mapa->generarEscenario();
}

int main( int argc, char* args[] )
{
	if( SDL_Init(SDL_INIT_AUDIO) < 0 ) exit(1);
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024) == -1)
	{ printf("%s",Mix_GetError()); }

	char ipAddr[20];
	string userName;
	int port;

	glog.createFile(3);
	ConnectionManager connectionManager(glog);
	struct gst* position;
	while (!connectionManager.isConnected()){
		mostrarLogin(ipAddr, port, userName);
		connectionManager.connectManager(ipAddr, port, userName, position);
	}

	loadScenario(&connectionManager);

	gsound.play(gsound.SONIDO_APLAUSO,25);

	playGame(&connectionManager, position);

	finish(&connectionManager);


	Mix_CloseAudio();
	//while(Mix_Init(0))
	//    Mix_Quit();
	SDL_Quit();
	return 0;
}
