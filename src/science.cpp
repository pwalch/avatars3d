#include <map>
#include <irrlicht.h>
#include "affinetransformation.h"
#include "science.h"

using namespace irr;
using namespace irr::core;


int Science::max(int a, int b)
{
    if(a >= b) {
        return a;
    } else {
        return b;
    }
}
