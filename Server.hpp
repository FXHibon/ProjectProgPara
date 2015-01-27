#ifndef _SERVER
#define _SERVER
#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include <map>
#include "ClientThread.hpp"

using namespace std;

class Server {

public:

    Server();
    Server(int i);

    struct thread_param {
        Server *server;
        SOCKET socket;
    };

    int init();
    int start();
    void stop();

    int getPort() const;

    void notifyToAll(string message);

private:
    int mPort;
    SOCKET mListeningSocket;
    bool mRunning;
    SOCKADDR_IN mServerAddr;

    int curClientID = 0;
    std::map<int, ClientThread> clientThreads;
    static DWORD WINAPI threadLauncher(void *p) {
        struct thread_param *Obj = reinterpret_cast<struct thread_param *>(p);
        Server *s = Obj->server;

        ClientThread thread = ClientThread(s->curClientID, Obj->socket, s);
        s->clientThreads[s->curClientID] = thread;

        cout << "client ID : " << s->curClientID << endl;
        s->curClientID++;

        s->notifyToAll("Someone connected !");

        DWORD result = thread.run();

        s->clientThreads.erase(thread.getClientID());

        s->notifyToAll("Someone disconnected !");

        return result;
    }
};
#endif