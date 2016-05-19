// use Surrogate?
// function,
// 미분 공식(use h)
//
// function class
#include <iostream>
using namespace std;
#include "global.h"

// make Differential를 상속받은 것을
//        Prototype으로 만든다.( 즉, 전역변수로 선언해놓고 골라서, &변수이름 처럼 사용한다? )
//

class Func{
public:
    virtual NUMBER f( NUMBER x )=0;
};

class Differential{
public:
    virtual NUMBER df( Func* func, NUMBER x, NUMBER h )=0;
};

class Richardson
{
public:
            // use dif->df and func..
    static NUMBER df( Differential* dif, Func* func, NUMBER x, NUMBER h )
    {
        return ( 4*( dif->df( func, x, h ) ) - ( dif->df( func, x, 2*h ) ) )
                                / 3;
    }
};
//
class DefaultDifferential : public Differential{
public:
    virtual NUMBER df( Func* func, NUMBER x, NUMBER h ){
        return ( func->f( x+h ) - func->f( x-h ) ) / ( 2*h );
    }
};
class FastCenteredDifferential : public Differential{
public:
    virtual NUMBER df( Func* func, NUMBER x, NUMBER h ){ // unsigned h
        return ( -1*func->f( x+2*h ) + 8*func->f( x+h )
                   - 8*func->f(x-h) + func->f( x-2*h ) )
                 / ( 12*h );
    }
};
DefaultDifferential DEFAULT_DIF;
FastCenteredDifferential FAST_CENTER_DIF;
class FFastCenteredDifferential : public Differential{ // FAST FAST CENTERED DIFFERENTIAL
public:
    virtual NUMBER df( Func* func, NUMBER x, NUMBER h ){
        return Richardson::df( &FAST_CENTER_DIF, func, x, h );
    }
};
FFastCenteredDifferential FFCD;
class f1 : public Func{
public:
    virtual NUMBER f( NUMBER x ){
        return SIN( x );
        //return -0.1*POW(x,4) -0.15*POW(x,3) - 0.5*x*x - 0.25*x +1.2;
    }
};
f1 f1Func;
//
int main( void ){
    NUMBER x = PI; //PI;
    NUMBER h = 0.0001;
    cout << setprecision( 20 );

    cout << DEFAULT_DIF.df( &f1Func, x, h ) << endl;

    cout << Richardson::df( &DEFAULT_DIF, &f1Func, x, h ) << endl;

    cout << -1 - FAST_CENTER_DIF.df( &f1Func, x, h ) << endl;

    cout << -1 - Richardson::df( &FAST_CENTER_DIF, &f1Func, x, h ) << endl;

    cout << FFCD.df( &f1Func, x, h ) << endl;
    cout << FFCD.df( &f1Func, x, 2*h ) << endl;
    cout << Richardson::df( &FFCD, &f1Func, x, h ) << endl;
    //
    return 0;
}
