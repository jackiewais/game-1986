/*
 * ConnectionManager.cpp
 *
 *  Created on: May 10, 2016
 *      Author: pablomd
 */


#include "ConnectionManager.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../Logger/Log.h"
#include "../Utils/messages.h"

#define BUFLEN 1000

using namespace std;

ConnectionManager::ConnectionManager(Log &log) {
	connected = false;
	audit = true;
	glog = log;

}

ConnectionManager::~ConnectionManager() {
	// TODO Auto-generated destructor stub
}

// INVOCACIÓN A LA LÓGICA PARA CONECTARNOS AL SERVIDOR.
int ConnectionManager::connectManager(char* ipAddr, int port, struct gst*& msg)
{
	ipChar = string(ipAddr);
	portNum = (unsigned short)port;
	struct sockaddr_in server;

	if (connected){
		glog.writeLine("El cliente ya fue conectado");
		return 0;
	}

	cout << "-----" << endl;
	cout << "Me intento conectar a: " << ipChar << ":" << portNum << endl;
	cout << "-----" << endl;

	 //CREO EL SOCKET.
	glog.writeLine("Creando Socket... ");
	mySocket =  socket(AF_INET , SOCK_STREAM , 0);

	if (mySocket < 0) {
		glog.writeErrorLine("ERROR al crear el socket");
		return 1;
	} else {
		glog.writeLine("Socket creado");
	}

	//INICIALIZO LAS VARIABLES DEL STRUCK SOCKADDR_IN
	glog.writeLine("Preparamos los valores del socket... ");
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	server.sin_addr.s_addr = inet_addr(ipChar.c_str());

	//HAGO CONNECT CON EL SERVER
	glog.writeLine("Conectando... ");
	if (connect(mySocket,(struct sockaddr *)&server, sizeof(server)) < 0) {
		glog.writeErrorLine("ERROR al conectar con el servidor ");
		return 1;
	} else {
		struct timeval timeout;
		timeout.tv_sec = 180;
		timeout.tv_usec = 0;

		 if (setsockopt (mySocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
			 glog.writeErrorLine("ERROR setting socket rcv timeout");

		if (setsockopt (mySocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
			glog.writeErrorLine("ERROR setting socket snd timeout");

		char respuestaServer[BUFLEN];
		memset(respuestaServer,0,BUFLEN);

		int msgQty;
		struct gst** msgArray;

		if (receive(respuestaServer) == -1){
			glog.writeErrorLine("ERROR recibiendo información del socket");
			return 1;
		}

		if (audit)
			cout << "AUDIT rcv: " << respuestaServer << endl;

		msgQty = decodeMessages(&msgArray, respuestaServer);
		if (msgQty == -1){
			glog.writeErrorLine("ERROR al decodificar la respuesta del servidor "
					+ string(respuestaServer));
			return 1;
		}

		// el primer mensaje recibidio del server tiene que ser de control con el status de la
		// conexion CON_SUCCESS o CON_FAIL

		if ((msgArray[0] -> type[0] != msgType::CONTROL) || (msgQty != 2)){
			glog.writeErrorLine("ERROR respuesta del servidor incorrecta"
								+ string(respuestaServer));
			return 1;
		}
		else if (msgArray[0] -> info[0] == (char)command::CON_FAIL){
			glog.writeErrorLine("ERROR al conectar con el servidor: conexion rechazada");
			return 1;

		}
		id = atoi(msgArray[0] ->id);


		glog.writeLine("Conectado correctamente con el servidor");

		connected = true;

		msg = msgArray[1];

		return 0;
	}
}

// INVOCACIÓN A LA LÓGICA PARA DESCONECTARNOS DEL SERVIDOR.
int ConnectionManager::disconnect()
{
	if (!connected)
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

	exitMsg = genAdminGst(id, command::DISCONNECT);
	bufferLen = encodeMessages(&buffer, &exitMsg, 1);

	send(mySocket , buffer , bufferLen , 0);
	close(mySocket);
	glog.writeLine("Socket correctamente cerrado.");
	connected = false;
	return 0;
}



int ConnectionManager::receive(char* buffer){


	char msgLenChar[3];
	int msgLen, rcvLen;

	rcvLen = recv(mySocket, buffer, BUFLEN -1 , 0);
	if( rcvLen < 0){
		glog.writeErrorLine("ERROR al recibir la respuesta. El servidor no responde");
	}

	else{
		if (audit)
			cout << "AUDIT rcv: " << buffer << endl;
		memcpy(msgLenChar, buffer, 3);
		msgLen = atoi(msgLenChar);

		if (rcvLen == msgLen){//full message received.
			return 0;
		}else{//message incomplete.
			int readed = rcvLen;
			while ( readed != msgLen){
				rcvLen = recv(mySocket, buffer+readed, msgLen-readed, 0);
				readed += rcvLen;
			}

		}
	}
	return rcvLen;
}

int ConnectionManager::sendMsg(char* buffer, int len){
	if (audit)
		cout << "AUDIT snd: " << buffer << endl;
	return send(mySocket , buffer , len , 0);
}

bool ConnectionManager::isConnected(){
	return connected;
}

int ConnectionManager::getId(){
	return id;
}
