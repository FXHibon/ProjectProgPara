#include <iostream>
#include "Server.hpp"

using namespace std;

int main() {
    int port = 3000;

    Server *s = new Server(port);

    cout << s->getPort() << endl;

    s->init();
    s->start();

    return 0;
}