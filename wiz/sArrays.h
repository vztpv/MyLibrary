#ifndef SARRAYS_H_INCLUDED
#define SARRAYS_H_INCLUDED

///#define SARRAYS_DEBUG
/// ExtendArray<KEY,DATA,COMP,EE> /// COMP : key1 < key2, EE : key1 == key2
/// PtrArray<KEY,DATA,COMP,EE> /// COMP : key1 < key2, EE : key1 == key2


#include <wiz/global.h>
#include <wiz/wizardError.h>
#include <wiz/newArrays.h>

namespace wiz{
template <class KEY, class DATA, class COMP, class EE > /// type of DATA is pointer!!
class PtrArray;

template <class KEY, class DATA>
class SNode /// rename??
{
    public:
        KEY key;
        DATA data;
    public:
        SNode() : key( KEY() ), data( DATA() ) { }
};

template < class KEY, class DATA, class COMP=wiz::ASC<KEY>, class EE=wiz::EE<KEY> >
class ExtendArray
{
    public:
        ExtendArray( const ExtendArray<KEY,DATA,COMP,EE>& ea )
        {
            num = ea.num;
            arr = ea.arr;
        }
        ExtendArray<KEY,DATA,COMP,EE>& operator=( const ExtendArray<KEY,DATA,COMP,EE>& ea )
        {
            if( arr != ea.arr ){
                arr.DoEmpty();
                arr = ea.arr;
                num = ea.num;
            }
            return *this;
        }
        ExtendArray( ExtendArray<KEY,DATA,COMP,EE>&& ea )
        {
            num = ea.num;
            arr = std::move( ea.arr );

            ea.arr = Array< SNode<KEY,DATA> >( 1 );
            ea.num = 0;
        }
        ExtendArray<KEY,DATA,COMP,EE>& operator=( ExtendArray<KEY,DATA,COMP,EE>&& ea )
        {
            if( arr != ea.arr )
            {
                arr = std::move( ea.arr );
                num = ea.num;

                ea.arr = Array< SNode<KEY,DATA> >( 1 );
                ea.num = 0;
            }
            return *this;
        }
    private:
        bool lShift( const int sIndex, const int eIndex, const int offset );
    private:
        Array< SNode<KEY,DATA> > arr; /** set of (key, data) */
        int num; /** num of valid key */
    protected:
        Array< SNode<KEY,DATA> >& getArray(){ return arr; }
    public:
        const Array< SNode<KEY,DATA> >& getConstArray()const{ return arr; }
    public:
        explicit ExtendArray( const int arraySize=1 );
        virtual ~ExtendArray();
    public:
         bool insert( const KEY& key, const DATA& data, const bool onlyOne=true ); /** only one - 같은 data는 한개 만  */
         bool removeFromKey( const KEY key )
        {
            if( !isExist( key ) ){ return false; }
            bool temp = removeFromIndex( search( key ) );

            return temp;
        }
         bool removeFromIndex( const int index );

         bool getDataFromKey( const KEY& key, DATA& t )const
        {
            int index = search( key );
            if( 0 > index ){ return false; }
            t = getConstArray()[index].data;
            return true;
        }
         DATA getDataFromIndex( const int index )const
        {
            #ifdef SARRAYS_DEBUG
            checkIndexError( index, getNum() );
            #endif
            return getConstArray()[index].data;
        }
         int getNum()const{ return num; } /// remove..
         int getKeyNum()const{ return num; }
         int getDataNum()const{ return num; }
         int getNumMax()const{ return arr.size(); }
         int getKeyIndexFromKey( const KEY& key )const{ return search( key ); }
         int getDataIndexFromKey( const KEY& key )const{ return search( key ); }
        //
         KEY getKeyFromIndex( const int index )const{
            #ifdef SARRAYS_DEBUG
            checkIndexError( index, getNum() );
            #endif
            return getConstArray()[index].key;
        }
		 bool setDataFromKey( const KEY& key, const DATA& data ) {
			 int index = getDataIndexFromKey(key);

			 if (-1 == index) {
				 return false;
			 }

			 getArray()[index].data = data;
			 return true;
		 }
        /** first, last -> remove?? */
         bool first( DATA& data )const{
            if( isEmpty() ){ return false; }
            data = getConstArray()[0].data;
            return true;
        }
         bool last( DATA& data )const{
            if( isEmpty() ){ return false; }
            data = getConstArray()[getNum()-1].data;
            return true;
        }

    public:
        bool isExist( const KEY& key )const{ return search( key ) >= 0; }
        bool isFull()const{ return arr.size() <= num; }
        bool isEmpty()const{ return 0 >= num; }

        ExtendArray<KEY,DATA,COMP,EE>* divide( const int index ){ return split( index, getNum()-1 ); }
        ExtendArray<KEY,DATA,COMP,EE>* split( const int sIndex, const int eIndex ); /**  까지를 따로 떼어낸다.,에러는 return NULL */
        // todo - merge..?
        bool insert( const ExtendArray<KEY,DATA,COMP,EE>& ea )
        {
            //if( ea.isEmpty() ){ return true; }
            // insert
            for( int i=0;i < ea.getDataNum();i++ ){
                this->insert( ea.getConstArray()[i].key, ea.getConstArray()[i].data );
            }
            return true;
        }
    protected:
        void addNum( const int offset ){ num = num + offset; }
        void sort(); /** 가짜 sort - sort인 것처럼 보인다. */ // Asc Ascending order
    public:
        int search( const KEY& key )const /** -1 : notFound */
        {
            //if( isEmpty() ){ return -1; }
            return search( 0, num-1, key );
        }
        int search( const int left, const int right, const KEY& key )const;
        /// friend??
        friend class PtrArray<KEY,DATA,COMP,EE>;
};
template <class KEY, class DATA, class COMP, class EE>
ExtendArray<KEY,DATA,COMP,EE>::ExtendArray( const int arraySize )
{
    #ifdef SARRAYS_DEBUG
    checkUnderError( 0, arraySize, 1 ); // 1보다 작으면 에러.
    #endif
    arr = Array< SNode<KEY,DATA> >( arraySize );
    num = 0;
}
template <class KEY, class DATA, class COMP, class EE>
ExtendArray<KEY,DATA,COMP,EE>::~ExtendArray()
{
}

template <class KEY, class DATA, class COMP, class EE>
bool ExtendArray<KEY,DATA,COMP,EE>::insert( const KEY& key, const DATA& data, const bool onlyOne )
{
    if( onlyOne && isExist( key ) ){ return false; }
    //
    if( getConstArray().size() <= num ) { getArray().expand(); }
    getArray()[num].key = key;
    getArray()[num].data = data;
    //
    num++;
    //
    sort();
    //
    return true;
}
template <class KEY, class DATA, class COMP, class EE>
int ExtendArray<KEY,DATA,COMP,EE>::search( int left, int right, const KEY& key )const
{
    //int left = 0;  int right = num-1;//
    if( right < left ){ return -1; }
    int middle = -3;

    while( left <= right )
    {
        middle = ( left + right ) >> 1;
        //if( key == getConstArray()[middle].key )
        if( EE()( key, getConstArray()[middle].key ) )
        {
            return middle;//
        }
        else if( left == right ) //
        {
            break; //
        }
        //else if( key < getConstArray()[middle].key )
        else if( COMP() ( key, getConstArray()[middle].key ) )
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }

    return  -2;
}
template <class KEY, class DATA, class COMP, class EE>
void ExtendArray<KEY,DATA,COMP,EE>::sort() // 오름차순,  cf) 내림차순.
{
    if( num <= 1 ){ return; }
    SNode<KEY, DATA> temp;
    //
    temp = std::move( getArray()[num-1] );
    //
    int lastIndex =  num-1;
    //
    for( int i = num-1; i >= 1; i-- )
    {
        // 오름차순..
        //if( getConstArray()[i-1].key > temp.key )
        if( COMP() ( temp.key, getConstArray()[i-1].key ) )
        {
            getArray()[i] = std::move( getArray()[i-1] );
            lastIndex = i - 1;
        }
        else{ //
            break;
        }
    }
    // setValue
    getArray()[lastIndex] = std::move( temp );
}

template <class KEY, class DATA, class COMP, class EE>
bool ExtendArray<KEY,DATA,COMP,EE>::removeFromIndex( const int index )
{
    // #ifdef DEBUG?
    #ifdef SARRAYS_DEBUG
    checkIndexError( index, getNum() );
    #endif

    lShift( index+1, num-1, 1 ); // after lshift, do addNum; ?

    getArray()[num-1].key = KEY();
    getArray()[num-1].data = DATA();

    addNum( -1 );

    return true;
}

template <class KEY, class DATA, class COMP, class EE>
bool ExtendArray<KEY,DATA,COMP,EE>::lShift( const int sIndex, const int eIndex, const int offset )
{
    if( eIndex < sIndex ){ return false; }
    // #ifdef DEBUG?
    #ifdef SARRAYS_DEBUG
    checkIndexError( sIndex, getNum() );
    checkIndexError( eIndex, getNum() );
    #endif
    for( int i = sIndex; i <= eIndex; i++ )
    {
        // chk sIndex-offset is Valid Index?
        if( i >= offset )
        { // offset is always plus value!
            /// checkIndexError( i-offset ); ?
            getArray()[i-offset] = std::move( getArray()[i] );
        }
    }
    /// chk..  eIndex < i < num, a[i] := T() ?
    return true;
}
template <class KEY, class DATA, class COMP, class EE>
ExtendArray<KEY,DATA,COMP,EE>* ExtendArray<KEY,DATA,COMP,EE>::split( const int sIndex, const int eIndex )
{
    if( eIndex < sIndex ){ return NULL; }
    // #ifdef DEBUG?
    #ifdef SARRAYS_DEBUG
    checkIndexError( sIndex, getNum() );
    checkIndexError( eIndex, getNum() );
    #endif

    const int offset =  eIndex - sIndex + 1; /// offset >= 1
    ExtendArray<KEY,DATA,COMP,EE>* temp = new ExtendArray<KEY,DATA,COMP,EE>( getConstArray().size() );
    //  key, data copy from sIndex to eIndex
    for( int i = sIndex; i <= eIndex; i++ )
    {
        temp->insert( this->getArray()[i].key, this->getArray()[i].data ); // false?
    }

    this->lShift( eIndex+1, this->getNum()-1, offset );
    {
        /// need chk!!
        for( int i = this->getNum()-offset; i < this->getNum(); i++ ) {
            getArray()[i].key = KEY();
            getArray()[i].data = DATA();
        }
        addNum( -offset );
    }

    return temp;
}

template <class KEY, class DATA, class COMP, class EE> /// type of DATA is pointer!!
class PtrArray  /// 상속 대신, has?
/// PtrArray has ExtendArray..??
{
    private:
        int arraySize;
    private:
        // 복사, 대입 금지??
        PtrArray( const PtrArray<KEY,DATA,COMP,EE>& eq );
        //DONOTCOPY( PtrArray )
        PtrArray<KEY,DATA,COMP,EE>& operator=( const PtrArray<KEY,DATA,COMP,EE>& eq );
    private:
        ExtendArray<KEY,DATA,COMP,EE> extendArray;
    public:
        explicit PtrArray( const int arraySize ):arraySize(arraySize) { extendArray = ExtendArray<KEY,DATA,COMP,EE>( arraySize ); }
        virtual ~PtrArray(){  }
    public:
        /// 맨 처음 NonTerminalSNode에 넣을때 아래 함수를 호출
        bool insert( const KEY& key, const DATA& data, const DATA& data2 ) /// data not bool!
        {
            #ifdef SARRAYS_DEBUG
            assertEquals<int>( "first..", getPtrNum(), 0 );
            #endif

            bool temp = extendArray.insert( key, data, true );
            temp = temp && extendArray.insert( key, data2, false );
            return temp;
        }
        /// 두번째부터 key,data를 넣을때는 일반적으로 한다.
         bool insert( const KEY& key, const DATA& data, const bool isSplit=false, const bool onlyOne=true ) /** only one - 같은 data는 한개 만  */
        {
            #ifdef SARRAYS_DEBUG
            assertNotEquals<int>( "second~insert", getPtrNum(), 0 );
            #endif

            bool temp = extendArray.insert( key, data, onlyOne ); //
            /// 위의 insert에서 onlyone == true and, 가장 큰 key값이 자기 자신이라면,
            //if( temp && key == extendArray.getConstArray()[getPtrNum()-1].key ){
            if( temp && EE() ( key, extendArray.getConstArray()[getPtrNum()-1].key ) )
            {
                /// 항상 가장 큰 key값만 2번 나오도록 한다.!!!
                extendArray.getArray()[getPtrNum()-2].key = extendArray.getConstArray()[getPtrNum()-1].key;
            }
            /** To Do
            *  change function name?
            */
            //if( !isSplit && temp && key != extendArray.getConstArray()[getPtrNum()-1].key ){ // shift가 하나라도 일어난다면..
            if( !isSplit && temp && (!EE()( key, extendArray.getConstArray()[getPtrNum()-1].key) ) )
            {
                // 오른쪽의 DATA과 바꾼다.- !isSplit?
                const int index = getKeyIndexFromKey( key );
                DATA dTemp = extendArray.getConstArray()[index].data;
                extendArray.getArray()[index].data = extendArray.getConstArray()[index+1].data;
                extendArray.getArray()[index+1].data = dTemp;
            }
            return temp;
        }
         bool removeFromIndex( const int index ) // key 기준..
        {
            const int offset = -1;

            #ifdef SARRAYS_DEBUG
            checkIndexError( index, getPtrNum() ); /// empty일때, 지우면 예외를 던진다.
            assertNotEquals<int>( "remove", getPtrNum(), 1 ); /// 0개, 2개,3개,4개...
            #endif

            if( getPtrNum() == 2 ){
                return extendArray.removeFromIndex( 0 ) && extendArray.removeFromIndex( 0 );
            }
            else if( index == getPtrNum()-1 ) /// 마지막 노드 ( , 마지막 노드-1 : key값이 중복되는 유일한 경우 ).
            {
                bool temp = extendArray.removeFromIndex( index );
                if( temp ){ extendArray.getArray()[getPtrNum()-1].key = extendArray.getConstArray()[getPtrNum()-2].key; }
                return temp;
            }
            else{
                /// 주의!!???                             // cf) <=
                if( index + offset >= 0 && index + offset < getPtrNum()-1 ) // offset only -1 !!
                {
                    extendArray.getArray()[index+offset].key = extendArray.getConstArray()[index].key;
                    /// for B+Tree Implementation..
                }

                return extendArray.removeFromIndex( index );
            }
        }
    public:
        int getKeyIndexFromKey( const KEY& key )const{ /// 가장 큰값인 경우, left가 리턴.
            return search( key );
        }
         int search( const KEY& key )const /** -1 : notFound */
        {
            return extendArray.search( 0, extendArray.getKeyNum()-1, key );
        }

        /// chk..
        int getDataIndexFromKey( const KEY& key )const{ /// key[index-1] <= key < key[index] , find index.
            if( extendArray.isEmpty() ){ return -1; }

            int left = 1, right = getPtrNum()-2; // ptr-2, ptr-1
            int middle=-1;
            // last
            //if( key >= extendArray.getConstArray()[getPtrNum()-2].key )
            if( COMP() ( extendArray.getConstArray()[getPtrNum()-2].key, key )
               || EE() ( extendArray.getConstArray()[getPtrNum()-2].key, key )
            )
            {
                //data = this->getConstArray()[getPtrNum()-1].data; //
                //cout << "dataIndex is " << getPtrNum()-1 << endl;
                return getPtrNum()-1;
            }
            // first
            //else if( key < extendArray.getConstArray()[0].key )
            else if( COMP() ( key, extendArray.getConstArray()[0].key ) )
            {
                //data = this->getConstArray()[0].data;//
                //cout << "dataIndex is " << 0 << endl;
                return 0;
            }

            middle = ( left  + right ) >> 1; // /2
            while( left < right )
            {
                //if( extendArray.getConstArray()[middle].key <= key )
                if( COMP() ( extendArray.getConstArray()[middle].key, key )
                   || EE() ( extendArray.getConstArray()[middle].key, key ) )
                {
                    left = middle+1; //
                }
                else
                {
                    right = middle; //
                }
                middle = ( left  + right ) >> 1; // /2
            }
/*
            // linear search..
            middle = 1;
            for( int i=1; i < extendArray.getConstArray().size()-1; i++ )
            {
                if( extendArray.getConstArray()[i].key > key )
                {
                    middle = i; break;
                }
            }*/
            return middle;
        }
    public:
         KEY getKeyFromIndex( const int index )const
        {
            #ifdef SARRAYS_DEBUG
            checkIndexError( index, getPtrNum() );
            #endif
            return extendArray.getConstArray()[index].key;
        }
         DATA getDataFromIndex( const int index )const
        {
            #ifdef SARRAYS_DEBUG
            checkIndexError( index, getPtrNum() );
            #endif
            return  extendArray.getConstArray()[index].data;
        }
         bool getDataFromKey( const KEY& key, DATA& data )const
        {
            const int index = getDataIndexFromKey( key );
            if( index < 0 ){ return false; }
            data = extendArray.getConstArray()[index].data;
            return true;
        }
         int getKeyNum()const
        {
            const int num =  extendArray.getNum();
            // error check
            ///assertNotEquals<int>( num, 1 ); // 0,2,3,4,... , num이 1일 수는 없다. insert에서 하나더 넣는다.
            // return value
            return num - 1;
        }
         int getDataNum()const{ return getPtrNum(); }
         int getPtrNum()const{
            return extendArray.getNum();
        }
    public:
        /// index번째의 key이후부터 나누므로 DATA는 index+1번째부터 나누어야한다.
        PtrArray<KEY,DATA,COMP,EE>* divide( const int index ) /**  index+ 1부터 divide.. 에러는 return NULL?? */
        {
            #ifdef SARRAYS_DEBUG
            checkUnderError( 1, index, 1 ); // must index >= 1
            checkOverError( 2, index, getPtrNum()-3 ); // getPtrNum >= 4
            #endif
            PtrArray<KEY,DATA,COMP,EE>* temp = new PtrArray<KEY,DATA,COMP,EE>( extendArray.getConstArray().size() );
            const int sIndex = index; const int eIndex = getPtrNum()-1;
            // // getKeyByIndex, getDataByIndex   // eIndex-1, eIndex-2 순서가 중요함!!
            temp->insert( extendArray.getConstArray()[eIndex].key, extendArray.getConstArray()[eIndex-1].data,
                         extendArray.getConstArray()[eIndex].data );
            for( int i = sIndex+1; i <= eIndex-2; i++ )
            {
                temp->insert( extendArray.getConstArray()[i].key, extendArray.getConstArray()[i].data, true );
            }

            /// use remove..
            for( int i = eIndex; i >= sIndex+1; i-- ) // 지울때는 뒤에서부터 지우는 게 더 낫다!
            {
                extendArray.removeFromIndex( extendArray.getDataNum()-1 ); // 뒤에서 부터 지운다!
            }
            //
            extendArray.getArray()[index].key = extendArray.getConstArray()[index-1].key;
            //
            return temp;
        }
    public: /// is~
        bool isEmpty()const{ return extendArray.isEmpty(); }
        bool isFull()const{ return extendArray.isFull(); }
        int getNum()const{ return extendArray.getNum(); }
        int getNumMax()const{ return extendArray.getNumMax(); } // getDataNumMax! cf KeyNumMax!
    public:
        const Array< SNode<KEY,DATA> >& getConstArray()const{ return extendArray.getConstArray(); }
};

}
#endif // SARRAYS_H_INCLUDED
