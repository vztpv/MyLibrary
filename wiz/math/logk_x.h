
#ifndef LOGK_X_H_INCLUDED
#define LOGK_X_H_INCLUDED

#include <cmath>
#include <wiz/wizarderror.h>

namespace wiz{

template <typename UNSIGNED_INTEGER> /// chk...
class Logk{
private:
    // 복사, 대입 금지..
    Logk& operator=( const Logk& );
    Logk( const Logk& );
private:
    UNSIGNED_INTEGER* logk;
    UNSIGNED_INTEGER num; // # of item?
protected:
    void initial( const UNSIGNED_INTEGER k )  {
        if( k <= (UNSIGNED_INTEGER)1 ){ throw wiz::Error( __FILE__, __LINE__,
                                                           "Logk-initial, k <= 1" ); }//
        UNSIGNED_INTEGER temp=k;
        ////////////////////////////////////////
        UNSIGNED_INTEGER MAXIMUM = UNSIGNED_INTEGER( -1 );

        // num = floor( log10( (double)MAXIMUM ) / log10( (double)k )  ) + 1; /// chk... TO-DO..
        num = 2; /// 1, k

        while( true )
        {
            UNSIGNED_INTEGER x = temp * k;
            if( x / temp != k ) { break; } /// chk overflow.
            num++;
            temp = x;
        }
        /////////////////////////////////////////////////////
        logk = new UNSIGNED_INTEGER[num];
        logk[0] = 1; //
        temp = k;
        for( UNSIGNED_INTEGER i=1;i< num;i++ ){
            logk[i] = temp;
            temp = temp * k;
        }
    }
public:
    // floor( getLogk(val) )
    UNSIGNED_INTEGER getLogkf( const UNSIGNED_INTEGER val )const{
        if( val <= (UNSIGNED_INTEGER)0 ){ throw wiz::Error( "err:Logk-getLogkf" ); }//

        for( UNSIGNED_INTEGER i=num-1; i >= 0; i-- ) {
            if( val >= logk[i] ) { return i; }
        }
        /// throw wiz::Error( "err2:Logk-getLogkf" );
    }
    // celing( getLogk(val) )
    UNSIGNED_INTEGER getLogkc( const UNSIGNED_INTEGER val )const
    {
        UNSIGNED_INTEGER temp = getLogkf( val );
        if( logk[temp] != val ){ temp++; }
        return temp;
    }
    bool isK_N( const UNSIGNED_INTEGER val ) const
    {
        UNSIGNED_INTEGER temp = getLogkf( val );
        return logk[temp] == val;
    }
    UNSIGNED_INTEGER getNum()const { return num; }
public:
    explicit Logk( const UNSIGNED_INTEGER k ){
        initial( k );
    }
    ~Logk(){
        if( logk != NULL ){
            delete[] logk;
        }
    }
};

}
#endif // LOGK_X_H_INCLUDED
