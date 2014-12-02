// exception pour les erreurs serveur
using namespace std;

class ServerException : Exception {
public:
    ServerException(int errorCode, String message);

    String getMessage() const;

    int getErrorCode() const;

    void showError();

private:
    int errorCode;
    String message;
};