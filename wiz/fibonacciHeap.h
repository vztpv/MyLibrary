#ifndef FIBONACCIHEAP_H_INCLUDED
#define FIBONACCIHEAP_H_INCLUDED


#include <wiz/global.h>
#include <wiz/newArrays.h>
#include <wiz/Deck2.h> // 양방향 원형 연결리스트
#include <wiz/math/log2_x.h> // for fast?? log(x)/log(2)

namespace wiz{
namespace FH{
 template < class KEY, class COMP = wiz::ASC<KEY> >
 class Data
 {
 public:
  KEY key;
  int degree;
  bool mark;
  Data<KEY, COMP>* left;
  Data<KEY, COMP>* right;
  Data<KEY, COMP>* parent;
  Deck2< KEY, Data<KEY, COMP> >* child;
 public:
  explicit Data(const KEY& key = KEY()) : key(key), degree(0), mark(false), left(this), right(this), parent(NULL), child(NULL) { }

 };

 template < class KEY, class COMP = wiz::ASC<KEY> >
 class Heap
 {
 private:
  Deck2< KEY, Data<KEY, COMP> >* node; /// root list
  Data<KEY, COMP>* root; /// min or max
  int n;
  static Log2<unsigned int> log2; /// remove static?
 public:
  explicit Heap() : node(NULL), root(NULL), n(0)
  {
   node = new Deck2< KEY, Data<KEY, COMP> >();
  }
  virtual ~Heap() {
   while (!empty()) {
    Extract();
   }
   if (node) {
    delete node; node = NULL;
   }
   root = NULL;
  }
 private:
    Heap( const Heap<KEY,COMP>& );
    Heap<KEY,COMP>& operator=( const Heap<KEY,COMP>& );

  static void FIB_HEAP_INSERT(Heap<KEY, COMP>* heap, Data<KEY, COMP>* data)
  {
   COMP comp;
   if (heap->node->empty() || comp(data->key, heap->root->key))
   {
    heap->root = data;
   }
   heap->node->push_back(data);
   heap->n++;
  }
  /// chk need test...
  public:
  static Heap<KEY, COMP>* FIB_HEAP_UNION(Heap<KEY, COMP>** heap1, Heap<KEY, COMP>** heap2)
  {
   COMP comp;
   Heap<KEY, COMP>* temp = new Heap<KEY, COMP>();

   temp->root = (*heap1)->root;
   temp->node->deck_union(*(*heap1)->node);
   temp->node->deck_union(*(*heap2)->node);
   temp->n = (*heap1)->n + (*heap2)->n;

   if ( ( NULL == (*heap1)->root  )|| ( NULL != (*heap2)->root && comp((*heap2)->root->key, (*heap1)->root->key) ) )
   {
    temp->root = (*heap2)->root;
   }

   // delete *heap1, *heap2
   (*heap1)->n = 0;
   (*heap2)->n = 0;
   (*heap1)->root = NULL;
   (*heap2)->root = NULL;
   delete *heap1;
   delete *heap2;
   *heap1 = NULL;
   *heap2 = NULL;

   return temp;
  }
  private:
  static Data<KEY, COMP>* FIB_HEAP_EXTRACT_MIN(Heap<KEY, COMP>* heap)
  {

   Data<KEY, COMP>* z = heap->root;

   if (z != NULL) {
    if (z->child) {
     while (!(z->child->empty())) {
      Data<KEY, COMP>* temp = (z->child->pop_front_node());
      temp->left = temp;
      temp->right = temp;
      temp->parent = NULL;

      heap->node->push_back(temp);
     }
     delete z->child; z->child = NULL;
    }
    heap->node->remove(z);
    z->left = z;
    z->right = z;
    z->parent = NULL;

    if (heap->node->empty())
    {
     heap->root = NULL;
    }
    else
    {
     heap->root = const_cast<Data<KEY, COMP>*>(heap->node->first());
     CONSOLIDATE(heap);
    }
    heap->n--;
   }

   return z;
  }
  static int D(const int size)
  {
   return log2.getLog2(size) + 2;
  }
  static void CONSOLIDATE(Heap<KEY, COMP>* heap)
  {
   Array< Data<KEY, COMP>* > A(D(heap->size()), NULL);
   COMP comp;


   while (!(heap->node->empty()))
   {
    Data<KEY, COMP>* x = heap->node->pop_front_node();

    x->left = x;
    x->right = x;
    x->parent = NULL;


    int d = x->degree;

    while ( A[d] != NULL) { ///
     Data<KEY, COMP>* y = A[d];

     if (comp(y->key, x->key)) {
        wiz::Swap(x, y);
     }

     FIB_HEAP_LINK(heap, y, x);

     A[d] = NULL;
     d++;
    }
    //cout << "degree " <<d << endl;

    A[d] = x;//if( A[A.size()-1] ) { cout << A[d] <<endl; }
   }

   heap->root = NULL;
   for (int i = 0; i < A.size(); i++) {
    if (A[i] != NULL) {
     A[i]->left = A[i];
     A[i]->right = A[i];
     A[i]->parent = NULL;

     heap->node->push_back(A[i]);
     if (NULL == heap->root || comp(A[i]->key, heap->root->key))
     {
      heap->root = A[i];
     }
    }
   }
  }
  static void FIB_HEAP_LINK(Heap<KEY, COMP>* heap, Data<KEY, COMP>* y, Data<KEY, COMP>* x)
  {
   x->degree++;

   if (NULL == x->child) {
    x->child = new Deck2<KEY, Data<KEY, COMP> >();
   }

   y->left = y;
   y->right = y;

   y->parent = x;
   y->mark = false;

   x->child->push_back(y);
  }
  static void CUT(Heap<KEY, COMP>* heap, Data<KEY, COMP>* x, Data<KEY, COMP>* y)
  {
   y->child->remove(x);

   if (y->child->empty())
   {
    delete y->child; y->child = NULL;
   }
   y->degree--;

   x->left = x;
   x->right = x;
   x->parent = NULL;
   x->mark = false;

   heap->node->push_back(x);
  }
  static void CASCADING_CUT(Heap<KEY, COMP>* heap, Data<KEY, COMP>* y)
  {
   Data<KEY, COMP>* z;

   z = y->parent;
   if (z != NULL) {
    if ( false == y->mark ) {
     y->mark = true;
    }
    else{
     CUT(heap, y, z);
     CASCADING_CUT(heap, z);
    }
   }
  }
  static void DecreaseKey(Heap<KEY, COMP>* heap, Data<KEY, COMP>* x, const KEY& k)
  {
   COMP comp;
   Data<KEY, COMP>* y;
   if (heap->empty()) {
    throw wiz::Error("heap is empty.");
   }
   if( NULL == x ) { throw wiz::Error( "x is NULL" ); }
   if (comp(x->key, k)) {
    throw wiz::Error("x->key < k");
   }

   x->key = k;
   y = x->parent;

   if (NULL != y && comp(x->key, y->key)) {
        CUT(heap, x, y);
        CASCADING_CUT(heap, y);
   }
   if (comp(x->key, heap->root->key))
   {
        heap->root = x;
   }


  }
 public:
  int size()const {
   return n;
  }
  bool empty()const{
   return 0 == n; //NULL == this->root; /// return 0 == n;
  }
  /// TODO - insert, find(min or max), Decrease Key
  Data<KEY, COMP>* Insert(const KEY& key)
  {
   Data<KEY, COMP>* temp = new Data<KEY, COMP>(key);
   FIB_HEAP_INSERT(this, temp);
   return temp;
  }
  KEY Extract()
  {
   Data<KEY, COMP>* temp = FIB_HEAP_EXTRACT_MIN(this);

   if (NULL == temp) {
    throw wiz::Error(__FILE__, __LINE__, "Extracct err");
   }
   KEY key = temp->key;
   delete temp;
   return key;
  }
  Data<KEY, COMP>* Extract2()
  {
   return FIB_HEAP_EXTRACT_MIN(this);
  }
  void DecreaseKey(Data<KEY, COMP>* data, const KEY& key)
  {
   DecreaseKey(this, data, key);
  }

  void Print()
  {
   if (node) {
    node->Print();
   }
  }
  KEY Root()const{ // rename root-key?
   if (root)
    return root->key;
   else
    throw wiz::Error( " root is NULL " );
  }
  int TotalNum()const {
   return node->TotalNum();
  }
  bool TotalExist(Data<KEY, COMP>* x)const{
   return this->node->TotalExist(x);
  }
 };

 template < class KEY, class COMP >
 Log2<unsigned int> Heap<KEY, COMP>::log2; /// move to math/log2.h ??

}
}

#endif // FIBONACCIHEAP_H_INCLUDED
