#ifndef BINARY_SEARCH_TREE_H_INCLUDED
#define BINARY_SEARCH_TREE_H_INCLUDED

#include <wiz/global.h>
#include <wiz/newArrays.h>


namespace wiz
{
	namespace bst {
		template <class T>
		T max(const T& t1, const T& t2)
		{
			if (t1 > t2) { return t1; }
			return t2;
		}
		template <class T>
		class BinaryNode
		{
		public:
			T val;

			BinaryNode<T>* left;
			BinaryNode<T>* right;
			BinaryNode<T>* parent;
			//
			explicit BinaryNode(const T& val = T()) : val(val), left(NULL), right(NULL), parent(NULL) { }
		};
		

		template < class T, class COMP = ASC<T>, class EE = EE<T> >
		class BinarySearchTree
		{
		private:
			BinaryNode<T>* root;
			int count;
		public:
			void InorderTraversalUsingStack(wiz::Array<T>& arr)
			{
				InorderTraversalUsingStack(root, arr);
			}
			void PreorderTraversalUsingStack(wiz::Array<T>& arr)
			{
				PreorderTraversalUsingStack(root, arr);
			}
		private:
			static void InorderTraversalUsingStack(BinaryNode<T>* root, wiz::Array<T>& arr)
			{
				if (NULL == root) { return; }
				int index = 0;
				std::vector<BinaryNode<T>*> nodeStack;
				bool use_left_loop = true;
				nodeStack.push_back(root);

				while (false == nodeStack.empty()) {
					while (use_left_loop && NULL != nodeStack.back()->left) {
						nodeStack.push_back(nodeStack.back()->left);
					}

					BinaryNode<T>* temp = nodeStack.back();
					arr[index] = temp->val;
					index++;
					
					nodeStack.pop_back();
					if (NULL != temp->right) {
						nodeStack.push_back(temp->right);
						use_left_loop = true;
					}
					else {
						use_left_loop = false;
					}
				}
			}
			
			static void PreorderTraversalUsingStack(BinaryNode<T>* root, wiz::Array<T>& arr)
			{
				if (NULL == root) { return; }
				int index = 0;
				std::vector<BinaryNode<T>*> nodeStack;
				nodeStack.push_back(root);

				while (false == nodeStack.empty()) {
					BinaryNode<T>* left = nodeStack.back()->left;
					BinaryNode<T>* right = nodeStack.back()->right;
					BinaryNode<T>* now = nodeStack.back();

					nodeStack.pop_back();
					if (NULL != right) { nodeStack.push_back(right); }
					if (NULL != left) { nodeStack.push_back(left); }
					
					arr[index] = now->val;
					index++;
				}
			}

			static int GetLevel(const BinaryNode<T>* node) /// chk...
			{
				if (NULL == node) {
					return 0;
				}

				int val = 1 + wiz::bst::max(GetLevel(node->left), GetLevel(node->right));
				return val;
			}
			static void PrintLevelDif(const BinaryNode<T>* node) /// chk..
			{
				if (NULL == node) {
					return;
				}
				int ret = GetLevel(node->left) - GetLevel(node->right);
				/// print ret..
				PrintLevelDif(node->left);
				PrintLevelDif(node->right);
			}
			static bool Equal(const BinaryNode<T>* node1, const BinaryNode<T>* node2)
			{
				// end condition..
				if (NULL == node1 && NULL == node2) {
					return true;
				}
				else if (NULL == node1 && NULL != node2) {
					return false;
				}
				else if (NULL != node1 && NULL == node2) {
					return false;
				}
				//
				bool bval = // node1->val == node2->val;
					EE() (node1->val, node2->val);
				return bval && Equal(node1->left, node2->left)
					&& Equal(node1->right, node2->right);
			}

			static BinaryNode<T>* Copy(BinaryNode<T>* node)
			{
				if (NULL == node) {
					return NULL;
				}
				BinaryNode<T>* temp = CreateBinaryNode(node->val);
				temp->left = Copy(node->left);
				temp->right = Copy(node->right);
				return temp;
			}
			static BinaryNode<T>* CreateBinaryNode(const T& val)
			{
				return new BinaryNode<T>(val);
			}
			static void InorderTraversal(wiz::Array<T>& arr, BinaryNode<T>* node, int& idx)
			{
				if (NULL == node) {
					return;
				}
				InorderTraversal(arr, node->left, idx);
				arr[idx] = node->val;
				idx++;
				InorderTraversal(arr, node->right, idx);
			}
			static void InverseInorderTraversal(wiz::Array<T>& arr, BinaryNode<T>* node, int& idx) /// rename?
			{
				if (NULL == node) {
					return;
				}
				InverseInorderTraversal(arr, node->right, idx);
				arr[idx] = node->val;
				idx++;
				InverseInorderTraversal(arr, node->left, idx);
			}
			static void PreorderTraversal(wiz::Array<T>& arr, BinaryNode<T>* node, int& idx)
			{
				if (NULL == node) {
					return;
				}
				arr[idx] = node->val;
				idx++;
				PreorderTraversal(arr, node->left, idx);
				PreorderTraversal(arr, node->right, idx);
			}
			static void RemoveAll(BinaryNode<T>* node)
			{
				if (NULL == node) {
					return;
				}
				BinaryNode<T>* left = node->left;
				BinaryNode<T>* right = node->right;

				delete node;

				RemoveAll(left);
				RemoveAll(right);
			}

			static BinaryNode<T>* Search(BinaryNode<T>* node, const T& val)
			{
				BinaryNode<T>* temp = node;

				while (temp) {
					//if (temp->val == val) {
					if (EE() (temp->val, val)) {
						return temp;
					}
					//else if ( temp->val > val) {
					else if (COMP() (val, temp->val)) {
						temp = temp->left;
					}
					else {
						temp = temp->right;
					}
				}

				return NULL;
			}
			static BinaryNode<T>* FindBeforeValueNodeFrom(BinaryNode<T>* node, int* op)
			{
				if (NULL == node) {
					return NULL;
				}
				BinaryNode<T>* bvNode = node->left; /// before value node
				if (NULL == bvNode) {
					return NULL;
				}

				*op = 0;
				while (bvNode->right)
				{
					*op = 1;
					bvNode = bvNode->right;
				}
				return bvNode;
			}
			static BinaryNode<T>* FindNextValueNodeFrom(BinaryNode<T>* node, int* op)
			{
				if (NULL == node) {
					return NULL;
				}
				BinaryNode<T>* nvNode = node->right; /// next value node
				if (NULL == nvNode) {
					return NULL;
				}

				*op = 0;
				while (nvNode->left)
				{
					*op = 1;
					nvNode = nvNode->left;
				}
				return nvNode;
			}

			static BinaryNode<T>* Balancing(const wiz::Array<T>& sortedArr, int start, int end)
			{
				if (start > end) {
					return NULL;
				}
				BinaryNode<T>* temp = NULL;
				int middle = (start + end) >> 1; // /2

				temp = CreateBinaryNode(sortedArr[middle]);

				temp->left = Balancing(sortedArr, start, middle - 1);
				temp->right = Balancing(sortedArr, middle + 1, end);

				return temp;
			}

		public:
			static void PrintLevelDif(const BinarySearchTree<T, COMP, EE>& bst)
			{
				PrintLevelDif(bst.root);
			}
			static bool Equal(const BinarySearchTree<T, COMP, EE>& bst1, const BinarySearchTree<T, COMP, EE>& bst2)
			{
				return Equal(bst1.root, bst2.root);
			}
			static void Balancing(BinarySearchTree<T, COMP, EE>& bt, const wiz::Array<T>& sortedArr)
			{
				if (0 < bt.GetCount()) {
					bt.RemoveAll();
				}
				bt.root = Balancing(sortedArr, 0, sortedArr.size() - 1);
				bt.count = sortedArr.size();
			}
			explicit BinarySearchTree() : root(NULL), count(0) { }
			virtual ~BinarySearchTree() {
				RemoveAll();
			}

			BinarySearchTree(const BinarySearchTree<T, COMP, EE>& bst)
			{
				root = NULL; count = 0;
				*this = bst;
			}
			BinarySearchTree<T, COMP, EE>& operator=(const BinarySearchTree<T, COMP, EE>& bst)
			{
				if (root != bst.root)
				{
					RemoveAll();//

					root = Copy(bst.root);
					count = bst.count;
				}
				return *this;
			}
			bool Insert(const T& val)
			{
				if (IsExist(val)) { ///chk..
					return false;
				}
				if (NULL == root)
				{
					root = CreateBinaryNode(val);
					root->parent = NULL;
				}
				else
				{
					BinaryNode<T>* temp = root;
					BinaryNode<T>* temp2 = temp;
					while (temp != NULL) {
						//if (temp->val < val)
						if (COMP() (temp->val, val))
						{
							temp2 = temp;
							temp = temp->right;
						}
						//else if (temp->val > val)
						else if (COMP() (val, temp->val))
						{
							temp2 = temp;
							temp = temp->left;
						}
					}
					// create, set parent.
					//if (temp2->val < val) {
					if (COMP() (temp2->val, val)) {
						temp2->right = CreateBinaryNode(val); temp2->right->parent = temp2;
					}
					//else if (temp2->val > val) {
					else if (COMP() (val, temp2->val)) {
						temp2->left = CreateBinaryNode(val); temp2->left->parent = temp2;
					}
				}
				count++;
				return true;
			}
			bool IsExist(const T& val)const
			{
				return NULL != Search(root, val);
			}
			void Remove(const T& val) /// chk!!
			{
				BinaryNode<T>* node = root;
				if (NULL == node) {
					return;
				}
				BinaryNode<T>* valNode = Search(node, val);

				if (NULL == valNode) {
					return;
				}
				int bvNo, nvNo;

				BinaryNode<T>* bvNode = FindBeforeValueNodeFrom(valNode, &bvNo);
				BinaryNode<T>* nvNode = FindNextValueNodeFrom(valNode, &nvNo);

				if (!bvNode && !nvNode) { /// valNode has not child.
					if (valNode == root) {
						delete root; root = NULL; count--; return;
					}

					if (valNode == valNode->parent->left) {
						valNode->parent->left = NULL;
					}
					else {
						valNode->parent->right = NULL;
					}

					delete valNode; valNode = NULL; count--;
				}
				else if (bvNode) { /// valNode has left child.
					if (0 == bvNo) {
						if (valNode->parent && valNode->parent->left == valNode)
						{
							valNode->parent->left = bvNode;//
						}
						else if (valNode->parent && valNode->parent->right == valNode)
						{
							valNode->parent->right = bvNode;
						}
						bvNode->right = valNode->right;
						if (valNode->right) {
							valNode->right->parent = bvNode;
						}
						bvNode->parent = valNode->parent;
						if (root == valNode) {
							root = bvNode;
						}
						delete valNode; valNode = NULL; count--;
					}
					else {
						bvNode->parent->right = NULL;

						bvNode->right = valNode->right;
						if (valNode->right) {
							valNode->right->parent = bvNode;
						}

						if (bvNode->left) {
							bvNode->left->parent = bvNode->parent;
							bvNode->parent->right = bvNode->left;
						}
						if (valNode->parent && valNode == valNode->parent->left)
						{
							valNode->parent->left = bvNode;
						}
						else if (valNode->parent && valNode == valNode->parent->right)
						{
							valNode->parent->right = bvNode;
						}

						bvNode->left = valNode->left;
						valNode->left->parent = bvNode;

						bvNode->parent = valNode->parent;
						if (root == valNode) {
							root = bvNode;
						}

						delete valNode; valNode = NULL;
						count--;
					}
				}
				else { /// Right <> Left...
					if (0 == nvNo) {
						if (valNode->parent && valNode->parent->right == valNode)
						{
							valNode->parent->right = nvNode;//
						}
						else if (valNode->parent && valNode->parent->left == valNode) {
							valNode->parent->left = nvNode;
						}

						nvNode->left = valNode->left;
						if (valNode->left) {
							valNode->left->parent = nvNode;
						}

						nvNode->parent = valNode->parent;
						if (valNode == root) {
							root = nvNode;
						}
						delete valNode; valNode = NULL; count--;
					}
					else {
						nvNode->parent->left = NULL;

						nvNode->left = valNode->left;
						if (valNode->left) {
							valNode->left->parent = nvNode;
						}

						if (nvNode->right) {
							nvNode->right->parent = nvNode->parent;
							nvNode->parent->left = nvNode->right;
						}

						if (valNode->parent && valNode == valNode->parent->right)
						{
							valNode->parent->right = nvNode;
						}
						else if (valNode->parent && valNode == valNode->parent->left)
						{
							valNode->parent->left = nvNode;
						}

						nvNode->right = valNode->right;
						valNode->right->parent = nvNode;

						nvNode->parent = valNode->parent;

						if (root == valNode) {
							root = nvNode;
						}
						delete valNode; valNode = NULL;
						count--;
					}
				}
			}
			void RemoveAll()
			{
				RemoveAll(root);
				root = NULL;
				count = 0;
			}
 			wiz::Array<T> ToSortedArray() const // To wiz::Array Using Inorder
			{
				if (count <= 0) {
					return wiz::Array<T>();
				}
				// inorder Traversal
				wiz::Array<T> arr(count);
				int i = 0;
				InorderTraversal(arr, root, i);
				return arr;
			}
			wiz::Array<T> ToInverseSortedArray() const
			{
				if (count <= 0) {
					return wiz::Array<T>();
				}
				wiz::Array<T> arr(count);
				int i = 0;
				InverseInorderTraversal(arr, root, i);
				return arr;
			}
			wiz::Array<T> ToUsingInorder() const
			{
				return ToSortedwiz::Array();
			}
			wiz::Array<T> ToUsingPreorder() const
			{
				if (count <= 0) {
					return wiz::Array<T>();
				}
				wiz::Array<T> arr(count);
				int i = 0;
				PreorderTraversal(arr, root, i);
				return arr;
			}
			int GetCount()const
			{
				return count;
			}
			int size()  const
			{
				return count;
			}
		};

	}
}


#endif // BINARY_SEARCH_TREE_H_INCLUDED
