#include <QWidget>
#include <QtWidgets>
#include <netdb.h>

class Client : public QWidget{
    Q_OBJECT
    public:
        Client();
        ~Client();
        QSize minimumSizeHint() const override;
        QSize sizeHint() const override;

    protected:
        void sendNumber();
        void prepareNetworking();
        void prepareAnswerNetworking();
        void waitForAnswer();

    private:
        QGridLayout * mainLayout;
        float number = 0.5f;
        int socketfd;
        int socketfd_answer;
        addrinfo * sendtoinfo, * res;
        char* buffer;
        QLabel * label;
};