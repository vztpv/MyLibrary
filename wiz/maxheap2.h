
/// for dijkstra algorithmm, and etc!!
/// default - minHeap!!
/// rename header name.
#ifndef MAXHEAP2_H_INCLUDED
#define MAXHEAP2_H_INCLUDED

#include <wiz/global.h>
#include <wiz/wizardError.h>

namespace wiz {
    /// T has a "index", and T is pointer type!
template <class T, class COMP=PDSC<T>, class EE=PEE<T> > // comp a > b
class Heap2 {
private:
	Heap2( const Heap2<T, COMP, EE>& );
	Heap2<T, COMP, EE>& operator=( const Heap2<T, COMP, EE>& );
private:
    int N; // 전체 개수.
    int nowN;
    T* ptr;
public:
    explicit Heap2( const int _N = 1 ):N(_N),nowN(0){
        if( N < 1 ) { throw wiz::Error( " Heap2 _N is not valid " ); }
        // index 0는 사용하지 않으므로, 1~N, N+1개가 필요.
        ptr = new T[ N+1 ]; // 0~N, 0은 사용하지 않는다. 아니면 전체 개수를 나타내는것도 괜찬을 듯?
        for(int i=0;i< N+1 ;i++) // RESET
            ptr[i] = T();
    }
    virtual ~Heap2(){
        if( ptr != NULL ) //
            delete[] ptr;  // 제거
    }
    int update( const int index ){
        if( index <= 0 || index > nowN ) { return index; }
        COMP comp;
        int i = index; const T item = ptr[index];
        while( ( i > 1 ) && comp( ptr[i>>1], item ) ){
            ptr[i] = ptr[i >> 1]; // i <= parent
            ptr[i]->index = i;
            i = i >> 1;
        }
        ptr[i] = item;
        ptr[i]->index = i;
///////////////////////
        return i;
    }
    int push( const T& item ){// return : pos of item in heap.
        if( isFull() ) {
            N = N * 2;
            T* temp = new T[N+1];
            for( int i=0; i <= nowN;i++ )
            {
                temp[i] = ptr[i];
            }
            delete[] ptr;
            ptr = temp;
        }
        int i = nowN + 1;
        COMP comp;
        while( ( i != 1 ) && comp( ptr[i >> 1], item ) ) {
            ptr[i] = ptr[i >> 1]; //
            ///
            ptr[i]->index = i;
            //
            i = i >> 1;
        }
        ptr[i] = item;
        ////////////////////
        ptr[i]->index = i;
        ////////////////////
        nowN++;

        return i; //
    }
    T pop(){
        if( isEmpty() ) { throw wiz::Error( " pop.. this heap is Empty " ); }
        int parent, child;
        T item, temp;
		COMP comp;
		EE ee;
        //
        item = ptr[1];
        //
        temp = ptr[nowN];
        ptr[nowN] = item; // 정렬을 위해서..
        nowN--;
        parent = 1;
        child = 2;
        //
        while( child <= nowN ){
            //
            if( child < nowN  && ( comp( ptr[child], ptr[child+1] ) ) ){
                child++;
            }

            if( comp( ptr[child], temp )
               || ee( ptr[child], temp ) ) break;
            //
            ptr[parent] = ptr[child];
            ///
            ptr[parent]->index = parent;
            //
            parent = child;
            child = child << 1;
        }
        //
        ptr[parent] = temp;
        /////////////////////////
        ptr[parent]->index = parent;
        /////////////////////////
        return item; //
    }
    //
    bool isFull()const{
        if( nowN >= N )
            return true;
        return false;
    }
    bool isEmpty()const{
        if( nowN == 0 )
            return true;
        return false;
    }
    bool empty()const{ return isEmpty(); }
    int getNMAX()const{ return N; } // 최대 개수..
    int getNowN()const { return nowN; } // 지금 개수.
    const T& operator[](int index)const{
        /// chk..
        return ptr[index];
    }
};

}
#endif // MAXHEAP_H_INCLUDED
