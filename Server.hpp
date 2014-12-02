#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include "ServerException.cpp"

using namespace std;

class Server {

public:
    int getPort() const;

    int start();

    void stop();

    int init();

    Server();

    Server(int i);

    DWORD ThreadLauncher(void *p);

    DWORD ClientThread(SOCKET soc);

    struct thread_param {
        Server *ser;
        SOCKET soc;
    };

private:
    int port;
    SOCKET ListeningSocket;
    bool running;
    SOCKADDR_IN ServerAddr;
};