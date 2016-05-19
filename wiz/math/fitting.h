#ifndef FITTING_H_INCLUDED
#define FITTING_H_INCLUDED

#include <wiz/global.h>
#include <wiz/newArrays.h>
#include <wiz/root.h>
using namespace wiz::ROOT;
/// remove register, INTEGER->template? 일부 INTEGER->int?
/**
* Linear Fitting
*/
namespace wiz{
INTEGER FITTING_DEBUG = 0;
INTEGER FITTING_COUT_PRECISION = 6;

void Regress( const Array<NUMBER>& x, const Array<NUMBER>& y,
             NUMBER& a1, NUMBER& a0, NUMBER& syx, NUMBER& r  ){ // syx is standard error, r = correlation cofficient
    register NUMBER sumx=0,sumxy=0,st=0;
    register NUMBER sumy=0,sumx2=0,sr=0;
    register NUMBER xm=-1,ym=-1;
    register const INTEGER n = x.size(); //

    if( x.size() != y.size() ){ return; } // error

    for( int i=0;i< n;i++ ){
        sumx = sumx + x[i];
        sumy = sumy + y[i];
        sumxy = sumxy + x[i]*y[i];
        sumx2 = sumx2 + x[i]*x[i];
    }

    xm = sumx / n;
    ym = sumy / n;

    a1 = ( n*sumxy - sumx*sumy ) / ( n*sumx2 - sumx*sumx );
    a0 = ym - a1*xm;

    for( int i=0;i< n;i++ ){
        register NUMBER t1 = y[i] - ym;
        register NUMBER t2 = y[i] - a1*x[i] - a0;
        st = st + t1*t1;
        sr = sr + t2*t2;
    }
    syx = SQRT( sr / ( n - 2 ) );
    r = SQRT( ( st - sr ) / st ); //
}
/**
* n-Polynomial Fitting
*/
class PolynomialFitting{
public:
	void get( const Array<NUMBER>& x, const Array<NUMBER>& y, // input data
			 Array<NUMBER>& _a, // 구하고자하는 값들, 계수..
			 NUMBER& syx, NUMBER& r, // 통계관련 정보
			 INTEGER& er ){ // 에러체크 1이면 에러.
		const INTEGER order = _a.size() - 1; // 상수항 포함..
		// if n < m + 1 // error
		register const INTEGER n = x.size();
		if( n != y.size() ){ return; } // (x,y)가 한 쌍이므로 개수가 같아야 한다.
		if( n < order + 1 ){ // (x,y)개수는 계수의 개수보다 크거나 같아야 한다.
			cout << n << " <  " << order << " + 1 " << endl;
			return;
		}
		SquareSecondArray<NUMBER> A( order+1 );
		Array<NUMBER> B( order+1 );
		CroutLU solveMatrix;
		//
		normalization( A, B, x, y, order );
		//
		solveMatrix.get( A, B, order+1, _a, er ); /// check..
		{ // get syx, r
			syx = 0; r = 0;
			register NUMBER sr = 0;
			register NUMBER st = 0;
			register NUMBER sum = 0;
			register NUMBER ymean = 0;
			// calcul ymean
			for( INTEGER i=0;i< n;i++ ){
				sum = sum + y[i];// sum calcul
			}
			ymean = sum / n;
			// calcul sr, st
			sum = 0;
			for( INTEGER i = 0; i< n; i++ ){
				sum = _a[0];
				NUMBER temp= x[i]; NUMBER multi = x[i];
				for( INTEGER j = 1; j< _a.size();j++ ){
					sum = sum + _a[j]*temp;
					temp = temp * multi;
				}
				sr = sr + ( y[i] - sum ) * ( y[i] - sum );
				st = st + ( y[i] - ymean ) * ( y[i] - ymean );
			}
			// calcul syx, r
			syx = SQRT( sr / ( n-_a.size() ) );
			r = SQRT( ( st - sr ) / st );
		}
	}
private:
	void normalization( SquareSecondArray<NUMBER>& a, Array<NUMBER>& b, const Array<NUMBER>& x, const Array<NUMBER>& y
		, const INTEGER order ){ // 교재 p474참고
		register NUMBER sum = 0;
		register const INTEGER n = x.size(); // input쌍의 개수..
		if( n < 1 ){ return; }
		/* a process */
		for( INTEGER i = 0; i <= order; i++ ){
			for( INTEGER j = 0; j<= i; j++ ){
				register INTEGER k = i + j; //
 				sum = 0; //reset
				for( INTEGER m = 0; m < n; m++ ){ // 교재에는 소문자 L
					sum = sum + POW( x[m], k );
				}
				a[i][j] = sum;
				a[j][i] = sum;
			}
		}
		/* b process */
		Array<NUMBER> a_temp( x.size(), 1 );
		register const INTEGER size = order+1;
		sum = 0;
		//
		for( int i=0;i< size;i++ ){
			sum = 0;
			for( int j=0;j< n;j++ ){
				sum = sum + a_temp[j] * y[j];
				a_temp[j] = a_temp[j] * x[j]; // for xi^(i+1), yi
			}
			b[i] = sum;
		}
	}
};
/**
* Interpolation
* - Newton`s Interpolating Polynomials. - error%포함
* - LAGRNG Interpolating Polynomials. -error%포함 하지않음.
*/
class NewtonInterPolatingPolynomial{
public:
    /// newX의 f값을 추측한다.
    void get( const NUMBER newX, const Array<NUMBER>& x, const Array<NUMBER>& b,
                  Array<NUMBER>& value, Array<NUMBER>& error ){
        // length check
        if( x.size() != b.size() ){ return; }
        if( x.size() != value.size() ){ return; }
        if( x.size() != error.size() ){ return; }
        register const INTEGER n = b.size();
        register NUMBER xvals=1;
        //
        value[0] = b[0];
        for( INTEGER i=1;i< n;i++ ){
            xvals = xvals * ( newX - x[i-1] );
            value[i] = value[i-1] + b[i] * xvals;
            error[i-1] = value[i] - value[i-1];
        }
    }
    void get( const Array<NUMBER>& x, const Array<NUMBER>& y, /* data.. */
            Array<NUMBER>& b // b0 ~ bn,
            ){
        // x.size == y.size
        if( x.size() != y.size() ){ return; }
        // order = b.size()-1
        const INTEGER order = b.size() - 1;
        /// x.size == order + 1 ???
        if( x.size() != order + 1 ){ return; }
        // make double Array
        SquareSecondArray<NUMBER> f_( x.size(), 0 ); // for error check.
        // f_setting
        f_setting( f_, x, y );
        // for 0 ~ b.size(), b[i] = f_[i][0] // 교과서에 나온 테이블을 Transpose한 것..
        for( int i=0; i< b.size(); i++ ){
            b[i] = f_[i][0];
        }
    }
private:
    void f_setting( SquareSecondArray<NUMBER>& f_,
                   const Array<NUMBER>& x, const Array<NUMBER>& y
        ){
        register INTEGER n = x.size(); //
        // fdd i,0 = yi -> fdd 0,i = yi,
        f_[0] = y; // Array<NUMBER> = Array<NUMBER> // look Array<T> class
        //
        for( INTEGER i=1;i< n;i++ ){
            for( INTEGER j=0;j< n-i;j++ ){
                f_[i][j] = ( f_[i-1][j+1] - f_[i-1][j] ) / ( x[i+j] - x[j] );
            }
        }
        //f_.debug( FITTING_COUT_PRECISION );
    }
};


}
#endif // FITTING_H_INCLUDED
