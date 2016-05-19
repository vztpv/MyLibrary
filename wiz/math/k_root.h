#ifndef K_ROOT_H_INCLUDED
#define K_ROOT_H_INCLUDED


#include <wiz/wizarderror.h>

/// <= 5 58??
/// > 5 Linear search?

namespace wiz{

template <typename UNSIGNED_INTEGER> /// chk...
class K_Root{
private:
    // 복사, 대입 금지..
    K_Root& operator=( const K_Root& );
    K_Root( const K_Root& );
private:
    UNSIGNED_INTEGER* k_root;
    UNSIGNED_INTEGER num; // # of item?
protected:
    void initial( const UNSIGNED_INTEGER k )  {
        if( k <= (UNSIGNED_INTEGER)1 ){ throw wiz::Error( __FILE__, __LINE__,
                                                           "K_Root-initial, k <= 1" ); }//
        UNSIGNED_INTEGER temp=k;
        ////////////////////////////////////////
        UNSIGNED_INTEGER MAXIMUM = UNSIGNED_INTEGER( -1 );

        num = 1; /// 0
        UNSIGNED_INTEGER x = 1;
        bool isOverflow = false;
        while( true ) {
            UNSIGNED_INTEGER temp = 1;
            for( UNSIGNED_INTEGER i = 1; i <= k; i++ )
            {
                UNSIGNED_INTEGER temp2 = temp;
                temp = temp * x;
                if( ( temp / temp2 ) != x ) { isOverflow = true; break; } ///
            }
            if( isOverflow ) { break; }
            else{ num++; x++; }
        }
        /////////////////////////////////////////////////////
        k_root = new UNSIGNED_INTEGER[num];
        for( UNSIGNED_INTEGER i=0; i < num; i++ )
        {
            UNSIGNED_INTEGER temp = 1;
            for( UNSIGNED_INTEGER j = 1; j <= k; j++ )
            {
                temp = temp * ( i );
            }
            k_root[i] = temp;
        }
    }
public:
    UNSIGNED_INTEGER getNum()const { return num; }
    UNSIGNED_INTEGER getK_Rootf( const UNSIGNED_INTEGER val )const{ ///using binary search or inverse linear search?
        if( num > ( (41+85) / 2 ) ) { ///chk... num >= 85 ???
            /// range binary search..??
            UNSIGNED_INTEGER left = 0, right = num-1;
            UNSIGNED_INTEGER middle = ( left + right ) >> 1;

            while( left <= right ) {
                if( val > k_root[middle] ) {
                    left = middle + 1;
                }
                else if( val == k_root[middle] ) {
                    return middle;
                }
                else
                {
                    right = middle - 1;
                }
                middle = ( left + right ) >> 1;
            }
            return middle;
        }
        else {
            for( UNSIGNED_INTEGER i=num-1; i >=0; i-- ) {
                if( val >= k_root[i] )
                {
                    return i;
                }
            }
        }
    }
    UNSIGNED_INTEGER getK_Rootc( const UNSIGNED_INTEGER val ) const
    {
        UNSIGNED_INTEGER temp = getK_Rootf( val );
        if( k_root[temp] != val ){ temp++; }
        return temp;
    }
    bool isX_K( const UNSIGNED_INTEGER val ) const
    {
        UNSIGNED_INTEGER temp = getK_Rootf( val );
        return k_root[temp] == val;
    }
public:
    explicit K_Root( const UNSIGNED_INTEGER k ){
        initial( k );
    }
    ~K_Root(){
        if( k_root != NULL ){
            delete[] k_root;
        }
    }
};

}

#endif // K_ROOT_H_INCLUDED
