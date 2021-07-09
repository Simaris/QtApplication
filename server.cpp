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

    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;
}

bool Server::receive(){
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof their_addr;
    if(poll(pfds, 1, 1000) == 0){
        return false;
    }
    if((pfds[0].revents & POLLIN) <= 0){
        return false;
    }
    if((numbytes = recvfrom(sockfd, buffer, sizeof(float), 0, (struct sockaddr *) &their_addr, &addr_len)) == -1){
        return false;
    }
    buffer[numbytes] = '\0';
    answer_queue.push_back(*((struct sockaddr*) &their_addr));
    return true;
}

void Server::answer(float modifier){
    addrinfo *res;
    sockaddr_in * address_storage = (sockaddr_in *) &answer_queue.back();
    res = (addrinfo*)malloc(sizeof(addrinfo));
    res->ai_flags = AI_PASSIVE;
    res->ai_family = AF_INET;
    res->ai_socktype = SOCK_DGRAM;
    res->ai_protocol = 17;
    res->ai_addrlen = sizeof(sockaddr_in);
    res->ai_addr = (sockaddr*)address_storage;
    res->ai_canonname = NULL;
    res->ai_next = NULL;
    float answer = modifier * *(float *)buffer;
    char * answer_bytes = reinterpret_cast<char *> (&answer); 
    sendto(sockfd, answer_bytes, sizeof(float), 0, res->ai_addr, res->ai_addrlen);
    answer_queue.pop_back();  // since I need address_storage as a pointer this has to be popped at the end
    free(res);
}

float Server::readNumber(){
    return *((float*) buffer);
}

Server::~Server(){
    close(sockfd);
}