#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <client.h>
#include <QApplication>
#include <QtWidgets>
#include <string>
#include <unistd.h>


int get_socket(addrinfo * res, addrinfo * &p){
    int sockfd;
    for(p = res; p != NULL; p= p->ai_next){
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            continue;
        }
        return sockfd;
    }
    fprintf(stderr, "talker: failed to create socket\n");
    return 2;
}

Client::Client(){
    label = new QLabel(this);
    lineEdit = new QLineEdit(this);
    QPushButton * sendButton = new QPushButton(this);
    label->setText(std::to_string(number).c_str());
    lineEdit->setText(std::to_string(number).c_str());
    sendButton->setText("Send");
    connect(sendButton, &QPushButton::clicked, this, QOverload<>::of(&Client::sendNumber));
    mainLayout = new QGridLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(sendButton);
    setLayout(mainLayout);
    prepareNetworking();
}

Client::~Client(){
    freeaddrinfo(res);
    ::close(socketfd);
    delete mainLayout;
}

QSize Client::minimumSizeHint() const{
    return QSize(100, 100);
}

QSize Client::sizeHint() const{
    return QSize(400, 200);
}

void Client::sendNumber(){
    QLocale converter(QLocale::German);
    number = converter.toFloat(lineEdit->text());
    buffer = reinterpret_cast<char *> (&number);
    sendto(socketfd, buffer, sizeof(float), 0, sendtoinfo->ai_addr, sendtoinfo->ai_addrlen);
    waitForAnswer();
}

void Client::prepareNetworking(){
    addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "3490", &hints, &res);
    socketfd = get_socket(res, sendtoinfo);
}

void Client::waitForAnswer(){
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof their_addr;
    if((numbytes = recvfrom(socketfd, buffer, sizeof(float), 0, (struct sockaddr *) &their_addr, &addr_len)) == -1){
        return;
    }
    buffer[numbytes] = '\0';
    float ans_value = *((float*)buffer);
    char msg[50];
    sprintf(msg, "%f\n", ans_value);
    number = ans_value;
    label->setText(msg);
    lineEdit->setText(std::to_string(number).c_str());
    return;
}

int main(int argc, char** argv){
    QApplication app(argc, argv);
    Client client;
    client.show();
    return app.exec();
}