//---------------------------------------------------------------------------

#ifndef LogToFileH
#define LogToFileH

#include <memory>

#include "LogToStream.h"
#include "LogCodes.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToFile : public LogToStream<T> {
public:
    explicit LogToFile( String FileName );

    template<typename F>
    LogToFile( F Filter, String FileName );
    ~LogToFile() override;
    void Open( String FileName );
    String GetFileName() const;
    HANDLE GetHandle() const;
protected:
    virtual void DoClear() override {}
    virtual void DoFlush() override;

    virtual String GetText( const T& Item ) const;

    void Close();
private:
    std::unique_ptr<TFileStream> fileStream_;

    static String GetHeaderText();
    String GetSeparatorText();
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template<typename T>
LogToFile<T>::LogToFile( String FileName )
{
    Open( FileName );
}
//---------------------------------------------------------------------------

template<typename T>
template<typename F>
LogToFile<T>::LogToFile( F Filter, String FileName )
  : LogToStream<T>( Filter )
{
    Open( FileName );
}
//---------------------------------------------------------------------------

template<typename T>
LogToFile<T>::~LogToFile()
{
    try {
        Close();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogToFile<T>::Open( String FileName )
{
    FileName = ExpandFileName( FileName );
    if ( FileExists( FileName ) ) {
        fileStream_.reset(
            new TFileStream( FileName, fmOpenWrite | fmShareDenyWrite )
        );
        LogToStream<T>::ConnectTo( *fileStream_ );
    }
    else {
        fileStream_.reset(
            new TFileStream( FileName, fmCreate | fmShareDenyWrite )
        );
        LogToStream<T>::ConnectTo( *fileStream_ );
        LogToStream<T>::AppendRawLine(
            FmtLoadStr(
                LOG_FILE_CREATION_MSG,
                ARRAYOFCONST( (
                    FormatDateTime(
                        LoadStr( LOG_DATE_TIME_FORMAT ), //_T( "ddddd tt" ),
                        Now()
                    )
                ) )
            )
        );
        LogToStream<T>::AppendRawLine();
        LogToStream<T>::AppendRawLine( GetHeaderText() );
        LogToStream<T>::AppendRawLine( GetSeparatorText() );
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogToFile<T>::DoFlush()
{
    LogToStream<T>::DoFlush();
    if ( HANDLE hStream = GetHandle() ) {
        ::FlushFileBuffers( hStream );
    }
}
//---------------------------------------------------------------------------

template<typename T>
String LogToFile<T>::GetText( const T& Item ) const
{
    return FmtLoadStr(
       LOG_ITEM_FMT_TXT,
       ARRAYOFCONST( (
           FormatDateTime(
               LoadStr( LOG_DATE_TIME_FORMAT ),  // _T( "ddddd tt" )
               Item.GetDateTime()
           ),
           LoadStr( LOG_SEVERITY_BASE_SHORT + static_cast<int>( Item.GetSeverity() ) ),
           LoadStr( LOG_FACILITY_BASE_SHORT + static_cast<int>( Item.GetFacility() ) ),
           LoadStr( Item.GetEntity() ),
           Item.GetEntity(),
           Item.GetMessage(),
           GetMessageText( Item )
       ) )
   );
}
//---------------------------------------------------------------------------


/*
template<typename T>
void LogToFile<T>::DoClose()
{
    LogToStream<T>::DoClose();
    fileStream_.reset();
}
*/

template<typename T>
void LogToFile<T>::Close()
{
    LogToStream<T>::Flush();
    fileStream_.reset();
}
//---------------------------------------------------------------------------

template<typename T>
String LogToFile<T>::GetFileName() const
{
    if ( TFileStream * const Stream = dynamic_cast<TFileStream*>(
            LogToStream<T>::GetStream() ) )
    {
        return ExpandFileName( Stream->FileName );
    }
    return String();
}
//---------------------------------------------------------------------------

template<typename T>
HANDLE LogToFile<T>::GetHandle() const
{
    if ( THandleStream * const Stream =
            dynamic_cast<THandleStream*>( LogToStream<T>::GetStream() ) )
    {
        return reinterpret_cast<HANDLE>( Stream ? Stream->Handle : 0 );
    }
    return 0;
}
//---------------------------------------------------------------------------

extern String PadStringToCenter( String Txt, int MaxLen, TCHAR FillChar = _D( ' ' ) );

template<typename T>
String LogToFile<T>::GetHeaderText()
{
    return FmtLoadStr(
        LOG_ITEM_HDR_TXT,
        ARRAYOFCONST( (
            PadStringToCenter(
                LoadStr( LOG_ITEM_HDR_DT_TXT ),
                FormatDateTime(
                    LoadStr( LOG_DATE_TIME_FORMAT ),  // _T( "ddddd tt" )
                    TDateTime()
                ).Length()
//                , _D( ' ' )
            )
        ) )
    );
}
//---------------------------------------------------------------------------

template<typename T>
String LogToFile<T>::GetSeparatorText()
{
    return LoadStr( LOG_ITEM_SEP_TXT );
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
