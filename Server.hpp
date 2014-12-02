#include <winsock2.h>

using namespace std;

class Server {

public:
    int getPort() const;

    void start();

    void stop();

    int init();

    Server();

    Server(int i);

private:
    int port;
    SOCKET ListeningSocket;
    bool running;
    SOCKADDR_IN ServerAddr;
};