#include <QApplication>
#include "engine.h"

int main(int argc, char *argv[])
{
    // Engine object handles the application
    Engine& e = Engine::getInstance();
    QApplication app(argc, argv);
    return e.start(app);
}
