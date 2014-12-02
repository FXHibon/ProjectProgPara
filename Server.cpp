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

    // WSAStartup dit au serveur d'utiliser les sockets
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup a échoué " << endl;
        throw  new ServerException(0, "WSAStartup a échoué ");
        return 0;
    }

    // Réccupère le hostname (avec des info comme l'IP de la machine)
    if (gethostname(HostName, 100) == SOCKET_ERROR) {
        cerr << "gethostname() a rencontré l'erreur " << WSAGetLastError() << endl;;
        throw  new ServerException(1, "gethostname() a rencontré l'erreur " + WSAGetLastError());
        return 0;
    }

    if ((host = gethostbyname(HostName)) == NULL) {
        cerr << "gethostbyname() a rencontré l'erreur " << WSAGetLastError() << endl;
        throw  new ServerException(2, "gethostbyname() a rencontré l'erreur " + WSAGetLastError());
        return 0;
    }

    // copie les adresses dans MyAddress (memcpy est une méthode de copie de mémoire => memcpy(destination, source)...
    memcpy(&MyAddress.s_addr, host->h_addr_list[0], sizeof(MyAddress.s_addr));

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr(inet_ntoa(MyAddress)); // inet_ntoa( MyAddress ) => ip de la machine

    cout << "server correctement initialisé" << endl;
    return 1;
}

// start server
int Server::start() {
    SOCKADDR_IN ClientAddr;
    int ClientAddrLen;
    HANDLE hProcessThread;
    SOCKET newConnection;
    struct thread_param p;

    if ((ListeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cerr << "ne peut créer la socket. Erreur n° " << WSAGetLastError() << endl;
        WSACleanup();
        throw  new ServerException(3, "gethostbyname() a rencontré l'erreur " + WSAGetLastError());
        return 0;
    }
    if (bind(ListeningSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR) {
        cerr << "bind a échoué avec l'erreur " << WSAGetLastError() << endl;
        cerr << "Le port est peut-être déjà utilisé par un autre processus " << endl;
        closesocket(ListeningSocket);
        WSACleanup();
        throw  new ServerException(4, "bind a échoué avec l'erreur " + WSAGetLastError());
        return 0;
    }
    if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
        cerr << "listen a échoué avec l'erreur " << WSAGetLastError() << endl;
        closesocket(ListeningSocket);
        WSACleanup();
        throw  new ServerException(5, "listen a échoué avec l'erreur " + WSAGetLastError());
        return 0;
    }
    cout << "serveur démarré : à l'écoute du port " << port << endl;
    running = true;
    ClientAddrLen = sizeof(ClientAddr);

    while (running) {
        if ((newConnection = accept(ListeningSocket, (SOCKADDR *) &ClientAddr, &ClientAddrLen)) == INVALID_SOCKET) {
            cerr << "accept a échoué avec l'erreur " << WSAGetLastError() << endl;;
            closesocket(ListeningSocket);
            WSACleanup();
            throw  new ServerException(6, "accept a échoué avec l'erreur " + WSAGetLastError());
            return 0;
        }
        p.ser = this;
        p.soc = newConnection;

        cout << "client connecté ::  IP : " << inet_ntoa(ClientAddr.sin_addr) << " ,port = " << ntohs(ClientAddr.sin_port) << endl;

        hProcessThread = CreateThread(NULL, 0, &Server::ThreadLauncher, &p, 0, NULL);
        if (hProcessThread == NULL) {
            cerr << "CreateThread a échoué avec l'erreur " << GetLastError() << endl;
        }
    }
    return 1;
}

// stop server
void Server::stop() {
    WSACleanup();
    this->running = false;
}

DWORD Server::ClientThread(SOCKET soc) {
    cout << "thread client démarré" << endl;

    /*    A mettre ici : code relatif au protocole utilisé    */

    return 0;
}

// return port litenning
int Server::getPort() const {
    return port;
}