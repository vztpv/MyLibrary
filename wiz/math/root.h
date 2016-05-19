#ifndef ROOT_H_INCLUDED
#define ROOT_H_INCLUDED

#include <cmath>
//#include "global.h"
#include <wiz/newArrays.h>
/// using template? NUMBER, INTEGER, F, DF <- template typename??
namespace wiz{
inline NUMBER ES( INTEGER sf ){ // significant figure
    return 0.5 * pow( 10, 2 - sf ); // ���� %
}

class Polinomial{
private:
    NUMBER coeffi;
    INTEGER exp;
public:
    Polinomial( NUMBER coeffi=0, NUMBER exp=0 ){
        this->coeffi = coeffi; this->exp = exp;
    }
public:
    NUMBER getX( NUMBER x )const{
        if( exp == 0 ){ return coeffi; }
        return coeffi * pow( x, exp );
    }
    Polinomial getDiffential()const{
        Polinomial temp;
        //
        if( this->exp != 0 ){
            temp.coeffi = this->coeffi * this->exp;
            temp.exp = this->exp - 1;
        }
        else{// temp.exp == 0 ������� ���. �̺��ϸ� 0�� �ȴ�.
            temp.coeffi = 0;
            temp.exp = 0;
        }
        //
        return temp;
    }
    NUMBER getCoeffi()const{ return coeffi; }
    NUMBER getExp()const{ return exp; }
};
namespace root{
//
/**
* Part1 �Ͽ��������� ��.
*/
class UsingBisect{
public:
    virtual NUMBER get(  NUMBER xl,  NUMBER xu,
               const NUMBER es, const NUMBER imax, INTEGER& iter, NUMBER& ea,  const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER fl, fr;
         NUMBER xr=0;
         NUMBER xrold;
         NUMBER test;
        //
        iter = 0;
        fl = f( xl );
        //Do
        while( true ){
            xrold = xr; // before_xr
            xr = ( xl + xu ) / 2; // mean of xl and xux
            fr = f( xr );
            //
            iter = iter + 1;
            /// ���� ��ó������ xrold�� 0�� ���� �ؼ�
            /// 100%�� ������ �Ͽ���.
            if( xr != 0 ){ // ea calcul
                ea = abs( ( xr - xrold ) / xr ) * 100; //
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }

            }// end if
            test = fl * fr;
            //  if, root exist in [xl,xu]
            if( test < 0 ){
                xu = xr; // ���� right->left
            }else if( test > 0 ){
                xl = xr;// ���� left->right
                fl = fr;
            }
            else{// find right root ( think Floating Point Number`s Round-Off Error )
                ea = 0;
            }// end if
            //
            if( ea < es || iter >= imax ){
                break;
            }
        }
        //
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0; // ���� ���� �Լ�.
private:
    //
};
class UsingFalsePosition{
public:
    virtual NUMBER get(  NUMBER xl,  NUMBER xu,
                const NUMBER es, const NUMBER imax, INTEGER& iter, NUMBER& ea, const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER fl, fu;
         NUMBER xrold;
         NUMBER fr;
         NUMBER xr=0; // ���� !! ó�� ea�� 100���� �Ѵ�.
         NUMBER test;
         NUMBER il = 0, iu = 0; // initial..
        //
        fl = f( xl );
        fu = f( xu );
        //
        iter = 0;
        //Do
        while( true ){
            xrold = xr;//
            xr = xu - fu * ( xl - xu  ) / ( fl - fu );
            fr = f( xr );
            //
            iter = iter + 1;
            //
            if( xr != 0 ){
                ea = abs( ( xr - xrold ) / xr ) * 100; // %
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }
            }// end if
            //
            test = fl * fr;
            // if root exist in [fl,fr]
            if( test < 0 ){
                xu = xr;
                fu = f( xu );
                iu = 0;
                il = il + 1;
                if( il >= 2 ){ fl = fl / 2; }
            }
            // may root don`t exist in [fl,fr],
            // so Let`s find root in [fr,fu]
            else if( test > 0 ){
                xl = xr;
                fl = f( xl );
                il = 0;
                iu = iu + 1;
                if( iu >= 2 ){ fu = fu / 2; }
            }
            // Floating Point Number`s Round-Off Error����������
            // ��ġ�Ѵ�.
            else{ // Find maybe Correct ROOT!!
                ea = 0;
            }// end if
            //
            if( ea < es || iter >= imax ){ break; }
        }
        //
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0;
private:
};
class UsingFixedPoint{
public:
    virtual NUMBER get(  NUMBER x0, const NUMBER es,
                       const NUMBER imax, INTEGER& iter, NUMBER& ea, const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER xr=x0;
         NUMBER xrold;
        //
        iter = 0;
        // Do
        while( true ){
            xrold = xr;
            xr = g( xrold );
            //
            iter = iter + 1;
            //
            if( xr != 0 ){
                ea = abs( ( xr - xrold ) / xr ) * 100;
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }
            }// end if
            //
            if( ea < es || iter >= imax ){ break; }
        }
        //
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0;
private:
    NUMBER g( NUMBER xl ){
        return f( xl ) + xl; //
    }
};
class UsingNewton{
public:
    virtual NUMBER get( NUMBER x0, const NUMBER es,
                       const NUMBER imax, INTEGER& iter, NUMBER& ea, const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER xrold; // before_val
         NUMBER xr=x0;
        //
        iter = 0;
        //Do
        while( true ){
            xrold = xr;
            //
            if( 0 == df( xrold ) ){
                //  cout << "df(xr) is 0 error" << endl;
               // exit( -333 );
               throw  "df(xr) is 0 error";
            }
            xr = xrold - ( f( xrold ) / df( xrold ) );
            //
            iter = iter + 1;
            //
            if( xr != 0 ){
                ea = abs( ( xr - xrold ) / xr ) * 100;
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }
            }// end if
            // end condition
            if( ea < es || iter >= imax ){ break; }
        }
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0;
    virtual NUMBER df( NUMBER x1 )=0; // f` (f�̺�)
private:
};
class UsingModifiedSecant{
public:
    virtual NUMBER get( NUMBER x0,  NUMBER e, const NUMBER es,
                       const NUMBER imax, INTEGER& iter, NUMBER& ea, const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER xrold; // before_val
         NUMBER xr=x0;
        //
        iter = 0;
        //Do
        while( true ){
            xrold = xr;
            //
            if( 0 == df( xrold, e ) ){
                throw "df(xr) is 0 error";
            }
            xr = xrold - ( f( xrold ) / df( xrold, e ) );
            //
            iter = iter + 1;
            //
            if( xr != 0 ){
                ea = abs( ( xr - xrold ) / xr ) * 100;
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }
            }// end if
            // end condition
            if( ea < es || iter >= imax ){ break; }
        }
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0;
    NUMBER df( NUMBER xl, NUMBER e ){
        return ( f( xl + e*xl ) - f( xl ) ) / ( e*xl );
    }
private:
};
class UsingSecant{
public:
    virtual NUMBER get(  NUMBER x0,  NUMBER x1, const NUMBER es,
                       const NUMBER imax, INTEGER& iter, NUMBER& ea, const NUMBER realVal=0, NUMBER* et = NULL ){
         NUMBER xrold; // before_val
         NUMBER xr=0;
        //
        iter = 0;
        //Do
        while( true ){
            xrold = xr;
            //
            if( 0 == df( x0, x1 ) ){
                throw "df(xr) is 0 error";
            }
            xr = x1 - f( x1 ) / df( x0, x1 ) ; // x0 xoldold, x1 xold, xr  xnew
            //
            x0 = x1; x1 = xr;
            //
            iter = iter + 1;
            //
            if( xr != 0 ){
                ea = abs( ( xr - xrold ) / xr ) * 100;
                if( et != NULL || realVal != 0 ){
                    *et = abs( ( realVal - xr ) / realVal ) * 100;
                }
            }// end if
            // end condition
            if( ea < es || iter >= imax ){ break; }
        }
        return xr;
    }
protected:
    virtual NUMBER f( NUMBER xl )=0;
private:
    NUMBER df( NUMBER x1, NUMBER x2 ){
        if( x1 != x2 ){
            return ( f( x1 ) - f( x2 ) ) / ( x1 - x2 );
        }else{
            throw "df error x1 == x2 ";
        }
    }
};
/**
* To Do
* UsingBairstow
* make Array<NUMBER> Version!!
*/

class UsingBairstow{
public:
    static void get( NUMBER* a,const INTEGER nn, const NUMBER es,
                        const NUMBER rr, const NUMBER ss,
                        const INTEGER maxit, NUMBER* re, NUMBER* im, INTEGER* ier ){
        NUMBER r, s; INTEGER n;
        NUMBER *b, *c; // both dynamic allocation array
        NUMBER ea1, ea2;
        INTEGER iter;
        // make array b,c, size is nn( a`size )
        b = new NUMBER[nn]; c = new NUMBER[nn];
        //  r, s, n, initial
        r = rr, s = ss, n = nn-1; // nn���� ������� ���ԵǾ� �ִ�.
        /// ter?? , ea1, ea2 initial
        ea1 = 1; ea2 = 1;
        if( *ier ) *ier = 0;
        iter = 0;
        // Do
        while( true ){
            // n < 3�̸� ���� �����̹Ƿ� ���� ������ �̿��� �� �����Ƿ� �����Ѵ�.
            if( n < 3 || iter >= maxit ){
                break;
            }
            iter = 0;
            // solve Equations
            while( true ){
                iter = iter + 1;
                b[n] = a[n];
                b[n-1] = a[n-1] + r * b[n];
                c[n] = b[n];
                c[n-1] = b[n-1] + r * c[n];
                // for
                for( int i=n-2; i>=0; i-- ){
                    b[i] = a[i] + r * b[i+1] + s * b[i+2];
                    c[i] = b[i] + r * c[i+1] + s * c[i+2];
                }
                NUMBER det = c[2] * c[2] - c[3] * c[1];
                if( 0 != det ){
                    NUMBER dr, ds;
                    dr = ( -b[1] * c[2] + b[0] * c[3] ) / det; //
                    ds = ( -b[0] * c[2] + b[1] * c[1] ) / det;
                    r = r + dr;
                    s = s + ds;
                    if( 0 != r ){
                        ea1 = abs( dr/r ) * 100;
                    }
                    if( 0 != s ){
                        ea2 = abs( ds/s ) * 100;
                    }
                    else{
                        r = r + 1;
                        s = s + 1;
                        iter = 0;
                    }
                    if( ( ea1 <= es && ea2 <= es ) || iter >= maxit ){
                        break;
                    }
                }
                //
            }// end while
            // print to real[], im[]
            NUMBER r1, i1, r2, i2;
            Quadroot( r, s, r1, i1, r2, i2 );
            //
            re[n-1] = r1;
            im[n-1] = i1;
            re[n-2] = r2;
            im[n-2] = i2;
            //
            n = n - 2;
            //
            for( int i=0;i<= n;i++ ){
                a[i] = b[i+2];
            }
        }
        if( iter < maxit ){
            if( 2 == n ){
                r = -a[1]/a[2];
                s = -a[0]/a[2];
                NUMBER r1, i1, r2, i2;
                Quadroot( r, s, r1, i1, r2, i2 );
                //
                re[n-1] = r1;
                im[n-1] = i1;
                re[n-2] = r2;
                im[n-2] = i2;
            }
            else{
                re[n-1] = -a[0]/a[1];
                im[n-1] = 0;
            }
        }
        else{
            if( *ier ) *ier = 1;
        }
        delete[] b;
        delete[] c;
        //cout << "iter is " << iter << endl;
    }
    static void get( Array<NUMBER> a, const INTEGER nn, const NUMBER es,
                        const NUMBER rr, const NUMBER ss,
                        const INTEGER maxit, Array<NUMBER>& re, Array<NUMBER>& im, INTEGER* ier ){
        get( &(a[0]), nn, es, rr, ss, maxit, &(re[0]), &(im[0]), ier );
    }
private:
    static void Quadroot( const NUMBER r, const NUMBER s,
                        NUMBER& r1, NUMBER& i1, NUMBER& r2, NUMBER& i2 ){
        NUMBER disc = r * r + 4 * s;
        if( disc > 0 ){
            r1 = ( r + SQRT( disc ) ) / 2;
            r2 = ( r - SQRT( disc ) ) / 2;
            i1 = 0;
            i2 = 0;
        }
        else{
            r1 = r / 2;
            r2 = r1;
            i1 = SQRT( abs( disc ) ) / 2;
            i2 = -i1;
        }
    }
};
/**
* Part 2 ���� ���� �������� ��.
* Gauss �ҰŹ�
*/
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
    LinearEquation( const LinearEquation& p )
    {
        this->N = p.N;
        this->cf = new NUMBER[N];
        *this = p;
    }

    LinearEquation& operator=( const LinearEquation& p ){
        if( p.N != this->N ){ throw "p.N is not equal with this->N "; }
        this->b = p.b;
        for( int i=0;i< this->N;i++ ){
            this->cf[i] = p.cf[i];
        }
        return *this;
    }
        //
     LinearEquation( const int N ) : N(N){
        cf = new NUMBER[ N ];
        for( int i=0;i< N;i++ ){
            cf[i] = 0; // initial.. check..
        }
    }
    ~LinearEquation(){
        if( cf ){ delete[] cf; }
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
    LinearEquationSystem& operator=( const LinearEquationSystem& les )
    {
        LinearEquationSystem temp( les );
        if( this->N != temp.N ) {
            for( int i=0; i < this->N; i++ )
            {
                delete equations[i];
            }
            delete[] equations;

            equations = new LinearEquation*[temp.N];
            for( int i=0; i < temp.N; i++ ) {
                equations[i] = new LinearEquation(temp.N);
            }
            this->N = temp.N;
        }
        for(int i=0; i < this->N; i++ ) {
            *(equations[i]) = *(temp.equations[i]);
        }
        return *this;
    }

    LinearEquationSystem( const int N ){
        this->N = N;
        equations = new LinearEquation*[N]; //
        for( int i=0;i< N;i++ ){
            equations[i] = new LinearEquation( N );
        }
    }
    LinearEquationSystem( const LinearEquationSystem& les )
    {
        this->N = les.N;
        equations = new LinearEquation*[N]; //
        for( int i=0;i< N;i++ ){
            equations[i] = new LinearEquation( *(les.equations[i]) );
        }
    }
    ~LinearEquationSystem(){
        for( int i=0;i< N;i++ ){
            delete equations[i];
        }
        delete[] equations;
    }
    void setCF( const int row, const int column, const NUMBER set ){
        equations[row]->cf[column] = set;
    }
    void setB( const int row, const NUMBER set ){
        equations[row]->b = set;
    }
    NUMBER getB( const int row )const { return equations[row]->b; }
private:
    LinearEquation** equations; //Equation ���Ӹ� e
    int N;
    //
    friend class Sol_LinearEquationSystem;
};//
//
class Sol_LinearEquationSystem{ /// To Do..
private:
    Sol_LinearEquationSystem( const Sol_LinearEquationSystem()& );
    Sol_LinearEquationSystem& operator=( const Sol_LinearEquationSystem& );
private:
    //
// ����ü , public ���� ����.
    int N;
    LeadingEntry leadingEntry;
    LinearEquationSystem* temp_EquationSystem;
    //
    void Interchange( const int low1, const int low2 ){ // change Two Rows.������ �ٲ۴�. //
        if( low1 == low2 ){
            return; //����ȭ..
        }
        LinearEquation temp( N );//
        // coffeient����.
        //1. temp ��  low1�� �����͸� ����.
        for( int i=0;i< N;i++ ){
            temp.cf[i] = temp_EquationSystem->equations[low1]->cf[i];
        }
        temp.b = temp_EquationSystem->equations[low1]->b;
        //
        //2. low1�� low2�� �����͸� ����.
        for( int i=0;i< N;i++ ){
            temp_EquationSystem->equations[low1]->cf[i] = temp_EquationSystem->equations[low2]->cf[i];
        }
        temp_EquationSystem->equations[low1]->b = temp_EquationSystem->equations[low2]->b;
        //
        //3. low2�� temp�� �����͸� ����
        for( int i=0;i< N;i++ ){
            temp_EquationSystem->equations[low2]->cf[i] = temp.cf[i];
        }
        temp_EquationSystem->equations[low2]->b = temp.b;
        //�Ϸ�..
    }
    void Replacement( const int low1, const int low2, const NUMBER multiple_number ){ // low1 += ( low2 * multiple_number )
        //cout<< "multipleNUmber" << multiple_number << endl;
        for( int i=0;i< N;i++){//
            temp_EquationSystem->equations[low1]->cf[i] += temp_EquationSystem->equations[low2]->cf[i] * multiple_number;
        }
        temp_EquationSystem->equations[low1]->b += temp_EquationSystem->equations[low2]->b * multiple_number;
    }
    void Scaling( const int low1, const NUMBER multiple_number ){ // multiple_number is ���� ��.
        for( int i=0;i<N;i++){
            if( temp_EquationSystem->equations[low1]->cf[i] !=0 )
                temp_EquationSystem->equations[low1]->cf[i] *= multiple_number;
        }
        if( temp_EquationSystem->equations[low1]->b != 0 )
            temp_EquationSystem->equations[low1]->b *= multiple_number;
        //
    }
    const NUMBER getabsolutelyNUMBER( const NUMBER db ){ //cf inline�Լ�. ��ü ��ũ��.
        return ( db >= 0 )? db : -db;
    }
    const int GetRow_BigNum( const int nowRow ){ // ���� ū ���� ���� ã�´�.
        //����, LeadingEntry�� �ٲ۴�. �ǽð�
        int rowBigNum = nowRow; // ���� ū���� �ִ� ��..
        //���� ū���� ã�´�.
        //��� 0�� ���.
        bool check_ZeroColumn = false;
        ///
        do{
            for( int i=nowRow+1; i< N;i++ ){ //�̶�, nowRow, i�� 0���� �����Կ� ����...
                if( getabsolutelyNUMBER( temp_EquationSystem->equations[i]->cf[leadingEntry.column] )
                   > getabsolutelyNUMBER( temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column] ) ){//
                    rowBigNum = i;
                }
            }
            //
            //0�̶��, �Ʒ���(nowRow����)���� ��� 0�� �÷��̴�. // ��, rowBigNum == nowRow..
            if( temp_EquationSystem->equations[rowBigNum]->cf[leadingEntry.column] == 0 ){
                if( leadingEntry.column < N-1 ){ //üũ..
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
        }while( check_ZeroColumn == true);
        // �� ���� ���� �Ѵ�.
        //
        return rowBigNum;
    }
    //
   void Debug(){ // Debug �� ����� ����Ʈ �Ѵ�.
     for( int i=0;i< N;i++){
            //cout<< "Debug" << i << " ";
            for( int k=0;k< N;k++ ){
                cout<< temp_EquationSystem->equations[i]->cf[k] << " ";
            }
            cout<< temp_EquationSystem->equations[i]->b;
            cout<< endl;
        }
        cout<< endl;

    }
    const int PlayGetSolution(){
        //Debug();
        //1 Reduced Echolonform�� �����.
        //InterChange, Replacement, Scaling//
        for( int i = leadingEntry.row; i < N && leadingEntry.column < N; i++ ){ // i�� ��..
            //leading Entry�� ���� �� ���� i 0,1,2, ... VARIABLE_N-1����..
            //������.

            //���� leading Entry�� ���� ū ���� ã�´�.
            const int row_BigLeadingEntry = GetRow_BigNum(i);
            //
            if( row_BigLeadingEntry == -1 ){ // ����, ���밪�� ���� ū ���� ã�Ҵµ�, 0 �̶��, ��� 0�� ����.
                //cout<< "ERR" << endl;
                break;
            }
            //�׸��� leading Entry�� ���� ū��� i��° ���� �ٲ۴�.
            Interchange( i, row_BigLeadingEntry );

            //i�࿡ LeadingEntry�� �ִ�. //���� �ٸ� �� �ִ�.
            //������ ���� Replacement�� �Ѵ�.
            for( int j=i+1;j<  N ; j++ ){
                // j�� i�� �� �Ѱ��� ���Ѵ�.
                //Debug();
                //cout<< "j,i" << j+1 << " " << i+1 << endl;
                //cout<< "leadingEntry.column" << leadingEntry.column+1 << endl;
                //ũ�⺯ȭ.
                if( temp_EquationSystem->equations[j]->cf[leadingEntry.column] != 0){
                    Scaling( j, -temp_EquationSystem->equations[i]->cf[leadingEntry.column]
                                /temp_EquationSystem->equations[j]->cf[leadingEntry.column] );

                    Replacement( j, i, 1 );
                }// 0�̸� ���ص� �ȴ�.
                // += -( leading Entry�� �ִ� ���� ���� ) * ( leading entry�� ���� / leading Entry�� �մ� ���� ����)
            }//
            //���� ��Ʈ���� ��� ��..
            leadingEntry.row += 1; // cf.. int& i =leadingEntry.row;
            leadingEntry.column += 1;
        }
        ////2. �ظ� ���Ѵ�.
        for( int i=0;i< N; i++ ){
            if( temp_EquationSystem->equations[i]->cf[i] != 0 )
                Scaling( i, 1.0/temp_EquationSystem->equations[i]->cf[i] );//�ڱ� �ڽ����� ������.
        }
        ///
        for( int i=N-2;i>=0;i-- ){
            Debug();
            for( int j=1;j< N-i ;j++ ){
                //�ڱ� �ڽŰ� �׾Ʒ��� �ִ� ��. // ����.
                Replacement( i, i+j, -(temp_EquationSystem->equations[i]->cf[i+j]) ); ////
            }
        }
        Debug();
        //
        ///   a. 0,0, ... , B , but, B != 0�� ��찡 �ִٸ� return -1 INCONSIST_SOLUTION
        for( int i=0;i < N; i++ ){
            if( temp_EquationSystem->equations[i]->cf[i] == 0 &&
                    temp_EquationSystem->equations[ i ]->b != 0 ){
                        return INCONSISTENT_SOLUTION;
            }
        }
        ///   b. free Variable�� �ִ°�?
        //        reading Entry�� ���� ���� �ֵ��� return 1; INFINITY_SOLUTION
        for( int i=0;i< N;i++ ){ //
            if( temp_EquationSystem->equations[i]->cf[i] == 0
               && 0 == temp_EquationSystem->equations[ i ]->b ){
                return INFINITY_SOLUTION;
            }
        }
        ///   c. free Valuable �� ���ٸ�.
        //        ��� ���� ó�� ���ڸ� 1�� �����. // ���� ������ ��߿�, ������ ���� ���� ��� 0 �� �����̴�.
        return UNIQUE_SOLUTION; // -1, 0, 1����.
    }
    //
public:
    Sol_LinearEquationSystem(){
        temp_EquationSystem = NULL;
    }
    ~Sol_LinearEquationSystem(){
        if( temp_EquationSystem ){
            delete temp_EquationSystem;
            temp_EquationSystem = NULL;
        }
    }
    // constructor, �Ҹ���.
    // print .. main function.
    LinearEquationSystem GetSolution( const LinearEquationSystem& equationSystem, int* op=NULL ){
        // Leading Entry �ʱ�ȭ.
        leadingEntry.row=0; leadingEntry.column=0;
        this->N = equationSystem.N;
        // ����  temp�� �ʱ�ȭ �Ѵ�.
        if( temp_EquationSystem != NULL ){ delete temp_EquationSystem; temp_EquationSystem = NULL; }
        temp_EquationSystem = new LinearEquationSystem( N );
        //
        for( int i=0; i< N; i++ ){      //
            ( *temp_EquationSystem->equations[i] ) = ( *equationSystem.equations[i] );
        }
        //
        int ival = PlayGetSolution();
        if( op ) { *op = ival; }

        LinearEquationSystem les = *temp_EquationSystem;
        if( temp_EquationSystem != NULL ){ delete temp_EquationSystem; temp_EquationSystem = NULL; }
        return les;
    }//
};
/**
* To Do
* naive Gauss �ҰŹ�.
* make SquareSecondArray Version!! ( SecondArray�� ��ӹ�����. )
*/
/**
*
*/
void naiveGauss( SecondArray<NUMBER> a, Array<NUMBER> b, const INTEGER n, Array<NUMBER>& x, INTEGER& er ){
     NUMBER factor;
     NUMBER sum;

    /* forward elimination */
    for( int k=0;k< n-1;k++ ){
        for( int i=k+1;i< n;i++ ){
            if( 0 == a[k][k] ){ er = 1; return; }
            factor = a[i][k] / a[k][k];

            for( int j=k+1;j< n;j++ ){
                a[i][j] = a[i][j] - factor * a[k][j];
            }
            b[i] = b[i] - factor * b[k];
        }
    }
    /* back substitution */

    if( 0 == a[n-1][n-1] ){ er = 1; return; }
    x[n-1] = b[n-1] / a[n-1][n-1];

    //
    for( int i=n-2; i >= 0; i-- ){
        sum = b[i];

        for( int j=i+1;j< n;j++ ){
            sum = sum - a[i][j] * x[j];
        }
        if( 0 == a[i][i] ){ er = 1; return; }
        x[i] = sum / a[i][i];
    }
}

/***
* To Do
* Part2-2 ���� ���� �������� �� and �����.
* Crout`s LU decomposition algorithm.
* make SquareSecondArray Version!! ( SecondArray�� ��ӹ�����. )
*/

class CroutLU{
public:
    static void get( SecondArray<NUMBER>& a, Array<NUMBER>& b, const INTEGER n, Array<NUMBER>& x,INTEGER* er ){
        Decomposition( a, n, er ); //

        if( er && 0 != *er ){
            return;
        }
        Substitute( a, b, n, x, er );

        if( er && 0 != *er ){
            return;
        }
    }
private:
    static void Decomposition( SecondArray<NUMBER>& a, const INTEGER n, INTEGER* er ){ // a is n*n matrix
        NUMBER sum = 0;
        if(er) *er = 0;
        /* prefare */
        for( int j=1;j < n;j++ ){
            if( 0 == a[0][0] ){ if(er) *er = 1; return; }
            a[0][j] = a[0][j] / a[0][0];
        }
        //
        for( int j=1;j < n-1;j++ ){
            for( int i=j;i < n;i++ ){
                sum = 0;
                for( int k=0;k <= j-1;k++ ){
                    sum = sum + a[i][k] * a[k][j]; //
                }
                a[i][j] = a[i][j] - sum;
            }
            for( int k=j+1; k < n; k++ ){
                sum = 0;
                for( int i=0; i <= j-1; i++ ){
                    sum = sum + a[j][i] * a[i][k];
                }
                if( 0 == a[j][j] ){ if(er) *er=2; return; }
                a[j][k] = ( a[j][k] - sum ) / a[j][j];
            }
        }
        sum = 0;
        for( int k=0; k < n-1;k++ ){
            sum = sum + a[n-1][k] * a[k][n-1];
        }
        a[n-1][n-1] = a[n-1][n-1] - sum;
    }
public:
    static void Substitute( SecondArray<NUMBER>& a, Array<NUMBER>& b, const INTEGER n, Array<NUMBER>& x, INTEGER* er ){
        NUMBER sum = 0;
        /* forward substitution */
        if(er) *er = 0;
        if( 0 == a[0][0] ){ if(er) *er = 3; return; }
        b[0] = b[0] / a[0][0];
        for( int i=1; i< n; i++ ){
            sum = b[i];
            for( int j=0; j<= i-1; j++ ){
                sum = sum - a[i][j] * b[j];
            }
            if( 0 == a[i][i] ){ if(er) *er = 4; return; }
            b[i] = sum / a[i][i];
        }

        /* backward substitution */
        x[n-1] = b[n-1];
       // cout << "x[n-1] is " << x[n-1] << endl;
        for( int i=n-2; i >= 0; i-- ){
            sum = 0;
            for( int j= n-1; j >= i+1; j-- ){
                sum = sum + a[i][j] * x[j];
              //  cout << "sum is " << sum << " ";
            }
           // cout << endl;
            x[i] = ( b[i] - sum );
        }
    }
};
SecondArray<NUMBER> multiple( const SecondArray<NUMBER>& a, const SecondArray<NUMBER>& b ){
    if( a.getColumnN() != b.getRowN() ){ throw "can not multiple(of matrix)"; }//cout << "���� ���� ���� ����.." << endl; exit( -55324 ); } // ����!!
    SecondArray<NUMBER> temp( a.getRowN(), b.getColumnN(), 0 );
     NUMBER sum = 0;
    for( int i=0;i< temp.getRowN();i++ ){
        for( int j=0;j< temp.getColumnN();j++ ){
            sum = 0;
            for( int k=0;k< a.getColumnN();k++ ){
                sum = sum + a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    return temp;
}
SquareSecondArray<NUMBER> multiple( const SquareSecondArray<NUMBER>& a, const SquareSecondArray<NUMBER>& b ){
    if( a.getColumnN() != b.getRowN() ){ throw "can not multiple(of matrix)"; }//cout << "���� ���� ���� ����.." << endl; exit( -55324 ); } // ����!!
    SquareSecondArray<NUMBER> temp( a.getRowN(), 0 );
     NUMBER sum = 0;
    for( int i=0;i< temp.getRowN();i++ ){
        for( int j=0;j< temp.getColumnN();j++ ){
            sum = 0;
            for( int k=0;k< a.getColumnN();k++ ){
                sum = sum + a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    return temp;
}
SquareSecondArray<NUMBER> getInverseMatrix( const SquareSecondArray<NUMBER>& A, INTEGER* er ) {
    /* if doesn`t exist inverse of A */
    if( A.getRowN() != A.getColumnN() ){ throw "A.getRowN !=  a.getColumnN";  }
    // and, need other solution..
    int N = A.getRowN();
    SquareSecondArray<NUMBER> temp = A;
    SquareSecondArray<NUMBER> E( N, 0 );// inverse of E is E!!
    SquareSecondArray<NUMBER> IA( N, -1 );

    // initial E!
    for( int i=0;i< N;i++ ){
        E[i][i] = 1;
    }
    //
    CroutLU::get( temp, E[0], N, IA[0], er );
    if( er && *er != 0 ) { return IA; }
    for( int i=1;i< N;i++ ){
        CroutLU::Substitute( temp, E[i], N, IA[i], er );
        if( er && *er !=0 ) { return IA; }
    }
    IA.Transpose(); //

    return IA;
}

/**
* Part2-3 Special Matrices and..
* Thomas algorithms, Jacobi iteration->can use Thread, Multicore programming!. but,
* ���⼭�� single core programming..
* make SquareSecondArray Version!! ( SecondArray�� ��ӹ�����. )
* and make Thread Version!! - Jacobi..
*/
class Thomas{
public:
    // use ���? SecondArray -> n*n Array?
    void get( SquareSecondArray<NUMBER>& A, Array<NUMBER>& b, Array<NUMBER>& x ){
        get( (SecondArray<NUMBER>&)A, b, x );
    }
    void get( SecondArray<NUMBER> A, Array<NUMBER> b, Array<NUMBER>& x ){
        decomposition( A );
        //
        substitution( A, b, x );
    }
private:
    void decomposition( SecondArray<NUMBER>& A ){
        const int n = A.getRowN();
        for( INTEGER k=1; k< n; k++ ){
            e(A,k) = e(A,k) / f(A,k-1); //
            f(A,k) = f(A,k) - e(A,k) * g(A,k-1);
        }
    }
    void substitution( SecondArray<NUMBER>& A, Array<NUMBER>& r, Array<NUMBER>& x ){
        const int n = A.getRowN();
        // Forward substitution
        for( INTEGER k=1; k < n; k++ ){
            r[k] = r[k] - e(A,k) * r[k-1];
        }
        // Back substitution
        x[n-1] = r[n-1] / f(A,n-1);
        for( INTEGER k=n-2;k >=0;k-- ){
            x[k] = ( r[k] - g(A,k)*x[k+1] ) / f(A,k);
        }
    }
private:
    NUMBER& f( SecondArray<NUMBER>& A,INTEGER k ){
        return A[k][k]; //
    }
    NUMBER& e( SecondArray<NUMBER>& A, INTEGER k ){
        return A[k][k-1];
    }
    NUMBER& g( SecondArray<NUMBER>& A, INTEGER k ){
        return A[k][k+1];
    }
};
//
class Jacobi{
public:
    // ��� ���? [][]���� �ܼ��� ���⸸ �Ѱ���, �ƴϸ� read�� ����Ұ�����??
    // X�� initial ���� �ִٰ� �����Ѵ�.
    void get( SquareSecondArray<NUMBER> A, Array<NUMBER> B, Array<NUMBER>& X, const INTEGER imax,
             const NUMBER es ){ // er
        Array<NUMBER> temp( X.size(), 0 );
        INTEGER iter=0;
        Array<NUMBER> eas( X.size(), 100 );
        while( true ){
            //cf) 0���� ������ ����, ���� ���� ���� ������ ó���� ���� ���� �ʾ����Ƿ�, �����ؾ���.
            for( int i=0;i< A.getN();i++ ){
                 NUMBER sum = 0;
                for( int count=0;count< A.getN()-1;count++ ){
                    sum = sum + X[(i+count+1)%A.getN()] * A[i][(i+count+1)%A.getN()]; //
                }
                sum = B[i] - sum;
                temp[i] = sum / A[i][i];
            }
            // ea ���
            bool allEa=true;

            for( int i=0;i< eas.size();i++ ){
                eas[i] = abs( ( temp[i] - X[i] ) ) / temp[i] * 100;
                //cout << "x" << i+1 << " is " << temp[i] << ", ea is " << eas[i] << "%" << endl;
                if( eas[i] > es ){ allEa=false; }
            }

            // X Update
            for( int i=0;i< temp.size();i++ ){
                X[i] = temp[i];
            }
            if( allEa ){ break; }
            // iter�� imax
            iter++;
            if( iter >= imax ){
                break;
            }
        }
    }
private:

};
}
}
#endif // ROOT_H_INCLUDED
