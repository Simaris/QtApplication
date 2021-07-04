#include <sys/types.h>
#include <sys/socket.h>
#include <server.h>
#include <netdb.h>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT "3490"
#define MSG_MAX_LENGTH 200

void *get_in_addr(struct sockaddr * sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

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

Server::Server(){
    struct addrinfo hints, *servinfo, *p;
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }  

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
}

bool Server::receive(){
    int numbytes;
    struct sockaddr_storage their_addr;
    char s[INET6_ADDRSTRLEN];
    socklen_t addr_len = sizeof their_addr;
    if((numbytes = recvfrom(sockfd, buffer, sizeof(float), 0, (struct sockaddr *) &their_addr, &addr_len)) == -1){
        return false;
    }
    inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof s);
    buffer[numbytes] = '\0';
    answer_queue.push_back((struct sockaddr *)&their_addr);
    return true;
}

void Server::answer(){
    int status;
    addrinfo hints, *res, *p;
    int socketfd_answer;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    status = getaddrinfo(NULL, "8999", &hints, &res); // TODO actually use answer_queue; do not use hardcoded values
    socketfd_answer = get_socket(res, p);
    // send answer
    int bytes_sent;
    float send_number = 1.0f; // send number here
    char * buffer = reinterpret_cast<char *> (&send_number); 
    bytes_sent = sendto(socketfd_answer, buffer, sizeof(float), 0, p->ai_addr, p->ai_addrlen);
}

float Server::readNumber(){
    return *((float*) buffer);
}

Server::~Server(){
    delete[] buffer;
    close(sockfd);
}