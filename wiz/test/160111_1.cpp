

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;
#include <conio.h>
#include <wiz/cpp_string.h>
#include <wiz/wizVar.h>
//#include <conio.h>

const string DATE = "1800.1.1";

inline bool isDigit( char ch )
{
    return ch >= '0' && ch <= '9';
}
bool isExistDot( string str )
{
    bool exist=false;

    for( int i=0; i < str.size(); i++ )
    {
        if( str[i] == '.' ) { exist = true; break; }
    }

    return exist;
}
bool isExistEqual( string str )
{
    bool exist=false;

    for( int i=0; i < str.size(); i++ )
    {
        if( str[i] == '=' ) { exist = true; break; }
    }

    return exist;
}
bool isDate( const string& str ) /// yyyy.mm.dd
{
    int state = 0; bool success = false;
    for( int i=0; i < str.size(); i++ ) {
        switch( state ) {
        case 0:
            if( isDigit( str[i] ) ) { state = 1; }
            else{ return false; }
            break;
        case 1:
            if( isDigit( str[i] ) ) { state = 1; }
            else if( '.' == str[i] ){ state = 2; }
            else { return false; }
            break;
        case 2:
            if( isDigit( str[i] ) ) { state = 3; }
            else{ return false; }
            break;
        case 3:
            if( isDigit( str[i] ) ) { state = 3; }
            else if( '.' == str[i] ) { state = 4; }
            else{ return false; }
            break;
        case 4://
            if( isDigit( str[i] ) ) { state = 5; success = true; }
            else { return false; }
            break;
        case 5:
            if( isDigit( str[i] ) ) { state = 5; }
            if( i == str.size()-1 ) { success = true; }
            else { return false; }
            break;
        }
    }
    return success;
}
inline bool isDate2( const string& str )
{
    return str[0] == '\"' && str[str.size()-1] == '\"' && isDate( str.substr( 1, str.size() - 2 ) );
}
string removeSpace(const string& str)
{
	int idx = 0;

	for (; idx < str.size(); ++idx)
	{
		if (!wiz::isWhitespace(str[idx]))
			break;
	}

	return string(str.c_str() + idx);
}
int main( void )
{
    wiz::StringTokenizer tokenizer;
    ifstream inFile;
    ofstream outFile;
    inFile.open( "input.eu4", ios::binary );
    outFile.open( "output.eu4", ios::binary );

    if( inFile.fail() ){ return -1; }

    string temp;
    while( getline( inFile, temp ) )
    {
		temp = removeSpace(temp);
        string temp2;
        string temp3;
        tokenizer = wiz::StringTokenizer( temp, vector<string>{ "=" } );

        temp2 = tokenizer.nextToken();
        temp3 = tokenizer.nextToken();

        if( wiz::String::endsWith( temp2, "date" ) || wiz::String::endsWith( temp2, "dates" ) )
        {
            if( !temp3.empty() ) {
                if( temp3 == "{" )
                {
                    outFile << temp2 << "=" << temp3;
                }
                else {
                    outFile << temp2 << "=" << DATE;
                }
               // cout << temp2 << "=" << DATE << "\n";
            }
            else {
                outFile << temp2 << "=" << " ";
              //  cout << temp2 << "=" << "\n";
            }
        }
        else if( isDate( temp2 ) || isDate2( temp2 ) ) /// dip..
        {
            if( !temp3.empty() ) {
                outFile << DATE << "=" << temp3;
              //  cout << DATE << "=" << temp3 << "\n";
            }
            else {
                if( isExistEqual( temp ) ) {
                    outFile << DATE << "=";
               //     cout << DATE <<"=" << "\n";
                }
                else
                {
                    outFile << DATE;
                //    cout << DATE <<"\n";
                }
            }
        }
        else if( isDate( temp3 ) || isDate2( temp3 ) )
        {
            outFile << temp2 << "=" << DATE;
         //   cout << temp2 << "=" << DATE << "\n";
        }
        else
        {
            outFile << temp;
           // cout << " " << temp << "\n";
        }
        //_getch();
        if( !inFile.eof() ) outFile << "\n";
    }
    inFile.close();
    outFile.close();
    return 0;
}
