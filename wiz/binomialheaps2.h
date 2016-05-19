#ifndef BINOMIALHEAPS_H_INCLUDED
#define BINOMIALHEAPS_H_INCLUDED

#include <wiz/global.h>


// use with WrapForInfinity!!
/// need more test!!, algorithms book chk..
/// public, private chk..
class Vertex;

namespace wiz{
 namespace BH2{ /// rename? for dijkstra algorithm...
	template <class T>
	class BinomialHeap{
	public:
		class BElement
		{
		public:
			explicit BElement(const T& val, const int no=-1) : no(no) { /// , int no=-1?
				key = val;
				parent = NULL; child = NULL; sibling = NULL;
				degree = 0;
			}
		public:
			BElement* parent;
			T key; // usually int..
			int degree;
			BElement* child;
			BElement* sibling;
			int no;
		};
	private:
		explicit BinomialHeap(){
			BinomialHeap::initial();
		}
        /**
		* DONE 2014.05.08
		* 복사, 대입 금지..
		*/
		BinomialHeap(const BinomialHeap<T>&);
		BinomialHeap<T>& operator=(const BinomialHeap<T>&);
	public:
		int size()const{
			return n;
		}
		bool empty()const{
			return NULL == head;
		}
	public:
		static void Insert(BinomialHeap<T>** H, T key,int no)
		{
			Insert(H, new BElement(key,no));
		}
		static T Extract(BinomialHeap<T>** H, Array<Vertex>& arr, const T& TINFINITY){
			BElement* temp = ExtractMinimum(H, TINFINITY);
			if( !temp ) { throw wiz::Error( __FILE__,__LINE__, "extract err" ); }
			T key = temp->key;
			arr[temp->no].node = NULL;
			delete temp;
			return key;
		}
		static void PrintInorder(BinomialHeap<T>* heap){ /// remove?
			PrintInorder(heap->head);
		}
		static BinomialHeap<T>* MakeHeap(){ // 주의  new한 포인터를 리턴..
			return new BinomialHeap<T>();
		}
		static T GetMinimum(BinomialHeap<T>* heap, const T& TINFINITY)
		{
			return Minimum(heap, TINFINITY)->key;
		}
		// make Union, using Merge
		// 받아 들인 두 개의 Heap의 Head가 없어진다?
		static BinomialHeap<T>* Union(BinomialHeap<T>** H1, BinomialHeap<T>** H2){ /// cf) case H1 or H2 == NULL...
			BinomialHeap<T>* tempHeap;
			BElement *prev_x, *x, *next_x;
			if (*H1 == *H2) {
				throw wiz::Error(__FILE__, __LINE__, "Union_err-two same heaps"); /// err?
			}
			tempHeap = BinomialHeap<T>::MakeHeap();
			if (*H1 && *H2) {
				tempHeap->head = Merge((*H1)->head, (*H2)->head); (*H1)->head = NULL;  (*H2)->head = NULL;
				tempHeap->n = (*H1)->n + (*H2)->n;
				(*H1)->n = 0; (*H2)->n = 0;
			}
			else if (NULL == *H1 && NULL != *H2) {
				tempHeap->head = (*H2)->head; (*H2)->head = NULL;
				tempHeap->n = (*H2)->n; (*H2)->n = 0;
			}
			else if (NULL == *H2 && NULL != *H1) {
				tempHeap->head = (*H1)->head; (*H1)->head = NULL;
				tempHeap->n = (*H1)->n;(*H1)->n = 0;
			}
			else {
				tempHeap->head = NULL; tempHeap->n = 0;
			}
			/// free the objects H1 and H2 but not the lists they point to
			if (*H1) delete *H1;
			if (*H2) delete *H2;
			*H1 = NULL; *H2 = NULL;

			if (tempHeap->head == NULL){
				return tempHeap;
			}
			prev_x = NULL;
			x = tempHeap->head;
			next_x = x->sibling;
			//
			while (next_x != NULL){
				if (x->degree != next_x->degree ||
					(next_x->sibling != NULL &&
					next_x->sibling->degree == x->degree))
				{
					prev_x = x;//
					x = next_x;
				}
				else if (x->key <= next_x->key){
					x->sibling = next_x->sibling;
					Link(next_x, x);
				}
				else {
					if (prev_x == NULL){
						tempHeap->head = next_x;
					}
					else{
						prev_x->sibling = next_x;
					}
					Link(x, next_x);
					x = next_x;
				}
				next_x = x->sibling;
			}

			return tempHeap;
		}
		static void PrintInorder(BElement* heap){ /// remove??
			if (heap == NULL){
				return;
			}
			PrintInorder(heap->child);
			cout << heap->key.val << endl;
			PrintInorder(heap->sibling);
		}
		// return BElement ( that has minimum key. )
		static BElement* Minimum(BinomialHeap<T>* heap, const T& TINFINITY){
			BElement* y = NULL; /** result 최소값을 갖고잇는 Belement의 포인터 */
			BElement* x = heap->head;
			T min = TINFINITY;
			//
			while (x != NULL){
				if (x->key < min){
					min = x->key;
					y = x;
				}
				x = x->sibling;
			}
			return y;
		}
    private:
		static BElement* Link(BElement* y, BElement* z){ // make parent, child 관계..
			if (y == NULL && z != NULL){
				return z;
			}
			else if (y != NULL && z == NULL){
				return y;
			}
			else if (y == NULL && z == NULL){
				return NULL;
			}
			//
			y->parent = z;
			y->sibling = z->child;
			z->child = y;
			z->degree = (z->degree) + 1;
			return z;
		}
		static BElement* Merge(BElement* H1, BElement* H2){
			if (H1 != NULL && H2 == NULL){
				return H1;
			}
			else if (H1 == NULL && H2 != NULL){
				return H2;
			}
			else if (H1 == NULL && H2 == NULL){
				return NULL;
			}
			// degree가 낮은순에서 높은 순으로 연결한다.
			BElement* nextH1 = H1;
			BElement* nextH2 = H2;
			BElement* temp; /** return value */
			BElement* nextTemp;

			if (nextH1->degree <= nextH2->degree){
				temp = nextH1;
				nextH1 = nextH1->sibling;
			}
			else{
				temp = nextH2;
				nextH2 = nextH2->sibling;
			}
			nextTemp = temp;
			while (nextH1 != NULL && nextH2 != NULL){
				if (nextH1->degree <= nextH2->degree){
					nextTemp->sibling = nextH1;//
					nextH1 = nextH1->sibling;
				}
				else{
					nextTemp->sibling = nextH2; //
					nextH2 = nextH2->sibling;
				}
				nextTemp = nextTemp->sibling;
			}
			while (nextH1 != NULL){
				nextTemp->sibling = nextH1;//
				nextH1 = nextH1->sibling;
				nextTemp = nextTemp->sibling;
			}
			while (nextH2 != NULL){
				nextTemp->sibling = nextH2;
				nextH2 = nextH2->sibling;
				nextTemp = nextTemp->sibling;
			}
			nextTemp->sibling = NULL;
			return temp;
		}

    public:
		// make Insert, using Union
		static void Insert(BinomialHeap<T>** H, BElement* x){
			BinomialHeap<T>* tempHeap = MakeHeap();
			//
			x->parent = NULL;
			x->child = NULL;
			x->sibling = NULL;
			x->degree = 0;
			tempHeap->head = x;
			tempHeap->n = 1;
			//
			(*H) = Union(H, &tempHeap);
		}
		//
		static BElement* ExtractMinimum(BinomialHeap<T>** H, const T& TINFINITY){
			BElement* x; /** minimum pointer */
			//
			x = Minimum((*H), TINFINITY);
			//
			if (x == NULL){
				return NULL;
			}

			if ((*H)->head->sibling == NULL){ // case1
				for (BElement* temp = x->child; temp != NULL; temp = temp->sibling){
					temp->parent = NULL;
				}
				(*H)->head = Reverse(x->child);//
				//cout << "case1" << endl;
			}
			else{
				if ((*H)->head == x){ // case2
					BinomialHeap<T>* tempHeap = MakeHeap();//
					for (BElement* temp = x->child; temp != NULL; temp = temp->sibling){
						temp->parent = NULL;
					}
					(*H)->head = x->sibling;//

					tempHeap->head = Reverse(x->child);

					(*H) = Union(H, &tempHeap);
					//cout << "case2" << endl;
				}
				else{ // case3
					BElement* before_x = (*H)->head;
					while (before_x->sibling != x){
						before_x = before_x->sibling;//
					}
					before_x->sibling = x->sibling;

					BinomialHeap<T>* tempHeap = MakeHeap();////
					for (BElement* temp = x->child; temp != NULL; temp = temp->sibling){
						temp->parent = NULL;
					}

					tempHeap->head = Reverse(x->child);
					(*H) = Union(H, &tempHeap);
					//cout << "case3" << endl;
				}
			}
			x->parent = NULL;
			x->degree = 0;
			x->child = NULL;
			x->sibling = NULL;
			(*H)->n--;
			// return
			return x;
		}
		//
    private:
		static BElement* Reverse(BinomialHeap<T>* heap){
			return Reverse(heap->head);
		}
		static BElement* Reverse(BElement* first){
			//return first; // for 반대가 안될때, 바로.
			if (first == NULL){
				return NULL;
			}
			if (first->sibling == NULL){
				return first;
			}
			BElement* next_x = first->sibling;
			BElement* temp = first;
			BElement* next_temp;
			//
			while (next_x != NULL){
				next_temp = next_x->sibling;
				next_x->sibling = temp;
				temp = next_x;
				next_x = next_temp;
			}
			first->sibling = NULL;
			//
			return temp;
		}
		//
    public:
		static void DecreaseKey(BElement* x, const T& k, Array<Vertex>& arr){
			if (NULL == x) {
				return;
			}
			if (x->key < k){
				// error new key is greater than current key
				return;
			}
			BElement *y; BElement *z;

			x->key = k;
			//
			y = x;
			z = y->parent;
			//

			while (z != NULL && y->key < z->key){
				//exchange key?
				wiz::Swap(y->key, z->key);
				///cout << y->no << " " <<  arr[y->no].node << " | " << z->no << " " << arr[z->no].node << endl;
				wiz::Swap(arr[y->no].node, arr[z->no].node);
				wiz::Swap(y->no, z->no);

				y = z;
				z = y->parent;
			}
		}
		static bool Delete(BinomialHeap<T>** heap, BElement* x, Array<Vertex>& arr, const T& TINFINITY, const T& TMINUSINFINITY){
			BElement* temp;
			DecreaseKey(x, TMINUSINFINITY, arr );
			temp = ExtractMinimum(heap, TINFINITY);
			if (temp){
				delete temp;
				return true;
			}
			else{
				return false;
			}
		}
	private:
		void initial(){
			head = NULL;
			n = 0;
		}
	private:
		BElement* head;//
		int n;
    private:
		void RemoveAll(BElement* element)
		{
			if (element != NULL)
			{
				BElement* child = element->child;
				BElement* sibling = element->sibling;

				delete element;
				RemoveAll(child);
				RemoveAll(sibling);
			}
		}
	public:
	    virtual ~BinomialHeap() {
			RemoveAll();
		}

		void RemoveAll()
		{
		    if( head ) {
                RemoveAll(head);
                head = NULL;
                n = 0;
		    }
		}
	};

	template < class T, class COMP = ASC<T>, class COMP2 = ASC<int>, class EE = EE<T> >
	class BinomialHeap2
	{
	private:
		BinomialHeap2(const BinomialHeap2<T,COMP,COMP2,EE>&);
		BinomialHeap2<T,COMP,COMP2,EE>& operator=(const BinomialHeap2<T,COMP,COMP2,EE>&);
	private:
		BinomialHeap< WrapForInfinity<T, COMP, COMP2,EE> >* heap;
	public:
		explicit BinomialHeap2()
		{
			heap = NULL;
		}
		virtual ~BinomialHeap2()
		{
		    if(heap) {
                delete heap;
		    }
		}
    public:
        static BinomialHeap2<T,COMP,COMP2,EE>* Union( BinomialHeap2<T,COMP,COMP2,EE>* H1, BinomialHeap2<T,COMP,COMP2,EE>* H2 )
        {
            BinomialHeap2<T,COMP,COMP2,EE>* temp = new BinomialHeap2<T,COMP,COMP2,EE>(); /// MakeBinomialHeap2()?

            temp->heap = BinomialHeap< WrapForInfinity<T,COMP,COMP2,EE> >::Union( &(H1->heap), &(H2->heap) );

            return temp;
        }


		class BinomialHeap< WrapForInfinity<T, COMP, COMP2, EE> >::BElement* Insert(const T& key, int no )
		{
			class BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::BElement* temp = new class BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::BElement(WrapForInfinity<T, COMP,COMP2, EE>(key), no);
			BinomialHeap< WrapForInfinity<T, COMP, COMP2,EE> >::Insert(&heap, temp);
			return temp;
		}
		T Extract(Array<Vertex>& arr)
		{
			return BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::Extract(&heap, arr, WrapForInfinity<T, COMP, COMP2,EE>::GetInfinity()).val;
		}
		T GetMinimum()const
		{
			return BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::GetMinimum(heap, WrapForInfinity<T, COMP, COMP2,EE>::GetInfinity()).val;
		}
		void DecreaseKey(class BinomialHeap< WrapForInfinity<T, COMP, COMP2,EE> >::BElement* x, const T& key, Array<Vertex>& arr)
		{
			BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::DecreaseKey(x, WrapForInfinity<T, COMP,COMP2, EE>(key), arr);
		}
		void Delete(class BinomialHeap< WrapForInfinity<T, COMP, COMP2,EE> >::BElement* x, Array<Vertex>& arr )
		{
			BinomialHeap< WrapForInfinity<T, COMP,COMP2, EE> >::Delete(&heap, x, arr, WrapForInfinity<T, COMP,COMP2, EE>::GetInfinity(), WrapForInfinity<T, COMP,COMP2, EE>::GetMinusInfinity());
		}
		/// debug..
		void PrintInorder()const /// remove??
		{
			BinomialHeap< WrapForInfinity<T, COMP, COMP2,EE> >::PrintInorder(heap);
		}
		bool empty()const {
		    if( NULL == heap ){ return true; }
			return heap->empty();
		}
		int size()const{
		    if( NULL == heap ) { return 0;}
			return heap->size();
		}
	};
 }
}
#endif // BINOMIALHEAPS_H_INCLUDED

