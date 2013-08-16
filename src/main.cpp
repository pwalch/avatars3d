/**
  * 3D Avatars
  * Pierre Walch
  */


#include <QApplication>
#include "engine.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    for(int i = 0; i < argc; ++i)
        args.push_back(argv[i]);

    // Engine object handles the application
    Engine& e = Engine::getInstance();
    QApplication app(argc, argv);
    return e.start(app, args);
}
