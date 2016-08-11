#ifndef WIZSMARTPOINTER_H_INCLUDED
#define WIZSMARTPOINTER_H_INCLUDED

//#include <wiz/wizardError.h>
/// Warning!
// do not make WizSmartPtr of WizSmartPtr of T(template)
// do not make Cycle of WizSmartPtrs
//  -  서로가 서로를 가르키게 되면, 안 지워질 수 있다.
/// cf) new AAA[num];
///#define WizSmartPtr_DEBUG ??
#include <wiz/global.h>

namespace wiz{
template < class T, class IS_SAME_VALUE=wiz::EE<T> >
class WizSmartPtr /// To Do - do NOT? 이동생성자, 이동대입연산자.
{
private:
    WizSmartPtr<T, IS_SAME_VALUE>* left;
    WizSmartPtr<T, IS_SAME_VALUE>* right;
    T* ptr;
	int option;
public:
    const WizSmartPtr<T, IS_SAME_VALUE>* getThis()const { return this; }
private:
    void doNULL( WizSmartPtr<T, IS_SAME_VALUE>* sp )
    {
        sp->ptr = NULL;
        sp->left = sp;
        sp->right = sp;
    }
    void quit()
    {
        right->left = this->left;
        left->right = this->right;
        doNULL( this );
    }

    void enter( const WizSmartPtr<T, IS_SAME_VALUE>& sp )
    {
        //wizard::assertNotNULL( sp );
        //wizard::assertNULL( this->ptr );

        if( NULL == sp.ptr )
        {
            doNULL( this );
        }
        else if( NULL == this->ptr )
        {
            this->ptr = sp.ptr;
			this->option = sp.option; ///
            this->right = const_cast<WizSmartPtr<T, IS_SAME_VALUE>*>( sp.getThis() );
            this->left = sp.left;
            this->left->right = this;
            this->right->left = this;
        }
    }

    void initFromOther( const WizSmartPtr<T, IS_SAME_VALUE>& sp )
    {
        //wizard::assertNotEquals( this, &sp );
       // if( this == sp.getThis() ) { return; } // pass 한다.

        // delete or quit linked list of this
        if( NULL != this->ptr )
        {
            if( isOnlyOne() ) //
            {
				remove();
            }
            else
            {
                quit();
            }
        }

        // enter linked list of sp
        enter( sp );

        return;
    }
public:
    explicit
	WizSmartPtr( T* ptr = NULL)
    : ptr( ptr ), option(0)
	{
		left = this; right = this;
	}
	explicit
	WizSmartPtr(T* ptr, const int option) // option 1,2,..
		: ptr( ptr ), option(option)
	{
		left = this; right = this; 
	}
    WizSmartPtr( const WizSmartPtr<T, IS_SAME_VALUE>& sp )
		: ptr( NULL ), option(sp.option)
    {
		left = this; right = this;

        initFromOther( sp );
	}
	virtual ~WizSmartPtr() /// virtual??
	{
		if (isOnlyOne())
		{
			remove(true);
		}
		else
		{
			remove(false);
		}
	}
public:
    WizSmartPtr<T, IS_SAME_VALUE>& operator=( const WizSmartPtr<T, IS_SAME_VALUE>& _sp )
    {
        // temp link
        WizSmartPtr<T, IS_SAME_VALUE> tempLink( _sp );

        initFromOther( tempLink );

        return *this;
    }
    T& operator*()
    {
        // wizard::assertNotNULL( ptr );
        return (*ptr);
    }
    const T& operator*()const
    {
        // wizard::assertNotNULL( ptr );
        return (*ptr);
    }
public:
    bool isOnlyOne()const
    {
        return ( this == left ) && ( this == right ); // && NULL != ptr ?
    }
    bool isNULL()const
    {
        return NULL == ptr;
    }
    bool empty()const
    {
        return isNULL();
    }
    /// remove return suceess?
    bool remove()
    {
        return remove( true );
    }
 	bool remove( const bool bremove ) // make private and delete =true, and make public remove() call remove( true ); - 2012.3.5 todo...
    {
        if( empty() ) { return false; }
        if( !bremove && isOnlyOne() ) { return false; } /// 2013.08.13 false means "no change"??
		if (NULL != ptr && bremove)
		{
			delete ptr; ptr = NULL;

            // reset all linked list`s ptr -> NULL
            WizSmartPtr<T, IS_SAME_VALUE>* temp = this->right;
            WizSmartPtr<T, IS_SAME_VALUE>* temp2;
            // doNULL others
			while( temp != this )
            {
				temp2 = temp;
                temp = temp->right;
                doNULL( temp2 );
            }
            //doNULL this
            doNULL( this );
        }
        else
        {
            //// 2012.3.5 todo  - make function - "quit" linkedList of this, "enter" linkeList of other
            // quit linked list of this
            quit();
        }
        return true;
    }
public:
    const T* operator->()const
    {
        //wizard::assertNotNULL( ptr );
        return ptr;
    }
    T* operator->()
    {
        //wizard::assertNotNULL( ptr );
        return ptr;
    }
    T* operator&()
    {
        return ptr;
    }
    const T* operator&()const
    {
        return ptr;
    }
    ///
public:
    bool operator!=( const WizSmartPtr<T, IS_SAME_VALUE>& wsp ) const
    {
        return (*ptr) != (*(wsp.ptr));
    }
    bool operator==( const WizSmartPtr<T, IS_SAME_VALUE>& wsp ) const ///
    {
        return (*ptr) == (*(wsp.ptr));
    }
    bool isSameValue( const WizSmartPtr<T, IS_SAME_VALUE>& wsp ) const /// 같은 값을 가지고 있는가?
    {
        return IS_SAME_VALUE()( (*ptr), (*wsp.ptr) );
    }
	bool hasSameObject(const WizSmartPtr<T, IS_SAME_VALUE>& wsp) const
	{
		return ptr == wsp.ptr;
	}
};

}

#endif // WIZSMARTPOINTER_H_INCLUDED
