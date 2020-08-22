//---------------------------------------------------------------------------

#ifndef LogToFileAutoBkpWeeklyH
#define LogToFileAutoBkpWeeklyH

#include <System.hpp>
#include <System.Dateutils.hpp>

#include "LogToFileAutoBkp.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToFileAutoBkpWeekly : public LogToFileWithAutoBackup<T> {
public:
    explicit LogToFileAutoBkpWeekly( String FileName )
      : LogToFileWithAutoBackup<T>( FileName ) {}

    template<typename F>
    LogToFileAutoBkpWeekly( F Filter, String FileName )
      : LogToFileWithAutoBackup<T>( Filter, FileName ) {}
protected:
    virtual bool DoCheckForCompression( TDateTime FileDT, TDateTime NewItemDT ) const override;
    virtual String DoGetArchiveFileName( TDateTime Value ) const override;
private:
};
//---------------------------------------------------------------------------

template<typename T>
bool LogToFileAutoBkpWeekly<T>::DoCheckForCompression( TDateTime FileDT,
                                                       TDateTime NewItemDT ) const
{
    if ( FileDT < NewItemDT ) {
        if ( WeekOfTheYear( FileDT ) == WeekOfTheYear( NewItemDT ) ) {
            if ( MonthOf( FileDT ) != 12 || MonthOf( NewItemDT ) != 1 ) {
                return YearOf( FileDT ) != YearOf( NewItemDT );
            }
        }
        else {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------

template<typename T>
String LogToFileAutoBkpWeekly<T>::DoGetArchiveFileName( TDateTime Value ) const
{
    return
        String::Format(
            _T( "Log-%s-W%d.zip" ),   // Do not localize
            ARRAYOFCONST( (
                Value.FormatString( _T( "yyyy" ) ),  // Do not localize
                (int)WeekOfTheYear( IncDay( Value, -1 ) )
            ) )
        );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
