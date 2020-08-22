//---------------------------------------------------------------------------

#ifndef LogToSyslogH
#define LogToSyslogH

#include <Classes.hpp>
#include <IdSysLog.hpp>

#include <memory>

#include "LogBase.h"
#include "LogCodes.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToSyslog : public FilteredLog<T> {
public:
	LogToSyslog( String Tag, String Host, int Port = 514 );
    template<typename F>
	LogToSyslog( F Filter, String Tag, String Host, int Port = 514 );
	String GetHost() const;
    int GetPort() const;
protected:
	virtual void DoAppend( const T& Item ) override;
private:
	using Facility = typename T::FacilityType;
	using Severity = typename T::SeverityType;

    String tag_;
	std::unique_ptr<TIdSysLog> syslog_;

	static TIdSyslogFacility FacilityToSyslogFacility( const typename T::FacilityType& Value );
	static TIdSyslogSeverity SeverityLevelToSyslogSeverity( const typename T::SeverityType& Value );
};
//---------------------------------------------------------------------------

template<typename T>
LogToSyslog<T>::LogToSyslog( String Tag, String Host, int Port )
  : tag_( Tag )
  , syslog_( new TIdSysLog( 0 ) )
{
	syslog_->Host = Host;
	syslog_->Port = Port;
	syslog_->Active = true;
}
//---------------------------------------------------------------------------

template<typename T>
template<typename F>
LogToSyslog<T>::LogToSyslog( F Filter, String Tag, String Host, int Port )
  : FilteredLog<T>( Filter )
  , tag_( Tag )
  , syslog_( new TIdSysLog( 0 ) )
{
	syslog_->Host = Host;
	syslog_->Port = Port;
	syslog_->Active = true;
}
//---------------------------------------------------------------------------

template<typename T>
String LogToSyslog<T>::GetHost() const
{
	return syslog_->Host;
}
//---------------------------------------------------------------------------

template<typename T>
int LogToSyslog<T>::GetPort() const
{
    return syslog_->Port;
}
//---------------------------------------------------------------------------

template<typename T>
TIdSyslogFacility LogToSyslog<T>::FacilityToSyslogFacility( const typename T::FacilityType& Value )
{
	switch ( Value ) {
        case Facility::Kernel:  return sfKernel;
        case Facility::UserLevel:         return sfUserLevel;
        case Facility::MailSystem:        return sfMailSystem;
        case Facility::SystemDaemon:      return sfSystemDaemon;
        case Facility::SecurityOne:       return sfSecurityOne;
        case Facility::SysLogInternal:    return sfSysLogInternal;
        case Facility::LPR:               return sfLPR;
        case Facility::NNTP:              return sfNNTP;
        case Facility::UUCP:              return sfUUCP;
        case Facility::ClockDaemonOne:    return sfClockDaemonOne;
        case Facility::SecurityTwo:       return sfSecurityTwo;
        case Facility::FTPDaemon:         return sfFTPDaemon;
        case Facility::NTP:               return sfNTP;
        case Facility::LogAudit:          return sfLogAudit;
        case Facility::LogAlert:          return sfLogAlert;
        case Facility::ClockDaemonTwo:    return sfClockDaemonTwo;
        case Facility::LocalUse0:         return sfLocalUseZero;
        case Facility::LocalUse1:         return sfLocalUseOne;
        case Facility::LocalUse2:         return sfLocalUseTwo;
        case Facility::LocalUse3:         return sfLocalUseThree;
        case Facility::LocalUse4:         return sfLocalUseFour;
        case Facility::LocalUse5:         return sfLocalUseFive;
        case Facility::LocalUse6:         return sfLocalUseSix;
        case Facility::LocalUse7:         return sfLocalUseSeven;
        default:                return sfUserLevel;

	}
}
//---------------------------------------------------------------------------

template<typename T>
TIdSyslogSeverity LogToSyslog<T>::SeverityLevelToSyslogSeverity( const typename T::SeverityType& Value )
{
    switch ( Value ) {
        case Severity::Emergency:     return slEmergency;
        case Severity::Alert:         return slAlert;
        case Severity::Critical:      return slCritical;
        case Severity::Error:         return slError;
        case Severity::Warning:       return slWarning;
        case Severity::Notice:        return slNotice;
        case Severity::Informational: return slInformational;
        case Severity::Debug:         return slDebug;
        default:                      return slNotice;
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogToSyslog<T>::DoAppend( const T& Item )
{
	if ( syslog_->Active ) {
		syslog_->SendLogMessage(
			tag_,
            FmtLoadStr(
                LOG_ITEM_SYSLOG_FMT_TXT,
                ARRAYOFCONST( (
                    LoadStr(
                        LOG_SEVERITY_BASE_SHORT +
                        static_cast<int>( Item.GetSeverity() )
                    ),
                    LoadStr(
                        LOG_FACILITY_BASE_SHORT +
                        static_cast<int>( Item.GetFacility() )
                    ),
                    LoadStr( Item.GetEntity() ),
                    Item.GetMessage(),
                    /*
                    Item.GetParams().empty() ?
                      LoadStr( Item.GetMessage() )
                    :
                      FmtLoadStr(
                          Item.GetMessage(),
                          &(Item.GetParams())[0], Item.GetParams().size()
                      )
                    */
                    GetMessageText( Item )
                ) )
            ),
			FacilityToSyslogFacility( Item.GetFacility() ),
			SeverityLevelToSyslogSeverity( Item.GetSeverity() ),
			true,
			Item.GetProcessId()
        );
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
