#include "ServerException.hpp"
#include <string>

// exception pour les erreurs serveur

ServerException::ServerException(int errorCode, string message) {
    this->errorCode = errorCode;
    this->message = message;
};

string ServerException::getMessage() const {
    return this->message;
};

int ServerException::getErrorCode() const {
    return this->errorCode;
};

void ServerException::showError() {
    cout << "error " << this->errorCode << ": " << this->message;
};