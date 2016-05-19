#ifndef CLONG_INTEGER_H_INCLUDED
#define CLONG_INTEGER_H_INCLUDED

#include <wiz/global.h>
#include <wiz/newArrays.h>

namespace wiz{
const wiz::Array<char> zero_int( 1, '0' ); /// ZERO_INT
// const wiz::Array<char> one_int( 1, '1' );
const wiz::Array<char> ndef_int; ///  empty state..
inline int to_int( const char ch ) { return ch - '0'; }
inline char to_char( const int ival ) { return ival + '0'; } // move to global.h?

template <class T>
bool IsSameValues( const Array<T>& arr1, const Array<T>& arr2 )
{
    if(arr1.size()!=arr2.size() ) { return false; }
    for( int i=0; i < arr1.size(); i++ )
    {
        if( arr1[i] != arr2[i] ) { return false; }
    }
    return true;
}
string to_str( const wiz::Array<char>& longInt )
{
    char* temp = new char[longInt.size()+1];

    for( int i=0; i < longInt.size(); i++ )
    {
        temp[i] = longInt[i];
    }
    temp[longInt.size()] = '\0';
    string str = temp;
    delete[] temp;
    return str;
}
wiz::Array<char> to_char_arr( const string& str )
{
    wiz::Array<char> temp( str.size() );

    for( int i=0; i < temp.size(); i++ )
    {
        temp[i] = str[i];
    }

    return temp;
}
wiz::Array<char> remove_first_zeros( const wiz::Array<char>& x ) /// remove first all zeros.
{
    int zero_num = 0;

    for( int i=0; i < x.size();i++ )
    {
        if( x[i] != '0' ) { break; }
        zero_num++;
    }
    if( 0 == zero_num ) { return x; }
    if( x.size() == zero_num ) { return ndef_int; } /// chk...
    wiz::Array<char> temp( x.size() - zero_num );
    for( int i=0; i < temp.size(); i++ )
    {
         temp[i] = x[i+zero_num];
    }

    return temp;
}

bool operator<( const wiz::Array<char>& x, const wiz::Array<char>& y )
{
    if( x.size() > y.size() ) { return false; }
    if( x.size() < y.size() ) { return true; }
    if( x.empty() ) // also y.empty..
    {
        return false; /// chk?
    }
    for( int i=0; i < x.size(); i++ )
    {
        if( x[i] > y[i] ) { return false; }
        if( x[i] < y[i] ) { return true; }
    }
    // same...
    return false;
}

wiz::Array<char> add_last_zeros( const wiz::Array<char>& x, const int n )
{
    if( x.size()+n <= 0 ) { return ndef_int; }
    wiz::Array<char> temp( x.size() + n, '0' );
    for( int i=0; i < x.size(); i++ )
    {
        temp[i] = x[i];
    }
    return temp;
}
wiz::Array<char> _int_plus( const wiz::Array<char>& x, const wiz::Array<char>& y )
{
    // err-> return empty array of char.
    if( x.empty() || y.empty() ) { return ndef_int; }
    wiz::Array<char> _x = remove_first_zeros( x );
    wiz::Array<char> _y = remove_first_zeros( y );
    if( _x.empty() ) { _x = zero_int; }
    if( _y.empty() ) { _y = zero_int; }

    const int max = wiz::max( _x.size(), _y.size() );
    const int min = wiz::min( _x.size(), _y.size() );
    wiz::Array<char> temp( max + 1, '0' );
    int itemp=0;
    int w = _x.size()-1;
    int v = _y.size()-1;
    int u = temp.size()-1;


    for( int num=0; num < min; num++ )
    {
        int sum = to_int( _x[w] ) + to_int( _y[v] ) + itemp;
        temp[u] = to_char( sum % 10 );
        itemp = sum / 10;
        w--;
        v--;
        u--;
    }
    for(;w >= 0; w-- )
    {
        int sum = to_int( _x[w] ) + itemp;
        temp[u] = to_char( sum % 10 );
        itemp = sum / 10;
        u--;
    }
    for(;v >= 0; v-- )
    {
        int sum = to_int( _y[v] ) + itemp;
        temp[u] = to_char( sum % 10 );
        itemp = sum / 10;
        u--;
    }
    temp[0] = to_char( itemp );

    wiz::Array<char> ret = remove_first_zeros( temp );
    if( ret.empty() ){ return wiz::zero_int; }
    return ret;
}
wiz::Array<char> _int_minus( const wiz::Array<char>& x, const wiz::Array<char>& y ) // x >= y
{
    if( x.empty() || y.empty() ) { return ndef_int; }
    wiz::Array<char> _x = remove_first_zeros( x );
    wiz::Array<char> _y = remove_first_zeros( y );
    if( _x.empty() ) { _x = zero_int; }
    if( _y.empty() ) { _y = zero_int; }

    if( _x == _y ) { return wiz::zero_int; }
    if( _x < _y ) { throw wiz::Error( __FILE__, __LINE__, " in int_minus, must be x >= y, but now x < y. " ); }


    wiz::Array<char> temp( _x.size(), '0' );
    const int min = _y.size();
    int w = _x.size()-1;
    int v = _y.size()-1;
    int u = temp.size()-1;
    int itemp = 0;

    for( int num=0; num < min; num++ )
    {
        int dif = to_int( _x[w] ) - to_int( _y[v] );
        if( dif < 0 ) /// chk!!
        {
            dif = dif + 10;
            int count=w-1;
            while( _x[count] == '0' ) {
                _x[count] = '9';
                count--;
            }
            _x[count]--;
        }
        temp[u] = to_char( dif % 10 );

        w--;
        v--;
        u--;
    }
    //
    for(;w >= 0; w-- )
    {
        int dif = to_int( _x[w] );
        temp[u] = to_char( dif % 10 );
        u--;
    }

    wiz::Array<char> ret = remove_first_zeros( temp );
    if( ret.empty() ){ return wiz::zero_int; }
    return ret;
}
wiz::Array<char> _int_multiple( const wiz::Array<char>& x, const char ch )
{
    if( x.empty() ) { return ndef_int; }
    wiz::Array<char> _x = remove_first_zeros( x );
    if( _x.empty() ) { _x = zero_int; }

    wiz::Array<char> temp( _x.size()+1, '0' );
    int itemp = 0;

    for( int i=temp.size()-1; i >= 1; i-- )
    {
        int sum = to_int( ch ) * to_int( _x[i-1] ) + itemp;
        temp[i] = to_char( sum%10 );
        itemp = sum / 10;
    }
    temp[0] = to_char( itemp );
    wiz::Array<char> ret = remove_first_zeros( temp );
    if( ret.empty() ){ return wiz::zero_int; }
    return ret;
}
wiz::Array<char> _int_multiple( const wiz::Array<char>& x, const wiz::Array<char>& y )
{
    if( x.empty() || y.empty() ) { return ndef_int; }
    wiz::Array<char> _x = remove_first_zeros( x );
    wiz::Array<char> _y = remove_first_zeros( y );
    if( _x.empty() ) { _x = zero_int; }
    if( _y.empty() ) { _y = zero_int; }

    wiz::Array<char> sum=zero_int;

    int w=0;
    for( int i=_y.size()-1; i >= 0; i-- ) {
        wiz::Array<char> temp = add_last_zeros( _int_multiple( _x, _y[i] ), w );
        if( temp.empty() ) {
            temp = wiz::zero_int;
        }
        wiz::Array<char> temp2 = _int_plus( sum, temp );
        sum.DoEmpty();
        sum = temp2;
        w++;
    }

    wiz::Array<char> ret = remove_first_zeros( sum );
    if( ret.empty() ){ return wiz::zero_int; }
    return ret;
}
wiz::Array<char> _int_sub( const wiz::Array<char>& x, const int start, const int end )
{
    // chk start <= end!
    wiz::Array<char> temp( end-start+1 );
    for( int i=0; i < temp.size(); i++ )
    {
        temp[i] = x[i+start];
    }

    return temp;
}

wiz::Array<char> to_Array( const vector<char>& vec )
{
    if( vec.empty() ){ return ndef_int; }
    wiz::Array<char> temp ( vec.size() );

    for( int i=0; i < temp.size(); i++ )
    {
        temp[i] = vec[i];
    }
    return temp;
}
wiz::Array<char> _int_concat( const wiz::Array<char>& x, const wiz::Array<char>& y )
{
    if( x.empty() && y.empty() ) { return ndef_int; }
    wiz::Array<char> temp( x.size() + y.size() );
    int count = 0;
    for( int i=0; i < x.size();i++ )
    {
        temp[count] = x[i];
        count++;
    }
    for( int i=0; i < y.size();i++ )
    {
        temp[count] = y[i];
        count++;
    }

    return temp;
}

/// chk this...
bool _int_divide( const wiz::Array<char>& x, const wiz::Array<char>& y,
                 wiz::Array<char>& quotient, // ¸ò
                 wiz::Array<char>& remainder ) // ³ª¸ÓÁö
{
    if( x.empty() || y.empty() ) { return false; } /// chk???
    wiz::Array<char> _x = remove_first_zeros( x );
    const wiz::Array<char> _y = remove_first_zeros( y );

    if( _y.empty() ) { throw wiz::Error( __FILE__, __LINE__, "_int_divide, x/0 error" ); }
    if( _x.empty() ) { _x = zero_int; }
    if( _x < _y ) {
        quotient.DoEmpty(); quotient = wiz::zero_int;
        remainder.DoEmpty();
        remainder = _x;
        return true;
    }

    //
    vector<char> vec_quo;
    int k = 0;
    int m = 0;
    wiz::Array<char> itemp;
    wiz::Array<char> val;

    while( true ) {
        if( m >= _x.size() ) { break; }
        const wiz::Array<char> temp_concat = _int_concat( itemp, _int_sub( _x, m, k ) );
        if( temp_concat < _y )
        {
            vec_quo.push_back( to_char( 0 ) );
            //cout << "chk1" << endl;
            k++;
            if( IsSameValues( zero_int, temp_concat ) )
            {
               // cout << "chk.." << endl;
                m++; k = m;
                itemp.DoEmpty();
            }
            else if( k == _x.size() ){
                itemp.DoEmpty();
                itemp = temp_concat; // remove_first_zeros( temp_concat );
                        /// chk 2014.01.07
                break;
            }
        }
        else
        {
            for( int j=1; j <= 9; j++ ){
                val.DoEmpty();
                val = _int_minus( temp_concat, _int_multiple( _y, to_char( j ) ) );

                if( val < _y ){
                   // cout << "chk2 " << j << endl;
                    vec_quo.push_back( to_char( j ) );//
                    itemp.DoEmpty();
                    itemp = remove_first_zeros( val );
                    break;
                }
            }
            m = k+1;
            k = m;
        }
    }

    // return
     quotient.DoEmpty(); quotient = remove_first_zeros( to_Array( vec_quo ) );
     if( quotient.empty() ) { quotient = zero_int; }
     remainder.DoEmpty();
     if( itemp.empty() ) { remainder = zero_int; }
     else remainder = itemp;
     return true;
}

class CLongInteger /// class : long integer...
{
public:
    wiz::Array<char> val; /// absolute value!
    bool sign; /// +(true), -(false)
public:
    CLongInteger()
    {
    }
    CLongInteger( const wiz::Array<char>& val, const bool& sign )
    : val(val), sign(sign)
    {
    }
    CLongInteger& operator=( const CLongInteger& longInt )
    {
        CLongInteger temp = longInt;
        val.DoEmpty();
        val = temp.val;
        sign = temp.sign;

        return *this;
    }
public:
    //  <
    // ==

    friend CLongInteger operator+( const CLongInteger& num1, const CLongInteger& num2 )
    {
        if( num1.val.empty() || num2.val.empty() )
        {
            throw wiz::Error( __FILE__,__LINE__, "empty problem, in + " );
        }
        CLongInteger number;

        if( num1.sign == num2.sign ){
            number.val = _int_plus( num1.val, num2.val );
            number.sign = num1.sign;
            if( IsSameValues( zero_int, number.val ) ) { number.sign = true; }// think only +0.
        }
        else if( num1.sign && !num2.sign ){
            if( num1.val < num2.val )
            {
                number.val = _int_minus( num2.val, num1.val );
                number.sign = false; // minus
            }
            else if( IsSameValues( num1.val, num2.val ) )
            {
                number.val = wiz::Array<char>( 1, to_char( 0 ) );
                number.sign = true; // +0 = 0 = -0 ??
            }
            else
            {
                number.val = _int_minus( num1.val, num2.val );
                number.sign = true;
            }
        }
        else if( !num1.sign && num2.sign ){
            if( num1.val < num2.val )
            {
                number.val = _int_minus( num2.val, num1.val );
                number.sign = true; // minus
            }
            else if( IsSameValues( num1.val, num2.val ) )
            {
                number.val = wiz::Array<char>( 1, to_char( 0 ) );
                number.sign = true; // +0 = 0 = -0 ??
            }
            else
            {
                number.val = _int_minus( num1.val, num2.val );
                number.sign = false;
            }
        }
        return number;
    }
    friend CLongInteger operator-( const CLongInteger& num1, const CLongInteger& num2 )
    {
        if( num1.val.empty() || num2.val.empty() )
        {
            throw wiz::Error( __FILE__,__LINE__, "empty problem, in - " );
        }
        CLongInteger number = num2;
        number.sign = !number.sign;

        return num1 + number;
    }
    friend CLongInteger operator*( const CLongInteger& num1, const CLongInteger& num2 )
    {
        if( num1.val.empty() || num2.val.empty() )
        {
            throw wiz::Error( __FILE__,__LINE__, "empty problem, in * " );
        }
        CLongInteger temp;
        temp.val = _int_multiple( num1.val, num2.val );
        temp.sign = ( num1.sign == num2.sign );

        return temp;
    }
    friend CLongInteger operator/( const CLongInteger& num1, const CLongInteger& num2 )
    {
        if( num1.val.empty() || num2.val.empty() )
        {
            throw wiz::Error( __FILE__,__LINE__, "empty problem, in / " );
        }
        CLongInteger temp;
        wiz::Array<char> quotient;
        wiz::Array<char> remainder;

        _int_divide( num1.val, num2.val, quotient, remainder );

        temp.val = quotient;
        temp.sign = ( num1.sign == num2.sign ); /// chk...

        return temp;
    }
    friend CLongInteger operator%( const CLongInteger& num1, const CLongInteger& num2 )
    {
        if( num1.val.empty() || num2.val.empty() )
        {
            throw wiz::Error( __FILE__,__LINE__, "empty problem, in % " );
        }
        if( !num1.sign || !num2.sign ) /// chk...
        {
            throw wiz::Error( __FILE__, __LINE__, "CLongInteger%, sign is minus" );
        }

        CLongInteger temp;
        wiz::Array<char> quotient;
        wiz::Array<char> remainder;

        _int_divide( num1.val, num2.val, quotient, remainder );

        temp.val = remainder;
        temp.sign = true;


        return temp;
    }

};

}

#endif // CLONG_INTEGER_H_INCLUDED
