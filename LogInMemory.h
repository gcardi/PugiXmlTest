//---------------------------------------------------------------------------

#ifndef LogInMemoryH
#define LogInMemoryH

#include <vector>
#include <algorithm>

#include "LogBase.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogInMemory : public FilteredLog<T> {
private:
    using LogItemCont = std::vector<T>;
public:
    using SizeType = typename LogItemCont::size_type;
    using FilterType = typename FilteredLog<T>::FilterType;

    LogInMemory( SizeType InitialResevedSize = 3000,
                 SizeType MaxSize = 3000, SizeType CutSize = 500,
                 FilterType Filter = FilterType{} );
    SizeType GetItemCount() const { return DoGetItemCount(); }
    const T GetItem( SizeType Idx ) const { return DoGetItem( Idx ); }
    SizeType GetMaxSize() const { return maxSize_; }
    SizeType GetCutSize() const { return cutSize_; }

    template<typename OutputIterator>
    void CopyTo( OutputIterator Out ) const;

protected:
    virtual void DoAppend( const T& Item ) override;
    virtual void DoClear() override;
    virtual SizeType DoGetItemCount() const;
    virtual const T DoGetItem( SizeType Idx ) const;
    virtual bool DoCutItems();
private:
    LogItemCont items_;
    SizeType maxSize_;
    SizeType cutSize_;

    bool CutItems() { return DoCutItems(); }
};
//---------------------------------------------------------------------------

template<typename T>
LogInMemory<T>::LogInMemory( SizeType InitialResevedSize, SizeType MaxSize,
                                SizeType CutSize, FilterType Filter )
  : FilteredLog<T>{ Filter }
  , maxSize_{ MaxSize }
  , cutSize_( std::max( CutSize, static_cast<SizeType>( 1U ) ) )
{
    items_.reserve( InitialResevedSize );
}
//---------------------------------------------------------------------------

template<typename T>
void LogInMemory<T>::DoAppend( T const & Item )
{
    if ( items_.size() >= maxSize_ ) {
        CutItems();
    }
    items_.push_back( Item );
}
//---------------------------------------------------------------------------

template<typename T>
void LogInMemory<T>::DoClear()
{
    items_.clear();
}
//---------------------------------------------------------------------------

template<typename T>
typename LogInMemory<T>::SizeType LogInMemory<T>::DoGetItemCount() const
{
    return items_.size();
}
//---------------------------------------------------------------------------

template<typename T>
const T LogInMemory<T>::DoGetItem( SizeType Idx ) const
{
    return items_.at( Idx );
}
//---------------------------------------------------------------------------

template<typename T>
bool LogInMemory<T>::DoCutItems()
{
    if ( items_.size() < cutSize_ ) {
        items_.clear();
        return false;
    }
    else {
        auto UpTo = items_.begin();
        advance( UpTo, cutSize_ );
        items_.erase( items_.begin(), UpTo );
    }
    return true;
}
//---------------------------------------------------------------------------

template<typename T>
template<typename OutputIterator>
void LogInMemory<T>::CopyTo( OutputIterator Out ) const
{
    std::copy( items_.begin(), items_.end(), Out );
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
