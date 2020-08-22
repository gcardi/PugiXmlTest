//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EventLogUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

EventLog::EventLog( String Name )
  : name_( Name )
  , messagesModule_( Name )
  , eventLog_( ::RegisterEventSource( 0, Name.c_str() ) )
{
    if ( eventLog_ == NULL ) {
        RaiseLastOSError();
    }
}
//---------------------------------------------------------------------------

EventLog::EventLog( String Name, HINSTANCE HInst )
  : name_( Name )
  , messagesModule_( HInst )
  , eventLog_( ::RegisterEventSource( 0, Name.c_str() ) )
{
    if ( eventLog_ == NULL ) {
        RaiseLastOSError();
    }
}
//---------------------------------------------------------------------------

EventLog::~EventLog()
{
    if ( eventLog_ ) {
        try {
            ::DeregisterEventSource( eventLog_ );
        }
        catch ( ... ) {
        }
    }
}
//---------------------------------------------------------------------------

void EventLog::AppendParams( LPCTSTR* Params, LPCTSTR Par )
{
    *Params++ = Par;
}
//---------------------------------------------------------------------------

String EventLog::LogMessage( unsigned EventType, int Category, int ID )
{
    if ( !::ReportEvent( eventLog_, EventType, Category, ID, 0, 0, 0, 0, 0 ) ) {
        RaiseLastOSError();
    }
    return messagesModule_.GetMessage( ID, 0 );
}
//---------------------------------------------------------------------------


