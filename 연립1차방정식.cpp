
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
	NUMBER* cf; // coefficient ���..
	NUMBER b; // cf1*x1 + cf2*x2 + cf3*x3 = b //���� b
	int N; // num of cf element
	//
	friend class LinearEquationSystem;
	friend class Sol_LinearEquationSystem;
};
// ���� N��, �ĵ� N���� ��츸 �ٷ��.
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
	LinearEquation** equations; //Equation ���Ӹ� e
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
	// ����ü , public ���� ����.
	int N;
	LeadingEntry leadingEntry;
	LinearEquationSystem* temp_EquationSystem;
	//
	void Interchange(const int low1, const int low2){ // change Two Rows.������ �ٲ۴�. //
		if (low1 == low2){
			return; //����ȭ..
		}
		LinearEquation temp(N);//
		// coffeient����.
		//1. temp ��  low1�� �����͸� ����.
		for (int i = 0; i< N; i++){
			temp.cf[i] = temp_EquationSystem->equations[low1]->cf[i];
		}
		temp.b = temp_EquationSystem->equations[low1]->b;
		//
		//2. low1�� low2�� �����͸� ����.
		for (int i = 0; i< N; i++){
			temp_EquationSystem->equations[low1]->cf[i] = temp_EquationSystem->equations[low2]->cf[i];
		}
		temp_EquationSystem->equations[low1]->b = temp_EquationSystem->equations[low2]->b;
		//
		//3. low2�� temp�� �����͸� ����
		for (int i = 0; i< N; i++){
			temp_EquationSystem->equations[low2]->cf[i] = temp.cf[i];
		}
		temp_EquationSystem->equations[low2]->b = temp.b;
		//�Ϸ�..
	}
	void Replacement(const int low1, const int low2, const NUMBER& multiple_NUMBER){ // low1 += ( low2 * multiple_NUMBER )
		//cout<< "multipleNUMBER" << multiple_NUMBER << endl;
		for (int i = 0; i< N; i++){//
			temp_EquationSystem->equations[low1]->cf[i] += temp_EquationSystem->equations[low2]->cf[i] * multiple_NUMBER;
		}
		temp_EquationSystem->equations[low1]->b += temp_EquationSystem->equations[low2]->b * multiple_NUMBER;
	}
	void Scaling(const int low1, const NUMBER& multiple_NUMBER){ // multiple_NUMBER is ���� ��.
		for (int i = 0; i<N; i++){
			if (temp_EquationSystem->equations[low1]->cf[i] != multiple_NUMBER )
				temp_EquationSystem->equations[low1]->cf[i] *= multiple_NUMBER;
		}
		if (temp_EquationSystem->equations[low1]->b != multiple_NUMBER)
			temp_EquationSystem->equations[low1]->b *= multiple_NUMBER;
		//
	}
	const NUMBER getabsolutelyNUMBER(const NUMBER& db){ //cf inline�Լ�. ��ü ��ũ��.
		return (db >= 0) ? db : -db;
	}
	const int GetRow_BigNum(const int nowRow){ // ���� ū ���� ���� ã�´�.
		//����, LeadingEntry�� �ٲ۴�. �ǽð�
		int rowBigNum = nowRow; // ���� ū���� �ִ� ��..
		//���� ū���� ã�´�.
		//��� 0�� ���.
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
			//0�̶��, �Ʒ���(nowRow����)���� ��� 0�� �÷��̴�. // ��, rowBigNum == nowRow..
			if (temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column] == 0){
				if (leadingEntry.column < N - 1){ //üũ..
					check_ZeroColumn = true;
					leadingEntry.column += 1;//Į���� 1��ŭ ���� ��Ų��.
				}
				else{ // leadingEntry.column >= VARIABLE_N-1 //
					check_ZeroColumn = false;
					return -1; // ������ �ٵ� ���
				}
			}
			else{
				check_ZeroColumn = false;
			}
		} while (check_ZeroColumn == true);
		// �� ���� ���� �Ѵ�.
		//
		return rowBigNum;
	}
	//
	void Debug(){ // Debug �� ����� ����Ʈ �Ѵ�.
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
		//1 Reduced Echolonform�� �����.
		//InterChange, Replacement, Scaling//
		for (int i = leadingEntry.row; i < N && leadingEntry.column < N; i++){ // i�� ��..
			//leading Entry�� ���� �� ���� i 0,1,2, ... VARIABLE_N-1����..
			//������.
			///  Debug();
			//���� leading Entry�� ���� ū ���� ã�´�.
			const int row_BigLeadingEntry = GetRow_BigNum(i);

			if (row_BigLeadingEntry == -1){ // ����, ���밪�� ���� ū ���� ã�Ҵµ�, 0 �̶��, ��� 0�� ����.
				//cout<< "ERR" << endl;
				break;
			}
			//�׸��� leading Entry�� ���� ū��� i��° ���� �ٲ۴�.
			Interchange(i, row_BigLeadingEntry);
			/// chk..
			vec.push_back(leadingEntry);

			//i�࿡ LeadingEntry�� �ִ�. //���� �ٸ� �� �ִ�.
			//������ ���� Replacement�� �Ѵ�.
			for (int j = i + 1; j< N; j++){
				// j�� i�� �� �Ѱ��� ���Ѵ�.
				//Debug();
				//cout<< "j,i" << j+1 << " " << i+1 << endl;
				//cout<< "leadingEntry.column" << leadingEntry.column+1 << endl;
				//ũ�⺯ȭ.
				if (temp_EquationSystem->equations[j]->cf[leadingEntry.column] != 0){
					Scaling(j, -temp_EquationSystem->equations[i]->cf[leadingEntry.column]
							/ temp_EquationSystem->equations[j]->cf[leadingEntry.column]);

					Replacement(j, i, 1);
				}// 0�̸� ���ص� �ȴ�.
				// += -( leading Entry�� �ִ� ���� ���� ) * ( leading entry�� ���� / leading Entry�� �մ� ���� ����)
			}//
			//���� ��Ʈ���� ��� ��..
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
		////2. �ظ� ���Ѵ�.
		for (int i = 0; i< N; i++){
			if (temp_EquationSystem->equations[i]->cf[i] != 0)
				Scaling(i, 1.0 / temp_EquationSystem->equations[i]->cf[i]);//�ڱ� �ڽ����� ������.
		}
		///
		for (int i = N - 2; i >= 0; i--){
			//Debug();
			for (int j = 1; j < N - i; j++){
				//�ڱ� �ڽŰ� �׾Ʒ��� �ִ� ��. // ����.
				if (temp_EquationSystem->equations[i + j]->cf[i + j] != 0)
					Replacement(i, i + j, -(temp_EquationSystem->equations[i]->cf[i + j])); ////
			}
		}

		///   a. 0,0, ... , B , but, B != 0�� ��찡 �ִٸ� return -1 INCONSIST_SOLUTION
		for (int i = 0; i < N; i++){
			if (temp_EquationSystem->equations[i]->cf[i] == 0 &&
				temp_EquationSystem->equations[i]->b != 0){
				SolInfo siTemp;
				siTemp.id = INCONSISTENT_SOLUTION;
				return siTemp;
			}
		}
		SolInfo si;
		///   b. free Variable�� �ִ°�?
		//        reading Entry�� ���� ���� �ֵ��� return 1; INFINITY_SOLUTION
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
		///   c. free Valuable �� ���ٸ�.
		//        ��� ���� ó�� ���ڸ� 1�� �����. // ���� ������ ��߿�, ������ ���� ���� ��� 0 �� �����̴�.
		if (si.vec.empty()) {
			si.id = UNIQUE_SOLUTION;
		}
		return si; // -1, 0, 1����.
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
	// constructor, �Ҹ���.
	// print .. main function.
	LinearEquationSystem GetSolution(const LinearEquationSystem& equationSystem, SolInfo* op = NULL){
		// Leading Entry �ʱ�ȭ.
		leadingEntry.row = 0; leadingEntry.column = 0;
		this->N = equationSystem.N;
		// ����  temp�� �ʱ�ȭ �Ѵ�.
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
