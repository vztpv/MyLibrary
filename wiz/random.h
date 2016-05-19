#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <wiz/global.h>
#include <cstdlib>
#include <random>

///
/// #define RANDOM_DEBUG
///

namespace wiz{
    template <typename T>
	T Rand() // T : maybe int, long, long long,.., 전체 범위 Rand...
	{
		size_t byteSize = sizeof(T);
		T val = 0;

		// char단위로 rand()호출,
		for (size_t i = 0; i < byteSize; i++)
		{
			val = val << 8; // left shift 1 byte

			val = val | (rand() % 256);
		}
		return val;
	}
    template <typename T>
    inline T Rand2() /// T : must be signed! and no return minus value.
    {
        T val = Rand<T>();
        return val & Signed_Maximum<T>();
    }
    template <typename T>
    inline T Rand( const T low, const T high ) /// changed 2014 11 03
    {
        #ifdef RANDOM_DEBUG
        if( high < low ) { throw wiz::Error( __FILE__, __LINE__, "high < low" ); }
        #endif
        return Rand<T>()%( high - low + 1 ) + low;
    }

    template <typename T>
    inline T Rand2( const T low, const T high ) /// T : must be signed type!
    {
        #ifdef RANDOM_DEBUG
        if( high < low ) { throw wiz::Error( __FILE__, __LINE__, "high < low" ); }
        #endif
        return Rand2<T>()%( high - low + 1 ) + low;
    }

    template <class Engine, class Distribution, typename T>
    class _Random
    {
        Engine engine;
        Distribution distribution;
    public:
        explicit _Random( unsigned int seed, T a, T b )
        {
            engine.seed( seed );
            distribution = Distribution( a, b );
        }
        T rand() { return distribution( engine ); }
    };

    class Random // rename??
    {
        default_random_engine generator;
        normal_distribution<double> distribution;
        uniform_real_distribution<double> distribution2;
        uniform_int_distribution<int> distribution3;
    public:
        explicit Random( unsigned int seed )//double mean, double dev )
        {
            generator.seed( seed );
            distribution = normal_distribution<double>( 0.0, 1.0 );
            distribution2 = uniform_real_distribution<double>( 0.0, 1.0 );
        }
        double nextGaussian()
        {
            return distribution( generator );
        }
        double nextDouble()
        {
            return distribution2( generator );
        }
        int nextInt()
        {
            return distribution3( generator );
        }

    };
}
#endif // RANDOM_H_INCLUDED
