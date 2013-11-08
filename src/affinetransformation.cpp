#include "affinetransformation.h"

AffineTransformation::AffineTransformation(const vector3df& scale,
                                           const vector3df& offset)
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
