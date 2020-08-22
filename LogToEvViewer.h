//---------------------------------------------------------------------------

#ifndef LogToEvViewerH
#define LogToEvViewerH

#include <Classes.hpp>
#include <System.SysUtils.hpp>

#include "Logbase.h"
#include "EventLogUtils.h"
#include "EvLogMessages.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToEvViewer : public FilteredLog<T> {
public:
    explicit LogToEvViewer( String ServiceName );

    template<typename F>
    LogToEvViewer( F Filter, String ServiceName );

    template<typename F>
    LogToEvViewer( F Filter, String ServiceName, HINSTANCE ModuleInst );
protected:
    virtual void DoAppend( const T& Item ) override;
private:
	using Severity = typename T::SeverityType;

	EventLog eventLog_;

	static DWORD GetEvLogType( const T& Item );
	static DWORD GetEvLogMessageId( const T& Item );
};
//---------------------------------------------------------------------------

template<typename T>
LogToEvViewer<T>::LogToEvViewer( String ServiceName )
  : eventLog_( ServiceName )
{
}
//---------------------------------------------------------------------------

template<typename T>
template<typename F>
LogToEvViewer<T>::LogToEvViewer( F Filter, String ServiceName )
  : FilteredLog<T>( Filter )
  , eventLog_( ServiceName )
{
}
//---------------------------------------------------------------------------

template<typename T>
template<typename F>
LogToEvViewer<T>::LogToEvViewer( F Filter, String ServiceName, HINSTANCE ModuleInst )
  : FilteredLog<T>( Filter )
  , eventLog_( ServiceName, ModuleInst )
{
}
//---------------------------------------------------------------------------

template<typename T>
DWORD LogToEvViewer<T>::GetEvLogType( const T& Item )
{
	switch( Item.GetSeverity() ) {
		case Severity::Emergency:      // system is unusable
			return EVENTLOG_ERROR_TYPE;
		case Severity::Alert:          // action must be taken immediately
			return EVENTLOG_WARNING_TYPE;
		case Severity::Critical:       // critical conditions
			return EVENTLOG_ERROR_TYPE;
		case Severity::Error:          // error conditions
			return EVENTLOG_ERROR_TYPE;
		case Severity::Warning:        // warning conditions
			return EVENTLOG_WARNING_TYPE;
		case Severity::Notice:         // normal but significant condition
			return EVENTLOG_SUCCESS;
		case Severity::Informational:  // informational messages
			return EVENTLOG_INFORMATION_TYPE;
		case Severity::Debug:          // debug-level messages
			return EVENTLOG_INFORMATION_TYPE;
		default:
			return EVENTLOG_SUCCESS;
	}
}
//---------------------------------------------------------------------------

template<typename T>
DWORD LogToEvViewer<T>::GetEvLogMessageId( const T& Item )
{

	switch( Item.GetSeverity() ) {
		case Severity::Emergency:      // system is unusable
		case Severity::Alert:          // action must be taken immediately
		case Severity::Critical:       // critical conditions
		case Severity::Error:          // error conditions
			return MSG_ERROR;
		case Severity::Warning:        // warning conditions
			return MSG_WARNING;
		case Severity::Notice:         // normal but significant condition
			return MSG_SUCCESS;
		case Severity::Informational:  // informational messages
		case Severity::Debug:          // debug-level messages
			return MSG_INFORMATIONAL;
		default:
			return MSG_SUCCESS;
	}
}
//---------------------------------------------------------------------------

template<typename T>
void LogToEvViewer<T>::DoAppend( const T& Item )
{
// Logga solo il testo del messaggio...
// magari vedere se è il caso di aggiungere il resto delle
// informazioni presenti in T
/*
    eventLog_.LogMessage(
        GetEvLogType( Item ),
        CAT_APP,
        GetEvLogMessageId( Item ),
        GetMessageText( Item ).c_str()
    );
*/
    String const Text = GetMessageText( Item );
    String Ret = eventLog_.LogMessage(
        GetEvLogType( Item ),
        CAT_APP,
        GetEvLogMessageId( Item ),
        Text.c_str()
    );

//#if defined( _DEBUG )
//    ::OutputDebugString( Ret.c_str() );
//#endif
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
