/**
* Main server class
*/

using namespace std;

class Server {

public:
    int getPort() const {
        return port;
    }

private:
    int port;

public:
    Server() {
        cout << "Server construction";
    }

    Server(int i);
};

Server::Server(int port) {
    this->port = port;
}
