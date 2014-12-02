#include <iostream>
#include "Server.cpp"
#include <pthread.h>

using namespace std;

int main() {
    int port = 3000;
    Server *s = new Server(port);
    cout << s->getPort() << endl;
    s->init();

    return 0;
}