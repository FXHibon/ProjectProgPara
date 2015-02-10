#ifndef _CLIENT_THREAD
#define _CLIENT_THREAD
#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include "ServerException.hpp"


using namespace std;

class Server;

class ClientThread {

public:
    ClientThread();
    ClientThread(int clientID, SOCKET socket, Server *server);

    void sendMessage(string message);
    DWORD run();

    int getClientID();
    string getPseudo();
private:
    int clientID;
    SOCKET socket;
    Server *server;

    string pseudo;

    std::string bufferToString(char* buffer, int bufflen);
};
#endif