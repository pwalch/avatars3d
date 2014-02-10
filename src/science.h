/*
 * 3D Avatars
 * Pierre Walch
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
