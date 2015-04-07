#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include <map>
#include "ClientThread.hpp"
#include "Server.hpp"

using namespace std;

// constructeur par défaut du server
Server::Server() {
    this->mRunning = false;
    this->mPort = 8080;
}

// constructeur du server
Server::Server(int port) {
    this->mPort = port;
}

// initialise le serveur (initialise des variable utile et lance les méthodes qui doivent être lancées avant de start le serveur
int Server::init() {
    struct in_addr myAddress;
    struct hostent *host;
    char hostName[100];
    WSADATA wsaData;

    this->state = ServerState::WAITING_FOR_PLAYERS;

    // WSAStartup dit au serveur d'utiliser les sockets
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup a échoué " << endl;
        throw  new ServerException(0, "WSAStartup a échoué ");
    }

    // Réccupère le hostname (avec des info comme l'IP de la machine)
    if (gethostname(hostName, 100) == SOCKET_ERROR) {
        cerr << "gethostname() a rencontré l'erreur " << WSAGetLastError() << endl;;
        throw  new ServerException(1, "gethostname() a rencontré l'erreur " + WSAGetLastError());
    }

    if ((host = gethostbyname(hostName)) == NULL) {
        cerr << "gethostbyname() a rencontré l'erreur " << WSAGetLastError() << endl;
        throw  new ServerException(2, "gethostbyname() a rencontré l'erreur " + WSAGetLastError());
    }

    // copie les adresses dans MyAddress (memcpy est une méthode de copie de mémoire => memcpy(destination, source)...
    memcpy(&myAddress.s_addr, host->h_addr_list[0], sizeof(myAddress.s_addr));

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_port = htons(mPort);
    //mServerAddr.sin_addr.s_addr = inet_addr(inet_ntoa(myAddress)); // inet_ntoa( MyAddress ) => ip de la machine
    mServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_ntoa( MyAddress ) => ip de la machine

    cout << "server correctement initialisé sur l'IP: " << inet_ntoa(myAddress) << endl;
    return 1;
}


// start server
int Server::start() {
    SOCKADDR_IN clientAddr;
    int clientAddrLen;
    HANDLE hProcessThread;
    SOCKET newConnection;
    struct thread_param *p = new thread_param();

    if ((mListeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cerr << "ne peut créer la socket. Erreur n° " << WSAGetLastError() << endl;
        WSACleanup();
        throw  new ServerException(3, "gethostbyname() a rencontré l'erreur " + WSAGetLastError());
    }
    if (bind(mListeningSocket, (SOCKADDR *) &mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) {
        cerr << "bind a échoué avec l'erreur " << WSAGetLastError() << endl;
        cerr << "Le port est peut-être déjà utilisé par un autre processus " << endl;
        closesocket(mListeningSocket);
        WSACleanup();
        throw  new ServerException(4, "bind a échoué avec l'erreur " + WSAGetLastError());
    }
    if (listen(mListeningSocket, 5) == SOCKET_ERROR) {
        cerr << "listen a échoué avec l'erreur " << WSAGetLastError() << endl;
        closesocket(mListeningSocket);
        WSACleanup();
        throw  new ServerException(5, "listen a échoué avec l'erreur " + WSAGetLastError());
    }
    cout << "serveur démarré : à l'écoute du port " << mPort << endl;
    mRunning = true;
    clientAddrLen = sizeof(clientAddr);

    while (mRunning) {

        switch (this->state) {
            case ServerState::WAITING_FOR_PLAYERS:

                if ((newConnection = accept(mListeningSocket, (SOCKADDR *) &clientAddr, &clientAddrLen)) == INVALID_SOCKET) {
                    cerr << "accept a échoué avec l'erreur " << WSAGetLastError() << endl;;
                    closesocket(mListeningSocket);
                    WSACleanup();
                    throw  new ServerException(6, "accept a échoué avec l'erreur " + WSAGetLastError());
                }
                p->server = this;
                p->socket = newConnection;

                cout << "client connecté :  IP : " << inet_ntoa(clientAddr.sin_addr) << ", port = " << ntohs(clientAddr.sin_port) << endl;

                hProcessThread = CreateThread(NULL, 0, &threadLauncher, p, 0, NULL);
                if (hProcessThread == NULL) {
                    cerr << "CreateThread a échoué avec l'erreur " << GetLastError() << endl;
                }

                break;
        }

    }
    return 1;
}

// stop server
void Server::stop() {
    cout << "stopping server" << endl;
    WSACleanup();
    this->mRunning = false;
}

// return port litenning
int Server::getPort() const {
    return mPort;
}

void Server::notifyToAll(string message) {
    for (const auto &kv : this->clientThreads) {
        ClientThread thread = (ClientThread) kv.second;
        //cout << "notify " << (int) kv.first << " !" << endl;
        thread.sendMessage(message);
    }
}

void Server::notifyTo(string pseudo, string message) {
    for (const auto &kv : this->clientThreads) {
        ClientThread thread = (ClientThread) kv.second;
        thread.sendMessageTo(pseudo, message);
    }
}

void Server::kickNotAuthenticatedUsers() {
    for (const auto &kv : this->clientThreads) {
        ClientThread thread = (ClientThread) kv.second;
        if (!thread.isAuthenticated()) {
            thread.kick("ahah !");
        }
    }
}

