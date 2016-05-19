#ifndef MAXHEAP_H_INCLUDED
#define MAXHEAP_H_INCLUDED

/// rename BinaryHeap? - !! maxHeap-ASC<T>, minHeap-DSC<T> !!

/// HEAP_DEBUG

#include <wiz/global.h>
#include <wiz/wizardError.h>

namespace wiz {

template <class T, class COMP=ASC<T>, class EE=EE<T> > // a < b
class Heap{
public:
    ///chk...
	Heap( const Heap<T,COMP,EE>& heap )
	{
        N = heap.N;
        nowN = heap.nowN;
        ptr = new T[heap.N+1];
        for( int i=0; i <= heap.N; i++ )
        {
            ptr[i] = heap.ptr[i];
        }
	}
	Heap<T,COMP,EE>& operator=( const Heap<T,COMP,EE>& heap )
	{
	    if( ptr == heap.ptr ) { return *this; }
	    delete[] ptr;

	    N = heap.N;
	    nowN = heap.nowN;
	    ptr = new T[heap.N + 1];
	    for( int i=0; i <= heap.N; i++ )
        {
            ptr[i] = heap.ptr[i];
        }
	    return *this;
	}
private:
    int N; // 전체 개수.
    int nowN;
    T* ptr;
public:
    explicit Heap( const int _N = 1 ) : N(_N), nowN(0) {
        if( N < 1 ) { throw wiz::Error( " Heap _N is not valid " ); }
        // index 0는 사용하지 않으므로, 1~N, N+1개가 필요.
        ptr = new T[ N+1 ]; // 0~N, 0은 사용하지 않는다. 아니면 전체 개수를 나타내는것도 괜찬을 듯?
        for(int i=0;i < N+1 ;i++) // RESET
        {
            ptr[i] = T();
        }
    }
    virtual ~Heap(){
        if( ptr != NULL ) //
            delete[] ptr;  // 제거
    }
    int push( const T& item ){// return : pos of item in heap.
        if( isFull() ) {
            N = N * 2;
            T* temp = new T[N+1];
            for( int i=0; i <= nowN;i++ )
            {
                temp[i] = std::move( ptr[i] );
            }
            delete[] ptr;
            ptr = temp;
        }
        int i = nowN + 1;
        COMP comp;
        while( ( i != 1 ) && comp( ptr[i>>1], item ) ){
            ptr[i] = std::move( ptr[i>>1] ); //
            i = i >> 1; // /2
        }
        ptr[i] = item;
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
        item = ( ptr[1] );
        //
        temp = std::move( ptr[nowN] );
        ptr[nowN] = item; // 정렬을 위해서..
        nowN--;
        parent = 1;
        child = 2;
        //
        while( child <= nowN ){
            if( child < nowN && comp( ptr[child], ptr[child+1] ) ){
                child++;
            }
            // temp >= heap[child]
            if( comp( ptr[child], temp )
               || ee( ptr[child], temp ) ) break;
            //
            ptr[parent] = std::move( ptr[child] );
            parent = child;
            child = child << 1; // *2
        }
        ptr[parent] = std::move( temp );
        return item; //
    }
    //
    bool isFull()const{
        return nowN >= N;
    }
    bool isEmpty()const{
        return 0 == nowN;
    }
    bool empty()const{ return isEmpty(); }
    int size()const{ return nowN; }
    int getNMAX()const{ return N; } // 최대 개수..
    int getNowN()const { return nowN; } // 지금 개수.
    const T& operator[](int index)const{
        /// chk..
        return ptr[index];
    }
    /// test
    void InorderTraversal( ostream& stream, int node_id )
    {
        if( node_id < 1 || node_id > nowN ) { return; }

        InorderTraversal( stream, node_id * 2 ); // left child
        stream << ptr[node_id] << " ";
        InorderTraversal( stream, node_id * 2 + 1 ); // right child
    }
    const T& top()const { return ptr[nowN-1]; }
    T& top() { return ptr[nowN-1]; }
};

}
#endif // MAXHEAP_H_INCLUDED
