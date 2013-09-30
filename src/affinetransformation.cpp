#include "affinetransformation.h"

AffineTransformation::AffineTransformation(const vector3df& scale, const vector3df& offset)
{
    this->scale = scale;
    this->offset = offset;
}

vector3df AffineTransformation::convertToVirtual(vector3df real) const
{
    return vector3df(real.X * scale.X + offset.X,
            real.Z * scale.Z + offset.Z,
            real.Y * scale.Y + offset.Y);
}

vector3df AffineTransformation::convertToReal(vector3df vrtl) const
{
    return vector3df((vrtl.X - offset.X) / scale.X,
            (vrtl.Z - offset.Y) / scale.Y,
            (vrtl.Y - offset.Z) / scale.Z);
}
