#ifndef INSERTSORT_H_INCLUDED
#define INSERTSORT_H_INCLUDED

/// add std::move??
#include <wiz/global.h>

namespace wiz{

template <class T, class COMP = ASC<T> >
class InsertSort
{
private:
   // static COMP comp;
public:
    static void sort( wizArray<T>& array ) { sort( array, 0, array.length()-1 ); }
    static void sort( wizArray<T>& array, const int start, const int last );
};

template <class T, class COMP >
void InsertSort<T,COMP>::sort( wizArray<T>& array, const int start, const int last )
{
	COMP comp;
    for( int pick=start+1; pick <= last; pick++ )
    {
        int current = pick;
        T temp = std::move( array[pick] ); //
        for( ; ( current > start ) && comp( temp, array[current-1] ); // ( temp < array[current-1]  );
                current-- )
        {
            array[current] = std::move( array[current-1] );
        }
        array[current] = std::move( temp );
    }
}

}

#endif // INSERTSORT_H_INCLUDED
