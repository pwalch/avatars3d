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

#ifndef AFFINETRANSFORMATION_H
#define AFFINETRANSFORMATION_H

#include "irrlicht.h"

using namespace irr::core;

/**
 * @brief Handles an affine transformation between the real coordinate system and the virtual one
 *
 * Represents the affine transformation used to convert real coordinates to virtual coordinates and conversely
 */
class AffineTransformation
{

public:
    /**
     * Creates an affine transformation
     * @param scale scale from real coordinates to virtual coordinates
     * @param offset virtual offset from real coordinates to virtual coordinates
     */
    AffineTransformation(const vector3df& scale, const vector3df& offset);

    /**
     * Converts real coordinates to virtual Irrlicht coordinates
     * @param realVector real vector
     * @return virtual vector
     */
    vector3df convertToVirtual(vector3df realVector) const;

    /**
     * Converts virtual Irrlicht coordinates to real coordinates in meters
     * @param virtualVector virtual vector
     * @return real vector
     */
    vector3df convertToReal(vector3df virtualVector) const;

private:
    vector3df mScale;
    vector3df offset;

};

#endif // AFFINETRANSFORMATION_H
