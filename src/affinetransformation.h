/*
  * 3D Avatars
  * Pierre Walch
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
