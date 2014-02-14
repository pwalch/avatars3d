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

#ifndef ITIMEABLE_H
#define ITIMEABLE_H

/**
 * @brief Interface for objects having a different state over time
 *
 * Provides an interface for all the objects depending on time in the program.
 */
class ITimeable
{

public:
    /**
     * Abstract method to adapt object state to the wanted time value
     * @param time frame index
     */
    virtual void setTime(int time) = 0;
};

#endif // ITIMEABLE_H
