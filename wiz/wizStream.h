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
    // ����, ��������� ����., �� ���� ����.
    CInFile( const CInFile& p ); // ���� ���� ����.
    CInFile& operator=( const CInFile& p );
    bool operator==( const CInFile& p );
    // new, delete ����..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    CInFile(){
        isOpen = false;
        inFile = new ifstream();
    }
    ~CInFile(){ // ����ؼ��� �� �ȴ�.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
        delete inFile;
    }
    bool open( string fileName ){ // ���� �̸�.
        if( isOpen ){ // ���� �ʾҴٸ�..
            close();
        }
        inFile->open( fileName.data() );
        isOpen = !inFile->fail(); // �ѹ� �����ϸ� ��� ���л����̴�.? //������� �������� ?
        if( !isOpen ){
            delete inFile;
            inFile = new ifstream();
        }
        return isOpen; //�����ϸ� false.
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
    // ����, ��������� ����., �� ���� ����. //10.3
    COutFile( const COutFile& p ); // ���� ���� ����.
    COutFile& operator=( const COutFile& p );
    bool operator==( const COutFile& p );
    // new, delete ����..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    COutFile(){
        isOpen = false;
        outFile = new ofstream();
    }
    ~COutFile(){ // ����ؼ��� �� �ȴ�.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
       delete outFile;
    }
    bool open( string fileName, bool append=false ){ // ���� �̸�.
        if( isOpen ){ // ���� �ʾҴٸ�..
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
        isOpen = !outFile->fail(); // �ѹ� �����ϸ� ��� ���л����̴�.? //������� �������� ?

        // err process
        if( !isOpen ){
            delete outFile;
            outFile = new ofstream();
        }

        return isOpen; //�����ϸ� false.
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
