#include "WebServer.h"
#include <string>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <vector> 
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#define BufSize MAX_PATH


void WebServer::onClientConnected(int clientSocket)
{
	std::cout << "Client Connected! " << "\r\n";
}
std::vector<std::string> WebServer::parseFileType(std::string fileName)
{
	std::string parsedHtml;
	std::vector<std::string> tokens;
	std::stringstream check1(fileName);
	while (getline(check1, parsedHtml, '.'))
	{
		tokens.push_back(parsedHtml);
	}
	return tokens;
}

void WebServer::onClientDisconnected(int clientSocket)
{

}
void WebServer::onMessageRecived(int clientSocket, const char* msg, int length)
{
	
	// GET /index.html HTTP/1.1
	//parse out the document requested
	std::istringstream iss(msg);
	//std::cout << "recvied a message from client :  \r\n";
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string content = "<h2>404 Not Found</h2>";
	SetCurrentDirectoryA("D:\\angular\\my-app\\src\\app\\");
	std::string HtmlFile = "/app.component.html";
	int errorcode = 404;
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		//open the document in the local file system
		HtmlFile = parsed[1];
		//std::cout << "parsed[1]: " << parsed[1] << "\r\n";
		if (HtmlFile == "/")
		{
			HtmlFile = "/app.component.html";
		}
	}
	tokens = parseFileType(HtmlFile);
	if (tokens[1] == "jpeg")
	{
		std::ifstream img("..\\app" + HtmlFile, std::ios::binary);
		if (img.good())
		{
			std::string str((std::istreambuf_iterator<char>(img)), std::istreambuf_iterator<char>());
			content = str;
			errorcode = 200;
		}
		img.close();
	}
	else
	{
		std::ifstream f("..\\app" + HtmlFile);
		if (f.good())
		{
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content = str;
			errorcode = 200;
		}
		f.close();
	}
	

	std::string type = "text/html";
	if (tokens[1] == "css")
	{
		type = "text/css";
	}
	else if (tokens[1] == "js")
	{
		type = "text/javascript";
	}
	
	if (parsed.size() >= 3)
	{
		if (parsed[0] == "GET")
		{
			//write the document back to the client
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorcode << " OK\r\n";
			oss << "Cache-Control: no-cache , private\r\n";
			if (type == "image/jpeg")
			{
				oss << "Content-Type: image/jpeg \r\n";
				oss << "Content-Transfer-Encoding: binary\r\n";
			}
			else
			{
				oss << "Content-Type: " << type << "\r\n";
			}
			oss << "Content-Length: " << content.size() << "\r\n";
			//to work on::::::::::::::::::::::::::::::::::::: all
			oss << "\r\n";
			oss << content;
			//std::cout << "size: " << content.size() << "\r\n";
			std::string output = oss.str();
			//std::cout << "output: " << output;
			int size = output.size() + 1;
			//std::cout << "content: \r\n" << content;
			sendtoClient(clientSocket, output.c_str(), size);
		}
		/*
		else if (parsed[0] == "POST" && parsed[1] == "/button")
		{
			std::string input;
			std::cout << "please enter what you would like to post: ";
			std::getline(std::cin, input);
			std::ostringstream postreq;
			postreq << "<h1>" << input << "</h1>";
			std::string str_postreq;
			str_postreq = postreq.str();
			//write the document back to the client
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorcode << "OK\r\n";
			oss << "Cache-Control: no-cache , private\r\n";
			oss << "Content-Type: " << type << "\r\n";
			oss << "Content-Length: " << str_postreq.size() << "\r\n";
			//to work on::::::::::::::::::::::::::::::::::::: all
			oss << "\r\n";
			oss << str_postreq;
			std::string output = oss.str();
			std::cout << "output: " << output;
			int size = output.size() + 1;
			std::cout << "content: \r\n" << content;
			sendtoClient(clientSocket, output.c_str(), size);
		}
		*/
		else if (parsed[0] == "POST" && parsed[1] == "/User-info")
		{
			//write the document back to the client
			std::string strsock;
			strsock = std::to_string(clientSocket);
			std::ostringstream Infostr;
			Infostr << "<h1>" <<"Client socket number : " << strsock << "</h1>";
			strsock = Infostr.str();
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorcode << "OK\r\n";
			oss << "Cache-Control: no-cache , private\r\n";
			oss << "Content-Type: " << type << "\r\n";
			oss << "Content-Length: " << strsock.size() << "\r\n";
			//to work on::::::::::::::::::::::::::::::::::::: all
			oss << "\r\n";
			oss << strsock;
			std::string output = oss.str();
			//std::cout << "output: \r\n" << output;
			int size = output.size() + 1;
			//std::cout << "content: \r\n" << content;
			sendtoClient(clientSocket, output.c_str(), size);
		}
	}
	
}
