#include "ServerException.hpp"
// exception pour les erreurs serveur

ServerException::ServerException(int errorCode, String message) {
    this->errorCode = errorCode;
    this->message = message;
};

String ServerException::getMessage() const {
    return this->message;
};

int ServerException::getErrorCode() const {
    return this->errorCode;
};

void ServerException::showError() {
    cout << "error " + this->errorCode + ": " + this->message;
};