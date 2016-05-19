#ifndef LOG2_X_H_INCLUDED
#define LOG2_X_H_INCLUDED

#include <wiz/wizarderror.h>

namespace wiz {
// check 1 -> 0, 2->1, 3->1, 4->2, 5->2,6->2, 7->2, 8->3, ..., 15->3, 16->4, 17->4, ...
template <typename UNSIGNED_INTEGER>
class Log2{
private:
	Log2(const Log2&);
	Log2& operator = (const Log2&);
protected:
    void initial(){
        UNSIGNED_INTEGER temp=2;
        UNSIGNED_INTEGER num = sizeof(UNSIGNED_INTEGER)*8;
        log2 = new UNSIGNED_INTEGER[num]; // 0 Æ÷ÇÔ 1
        log2[0] = 1; //
        for( UNSIGNED_INTEGER i=1;i< num;i++ ){
            log2[i] = temp;//
            temp = temp << 1; // temp = temp * 2;
        }
    }
public:
    UNSIGNED_INTEGER getLog2( UNSIGNED_INTEGER val )const{
        if( val <= 0 ){ throw wiz::Error( "err:Log2-getLog2" ); }//

        for( unsigned int i=sizeof(UNSIGNED_INTEGER)*8-1; i >= 0; i-- ) {
            if( val >= log2[i] ) { return i; }
        }
        //throw wiz::Error( "err2:Log2-getLog2" );
    }
public:
    Log2(){
        initial();
    }
    ~Log2(){
        if( log2 != NULL ){
            delete[] log2;
        }
    }
    UNSIGNED_INTEGER* log2;
};

}
#endif // LOG2_X_H_INCLUDED
