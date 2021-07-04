#include <netdb.h>
#include <vector>
#define MSG_MAX_LENGTH 200

class Server{
    public:
        Server();
        ~Server();
        bool receive();
        void answer();
        float readNumber();
    private:
        int sockfd;
        char buffer[MSG_MAX_LENGTH];
        std::vector<sockaddr*> answer_queue;
};