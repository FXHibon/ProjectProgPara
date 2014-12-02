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

    static DWORD WINAPI ThreadLauncher(void *p) {
        struct thread_param *Obj = reinterpret_cast<struct thread_param *>(p);
        Server *s = Obj->ser;
        return s->ClientThread(Obj->soc);
    }

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