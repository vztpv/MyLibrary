#ifndef RADIXSORT_H_INCLUDED
#define RADIXSORT_H_INCLUDED


#include <utility>
#include <wiz/wizardError.h>
using namespace std;

/// #define RADIXSORT_DEBUG ///

namespace wiz{
/// namespace RadixSort or Sort or etc...
template < typename INTEGER, class T >
class Convert {
public:
    virtual INTEGER toInt( const T& val )const=0;
    INTEGER convert ( const T& val, const INTEGER offset, const INTEGER base )const
    {
        /// if( offset < 1 || offset > getSize(val) ) { throw wiz::Error( "findDigit err" ); }
        INTEGER temp = 1;
        INTEGER digit = -1;

        for( INTEGER i=0; i < offset; i++ )
        {
            temp = temp * base;
        }
        digit = toInt( val ) % temp;
        digit = digit / ( temp / base );

        return digit;
    }

    INTEGER findMaximum( const wizArray<T>& arr )const
    {
        if( arr.empty() ) { throw wiz::Error( "findMaximum, err" ); }
        INTEGER max = toInt( arr[0] );
        for( INTEGER i=1; i < arr.size(); i++ )
        {
            if( max < toInt( arr[i] ) ) { max = toInt( arr[i] ); }
        }
        return max;
    }
};
class ConvertInt : public Convert< int, int > {
public:
    virtual int toInt( const int& val )const { return val; }
};
template < typename INTEGER = int, class T = int > /// int, long long
class RadixSort{
private:
    static INTEGER getSize( const INTEGER x, const INTEGER base )
    {
        INTEGER temp = x;
        INTEGER num = 0;
        if( 0 == temp ) { return 1; } /// chk...
        while( 1 <= temp ) {
            temp = temp / base;
            num++;
        }
        return num;
    }
    static INTEGER convert( const Convert<INTEGER, T>& cvt, const T val, const INTEGER offset, const INTEGER base  )
    {
        return cvt.convert( val, offset, base );
    }
    static void Sort( const Convert<INTEGER, T>& cvt, const wizArray<T>& a, wizArray<T>& b, const INTEGER base, const INTEGER offset )
    {
        wizArray<INTEGER> c( base );

        for( INTEGER i=0; i < c.size(); i++ ) { c[i] = 0; }
        for( INTEGER i=0; i < a.size(); i++ )
        {
            c[ convert( cvt, a[i], offset, base ) ]++;
        }
        for( INTEGER i=1; i < base; i++ )
        {
            c[i] = c[i] + c[i-1];
        }
        for( INTEGER i=a.size()-1; i >= 0; i-- )
        {
            const INTEGER idx = convert( cvt, a[i], offset, base ); /// digit..
            b[ c[ idx ]-1 ] = a[i];
            c[ idx ]--;
        }
    }
public:
    static void sort( const Convert<INTEGER, T>& cvt, const wizArray<T>& input, wizArray<T>& result, const INTEGER base )
    /// input[idx] >= 0
    {
        #ifdef RADIXSORT_DEBUG
        if( input.size() > result.size() ) { throw wiz::Error( __FILE__, __LINE__, "err.." ); }
        if( base < 2 ) { throw wiz::Error( " base < 2 " ); }
        #endif

        if( input.size() <= 1 ) { result = input; return; }

        wizArray<T> temp( input.size() );
        INTEGER max = cvt.findMaximum( input ); /// maybe.. max > 0
        INTEGER size = getSize( max, base );
        temp = input;
        Sort( cvt, temp, result, base, 1 );
        for( INTEGER i=1; i < size; i++ ) {
           /// temp = result;
            temp.swap( result );
            Sort( cvt, temp, result, base, i+1 );
        }
    }
};

}


#endif // RADIXSORT_H_INCLUDED
