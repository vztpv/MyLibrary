
#ifndef MYGROUPMANAGER_H
#define MYGROUPMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <wiz/wizardError.h>
#include <wiz/wizSmartPointer.h> //

namespace wiz{

namespace MGM // My Group Manager
{
	template <class T>
	class Item /// To Do Add �̵� ������, �̵� ���� ������..
	{
	private:
		std::string name; // key
		wiz::WizSmartPtr<T> value; // data
	public:
		explicit Item() : name("") { } // chk name="", �޾� ���� ��!
		explicit Item(const std::string& name, const T& value)
			: name(name)
		{
			wiz::WizSmartPtr<T> temp(new T());
			this->value = temp;
			*(this->value) = value;
		}
		explicit Item(const std::string name, const wiz::WizSmartPtr<T>& value)
			: name(name)
		{
			this->value = value;
			// chk NULL !
		}
		virtual ~Item() { } // fixed! cout << "called" << endl; value.remove( true ); }
	public:
		bool empty() const { return value.isNULL(); }
		std::string getName()const { return name; }
		T getValue()const { return *value; }
		void setValue(const T& val) { *value = val; }
	};
	// ItemArray! name, (index, value)+ ?

	template <class T>
	class Group /// To Do Add �̵� ������, �̵� ���� ������.. // friend!!,�Ϲ������δ� get�� �����ϵ���, friend�� ���ؼ� set�� �����ϰ�?
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
		std::string name;
		std::vector< wiz::WizSmartPtr< Item<T> > > itemMember;
		std::vector< wiz::WizSmartPtr< Group<T> > > groupMember;
		int itemMemberN; // for binary search!
		int groupMemberN; // for binary search!
	public:
		Group() { } // chk!! name = "", int =0, bool = false, �޾� ���� ��!
		Group(const std::string& name, const int itemMemberMax, const int groupMemberMax)
			: name(name), itemMemberN(0), groupMemberN(0)
		{
			// chk if itemMemberN > 0 ?
			itemMember.reserve(itemMemberMax);
			groupMember.reserve(groupMemberMax);
		}
		virtual ~Group() /// chk!!!
		{ }
	public:
		std::string getName()const { return name; }
		int getItemMemberN()const { return itemMemberN; }
		int getGroupMemberN()const { return groupMemberN; }

		bool isItemExist(const std::string& itemName)
		{
			int index = searchItem(itemName);
			return index > -1;// fixed 2012.5.4 && !itemMember[index].isNULL();
		}
		bool isGroupExist(const std::string& groupName)
		{
			int index = searchGroup(groupName);
			return index > -1;// fixed 2012.5.4 && !groupMember[index].isNULL();
		}
		// bool isItemExist( const int index );
		// bool isGroupExist( const int index );
		bool getItem(const std::string& itemName, wiz::WizSmartPtr< Item<T> >& ptr)
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
			// for ����Ű�� �� Group���� Do�� �Ѵ�.
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
			// for ����Ű�� �� Group���� Do�� �Ѵ�.
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
															// cf, �ִ���, NULL���� Ȯ���Ѵ�?

			if (isSuccess)
			{
				itemMember.push_back(item);

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
				groupMember.push_back(group);

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

		// bool getItemValue( const std::string itemName ); // todo
		bool setItemValue(const std::string& itemName, const T& value)
		{
			bool isSuccess = isItemExist(itemName);

			if (isSuccess) /// chk!!
			{
				int index = searchItem(itemName);
				itemMember[index]->setValue(value);
			}

			return isSuccess;
		}
		bool getGroup(const std::string& groupName, wiz::WizSmartPtr< Group<T> >& ptr)
		{
			bool isSuccess = isGroupExist(groupName);

			if (isSuccess) /// chk!!
			{
				int index = searchGroup(groupName);
				(ptr) = groupMember[index];
			}

			return isSuccess;
		}
		bool removeItem(const std::string& itemName, const bool bremove) // bremove - remove? delete?
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

		bool removeGroup(const std::string& groupName, const bool bremove)
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


		int searchItem(const std::string& itemName) // return index! if index == -1 -> not exist
		{
			int index = -1;
			int left = 0; int right = itemMemberN - 1;
			int middle; std::string middleKey;

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
		int searchGroup(const std::string& groupName)
		{
			int index = -1;
			int left = 0; int right = groupMemberN - 1;
			int middle; std::string middleKey;

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
					left = 0; right = groupMemberN - 1; /// -1����! index�̹Ƿ� -1�� �ʿ���!
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

		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemBegin(const std::string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if (rootGroup->getGroup(groupName, temp)) { if (temp->hasNoItem()) { return temp->itemEnd(); } else { return temp->itemBegin(); } }
			throw wiz::Error(__FILE__, __LINE__, "groupItemBegin err");
		}
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupBegin(const std::string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if (rootGroup->getGroup(groupName, temp)) { if (temp->hasNoGroup()) { return temp->groupEnd(); } else { return temp->groupBegin(); } }
			throw wiz::Error(__FILE__, __LINE__, "groupGroupBegin err");
		}

		typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemEnd(const std::string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if (rootGroup->getGroup(groupName, temp)) { return temp->itemEnd(); }
			throw wiz::Error(__FILE__, __LINE__, "groupItemEnd err");
		}
		typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupEnd(const std::string& groupName)
		{
			wiz::WizSmartPtr< Group<T> > temp;
			if (rootGroup->getGroup(groupName, temp)) { return temp->groupEnd(); }
			throw wiz::Error(__FILE__, __LINE__, "groupGroupEnd err");
		}

	private:
		wiz::WizSmartPtr< Group<T> > rootGroup;
		std::string coreName;

	public:
		GroupManager(const std::string& coreName)
			: coreName(coreName)
		{
			wiz::WizSmartPtr< Group<T> > temp(new Group<T>(coreName, 100, 20)); // 1 or 0?
			rootGroup = temp;
		}
		virtual ~GroupManager() { rootGroup.remove(true); } //rootGroup.remove(); }
	public:
		std::string GetCoreName()const { return rootGroup->getName(); }

		bool IsItemExist(const std::string& itemName)
		{
			return rootGroup->isItemExist(itemName);
		}
		bool IsGroupExist(const std::string& groupName)
		{
			return rootGroup->isGroupExist(groupName);
		}

		bool NewItem(const std::string& itemName, const T& value)
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

		bool NewGroup(const std::string& groupName, const int itemMemberMax, const int groupMemberMax)
		{
			// ����! �ھ� �̸��� �ٸ��� �Ѵ�!
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
		bool AddItem(const std::string& itemName, wiz::WizSmartPtr< T >& ptr)
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
		bool AddItemToGroup(const std::string& itemName, const std::string& groupName)
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
		bool AddGroupToGroup(const std::string& groupName1, const std::string& groupName2) // from 1, to 2
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
		bool GetItem(const std::string& itemName, Item<T>& itemPtr)
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
		bool GetValue(const std::string& itemName, T& value)
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
		bool SetValue(const std::string& itemName, const T value)
		{
			return rootGroup->setItemValue(itemName, value);
		}

		bool RemoveItem(const std::string& itemName) // must true!
		{
			return rootGroup->removeItem(itemName, true);
		}
		bool RemoveGroup(const std::string& groupName) // must true!
		{
			return rootGroup->removeGroup(groupName, true);
		}
		bool RemoveItemFromGroup(const std::string& itemName, const std::string& groupName, const bool bRemove = false) // maybe false
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
		bool RemoveGroupFromGroup(const std::string& groupName1, const std::string& groupName2, const bool bRemove = false) // maybe false
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

		// �ߺ��� ���� ���� �� ���� �ȴ�. - solution??
		/// int functionNo ??
		// functionObject table??
		//
		bool Do(const std::string& groupName, void(*f)(const Item<T>&), bool(*condition)(const Item<T>&, const Group<T>&)) // int or enum! ( ALL, RANDOM- 00.00% )
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

		// ���� Group�� ���� Element����?
		bool Do(const std::string& groupName, void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&))
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
		// �ٷ� ������ �ִ� Item�� ���ؼ��� �Ѵ�!
		void DoAll(void(*f)(const Item<T>&), bool(*condition)(const Item<T>&, const Group<T>&))//DoAll( ~, groupName ); // �̸����� �����Ѵ�? = groupName-Local?
		{
			rootGroup->Do(f, condition, true);
		}
		void DoAll(void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&))
		{
			rootGroup->Do(f, condition, true);
		}
		/*
		private:
		bool AddGroup( const std::string& groupName, const int itemMemberMax, const int groupMemberMax )
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
		// void Do( const std::string& cmd );
	};
}

}

#endif