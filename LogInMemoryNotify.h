//---------------------------------------------------------------------------

#ifndef LogInMemoryNotifyH
#define LogInMemoryNotifyH
//---------------------------------------------------------------------------

#include "LogInMemory.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogInMemoryNotify : public LogInMemory<T> {
public:
    using SizeType = typename LogInMemory<T>::SizeType;

    enum class ReasonType { Clear, Append, Cut };

    using ChgFnType = std::function<void ( LogInMemoryNotify&, ReasonType )>;

    LogInMemoryNotify( ChgFnType OnChange, SizeType InitialResevedSize = 3000,
                       SizeType MaxSize = 3000, SizeType CutSize = 500 );

protected:
    virtual void DoAppend( const T& Item ) override;
    virtual void DoClear() override;
    virtual bool DoCutItems() override;
private:
    ChgFnType onChange_;

    void NotifyChange( ReasonType Reason );
};
//---------------------------------------------------------------------------

template<typename T>
LogInMemoryNotify<T>::LogInMemoryNotify( ChgFnType OnChange,
                                            SizeType InitialResevedSize,
                                            SizeType MaxSize,
                                            SizeType CutSize )
  : LogInMemory<T>( InitialResevedSize, MaxSize, CutSize )
  , onChange_( OnChange )
{
}
//---------------------------------------------------------------------------

template<typename T>
void LogInMemoryNotify<T>::DoAppend( const T& Item )
{
    LogInMemory<T>::DoAppend( Item );
    NotifyChange( ReasonType::Append );
}
//---------------------------------------------------------------------------

template<typename T>
void LogInMemoryNotify<T>::DoClear()
{
    LogInMemory<T>::DoClear();
    NotifyChange( ReasonType::Clear );
}
//---------------------------------------------------------------------------

template<typename T>
bool LogInMemoryNotify<T>::DoCutItems()
{
    if ( LogInMemory<T>::DoCutItems() ) {
        NotifyChange( ReasonType::Cut );
        return false;
    }
    else {
        NotifyChange( ReasonType::Clear );
        return true;
    }
}
//---------------------------------------------------------------------------

template<typename T>
void LogInMemoryNotify<T>::NotifyChange( ReasonType Reason )
{
    if ( onChange_ ) {
        onChange_( *this, Reason );
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
