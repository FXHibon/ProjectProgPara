#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include "ServerException.hpp"
#include "Server.hpp"
#include "ClientThread.hpp"
using namespace std;

ClientThread::ClientThread() {
    this->clientID = 0;
}

ClientThread::ClientThread(int clientID, SOCKET socket, Server *server) {
    this->clientID = clientID;
    this->socket = socket;
    this->server = server;
}

DWORD ClientThread::run() {
    char buffer[50];

    cout << "thread client démarré" << endl;

    int length = 1;
    boolean endWhile = false;

    while (true) {
        length = recv(this->socket, buffer, 50, 0);
        cout << length << endl;
        if (length == -1) {
            cout << "error: " << WSAGetLastError() << endl;
            break;
        } else if (length < 4) {
            continue;
        }

        string message = bufferToString(buffer, length);
        cout << message << endl;
        string instr = message.substr(0, 4);
        string data = message.substr(4, length - 4);
        cout << instr << endl;
        if (instr == "auth") {
            this->pseudo = data;
        } else if (instr == "mssg") {
            string message = bufferToString(buffer, length);
            cout << this->pseudo << " says : " << data << endl;
            this->server->notifyToAll(this->pseudo + " says : " + data);
        } else if (instr == "exit") {
            break;
        }
    }

    cout << "client socket closed" << endl;
    closesocket(this->socket);
    return 0;
}

void ClientThread::sendMessage(string message) {
    send(this->socket, message.c_str(), (int) strlen(message.c_str()), 0);
}

std::string ClientThread::bufferToString(char* buffer, int bufflen) {
    std::string ret(buffer, bufflen);
    return ret;
}

int ClientThread::getClientID() {
    return this->clientID;
}