
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

map<tipoELemento, type_Elemento> elements;
Escenario* mapa;

enum messageType {CHAR, INT, DOUBLE, STRING, ERROR};


void playGame(ConnectionManager* connectionManager, struct gst* position){
	bool quit = false;
	type_Elemento elem;
	/*descomentar esto cuando este listo el server
	 * ya no sería necesaria la parte del for con los elementos del escenario (si con los jugadores)
	int height;
	while (!quit){
		for (map<tipoELemento,type_Elemento>::iterator it=elements.begin(); it!=elements.end(); ++it) {
			elem = it->second;
			if (it->first == FONDO){
				height = elem.alto;
		    	mapa->setSize(elem.ancho, height);
		    } else {
		    	elem = it->second;
		    	mapa->insertBackgroundObject(elem.elementoId, elem.posicionX, elem.posicionY,
		    			elem.alto, height);
		    }

		}*/
	while (!quit){ // borrar esta linea cuando se descomente lo de arriba
		int width= 500;	// tiene que ser el valor de ancho del escenario. Entonces el escenario es igual de ancho que la ventana (se levanta del xml)
		int height = 500; // puede ser cualquiera (es el alto de la ventana)

		//crea una ventana y muestra una imagen
		Escenario* mapa = new Escenario(width,height,connectionManager,1);
		int x = 200;
		int y = 500;
		//cargo varios backround
		mapa->setEscenarioSize(1000); //setea el alto del escenario. Esto se levanta del xml

		mapa->insertBackgroundObject("enfermera", 0,0,84,height);
		mapa->insertBackgroundObject("sprites/pelota.png", x,y,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 200,2000,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 150,700,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 100,100,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 450,657,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 300,500,40,height);

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


/*void interchangeStatus(map<int,Elemento*> &elementos){

	char *bufferSnd, bufferRcv[BUFLEN];
	struct gst* sndMsg, ** rcvMsgs;
	int bufferSndLen, rcvMsgsQty;

	sndMsg = genUpdateGstFromElemento(elementos[clientId]);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	send(socketCliente,bufferSnd,bufferSndLen,0);

	memset(bufferRcv,0,BUFLEN);
	if (receiveMsg(bufferRcv) == 0){
		rcvMsgsQty = decodeMessages(&rcvMsgs, bufferRcv);

		if (rcvMsgsQty != -1){
			processMessages(elementos, rcvMsgs, rcvMsgsQty);
		}
	}

}
*/

void mostrarLogin(char* ipChar, int& portNumber) {

	bool ok = false;
	cout << "--- Log in ---" << endl;
	cout << "Por favor ingrese los siguientes datos: " << endl;
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

string toUpercase(string id){
	string upId;
	for (int i=0; i<id.length(); ++i)
		upId[i] = toupper(id[i]);
	return upId;
}

tipoELemento getId(string id) {
	if (id == "FONDO")
		return FONDO;
	else if (id == "ELEMENTO")
		return ELEMENTO;
	else if (id == "JUGADOR1")
		return JUGADOR1;
	else if (id == "JUGADOR2")
		return JUGADOR2;
	else if (id == "JUGADOR3")
		return JUGADOR3;
	else if (id == "JUGADOR4")
		return JUGADOR4;
	else if (id == "JUGADOR5")
		return JUGADOR5;
	else return NO_ELEM;
}

void loadScenario(ConnectionManager* connectionManager) {
	char *bufferSnd, bufferRcv[BUFLEN];
	struct gst* sndMsg;
	int bufferSndLen;
	mapa = new Escenario();

	sndMsg = genAdminGst(0,command::REQ_SCENARIO);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	connectionManager -> sendMsg(bufferSnd,bufferSndLen);
	memset(bufferRcv,0,BUFLEN);
	if (connectionManager -> receive(bufferRcv) != 0) {
		msgsQty = atoi(bufferRcv);
		for (int i = 0; i < msgsQty; i++) {
			memset(bufferRcv,0,BUFLEN);
			if (connectionManager -> receive(bufferRcv) == 0){
				type_Elemento element = mapa->parseMsg(bufferRcv);
				tipoELemento id = getId(toUpercase(element.elementoId));
				//elements.insert(pair<tipoELemento,type_Elemento>(id,element));
			}
		}
	}
	mapa->generarEscenario();
}

int main( int argc, char* args[] )
{
	char ipAddr[20];
	int port;
	isConnected = false;
	bool isRunning = false;
	glog.createFile(3);
	ConnectionManager connectionManager(glog);
	struct gst* position;
//	leerXML();
	while (!connectionManager.isConnected()){
		mostrarLogin(ipAddr, port);
		connectionManager.connectManager(ipAddr, port, position);
	}

	//elementos[clientId] es el elemento controlado por el cliente
//	loadScenario(); descomentar esto cuando este listo el server
	isRunning = true;
	while(isRunning)
	{
		//interchangeStatus(elementos);
		playGame(&connectionManager, position);
		isRunning = false;
	}
	finish(&connectionManager);


	return 0;
}
