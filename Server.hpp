#include <WinSock2.h>

using namespace std;

class Server {

public:
    int getPort() const;

    void start();

    void stop();

    Server();

    Server(int i);

private:
    int port;
    SOCKET ListeningSocket;
    bool running;
};