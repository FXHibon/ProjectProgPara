// exception pour les erreurs serveur
#ifndef _SERVER_EXCEPTION
#define _SERVER_EXCEPTION
#include <stdexcept>
#include <string>
#include <iostream>
using namespace std;

class ServerException : public exception {
public:
    ServerException(int errorCode, string message);

    string getMessage() const;

    int getErrorCode() const;

    void showError();

private:
    int errorCode;
    string message;
};
#endif