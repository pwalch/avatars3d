/*
 * 3D Avatars
 * Pierre Walch
 */

#ifndef SCIENCE_H
#define SCIENCE_H

#include <map>
#include "irrlicht.h"
#include "affinetransformation.h"

using namespace irr;
using namespace irr::core;

/**
 * Class only with static methods, to compute various expressions.
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

};

#endif // SCIENCE_H
