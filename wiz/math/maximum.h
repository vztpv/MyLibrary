#ifndef MAXIMUM_H_INCLUDED
#define MAXIMUM_H_INCLUDED

#include <wiz/global.h>
#include <iostream>
using namespace std;

int MAXIMUM_DEBUG = 1;

// using NAMESPACE NUMERICALMETHOD_OPTIMIZATION?
int MAXIMUM_COUT_PRECISION = 6;
/**
* - RoundDifferential
* - Golden-Section Search
* - Parabolic Interpolation
* - Newton`s method-> use root`s Newton-Rapshon!!
* - OptimalSteepestAsecent
*/
/** 편미분, det of the Hessian 중복 주의..*/
class RoundDifferential{
public:
    virtual NUMBER roundFx( const NUMBER x, const NUMBER y, const NUMBER delta ){ // round f value
         NUMBER temp = delta * x;
        return ( f( x + temp , y ) -f( x - temp, y ) )
                    / ( 2* temp ); //
    }
    virtual NUMBER roundFy( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * y;
        return ( f( x, y + temp) - f( x, y - temp ) )
                    / ( 2 * temp );
    }
    virtual NUMBER roundroundFxx( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * x;
        return ( f( x + temp, y ) - 2*f( x, y ) + f( x - temp, y ) )
                        / ( temp * temp );
    }
    virtual NUMBER roundroundFxy( const NUMBER x, const NUMBER y, const NUMBER delta ){
        return roundroundFyx( x, y, delta );
    }
    virtual NUMBER roundroundFyx( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER deltax = delta * x;
         NUMBER deltay = delta * y;
         NUMBER xsumdeltax = x + deltax;
         NUMBER xsubdeltax = x - deltax;
         NUMBER ysumdeltay = y + deltay;
         NUMBER ysubdeltay = y - deltay;
        return ( f( xsumdeltax, ysumdeltay ) - f( xsumdeltax, ysubdeltay )
                    - f( xsubdeltax, ysumdeltay ) + f( xsubdeltax, ysubdeltay ) )
                    / ( 4 * deltax * deltay );
    }
    virtual NUMBER roundroundFyy( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * y;
        return ( f( x , y + temp ) - 2*f( x, y ) + f( x, y - temp ) )
                        / ( temp * temp );
    }
    //
    NUMBER detHessian( const NUMBER x, const NUMBER y, const NUMBER delta ){
        NUMBER temp = roundroundFyx( x, y, delta );
        return roundroundFxx( x, y, delta ) * roundroundFyy( x, y, delta ) - ( temp * temp );
    }
protected:
    virtual NUMBER f( const NUMBER x, const NUMBER y )=0; // cf) function pointer??
private:
};
/**
* Golden-Section Search, find Maximum,
* if you want Minimum of function F,
* find Maximum of -F !!
*/
class GoldenSectionSearch{
public:
    NUMBER get( const NUMBER xlow, const NUMBER xhigh,
             const INTEGER maxit, const NUMBER es, NUMBER& fx ){
         const NUMBER R = ( SQRT( 5 ) - 1 ) / 2; //
         NUMBER xl = xlow;
         NUMBER xu = xhigh;
         INTEGER iter = 0;
         NUMBER d = R * ( xu - xl );
         NUMBER x1 = xl + d;
         NUMBER x2 = xu - d;
         NUMBER f1 = f( x1 );
         NUMBER f2 = f( x2 );
         NUMBER xopt;
         NUMBER ea;
        //
        if( f1 > f2 ){
            xopt = x1;
            fx = f1;
        }
        else{
            xopt = x2;
            fx = f2;
        }
        //
        while( true ){
             d = R * d;
             if( f1 > f2 ){
                xl = x2;
                x2 = x1;
                x1 = xl + d;
                f2 = f1;
                f1 = f( x1 );
             }
             else{
                xu = x1;
                x1 = x2;
                x2 = xu - d;
                f1 = f2;
                f2 = f( x2 );
             }
             //
             iter = iter + 1;
             if( f1 > f2 ){
                xopt = x1;
                fx = f1;
             }
             else{
                xopt = x2;
                fx = f2;
             }
             //
             if( xopt != 0 ){
                ea = ( 1.0 - R ) * ABS( ( xu - xl ) / xopt ) * 100;
                if( 1 == MAXIMUM_DEBUG ){
                    cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "xopt is " << xopt << ", fx is " << fx << ", ea is " << ea << endl;
                }
                //
            }
            if( ea <= es || iter >= maxit ){
                break;
            }
        }
        return xopt;
    }
protected:
    virtual NUMBER f( const NUMBER x )=0;
private:
};
/**
* Parabolic Interpolation
*/
class ParabolicInterpolation{
public:
NUMBER get(  NUMBER x0,  NUMBER x1,  NUMBER x2,
               const INTEGER maxit, const NUMBER es, NUMBER& fx ){
         NUMBER x3=0, xold;
         NUMBER ea;
         NUMBER dx12; // difference of a and b
         NUMBER dx20, dx01;
         NUMBER sx12; // sum of a and b
         NUMBER sx20, sx01;
         INTEGER iter=0;
         NUMBER f3;
        //
        while( true ){
            dx12 = x1 - x2;
            dx20 = x2 - x0;
            dx01 = x0 - x1;
            sx12 = x1 + x2;
            sx20 = x2 + x0;
            sx01 = x0 + x1;
            //
            xold = x3;
            x3 = ( f(x0)*dx12*sx12 + f(x1)*dx20*sx20 + f(x2)*dx01*sx01 )
                    /  ( 2*f(x0)*dx12 + 2*f(x1)*dx20 + 2*f(x2)*dx01 );
            f3 = f( x3 );

            if( x3 != 0 ){ // ea 계산..
                if( iter != 0 ){
                    ea = ABS( ( x3 - xold ) / x3 ) * 100;
                }else{ ea = 100; }
                if( 1 == MAXIMUM_DEBUG ){
                    cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "x3 is " << x3 << ", ea is " << ea << endl; //
                    cout << "f3 is " << f3 << endl;
                }
            }
            //
            if( x3 == x1 || x3 == x2 || x3 == x0 ){
                break;
            }
            // next x0, x1, x2..
            // check 여러가지 방법이 있다.
            ///x0 = x1; x1 = x2; x2 = x3;

            if( x3 < x0 ){ // newx x3 x0
                x2 = x0; x1 = x3;
                x0 = x3 - ( x0 - x3 ); // newx0
            }
            else if( x3 < x1 ){// x0 x3 x1
                x2 = x1; x1 = x3;
            }
            else if( x3 < x2 ){ // x1 x3 x2
                x0 = x1; x1 = x3;
            }
            else if( x3 > x2 ){ // x2 x3 newx
                x0 = x2; x1 = x3;
                x2 = x3 + ( x3 - x2 ); //
            }
            else{
                exit( -9935 );// program error
            }

            iter++;

            if( ea <= es || iter >= maxit ){
                break;
            }
        }

        fx = f( x3 );
        return x3;
    }
    /*NUMBER get(  NUMBER x0,  NUMBER x1,  NUMBER x2,
               const INTEGER maxit, const NUMBER es, NUMBER& fx ){
         NUMBER x3=0;
         NUMBER ea=100, eamin=ea;
         NUMBER dx12; // difference of a and b
         NUMBER dx20, dx01;
         NUMBER sx12; // sum of a and b
         NUMBER sx20, sx01;
         INTEGER iter=0;
         NUMBER f3;
        //
        while( true ){
            dx12 = x1 - x2;
            dx20 = x2 - x0;
            dx01 = x0 - x1;
            sx12 = x1 + x2;
            sx20 = x2 + x0;
            sx01 = x0 + x1;
            //
            x3 = ( f(x0)*dx12*sx12 + f(x1)*dx20*sx20 + f(x2)*dx01*sx01 )
                    /  ( 2*f(x0)*dx12 + 2*f(x1)*dx20 + 2*f(x2)*dx01 );
            f3 = f( x3 );
            //
            if( x3 != 0 ){ // ea 계산..
                if( iter != 0 ){
                    ea = ABS( ( x3 - x2 ) / x3 ) * 100;
                    if( ea < eamin ){ eamin = ea; }
                }else{ ea = 100; }
                if( 1 == MAXIMUM_DEBUG ){
                    cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "x3 is " << x3 << ", ea is " << ea << endl; //
                }
            }
            if( !( f3 > f(x0) && f3 > f(x1) && f3 > f(x2)  ) && ( ea <= es || ea > eamin ) ){
                break;
            }
            // next x0, x1, x2
            x0 = x1; x1 = x2; x2 = x3;
            iter++;

            if( iter >= maxit ){
                break;
            }
        }

        fx = f( x2 ); // 주의!!
        return x2;
    }
*/
protected:
    virtual NUMBER f( const NUMBER x )=0;
private:
};
/**
* Optimal SteepestAsecent
*/
class OptimalSteepestAsecent{
public:
    void get( NUMBER x0, NUMBER y0,
             NUMBER& x, NUMBER& y, // out x, out y
             NUMBER& fmax, // out max of f
             const NUMBER delta, //
             const INTEGER maxit, const NUMBER es ){
         INTEGER iter=0;
         NUMBER eax=100, eay=100;
         NUMBER rfx, rfy;
         NUMBER nx = x0, ny = y0; // new x, new y
         NUMBER h; NUMBER gx;
         NUMBER det, fxx;
        //
        while( true ){
            //
            rfx = roundFx( nx, ny, delta );
            rfy = roundFy( nx, ny, delta );
            if( rfx >= 0 - NUMBERERROR
               && rfx <= 0 + NUMBERERROR
               && rfy >= 0 - NUMBERERROR
               && rfy <= 0 + NUMBERERROR ){
                cout << "both rfx, rfy almost or really are 0" << endl
                     << " this means there are maybe maximum or minimum" << endl;
                x = nx; y = ny;
                fmax = f( x, y );
                break;
            }
            cout << "rfx " << rfx << " rfy " << rfy << endl;
            //
            h = useParabolicInterpolation( -1, 0, 1,
                                      rfx, rfy, nx, ny, maxit, es/10, gx );

            //
            x0 = nx; y0 = ny;
            nx = nx + rfx * h;
            ny = ny + rfy * h;
            //
            cout << "nx " << nx << " ny " << ny << " h " << h << endl;
            det = detHessian( nx, ny, delta );
            if( det > 0 ){
                fxx = roundroundFxx( nx, ny, delta );
                if( fxx > 0 ){
                     cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "(" << nx << "," << ny << ") maybe local minimum." << endl;
                }
                else if( fxx < 0 ){
                     cout << setprecision( MAXIMUM_COUT_PRECISION );
                     cout << "(" << nx << "," << ny << ") maybe local maximum." << endl;
                }
            }
            else if( det < 0 ){
                 cout << setprecision( MAXIMUM_COUT_PRECISION );
                cout << "(" << nx << "," << ny  << ") maybe saddle Point." << endl; //
            }
            else{
                cout << "적절하지 않은 경우 입니다. " << endl;
                x = nx; y = ny;
                fmax = f( x, y );
                return ;
            }
            //

            //
            iter++;
            // ea calcul
            if( nx != 0 ){
                eax = ABS( ( nx - x0 ) / nx ) * 100;
                if( 1 == MAXIMUM_DEBUG ){
                     cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "nx is " << nx << ", ea is " << eax << endl;//
                }
            }
            if( ny != 0 ){
                eay = ABS( ( ny - y0 ) / ny ) * 100;
                if( 1 == MAXIMUM_DEBUG ){
                     cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "ny is " << ny << ", ea is " << eay << endl; //
                }
            }
            if( iter >= maxit || ( eax <= es && eay <= es ) ){
                x = nx; y = ny;
                fmax = f( x, y );
                break;
            }
        }
    }
protected:
private:
    virtual NUMBER f( const NUMBER x, const NUMBER y )=0;
    // max or minimum!!
   NUMBER useParabolicInterpolation(  NUMBER x0,  NUMBER x1,  NUMBER x2,
               const NUMBER rfx, const NUMBER rfy, const NUMBER initx0, const NUMBER inity0,
               const INTEGER maxit, const NUMBER es, NUMBER& fx ){ // return x3;
         NUMBER x3=0, xold;
         NUMBER ea;
         NUMBER dx12; // difference of a and b
         NUMBER dx20, dx01;
         NUMBER sx12; // sum of a and b
         NUMBER sx20, sx01;
         INTEGER iter=0;
         NUMBER f0, f1, f2, f3;
        //
        while( true ){
            dx12 = x1 - x2;
            dx20 = x2 - x0;
            dx01 = x0 - x1;
            sx12 = x1 + x2;
            sx20 = x2 + x0;
            sx01 = x0 + x1;
            /// rfx~0, rfy~0 then, f0 ~ f1 ~ f2 -> 분모 ~ 0, then error!!
            f0 = g( initx0,rfx, inity0,rfy, x0 );
            f1 = g( initx0,rfx, inity0,rfy, x1 );
            f2 = g( initx0,rfx, inity0,rfy, x2 );
            //
            xold = x3;
            /// if f0 = f1 = f2 , then 분모 = 0!!!
            x3 = ( f0*dx12*sx12 + f1*dx20*sx20 + f2*dx01*sx01 )
                    /  ( 2*f0*dx12 + 2*f1*dx20 + 2*f2*dx01 );

            if( 1 == MAXIMUM_DEBUG ){
                cout << "x0 " << x0 << " x1 " << x1 << " x2 " << x2 << endl;
            }
            //
            f3 = g( initx0,rfx, inity0,rfy, x3 );
            if( x3 == x1 || x3 == x2 || x3 == x0 ){
                break;
            }

            // next x0, x1, x2
            ///x0 = x1; x1 = x2; x2 = x3;
            if( x3 < x0 ){ // newx x3 x0
                x2 = x0; x1 = x3;
                x0 = x3 - ( x0 - x3 ); // newx0
            }
            else if( x3 < x1 ){// x0 x3 x1
                x2 = x1; x1 = x3;
            }
            else if( x3 < x2 ){ // x1 x3 x2
                x0 = x1; x1 = x3;
            }
            else if( x3 > x2 ){ // x2 x3 newx
                x0 = x2; x1 = x3;
                x2 = x3 + ( x3 - x2 ); //
            }
            else{
                exit( -9933 );// program error
            }
            iter++;
            if( x3 != 0 ){ // ea 계산..
                if( iter != 1 ){
                    ea = ABS( ( x3 - x1 ) / x3 ) * 100;
                }else{ ea = 100; }
                if( 1 == MAXIMUM_DEBUG ){
                    cout << setprecision( MAXIMUM_COUT_PRECISION );
                    cout << "x3 is " << x3 << ", ea is " << ea << endl; //
                }
            }
            if( ea <= es || iter >= maxit ){
                if( iter >= maxit ){ cout << "maybe over maxit..!!" << endl; }
                //xold = x3; //
                break;
            }
        }
        //
        fx = g( initx0,rfx, inity0,rfy, x3 );
        return x3;
    }
    //
    NUMBER g( const NUMBER x0, const NUMBER rfx,
                const NUMBER y0, const NUMBER rfy,
                const NUMBER h ){
        return f( x0 + (rfx * h), y0 + (rfy * h) ); // h에 관한 함수값을 리턴한다..!!
    }
    virtual NUMBER roundFx( const NUMBER x, const NUMBER y, const NUMBER delta ){ // round f value
         NUMBER temp = delta * x;
        return ( f( x + temp , y ) -f( x - temp, y ) )
                    / ( 2* temp ); //
    }
    virtual NUMBER roundFy( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * y;
        return ( f( x, y + temp) - f( x, y - temp ) )
                    / ( 2 * temp );
    }
    virtual NUMBER roundroundFxx( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * x;
        return ( f( x + temp, y ) - 2*f( x, y ) + f( x - temp, y ) )
                        / ( temp * temp );
    }
    virtual NUMBER roundroundFxy( const NUMBER x, const NUMBER y, const NUMBER delta ){
        return roundroundFyx( x, y, delta );
    }
    virtual NUMBER roundroundFyx( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER deltax = delta * x;
         NUMBER deltay = delta * y;
         NUMBER xsumdeltax = x + deltax;
         NUMBER xsubdeltax = x - deltax;
         NUMBER ysumdeltay = y + deltay;
         NUMBER ysubdeltay = y - deltay;
        return ( f( xsumdeltax, ysumdeltay ) - f( xsumdeltax, ysubdeltay )
                    - f( xsubdeltax, ysumdeltay ) + f( xsubdeltax, ysubdeltay ) )
                    / ( 4 * deltax * deltay );
    }
    virtual NUMBER roundroundFyy( const NUMBER x, const NUMBER y, const NUMBER delta ){
         NUMBER temp = delta * y;
        return ( f( x , y + temp ) - 2*f( x, y ) + f( x, y - temp ) )
                        / ( temp * temp );
    }
    //
    NUMBER detHessian( const NUMBER x, const NUMBER y, const NUMBER delta ){
        NUMBER temp = roundroundFyx( x, y, delta );
        return roundroundFxx( x, y, delta ) * roundroundFyy( x, y, delta ) - ( temp * temp );
    }
};



#endif // MAXIMUM_H_INCLUDED
