/**
* Main server class
*/
#include "Server.hpp"

// constructeur du server
Server::Server(int port) {
    this->port = port;
}

// constructeur par défaut du server
Server::Server() {
    this->running = false;
    this->port = 8080;
}

// initialise le serveur (initialise des variable utile et lance les méthodes qui doivent être lancées avant de start le serveur
int Server::init() {
    struct in_addr MyAddress;
    struct hostent *host;
    char HostName[100];
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup a échoué " << endl;
        return 1;
    }

    if (gethostname(HostName, 100) == SOCKET_ERROR) {
        cerr << "gethostname() a rencontré l'erreur " << WSAGetLastError() << endl;
        return 1;
    }

    if ((host = gethostbyname(HostName)) == NULL) {
        cerr << "gethostbyname() a rencontré l'erreur " << WSAGetLastError() << endl;
        return 1;
    }

    memcpy(&MyAddress.s_addr, host->h_addr_list[0], sizeof(MyAddress.s_addr));

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr(inet_ntoa(MyAddress));

    cout << "server correctement initialisé" << endl;
    return 0;
}

// start server
void Server::start() {
    this->running = true;
}

// stop server
void Server::stop() {
    this->running = false;
}

// return port litenning
int Server::getPort() const {
    return port;
}
