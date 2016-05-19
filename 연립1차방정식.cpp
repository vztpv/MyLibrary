
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define NUMBER double

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
		this->cf = new NUMBER[N];
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
		cf = new NUMBER[N];
	}
	~LinearEquation(){
		if (cf){
			delete[] cf;
		}
	}
private:
	NUMBER* cf; // coefficient 계수..
	NUMBER b; // cf1*x1 + cf2*x2 + cf3*x3 = b //에서 b
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
	void setCF(const int row, const int column, const NUMBER& set){
		equations[row]->cf[column] = set;
	}
	void setB(const int row, const NUMBER& set){
		equations[row]->b = set;
	}
	NUMBER getB(const int row)const {
		return equations[row]->b;
	}
	NUMBER getCF(const int row, const int col) const {
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
	void Replacement(const int low1, const int low2, const NUMBER& multiple_NUMBER){ // low1 += ( low2 * multiple_NUMBER )
		//cout<< "multipleNUMBER" << multiple_NUMBER << endl;
		for (int i = 0; i< N; i++){//
			temp_EquationSystem->equations[low1]->cf[i] += temp_EquationSystem->equations[low2]->cf[i] * multiple_NUMBER;
		}
		temp_EquationSystem->equations[low1]->b += temp_EquationSystem->equations[low2]->b * multiple_NUMBER;
	}
	void Scaling(const int low1, const NUMBER& multiple_NUMBER){ // multiple_NUMBER is 곱할 수.
		for (int i = 0; i<N; i++){
			if (temp_EquationSystem->equations[low1]->cf[i] != multiple_NUMBER )
				temp_EquationSystem->equations[low1]->cf[i] *= multiple_NUMBER;
		}
		if (temp_EquationSystem->equations[low1]->b != multiple_NUMBER)
			temp_EquationSystem->equations[low1]->b *= multiple_NUMBER;
		//
	}
	const NUMBER getabsolutelyNUMBER(const NUMBER& db){ //cf inline함수. 전체 매크로.
		return (db >= 0) ? db : -db;
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
				if (getabsolutelyNUMBER(temp_EquationSystem->equations[i]->cf[leadingEntry.column])
			> getabsolutelyNUMBER(temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column])){//
					rowBigNum = i;
				}
			}
			//
			//0이라면, 아래쪽(nowRow부터)으로 모두 0인 컬럼이다. // 즉, rowBigNum == nowRow..
			if (temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column] == 0){
				if (leadingEntry.column < N - 1){ //체크..
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
		//Debug();
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
				if (temp_EquationSystem->equations[j]->cf[leadingEntry.column] != 0){
					Scaling(j, -temp_EquationSystem->equations[i]->cf[leadingEntry.column]
							/ temp_EquationSystem->equations[j]->cf[leadingEntry.column]);

					Replacement(j, i, 1);
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
			if (temp_EquationSystem->equations[i]->cf[i] != 0)
				Scaling(i, 1.0 / temp_EquationSystem->equations[i]->cf[i]);//자기 자신으로 나눈다.
		}
		///
		for (int i = N - 2; i >= 0; i--){
			//Debug();
			for (int j = 1; j < N - i; j++){
				//자기 자신과 그아래에 있는 것. // 뺀다.
				if (temp_EquationSystem->equations[i + j]->cf[i + j] != 0)
					Replacement(i, i + j, -(temp_EquationSystem->equations[i]->cf[i + j])); ////
			}
		}

		///   a. 0,0, ... , B , but, B != 0인 경우가 있다면 return -1 INCONSIST_SOLUTION
		for (int i = 0; i < N; i++){
			if (temp_EquationSystem->equations[i]->cf[i] == 0 &&
				temp_EquationSystem->equations[i]->b != 0){
				SolInfo siTemp;
				siTemp.id = INCONSISTENT_SOLUTION;
				return siTemp;
			}
		}
		SolInfo si;
		///   b. free Variable이 있는가?
		//        reading Entry가 없는 열이 있따면 return 1; INFINITY_SOLUTION
		for (int i = 0; i< N; i++){ //
			if (temp_EquationSystem->equations[i]->cf[i] == 0
				&& 0 == temp_EquationSystem->equations[i]->b){
				si.id = INFINITY_SOLUTION;
				si.vec.push_back(i);
			}
		}
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if ((si.vec.end() != find(si.vec.begin(), si.vec.end(), j))
					&& (temp_EquationSystem->equations[i]->cf[j] != 0)
					)
				{
					si.vec.push_back(i);
					break;
				}
			}
		}
		///   c. free Valuable 이 없다면.
		//        모든 행의 처음 숫자를 1로 맞춘다. // 행이 열보다 긴견우, 나머지 밑의 행은 모두 0 인 상태이다.
		if (si.vec.empty()) {
			si.id = UNIQUE_SOLUTION;
		}
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

int main(void) {
	/// test
	Sol_LinearEquationSystem solution;
	LinearEquationSystem les(2);
	SolInfo op;

	les.setB(0, 1);
	les.setB(1, 0);

	les.setCF(0, 0, 1);
	les.setCF(0, 1, 0);
	les.setCF(1, 0, 0);
	les.setCF(1, 1, 0);


	les = solution.GetSolution(les, &op);

	if (UNIQUE_SOLUTION == op.id)
	{
		cout << les.getB(0) << endl;
		cout << les.getB(1);
	}
	else if (INCONSISTENT_SOLUTION == op.id)
	{
		cout << "don't know" << endl;
		cout << "don't know";
	}
	else if (INFINITY_SOLUTION == op.id)
	{
		if (find(op.vec.begin(), op.vec.end(), 0) != op.vec.end())
		{
			cout << "don't know" << endl;
		}
		else{
			cout << les.getB(0) << endl;
		}
		if (find(op.vec.begin(), op.vec.end(), 1) != op.vec.end())
		{
			cout << "don't know";
		}
		else{
			cout << les.getB(1);
		}
	}
	return 0;
}
