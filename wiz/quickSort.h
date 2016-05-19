#ifndef QUICKSORT_H_INCLUDED
#define QUICKSORT_H_INCLUDED


#include <wiz/global.h>
#include <wiz/random.h>


namespace wiz{
template < class T, class COMP = ASC<T>, class SWAP = WIZ_SWAP<T> >
class QuickSort{
private:
    static void exchange( T& a, T& b ){
        SWAP()( a, b );
    }
    static void partition( wizArray<T>& arr, const int low, const int high, int& pivotpoint ){
        int i = low + 1, j = low; //
        register const T pivotitem = (arr)[low];
        COMP comp;
		//
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
        const int i = wiz::Rand2<int>( low, high );
        exchange( (arr)[low], (arr)[i] );
        partition( arr, low, high, pivotpoint );
    }

    static void quicksort( wizArray<T>& arr, int low, int high ){
        int pivotpoint = -1; //for  error check.

        while( low < high ){
            rand_partition( arr, low, high, pivotpoint );//
            if( pivotpoint - low <= high - pivotpoint ){
                quicksort( arr, low, pivotpoint - 1 );
                low = pivotpoint + 1;
            }
            else{
                quicksort( arr, pivotpoint + 1, high );
                high = pivotpoint - 1;
            }
        }
    }
private:
    QuickSort();
    QuickSort( const QuickSort<T,COMP,SWAP>& p );
    QuickSort<T,COMP,SWAP>& operator=( const QuickSort<T,COMP,SWAP>& p );
public:
    static void sort( wizArray<T>& arr, int low, int high ) { /// chk...
        quicksort( arr, low, high );
    }
    static void sort( wizArray<T>& arr )
    {
        quicksort( arr, 0, arr.size()-1 );
    }
};

}
#endif // QUICKSORT_H_INCLUDED
