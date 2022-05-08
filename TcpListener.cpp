#include "TcpListener.h"
#include <iostream>
#include <string>
#include <sstream>

int TcpListener::init()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}
	// Create a socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(_port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	// Tell Winsock the socket is for listening
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	// Create the master file descriptor set and zero it
	FD_ZERO(&_master);
	FD_SET(_socket, &_master);
	return 0;
}

int TcpListener::run()
{
	bool running = true;

	while (running)
	{
		fd_set copy = _master;

		// See how much clients are there 
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		//trying
		//std::cout << "count : " << socketCount << std::endl;

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == _socket)
			{
				// Accept a new connection
				SOCKET client = accept(_socket, nullptr, nullptr);
			
				// Add the new connection to the list of connected clients
				FD_SET(client, &_master);
				//trying
				_clientCount++;
				if (_clientCount <= 1)
				{
					onClientConnected(client);
				}
			

			}
			else // It's an incomming message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &_master);
				}
				else
				{
					onMessageRecived(sock, buf, bytesIn);
					// Send message to other clients, accept the listening socket
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(_socket, &_master);
	closesocket(_socket);

	while (_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = _master.fd_array[0];

		// Send the goodbye message
		//send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	return 0;
}

void TcpListener::sendtoClient(int clientsocket, const char* msg, int length)
{
	send(clientsocket, msg, length, 0);
}

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length)
{
	for (int i = 0; i < _master.fd_count; i++)
	{
		SOCKET outSock = _master.fd_array[i];
		if (outSock != _socket && outSock != sendingClient)
		{
			sendtoClient(outSock, msg, length);
		}
	}
}

std::vector<std::string> TcpListener::parseFileType(std::string FileName)
{
	return std::vector<std::string>();
}

void TcpListener::onClientConnected(int clientSocket)
{

}

void TcpListener::onClientDisconnected(int clientSocket)
{

}

void TcpListener::onMessageRecived(int clientSocket, const char* msg, int length)
{

}

