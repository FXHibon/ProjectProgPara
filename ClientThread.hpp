#ifndef _CLIENT_THREAD
#define _CLIENT_THREAD

#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include "ServerException.hpp"

using namespace std;

class Server;

enum ClientState {
    HELLO,
    NICE_2_MEET_YOU,
    BYE
};

class ClientThread {

public:
    ClientThread();

    ClientThread(int clientID, SOCKET socket, Server *server);

    void sendMessage(string message);

    DWORD run();

    int getClientID();

    string getPseudo();
    bool isAuthenticated();

    void kick(string message);


private:


    int clientID;
    SOCKET socket;
    Server *server;

    string pseudo;
    ClientState state;

    std::string bufferToString(char *buffer, int bufflen);

    ClientState handleHello(string instr, string data);

    ClientState handleNice2MeetYou(string instr, string data);

};

#endif