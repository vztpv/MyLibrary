#ifndef RED_BLACK_TREE_H_INCLUDED
#define RED_BLACK_TREE_H_INCLUDED

#include <wiz/global.h>
#include <wiz/newArrays.h>
#include <wiz/wizardError.h>

/// namespace wiz_rbt??
namespace wiz {
namespace rbt {

typedef enum _Color{ RED=0, BLACK=1 } Color;

template < class T, class DATA >
class RB_Node
{
public:
    T key;
    DATA data;
    RB_Node<T, DATA>* left;
    RB_Node<T, DATA>* right;
    RB_Node<T, DATA>* p; // paraent
    Color color;
public:
    explicit RB_Node( T key = T(), DATA data = DATA() )
    : key( key ), data( data ), left( NULL ), right( NULL ), p( NULL ), color( BLACK ) { }
};
template < class T, class DATA >
class NIL
{
private:
    static RB_Node<T,DATA> nil;
public:
    static const RB_Node<T,DATA>* Get() {
        return &nil;
    }
};
template < class T, class DATA >
RB_Node<T,DATA> NIL<T,DATA>::nil;

template < class T, class DATA, class COMP=ASC<T>, class NOT_EE=NOT_EE<T> >
class RB_Tree
{
private:
    RB_Node<T,DATA>* root;
    int count;
public:
    explicit RB_Tree() : count(0) { root = (RB_Node<T,DATA>*)NIL<T,DATA>::Get(); }
    ~RB_Tree() {
        if( root != NIL<T,DATA>::Get() ) {
            RemoveAll( root );
        }
    }
    RB_Tree<T,DATA,COMP,NOT_EE>& operator=( const RB_Tree<T,DATA,COMP,NOT_EE>& rbt )
    {
        if( this->root != rbt.root )
        {
            RemoveAll( this->root );

            // init this.
            this->root = const_cast<RB_Node<T,DATA>*>( NIL<T,DATA>::Get() );
            this->root = Copy( rbt.root );
            this->count = rbt.count;
        }
        return *this;
    }
	RB_Tree( const RB_Tree<T,DATA,COMP,NOT_EE>& tree )
	{
	    this->root = const_cast<RB_Node<T,DATA>*>( NIL<T,DATA>::Get() );
        this->count = 0;
	    *this = tree;
	}
private:
    RB_Node<T,DATA>* Copy( RB_Node<T,DATA>* node )
    {
        if( node == NIL<T,DATA>::Get() ) { return const_cast<RB_Node<T,DATA>*>( NIL<T,DATA>::Get() ); }
        RB_Node<T,DATA>* temp = new RB_Node<T,DATA>( *node );

        temp->left = Copy( node->left );
        temp->right = Copy( node->right );

        return temp;
    }
    void RemoveAll( RB_Node<T,DATA>* node )
    {
        // exit condition.
        if( node == NIL<T,DATA>::Get() ) { return; }
        RB_Node<T,DATA>* left = node->left;
        RB_Node<T,DATA>* right = node->right;
        delete node;
        RemoveAll( left );
        RemoveAll( right );
    }
private:// LEFT ROTATE Tree x
    void LEFT_ROTATE( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* x ) {
        //if( x !=NIL<T,DATA>::Get() || tree->root != NIL<T,DATA>::Get() ) { return; }
        RB_Node<T,DATA>* y;
        // y = right[x]
        y = x->right;
        // right[x] = left[y]
        x->right = y->left;
        // if(left[y] != nil[T]
        if( y->left != NIL<T,DATA>::Get() ) {
        //    then p[left[y]] = x
            y->left->p = x;
        }
        // p[y] = p[x]
        y->p = x->p;
        // if( p[x] = nil[T]
        if( x->p == NIL<T,DATA>::Get() ) {
        //  then root[T] = y
            tree->root = y;
        }
        //  else if x = left[p[x]]
        else if( x == x->p->left ) {
        //      then left[p[x]] = y
            x->p->left = y;
        }
        //  else right[p[x]] = y
        else { x->p->right = y; }
        // left[y] = x
        y->left = x;
        // p[x] = y
        x->p = y;
    }
    // Right ROTATE Tree x // left <> right
    void RIGHT_ROTATE( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* x ) {
        //if( x !=NIL<T,DATA>::Get() || tree->root != NIL<T,DATA>::Get() ) { return; }

        RB_Node<T,DATA>* y;
        // y = right[x]
        y = x->left;
        // right[x] = left[y]
        x->left = y->right;
        // if(left[y] != nil[T]
        if( y->right != NIL<T,DATA>::Get() ) {
        //    then p[left[y]] = x
            y->right->p = x;
        }
        // p[y] = p[x]
        y->p = x->p;
        // if( p[x] = nil[T]
        if( x->p == NIL<T,DATA>::Get() ) {
        //  then root[T] = y
            tree->root = y;
        }
        //  else if x = left[p[x]]
        else if( x == x->p->right ) {
        //      then left[p[x]] = y
            x->p->right = y;
        }
        //  else right[p[x]] = y
        else { x->p->left = y; }
        // left[y] = x
        y->right = x;
        // p[x] = y
        x->p = y;
    }
    RB_Node<T,DATA>* SEARCH( RB_Node<T,DATA>* x, const T& k ) const
    {
        COMP comp;

        while( x != NIL<T,DATA>::Get() && NOT_EE() ( k, x->key ) ) { // != nil
            if( comp( k, x->key ) ) { // k < x->key
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return x;
    }
    RB_Node<T,DATA>* MINIMUM( RB_Node<T,DATA>* x )
    {
        while( x->left != NIL<T,DATA>::Get() ) { // != nil
            x = x->left;
        }
        return x;
    }
    RB_Node<T,DATA>* SUCCESSOR( RB_Node<T,DATA>* x )
    {
        if( x->right != NIL<T,DATA>::Get() ) // nil
            return MINIMUM( x->right );
        RB_Node<T,DATA>* y = x->p;
        while( y != NIL<T,DATA>::Get() && x == y->right )
        {
            x = y; y = y->p;
        }
        return y;
    }
    void INSERT_FIXUP( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* z ) /// google ..
    {
        RB_Node<T,DATA>* y;
        while( z->p->color == RED ) {
            if( z->p == z->p->p->left ) {
                y = z->p->p->right;
                if( y->color == RED ) {
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else
                {
                    if( z == z->p->right ) {
                        z = z->p;
                        LEFT_ROTATE( tree, z );
                    }
                    z->p->color = BLACK;
                    z->p->p->color = RED;
                    RIGHT_ROTATE( tree, z->p->p );
                }
            } else {
                y = z->p->p->left;
                if( y->color == RED ) {
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else {
                    if( z == z->p->left ) {
                        z = z->p;
                        RIGHT_ROTATE( tree, z );
                    }
                    z->p->color = BLACK;
                    z->p->p->color = RED;
                    LEFT_ROTATE( tree, z->p->p );
                }
            }
        }
        tree->root->color = BLACK;
    }
    void INSERT( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* z )
    {
        COMP comp;
        // y= nil[T}
        RB_Node<T,DATA>* y = (RB_Node<T,DATA>*)NIL<T,DATA>::Get();
        RB_Node<T,DATA>* x = tree->root;
        while( x != NIL<T,DATA>::Get() )
        {
            y = x;
            // if( z->key < x->key )
            if( comp( z->key, x->key ) )
            {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        z->p = y;
        if( y == NIL<T,DATA>::Get() ) {
            tree->root = z;
        } else if( comp( z->key, y->key ) ) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = (RB_Node<T,DATA>*)NIL<T,DATA>::Get(); // = nil
        z->right = (RB_Node<T,DATA>*)NIL<T,DATA>::Get();
        z->color = RED; // = RED
        // insert fixup
        INSERT_FIXUP( tree, z );
    }
    void InorderTraversal( ostream& stream, RB_Node<T,DATA>* x ) const
    {
        // exit condition.
        if( x == NIL<T,DATA>::Get() ) { return; }
        InorderTraversal( stream, x->left );
        stream << x->key << " " << x->data << "\n";
        InorderTraversal( stream, x->right );
    }

    void PreorderTraversal( ostream& stream, RB_Node<T,DATA>* x )const
    {
        if( x == NIL<T,DATA>::Get() ) { return; }
        stream << x->key << " " << x->data << "\n";
        PreorderTraversal( stream, x->left );
        PreorderTraversal( stream, x->right );
    }

    void REMOVE_FIXUP( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* x )
    {
        RB_Node<T,DATA>* w;
        while( x != root && x->color == BLACK )
        {
            if( x == x->p->left ) {
                w = x->p->right;
                if( w->color == RED ) {
                    w->color = BLACK;
                    x->p->color = RED;
                    LEFT_ROTATE( tree, x->p );
                    w = x->p->right;
                }
                if( w->left->color == BLACK && w->right->color == BLACK ) {
                    w->color = RED;
                    x = x->p;
                } else {
                    if( w->right->color == BLACK ) {
                        w->left->color = BLACK;
                        w->color = RED;
                        RIGHT_ROTATE( tree, w );
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = BLACK;
                    w->right->color = BLACK;
                    LEFT_ROTATE( tree, x->p );
                    x = root;
                }
            } else {
                w = x->p->left;
                if( w->color == RED ) {
                    w->color = BLACK;
                    x->p->color = RED;
                    RIGHT_ROTATE( tree, x->p );
                    w = x->p->left;
                }
                if( w->left->color == BLACK && w->right->color == BLACK ) {
                    w->color = RED;
                    x = x->p;
                } else {
                    if( w->left->color == BLACK ) {
                        w->right->color = BLACK;
                        w->color = RED;
                        LEFT_ROTATE( tree, w );
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = BLACK;
                    w->left->color = BLACK;
                    RIGHT_ROTATE( tree, x->p );
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }


	RB_Node<T,DATA>* REMOVE( RB_Tree<T,DATA,COMP,NOT_EE>* tree, RB_Node<T,DATA>* z )
	{
        RB_Node<T,DATA>* x;
        RB_Node<T,DATA>* y;

        if( z->left == NIL<T,DATA>::Get()
           || z->right == NIL<T,DATA>::Get() ) {
            y = z;
        } else { y = SUCCESSOR(z); }

        if( y->left != NIL<T,DATA>::Get() ) {
            x = y->left;
        } else { x = y->right; }

        x->p = y->p;

        if( y->p == NIL<T,DATA>::Get() ) {
            root = x;
        } else if( y == y->p->left ) {
            y->p->left = x;
        } else { y->p->right = x; }

        if( y != z ) { /// chk!
            z->key = y->key;
            z->data = y->data;
        }
        if( y->color == BLACK ) {
            REMOVE_FIXUP( tree, x );
        }
        return y;
    }
    void InorderTraversal( Array<T>& arr, int& count, RB_Node<T,DATA>* node ) const
    {
        // exit condition..
        if( node == NIL<T,DATA>::Get() ) { return; }
        InorderTraversal( arr, count, node->left );
        arr[count] = node->key;
		count++;
		InorderTraversal( arr, count, node->right );
    }
    void PreorderTraversal( Array<T>& arr, int& count, RB_Node<T,DATA>* node ) const
    { /// chk.. 2013.07.26
        // exit condition..
        if( node == NIL<T,DATA>::Get() ) { return; }

        arr[count] = node->key;
		count++;
        PreorderTraversal( arr, count, node->left );
		PreorderTraversal( arr, count, node->right );
    }
    /// To Do - traversal - Array<DATA>, and traversalKey, traversalData, traversalKeyAndData

public:
    bool empty()const { return root == NIL<T,DATA>::Get(); }
    // insert, search, remove.
    void Insert( const T& key, const DATA& data ) // return false, true?? - 20014.01.13
    {
        if( !IsExist( key ) ) {
            INSERT( this, new RB_Node<T,DATA>( key, data ) );
            count++;
        }
    }
    bool IsExist( const T& key )
    {
        // NULL != ~
        return NIL<T,DATA>::Get() != SEARCH( root, key );
    }
    T Find( const T& key ) /// for wiz smart pointer
    {
        RB_Node<T,DATA>* temp = SEARCH( root, key );
        /// if temp != NIL<T>::Get() -> err.
        return temp->key;
    }
    DATA& Search( const T& key ) // bool ~ key, DATA& ??
    {
        RB_Node<T,DATA>* data = SEARCH( root, key );
        if( data == NIL<T,DATA>::Get() )
            throw wiz::Error( __FILE__, __LINE__, "Search - do not exist" );
        return data->data;
    }
    void ChangeData( const T& key, const DATA& data ) /// need test!s
    {
        Search( key ) = data;
    }
    const DATA& Search( const T& key ) const
    {
        RB_Node<T,DATA>* data = SEARCH( root, key );
        if( data == NIL<T,DATA>::Get() )
            throw wiz::Error( __FILE__, __LINE__, "Search - do not exist" );
        return data->data;
    }
    void Remove( const T& key )
    {
        RB_Node<T,DATA>* node = SEARCH( root, key );
        if( node != NIL<T,DATA>::Get() )
        {
            RB_Node<T,DATA>* temp = REMOVE( this, node );
            delete temp;
			count--;
        }
    }
    void InorderTraversal( ostream& stream ) const
    {
        InorderTraversal( stream, this->root );
    }
    void PreorderTraversal( ostream& stream ) const
    {
        PreorderTraversal( stream, this->root );
    }
    bool IsEmpty() const
    {
        return root == NIL<T,DATA>::Get();
    }
    int GetCount() const
    {
        return count;
    }
    int size()const{ return count; }

    Array<T> ToArrayUsingInorder() const
    {
        Array<T> arr( count );
        int _count = 0;

        InorderTraversal( arr, _count, root );

        return arr;
    }
    Array<T> ToArrayUsingPreorder() const
    {
        Array<T> arr( count );
        int _count = 0;

        PreorderTraversal( arr, _count, root );

        return arr;
    }


};


}
}

#endif // RED_BLACK_TREE_H_INCLUDED
