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

    void sendMessage(SOCKET socket, string message);

    Server();

    Server(int i);

    static DWORD WINAPI threadLauncher(void *p) {
        struct thread_param *Obj = reinterpret_cast<struct thread_param *>(p);
        Server *s = Obj->server;
        return s->clientThread(Obj->socket);
    }

    DWORD clientThread(SOCKET socket);

    struct thread_param {
        Server *server;
        SOCKET socket;
    };

private:
    int mPort;
    SOCKET mListeningSocket;
    bool mRunning;
    SOCKADDR_IN mServerAddr;
};