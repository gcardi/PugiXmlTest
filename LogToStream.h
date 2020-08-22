//---------------------------------------------------------------------------

#ifndef LogToStreamH
#define LogToStreamH

#include <System.Classes.hpp>

#include <memory>

#include "LogBase.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToStream : public FilteredLog<T> {
public:
    explicit LogToStream( TStream* Stream = 0 )
      : stream_( Stream ) {}

    template<typename F>
    LogToStream( F Filter, TStream* Stream = 0 )
      : FilteredLog<T>( Filter )
      , stream_( Stream ) {}
protected:
    void AppendRawLine( String Line = EmptyStr );

    virtual void DoAppend( const T& Item ) override;
    virtual void DoClear() override;
    virtual void DoFlush() override;
    virtual void DoAppendRawLine( String Line );

    virtual String GetText( const T& Item ) const = 0;

    void ConnectTo( TStream& Stream );
    TStream* GetStream() const { return stream_; }
private:
    TStream* stream_;
    std::unique_ptr<TStreamWriter> writer_;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::ConnectTo( TStream& Stream )
{
    stream_ = &Stream;
    writer_.reset( new TStreamWriter( stream_ ) );
}
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::AppendRawLine( String Line )
{
    DoAppendRawLine( Line );
}
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::DoAppendRawLine( String Line )
{
    if ( stream_ ) {
        stream_->Seek( 0, soFromEnd );
        writer_->WriteLine( Line );
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::DoAppend( T const & Item )
{
    AppendRawLine( GetText( Item ) );
}
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::DoClear()
{
    if ( stream_ ) {
        stream_->Size = 0;
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogToStream<T>::DoFlush()
{
    LogBase<T>::DoFlush();
    if ( writer_ ) {
        writer_->Flush();
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
