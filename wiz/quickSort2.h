#ifndef QUICKSORT2_H_INCLUDED
#define QUICKSORT2_H_INCLUDED

#include <wiz/global.h>
#include <wiz/random.h>
#include <wiz/stacks.h>

namespace wiz{
    template <class T, class COMP = ASC<T>, class SWAP = WIZ_SWAP<T> > /// , SWAP=WIZ_SWAP<T>
    class QuickSort2{
    private:
        static COMP comp;
        class QSData
        {
        public:
            int low;
            int high;
            int pivotpoint;
            int op;
            bool valid; //
        public:
            QSData() : valid(false) { }
            QSData( int low, int high, int pivotpoint, int op )
            : low(low), high(high), pivotpoint(pivotpoint), op(op),
            valid( true )
            {
                //
            }
        };

        static int random( const int low, const int high ){
            return Rand<unsigned int>()%( high - low + 1 ) + low;
        }
        static void exchange( T& a, T& b ){
             SWAP()( a, b );
        }
        static void partition( Array<T>& array, const int low, const int high, int& pivotpoint ){
            int i = low + 1, j = low; //
            register const T pivotitem = (array)[low]; // Quick arrayort가 빠른 이유!!
            //
            for(; i <= high;i++ ){
                ///if( (array)[i] < pivotitem ){
                if( comp( array[i], pivotitem ) ) {
                    j++;
                    exchange( (array)[i] , (array)[j] );
                }
            }
            //
            pivotpoint = j;
            exchange( (array)[low], (array)[pivotpoint] );
            //
        }
        static void rand_partition( Array<T>& array, const int low, const int high, int& pivotpoint ){
            const int i = random( low, high );
            exchange( (array)[low], (array)[i] );
            partition( array, low, high, pivotpoint );
        }
        static void quicksort( Array<T>& array, const int low, const int high ){
            ArrayStack<QSData> arrayStack( 1 );
           // int count = 0;
            arrayStack.push( QSData( low, high, -1, 0 ) );
            QSData top = arrayStack.top(); // get top value!

            while( !arrayStack.empty() ) {
                while( top.low < top.high ){
                    rand_partition( array, top.low, top.high, top.pivotpoint );//

                    arrayStack.top() = top;

                    if( top.pivotpoint - top.low <= top.high - top.pivotpoint ){
                     //   count++;
                        arrayStack.push( QSData( top.low, top.pivotpoint-1, -1, 1 ) );
                    }
                    else{
                        arrayStack.push( QSData( top.pivotpoint+1, top.high, -1, 2 ) );
                     //   count++;
                    }

                    top = arrayStack.top();
                }

                // pop
                switch( arrayStack.pop().op )
                {
                case 1:
                    top = arrayStack.top();
                    top.low = top.pivotpoint + 1;
                    //arrayStack.top() = top;
                    break;
                case 2:
                    top = arrayStack.top();
                    top.high = top.pivotpoint - 1;
                    //arrayStack.top() = top;
                    break;
                }
            }
            //cout << "count " << count << endl;
        }
    private:
        /* 대입, 복사생성자 금지.. */
        QuickSort2( const QuickSort2& p );
        QuickSort2& operator=( const QuickSort2& p );
        QuickSort2();
    public:
        static void sort( Array<T>& _array, int low, int high ) { /// chk &
            quicksort( _array, low, high );
        }
        static void sort( Array<T>& _array ) {
            sort( _array, 0, _array.size()-1 );
        }
    };
}

#endif // QUICKSORT2_H_INCLUDED
