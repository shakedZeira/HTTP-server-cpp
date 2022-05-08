#pragma once
#include "TcpListener.h"

class MultiClientChat : public TcpListener
{
public:

	MultiClientChat(const char* IpAddress, int port) :
		TcpListener(IpAddress, port) {}

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageRecived(int clientSocket, const char* msg, int length);

};