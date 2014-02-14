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

#include "affinetransformation.h"

AffineTransformation::AffineTransformation(const vector3df& scale, const vector3df& offset)
{
    this->mScale = scale;
    this->offset = offset;
}

vector3df AffineTransformation::convertToVirtual(vector3df real) const
{
    return vector3df(real.X * mScale.X + offset.X,
                     real.Z * mScale.Z + offset.Z,
                     real.Y * mScale.Y + offset.Y);
}

vector3df AffineTransformation::convertToReal(vector3df vrtl) const
{
    return vector3df((vrtl.X - offset.X) / mScale.X,
                     (vrtl.Z - offset.Y) / mScale.Y,
                     (vrtl.Y - offset.Z) / mScale.Z);
}
