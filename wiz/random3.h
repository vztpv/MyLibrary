#ifndef RANDOM3_H_INCLUDED
#define RANDOM3_H_INCLUDED

#include <vector>
#include <wiz/random.h>
#include <wiz/wizardError.h>

namespace wiz {

class RandomWithWeight
{
public:
    class Data
    {
    public:
        int val;
        int weight;
    public:
        explicit Data( const int val, const int weight ) :val( val ), weight( weight ) { }
        explicit Data() : val( -1 ), weight( 0 ) { }
    };
private:
    std::vector<Data> vec;
public:
    void add( const Data& data ) { vec.push_back( data ); }
public:
    bool empty() { return vec.empty(); }
    int size() { return vec.size(); }
    int rand() { /// chk??
        if( empty() ) { throw wiz::Error( "rand, empty state.." ); }
        std::vector<int> temp( vec.size() );
        int ret = -1;
        temp[0] = vec[0].weight;
        for( int i=1; i < temp.size(); i++ )
        {
            temp[i] = vec[i].weight + temp[i-1];
        }
        const int x = wiz::Rand2<int>() % temp[temp.size()-1];
        for( int i=0; i < temp.size(); i++ ) {
            if( temp[i] > x ) {
                ret = vec[i].val;
                vec[i] = vec[vec.size()-1];
                vec.resize( vec.size()-1 );
                break;
            }
        }
        return ret;
    }
};


}

#endif // RANDOM3_H_INCLUDED
