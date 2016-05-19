#ifndef THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <windows.h>
#include <process.h> // fpr _begin thread (ex)

// make namespace wiz?
// todo..
    //template + fuction object??
namespace wiz{
    /// Run - ()
template < class RUN >
class Thread{
private:
    // 복사생성자, 대입연산 금지
    Thread( const Thread<RUN>& );
    Thread<RUN> operator=( const Thread<RUN>& );
private:
    static unsigned int WINAPI startRoutine(LPVOID);
    unsigned int _threadId;
    DWORD _exitCode;
    HANDLE _threadHandle;
public:
    RUN _run;

    Thread();
    virtual ~Thread();
    bool isRunning();
    unsigned int getThreadId();
    int getExitCode();
    unsigned int start();
    void wait( int waitTime=INFINITE ); // wait( time )??
    void dispose(); // close Thread
private:
    void run() { _run(); }
};
//
template < class RUN >
inline unsigned int WINAPI Thread<RUN>::startRoutine( void* pv )
{
    static_cast<Thread<RUN>*>(pv)->run();
    return 0;
}
template < class RUN >
Thread<RUN>::Thread()
{
    _exitCode = static_cast<DWORD>( -1 );
}

template < class RUN >
Thread<RUN>::~Thread()
{ }
template < class RUN >
inline bool Thread<RUN>::isRunning() /// chk..
{
    return GetExitCodeThread( _threadHandle, &_exitCode );
}
template < class RUN >
inline unsigned int Thread<RUN>::getThreadId()
{
    return (unsigned int)_threadId;
}
template < class RUN >
inline int Thread<RUN>::getExitCode()
{
    isRunning();
    return _exitCode;
}
template < class RUN >
inline unsigned int Thread<RUN>::start()
{
    _threadHandle = (HANDLE)_beginthreadex( NULL, /// other create func?
        0, startRoutine, this, 0,
        &_threadId );
    return (unsigned int)_threadId;
}
template < class RUN >
inline void Thread<RUN>::wait( int waitTime )
{
    WaitForSingleObject( _threadHandle, waitTime );
}
template < class RUN >
inline void Thread<RUN>::dispose()
{
    CloseHandle( _threadHandle );
}


}
#endif // THREADS_H_INCLUDED
