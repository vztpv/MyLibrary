#ifndef LIST2_H_INCLUDED
#define LIST2_H_INCLUDED

#include <wiz/wizardError.h>
#include <wiz/newArrays.h>

namespace wiz {
template <class T>
class List2 {
private:
    class Node{
    public:
        T val;
        Node* next;
    public:
        Node( T _val = T() ) : val( _val ), next( NULL ) { }
    };
    class COMP_NODE
    {
    public:
        bool operator() ( Node* n1, Node* n2 ) const
        {
            return ( n1->val ) < ( n2->val );
        }
    };
private:
    Node* first;
    Node* rear;
    int count;
public:
    explicit List2() : count( 0 ){
        first = new Node();
        rear = first;
    }
    virtual ~List2() { RemoveTotal(); delete first; first = rear = NULL; }
    void Append( const T val )
    {
        rear->next = new Node( val );
        rear = rear->next;
        count++;
    }
    int Len()const { return count; }
    void RemoveTotal()
    {
        Node* nodeTemp = first->next;
        Node* nextTemp;
        while( nodeTemp != NULL ) {
            nextTemp = nodeTemp->next;
            delete nodeTemp;
            nodeTemp = nextTemp;
        }
        rear = first;
        first->next = NULL;
        count = 0;
    }
    void Sort()
    {
        const int len = count;
        if( len <= 1 ) { return; }
        Node* nodePtrTemp = first->next;
        wiz::Array< Node* > arr( len );


        // init array.
        for( int i=0; i < len; i++ ) {
            arr[i] = nodePtrTemp;
            nodePtrTemp = nodePtrTemp->next;
        }

        wiz::QuickSort< Node*, COMP_NODE >::sort( arr, 0, len-1 );

        //
        first->next = arr[0];
        rear = arr[0];

        for( int i=1; i < len; i++ ) {
            rear->next = arr[i];
            rear = arr[i];
        }
        rear->next = NULL; /// list end
    }
    class Iter //ator
    {
    private:
        Node* node;//
    public:
        explicit Iter( Node* node = NULL ) : node( node ) { }
        void right() {
            #ifdef wiz_list2_DEBUG /// rename!!
            if( !node ) { throw wiz::Error( __FILE__, __LINE__, " Iter err " ); }
            #endif
            node = node->next;
        }
        T data()const { return *node; }
        bool isNULL()const { return NULL == node; }
        bool operator!=( const Iter& x ) const
        {
            return x.node != this->node;
        }
        bool operator==( const Iter& x ) const
        {
            return x.node == this->node;
        }
        void operator++() { right(); }
        T& operator*() { return node->val; }
        T* operator->() { return &(node->val); }
        const T* operator->()const { return &(node->val); }
        const T& operator*()const { return node->val; }
    };
    class ConstIter //ator
    {
    private:
        Node* node;//
    public:
        explicit ConstIter( Node* node = NULL ) : node( node ) { }
        void right() {
            #ifdef wiz_list2_DEBUG
            if( !node ) { throw wiz::Error( __FILE__, __LINE__, " Iter err " ); }
            #endif
            node = node->next;
        }
        T data()const { return *node; }
        bool isNULL()const { return NULL == node; }
        bool operator!=( const ConstIter& x ) const
        {
            return x.node != this->node;
        }
        bool operator==( const ConstIter& x ) const
        {
            return x.node == this->node;
        }
        void operator++() { right(); }
        const T* operator->()const { return &(node->val); }
        const T& operator*()const { return node->val; }
    };
public:
    Iter begin() { return Iter( first->next ); }
    Iter end() { return Iter(); }
    ConstIter cbegin() const { return ConstIter( first->next ); }
    ConstIter cend() const { return ConstIter(); }

    friend ostream& operator<<( ostream& stream, const List2<T>& list )
    {
        Node* temp = list.first->next;
        while( temp != NULL )
        {
            stream << ( temp->val ) << "\n";
            temp = temp->next;
        }
        return stream;
    }
};

}
#endif // LIST2_H_INCLUDED
