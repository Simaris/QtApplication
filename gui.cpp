#include "window.h"
#include <QApplication>
#include "server.h"
#include <thread>

void server_thread(){
    Server server;
    while(true){
        server.receive();
        server.answer();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    std::thread gui_object(server_thread);
    return app.exec();
}
