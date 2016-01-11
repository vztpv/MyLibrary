#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

/// #define BTREE_DEBUG /// B_PLUS_TREE_DEBUG ?
/// BTree<KEY,DATA,COMP,EE> /// COMP : key1 < key2, EE : key1 == key2
/// B+ Tree!
#include <vector>
#include <wiz/sArrays.h>

/// NOTE
/// fanout : num of child!
/// fanout >= 3
namespace wiz{
 // namespace b_plus
template <class KEY, class DATA, class COMP, class EE>
class B_Internal_Node;
template <class KEY, class DATA, class COMP, class EE>
class B_Terminal_Node;
template <class KEY, class DATA, class COMP, class EE>
class BTree;

template <class KEY, class DATA, class COMP, class EE>
class B_Node{ /// this is likely "Interface"..
public:
    virtual bool isInternal()const=0;
    virtual bool isTerminal()const=0;
    //
    virtual bool isRoot()const=0;
    virtual long getEfficiency()const=0;
    virtual void setParent( B_Internal_Node<KEY,DATA,COMP,EE>* in )=0;
    virtual B_Internal_Node<KEY,DATA,COMP,EE>* getParent()const=0; /// cf) const B_Internal_Node<KEY,DATA,COMP,EE>* ? getParent()
    virtual int getKeyNum()const=0;
    virtual int getDataNum()const=0;
    virtual ~B_Node() { } /// 2012.1.21..
};


template <class KEY, class DATA, class COMP, class EE>
class B_Terminal_Node : public B_Node<KEY,DATA,COMP,EE> {
public:
    friend class BTree<KEY,DATA,COMP,EE>;
private:
    B_Terminal_Node( const B_Terminal_Node& tn );
    B_Terminal_Node& operator=( const B_Terminal_Node& tn );
private:
    ExtendArray<KEY,DATA,COMP,EE>* arr; // new initially
    B_Internal_Node<KEY,DATA,COMP,EE>* parent; // NULL initially
    //int parentNo; // -1 initially
    int fanout;
    B_Terminal_Node<KEY,DATA,COMP,EE>* left;
    B_Terminal_Node<KEY,DATA,COMP,EE>* right;
public:
    void setLeft( B_Terminal_Node<KEY,DATA,COMP,EE>* left ){ this->left = left; }
    void setRight( B_Terminal_Node<KEY,DATA,COMP,EE>* right ){ this->right = right; }
    void setParent( B_Internal_Node<KEY,DATA,COMP,EE>* parent ){
        this->parent = parent;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* getParent()const
    {
        return parent;
    }
    //////////////////////////////////////////////////////////
    B_Terminal_Node<KEY,DATA,COMP,EE>* Left(){ return left; }
    B_Terminal_Node<KEY,DATA,COMP,EE>* Right(){ return right; }
    /////////////////////////////////////////////////////////////////////////////////
    explicit B_Terminal_Node( ExtendArray<KEY,DATA,COMP,EE>* arr, B_Internal_Node<KEY,DATA,COMP,EE>* parent=NULL )
    :arr(arr),parent(parent),left(NULL),right(NULL)
    { //must not NULL array
        #ifdef BTREE_DEBUG
        assertNotNULL< ExtendArray<KEY,DATA,COMP,EE>* >( arr );
        #endif
        //
        fanout = arr->getNumMax()-1;
    }
    explicit B_Terminal_Node( const int fanout, B_Internal_Node<KEY,DATA,COMP,EE>* parent=NULL )
    :fanout(fanout), parent(parent),arr( new ExtendArray<KEY,DATA,COMP,EE>(fanout+1) ),
     left(NULL), right(NULL)
    { }               /// make fanout+1!!!
    virtual ~B_Terminal_Node(){ if( NULL != arr ){ delete arr; } }
public: /// maybe private?
    bool isOver()const{ return arr->isFull(); }
    bool isEmpty()const{ return arr->isEmpty(); }
    bool isExist( const KEY& key )const{ return arr->isExist(); }
    bool isInternal()const{ return false; }
    bool isTerminal()const{ return true; }
    bool isRoot()const{ return NULL == parent; } ///
    long getEfficiency()const{
        return static_cast<long>( static_cast<double>( arr->getDataNum() ) / fanout * 100 );
    }
public:
public:
    bool insert( const KEY& key, const DATA& data )
    {
        bool temp =  arr->insert( key, data, true );
        return temp;
    }
    bool insert( B_Terminal_Node<KEY,DATA,COMP,EE>& tn )
    {
        bool bTemp = this->arr->insert( *(tn.arr) );
        return bTemp;
    }
    bool remove( const KEY& key ){
        bool btemp = arr->removeFromKey( key );
        return btemp;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* copyUp();
    bool getData( const KEY& key, DATA& data )const
    {
        return arr->getDataFromKey( key, data );
    }
    KEY getKey( const int index )const
    {
        return arr->getKeyFromIndex( index ); //
    }
    DATA getDataFromIndex( const int index )const
    {
    	return arr->getDataFromIndex( index );
    }
	bool setData(const KEY& key, const DATA& data)
	{
		return arr->setDataFromKey(key, data);
	}
    int getKeyNum()const{
        return arr->getKeyNum();
    }
    int getDataNum()const{
        return arr->getDataNum();
    }
private:
    B_Terminal_Node<KEY,DATA,COMP,EE>* divide( const int index )
    {
        B_Terminal_Node<KEY,DATA,COMP,EE>* temp = new B_Terminal_Node<KEY,DATA,COMP,EE>( arr->divide( index ) );
        // left, right setting
        temp->right = this->right;
        temp->left = this;
        if( NULL != this->right )
        {
            this->right->left = temp;//
        }
        this->right = temp;
        return temp;
    }
};

template <class KEY, class DATA, class COMP, class EE>
class B_Internal_Node : public B_Node<KEY,DATA,COMP,EE>
{
private:
    B_Internal_Node( const B_Internal_Node& in );
    B_Internal_Node& operator=( const B_Internal_Node& in );
private:
    PtrArray<KEY,B_Node<KEY,DATA,COMP,EE>*,COMP,EE>* arr; // new initially
    B_Internal_Node<KEY,DATA,COMP,EE>* parent; // NULL initially
    //int parentNo; // -1 initially
    int fanout;
    B_Internal_Node<KEY,DATA,COMP,EE>* left;
    B_Internal_Node<KEY,DATA,COMP,EE>* right;
public:
    void setLeft( B_Internal_Node<KEY,DATA,COMP,EE>* left ){ this->left = left; }
    void setRight( B_Internal_Node<KEY,DATA,COMP,EE>* right ){ this->right = right; }
    void setParent( B_Internal_Node<KEY,DATA,COMP,EE>* parent ){
        this->parent = parent;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* getParent()const
    {
        return parent;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    explicit B_Internal_Node( PtrArray<KEY,B_Node<KEY,DATA,COMP,EE>*, COMP, EE>* arr, B_Internal_Node<KEY,DATA,COMP,EE>* parent = NULL )
    : arr(arr), parent( parent ), left(NULL), right(NULL)
    {
        this->fanout = arr->getNumMax()-1;//
    }
    explicit B_Internal_Node( const int fanout, B_Internal_Node<KEY,DATA,COMP,EE>* parent=NULL )
    : fanout( fanout ), parent( parent ), left(NULL), right(NULL)
    {
        arr = new PtrArray<KEY,B_Node<KEY,DATA,COMP,EE>*,COMP,EE>( fanout+1 ); // fanout :  num of child!
    }
    virtual ~B_Internal_Node()
    {
        if( NULL != arr ){ delete arr; }
    }
public:
    bool isOver()const{ return arr->isFull(); }
    bool isEmpty()const{ return arr->isEmpty(); }
public:
    bool insert( KEY key, B_Node<KEY,DATA,COMP,EE>* leftB_Node, B_Node<KEY,DATA,COMP,EE>* rightB_Node )
    {
        return arr->insert( key, leftB_Node, rightB_Node ); //
    }
    bool insert( KEY key, B_Node<KEY,DATA,COMP,EE>* B_Node )
    {
        return arr->insert( key, B_Node  ); //
    }
    /** To Do
    * key - from we find key!!
    */
    /** To Do
    * - 2012.2.15
    * check below funtion..
    */
    bool insert( const B_Internal_Node<KEY,DATA,COMP,EE>& rightIn ) // only used remove in btree !!, only left <- right..
    {
        /** To Do
        *  right == empty? then?? if left == empty??  then??
        */
        /// if right == empty.. then??
        if( NULL == rightIn.getParent() ){ return false; }
        if( rightIn.getParent()->isEmpty() ){ return true; }
        if( rightIn.Left() != static_cast<B_Internal_Node<KEY,DATA,COMP,EE>* >(this) ){ return false; }

        //////////////////////////////////////////////////////////////////////
        bool bTemp = true;
        B_Internal_Node<KEY,DATA,COMP,EE>* rightParent = rightIn.getParent();
        // insert ( parent->findKeyFromIndex( parent->findDataIndexFrom(Left->any) ), NULL )
        const int middleIndex = rightParent->getDataIndexFromKey( rightIn.getKeyFromIndex( 0 ) )-1;
        const KEY middleKey = rightParent->getKeyFromIndex( middleIndex );
        int start = 1;
        if( this->isEmpty() != true )
        {
            bTemp = this->arr->insert( middleKey, rightIn.getDataFromIndex( 0 ) );
            start = 1;
        }
        else{
            bTemp = this->arr->insert( middleKey, rightIn.getDataFromIndex( 0 ),
                                       rightIn.getDataFromIndex( 1 ) );
            start = 2;
        }
        // copy
        for( int i=start;i <= rightIn.getKeyNum();i++ ){
                                                        //
            bTemp = bTemp && this->arr->insert( rightIn.getKeyFromIndex( i-1 ), rightIn.getDataFromIndex( i ) );
        }
        return bTemp;
    }
    bool removeFromKey( const KEY key ) /// rightChild를 지운다.
    {
        return arr->removeFromIndex( arr->getDataIndexFromKey( key ) );
    }
    bool removeFromIndex( const int index )
    {
        return arr->removeFromIndex( index );
    }
    B_Node<KEY,DATA,COMP,EE>* getChild( const KEY key )const
    {
        B_Node<KEY,DATA,COMP,EE>* temp = NULL;
        arr->getDataFromKey( key, temp );
        return temp;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* moveUp();
public:
    bool isInternal()const{ return true; }
    bool isTerminal()const{ return false; }
    bool isRoot()const{ return NULL == parent; }
    long getEfficiency()const{                              // cf) getKeyNum()
        return static_cast<long>( static_cast<double>( arr->getDataNum() ) / fanout * 100 );
    }
    int getKeyNum()const{
        return arr->getKeyNum();
    }
    int getDataNum()const{
        return arr->getDataNum();
    }
    int getDataIndexFromKey( KEY key )const
    {
        return arr->getDataIndexFromKey( key );
    }
    KEY getKeyFromIndex( int index )const
    {
        return arr->getKeyFromIndex( index );
    }
    B_Node<KEY,DATA,COMP,EE>* getDataFromIndex( int index )const
    {
        return arr->getDataFromIndex( index );
    }

    B_Internal_Node<KEY,DATA,COMP,EE>* Left()const///or use friend??
    { /// empty일 때, key를 이용한다.?
        return left;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* Right()const{ /// empty일 때, key를 이용한다.?
        return right;
    }
public:
    /** To Do
    *
    */
    /// moveToOtherB_Terminal_Node
    /// left or right 합치기
private:
    B_Internal_Node<KEY,DATA,COMP,EE>* divide( const int index ){
        B_Internal_Node<KEY,DATA,COMP,EE>* temp = new B_Internal_Node<KEY,DATA,COMP,EE>( arr->divide(index) );
        // child들의 parent를 temp로 한다.
        for( int i=0;i < temp->arr->getPtrNum();i++ )
        {
            temp->arr->getConstArray()[i].data->setParent( temp );
        }
        // left, right setting
        temp->right = this->right;
        temp->left = this;
        if( NULL != this->right )
        {
            this->right->left = temp;//
        }
        this->right = temp;
        return temp;
    }
};

template <class KEY, class DATA, class COMP, class EE>
B_Internal_Node<KEY,DATA,COMP,EE>* B_Terminal_Node<KEY,DATA,COMP,EE>::copyUp()
{
    // must isOver
    if( !isOver() ){ return NULL; }
    // select index
    const int index = getKeyNum() >> 1; // /2
    // and divide
    B_Terminal_Node<KEY,DATA,COMP,EE>* temp = divide( index );
    B_Internal_Node<KEY,DATA,COMP,EE>* parent = this->parent;

    // checkNULL
    //if( NULL = temp ){ return NULL; }
    ///assertNotNULL<B_Terminal_Node<KEY,DATA,COMP,EE>*>( temp );
    // and ( make B_Internal_Node<KEY,DATA,COMP,EE>* ) optionally,
    if( NULL == parent ){
        parent = new B_Internal_Node<KEY,DATA,COMP,EE>( this->fanout );
        // parent reset
        this->parent = parent;
        temp->parent = parent;
        // parentNo reset
        //this->parentNo = 0;
        //temp->parentNo = 1;
    }
    // parent setting, parentNo setting
    temp->parent = parent;
    //temp->parentNo = this->parentNo + 1;
    // and insert newKey, newChild to B_Internal_Node<KEY,DATA,COMP,EE>
    if( parent->isEmpty() ) //
    {
        parent->insert( temp->getKey( 0 ), this, temp ); // left, right
    }
    else
    {
        parent->insert( temp->getKey( 0 ), temp );
    }
    ///cout << "parent Insert" << temp->getKey(0) << endl << endl;
    return parent;
}
template <class KEY, class DATA, class COMP, class EE>
B_Internal_Node<KEY,DATA,COMP,EE>* B_Internal_Node<KEY,DATA,COMP,EE>::moveUp()
{
    // isOver()
    if( !isOver() ){ return NULL; }
    //
    B_Internal_Node<KEY,DATA,COMP,EE>* temp = NULL;
    B_Internal_Node<KEY,DATA,COMP,EE>* parent = this->parent;
    // select index
    const int index = getKeyNum() >> 1;
    // key that moveUped!!
    const KEY moveKey = arr->getKeyFromIndex( index );
    // and divide
    temp = this->divide( index );

    // child들의 parent들을 바꾼다.
    // checkNULL
    ///assertNotNULL( temp );
    // and ( make B_Internal_Node<KEY,DATA,COMP,EE>* ) optionally,
    if( NULL == parent ){
        parent = new B_Internal_Node<KEY,DATA,COMP,EE>( this->fanout );
        // parent reset
        this->parent = parent;
        temp->parent = parent;
        // parentNo reset
        //this->parentNo = 0;
        //temp->parentNo = 1;
    }
    // parent setting, parentNo setting
    temp->parent = parent;
    //temp->parentNo = this->parentNo + 1;
    // and insert newKey, newChild to B_Internal_Node<KEY,DATA,COMP,EE>
    if( parent->isEmpty() )
    {
        parent->insert( moveKey, this, temp );
    }
    else
    {
        parent->insert( moveKey, temp );
    }
    // and return
    return parent;
}

template < class KEY, class DATA, class COMP=wiz::ASC<KEY>, class EE=wiz::EE<KEY> >
class BTree /// B+ Tree
{
public:
	BTree(const BTree& bt)
	{
		fanout = bt.fanout;
		root = NULL;
		efficiency = bt.efficiency;
		count = count;
		first_node = NULL;
		last_node = NULL;

		
		for ( auto x = bt.cbegin(); x != bt.cend(); ++x )
			for (int i = 0; i < x->getDataNum(); i++)
			{
				insert( x->getKey(i), x->getDataFromIndex(i) );
			}
	}
	BTree& operator=(const BTree& bt)
	{
		if( root == bt.root ) { return *this; }
		
		removeTree();

		for ( auto x = bt.cbegin(); x != bt.cend(); ++x )
			for (int i = 0; i < x->getDataNum(); i++)
			{
				insert( x->getKey(i), x->getDataFromIndex(i) );
			}

		return *this;
	}
private:
    int fanout;
	/// chk fanout >= 3 ?
    B_Node<KEY,DATA,COMP,EE>* root;
    B_Terminal_Node<KEY,DATA,COMP,EE>* first_node;
    B_Terminal_Node<KEY,DATA,COMP,EE>* last_node;
    long efficiency; /** 0 ~ 100 */
	int count; /// chk..
public:
	BTree(const int fanout = 100, const long efficiency = 50) : fanout(fanout), root(NULL), first_node(NULL), last_node(NULL), efficiency(efficiency), count(0)
    { }
    virtual ~BTree(){
        /** Done
        * // if root != NULL
        * // find empty B_Node, while ->right !=NULL delete,
        * // find 위 B_Node의 parent, also do same..
        * // while parent is not root
        * // then delete root.
        */
        removeTree();
    }
public:
    bool insert( const KEY& key, const DATA& data );
    bool remove( const KEY& key );
    bool getData( const KEY& key, DATA& data )const;
    bool isExist( const KEY& key )const;
public:
    bool isRoot( const B_Node<KEY,DATA,COMP,EE>* B_Node )const{ return B_Node != NULL && B_Node == root; }
    void removeTree( void (*f)(DATA data)=NULL ) // ptr, ->no ref? 2012.5.13
    {
        if( NULL != root )
        {
            if( root->isTerminal() ){
                delete root; root = NULL;
            }
            else{ // root->isInternal()
                B_Node<KEY,DATA,COMP,EE>* B_NodeTemp = root;
                // find left Terminal B_Node
                while( B_NodeTemp->isInternal() ){
                    B_NodeTemp = static_cast<B_Internal_Node<KEY,DATA,COMP,EE>*>( B_NodeTemp )->getDataFromIndex( 0 );
                }
                B_Terminal_Node<KEY,DATA,COMP,EE>* tnTemp = static_cast<B_Terminal_Node<KEY,DATA,COMP,EE>*>( B_NodeTemp );
                B_Internal_Node<KEY,DATA,COMP,EE>* inTemp = tnTemp->getParent();
                // Terminal B_Node Remove
                while( NULL != tnTemp ){
                    B_Terminal_Node<KEY,DATA,COMP,EE>* nextTnTemp = tnTemp->Right();
                    if( NULL != f )
					{
						for( int i=0; i < tnTemp->getDataNum(); i++ )
						{
							f( tnTemp->getDataFromIndex( i ) ); // remove i !
						}
					}
                    delete tnTemp; tnTemp = NULL;
                    tnTemp = nextTnTemp;
                }
                // Internal B_Node Remove
                while( NULL != inTemp )
                {
                    B_Internal_Node<KEY,DATA,COMP,EE>* inParentTemp = inTemp->getParent();
                    while( NULL != inTemp )
                    {
                        B_Internal_Node<KEY,DATA,COMP,EE>* nextInTemp = inTemp->Right();
                        delete inTemp; inTemp = NULL;
                        inTemp = nextInTemp;
                    }
                    inTemp = inParentTemp;
                }
            }
        }
        root = NULL;
        first_node = NULL;
        last_node = NULL;
    }
    static bool isSameGroup( B_Terminal_Node<KEY,DATA,COMP,EE>* tn, B_Terminal_Node<KEY,DATA,COMP,EE>* tn2 )
    {
        return NULL != tn && NULL != tn2 && tn->getParent() == tn2->getParent();
    }
    static bool isSameGroup( B_Internal_Node<KEY,DATA,COMP,EE>* in, B_Internal_Node<KEY,DATA,COMP,EE>* in2 )
    {
        return NULL != in && NULL != in2 && in->getParent() == in2->getParent();
    }
private:
    void setRoot( B_Internal_Node<KEY,DATA,COMP,EE>* in ){
        root = static_cast< B_Node<KEY,DATA,COMP,EE>* >( in );
        in->setParent( NULL );
    }
    void setRoot( B_Terminal_Node<KEY,DATA,COMP,EE>* tn ){
        root = static_cast< B_Node<KEY,DATA,COMP,EE>* >( tn );
        tn->setParent( NULL );
    }
    B_Terminal_Node<KEY,DATA,COMP,EE>* findB_Terminal_Node( const KEY& key, B_Node<KEY,DATA,COMP,EE>* in )const{
        // Null test
        if( NULL == in ){ return NULL; }
        //
        B_Node<KEY,DATA,COMP,EE>* temp = in;
        while( temp != NULL && !( temp->isTerminal() ) ){
            temp = static_cast<B_Internal_Node<KEY,DATA,COMP,EE>*>( temp )->getChild( key );
        }
        return static_cast<B_Terminal_Node<KEY,DATA,COMP,EE>*>( temp );
    }
    // left에 right를 합쳐서 left를 리턴한다.
    B_Terminal_Node<KEY,DATA,COMP,EE>* unionB_Terminal_Node( B_Terminal_Node<KEY,DATA,COMP,EE>* left )
    {
        // right = left->Right();
        B_Terminal_Node<KEY,DATA,COMP,EE>* right = left->Right();
        // if don`t exist  then return NULL;
        if( NULL == right ){ return NULL; }
        // left->insert( right );
        left->insert( *right );

        return left;
    }
    B_Internal_Node<KEY,DATA,COMP,EE>* unionB_Internal_Node( B_Internal_Node<KEY,DATA,COMP,EE>* left )
    {
        // right = left->Right();
        B_Internal_Node<KEY,DATA,COMP,EE>* right = left->Right();
        // if don`t exist right then return NULL;
        if( NULL == right ){ return NULL; }
        // right->child->parent reset..
        for( int i=0; i < right->getDataNum();i++ )
        {
            right->getDataFromIndex(i)->setParent( left );
        }
        // left->insert( right );
        left->insert( *right );

        return left;
    }
public:
	int size() const {
		return count;
	}
	Array<DATA> getDataToArray() const
	{
		Array<DATA> tempArr(size());
		int k=0;
		for (auto x= cbegin(); x != cend() ; ++x )
			for (int i = 0; i < (*x).getDataNum(); i++){
				tempArr[k] = (*x).getDataFromIndex(i);
				k++;
			}

		return tempArr;
	}
	/// range search
    bool getData( const KEY& key1, const KEY& key2, OUT std::vector<DATA>& dataVec )
    {
        ///if( key1 > key2 ) { return false; } /// or return getData( key2, key1 )
        if( COMP() ( key2, key1 ) ) { return false; }
        B_Terminal_Node<KEY,DATA,COMP,EE>* tnTemp = findB_Terminal_Node( key1, root );

        bool start = false;

        while( tnTemp )
        {
            for( int i=0; i < tnTemp->getDataNum(); i++ )
            {
                DATA data = tnTemp->getDataFromIndex( i );
                KEY key = tnTemp->getKey( i );

                //if( key1 <= key && key <= key2 )
                if( ( COMP() ( key1, key ) || EE() ( key1, key )  )
                   && ( COMP() ( key, key2 ) ) || EE() ( key, key2 ) )
                {
                    dataVec.push_back( data );
                    start = true;
                }
                //else if ( key > key2 ) { return start; }
                else if( COMP() ( key2, key ) ) { return start; }
            }
            tnTemp = tnTemp->Right();
        }
        return start; // chk
    }
	bool setData(const KEY& key, const DATA& data)
	{
		B_Terminal_Node<KEY, DATA, COMP, EE>* tnTemp = findB_Terminal_Node(key, root);
		if (!tnTemp) {
			return false; 
		}
		return tnTemp->setData(key, data);
	}
    class Iter{
    private:
        B_Terminal_Node<KEY,DATA,COMP,EE>* node;
    private:
        void left() {
            if( node == NULL ) { throw Error( __FILE__, __LINE__, "node == NULL" ); }
            node = node->left;
        }
        void right() {
            if( node == NULL ) { throw Error( __FILE__, __LINE__, "node == NULL" ); }
            node = node->right;
        }
    public:
        explicit Iter( B_Terminal_Node<KEY,DATA,COMP,EE>* node = NULL ) : node(node) {  }
    public:
        bool operator!=( const Iter& iter ) { return node != iter.node; }
        void operator++() { right(); }
        void operator--() { left(); }
        B_Terminal_Node<KEY,DATA,COMP,EE>& operator*() { return *node; }
        const B_Terminal_Node<KEY,DATA,COMP,EE>& operator*()const { return *node; }
		B_Terminal_Node<KEY,DATA,COMP,EE>* operator->() { return node; }
		const B_Terminal_Node<KEY,DATA,COMP,EE>* operator->()const{ return node; }
    };
    class ConstIter{
    private:
        B_Terminal_Node<KEY,DATA,COMP,EE>* node;
    private:
        void left() {
            if( node == NULL ) { throw Error( __FILE__, __LINE__, "node == NULL" ); }
            node = node->left;
        }
        void right() {
            if( node == NULL ) { throw Error( __FILE__, __LINE__, "node == NULL" ); }
            node = node->right;
        }
    public:
        explicit ConstIter( B_Terminal_Node<KEY,DATA,COMP,EE>* node = NULL ) : node(node) {  }
    public:
        bool operator!=( const ConstIter& iter ) { return node != iter.node; }
        void operator++() { right(); }
        void operator--() { left(); }
        const B_Terminal_Node<KEY,DATA,COMP,EE>& operator*()const { return *node; }
		const B_Terminal_Node<KEY,DATA,COMP,EE>* operator->()const{ return node; }
    };
	/// chk....
    Iter begin() { return Iter( first_node ); }
    Iter end() { return Iter(); }
    Iter rbegin() { return Iter( last_node ); }
    Iter rend() { return Iter(); }
    ConstIter cbegin()const { return ConstIter( first_node ); }
    ConstIter cend()const { return ConstIter(); }
    ConstIter crbegin()const { return ConstIter( last_node ); }
    ConstIter crend()const { return ConstIter(); }
};

template <class KEY, class DATA, class COMP, class EE>
bool BTree<KEY,DATA,COMP,EE>::insert( const KEY& key, const DATA& data )
{
    // if NULL root
    if( NULL == root ){
        // make B_Terminal_Node
        root = static_cast<B_Node<KEY,DATA,COMP,EE>*>( new B_Terminal_Node<KEY,DATA,COMP,EE>( fanout ) );
        first_node = static_cast<B_Terminal_Node<KEY,DATA,COMP,EE>*>( root );
        last_node = first_node;
    }
    // find Terminal B_Node
    //cout << "will find tnTemp.." << endl;
    B_Terminal_Node<KEY,DATA,COMP,EE>* tnTemp = findB_Terminal_Node( key, root );
    //cout << "finded tnTemp.." << endl;
    // temp for copyUp
    B_Internal_Node<KEY,DATA,COMP,EE>* inTemp = NULL;
    // insert
    bool isOk = tnTemp->insert( key, data );
    if( isOk && tnTemp->isOver() )
    {
        inTemp = tnTemp->copyUp();

        if( last_node == tnTemp ) { last_node = tnTemp->Right(); }

        while( inTemp->isOver() )
        {
            inTemp = inTemp->moveUp();
        }
        if( !isRoot( inTemp ) && inTemp->isRoot() ) /// canRoot??
        {
            // now new Root!!
            setRoot( inTemp );
        }
    }
	if (isOk) {
		count++;
	}
    return isOk;
}
/** To Do
*  remove인 경우는 Terminal에서만 remove하거나 실제로 지우거나 .. ??
*/
template <class KEY, class DATA, class COMP, class EE>
bool BTree<KEY,DATA,COMP,EE>::remove( const KEY& key ) // const KEY key, const int condSize // 합칠 조건??
{
    bool bTemp = true;

    // find teminal and terminal`s parent
    B_Terminal_Node<KEY,DATA,COMP,EE>* terminal = findB_Terminal_Node( key, root );
    if( NULL == terminal ) // return false;
	{ return false; } // throw exception!

    B_Internal_Node<KEY,DATA,COMP,EE>* terminalParent = terminal->getParent();
    B_Terminal_Node<KEY,DATA,COMP,EE>* terminalFriend = NULL; // may be left!

    // data remove in B_Terminal_Node<KEY,DATA,COMP,EE>
    bTemp = terminal->remove( key );

	if (bTemp) {
		count--;
	}
    // chk terminalFriend.
    bool isLeftFriend = true; //

    if( isSameGroup( terminal, terminal->Left() ) )
    {
        terminalFriend = terminal->Left();
    }
    else if( isSameGroup( terminal, terminal->Right() ) )
    {
        terminalFriend = terminal->Right();
        isLeftFriend = false;
	}

    // case 1 remove root
    if( bTemp && isRoot( terminal ) && terminal->isEmpty() )
    {
        // terminal를 지운다. null로 설정.
        removeTree();
    }
    // case 2 remove B_Node?
    else if( bTemp )
    {
        // chk condition
        if( !isRoot( terminal ) )// not root -> must have friend !
        {
            if( terminalParent->getKeyNum() > 1 // 윗부분(internal)은 항상 남긴다.
			&& ( terminalFriend->getEfficiency() < efficiency
			|| terminal->getEfficiency() < efficiency  // efficiency condition
            )
            && terminal->getDataNum() + terminalFriend->getDataNum() <= fanout ) // 합치기 위한 조건
            //  두개의 terminal를 합친다.
            {
                B_Terminal_Node<KEY,DATA,COMP,EE>* leftTemp;
                B_Terminal_Node<KEY,DATA,COMP,EE>* rightTemp;
                int tpIndex = terminalParent->getDataIndexFromKey( key ); // index for rightTemp at TerminalParent
                        // tpIndex : rightIndex!
                if( isLeftFriend )
                {
                    leftTemp = terminalFriend;
                    rightTemp = terminal;
                }
                else
                {
                    leftTemp = terminal;
                    rightTemp = terminalFriend;
                    tpIndex = tpIndex + 1; //
                }

                unionB_Terminal_Node( leftTemp );
                if( rightTemp == last_node ) { last_node = leftTemp; }
 //  두개의 terminal들의 부모를 처리한다.
                if( !terminalParent->removeFromIndex( tpIndex ) )
                {
                }

                // leftTemp와 rightTemp의 right에 대한 셋팅.
                leftTemp->setRight( rightTemp->Right() );
                if( NULL != rightTemp->Right() )
                {
                    rightTemp->Right()->setLeft( leftTemp );
                }

                // right를 제거한다.
                delete rightTemp; rightTemp = NULL;
                // terminalParent가 empty인경우. and root
                if( terminalParent->isEmpty() && isRoot( terminalParent ) ) // case 3
                {
                    delete terminalParent; terminalParent = NULL;
                    setRoot( leftTemp ); return bTemp;
                }
            }
            // case 4
            {
                B_Internal_Node<KEY,DATA,COMP,EE>* tpParent = terminalParent;

                while( !isRoot( tpParent ) ) // root가 아니면, 항상 parent가 있어야 한다.
                {
                    B_Internal_Node<KEY,DATA,COMP,EE>* tpFriend = NULL;
                    B_Internal_Node<KEY,DATA,COMP,EE>* tpLeftTemp = NULL;
                    B_Internal_Node<KEY,DATA,COMP,EE>* tpRightTemp = NULL;
                    int tppIndex = tpParent->getParent()->getDataIndexFromKey( key );
                    bool isLeftTpFriend = true;
                    B_Internal_Node<KEY,DATA,COMP,EE>* tpParentTemp = tpParent->getParent();

                    if( isSameGroup( tpParent, tpParent->Left() ) )
                    {
                        tpFriend = tpParent->Left(); //
                        tpLeftTemp = tpFriend;
                        tpRightTemp = tpParent;
                    }
                    else if( isSameGroup( tpParent, tpParent->Right() ))
                    {
                        tpFriend = tpParent->Right();
                        tpRightTemp = tpFriend;
                        tpLeftTemp = tpParent;
                        isLeftTpFriend = false; //
                        tppIndex++;
                    }
					// else { err }

                    if( tpParent->getParent()->getKeyNum() > 1 // removed tpFreind Null check
					&&
					(  tpFriend->getEfficiency() < efficiency
                        || tpParent->getEfficiency() < efficiency  // efficiency condition
                        )
                        && tpParent->getDataNum() + tpFriend->getDataNum()  <= fanout ) /// 2012.6.8
                    {
                        int leftN = tpLeftTemp->getDataNum();

                        //  left와 right를 합친다.
                        unionB_Internal_Node( tpLeftTemp );


                        ///  두개의 Internal들의 부모를 처리한다. /// chk.. == 1, then remove root!!
                        {
                            tpParent->getParent()->removeFromIndex( tppIndex ); /// chk!! why -1?
                        }

                         //leftTemp와 rightTemp의 right에 대한 셋팅.
                        tpLeftTemp->setRight( tpRightTemp->Right() );
                        if( NULL != tpRightTemp->Right() )
                        {
                            tpRightTemp->Right()->setLeft( tpLeftTemp );
                        }

                        //  right를 지운다.
                        delete tpRightTemp; tpRightTemp = NULL;
                    }

                    tpParent = tpParentTemp;
                }

            }
        }
    }

    return bTemp;
}
template <class KEY, class DATA, class COMP, class EE>
bool BTree<KEY,DATA,COMP,EE>::getData( const KEY& key, DATA& data )const // return 주소! null이면 없다?
{
    // NULL ROOT
    if( NULL == root ){ return false; }
    const B_Terminal_Node<KEY,DATA,COMP,EE>* tnTemp = findB_Terminal_Node( key, root );

	bool bTemp = false;

	//if( NULL != tnTemp )
	{
        bTemp = tnTemp->getData( key, data ); // .copy! ?
	}

	return bTemp;
}
template <class KEY, class DATA, class COMP, class EE>
bool BTree<KEY,DATA,COMP,EE>::isExist( const KEY& key )const
{
	DATA data;

	return getData( key, data );
}

}
#endif // BTREE_H_INCLUDED
