#ifndef CPP_INT_H_INCLUDED
#define CPP_INT_H_INCLUDED

#include <wiz/global.h>
#include <wiz/wizardError.h>
#include <cmath>

namespace wiz{

int _toInt( const string str )
{
    int x = atoi( str.c_str() );
    if( 0 == x && str != "0" ) /// cf) -0? +0? 0.0?
    {
        throw Error( __FILE__, __LINE__, "_toInt" );
    }
    return x;
}

}
#endif // CPP_INT_H_INCLUDED
