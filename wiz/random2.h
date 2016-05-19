#ifndef RANDOM2_H_INCLUDED
#define RANDOM2_H_INCLUDED

#include <wiz/random.h>
#include <wiz/newArrays.h>

namespace wiz {
    class Random2
    {
    private:
        Array<int> arr;
        int N;
    public:
        explicit Random2( const int n ) : N ( n )
        {
            arr = Array<int>( n );
            for(int i=0; i< n; i++ ) {
                arr[i] = i;
            }
        }
        explicit Random2( const Array<int>& arr2 )
        {
            arr = arr2;
            N = arr2.size();
        }
        int Rand() {
            if( End() ) { throw wiz::Error( __FILE__, __LINE__, " Rand ended" ); }
            const int index = wiz::Rand2<int>()%N;
            int ret = arr[index];
            ///wiz::Swap( arr[index], arr[N-1] );
            arr[index] = arr[N-1];
            N--;
            return ret;
        }
        bool End() const {
            return 0 >= N;
        }

    };
}

#endif // RANDOM2_H_INCLUDED
