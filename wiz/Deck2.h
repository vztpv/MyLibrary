#ifndef DECK2_H_INCLUDED
#define DECK2_H_INCLUDED

#include <wiz/wizardError.h>

namespace wiz{

	template <class T>
	class Deck2_Node{ /// deck2_node?
	public:
		T key;
		Deck2_Node<T>* left;
		Deck2_Node<T>* right;
	public:
		explicit Deck2_Node(T key = T()) : key(key), left(this), right(this) { }
	};

	template <class T, class NODE = Deck2_Node<T> > /// node->left, right -> default - node..
	class Deck2 /// rename Deck2
	{
	private: /// protected??
		NODE* p;
		int N;
	private:
		Deck2<T, NODE>& operator=(Deck2<T, NODE>& _deck);
		Deck2(Deck2<T, NODE>& _deck);
	public:
		explicit Deck2() : p(NULL), N(0) { }
		Deck2(initializer_list<T> args) // : Deck()
		{
			p = NULL; N = 0;
			for (T _x : args)
			{
				push_back(_x);
			}
		}

		virtual ~Deck2() {
			while (!empty()) {
				pop_back();
			}
		} /// To Do...
	public:
		void push_back(const T& key)
		{
			if (NULL == p) {
				p = new NODE(key);
			}
			else
			{
				NODE* temp = new NODE(key);
				temp->left = p->left;
				temp->left->right = temp;
				p->left = temp;
				temp->right = p;
			}
			N++;
		}
		void push_back(NODE* temp) /// move to deck3?
		{
			if (NULL == p) {
				p = temp;
			}
			else
			{
				temp->left = p->left;
				temp->left->right = temp;
				p->left = temp;
				temp->right = p;
			}
			N++;
		}
		void push_front(const T& key)
		{
			if (NULL == p) {
				p = new NODE(key);
			}
			else
			{
				NODE* temp = new NODE(key);
				temp->right = p;
				temp->left = p->left;
				temp->left->right = temp;
				p->left = temp;
				p = temp;
			}
			N++;
		}
		void push_front(NODE* temp)
		{
			if (NULL == p) {
				p = temp;
			}
			else
			{
				temp->right = p;
				temp->left = p->left;
				temp->left->right = temp;
				p->left = temp;
				p = temp;
			}
			N++;
		}
		T pop_back()
		{
			if (empty()) {
				throw wiz::Error("pop_back :err");
			}
			NODE* temp = p->left;

			temp->left->right = p;
			p->left = temp->left;
			T key = temp->key;

			delete temp;
			N--; if (N == 0) {
				p = NULL;
			}
			return key;
		}
		T pop_front() {
			if (empty()) {
				throw wiz::Error("pop_front :err");
			}
			NODE* temp = p;

			temp->left->right = p->right;
			p->right->left = temp->left;
			p = p->right;

			T key = temp->key;
			delete temp;
			N--; if (N == 0) {
				p = NULL;
			}
			return key;
		}
		NODE* pop_front_node() {
			if (empty()) {
				return NULL; // throw wiz::Error("pop_front :err");
			}
			NODE* temp = p;

			temp->left->right = p->right;
			p->right->left = temp->left;
			p = p->right;
			N--;
			if (N == 0) {
				p = NULL;
			}
			return temp;

		}
		int size()const {
			return N;
		}
		bool empty()const
		{
			return 0 == N;
		}
		void remove(const T& key, bool __delete = false) /// if exist, remove only one node...
		{
			NODE* temp = p;
			while (temp)
			{
				if (temp->key == key){
					remove(temp, __delete);
					return;
				}
				temp = temp->right;
				if (temp == p) {
					return;
				}
			}
		}

		void remove(NODE* node, bool __delete = false)
		{
			NODE* temp = node;
			if (temp) {
				if (p == node) {
					p = p->right;
				}

				temp->left->right = temp->right;
				temp->right->left = temp->left;

				//temp->left = temp;
				//temp->right = temp;

				if (__delete) {
					delete temp;
				}

				N--; if (0 == N) {
					p = NULL;
				}
			}
		}
		void deck_union(Deck2<T, NODE>& deck) /// deck=>empty...
		{
			while (!deck.empty())
				push_back(deck.pop_front_node());
		}
		const NODE* first()const{
			return p;
		}
		NODE* first()
		{
		    return p;
		}
		/// for debug - remove??
		void Print()const
		{
			NODE* temp = p;

			while (true)
			{
				if (!temp) {
					return;
				}
				cout << temp->key  << " ";
                if (temp->child){
					cout << "={ ";
					temp->child->Print();
                    cout << " } ";
				}
				temp = temp->right;

				if (temp == p)
				{
					return;
				}
			}
		}
		/// following methods used in Node that has child!
		int TotalNum()const
		{
			int sum = 0;
			TotalNum(p, &sum);
			return sum;
		}

		bool TotalExist(const NODE* x)const
		{
			bool chk = false;
			TotalExist(p, x, &chk);
			return chk;
		}

	private:
		void TotalNum( const NODE* x, int* sum )const
		{
			NODE* temp = const_cast<NODE*>( x );

			while (true)
			{
				if (!temp) {
					return;
				}
				(*sum)++;
				if (temp->child){
					temp->child->TotalNum( temp->child->first(), sum );
				}
				temp = temp->right;

				if (temp == x)
				{
					return;
				}
			}
		}
		void TotalExist(const NODE* z, const NODE* x, bool* bval)const
		{
			NODE* temp = const_cast<NODE*>( z );

			while (true)
			{
				if (!temp) {
					return;
				}
				if (*bval) {
					return;
				}
				if (temp == x) {
					*bval = true;
					return;
				}
				if (temp->child) {
					temp->child->TotalExist( temp->child->first(), x, bval);
				}
				if (*bval) {
					return;
				}
				temp = temp->right;

				if (temp == z)
				{
					return;
				}
			}
		}
	};
}

#endif // DECK2_H_INCLUDED
