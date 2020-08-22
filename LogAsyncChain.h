//---------------------------------------------------------------------------

#ifndef LogAsyncChainH
#define LogAsyncChainH

#include <System.SyncObjs.hpp>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <atomic>

#include "LogChain.h"
//#include "Synchronize.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogAsyncChain : public LogChain<T> {
public:
    using LogObjPtrType = typename LogChain<T>::LogObjPtrType;

    LogAsyncChain();

    template<typename F>
    LogAsyncChain( F Filter );

    ~LogAsyncChain() override;

    LogAsyncChain( LogAsyncChain const & ) = delete;
    LogAsyncChain& operator=( LogAsyncChain const & ) = delete;
protected:
    virtual void DoAppend( T const & Item ) override;
    virtual void DoClear() override;
    virtual void DoFlush() override;
    virtual void DoAppendLogObj( LogObjPtrType&& ObjPtr ) override;
//    virtual void DoAppendLogObj( LogBase<T>* ObjPtr ) override;
private:
    using inherited = LogChain<T>;

    std::atomic_bool stopped_ { false };
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condVar_;
    std::thread thConsumer_;

    void Run();
};
//---------------------------------------------------------------------------

template<typename T>
LogAsyncChain<T>::LogAsyncChain()
  : LogChain<T>()
{
    Run();
}
//---------------------------------------------------------------------------

template<typename T>
template<typename F>
LogAsyncChain<T>::LogAsyncChain( F Filter )
  : LogChain<T>( Filter )
{
    Run();
}
//---------------------------------------------------------------------------

template<typename T>
LogAsyncChain<T>::~LogAsyncChain()
{
    stopped_ = true;;
    condVar_.notify_all();
    thConsumer_.join();
}
//---------------------------------------------------------------------------

template<typename T>
void LogAsyncChain<T>::Run()
{
    thConsumer_ =
        std::thread(
            [this](){
                T Item;
                while ( !stopped_ ) {
                    {
                        std::unique_lock<std::mutex> Lock( mutex_ );
                        condVar_.wait(
                            Lock, [&](){ return !queue_.empty() || stopped_; }
                        );

                        if ( !stopped_ ) {
                            if ( !queue_.empty() ) {
                                Item = std::move( queue_.front() );
                                queue_.pop();
                            }
                        }
                        else {
                            break;
                        }
                    }
                    inherited::DoAppend( Item );
                }
                {
                    std::unique_lock<std::mutex> Lock( mutex_ );
                    while ( !queue_.empty() ) {
                        Item = std::move( queue_.front() );
                        queue_.pop();
                        inherited::DoAppend( Item );
                    }
                }
                inherited::DoFlush();
            }
        );
}
//---------------------------------------------------------------------------

template<typename T>
void LogAsyncChain<T>::DoAppend( T const & Item )
{
    std::unique_lock<std::mutex> Lock( mutex_ );
    queue_.push( Item );
    condVar_.notify_one();
}
//---------------------------------------------------------------------------

template<typename T>
void LogAsyncChain<T>::DoClear()
{
    std::unique_lock<std::mutex> Lock( mutex_ );
    inherited::DoClear();
}
//---------------------------------------------------------------------------

template<typename T>
void LogAsyncChain<T>::DoFlush()
{
//    std::unique_lock<std::mutex> Lock( mutex_ );
//    inherited::DoFlush();
}
//---------------------------------------------------------------------------

template<typename T>
void LogAsyncChain<T>::DoAppendLogObj( LogObjPtrType&& ObjPtr )
//void LogAsyncChain<T>::DoAppendLogObj( LogBase<T>* ObjPtr )
{
    std::unique_lock<std::mutex> Lock( mutex_ );
    inherited::DoAppendLogObj( std::move( ObjPtr ) );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
