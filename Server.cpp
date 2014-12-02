/**
* Main server class
*/

Server::Server(int port) {
    this->port = port;
}

Server::Server() {
    this->port = 8080;
}

void Server::start() {
    this->running = true;
}

void Server::stop() {
    this->running = false;
}

int Server::getPort() const {
    return port;
}
