#ifndef SELECTIONSORT2_H_INCLUDED
#define SELECTIONSORT2_H_INCLUDED

#include <wiz/global.h>

// 2013.08.29 - not stable -> stable?
namespace wiz{
template < class T, class COMP = ASC<T>, class EE = EE<T> >
class SelectionSort2
{
    //static COMP comp;
public:
    static void sort( wizArray<T>& arr ) { sort( arr, 0, arr.size()-1 ); }
    static void sort( wizArray<T>& arr, int start, int last );
};


template < class T, class COMP, class EE >
void SelectionSort2<T, COMP, EE>::sort( wizArray<T>& arr, int start, int last )
{
    if( arr.empty() || ( start >= last ) ) { return; }
    COMP comp;
    EE ee;
    wizArray<bool> chkTemp( last-start+1, true );
    wizArray<T> result( last-start+1 );

    for( int i=last; i >=start; i-- )
    {
        int first_index = start-1;
        {   // find max index(first)
            for(int i=start; i <= last; i++ )
                if( chkTemp[i-start] ) { first_index = i; break; }
        }
        // find maximum
        T max = arr[first_index];
        int index = first_index;
        for( int j=first_index+1; j <= last; j++ )
        {
            // if( max <= arr[j] ) { max = arr[j]; index = j; }
            if( chkTemp[j-start] && ( comp( max, arr[j] ) || ee( max, arr[j] ) ) ) { max = arr[j]; index = j; }
        }
        result[i-start] = max;
        chkTemp[index-start] = false;
    }
    for( int i=start; i <= last; i++ )
    {
        arr[i] = std::move( result[i-start] );
    }
}
}
#endif // SELECTIONSORT_H_INCLUDED

