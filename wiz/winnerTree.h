#ifndef WINNERTREE_H_INCLUDED
#define WINNERTREE_H_INCLUDED


#include <wiz/wizardError.h>
#include <wiz/newArrays.h>

namespace wiz{
class WinnerTreeInitialError : public wiz::Error
{
public:
    WinnerTreeInitialError()
    : wiz::Error( "winner tree initial Error" )
    { }
};
class WinnerTreeTerminalSizeError : public wiz::Error
{
public:
    WinnerTreeTerminalSizeError()
    : wiz::Error( "terminals size must 2^n" )
    { }
};



/// warnning!
// tree->size()/2 == terminal`s size
// and terminal`s size must 2^n, n>=1, n is element of set N(Natural Number)
// before second~ game, you must call Update()
// before DoGame(), you must call Initial()
template <class T> // T must be WTItem<otherT>
class WinnerTree // more smaller one win
{
private:
    class WTItem // Winner Tree Item
    {
    public:
        T item;
        int terminalNo; // start at zero!
    public:
        explicit WTItem() : terminalNo( -2 )
        { }
        explicit WTItem( T item ) : item( item ), terminalNo( -1 )
        { }
    public:
        bool operator<=( const WTItem& otherItem )
        {
            return item <= otherItem.item;
        }
    };
private:
    WinnerTree( const WinnerTree<T>& );
    WinnerTree<T>& operator=( const WinnerTree<T>& );
private:
    Array< WTItem >* tree;
    bool isInitialed; //
    bool isFirst;
    int nextTerminalNo;
    // int terminalNum; //
public:
    explicit WinnerTree( const int terminalSize ) // 2^n n >= 1, const int n??
    : isInitialed( false ), tree( NULL ), isFirst( true ), nextTerminalNo( -1 )
    {
        wiz::checkUnderError( 0, terminalSize*2, 2 );

        // interal num 2*terminalSize-1 but,
            // we start 1, for easy programming. so add 1 ( index 0 is not used )
		// terminal Size must 2^n !
        bool isPowof2 = false;
        int mask = 1;

        for( int i=1; i < sizeof( int )*8; i++ ) {
            if( mask == terminalSize ) {
                isPowof2 = true;
                break;
            }
            mask = mask << 1; // *2
        }
        if( !isPowof2 )
        {
            throw wiz::Error( "WinnerTree, terminalSize must 2^n" );
        }

        tree = new Array< WTItem >( terminalSize*2 ); // tree[1] ~ tree[end]
    }
    virtual ~WinnerTree()
    {
        if( NULL != tree )
        {
            delete tree;
        }
    }
public:
    /// you must call Initial, before Update!
    // Initial( const Array<T>& terminals ); // used first initial?
    void Initial( const Array<T>& terminals )
    {
        const int terminalNum = tree->size()/2;

        //wiz::assertEquals( tree->size()/2, terminals.size() );

        for( int count=0; count < terminalNum; count++ )
        {
            const int index = terminalNum+count;
           (*tree)[index].item = terminals[count];
           (*tree)[index].terminalNo = count;
        }

        isInitialed = true;
    }
    // Update( int index, const T& val ); // used second~
        // if not initialed then throw WinnerTreeInitialError
    void NextUpdate( const T& val )
    {
        if( isInitialed )
        {
            const int index = (*tree)[0].terminalNo;
            if( index < 0 ) // 아직 DoGame()을 호출 하지 않았음...
            {
                throw wiz::Error( "you first call DoGame()!" );
            }

            const int terminalStart = tree->size()/2;

            (*tree)[terminalStart + index].item = val;
           // (*tree)[terminalStart + index].terminalNo = index;

            nextTerminalNo = index;
        }
        else
        {
            throw WinnerTreeInitialError();
        }
    }

    /// doGame && GetWinner!!
    // doGame(); // child( parent, LEFT ), child( parent, RIGHT )
    void DoGame()
    {
        if( !isInitialed )
        {
            throw WinnerTreeInitialError();
        }
        int startIndex = tree->size() / 4; // go first terminal`s parent.

        if( !isFirst )
        {
            startIndex = ( tree->size() / 2 + nextTerminalNo ) / 2;
        }

        while( startIndex >= 1 )
        {
            int countMax = startIndex;

            if( !isFirst )
            {
                countMax = 1;
            }
            // temp  ~ temp
            for( int count=0; count < countMax; count++ )
            {
                const int index = startIndex+count;
                const int leftChildIndex = index * 2;
                const int rightChildIndex = leftChildIndex + 1;


                if( (*tree)[leftChildIndex] <= (*tree)[rightChildIndex] )
                {
                    (*tree)[index] = (*tree)[leftChildIndex];
                }
                else
                {
                    (*tree)[index] = (*tree)[rightChildIndex];
                }
            }
            // temp /= 2
            startIndex /= 2;// go to parent..
        }

        // tree[0] = tree[1];
        (*tree)[0] = (*tree)[1];


        if( isFirst ) //O(n)
        {
            isFirst = false; // next time 2~
        }
    }

    // GetWinner()const;
        // Index 0 <- winner value!
    T GetWinner()const
    {
        if( !isInitialed )
        {
            throw WinnerTreeInitialError();
        }
        return (*tree)[0].item;
    }

    int GetWinnerPos()const
    {
        if( !isInitialed )
        {
            throw WinnerTreeInitialError();
        }
        return (*tree)[0].terminalNo;
    }
};

}
#endif // WINNERTREE_H_INCLUDED
