#ifndef WIZBITGLOBAL_H_INCLUDED
#define WIZBITGLOBAL_H_INCLUDED

#include <wiz/global.h>

namespace wiz {

// isSmallEndian?
inline bool isSmallEndian() // Little
{
    int x = 1;
    char* c = (char*)&x;

    return (*c) == 1; // in intel?
}
inline bool isBigEndian()
{
    return !isSmallEndian();
}

}
#endif // BITGLOBAL_H_INCLUDED
