
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <limits>
#include "Logger/Log.h"
#include "Parser/Parser.h"
#include "Utils/messages.h"
#include "Elemento/Elemento.h"
#include "connectionManager/ConnectionManager.h"
#include "Escenario.h"

using namespace std;

#define BUFLEN 1000


unsigned short logLevel;
unsigned short clientId;

bool isConnected, isRunning;
Log glog;
int msgsQty;
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
		    	mapa->insertBackgroundObject(elem.spritePath, elem.posicionX, elem.posicionY,
		    			elem.alto, height);
		    }

		}
/*	while (!quit){  borrar esta linea cuando se descomente lo de arriba
		int width= 500;	// tiene que ser el valor de ancho del escenario. Entonces el escenario es igual de ancho que la ventana (se levanta del xml)
		int height = 500; // puede ser cualquiera (es el alto de la ventana)

		//crea una ventana y muestra una imagen
		Escenario* mapa = new Escenario(width,height,connectionManager,1);
		int x = 200;
		int y = 500;
		//cargo varios backround
		mapa->setEscenarioSize(2000); //setea el alto del escenario. Esto se levanta del xml

		mapa->insertBackgroundObject("enfermera", 0,-416,84,height);
mapa->insertBackgroundObject("enfermera", 250,250,84,height);
mapa->insertBackgroundObject("enfermera", 400,400,84,height);
mapa->insertBackgroundObject("enfermera", 300,700,84,height);
		mapa->insertBackgroundObject("sprites/pelota.png", x,y,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 200,2000,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 150,700,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 100,100,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 450,657,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 300,500,40,height);*/

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


void mostrarLogin(char* ipChar, int& portNumber, string& name) {


	bool ok = false;
	cout << "--- Log in ---" << endl;
	cout << "Por favor ingrese los siguientes datos: " << endl;
	cout << "Nombre de usuario:" << endl;
	getline(cin,name);
	cout << "Ip del servidor: " << endl;
	cin >> ipChar;
	while (!ok){
		cout << "Puerto: " << endl;
		cin >> portNumber;
		if (!cin){
			cout << "Error: Debe ingresar un número" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			ok = false;
		}else
			ok = true;
	}
}

void loadScenario(ConnectionManager* connectionManager) {
	char *bufferSnd, bufferRcv[BUFLEN];
	struct gst* sndMsg;
	struct gst** msgs;
	int bufferSndLen;
	mapa = new Escenario(connectionManager,1);

	sndMsg = genAdminGst(0,command::REQ_SCENARIO);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	connectionManager -> sendMsg(bufferSnd,bufferSndLen);
	memset(bufferRcv,0,BUFLEN);
	if (connectionManager -> receive(bufferRcv) == 0) {
		msgsQty = decodeMessages(&msgs, bufferRcv);
		for (int i = 0; i < msgsQty; i++) {
			type_Elemento element = mapa->parseMsg(msgs[i]);
			elements.insert(pair<int,type_Elemento>(i,element));
		}
	}
	mapa->generarEscenario();
}

int main( int argc, char* args[] )
{
	char ipAddr[20];
	string userName;
	int port;
	isConnected = false;
	bool isRunning = false;
	glog.createFile(3);
	ConnectionManager connectionManager(glog);
	struct gst* position;
	while (!connectionManager.isConnected()){
		mostrarLogin(ipAddr, port, userName);
		connectionManager.connectManager(ipAddr, port, userName, position);
	}

	loadScenario(&connectionManager);
	isRunning = true;
	while(isRunning)
	{
		playGame(&connectionManager, position);
		isRunning = false;
	}
	finish(&connectionManager);


	return 0;
}
