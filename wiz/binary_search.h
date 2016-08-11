
#ifndef BINARY_SEARCH_H_INCLUDED
#define BINARY_SEARCH_H_INCLUDED


namespace wiz{
	template <class T, template< class A > class ARR,  class COMP = ASC<T>, class EE = EE<T> > /// ARR -> ??
	class BinarySearch {
	public:
		static bool search(const ARR<T>& arr, const T& key, int* index=NULL)
		{
			return search(arr, key, 0, arr.size() - 1, index);
		}
		static bool search(const ARR<T>& arr, const T& key, const int low, const int high, int* index)
		{
			COMP comp;
			EE ee;
			int left = low;
			int right = high;
			int middle;

			while (left <= right) {
				middle = (left + right) >> 1; // /2
				if (ee(arr[middle], key)) {
					if (index) {
						*index = middle;
					}
					return true;
				}
				else if (comp(arr[middle], key)) {
					left = middle + 1;
				}
				else {
					right = middle - 1;
				}
			}

			return false;
		}
	};

}
#endif
