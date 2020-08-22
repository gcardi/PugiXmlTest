//---------------------------------------------------------------------------

#ifndef WaitableTimerH
#define WaitableTimerH

#include <windows.h>

#include <System.SyncObjs.hpp>

class TWaitableTimer {
public:
    explicit TWaitableTimer( bool ManualReset = false );
    ~TWaitableTimer() /* throw() */;
    TWaitableTimer( TWaitableTimer const & ) = delete;
    TWaitableTimer& operator=( TWaitableTimer const & ) = delete;
    void SetPeriod( __int64 Time100NSTicks, LONG Period = 0 );
    void Stop();
    HANDLE GetHandle() const { return handle_; }
    TWaitResult WaitFor( DWORD Timeout ) const;
private:
    HANDLE handle_;
};

//---------------------------------------------------------------------------
#endif
