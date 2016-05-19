#ifndef MERGESORT_H_INCLUDED
#define MERGESORT_H_INCLUDED

#include <wiz/global.h>
#include <wiz/InsertSort.h>
namespace wiz {

/// iteraotr
template < class T, class COMP=ASC<T>, class EE=EE<T> >
class MergeSort
{
private:
    // DO NOT
    MergeSort();
    MergeSort<T,COMP,EE>& operator=( const MergeSort<T,COMP,EE>& );
    MergeSort( const MergeSort<T,COMP,EE>& );
public:
    static void sort( wizArray<T>& arr ) { sort( arr, 0, arr.size()-1 ); }
    static void sort( wizArray<T>& arr, const int low, const int high )
    {
        if( low >= high ) { return; }
        wizArray<T> arrTemp( high - low + 1 );
        const int middle = ( low + high ) >> 1;
        EE ee; COMP comp;

        sort( arr, low, middle );
        sort( arr, middle+1, high );

        int count = 0;
        int i = low, j = middle+1;
        while( i <= middle && j <= high )
        {
            if( comp( arr[i], arr[j] )
               || ee( arr[i], arr[j] )
               ) /// stable?
            {
                arrTemp[count] = std::move( arr[i] );
                i++;
            }
            else
            {
                arrTemp[count] = std::move( arr[j] );
                j++; cout << low << " " << high << endl;
        cout << arr << endl;
            }
            count++;
        }
        for(; i <= middle; i++ ) { arrTemp[count] = std::move( arr[i] ); count++; }
        for(; j <= high; j++ ) { arrTemp[count] = std::move( arr[j] ); count++; }
        for( int k = low; k <= high; k++ ) { arr[k] = std::move( arrTemp[k-low] ); }
    }
    static void inplace_sort( wizArray<T>& arr )
    {
        inplace_sort( arr, 0, arr.size()-1 );
    }
    static void inplace_sort( wizArray<T>& arr, const int low, const int high )
    {
        COMP comp;
        EE ee;

        if( low >= high ) { return; }
        if( high - low + 1 == 2 )
        {
            if( comp( arr[high], arr[low] ) ) {
                std::swap( arr[low], arr[high] );
            }
            return;
        }

        int middle = ( ( low + high ) >> 1 );


        inplace_sort( arr, low, middle );
        inplace_sort( arr, middle+1, high );

     //   if( comp( arr[middle], arr[middle+1])
      //     || ee( arr[middle], arr[middle+1]) )
        {
      //      return;
        }

        int i = middle, j = middle+1;
        bool oneOver = false;

        //cout << "i " << i << " j " << j << endl;

        while( i >= low && j <= high )
        {
            if( comp( arr[j], arr[i] )
               //|| ee( arr[j], arr[i] )
               ) /// stable?
            {
                oneOver = true;
                --i; ++j;
            }
            else
            {
                break;
            }
        }

        if( oneOver ) {
            ++i; --j;
            int count = 0;
            //cout << "i " << i << " j " << j << endl;
            for( int start = i; start <= middle; ++start )
            {
                ++count;
                std::swap( arr[start], arr[middle+count]);
                 //   cout << "start "<< start << " middle+count" <<middle+count << endl;
                // cout << low << " " << high << endl;
                //    cout << arr << endl;
            }
        }

        ///
        inplace_sort( arr, low, middle );
        inplace_sort( arr, middle+1, high );

        static int k = 0;
        k++;
        cout << k <<"th " << endl;
        cout << low << " " << high << endl;
        cout << arr << endl;
    }
};

}
#endif // MERGESORT_H_INCLUDED
