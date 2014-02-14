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

#ifndef SCIENCE_H
#define SCIENCE_H

#include <vector>
#include <string>
#include <map>
#include "irrlicht.h"
#include "affinetransformation.h"

using namespace irr;
using namespace irr::core;

/**
 * Class only with static methods, to compute various expressions
 */
class Science
{

public:

    /**
     * Returns the biggest integer between a and b
     * @param a first integer
     * @param b second integer
     * @return biggest integer
     */
    static int max(int a, int b);

    /**
     * Splits a trajectory line into string tokens with space separator, ignoring empty strings
     * @param line to parse
     * @return string token list
     */
    static std::vector<std::string> split(const std::string& line);

private:
    static std::vector<std::string> split(const std::string& str, const std::string& delim, bool keep_empty = true);

};

#endif // SCIENCE_H
