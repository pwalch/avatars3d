/*
 * 3D Avatars
 * Pierre Walch
 */

#include <algorithm>
#include "science.h"


int Science::max(int a, int b)
{
    if(a >= b) {
        return a;
    } else {
        return b;
    }
}

std::vector<std::string> Science::split(const std::string &line)
{
    return split(line, " ", false);
}

std::vector<std::string> Science::split(const std::string &s, const std::string &delim, bool keep_empty)
{
    std::vector<std::string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

