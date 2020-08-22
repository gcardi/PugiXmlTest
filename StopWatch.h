//---------------------------------------------------------------------------

#ifndef StopWatchH
#define StopWatchH

#include <windows.h>

#include <System.SysUtils.hpp>

class StopWatch {
public:
    StopWatch( bool AutoStart = true ) {
        if ( AutoStart ) {
            Start();
        }
    }

    void Start() {
        if ( !running_ ) {
            running_ = true;
            Win32Check( ::QueryPerformanceFrequency( &frequency_ ) );
            Win32Check( ::QueryPerformanceCounter( &startingTime_ ) );
        }
    }

    void Stop() {
        Win32Check( ::QueryPerformanceCounter( &endingTime_ ) );
        if ( running_ ) {
            elapsedMicroseconds_.QuadPart =
                endingTime_.QuadPart - startingTime_.QuadPart;
            elapsedMicroseconds_.QuadPart *= 1000000;
            elapsedMicroseconds_.QuadPart /= frequency_.QuadPart;
            running_ = false;
        }
    }

    LARGE_INTEGER GetElapsedMicroseconds() {
        Stop();
        return elapsedMicroseconds_;
    }

    /*
    long double GetElapsedTime() {
        return static_cast<long double>( GetElapsedMicroseconds().QuadPart ) / 1E6;
    }
    */
private:
    bool running_ {};
    LARGE_INTEGER startingTime_ {};
    LARGE_INTEGER endingTime_ {};
    LARGE_INTEGER elapsedMicroseconds_ {};
    LARGE_INTEGER frequency_ { 1 };
};

//---------------------------------------------------------------------------
#endif
