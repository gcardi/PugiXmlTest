//---------------------------------------------------------------------------

#ifndef EventLogUtilsH
#define EventLogUtilsH

#include <windows.h>

#include <System.hpp>
#include <Classes.hpp>

#include <vector>

#include "MsgsMod.h"

class EventLog {
public:
    explicit EventLog( String Name );
    EventLog( String Name, HINSTANCE HInst );
    ~EventLog();

    String LogMessage( unsigned EventType, int Category, int ID );

    template<typename... A>
    String LogMessage( unsigned EventType, int Category, int ID,
                       LPCTSTR Par, A... Args );

    template<typename InputIterator>
    String LogMessage( unsigned EventType, int Category, int ID,
                       InputIterator Begin, InputIterator End );

    // Sarebbe bello mettere una funzione che becca ARRAYOFCONST,
    // insomma, TVarRec& e compagnia bella...
    // Non sarà una passeggiata adattare il formato della ReportEvent
    // per farlo funzionare come la FmtStr di Delphi...
    // Ogni TVarRec& deve essere convertibile in stringa in modo plausibile
    // Si portrebbe passare anche un formattatore...

private:
    String name_;
    HANDLE eventLog_;
    MessagesModule messagesModule_;

    void AppendParams( LPCTSTR* Params, LPCTSTR Par );

    template<typename... A>
    void AppendParams( LPCTSTR* Params, LPCTSTR Par, A... Args );
};
//---------------------------------------------------------------------------

template<typename... A>
void EventLog::AppendParams( LPCTSTR* Params, LPCTSTR Par, A... Args )
{
    *Params++ = Par;
    AppendParams( Params, Args... );
}

template<typename... A>
String EventLog::LogMessage( unsigned EventType, int Category, int ID,
                             LPCTSTR Par, A... Args )
{
    LPCTSTR ParamPtrs[1 + sizeof...( Args )];
    AppendParams( ParamPtrs, Par, Args... );
    if ( !::ReportEvent( eventLog_, EventType, Category, ID, 0,
                         1 + sizeof...( Args ), 0, ParamPtrs, 0 ) )
    {
        RaiseLastOSError();
    }
    return messagesModule_.GetMessage( ID, ParamPtrs );
}
//---------------------------------------------------------------------------

template<class InputIterator>
String EventLog::LogMessage( unsigned EventType, int Category, int ID,
                             InputIterator Begin, InputIterator End )
{
    std::vector<LPCTSTR> ParamPtrs;
    for ( ; Begin != End ; Begin++ ) {
        ParamPtrs.push_back( Begin->c_str() );
    }
    if ( !::ReportEvent( eventLog_, EventType, Category, ID, 0,
                         static_cast<WORD>( ParamPtrs.size() ),
                         0, &ParamPtrs[0], 0 ) )
    {
        RaiseLastOSError();
    }
    return messagesModule_.GetMessage( ID, &ParamPtrs[0] );
}
//---------------------------------------------------------------------------
#endif
