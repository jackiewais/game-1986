/*
 * ConnectionManager.h
 *
 *  Created on: May 10, 2016
 *      Author: pablomd
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include <string>
#include "../Logger/Log.h"
#include "../Utils/messages.h"

class ConnectionManager {
private:
	int id;
	unsigned short portNum;
	int mySocket;
	std::string ipChar;
	bool connected;
	bool audit;
	Log glog;
public:
	ConnectionManager(Log &log);
	virtual ~ConnectionManager();
	int connectManager(char* ipAddr, int port, struct gst*& msg);
	int disconnect();
	int receive(char* buffer);
	int sendMsg(char* buffer, int len);
	int getId();
	bool isConnected();
};

#endif /* CONNECTIONMANAGER_H_ */
