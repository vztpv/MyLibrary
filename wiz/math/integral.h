#ifndef INTEGRAL_H_INCLUDED
#define INTEGRAL_H_INCLUDED

#include "global.h"

/** To Do
* single app of trapezoidal rule
* multiple app of t~
* composite t~
* single app of Simpson`s 1/3 rule
* multi app of S~ 1/3 rule
* single app of Simpson`s 3/8 rule
* multi app of Simpson`s rule
* Simpson`s 3/8 rule
* Boole`s rule
*/
namespace INTEGRAL{
class Integral{
public:
    /* single and multiple trapezoidal */
    NUMBER Trap( const NUMBER a, const NUMBER b ){
        return  (b-a) * ( f(a) + f(b) ) / 2;
    }
    NUMBER TrapM( const NUMBER a, const NUMBER b, const INTEGER n ){
        Array<NUMBER> fx( n+1, 0 );
        NUMBER sum;
        NUMBER h = ( b - a ) / n;

        fxInitial( a, b, n, fx );

        sum = fx[0];
        for( int i=1;i <= n-1;i++ ){
            sum = sum + 2 * fx[i];//
        }
        sum = sum + fx[n];
        sum = sum * h / 2;
        return sum;
    }
    /* ~single and multiple trapezoidal */
    /* Simpson`s rules */
    /// 1/3rule은  segment수가 짝수개여야 한다.
    NUMBER Simp13( const NUMBER a, const NUMBER b ){
        NUMBER h = ( b - a )/ 2;
        Array<NUMBER> fx( 3, 0 );
        fxInitial( a, b, 2, fx );
        return (b-a) * ( fx[0] + 4*fx[1] + fx[2] ) / 6;
    }
    NUMBER Simp38( const NUMBER a, const NUMBER b ){
        Array<NUMBER> fx( 4, 0 );
        fxInitial( a, b, 3, fx );
        return ( b-a ) * ( fx[0] + 3*(fx[1]+fx[2]) + fx[3] ) / 8;
    }
    /// 1/3rule은  segment수가 짝수개여야 한다.
    NUMBER Simp13M( const NUMBER a, const NUMBER b, const INTEGER n ){
        Array<NUMBER> fx( n+1, 0 );
        NUMBER sum;
        NUMBER h = ( b - a ) / n;

        fxInitial( a, b, n, fx );

        sum = fx[0];
        for( int i=1;i <= n-2;i=i+2 ){
            sum = sum + 4*fx[i] + 2*fx[i+1];
        }
        sum = sum + 4*fx[n-1] + fx[n];
        return h* sum / 3;
    }
    NUMBER SimpInt( const NUMBER a, const NUMBER b,
                    const INTEGER n ){
        const NUMBER h = ( b - a ) / n; //
        Array<NUMBER> fx( n+1, 0 );
        NUMBER sum = 0;

        fxInitial( a, b, n, fx );

        if( 1 == n ){
            sum = Trap( xk( a, h, n-1), xk( a, h, n ) );
        }
        else{
            INTEGER M = n;
            NUMBER oddTemp = M / 2;
            NUMBER odd = (NUMBER) M / 2 - oddTemp;
            if( odd > 0 AND n > 1 ){
                sum = sum + Simp38( xk( a, h, n-3 ), xk( a, h, n ) );
                M = n - 3;
            }
            if( M > 1 ){
                sum = sum + Simp13M( a, xk( a, h, M ), M );
            }
        }
        return sum;
    }
    /*~Simpson`s rules */
	/**
	* to do
	* Trapun
	* combination rule 
	*/
	NUMBER Trapun( const Array<NUMBER>& x, const INTEGER n ){
		NUMBER sum=0;
		for(int i=1;i <= n;i++ ){
			sum = sum + ( x[i] - x[i-1] ) * ( f( x[i-1] ) + f( x[i] ) ) / 2;
		}
		return sum;
	}
	/* combination rule */
	NUMBER Uneven( const Array<NUMBER>& x, const int n ){
		/**
		* to do...
		*/
	}

    /* Boole`s rule */
	/// NUMBER Boole( const NUMBER a, constNUMBER b ) // h = (b-a)/4
    NUMBER Boole( const NUMBER a, const NUMBER b){
		Array<NUMBER> fx( 5 , 0); // 4+1
		fxInitial( a, b, 4, fx );
        return (b-a) * ( 7*fx[0] + 32*fx[1] + 12*fx[2] + 32*fx[3] + 7*fx[4] ) / 90; //
    }
protected:
    virtual NUMBER f( const NUMBER x )=0;
private:
    void fxInitial( const NUMBER a, const NUMBER b, const INTEGER n, Array<NUMBER>& fx ){
        const NUMBER h = (b-a)/n;
        fx[0] = f( a ); fx[n] = f( b );
        for( int i=1;i< n;i++ ){
            fx[i] = f( a + h*i ); //
        }
        //cout << "fx check.." << endl;
        //fx.debug();
    }
    NUMBER xk( const NUMBER a, const NUMBER h, const NUMBER k ){
        return a + h * k ;
    }
};

}
#endif // INTEGRAL_H_INCLUDED
