#ifndef WIZSTREAM_H_INCLUDED
#define WIZSTREAM_H_INCLUDED

#ifndef WIZFILESTREAM_H_INCLUDED
#define WIZFILESTREAM_H_INCLUDED
//
#include <fstream>
#include <string>
#include <windows.h> // for CreateDirectory
using namespace std;

void MakeFolder( const string folderName )
{
    for( int i=1; i < folderName.length(); i++ )
    {
        if( folderName[i] == '\\' || folderName[i] == '/' ) //
        {
            string subStr = folderName.substr( 0, i );
            const char* ccSubStr = subStr.c_str();

            CreateDirectory( ccSubStr, NULL ); // for gnu, minGW compiler!, not Visual Compiler!
        }
    }
}

class CInFile{
private:
    bool isOpen;
    ifstream* inFile;
    // 대입, 복사생성자 방지., 비교 연산 방지.
    CInFile( const CInFile& p ); // 별도 구현 없음.
    CInFile& operator=( const CInFile& p );
    bool operator==( const CInFile& p );
    // new, delete 금지..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    CInFile(){
        isOpen = false;
        inFile = new ifstream();
    }
    ~CInFile(){ // 상속해서는 안 된다.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
        delete inFile;
    }
    bool open( string fileName ){ // 파일 이름.
        if( isOpen ){ // 닫지 않았다면..
            close();
        }
        inFile->open( fileName.data() );
        isOpen = !inFile->fail(); // 한번 실패하면 계속 실패상태이다.? //원래대로 못돌리나 ?
        if( !isOpen ){
            delete inFile;
            inFile = new ifstream();
        }
        return isOpen; //실패하면 false.
    }
    void close() {
        inFile->close();
        isOpen = false;
    }
    bool eof(){ return inFile->eof(); }
    void useSKIPWS(){ // skip white space.
        inFile->unsetf( ios::skipws );
    }
    string getLine() {
        string temp;
        getline( *inFile, temp );
        return temp;
    }
    bool isOpened()const{
        return isOpen;
    }
public:
    template <class T>
    friend
    CInFile& operator>>( CInFile& stream, T& x );
};

class COutFile{
private:
    bool isOpen;
    ofstream* outFile;
    // 대입, 복사생성자 방지., 비교 연산 방지. //10.3
    COutFile( const COutFile& p ); // 별도 구현 없음.
    COutFile& operator=( const COutFile& p );
    bool operator==( const COutFile& p );
    // new, delete 금지..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    COutFile(){
        isOpen = false;
        outFile = new ofstream();
    }
    ~COutFile(){ // 상속해서는 안 된다.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
       delete outFile;
    }
    bool open( string fileName, bool append=false ){ // 파일 이름.
        if( isOpen ){ // 닫지 않았다면..
            outFile->close();
        }

        // create folder..
        MakeFolder( fileName );

        // append chk
        if( append ){
            outFile->open( fileName.data(), ios::app );
        }
        else{
            outFile->open( fileName.data() );
        }

        // open chk
        isOpen = !outFile->fail(); // 한번 실패하면 계속 실패상태이다.? //원래대로 못돌리나 ?

        // err process
        if( !isOpen ){
            delete outFile;
            outFile = new ofstream();
        }

        return isOpen; //실패하면 false.
    }
    void close(){
        outFile->close();
        isOpen = false;
    }
    bool eof()const{ return outFile->eof(); }
    bool isOpened()const
    {
        return isOpen;
    }

    template <class T>
    friend
    COutFile& operator<<( COutFile& stream, T x );
};


template <class T>
CInFile& operator>>( CInFile& stream, T& x )
{
    (*stream.inFile) >> x ;
    return stream;
}

template <class T>
COutFile& operator<<( COutFile& stream, const T& x )
{
    (*stream.outFile) << x;
    return stream;
}


#endif // WIZFILESTREAM_H_INCLUDED

#endif // WIZSTREAM_H_INCLUDED
