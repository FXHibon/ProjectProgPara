// exception pour les erreurs serveur
#include <stdexcept>
#include <string>
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