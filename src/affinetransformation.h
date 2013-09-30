#ifndef AFFINETRANSFORMATION_H
#define AFFINETRANSFORMATION_H

#include "irrlicht.h"

using namespace irr::core;

/**
 * @brief Handles an affine transformation
 *
 * Represents the affine transformation used to convert real coordinates from/to virtual
 * coordinates
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
         * Converts real coordinates in meters of a point to virtual Irrlicht coordinates
         * @param real position in meters
         * @return virtual position of the input point
         */
        vector3df convertToVirtual(vector3df real) const;

        /**
         * Converts virtual Irrlicht coordinates to real coordinates in meters
         * @param vrtl virtual position
         * @return real position of the input point
         */
        vector3df convertToReal(vector3df vrtl) const;

    private:
        vector3df scale;
        vector3df offset;


};

#endif // AFFINETRANSFORMATION_H
