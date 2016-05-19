#ifndef GETKTHDATA_H_INCLUDED
#define GETKTHDATA_H_INCLUDED

#include <wiz/random.h>
#include <wiz/wizardError.h>

/// GKD_DEBUG

namespace wiz{

template <class T, class COMP=ASC<T> >
class GetKthData
{
private:
    static int random( const int low, const int high ){
        return Rand2<int>( low, high );
    }
    static void exchange( T& a, T& b ){
        wiz::Swap( a, b );
    }
    static void partition( wizArray<T>& arr, const int low, const int high, int& pivotpoint ){
        int i = low + 1, j = low; //
        register const T pivotitem = (arr)[low];
        COMP comp;
        for(; i <= high;i++ ){
            ///if( (array)[i] < pivotitem ){
            if( comp( arr[i], pivotitem ) ) {
                j++;
                exchange( (arr)[i] , (arr)[j] );
            }
        }
        //
        pivotpoint = j;
        exchange( (arr)[low], (arr)[pivotpoint] );
    }
    static void rand_partition( wizArray<T>& arr, const int low, const int high, int& pivotpoint ){
        const int i = random( low, high );
        exchange( (arr)[low], (arr)[i] );
        partition( arr, low, high, pivotpoint );
    }
private:
    static T get( INOUT wizArray<T>& arr, const int low, const int high, const int k, const int base ) /// k : 1 ~ high-low+1
    {
        int pivot = -1;

        rand_partition( arr, low, high, pivot );

        if( pivot < (base+k-1) )
        {
            return get( arr, pivot+1, high, k, base );
        }
        else if( pivot == (base+k-1) )
        {
            return arr[pivot];
        }
        else
        {
            return get( arr, low, pivot-1, k, base );
        }
    }
public:
    static T get( INOUT wizArray<T>& arr, const int low, const int high, const int k )
    {
        wiz::checkIndexError( low, arr.size() );
        wiz::checkIndexError( high, arr.size() );
        if( low > high ) { throw wiz::Error( __FILE__, __LINE__, " low > high " ); }
        if( k < 1  || k > (high-low+1) ) { throw wiz::Error( __FILE__, __LINE__, " k < 1 || k > ( high - low + 1 ) " ); }
        return get( arr, low, high, k, low );
    }
    static T get( INOUT wizArray<T>& arr, const int k )
    {
        return get( arr, 0, arr.size()-1, k );
    }
};

}
#endif // GETKTHDATA_H_INCLUDED
