#include "window.h"
#include <QApplication>
#include "server.h"
#include <thread>

bool* no_terminate_p;

void server_thread(Window* window){
    Server server;
    while(*no_terminate_p){
        if(server.receive()){
            server.answer(window->gainCurrentModifier());
        }
    }
}

int main(int argc, char *argv[])
{
    bool no_terminate = true;
    no_terminate_p = &no_terminate;
    QApplication app(argc, argv);
    Window window;
    window.show();
    std::thread gui_object(server_thread, &window);
    int status = app.exec();
    *no_terminate_p = false;
    gui_object.join();
    return status;
}
