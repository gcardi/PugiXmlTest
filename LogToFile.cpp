//---------------------------------------------------------------------------

#pragma hdrstop

#include <algorithm>

#include "LogToFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

String PadStringToCenter( String Txt, int MaxLen, TCHAR FillChar )
{
    const int Diff = std::max( 0, MaxLen - Txt.Length() );
    const int HalfDiff = Diff / 2;
    const int LeftPadLen = Diff - HalfDiff;
    const int RightPadLen = HalfDiff;
    const int StrLen = MaxLen - Diff;
    return Format(
               _T( "%*.*s%-*.*s%*.*s" ),  // Do not localize
               ARRAYOFCONST( (
                   LeftPadLen, LeftPadLen, StringOfChar( FillChar, LeftPadLen ),
                   StrLen, StrLen, Txt,
                   RightPadLen, RightPadLen, StringOfChar( FillChar, RightPadLen )
               ) )
           );
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
