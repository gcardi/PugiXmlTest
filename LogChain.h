//---------------------------------------------------------------------------

#ifndef LogChainH
#define LogChainH

#include <vector>
#include <utility>
#include <memory>

#include "LogBase.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogChain : public FilteredLog<T> {
public:
//    using LogObjPtrType = std::shared_ptr<LogBase<T>>;
    using LogObjPtrType = std::unique_ptr<LogBase<T>>;

    LogChain() : FilteredLog<T>() {}

    template<typename F>
    explicit LogChain( F Filter ) : FilteredLog<T>( Filter ) {}

    ~LogChain() override {}

    void AppendLogObj( LogObjPtrType&& ObjPtr ) { DoAppendLogObj( std::move( ObjPtr ) ); }
protected:
    virtual void DoAppend( T const & Item ) override;
    virtual void DoClear() override;
    virtual void DoFlush() override;
    virtual void DoAppendLogObj( LogObjPtrType&& ObjPtr );
private:
    using LogObjPtrCont = std::vector<LogObjPtrType>;

    LogObjPtrCont objs_;
};
//---------------------------------------------------------------------------

template<typename T>
void LogChain<T>::DoAppendLogObj( LogObjPtrType&& ObjPtr )
{
//    objs_.push_back( typename LogObjPtrCont::value_type{ ObjPtr } );
    objs_.push_back( std::move( ObjPtr ) );
}
//---------------------------------------------------------------------------

template<typename T>
void LogChain<T>::DoAppend( T const & Item )
{
    for ( auto& ObjPtr : objs_ ) {
        try {
            ObjPtr->Append( Item );
        }
        catch ( ... ) {
        }
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogChain<T>::DoClear()
{
    for ( auto& ObjPtr : objs_ ) {
        try {
            ObjPtr->Clear();
        }
        catch ( ... ) {
        }
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogChain<T>::DoFlush()
{
    for ( auto& ObjPtr : objs_ ) {
        try {
            ObjPtr->Flush();
        }
        catch ( ... ) {
        }
    }
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
