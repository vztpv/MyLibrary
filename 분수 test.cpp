
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

#include <stdio.h>
/*
namespace wiz{
	template <class T>
	inline T max(const T& t1, const T& t2)
	{
		if (t2 < t1) {
			return t1;
		}
		return t2;
	}

	template <class T>
	inline T min(const T& t1, const T& t2)
	{
		if (t1 < t2) {
			return t1;
		}
		return t2;
	}
	///
}
*/
namespace list{
	/// #define wiz_list_DEBUG ???
	template <class T>
	class Node {
	public:
		T val; // Data
		Node<T>* next;
	};
	template <class T>
	class DSC_EE {
	public:
		bool operator() (Node<T>* t1, Node<T>* t2) const {
			return t1 >= t2;
		}
	};
	template <class T>
	class FIRST{
	public:
		bool operator() (Node<T>* n1, Node<T>* n2) const {
			return false;
		}
	};
	template <class T>
	class APPEND{
	public:
		bool operator() (Node<T>* n1, Node<T>* n2) const {
			return true;
		}
	};

	template <class T, class COMP>
	class List{
	private:
		Node<T> head;
		COMP comp; /// chk??
	public:
		List(List<T, COMP>&& list)
		{
			head = list.head;
			list.head.next = NULL;
		}
		List<T, COMP>& operator=(List<T, COMP>&& list)
		{
			if (head.next == list.head.next) {
				return *this;
			}
			swap(head.next, list.head.next);
			return *this;
		}
		List(const List<T, COMP>& list)
		{
			initList(this);

			for (auto x = list.cbegin(); x != list.cend(); ++x)
			{
				insert(*x);
			}
		}
		List<T, COMP>& operator=(const List<T, COMP>& list)
		{
			List<T, COMP> temp = list;

			removeAll();

			for (auto x = temp.cbegin(); x != temp.cend(); ++x)
			{
				insert(*x);
			}

			return *this;
		}
	public:
		void removeAll()
		{
			Node<T>* node = head.next;
			Node<T>* node2;

			// doEmpty..
			while (node)
			{
				// backup
				node2 = node;
				// next
				node = node->next;
				// remove
				delete node2;
			}
			//
			head.next = NULL;
		}
	public:
		explicit List() {
			initList(this);
		}
		~List()
		{
			removeAll();
		}
		//
		bool empty()const{
			return NULL == head.next;
		}
		void insert(const T& val)
		{
			insert(this, makeNode(val));
		}
		// cf) bool getVal( int ith, T* pval ) // ?
		// show,
		friend ostream& operator<<(ostream& stream, const List<T, COMP>& list)
		{
			Node<T>* pNode = list.head.next;


			while (pNode)
			{
				stream << pNode->val << "\n";
				pNode = pNode->next;
			}
			return stream;
		}
	private:
		void initNode(Node<T>* node, const T& val){
			node->val = val; node->next = NULL;
		}
		void initList(List<T, COMP>* list) {
			initNode(&(list->head), T());
		}
		Node<T>* makeNode(T val)
		{
			Node<T>* node = (Node<T>*) new Node<T>();


			if (NULL == node) {
				return NULL;
			} // print err.


			initNode(node, val);


			return node;
		}
		// use Function Object and Template.// then can inline!
		void insert(List<T, COMP>* list, Node<T>* node)
		{
			Node<T>* pNode;


			pNode = &(list->head);


			// find good place
			while (NULL != (pNode->next) &&
				   comp(pNode->next, node)///( pNode->next->ival ) <= ( node->ival )  /// comp! cf) using function ptr!
				   )
			{
				pNode = pNode->next;
			}

			// insert
			node->next = pNode->next;
			pNode->next = node;
		}
	public: /// To Do - make iterator, begin, end...
		class Iter //ator
		{
		private:
			Node<T>* node;//
		public:
			explicit Iter(Node<T>* node = NULL) : node(node) { }
			void right() {
#ifdef wiz_list_DEBUG
				if (!node) {
					throw Error(__FILE__, __LINE__, " Iter err ");
				}
#endif
				node = node->next;
			}
			T data()const {
				return *node;
			}
			bool isNULLcomp()const {
				return NULL == node;
			}
			bool operator!=(const Iter& x) const
			{
				return x.node != this->node;
			}
			bool operator==(const Iter& x) const
			{
				return x.node == this->node;
			}
			void operator++() {
				right();
			}
			T& operator*() {
				return node->val;
			}
			T* operator->() {
				return &(node->val);
			}
			const T* operator->()const {
				return &(node->val);
			}
			const T& operator*()const {
				return node->val;
			}
		};
		class ConstIter //ator
		{
		private:
			Node<T>* node;//
		public:
			explicit ConstIter(Node<T>* node = NULL) : node(node) { }
			void right() {
#ifdef wiz_list_DEBUG
				if (!node) {
					throw Error(__FILE__, __LINE__, " Iter err ");
				}
#endif
				node = node->next;
			}
			T data()const {
				return *node;
			}
			bool isNULL()const {
				return NULL == node;
			}
			bool operator!=(const ConstIter& x) const
			{
				return x.node != this->node;
			}
			bool operator==(const ConstIter& x) const
			{
				return x.node == this->node;
			}
			void operator++() {
				right();
			}
			const T* operator->()const {
				return &(node->val);
			}
			const T& operator*()const {
				return node->val;
			}
		};
	public:
		Iter begin() {
			return Iter(head.next);
		}
		Iter end() {
			return Iter();
		}
		ConstIter cbegin() const {
			return ConstIter(head.next);
		}
		ConstIter cend() const {
			return ConstIter();
		}
	public:
		void remove(const T& x) /// nedd fix!!, why err??
		{
			Node<T>* node = &head;

			while (node->next){
				if (x == node->next->val){
					Node<T>* temp = node->next;
					node->next = temp->next;
					delete temp;
					break;
				}
				node = node->next;
			}

		}
	};
}


/// template UNSIGNED_INT?? for long long?
unsigned int _getGCD(const unsigned int x, const unsigned int y)
{
	unsigned int a = wiz::max(x, y);
	unsigned int b = wiz::min(x, y);
	unsigned int c = a % b;

	while (0 != c)
	{
		a = b;
		b = c;
		c = a % b;
	}
	return b;
}
/// template UNSIGNED_INT?? for long long?
inline unsigned int _getLCM(const unsigned int x, const unsigned int y)
{
	return (x * y) / (_getGCD(x, y));
}
/// template UNSIGNED_INT?? for long long?
class Fraction{
private:
	bool m; /// is_minus
	unsigned int a, b; /// a/b
public:
	unsigned int getA()const{
		return a;
	}
	unsigned int getB()const{
		return b;
	}
	explicit Fraction() : a(0), b(1) { }
	explicit Fraction(const bool bm, const unsigned int x, const unsigned int y)
	{
		m = bm;
		a = x;
		b = y;
		if (0 == b) {
			throw "err: b(divider) is zero.";
		}
		simple();
	}
	void simple()
	{
		if (a > 1 && b > 1) { /// chk..
			const int gcd = _getGCD(a, b);
			a = a / gcd;
			b = b / gcd;
		}
	}
	Fraction operator+(const Fraction& frc)const
	{
		/// temp1, temp2
		Fraction temp1 = *this;
		Fraction temp2 = frc;
		Fraction result;
		const unsigned int lcm = _getLCM(temp1.b, temp2.b);

		///
		temp1.a *= lcm / temp1.b;
		temp2.a *= lcm / temp2.b;
		temp1.b = lcm;
		temp2.b = lcm;
		result.b = lcm;
		///
		if (temp1.m == temp2.m)
		{
			result.m = temp1.m;
			result.a = temp1.a + temp2.a;
		}
		else if (temp1.a == temp2.a) {
			result.m = false;
			result.a = 0;
			result.b = 1;
		}
		else if (temp1.a > temp2.a)
		{
			result.a = temp1.a - temp2.a; ///
			result.m = temp1.m;
		}
		else
		{
			result.a = temp2.a - temp1.a;
			result.m = temp2.m;
		}

		result.simple();
		return result;
	}
	void operator+=(const Fraction& frc)
	{
		*this = (*this) + frc;
	}
	Fraction operator-(const Fraction& frc)const
	{
		return (*this) + Fraction(!frc.m, frc.a, frc.b);
	}

	Fraction operator-()const
	{
		return Fraction(!m, a, b);
	}
	void operator-=(const Fraction& frc)
	{
		(*this) = (*this) - frc;
	}
	Fraction operator*(const Fraction& frc)const
	{
		Fraction temp1 = *this, temp2 = frc;
		Fraction result;

		result.m = !(temp1.m == temp2.m);
		result.a = temp1.a * temp2.a;
		result.b = temp1.b * temp2.b;
        if( result.b == 0 ) { throw "err.: b is zero"; }
		result.simple();
		return result;
	}
	void operator*=(const Fraction& frc)
	{
		(*this) = (*this) * frc;
	}
	Fraction operator/(const Fraction& frc)const
	{
		return (*this) * Fraction(frc.m, frc.b, frc.a);
	}
	void operator/=(const Fraction& frc)
	{
		(*this) = (*this) / frc;
	}
	friend
		ostream& operator<<(ostream& os, const Fraction& frc)
	{
		if (frc.m) {
			os << "-";
		}
		if (1 == frc.b) {
			os << frc.a;
		}
		else {
			os << frc.a << "/" << frc.b;
		}
		return os;
	}
	bool operator==(const Fraction& frc)const
	{
		return m == frc.m && a == frc.a && b == frc.b;
	}
	bool operator!=(const Fraction& frc)const
	{
		return !(*this == frc);
	}
	bool isMinusValue() const
	{
		return m;
	}
	bool operator<(const Fraction& frc)const
	{
		Fraction temp1 = *this, temp2 = frc;

		Fraction result = temp1 - temp2;
		return result.isMinusValue();
	}
	bool operator>(const Fraction& frc)const
	{
		return frc < (*this);
	}
	bool operator>=( const Fraction& frc)const
	{
	    return (*this) > frc || (*this) == frc;
	}
};

inline bool IsDigit(const char ch)
{
	return ch >= '0' && ch <= '9';
}
inline bool IsSpace(const char ch)
{
	switch (ch)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	return true;
	}
	return false;
}
bool IsMinus(const string& str, const int start_index, int* end_index, bool* chk)
{
	bool isMinus = false;
	*chk = false;
	for (int i = start_index; i < str.size(); i++)
	{
		if (IsSpace(str[i])) {
			continue;
		}

		if ('-' == str[i]) {
			isMinus = !isMinus;
		}
		else if ('+' == str[i]) {
		}
		else {
			*chk = true;
			*end_index = i - 1;
			break;
		}
	}

	return isMinus;
}
bool ToInt(const string& str, const int start, int* end, int* ival)  /// to unsigned int?
{
	*end = -1;
	int state = 0;
	int val = 0;
	bool success = false;

	for (int i = start; i < str.size(); i++) {
		const char ch = str[i];
		switch (state) {
		case 0:
		if (IsDigit(ch)) {
			val = ch - '0'; *ival = val; *end = i; state = 1; success = true;
		}
		else {
			return false;
		}
		break;
		case 1:
		if (IsDigit(ch)) {
			val = val * 10 + ch - '0'; *ival = val; *end = i; state = 1; success = true;
		}
		else {
			return true;
		}
		break;
		}
	}
	return success;
}

class Polinomial
{
public:
	int coeffi; //
	string name; /// x or y?
	int exp; /// exp >= 0, integer...
public:
	explicit Polinomial(const int coeffi = 0, const string name = "", const int exp = 0) : coeffi(coeffi), name(name), exp(exp) { }
public:
	bool operator>=(const Polinomial& pnl) const
	{
		return exp >= pnl.exp;
	}
	bool operator==(const Polinomial& pnl) const
	{
		return pnl.exp == exp;
	}
	friend
		ostream& operator<<(ostream& stream, const Polinomial& pnl)
	{
		if (pnl.exp == 1)
		{
			stream << pnl.coeffi << pnl.name << "\n";
		}
		else if (pnl.exp == 0)
		{
			stream << pnl.coeffi << "\n";
		}
		else {
			stream << pnl.coeffi << pnl.name << "^" << pnl.exp << "\n";
		}
		return stream;
	}
};
typedef list::List< Polinomial, list::DSC_EE< Polinomial > > PolyList;
void insert(PolyList& list_1, const Polinomial& poly)
{
	if (0 == poly.coeffi && poly.exp > 0 ) {
		return;
	}
	for (auto& x : list_1)
	{
		if (x.name == poly.name && x.exp == poly.exp  ) {
			x.coeffi = x.coeffi + poly.coeffi;
            return;
		}
	}
	list_1.insert(poly);
}
PolyList f(const string& str, bool* success)
{
	PolyList polyList;
	Polinomial polyTemp;
	int state = 0;
	bool is_minus = false;
	int end_index = -2;
	int ival = -4;
	bool chk = false;

	*success = true;

	for (int i = 0; i < str.size(); i++) {
		if (IsSpace(str[i])) {
			continue;
		}
        bool chk_1 = true;
        polyTemp.exp = 0; polyTemp.coeffi = 0;
        polyTemp.name = "";
        is_minus = IsMinus(str, i, &end_index, &chk);
        if( chk ) {
            i = end_index + 1;
        }
        if( ToInt( str, i, &end_index, &ival ) ) {
            i = end_index;
            polyTemp.coeffi = (is_minus) ? -ival : ival;
            polyTemp.exp = 0;
            chk_1 = false;
        }
        if( chk_1 && ( 'x' == str[i] || 'y' == str[i] ) )
        {
            polyTemp.name = string("") + str[i];
            polyTemp.exp = 1;
            if( is_minus )
                polyTemp.coeffi = -1.0;
            else
                polyTemp.coeffi = 1.0;
            insert( polyList, polyTemp );
        }
        else if( i+1 <= str.size()-1 && ( 'x' == str[i+1] || 'y' == str[i+1] ) ) {
            polyTemp.name = string("") + str[i+1];
            polyTemp.exp = 1;
            i = i + 1;
            if( !chk_1 ) { insert( polyList, polyTemp ); continue; }
        }
        if( !chk_1 )
        {
            insert( polyList, polyTemp );
        }
	}
	return polyList;
}
PolyList g(const string& str, bool* success)
{
	*success = true;
	PolyList polyList;
	PolyList polyList2;
	int index = -1;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '=') {
			index = i; break;
		}
	}
	/// if( index == -1 ){ *success =false; throw err; }
	polyList = f(str.substr(0, index ), success);
	if (false == *success) {
		throw "err";
	}

	polyList2 = f(str.substr(index + 1, (str.size() - 1) - (index + 1) + 1), success);

	if( false == *success )
    {
        throw "err2";
    }
	PolyList result = polyList;
	for (auto& poly : polyList2)
	{
		poly.coeffi *= -1;
		insert(result, poly);
	}
	return result;
}
const int INCONSISTENT_SOLUTION = -1;
const int UNIQUE_SOLUTION = 0;
const int INFINITY_SOLUTION = 1;
//
class LinearEquationSystem;
class Sol_LinearEquationSystem;
//
class LeadingEntry{
private:
	int row;
	int column;
	//
	friend class Sol_LinearEquationSystem;
};

class LinearEquation{
public:
	LinearEquation(const LinearEquation& p)
	{
		this->N = p.N;
		this->cf = new Fraction[N];
		*this = p;
	}

	LinearEquation& operator=(const LinearEquation& p){
		if (p.N != this->N){
			throw "p.N is not equal with this->N ";
		}
		this->b = p.b;
		for (int i = 0; i< this->N; i++){
			this->cf[i] = p.cf[i];
		}
		return *this;
	}

	LinearEquation(const int N) : N(N){
		cf = new Fraction[N];
	}
	~LinearEquation(){
		if (cf){
			delete[] cf;
		}
	}
private:
	Fraction* cf; // coefficient 계수..
	Fraction b; // cf1*x1 + cf2*x2 + cf3*x3 = b //에서 b
	int N; // num of cf element
	//
	friend class LinearEquationSystem;
	friend class Sol_LinearEquationSystem;
};
// 변수 N개, 식도 N개인 경우만 다룬다.
class LinearEquationSystem{
public:
	LinearEquationSystem& operator=(const LinearEquationSystem& les)
	{
		LinearEquationSystem temp(les);
		if (this->N != temp.N) {
			for (int i = 0; i < this->N; i++)
			{
				delete equations[i];
			}
			delete[] equations;

			equations = new LinearEquation*[temp.N];
			for (int i = 0; i < temp.N; i++) {
				equations[i] = new LinearEquation(temp.N);
			}
			this->N = temp.N;
		}
		for (int i = 0; i < this->N; i++) {
			*(equations[i]) = *(temp.equations[i]);
		}
		return *this;
	}

	LinearEquationSystem(const int N){
		this->N = N;
		equations = new LinearEquation*[N]; //
		for (int i = 0; i< N; i++){
			equations[i] = new LinearEquation(N);
		}
	}
	LinearEquationSystem(const LinearEquationSystem& les)
	{
		this->N = les.N;
		equations = new LinearEquation*[N]; //
		for (int i = 0; i< N; i++){
			equations[i] = new LinearEquation(*(les.equations[i]));
		}
	}
	~LinearEquationSystem(){
		for (int i = 0; i< N; i++){
			delete equations[i];
		}
		delete[] equations;
	}
	void setCF(const int row, const int column, const Fraction& set){
		equations[row]->cf[column] = set;
	}
	void setB(const int row, const Fraction& set){
		equations[row]->b = set;
	}
	Fraction getB(const int row)const {
		return equations[row]->b;
	}
	Fraction getCF(const int row, const int col) const {
		return equations[row]->cf[col];
	}
	int getN()const{
		return N;
	}
private:
	LinearEquation** equations; //Equation 줄임말 e
	int N;
	//
	friend class Sol_LinearEquationSystem;
};//
class SolInfo
{
public:
    int id;
    vector<int> vec;
};
class Sol_LinearEquationSystem{
private:
	Sol_LinearEquationSystem(const Sol_LinearEquationSystem&);
	Sol_LinearEquationSystem& operator=(const Sol_LinearEquationSystem&);
private:
	//
	// 구조체 , public 접근 가능.
	int N;
	LeadingEntry leadingEntry;
	LinearEquationSystem* temp_EquationSystem;
	//
	void Interchange(const int low1, const int low2){ // change Two Rows.두줄을 바꾼다. //
		if (low1 == low2){
			return; //최적화..
		}
		LinearEquation temp(N);//
		// coffeient설정.
		//1. temp 에  low1의 데이터를 저장.
		for (int i = 0; i< N; i++){
			temp.cf[i] = temp_EquationSystem->equations[low1]->cf[i];
		}
		temp.b = temp_EquationSystem->equations[low1]->b;
		//
		//2. low1에 low2의 데이터를 저장.
		for (int i = 0; i< N; i++){
			temp_EquationSystem->equations[low1]->cf[i] = temp_EquationSystem->equations[low2]->cf[i];
		}
		temp_EquationSystem->equations[low1]->b = temp_EquationSystem->equations[low2]->b;
		//
		//3. low2에 temp의 데이터를 저장
		for (int i = 0; i< N; i++){
			temp_EquationSystem->equations[low2]->cf[i] = temp.cf[i];
		}
		temp_EquationSystem->equations[low2]->b = temp.b;
		//완료..
	}
	void Replacement(const int low1, const int low2, const Fraction& multiple_Fraction){ // low1 += ( low2 * multiple_Fraction )
		//cout<< "multipleFraction" << multiple_Fraction << endl;
		for (int i = 0; i< N; i++){//
			temp_EquationSystem->equations[low1]->cf[i] += temp_EquationSystem->equations[low2]->cf[i] * multiple_Fraction;
		}
		temp_EquationSystem->equations[low1]->b += temp_EquationSystem->equations[low2]->b * multiple_Fraction;
	}
	void Scaling(const int low1, const Fraction& multiple_Fraction){ // multiple_Fraction is 곱할 수.
		for (int i = 0; i<N; i++){
			if (temp_EquationSystem->equations[low1]->cf[i] != Fraction(false, 0, 1))
				temp_EquationSystem->equations[low1]->cf[i] *= multiple_Fraction;
		}
		if (temp_EquationSystem->equations[low1]->b != Fraction(false, 0, 1))
			temp_EquationSystem->equations[low1]->b *= multiple_Fraction;
		//
	}
	const Fraction getabsolutelyFraction(const Fraction& db){ //cf inline함수. 전체 매크로.
		return ( db >= Fraction( false, 0, 1 ) ) ? db : -db;
	}
	const int GetRow_BigNum(const int nowRow){ // 가장 큰 수의 행을 찾는다.
		//또한, LeadingEntry를 바꾼다. 실시간
		int rowBigNum = nowRow; // 가장 큰수가 있는 행..
		//가장 큰수를 찾는다.
		//모두 0인 경우.
		bool check_ZeroColumn = false;
		///
		do{
			for (int i = nowRow + 1; i< N; i++){
				if (getabsolutelyFraction(temp_EquationSystem->equations[i]->cf[leadingEntry.column])
			> getabsolutelyFraction(temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column])){//
					rowBigNum = i;
				}
			}
			//
			//0이라면, 아래쪽(nowRow부터)으로 모두 0인 컬럼이다. // 즉, rowBigNum == nowRow..
			if (temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column] == Fraction(false, 0, 1)){
				if (leadingEntry.column < N-1 ){ //체크..
					check_ZeroColumn = true;
					leadingEntry.column += 1;//칼럼을 1만큼 증가 시킨다.
				}
				else{ // leadingEntry.column >= VARIABLE_N-1 //
					check_ZeroColumn = false;
					return -1; // 변수가 다된 경우
				}
			}
			else{
				check_ZeroColumn = false;
			}
		} while (check_ZeroColumn == true);
		// 그 값을 리턴 한다.
		//
		return rowBigNum;
	}
	//
	void Debug(){ // Debug 겸 행렬을 프린트 한다.
		for (int i = 0; i< N; i++){
			//cout<< "Debug" << i << " ";
			for (int k = 0; k< N; k++){
				cout << temp_EquationSystem->equations[i]->cf[k] << " ";
			}
			cout << temp_EquationSystem->equations[i]->b;
			cout << endl;
		}
		cout << endl;

	}
	const SolInfo PlayGetSolution(){
		vector<LeadingEntry> vec;
		///Debug();
		//1 Reduced Echolonform을 만든다.
		//InterChange, Replacement, Scaling//
		for (int i = leadingEntry.row; i < N && leadingEntry.column < N; i++){ // i는 행..
			//leading Entry가 존재 할 경우는 i 0,1,2, ... VARIABLE_N-1까지..
			//끝낸다.
          ///  Debug();
			//먼저 leading Entry가 가장 큰 행을 찾는다.
			const int row_BigLeadingEntry = GetRow_BigNum(i);

			if (row_BigLeadingEntry == -1){ // 만약, 절대값이 가장 큰 행을 찾았는데, 0 이라면, 모두 0인 상태.
				//cout<< "ERR" << endl;
				break;
			}
			//그리고 leading Entry가 가장 큰행과 i번째 행을 바꾼다.
			Interchange(i, row_BigLeadingEntry);
			/// chk..
			vec.push_back(leadingEntry);

			//i행에 LeadingEntry가 있다. //열은 다를 수 있다.
			//나머지 행을 Replacement를 한다.
			for (int j = i + 1; j< N; j++){
				// j에 i의 배 한것을 곱한다.
				//Debug();
				//cout<< "j,i" << j+1 << " " << i+1 << endl;
				//cout<< "leadingEntry.column" << leadingEntry.column+1 << endl;
				//크기변화.
				if (temp_EquationSystem->equations[j]->cf[leadingEntry.column] != Fraction(false, 0, 1)){
					Scaling(j, -temp_EquationSystem->equations[i]->cf[leadingEntry.column]
							/ temp_EquationSystem->equations[j]->cf[leadingEntry.column]);

					Replacement(j, i, Fraction(false, 1, 1));
				}// 0이면 안해도 된다.
				// += -( leading Entry가 있는 열의 숫자 ) * ( leading entry의 숫자 / leading Entry가 잇는 열의 숫자)
			}//
			//다음 엔트리의 행과 열..
			leadingEntry.row += 1; // cf.. int& i =leadingEntry.row;
			leadingEntry.column += 1;
		}

		/// chk...
		for (int i = vec.size() - 1; i >= 0; i--)
		{
			if (vec[i].row != vec[i].column) {
                Interchange(vec[i].row, vec[i].column);
			}
		}
		////2. 해를 구한다.
		for (int i = 0; i< N; i++){
			if (temp_EquationSystem->equations[i]->cf[i] != Fraction(false, 0, 1))
				Scaling(i, Fraction(false, 1, 1) / temp_EquationSystem->equations[i]->cf[i]);//자기 자신으로 나눈다.
		}
		///
		for (int i = N - 2; i >= 0; i--){
			//Debug();
			for (int j = 1; j < N - i; j++){
				//자기 자신과 그아래에 있는 것. // 뺀다.
				if( temp_EquationSystem->equations[i+j]->cf[i+j] != Fraction(false, 0, 1) )
                    Replacement(i, i + j, -(temp_EquationSystem->equations[i]->cf[i + j])); ////
			}
		}

		///   a. 0,0, ... , B , but, B != 0인 경우가 있다면 return -1 INCONSIST_SOLUTION
		for (int i = 0; i < N; i++){
			if (temp_EquationSystem->equations[i]->cf[i] == Fraction(false, 0, 1) &&
				temp_EquationSystem->equations[i]->b != Fraction(false, 0, 1)){
                SolInfo siTemp;
                siTemp.id = INCONSISTENT_SOLUTION;
				return siTemp;
			}
		}
		SolInfo si;
		///   b. free Variable이 있는가?
		//        reading Entry가 없는 열이 있따면 return 1; INFINITY_SOLUTION
		for (int i = 0; i< N; i++){ //
            if( temp_EquationSystem->equations[i]->cf[i] == Fraction(false, 0, 1)
                && Fraction(false, 0, 1) == temp_EquationSystem->equations[i]->b ){
                si.id = INFINITY_SOLUTION;
                si.vec.push_back( i );
            }
		}
		for( int i=0; i < N; i++ ) {
            for( int j=i+1; j < N; j++ ) {
                if( ( si.vec.end() != find( si.vec.begin(), si.vec.end(), j ) )
                   && ( temp_EquationSystem->equations[i]->cf[j] != Fraction(false, 0 , 1) )
                )
                {
                    si.vec.push_back( i );
                    break;
                }
            }
		}

		///   c. free Valuable 이 없다면.
		//        모든 행의 처음 숫자를 1로 맞춘다. // 행이 열보다 긴견우, 나머지 밑의 행은 모두 0 인 상태이다.
		if( si.vec.empty() ) { si.id = UNIQUE_SOLUTION; }
		return si; // -1, 0, 1리턴.
	}
	//
public:
	Sol_LinearEquationSystem(){
		temp_EquationSystem = NULL;
	}
	~Sol_LinearEquationSystem(){
		if (temp_EquationSystem){
			delete temp_EquationSystem;
			temp_EquationSystem = NULL;
		}
	}
	// constructor, 소멸자.
	// print .. main function.
	LinearEquationSystem GetSolution(const LinearEquationSystem& equationSystem, SolInfo* op = NULL){
		// Leading Entry 초기화.
		leadingEntry.row = 0; leadingEntry.column = 0;
		this->N = equationSystem.N;
		// 먼저  temp를 초기화 한다.
		if (temp_EquationSystem != NULL){
			delete temp_EquationSystem; temp_EquationSystem = NULL;
		}
		temp_EquationSystem = new LinearEquationSystem(N);
		//
		for (int i = 0; i< N; i++){      //
			(*temp_EquationSystem->equations[i]) = (*equationSystem.equations[i]);
		}
		//
		SolInfo si = PlayGetSolution();
		if (op) {
			*op = si;
		}

		LinearEquationSystem les = *temp_EquationSystem;
		if (temp_EquationSystem != NULL){
			delete temp_EquationSystem; temp_EquationSystem = NULL;
		}
		return les;
	}//
};

int main(void)
{
	/// test
	bool success = false;
	int x=1;
    vector<string> strVec, strVec2;
    string temp;
	cin >> x;
    cin.get();
	for (int i = 0; i < x; i++) {
		getline( cin, temp );
		strVec.push_back( temp );
        getline( cin, temp );
		strVec2.push_back( temp );
		if( i < x -1 ) {
            getline( cin, temp );
		}
	}
	for( int i=0; i < x; i++ ) {
		PolyList polyList1 = g(strVec[i], &success);
		PolyList polyList2 = g(strVec2[i], &success);
        //cout << "------------" << endl;
        if( i > 0 ) {
            cout << endl;
        }
		SolInfo op;
		Sol_LinearEquationSystem solution;
		LinearEquationSystem les(2);

		les.setB(0, Fraction(false, 0, 1));
		les.setB(1, Fraction(false, 0, 1));

		les.setCF(0, 0, Fraction(false, 0, 1));
		les.setCF(0, 1, Fraction(false, 0, 1));
		les.setCF(1, 0, Fraction(false, 0, 1));
		les.setCF(1, 1, Fraction(false, 0, 1));

		///
		for (auto& x : polyList1)
		{
			if (x.exp == 0) {
				les.setB(0, Fraction(x.coeffi > 0, abs( -x.coeffi ), 1));
			}
			else if (x.exp == 1 && x.name == "x") {
				les.setCF(0, 0, Fraction(x.coeffi<0, abs( x.coeffi ), 1));
			}
			else if (x.exp == 1 && x.name == "y") {
				les.setCF(0, 1, Fraction(x.coeffi<0, abs( x.coeffi ), 1));
			}
		}
		for (auto& x : polyList2)
		{
			if (x.exp == 0) {
				les.setB(1, Fraction(x.coeffi > 0, abs( -x.coeffi ), 1));
			}
			else if (x.exp == 1 && x.name == "x") {
				les.setCF(1, 0, Fraction(x.coeffi<0, abs( x.coeffi ), 1));
			}
			else if (x.exp == 1 && x.name == "y") {
				les.setCF(1, 1, Fraction(x.coeffi<0, abs( x.coeffi ), 1));
			}
		}

		les = solution.GetSolution(les, &op);

		if ( UNIQUE_SOLUTION == op.id )
		{
			cout << les.getB(0) << endl;
			cout << les.getB(1);
		}
		else if( INCONSISTENT_SOLUTION == op.id )
        {
            cout << "don't know" << endl;
            cout << "don't know";
        }
        else if( INFINITY_SOLUTION == op.id )
        {
            if( find( op.vec.begin(), op.vec.end(), 0 ) != op.vec.end() )
            {
                cout << "don't know" << endl;
            }
            else{
                cout << les.getB(0) << endl;
            }
            if( find( op.vec.begin(), op.vec.end(), 1 ) != op.vec.end() )
            {
                cout << "don't know";
            }
            else{
                cout << les.getB(1);
            }
        }
        if( i < x-1 ) { cout << endl; }
	}
	return 0;
}
