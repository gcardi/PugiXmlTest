//---------------------------------------------------------------------------

#ifndef GlobalsH
#define GlobalsH

#include <memory>

#include "LogBase.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------

extern std::unique_ptr<Log::LogBase<Log::LogItem>> LogObj;

//---------------------------------------------------------------------------

template<typename M, typename S>
void LogMessageTo( Log::LogBase<Log::LogItem>& ALogObj, M Message,
                   S const & Severity )
{
    ALogObj.Append(
        Log::LogItem(
            LOG_ENT_APPLICATION, Message, Now(), Severity
        )
    );
}
//---------------------------------------------------------------------------

template<typename M, typename S, typename... A>
void LogMessageTo( Log::LogBase<Log::LogItem>& ALogObj, M Message,
                   S const & Severity, A&&... Args )
{
    ALogObj.Append(
        SvcApp::Log::LogItem(
            LOG_ENT_APPLICATION, Message,
            Log::LogParams( std::forward<A...>( Args )... ),
            Now(), Severity
        )
    );
}
//---------------------------------------------------------------------------

template<typename M, typename S>
void LogMessage( M Message, S const & Severity )
{
    if ( LogObj ) {
        LogMessageTo( *LogObj, Message, Severity );
    }
}
//---------------------------------------------------------------------------

template<typename M, typename S, typename... A>
void LogMessage( M Message, S const & Severity, A&&... Args )
{
    if ( LogObj ) {
        LogMessageTo( *LogObj, Message, Severity, std::forward<A...>( Args )... );
    }
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogEmergencyMessage( M Message, A&&... Args )
{
    LogMessage(
        Message,
        SvcApp::Log::DefSeverityType::Emergency,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogAlertMessage( M Message, A&&... Args )
{
    LogMessage(
        Message, SvcApp::Log::DefSeverityType::Alert,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogCriticalMessage( M Message, A&&... Args )
{
    LogMessage(
        Message, SvcApp::Log::DefSeverityType::Critical,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename... A>
void LogErrorMessage( A&&... Args )
{
    LogMessage(
        LOG_MSG_FAILURE, SvcApp::Log::DefSeverityType::Error,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogWarnMessage( M Message, A&&... Args )
{
    LogMessage(
        Message, SvcApp::Log::DefSeverityType::Warning,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogNoticeMessage( M Message, A&&... Args )
{
    LogMessage(
        Message, SvcApp::Log::DefSeverityType::Notice,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename M, typename... A>
void LogInfoMessage( M Message, A&&... Args )
{
    LogMessage(
        Message, SvcApp::Log::DefSeverityType::Informational,
        std::forward<A...>( Args )...
    );
}
//---------------------------------------------------------------------------

template<typename... A>
void LogDbgMessage( A&&... Args )
{
    LogMessage(
        LOG_MSG_DBG, SvcApp::Log::DefSeverityType::Debug,
        std::forward<A...>( Args )...
    );
}

//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
