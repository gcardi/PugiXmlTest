//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#include <System.SysUtils.hpp>

#include "WaitableTimer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TWaitableTimer::TWaitableTimer( bool ManualReset )
{
    handle_ = ::CreateWaitableTimer( 0, ManualReset, 0 );
    if ( !handle_ ) {
        RaiseLastOSError();
    }
}
//---------------------------------------------------------------------------

TWaitableTimer::~TWaitableTimer() /* throw() */
{
    try {
        Stop();
    }
    catch ( ... ) {
        ::CloseHandle( handle_ );
    }
}
//---------------------------------------------------------------------------

void TWaitableTimer::SetPeriod( __int64 Time100NSTicks, LONG Period )
{
    LARGE_INTEGER DueTime;

    DueTime.QuadPart = Time100NSTicks;

    if ( !::SetWaitableTimer( handle_, &DueTime, Period, 0, 0, false ) ) {
        RaiseLastOSError();
    }
}
//---------------------------------------------------------------------------

void TWaitableTimer::Stop()
{
    if ( !::CancelWaitableTimer( handle_ ) ) {
        RaiseLastOSError();
    }
}
//---------------------------------------------------------------------------

TWaitResult TWaitableTimer::WaitFor( DWORD Timeout ) const
{
    DWORD Index;

    switch ( ::WaitForMultipleObjectsEx( 1, &handle_, true, Timeout, false ) ) {
        case WAIT_ABANDONED: return wrAbandoned;
        case WAIT_OBJECT_0:  return wrSignaled;
        case WAIT_TIMEOUT:   return wrTimeout;
        case WAIT_FAILED:    return wrError;
        default:             return wrError;
    }
}
//---------------------------------------------------------------------------


