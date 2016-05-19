#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED


#include <wiz/wizardError.h>

namespace wiz{
namespace list{
    /// #define wiz_list_DEBUG ???
    template <class T>
    class Node {
    public:
        T val; // Data
        Node<T>* next;
    };
    template <class T>
    class FIRST{
    public:
        bool operator() ( Node<T>* n1, Node<T>* n2 ) const { return false; }
    };
    template <class T>
    class APPEND{
    public:
        bool operator() ( Node<T>* n1, Node<T>* n2 ) const { return true; }
    };
    template <class T>
    class DSC_EE {
    public:
        bool operator() ( Node<T>* t1,  Node<T>* t2) const { return t1 >= t2; }
    };
    template <class T>
    class ASC_EE {
    public:
        bool operator() ( Node<T>* t1,  Node<T>* t2) const { return t1 <= t2; }
    };
    template <class T, class COMP>
    class List{
    private:
        Node<T> head;
        COMP comp; /// chk??
    public:
        List( List<T,COMP>&& list )
        {
            head = list.head;
            list.head.next = NULL;
        }
        List<T,COMP>& operator=( List<T,COMP>&& list )
        {
            if( head.next == list.head.next ) { return *this; }
            wiz::Swap( head.next, list.head.next );
            return *this;
        }
        List( const List<T,COMP>& list )
        {
            initList( this );

            for( auto x=list.cbegin(); x != list.cend(); ++x )
            {
                insert( *x );
            }
        }
        List<T,COMP>& operator=( const List<T,COMP>& list )
        {
            List<T,COMP> temp = list;

            removeAll();

            for( auto x=temp.cbegin(); x != temp.cend(); ++x )
            {
                insert( *x );
            }

            return *this;
        }
    public:
        void removeAll()
        {
            Node<T>* node = head.next;
            Node<T>* node2;

            // doEmpty..
            while( node )
            {
                // backup
                node2 = node;
                // next
                node = node->next;
                // remove
                delete node2;
            }
            //
            head.next = NULL;
        }
    public:
        explicit List() { initList( this ); }
        ~List()
        {
            removeAll();
        }
        //
        bool empty()const{ return NULL == head.next; }
        void insert( const T& val )
        {
            insert( this, makeNode( val ) );
        }
        // cf) bool getVal( int ith, T* pval ) // ?
        // show,
        friend ostream& operator<<( ostream& stream, const List<T, COMP>& list )
        {
            Node<T>* pNode = list.head.next;


            while( pNode )
            {
                stream << pNode->val << "\n";
                pNode = pNode->next;
            }
            return stream;
        }
    private:
        void initNode( Node<T>* node, const T& val ){ node->val = val; node->next = NULL; }
        void initList( List<T,COMP>* list ) { initNode( &( list->head ), T() ); }
        Node<T>* makeNode( T val )
        {
            Node<T>* node = (Node<T>*) new Node<T>();


            if( NULL == node ) { return NULL; } // print err.


            initNode( node, val );


            return node;
        }
    // use Function Object and Template.// then can inline!
        void insert( List<T,COMP>* list, Node<T>* node )
        {
            Node<T>* pNode;


            pNode = &( list->head );


            // find good place
            while( NULL != (pNode->next) &&
                    comp( pNode->next, node )//( pNode->next->ival ) <= ( node->ival )  /// comp! cf) using function ptr!
                  )
            {
                pNode = pNode->next;
            }

            // insert
            node->next = pNode->next;
            pNode->next = node;
        }
    public: /// To Do - make iterator, begin, end...
        class Iter //ator
        {
        private:
            Node<T>* node;//
        public:
            explicit Iter( Node<T>* node = NULL ) : node( node ) { }
            void right() {
                #ifdef wiz_list_DEBUG
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
            Node<T>* node;//
        public:
            explicit ConstIter( Node<T>* node = NULL ) : node( node ) { }
            void right() {
                #ifdef wiz_list_DEBUG
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
        Iter begin() { return Iter( head.next ); }
        Iter end() { return Iter(); }
        ConstIter cbegin() const { return ConstIter( head.next ); }
        ConstIter cend() const { return ConstIter(); }
    public:
        void remove( const T& x )
        {
            Node<T>* node = &head;

            while( node->next ){
                if( x == node->next->val ){
                    Node<T>* temp = node->next;
                    node->next = temp->next;
                    delete temp;
                    break;
                }
                node = node->next;
            }

        }
    };
}
}

#endif // LIST_H_INCLUDED
