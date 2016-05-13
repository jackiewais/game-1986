#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "messages.h"
#include "../Elemento/Elemento.h"

using namespace std;

int msg1len = typel + idl + posl + posl,
	msg2len = typel + idl + posl + posl + infol,
	msg3len = typel + idl,
	msg8len = typel + idl + infol;

void intToFixedChar(int n, char* p, int length){
	
	for (char* it = p + length - 1; it >= p; it--){
		*it = '0' + (n % 10);
		n = n/10;
	}

}

void cleanGst(struct gst* msg){
	memset(msg->type,0,typel +1);
	memset(msg->id,0,idl +1);
	memset(msg->posx,0,posl +1);
	memset(msg->posy,0,posl +1);
	memset(msg->info,0,infol +1);

}

int decodeMessages(struct gst*** msgs, char* msgsChar){

	int nOfMsgs;

	char nOfMsgsChar[nOfMsgsl],
		  *charIdx = msgsChar;

	charIdx += lengthl;
	memcpy(nOfMsgsChar, charIdx, nOfMsgsl);
	nOfMsgs = atoi(nOfMsgsChar);
	charIdx += nOfMsgsl;

	*msgs = new struct gst*[nOfMsgs];
	struct gst** msgIdx = *msgs;

	for (int i = 0; i < nOfMsgs; i++){

		*msgIdx = new struct gst;
		cleanGst(*msgIdx);

		memcpy((*msgIdx) -> type, charIdx, typel);
		charIdx += typel;

		memcpy((*msgIdx) -> id, charIdx, idl);
		charIdx += idl;

		if ((*msgIdx) -> type[0] == '1' || (*msgIdx) -> type[0] == '2'){
			memcpy((*msgIdx) -> posx, charIdx, posl);
			charIdx += posl;
			memcpy((*msgIdx) -> posy, charIdx, posl);
			charIdx += posl;
		}

		if ((*msgIdx) -> type[0] == '2' || (*msgIdx) -> type[0] == '8'){
			memcpy((*msgIdx) -> info, charIdx, infol);
			charIdx += infol;
		}
		else if ((*msgIdx) -> type[0] != '3'){
			cout << "DEBUG decodeMessages msgIdx -> type = " << (*msgIdx) -> type[0] << endl;
			cout << "DEBUG decodeMessages msgQty = " << i << endl;
			cout << "DEBUG decodeMessages charIdx = " << charIdx << endl;
			return -1;
		}

		msgIdx++;
	}

	return nOfMsgs;
}




int encodeMessages(char** msgsChar, struct gst** msgs, int qty){

	int pkglen = lengthl + nOfMsgsl;

	*msgsChar = new char[999];
	char* buffer = *msgsChar;
	char* bufferIt = buffer + lengthl;
	
	//copio la cantidad de mensajes
	intToFixedChar(qty, bufferIt, nOfMsgsl);
	bufferIt += nOfMsgsl;
	
	for (int i = 0; i < qty; i++){
		*bufferIt++ = (*msgs) -> type[0];
		
		memcpy(bufferIt, (*msgs) -> id, idl);
		bufferIt += idl;
		
		pkglen += typel + idl;

		if ((*msgs) -> type[0] == '1' || (*msgs) -> type[0] == '2'){
			memcpy(bufferIt, (*msgs) -> posx, posl);
			bufferIt += posl;
			memcpy(bufferIt, (*msgs) -> posy, posl);
			bufferIt += posl;

			pkglen += posl + posl;
		}

		if ((*msgs) -> type[0] == '2' || (*msgs) -> type[0] == '8'){
			memcpy(bufferIt, (*msgs) -> info, infol);
			bufferIt += infol;
	
			pkglen += infol;
		}

		msgs++;
		
	}


	intToFixedChar(pkglen, buffer, lengthl);
	return pkglen;
}

struct gst* genUpdateGstFromElemento(Elemento* elemento){

	struct gst* newMsg = new struct gst;
	cleanGst(newMsg);

	newMsg -> type[0] = (char) msgType::UPDATE;

	char charId[idl], charPos[posl];
	intToFixedChar(elemento->getId(), charId, idl);
	memcpy(newMsg -> id, charId, idl);

	intToFixedChar(elemento -> getPosX(), charPos, posl);
	memcpy(newMsg -> posx, charPos, posl);

	intToFixedChar(elemento -> getPosY(), charPos, posl);
	memcpy(newMsg -> posy, charPos, posl);

	newMsg -> info[0] = (char) elemento -> getEstado();
	return newMsg;
}

struct gst* genAdminGst(int clientId, command comando){

	struct gst* newMsg = new struct gst;
	cleanGst(newMsg);

	newMsg -> type[0] = (char) msgType::CONTROL;

	char charId[idl];
	intToFixedChar(clientId, charId, idl);
	memcpy(newMsg -> id, charId, idl);

	newMsg -> info[0] = (char) comando;
	return newMsg;
}
