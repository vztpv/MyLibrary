#ifndef CPP_DOUBLE_H_INCLUDED
#define CPP_DOUBLE_H_INCLUDED

#include <wiz/global.h>
#include <sstream>
using namespace std;
#include <wiz/wizarderror.h>

namespace wiz {
    double toDouble( string str )
    {
        stringstream stream;
        stream <<str;
        double x;
        if( !(bool)(stream >> x) ) {  throw Error( __FILE__, __LINE__, "str do not convert double type."  ); }

        return x;
    }
    class _double
    {
    private:
        double x;
        bool binit;
    public:
        _double():x(0), binit(false) { }
        _double( const double x ) : x( x ), binit(true)
        { }
        void set( double x ) { binit = true; this->x = x; }
        double get()const{
            if( !binit ) {
                throw wiz::Error("_double is not init..");
            }
            return x;        }
        bool isInit()const { return binit; }
    };
}

#endif // CPP_DOUBLE_H_INCLUDED
