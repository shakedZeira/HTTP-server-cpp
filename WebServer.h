#pragma once
#include "TcpListener.h"

class WebServer : public TcpListener
{
public:

	WebServer(const char* IpAddress, int port) :
		TcpListener(IpAddress, port) {}

	std::vector<std::string> parseFileType(std::string fileName);

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageRecived(int clientSocket, const char* msg, int length);

private:
	std::vector<std::string> tokens;

};