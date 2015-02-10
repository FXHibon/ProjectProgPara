#include <winsock2.h>
#include <iostream>
#include "ServerException.hpp"
#include "Server.hpp"

using namespace std;

ClientThread::ClientThread() {
    this->clientID = 0;
}

ClientThread::ClientThread(int clientID, SOCKET socket, Server *server) {
    this->clientID = clientID;
    this->socket = socket;
    this->server = server;
    this->state = ClientState::HELLO;
}

DWORD ClientThread::run() {
    char buffer[50];
    bool activeClient = true;

    cout << "thread client démarré" << endl;

    int length = 1;
    bool endWhile = false;

    this->sendMessage("hello");

    while (activeClient) {
        length = recv(this->socket, buffer, 50, 0);

        if (length == -1) {
            cout << "error: " << WSAGetLastError() << endl;
            break;
        } else if (length < 4) {
            continue;
        }

        string message = bufferToString(buffer, length);

        string instr = message.substr(0, 4);
        string data = message.substr(4, length - 4);

        if (instr == "exit") {
            this->state = ClientState::BYE;
        }

        switch (this->state) {
            case ClientState::HELLO:
                this->state = this->handleHello(instr, data);
                break;

            case ClientState::NICE_2_MEET_YOU:
                this->state = this->handleNice2MeetYou(instr, data);
                break;

            case ClientState::BYE:
                activeClient = false;
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

std::string ClientThread::bufferToString(char *buffer, int bufflen) {
    std::string ret(buffer, bufflen);
    return ret;
}

int ClientThread::getClientID() {
    return this->clientID;
}

string ClientThread::getPseudo() {
    return this->pseudo;
}

ClientState ClientThread::handleHello(string instr, string data) {
    if (instr == "auth") {

        if (data.length() >= 1) {

            this->pseudo = data;
            this->server->notifyToAll(this->pseudo + " connected !");

            cout << this->pseudo << " connected " << endl;

            this->sendMessage("ntmy");

            return ClientState::NICE_2_MEET_YOU;


        }

    }
    this->sendMessage("argh");
    return this->state;
}

ClientState ClientThread::handleNice2MeetYou(string instr, string data) {
    if (instr == "mssg") {

        cout << this->pseudo << " says : " << data << endl;

        this->server->notifyToAll(this->pseudo + " says : " + data);

    }

    this->sendMessage("argh");
    return this->state;
}
