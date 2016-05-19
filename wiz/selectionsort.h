#ifndef SELECTIONSORT_H_INCLUDED
#define SELECTIONSORT_H_INCLUDED

// 2013.08.29 - not stable...
#include <wiz/global.h>

namespace wiz{
    template < class T, class COMP = ASC<T> >
    class SelectionSort
    {
        //static COMP comp;
    public:
        static void sort( wizArray<T>& arr ) { sort( arr, 0, arr.size()-1 ); }
        static void sort( wizArray<T>& arr, int start, int last ); /// chk...
    };


    template < class T, class COMP >
    void SelectionSort<T, COMP>::sort( wizArray<T>& arr, int start, int last )
    {
        if( arr.empty() || ( start >= last ) ) { return; }
        COMP comp;
        for( int i=last; i >=start+1; i-- )
        {
            // find maximum
            T max = arr[start];
            int index = start;
            for( int j=start+1; j <= i; j++ )
            {
                // if( max < arr[j] ) { max = arr[j]; index = j; }
                if( comp( max, arr[j] ) ) { max = arr[j]; index = j; } /// chk...
            }
            Swap( arr[i], arr[index] );
        }

    }
}
#endif // SELECTIONSORT_H_INCLUDED
