/// todo 2012.5.4 in GroupManager<T>, MakeGroupFromItems, RemoveEmptyGroups
// 2012.5.3 last Corrected., version 1.0
// 2012.5.1 created

/// chk Renewal!! remove하고 나서 다시 똑같은 key로 들어노는 경우등
// isExist, search에서 renewal를 해야한다면 renewal를 한다.

/// todo- op, op_offset set!, random!
/// groupMemberMax를 0으로 하면 그냥 두도록? and valid chk?( p==NULL, 초기화가 되지 않은 경우! )
/// chk - Do ( void (*f) ( Item<T>& item ), bool (*condition)( Item<T>& item, Group<T>& group ), const bool isOnlyItem );
// DON`T MAKE CYCLE!!

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

#include <conio.h>
#include <wiz/wizardError.h>
#include <wiz/wizSmartPointer.h> //


namespace wiz
{
	template <class T>
	class Item /// To Do Add 이동 생성자, 이동 대입 연산자..
	{
	private:
		string name; // key
		wiz::WizSmartPtr<T> value; // data
	public:
		explicit Item() : name("") { } // chk name="", 받아 들일 때!
		explicit Item(const string& name, const T& value)
			: name(name)
		{
			wiz::WizSmartPtr<T> temp(new T());
			this->value = temp;
			*(this->value) = value;
		}
		explicit Item(const string name, const wiz::WizSmartPtr<T>& value)
			: name(name)
		{
			this->value = value;
			// chk NULL !
		}
		virtual ~Item() { } // fixed! cout << "called" << endl; value.remove( true ); }
	public:
		bool empty() const { return value.isNULL(); }
		string getName()const { return name; }
		T getValue()const { return *value; }
		void setValue(const T& val) { *value = val; }
	};
	// ItemArray! name, (index, value)+ ?

	template <class T>
	class Group /// To Do Add 이동 생성자, 이동 대입 연산자.. // friend!!,일반적으로는 get만 가능하도록, friend를 통해서 set도 가능하게?
	{
    public:
        bool hasNoItem()const { return 0 == itemMemberN; }
        bool hasNoGroup()const { return 0 == groupMemberN; }
	public:
		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator itemBegin() { return itemMember.begin(); }
		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator itemEnd() { return itemMember.end(); }
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupBegin() { return groupMember.begin(); }
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupEnd() { return groupMember.end(); }
	private:
		string name;
		std::vector< wiz::WizSmartPtr< Item<T> > > itemMember;
		std::vector< wiz::WizSmartPtr< Group<T> > > groupMember;
		int itemMemberN; // for binary search!
		int groupMemberN; // for binary search!
	public:
		Group() { } // chk!! name = "", int =0, bool = false, 받아 들일 때!
		Group(const string& name, const int itemMemberMax, const int groupMemberMax)
			: name(name), itemMemberN(0), groupMemberN(0)
		{
			// chk if itemMemberN > 0 ?
			itemMember.reserve(itemMemberMax);
			groupMember.reserve(groupMemberMax);
		}
		virtual ~Group() /// chk!!!
		{ }
	public:
		string getName()const { return name; }
		int getItemMemberN()const { return itemMemberN; }
		int getGroupMemberN()const { return groupMemberN; }

		bool isItemExist(const string& itemName)
		{
			int index = searchItem(itemName);
			return index > -1;// fixed 2012.5.4 && !itemMember[index].isNULL();
		}
		bool isGroupExist(const string& groupName)
		{
			int index = searchGroup(groupName);
			return index > -1;// fixed 2012.5.4 && !groupMember[index].isNULL();
		}
		// bool isItemExist( const int index );
		// bool isGroupExist( const int index );
		bool getItem(const string& itemName, wiz::WizSmartPtr< Item<T> >& ptr)
		{
			bool isSuccess = isItemExist(itemName);

			if (isSuccess) /// chk!!
			{
				int index = searchItem(itemName);
				(ptr) = itemMember[index];
			}

			return isSuccess;
		}

		// void Do( void(*f)( Item<T>& ), bool (*condition)( Item<T>&, Group<T>& ) ); - chk!
		void Do(void(*f)(const Item<T>&), bool(*condition)(const Item<T>&, const Group<T>&), bool isOnlyItem) // int or enum! ( ALL, RANDOM- 00.00% )
		{
			/// todo- op, op_offset set!, random!
			// for item,
			for (int i = 0; i < itemMemberN; i++)
			{
				if (!itemMember[i].isNULL() && condition(*itemMember[i], *this))
				{
					f(*itemMember[i]);
				}
			}
			// for 가르키는 각 Group마다 Do를 한다.
			if (!isOnlyItem)
			{
				for (int i = 0; i < groupMemberN; i++)
				{
					if (!groupMember[i].isNULL())
					{
						groupMember[i]->Do(f, condition, isOnlyItem);
					}
				}
			}
		}

		void Do(void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&), bool isOnlyItem) // int or enum! ( ALL, RANDOM- 00.00% )
		{
			/// todo- op, op_offset set!, random!
			// for item,
			for (int i = 0; i < itemMemberN - 1; i++)
			{
				for (int j = i + 1; j < itemMemberN; j++)
				{
					if (!itemMember[i].isNULL() && condition(*itemMember[i], *itemMember[j], *this))
					{
						f(*itemMember[i], *itemMember[j]);
					}
				}
			}
			// for 가르키는 각 Group마다 Do를 한다.
			if (!isOnlyItem)
			{
				for (int i = 0; i < groupMemberN; i++)
				{
					if (!groupMember[i].isNULL())
					{
						groupMember[i]->Do(f, condition, isOnlyItem);
					}
				}
			}
		}


		bool addItem(wiz::WizSmartPtr< Item<T> >& item)
		{
			bool isSuccess = !isItemExist(item->getName()); //
															// cf, 있더라도, NULL인지 확인한다?

			if (isSuccess)
			{
				itemMember.push_back( item );

				itemMemberN++;

				// sort O(n) + O(n) !
				{
					renewalItem();

					int start = 0; int end = itemMemberN - 1;

					for (int pick = start + 1; pick <= end; pick++)
					{
						int current = pick;
						wiz::WizSmartPtr< Item<T> > temp = itemMember[pick]; //
						for (; (current > 0) && (itemMember[current - 1]->getName() > temp->getName()); current--)
						{
							itemMember[current] = itemMember[current - 1];
						}
						itemMember[current] = temp;
					}
				}
			}

			return isSuccess;
		}
		bool addGroup(wiz::WizSmartPtr< Group<T> >& group) // empty group!
		{
			bool isSuccess = !isGroupExist(group->getName());

			if (isSuccess)
			{
                groupMember.push_back( group );

				groupMemberN++;

				// sort
				{
					renewalGroup();

					int start = 0; int end = groupMemberN - 1;

					for (int pick = start + 1; pick <= end; pick++)
					{
						int current = pick;
						wiz::WizSmartPtr< Group<T> > temp = groupMember[pick]; //
						for (; (current > 0) && (groupMember[current - 1]->getName() > temp->getName()); current--)
						{
							groupMember[current] = groupMember[current - 1];
						}
						groupMember[current] = temp;
					}
				}
			}

			return isSuccess;
		}

		// bool getItemValue( const string itemName ); // todo
		bool setItemValue(const string& itemName, const T& value)
		{
			bool isSuccess = isItemExist(itemName);

			if (isSuccess) /// chk!!
			{
				int index = searchItem(itemName);
				itemMember[index]->setValue(value);
			}

			return isSuccess;
		}
		bool getGroup(const string& groupName, wiz::WizSmartPtr< Group<T> >& ptr)
		{
			bool isSuccess = isGroupExist(groupName);

			if (isSuccess) /// chk!!
			{
				int index = searchGroup(groupName);
				(ptr) = groupMember[index];
			}

			return isSuccess;
		}
		bool removeItem(const string& itemName, const bool bremove) // bremove - remove? delete?
		{
			bool isSuccess = isItemExist(itemName);

			if (isSuccess)
			{
				int index = searchItem(itemName);

				// cf) expand <> shrink?

				itemMember[index].remove(bremove);

				renewalItem(index);

				itemMember.pop_back();
			}

			return isSuccess;
		}

		bool removeGroup(const string& groupName, const bool bremove)
		{
			bool isSuccess = isGroupExist(groupName);

			if (isSuccess)
			{
				int index = searchGroup(groupName);

				// cf) expand <> shrink?

				groupMember[index].remove(bremove);

				renewalGroup(index);

				groupMember.pop_back();
			}

			return isSuccess;
		}
	private:
		void renewalItem(const int start = 0)
		{
			int count = start;
			for (int i = start; i < itemMemberN; i++)
			{
				if (!itemMember[i].isNULL())
				{
					itemMember[count] = std::move(itemMember[i]);
					count++;
				}
			}
			// null initial!
			for (int i = count; i < itemMemberN; i++)
			{
				wiz::WizSmartPtr< Item<T> > temp(NULL);
				itemMember[i] = temp;
			}

			itemMemberN = count;
		}
		void renewalGroup(const int start = 0)
		{
			int count = start;
			for (int i = start; i < groupMemberN; i++)
			{
				if (!groupMember[i].isNULL())
				{
					groupMember[count] = std::move(groupMember[i]);
					count++;
				}
			}
			// null initial!
			for (int i = count; i < groupMemberN; i++)
			{
				wiz::WizSmartPtr< Group<T> > temp(NULL);
				groupMember[i] = temp; /// bug fixed!! 2012.5.2 7.43PM - why?
			}

			groupMemberN = count;
		}


		int searchItem(const string& itemName) // return index! if index == -1 -> not exist
		{
			int index = -1;
			int left = 0; int right = itemMemberN - 1;
			int middle; string middleKey;

			while (left <= right)
			{
				middle = (left + right) >> 1; // more fast than ( left + right )/2!
				if (!(itemMember[middle].isNULL()))
				{
					middleKey = itemMember[middle]->getName();
					if (itemName == middleKey)
					{
						index = middle; break;
					}
					else if (itemName < middleKey)
					{
						right = middle - 1;
					}
					else
					{
						left = middle + 1;
					}
				}
				else
				{
					renewalItem();
					left = 0; right = itemMemberN - 1; // restart!
				}
			}

			return index;
		}
		int searchGroup(const string& groupName)
		{
			int index = -1;
			int left = 0; int right = groupMemberN - 1;
			int middle; string middleKey;

			while (left <= right)
			{
				middle = (left + right) >> 1; // more fast than ( left + right )/2!
				if (!groupMember[middle].isNULL())
				{
					middleKey = groupMember[middle]->getName();
					if (groupName == middleKey)
					{
						index = middle; break;
					}
					else if (groupName < middleKey)
					{
						right = middle - 1;
					}
					else
					{
						left = middle + 1;
					}
				}
				else
				{
					renewalGroup();
					left = 0; right = groupMemberN - 1; /// -1주의! index이므로 -1이 필요함!
				}
			}

			return index;
		}

	};

	template <class T>
	class GroupManager /// has functionObject table.
	{
	public:
        /// typename or class??
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupBegin() { return rootGroup->groupBegin(); }
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupEnd() { return rootGroup->groupEnd(); }

		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemBegin(const string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if( rootGroup->getGroup(groupName, temp) ) { if( temp->hasNoItem() ) { return temp->itemEnd(); } else { return temp->itemBegin(); } }
            throw wiz::Error( __FILE__, __LINE__, "groupItemBegin err" );
		}
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupBegin(const string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if( rootGroup->getGroup(groupName, temp) ) { if( temp->hasNoGroup() ) { return temp->groupEnd(); } else { return temp->groupBegin(); } }
            throw wiz::Error( __FILE__, __LINE__, "groupGroupBegin err" );
		}

		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemEnd(const string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if( rootGroup->getGroup(groupName, temp) )  { return temp->itemEnd(); }
			throw wiz::Error( __FILE__, __LINE__, "groupItemEnd err" );
		}
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupEnd(const string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if( rootGroup->getGroup(groupName, temp) ) { return temp->groupEnd(); }
			throw wiz::Error( __FILE__, __LINE__, "groupGroupEnd err" );
		}

	private:
		wiz::WizSmartPtr< Group<T> > rootGroup;
		string coreName;

	public:
		GroupManager(const string& coreName)
			: coreName(coreName)
		{
			wiz::WizSmartPtr< Group<T> > temp(new Group<T>(coreName, 100, 20)); // 1 or 0?
			rootGroup = temp;
		}
		virtual ~GroupManager() { rootGroup.remove(true); } //rootGroup.remove(); }
	public:
		string GetCoreName()const { return rootGroup->getName(); }

		bool IsItemExist(const string& itemName)
		{
			return rootGroup->isItemExist(itemName);
		}
		bool IsGroupExist(const string& groupName)
		{
			return rootGroup->isGroupExist(groupName);
		}

		bool NewItem(const string& itemName, const T& value)
		{
			wiz::WizSmartPtr< Item<T> > itemTemp(new Item<T>(itemName, value));

			/// add item to group
			if (rootGroup->addItem(itemTemp))
			{
				return true;
			}
			else
			{
				itemTemp.remove();
				return false;
			}
		}

		bool NewGroup(const string& groupName, const int itemMemberMax, const int groupMemberMax)
		{
			// 주의! 코어 이름과 다르게 한다!
			wiz::WizSmartPtr< Group<T> > temp(new Group<T>(groupName, itemMemberMax, groupMemberMax));

			if (coreName != groupName && rootGroup->addGroup(temp))
			{
				return true;
			}
			else
			{
				temp.remove();
				return false;
			}
		}
		bool AddItem(const string& itemName, wiz::WizSmartPtr< T >& ptr)
		{
			wiz::WizSmartPtr< Item<T> > itemTemp(new Item<T>(itemName, ptr));

			/// add item to group
			if (rootGroup->addItem(itemTemp))
			{
				return true;
			}
			else
			{
				itemTemp.remove();
				return false;
			}
		}
		bool AddItemToGroup(const string& itemName, const string& groupName)
		{
			/// find Group - name is groupName
			wiz::WizSmartPtr< Group<T> > groupTemp;
			wiz::WizSmartPtr< Item<T> > itemTemp;
			// get Group Ptr
			if (rootGroup->getGroup(groupName, groupTemp)
				&& rootGroup->getItem(itemName, itemTemp))
			{
				return groupTemp->addItem(itemTemp);
			}
			else
			{
				return false;
			}
		}
		bool AddGroupToGroup(const string& groupName1, const string& groupName2) // from 1, to 2
		{
			/// find Group - name is groupName
			wiz::WizSmartPtr< Group<T> > groupTemp1;
			wiz::WizSmartPtr< Group<T> > groupTemp2;
			// get Group Ptr
			if (rootGroup->getGroup(groupName1, groupTemp1)
				&& rootGroup->getGroup(groupName2, groupTemp2))
			{
				return groupTemp2->addGroup(groupTemp1);
			}
			else
			{
				return false;
			}
		}
		bool GetItem(const string& itemName, Item<T>& itemPtr)
		{
			wiz::WizSmartPtr< Item<T> > ptr;

			if (rootGroup->getItem(itemName, ptr))
			{
				itemPtr = *ptr;
				return true;
			}
			else
			{
				return false;
			}
		}
		bool GetValue(const string& itemName, T& value)
		{
			Item<T> itemPtr;
			if (GetItem(itemPtr))
			{
				value = itemPtr.getValue();
				return true;
			}
			else
			{
				return false;
			}
		}
		bool SetValue(const string& itemName, const T value)
		{
			return rootGroup->setItemValue(itemName, value);
		}

		bool RemoveItem(const string& itemName) // must true!
		{
			return rootGroup->removeItem(itemName, true);
		}
		bool RemoveGroup(const string& groupName) // must true!
		{
			return rootGroup->removeGroup(groupName, true);
		}
		bool RemoveItemFromGroup(const string& itemName, const string& groupName, const bool bRemove = false) // maybe false
		{
			/// find Group - name is groupName
			wiz::WizSmartPtr< Group<T> > groupTemp;
			// get Group Ptr
			if (rootGroup->getGroup(groupName, groupTemp))
			{
				return groupTemp->removeItem(itemName, bRemove);
			}
			else
			{
				return false;
			}
		}
		bool RemoveGroupFromGroup(const string& groupName1, const string& groupName2, const bool bRemove = false) // maybe false
		{
			/// find Group - name is groupName
			wiz::WizSmartPtr< Group<T> > groupTemp;
			// get Group Ptr
			if (rootGroup->getGroup(groupName2, groupTemp))
			{
				return groupTemp->removeGroup(groupName1, bRemove);
			}
			else
			{
				return false;
			}
		}

		// 중복된 것은 여러 번 실행 된다. - solution??
		/// int functionNo ??
		// functionObject table??
		//
		bool Do(const string& groupName, void(*f)(const Item<T>&), bool(*condition)(const Item<T>&, const Group<T>&)) // int or enum! ( ALL, RANDOM- 00.00% )
		{
			wiz::WizSmartPtr< Group<T> > ptr;

			if (rootGroup->getGroup(groupName, ptr))
			{
				ptr->Do(f, condition, false);
				return true;
			}
			else
			{
				return false;
			}
		}

		// 같은 Group에 속한 Element끼리?
		bool Do(const string& groupName, void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&))
		{
			wiz::WizSmartPtr< Group<T> > ptr;

			if (rootGroup->getGroup(groupName, ptr))
			{
				ptr->Do(f, condition, false);
				return true;
			}
			else
			{
				return false;
			}
		}
		// 바로 가지고 있는 Item에 대해서만 한다!
		void DoAll(void(*f)(const Item<T>&), bool(*condition)(const Item<T>&, const Group<T>&))//DoAll( ~, groupName ); // 이름으로 구분한다? = groupName-Local?
		{
			rootGroup->Do(f, condition, true);
		}
		void DoAll(void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&))
		{
			rootGroup->Do(f, condition, true);
		}
		/*
		private:
		bool AddGroup( const string& groupName, const int itemMemberMax, const int groupMemberMax )
		{
		wiz::WizSmartPtr< Group<T> > temp( new Group<T>( groupName, itemMemberMax, groupMemberMax ) );
		if( rootGroup->addGroup( temp ) )
		{
		return true;
		}
		else
		{
		temp.remove();
		return false;
		}
		}*/
		// void Do( const string& cmd );
	};
}

/*
void print(const Item<int>& item)
{
	cout << item.getName() << " " << item.getValue() << endl;
}

class Number
{
public:
	virtual void f() { cout << "Number" << endl; }
};

class Int : public Number
{
public:
	virtual void f() { cout << "Int" << endl; }
};

template <class T>
inline bool ALWAYS(const Item<T>& item, const Group<T>& group)
{
	return true;
}
template <class T>
inline bool NEVER(const Item<T>& item, const Group<T>& group)
{
	return false;
}
template <class T>
inline bool random50(const Item<T>& item, const Group<T>& group)
{
	return rand() % 2;
}

template <class T>
inline bool random50(const Item<T>& item1, const Item<T>& item2, const Group<T>& group)
{
	return rand() % 2;
}
template <class T>
inline bool ALWAYS(const Item<T>& item1, const Item<T>& item2, const Group<T>& group)
{
	return true;
}
template <class T>
inline bool NEVER(const Item<T>& item1, const Item<T>& item2, const Group<T>& group)
{
	return false;
}
void f(const Item<Number>& item)
{
	item.getValue().f();
}
void g(const Item<Number>& item1, const Item<Number>& item2)
{
	cout << item1.getName() << " " << item2.getName() << endl;
	cout << "meet" << endl;
}


int main(void)
{
	// for random,
	srand(time(NULL));

	GroupManager<Number> m("NumberCore");
	m.NewItem("a", Number()); // addItem( WizSmartPointer<Number>( new Int() ) );
	m.NewItem("c", Number());
	wiz::WizSmartPtr<Number> n(new Int());
	m.AddItem("b", n);
	n.remove(false);

	m.NewGroup("school", 10, 2);
	m.AddItemToGroup("a", "school");
	m.AddItemToGroup("b", "school");
	m.NewGroup("student", 10, 2);
	m.AddItemToGroup("c", "student");
	m.AddGroupToGroup("student", "school");

	// select! one, one? total or random 몇번?
	m.Do("school", g, ALWAYS); // m.AllDo( g, RANDOM, );

	cout << "allDo test" << endl;

	m.DoAll(g, ALWAYS);
	//Item&, Item&, onlySameGroup

	GroupManager<int> mgl("IntCore");

	mgl.NewGroup("AAA", 30, 1); // no group mode?
	mgl.NewGroup("BBB", 26, 1);
	// 소문자.
	for (char c = 'z'; c >= 'a'; c--)
	{
		mgl.NewItem(string("") + c, c - 'a');
	}
	for (char c = 'z'; c >= 'a'; c--)
	{
		Item<int> temp;
		mgl.GetItem(string("") + c, temp);
		mgl.AddItemToGroup(string("") + c, "AAA");

		//cout << temp.getName() << " " << temp.getValue() << endl;
	}
	// 대문자
	for (char c = 'A'; c <= 'Z'; c++)
	{
		mgl.NewItem(string("") + c, c - 'a');
	}
	for (char c = 'A'; c <= 'Z'; c++)
	{
		Item<int> temp;
		mgl.GetItem(string("") + c, temp);
		mgl.AddItemToGroup(string("") + c, "BBB");

		//cout << temp.getName() << " " << temp.getValue() << endl;
	}
	mgl.NewGroup("CCC", 26, 1);
	mgl.AddGroupToGroup("AAA", "CCC");
	mgl.AddGroupToGroup("BBB", "CCC");

	cout << "do" << endl;
	// Do
	mgl.Do("CCC", print, ALWAYS); // 소문자 Group에서 접근 할 수 있는 모든 Item에 대해서 print를 실행한다!
	cout << "remove test" << endl;

	mgl.RemoveGroupFromGroup("AAA", "CCC", false);


	mgl.Do("CCC", print, ALWAYS);
	cout << "---------------------------" << endl;
	cout << mgl.IsGroupExist("AAA") << endl;
	cout << "continued remove test" << endl;
	Item<int> temp;
	if (mgl.GetItem("C", temp))
		print(temp);
	//mgl.DoAll( print );
	cout << "end" << endl;

	wiz::WizSmartPtr<int> x(new int);
	*x = 3;
	cout << *x << endl;
	x = x;
	cout << *x << endl;
	const wiz::WizSmartPtr<int> y = x;

	x.remove(false);
	cout << x.isNULL() << endl;
	cout << *y << " " << y.isNULL() << endl;
	int zz = 0;

	if ((0) || (zz++)) { cout << "zz is 1 " << endl; }

	//getch();
    try{
        /// test
        // A-Z // mgl.Do( "CCC", print, ALWAYS ); // 소문자 Group에서 접근 할 수 있는 모든 Item에 대해서 print를 실행한다!
        auto CCCend = mgl.groupGroupEnd("CCC"); // cf CCC is empty?
        for (auto _x = mgl.groupGroupBegin("CCC"); _x != CCCend; ++_x)
        {
            cout << (*_x)->getName() <<endl;
            auto CCCend2 = (*_x)->itemEnd();
            for( auto _y = (*_x)->itemBegin(); _y != CCCend2; ++_y )
            {
                cout << (*_y)->getValue() << endl;
            }
            cout <<"-----" <<endl;
        }
	}catch(wiz::Error e ) {
        cout << e <<endl;
	}

	return 0;
}
*/
