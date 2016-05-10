
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <limits>
#include <arpa/inet.h>
#include <ctime>
#include <sstream>
#include "Logger/Log.h"
#include "Parser/Parser.h"
#include "Utils/messages.h"
#include "Elemento/Elemento.h"
#include <string>
#include <cstring>
#include "Escenario.h"

using namespace std;

#define BUFLEN 1000

unsigned short portNumber;
unsigned short logLevel;
unsigned short clientId;
string ipChar;
bool isConnected, isRunning;
Log glog;
int socketCliente, msgsQty;
char userName[50];
map<tipoELemento, type_Elemento> elements;
Escenario* mapa;


enum messageType {CHAR, INT, DOUBLE, STRING, ERROR};

bool mockConnection = false;

void playGame(){
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
		int width= 500;
		int height = 500;

		//crea una ventana y muestra una imagen
		Escenario* mapa = new Escenario(width,height);
		int x = 200;
		int y = 500;
		//cargo varios backround

		mapa->insertBackgroundObject("enfermera", 0,0,84,height);
		mapa->insertBackgroundObject("sprites/pelota.png", x,y,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 200,2000,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 150,700,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 100,100,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 450,657,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 300,500,40,height);

		quit = mapa->lunchScreen();
	}

}

void leerXML(){

	Parser::type_datosCliente xml;

	//struct mensaje item;
	string path;

	cout << "Por favor, ingrese el nombre del archivo xml a utilizar o 'default' para usar el de defecto" << endl;
	cin >> path;
	if (path == "default"){
		glog.writeWarningLine("Se toma el XML de default");
		path = Parser::getDefaultNameClient();
	}

	while (!Parser::fileExists(path.c_str())){
		cout << "Ruta ingresada no válida. Por favor, ingrese el nombre del archivo xml a utilizar o 'default' para usar el de defecto" << endl;
		cin >> path;
		if (path == "default"){
			glog.writeWarningLine("Se toma el XML de default");
			path = Parser::getDefaultNameClient();
		}
	}

	xml = Parser::parseXMLCliente(path.c_str(), &glog);

	logLevel = xml.logLevel;
	portNumber = xml.puerto;
	ipChar = xml.ip;
	/*
	std::map<int,Parser::type_mensaje>::iterator parserIterator;
	for (parserIterator = xml.mensajes->begin(); parserIterator != xml.mensajes->end(); ++parserIterator)
	{
		const char* sTipo = ToString(parserIterator->second.tipo);

	    std::ostringstream ostr;
	    ostr << parserIterator->second.id;
	    std::string sId = ostr.str();

	    std::ostringstream ostr2;
	    ostr2 << parserIterator->second.valor;
	    std::string sValor = ostr2.str();

		item.Id = sId;
		item.Tipo = sTipo;
		item.Valor = sValor;
		listaMensajes.push_back(item);
	}
	*/
}


int receiveMsg(char* buffer){

	char msgLenChar[3];
	int msgLen, rcvLen;

	rcvLen = recv(socketCliente, buffer, BUFLEN -1 , 0);
	if( rcvLen < 0){
		glog.writeErrorLine("ERROR al recibir la respuesta. El servidor no responde");
		return 1;
	}


	memcpy(msgLenChar, buffer, 3);
	msgLen = stoi(msgLenChar, nullptr);

	if (rcvLen == msgLen){//full message received.
		return 0;
	}else{//message incomplete.
		int readed = rcvLen;
		while ( readed != msgLen){
			rcvLen = recv(socketCliente, buffer+readed, msgLen-readed, 0);
			readed += rcvLen;
		}

	}

	return 0;
}

void processMessages(map<int,Elemento*> &elementos, struct gst** rcvMsgsQty, int msgsQty){

	//TODO

}





// INVOCACIÓN A LA LÓGICA PARA CONECTARNOS AL SERVIDOR.
int connect(map<int,Elemento*> &elementos)
{
	struct sockaddr_in server;

	if (isConnected){
		glog.writeLine("El cliente ya fue conectado");
		return 0;
	}

	cout << "-----" << endl;
	cout << "Me intento conectar a: " << ipChar << ":" << portNumber << endl;
	cout << "-----" << endl;

	 //CREO EL SOCKET.
	glog.writeLine("Creando Socket... ");
	socketCliente =  socket(AF_INET , SOCK_STREAM , 0);

	if (socketCliente < 0) {
		glog.writeErrorLine("ERROR al crear el socket");
		return 1;
	} else {
		glog.writeLine("Socket creado");
	}

	//INICIALIZO LAS VARIABLES DEL STRUCK SOCKADDR_IN
	glog.writeLine("Preparamos los valores del socket... ");
	server.sin_family = AF_INET;
	server.sin_port = htons(portNumber);
	server.sin_addr.s_addr = inet_addr(ipChar.c_str());

	//HAGO CONNECT CON EL SERVER
	glog.writeLine("Conectando... ");
	if (connect(socketCliente,(struct sockaddr *)&server, sizeof(server)) < 0) {
		glog.writeErrorLine("ERROR al conectar con el servidor ");
		return 1;
	} else {
 		struct timeval timeout;
		 timeout.tv_sec = 180;
		 timeout.tv_usec = 0;

		 if (setsockopt (socketCliente, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
			 glog.writeErrorLine("ERROR setting socket rcv timeout");

		 if (setsockopt (socketCliente, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		 	glog.writeErrorLine("ERROR setting socket snd timeout");

		 char respuestaServer[BUFLEN];
		 memset(respuestaServer,0,BUFLEN);

		int msgQty;
		struct gst** msgArray;

		if (!mockConnection){
			if (receiveMsg(respuestaServer) != 0){
				glog.writeErrorLine("ERROR recibiendo información del socket");
				return 1;
			}

			msgQty = decodeMessages(&msgArray, respuestaServer);
			if (msgQty == -1){
				glog.writeErrorLine("ERROR al decodificar la respuesta del servidor"
						+ string(respuestaServer));
				return 1;
			}

			// el primer mensaje recibidio del server tiene que ser de control con el status de la
			// conexion CON_SUCCESS o CON_FAIL

			if (msgArray[0] -> type[0] != msgType::CONTROL){
				glog.writeErrorLine("ERROR respuesta del servidor incorrecta"
									+ string(respuestaServer));
				return 1;
			}
			else if (msgArray[0] -> info[0] == (char)command::CON_FAIL){
				glog.writeErrorLine("ERROR al conectar con el servidor: conexion rechazada");
				return 1;

			}
		}

		glog.writeLine("Conectado correctamente con el servidor");

		if (!mockConnection){
			clientId = atoi(msgArray[0] -> id);
		}

		msgArray++;
		processMessages(elementos, msgArray, msgQty - 1);

		isConnected = true;

		cout << "User name: " << endl;
		cin >> userName;

		return 0;
	}
}


// INVOCACIÓN A LA LÓGICA PARA DESCONECTARNOS DEL SERVIDOR.
int disconnect()
{
	if (!isConnected)
	{
		glog.writeLine("El servidor ya está desconectado");
		return 0;
	}

	cout << "-----" << endl;
	cout << "Desconectando del servidor" << endl;
	cout << "-----" << endl;

	// ENVÍO MENSAJE DE EXIT Y CIERRO SOCKET

	struct gst* exitMsg;
	char* buffer;
	int bufferLen;

	exitMsg = genAdminGst(clientId, command::DISCONNECT);
	bufferLen = encodeMessages(&buffer, &exitMsg, 1);

	send(socketCliente , buffer , bufferLen , 0);
	close(socketCliente);
	glog.writeLine("Socket correctamente cerrado.");
	isConnected = false;
	return 0;
}


// MÉTODO QUE DEBE CERRAR TODAS LAS OPERACIONES INCONCLUSAS Y CERRAR LA APLICACIÓN PROLIJAMENTE.
int finish()
{
	cout << "-----" << endl;
	cout << "Terminamos la ejecución del programa." << endl;
	cout << "-----" << endl;

	glog.writeLine("antes de terminar nos desconectamos del servidor.");
	if (isConnected)
		disconnect();

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

void mostrarLogin() {

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

void loadScenario() {
	char *bufferSnd, bufferRcv[BUFLEN];
	struct gst* sndMsg;
	int bufferSndLen;
	mapa = new Escenario();

	sndMsg = genAdminGst(0,command::REQ_SCENARIO);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	send(socketCliente,bufferSnd,bufferSndLen,0);
	memset(bufferRcv,0,BUFLEN);
	if (receiveMsg(bufferRcv) == 0) {
		msgsQty = atoi(bufferRcv);
		for (int i = 0; i < msgsQty; i++) {
			memset(bufferRcv,0,BUFLEN);
			if (receiveMsg(bufferRcv) == 0){
				type_Elemento element = mapa->parseMsg(bufferRcv);
				tipoELemento id = getId(toUpercase(element.elementoId));
				elements.insert(pair<tipoELemento,type_Elemento>(id,element));
			}
		}
	}
	mapa->generarEscenario();
}

int main( int argc, char* args[] )
{

	isConnected = false;
	bool isRunning = false;
	map<int,Elemento*> elementos;

	glog.createFile(3);

//	leerXML();
	while (!isConnected){
		mostrarLogin();
		connect(elementos);
	}

	//elementos[clientId] es el elemento controlado por el cliente
	//dettachGraphicsThread(elementos, elementos[clientId]);
//	loadScenario(); descomentar esto cuando este listo el server
	isRunning = true;
	while(isRunning)
	{
		//interchangeStatus(elementos);
		playGame();
		isRunning = false;
	}
	finish();


	return 0;
}
