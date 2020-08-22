//---------------------------------------------------------------------------

#ifndef LogBaseH
#define LogBaseH

#include <tchar.h>

#include <System.hpp>
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <System.SyncObjs.hpp>

#include <stdint.h>

#include <vector>
#include <iterator>
#include <functional>
#include <memory>

#include "LogCodes.h"
//#include "ToRawFn.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

class LogParams {
private:
    using ParamsCont = std::vector<Variant>;

    ParamsCont params_;

    void AppendLogParams( LPCTSTR Par ) {
        params_.push_back( String( Par ) );
    }

    template<typename T>
    void AppendLogParams( T const & Par ) {
        params_.push_back( Par );
    }

    template<typename T, typename... A>
    void AppendLogParams( T const & Par, A... Args ) {
        params_.push_back( Par );
        AppendLogParams( std::forward<A...>( Args )... );
    }
public:
    LogParams() {}

    template<typename T, typename... A>
    LogParams( T const & Par, A... Args ) {
        params_.reserve( 1 + sizeof...( Args ) );
        AppendLogParams( Par, std::forward<A...>( Args )... );
    }

    bool IsEmpty() const { return params_.empty(); }
    ParamsCont::size_type Count() const { return params_.size(); }

    template<typename OutputIt>
    void CopyTo( OutputIt Out ) const {
        std::copy( params_.begin(), params_.end(), Out );
    }
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template<
    typename E=int,
    typename M=int,
    typename D=TDateTime
>
class LogItemBase {
public:
    using EntityType = E;
    using MessageType = M;
    using DateTimeType = D;
    using LogParamsType = LogParams;

    LogItemBase() {}
    virtual ~LogItemBase() {}
    explicit LogItemBase( EntityType const & Entity, MessageType const & Message,
                          DateTimeType const & DateTime )
      : entity_( Entity ) , message_( Message ), dateTime_( DateTime ) {}

    explicit LogItemBase( EntityType const & Entity, MessageType const & Message,
                          LogParamsType const & Params, DateTimeType const & DateTime )
      : entity_( Entity ), message_( Message ), params_( Params )
      , dateTime_( DateTime ) {}

	EntityType GetEntity() const { return entity_; }
    MessageType GetMessage() const { return message_; }
    DateTimeType GetDateTime() const { return dateTime_; }
    LogParams const & GetParams() const { return params_; }
protected:
private:
    EntityType entity_;           // aka Category in Event Log
    MessageType message_;         // aka Facility Status Code in Event Log
    DateTimeType dateTime_;       // as generated
    LogParams params_;
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template<
    typename E=int,
    typename M=int,
    typename P=DWORD,
    typename TH=DWORD,
    typename D=TDateTime
>
class LogItemExecData : public LogItemBase<E,M,D> {
public:
    using EntityType = typename LogItemBase<E,M,D>::EntityType;
    using MessageType = typename LogItemBase<E,M,D>::MessageType;
    using DateTimeType = typename LogItemBase<E,M,D>::DateTimeType;
    using LogParamsType = LogParams;
	using ProcessIdType = P;
	using ThreadIdType = TH;

    LogItemExecData() {}
    explicit LogItemExecData( EntityType Entity,
                              MessageType Message,
                              DateTimeType DateTime,
                              ProcessIdType ProcessId = ::GetCurrentProcessId(),
                              ThreadIdType ThreadId = ::GetCurrentThreadId() )
      : LogItemBase<E,M,D>( Entity, Message, DateTime )
      , processId_( ProcessId )
      , threadId_( ThreadId ) {}

    explicit LogItemExecData( EntityType Entity,
                              MessageType Message,
                              LogParamsType const & Params,
                              DateTimeType DateTime,
                              ProcessIdType ProcessId = ::GetCurrentProcessId(),
                              ThreadIdType ThreadId = ::GetCurrentThreadId() )
      : LogItemBase<E,M,D>( Entity, Message, Params, DateTime )
      , processId_( ProcessId )
      , threadId_( ThreadId ) {}

    ProcessIdType GetProcessId() const { return processId_; }
    ThreadIdType GetThreadId() const { return threadId_; }
private:
    ProcessIdType processId_;
    ThreadIdType threadId_;
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

enum class DefSeverityType {
    Emergency,      // system is unusable                 //  EVENTLOG_ERROR_TYPE
    Alert,          // action must be taken immediately   //  EVENTLOG_ERROR_TYPE
    Critical,       // critical conditions                //  EVENTLOG_ERROR_TYPE
    Error,          // error conditions                   //  EVENTLOG_ERROR_TYPE
    Warning,        // warning conditions                 //  EVENTLOG_WARNING_TYPE
    Notice,         // normal but significant condition   //  EVENTLOG_SUCCESS
    Informational,  // informational messages             //  EVENTLOG_INFORMATION_TYPE
    Debug,          // debug-level messages               //  EVENTLOG_INFORMATION_TYPE
};
//---------------------------------------------------------------------------

enum class DefFacilityType {
    Kernel,             //   0       Kernel
    UserLevel,          //   1       UserLevel
    MailSystem,         //   2       MailSystem
    SystemDaemon,       //   3       SystemDaemon
    SecurityOne,        //   4       SecurityOne
    SysLogInternal,     //   5       SysLogInternal
    LPR,                //   6       LPR
    NNTP,               //   7       NNTP
    UUCP,               //   8       UUCP
    ClockDaemonOne,     //   9       ClockDaemonOne
    SecurityTwo,        //  10       SecurityTwo
    FTPDaemon,          //  11       FTPDaemon
    NTP,                //  12       NTP
    LogAudit,           //  13       LogAudit
    LogAlert,           //  14       LogAlert
    ClockDaemonTwo,     //  15       ClockDaemonTwo
    LocalUse0,          //  16       LocalUse0
    LocalUse1,          //  17       LocalUse1
    LocalUse2,          //  18       LocalUse2
    LocalUse3,          //  19       LocalUse3
    LocalUse4,          //  20       LocalUse4
    LocalUse5,          //  21       LocalUse5
    LocalUse6,          //  22       LocalUse6
    LocalUse7           //  23       LocalUse7
};
//---------------------------------------------------------------------------

template<
    typename E = int
  , typename M = int
  , typename P = DWORD
  , typename TH = DWORD
  , typename D = TDateTime
  , typename S = DefSeverityType
  , typename F = DefFacilityType
>
class ClassifiedLogItem : public LogItemExecData<E,M,P,TH,D> {
public:
    using EntityType = typename LogItemExecData<E,M,P,TH,D>::EntityType;
    using MessageType = typename LogItemExecData<E,M,P,TH,D>::MessageType;
    using DateTimeType = typename LogItemExecData<E,M,P,TH,D>::DateTimeType;
    using LogParamsType = typename LogItemExecData<E,M,P,TH,D>::LogParamsType;
	using ProcessIdType = typename LogItemExecData<E,M,P,TH,D>::ProcessIdType;
	using ThreadIdType = typename LogItemExecData<E,M,P,TH,D>::ThreadIdType;
	using SeverityType = S;
	using FacilityType = F;

    ClassifiedLogItem() {}
    ClassifiedLogItem( EntityType Entity,
                       MessageType Message,
                       DateTimeType DateTime,
                       SeverityType Severity = S::Informational,
                       FacilityType Facility = F::UserLevel,
                       ProcessIdType const & ProcessId = ::GetCurrentProcessId(),
                       ThreadIdType const & ThreadId = ::GetCurrentThreadId() )
      : LogItemExecData<E,M,P,TH,D>( Entity, Message, DateTime, ProcessId,
                                     ThreadId )
      , severity_( Severity )
      , facility_( Facility ) {}

    ClassifiedLogItem( EntityType Entity,
                       MessageType Message,
                       LogParamsType&& Params,
                       DateTimeType  DateTime,
                       SeverityType  Severity = S::Informational,
                       FacilityType  Facility = F::UserLevel,
                       ProcessIdType ProcessId = ::GetCurrentProcessId(),
                       ThreadIdType const & ThreadId = ::GetCurrentThreadId() )
      : LogItemExecData<E,M,P,TH,D>( Entity, Message, Params, DateTime,
                                     ProcessId, ThreadId )
      , severity_( Severity )
      , facility_( Facility ) {}

    SeverityType GetSeverity() const { return severity_; }
    FacilityType GetFacility() const { return facility_; }
private:
    SeverityType severity_;
    FacilityType facility_;
};
//---------------------------------------------------------------------------

template<typename T>
String GetMessageText( T const & Item )
{
    if ( Item.GetParams().IsEmpty() ) {
        return LoadStr( Item.GetMessage() );
    }
    else {
        LogParams const & Params = Item.GetParams();
        std::vector<TVarRec> Array;
        Array.reserve( Params.Count() );
        Params.CopyTo( std::back_inserter( Array ) );
        return FmtLoadStr( Item.GetMessage(), &Array[0], Array.size() - 1 );
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

using LogItem = ClassifiedLogItem<>;

template<typename T>
class LogBase {
public:
    using ItemType = T;

    void Append( T const & Item, bool HaveToFlush = true );
    virtual ~LogBase() { Flush(); }
    void Clear() { DoClear(); }
    void Flush() { DoFlush(); }

private:
/*
    using AppendCallbackRawType =
        typename ToRawFnType<decltype(&LogBase<T>::Append)>::RawFnType;
*/
public:
//    using AppendCallbackType = std::function<AppendCallbackRawType>;
    using AppendCallbackType = std::function<void( T const &, bool )>;

    void SetAppendCallback( AppendCallbackType Callback ) {
        appendCallback_ = Callback;
    }
protected:
    virtual void DoAppend( T const & Item ) = 0;
    virtual void DoClear() {}
    virtual void DoFlush() {}
    virtual bool DoHaveToLog( T const & Item ) const = 0;
private:
    AppendCallbackType appendCallback_;

    // Privato, perché l'abilitazione dei log è differenziata per destinazioni
    bool HaveToLog( T const & Item ) const { return DoHaveToLog( Item ); }
};
//---------------------------------------------------------------------------

template<typename T>
void LogBase<T>::Append( T const & Item, bool HaveToFlush )
{
    if ( HaveToLog( Item ) ) {
        if ( appendCallback_ ) {
            appendCallback_( Item, HaveToFlush );
        }
        DoAppend( Item );
        if ( HaveToFlush ) {
            DoFlush();
        }
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template<typename T>
class FilteredLog : public LogBase<T> {
public:
    using FilterType = std::function<bool(T const &)>;

    explicit FilteredLog( FilterType Filter = FilterType{} ) : filter_( Filter ) {}
protected:
    virtual bool DoHaveToLog( T const & Item ) const override;
private:
    FilterType filter_;
};
//---------------------------------------------------------------------------

template<typename T>
bool FilteredLog<T>::DoHaveToLog( T const & Item ) const
{
    return filter_ ? filter_( Item ) : true;
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif

