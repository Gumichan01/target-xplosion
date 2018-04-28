
/*
*   Copyright © 2018 Luxon Jean-Pierre
*   https://gumichan01.github.io/
*
*   This library is under the MIT license
*
*   Luxon Jean-Pierre (Gumichan01)
*   luxon.jean.pierre@gmail.com
*/

/**
*   @file misc.hpp
*   Miscellaneous classes/functions/idoms
*/

#ifndef MISC_INCLUDED
#define MISC_INCLUDED

#include <iostream>
#include <sstream>

namespace misc
{
// This implemantation exists in C++11, but is missing in GCC 4.9
template <typename T>
std::string to_string(const T& v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

// This implemantation exists in C++17, but I am still using C++11, so I need it
template< class ForwardIt, class T >
ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
{
    first = std::find(first, last, value);
    if (first != last)
    {
        for(ForwardIt i = first; ++i != last; )
        {
            if (!(*i == value))
                *first++ = std::move(*i);
        }
    }
    return first;
}

}

#endif // MISC_INCLUDED
