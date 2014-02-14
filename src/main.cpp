/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include "engine.h"

/**
 * @mainpage Avatars
 *
 * This program is able to visualize trajectory data in virtual 3D scenes. The entry point is Engine::start().
 */

/**
 * Creates Engine and initializes it
 * @param argc argument count
 * @param argv actual arguments
 * @return status
 */
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
