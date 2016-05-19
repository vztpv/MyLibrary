#ifndef RED_BLACK_TREE2_H_INCLUDED
#define RED_BLACK_TREE2_H_INCLUDED


#include <wiz/global.h>
#include <wiz/newArrays.h>

/// namespace wiz_rbt??
namespace wiz {
namespace rbt2 { // wiz_rbt2??

typedef enum _Color{ RED=0, BLACK=1 } Color;

template < class T >
class RB_Node
{
public:
    T key;
    RB_Node<T>* left;
    RB_Node<T>* right;
    RB_Node<T>* p; // paraent
    Color color;
public:
    explicit RB_Node( const T& key = T() ) : key( key ), left( NULL ), right( NULL ), p( NULL ), color( BLACK ) { }
};
template < class T >
class NIL
{
private:
    static RB_Node<T> nil;
public:
    static const RB_Node<T>* Get() {
        return &nil;
    }
};
template < class T >
RB_Node<T> NIL<T>::nil;

template < class T, class COMP=ASC<T>, class NOT_EE=NOT_EE<T> >
class RB_Tree
{
private:
    RB_Node<T>* root;
    int count;
public:
    explicit RB_Tree() : count(0) { root = (RB_Node<T>*)NIL<T>::Get(); }
    ~RB_Tree() {
        if( root != NIL<T>::Get() ) {
            RemoveAll( root );
        }
    }
    RB_Tree<T, COMP, NOT_EE>& operator=( const RB_Tree<T, COMP, NOT_EE>& rbt )
    {
        if( this->root != rbt.root )
        {
            RemoveAll( this->root );

            // init this.
            this->root = const_cast<RB_Node<T>*>( NIL<T>::Get() );
            this->root = Copy( rbt.root );
            this->count = rbt.count;
        }
        return *this;
    }
	RB_Tree( const RB_Tree<T, COMP, NOT_EE>& tree )
	{
        this->root = const_cast<RB_Node<T>*>( NIL<T>::Get() );
        this->count = 0;
	    *this = tree;
	}
private:
    RB_Node<T>* Copy( RB_Node<T>* node )
    {
        if( node == NIL<T>::Get() ) { return const_cast<RB_Node<T>*>( NIL<T>::Get() ); }
        RB_Node<T>* temp = new RB_Node<T>( *node );

        temp->left = Copy( node->left );
        temp->right = Copy( node->right );

        return temp;
    }
    void RemoveAll( RB_Node<T>* node )
    {
        // exit condition.
        if( node == NIL<T>::Get() ) { return; }
        RB_Node<T>* left = node->left;
        RB_Node<T>* right = node->right;
        delete node;
        RemoveAll( left );
        RemoveAll( right );
    }
private:// LEFT ROTATE Tree x
    void LEFT_ROTATE( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* x ) {
        //if( x !=NIL<T>::Get() || tree->root != NIL<T>::Get() ) { return; }
        RB_Node<T>* y;
        // y = right[x]
        y = x->right;
        // right[x] = left[y]
        x->right = y->left;
        // if(left[y] != nil[T]
        if( y->left != NIL<T>::Get() ) {
        //    then p[left[y]] = x
            y->left->p = x;
        }
        // p[y] = p[x]
        y->p = x->p;
        // if( p[x] = nil[T]
        if( x->p == NIL<T>::Get() ) {
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
    void RIGHT_ROTATE( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* x ) {
        //if( x !=NIL<T>::Get() || tree->root != NIL<T>::Get() ) { return; }

        RB_Node<T>* y;
        // y = right[x]
        y = x->left;
        // right[x] = left[y]
        x->left = y->right;
        // if(left[y] != nil[T]
        if( y->right != NIL<T>::Get() ) {
        //    then p[left[y]] = x
            y->right->p = x;
        }
        // p[y] = p[x]
        y->p = x->p;
        // if( p[x] = nil[T]
        if( x->p == NIL<T>::Get() ) {
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
    RB_Node<T>* SEARCH( RB_Node<T>* x,const T& k ) const
    {
        COMP comp;

        while( x != NIL<T>::Get() && NOT_EE() ( k, x->key ) ) { // != nil
            if( comp( k, x->key ) ) { // k < x->key
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return x;
    }
    RB_Node<T>* MINIMUM( RB_Node<T>* x )
    {
        while( x->left != NIL<T>::Get() ) { // != nil
            x = x->left;
        }
        return x;
    }
    RB_Node<T>* SUCCESSOR( RB_Node<T>* x )
    {
        if( x->right != NIL<T>::Get() ) // nil
            return MINIMUM( x->right );
        RB_Node<T>* y = x->p;
        while( y != NIL<T>::Get() && x == y->right )
        {
            x = y; y = y->p;
        }
        return y;
    }
    void INSERT_FIXUP( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* z ) /// google ..
    {
        RB_Node<T>* y;
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
    void INSERT( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* z )
    {
        COMP comp;
        // y= nil[T}
        RB_Node<T>* y = (RB_Node<T>*)NIL<T>::Get();
        RB_Node<T>* x = tree->root;
        while( x != NIL<T>::Get() )
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
        if( y == NIL<T>::Get() ) {
            tree->root = z;
        } else if( comp( z->key, y->key ) ) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = (RB_Node<T>*)NIL<T>::Get(); // = nil
        z->right = (RB_Node<T>*)NIL<T>::Get();
        z->color = RED; // = RED
        // insert fixup
        INSERT_FIXUP( tree, z );
    }
    void InorderTraversal( ostream& stream, const RB_Node<T>* x ) const
    {
        // exit condition.
        if( x == NIL<T>::Get() ) { return; }
        InorderTraversal( stream, x->left );
        stream << x->key << "\n";
        InorderTraversal( stream, x->right );
    }

    void PreorderTraversal( ostream& stream, const RB_Node<T>* x )const
    {
        if( x == NIL<T>::Get() ) { return; }
        stream << x->key << "\n";
        PreorderTraversal( stream, x->left );
        PreorderTraversal( stream, x->right );
    }
    void InorderTraversal( Array<T>& arr, int& count, RB_Node<T>* node ) const
    {
        // exit condition..
        if( node == NIL<T>::Get() ) { return; }
        InorderTraversal( arr, count, node->left );
        arr[count] = node->key;
		count++;
		InorderTraversal( arr, count, node->right );
    }
    void PreorderTraversal( Array<T>& arr, int& count, RB_Node<T>* node ) const
    { /// chk.. 2013.07.26
        // exit condition..
        if( node == NIL<T>::Get() ) { return; }

        arr[count] = node->key;
		count++;
        PreorderTraversal( arr, count, node->left );
		PreorderTraversal( arr, count, node->right );
    }

    void REMOVE_FIXUP( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* x )
    {
        RB_Node<T>* w;
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


	RB_Node<T>* REMOVE( RB_Tree<T, COMP, NOT_EE>* tree, RB_Node<T>* z )
	{
        RB_Node<T>* x;
        RB_Node<T>* y;

        if( z->left == NIL<T>::Get()
           || z->right == NIL<T>::Get() ) {
            y = z;
        } else { y = SUCCESSOR(z); }

        if( y->left != NIL<T>::Get() ) {
            x = y->left;
        } else { x = y->right; }

        x->p = y->p;

        if( y->p == NIL<T>::Get() ) {
            root = x;
        } else if( y == y->p->left ) {
            y->p->left = x;
        } else { y->p->right = x; }

        if( y != z ) {
            z->key = y->key;
        }
        if( y->color == BLACK ) {
            REMOVE_FIXUP( tree, x );
        }
        return y;
    }

public:
    // insert, search, remove.
    void Insert( const T& key )
    {
        if( !IsExist( key ) ) {
            INSERT( this, new RB_Node<T>( key ) );
            count++;
        }
    }
    bool IsExist( const T& key )
    {
        // NULL != ~
        return NIL<T>::Get() != SEARCH( root, key );
    }
    T Find( const T& key )
    {
        RB_Node<T>* temp = SEARCH( root, key );
        /// if temp != NIL<T>::Get() -> err.
        return temp->key;
    }
    void Remove( const T& key )
    {
        RB_Node<T>* node = SEARCH( root, key );
        if( node != NIL<T>::Get() )
        {
            RB_Node<T>* temp = REMOVE( this, node );
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
    bool IsEmpty() const
    {
        return root == NIL<T>::Get();
    }
    bool empty() const { return IsEmpty(); }
    int GetCount() const
    {
        return count;
    }
    int size() const { return count; }
};
}
}

#endif // RED_BLACK_TREE_H_INCLUDED

