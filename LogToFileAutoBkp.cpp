//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.Zip.hpp>

#include <memory>

#include "LogToFileAutoBkp.h"

using std::unique_ptr;

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

void ZipStream( TStream& InputStream, String InputFileName, String OutputArchiveFileName )
{
    unique_ptr<TZipFile> Zip( new TZipFile() );
    Zip->Open( OutputArchiveFileName, zmWrite );
    Zip->Add( &InputStream, InputFileName );
    Zip->Close();
}
//---------------------------------------------------------------------------

void ZipFile( String InputFileName, String OutputArchiveFileName )
{
    unique_ptr<TZipFile> Zip( new TZipFile() );
    Zip->Open( OutputArchiveFileName, zmWrite );

    //Zip->Add( InputFileName ); <-- dava errore se aperto con
    //    tail --follow=name --retry Test.log
    //  sostituito con le 4 righe sotto
    unique_ptr<TFileStream> FS(
        new TFileStream( InputFileName, fmOpenRead | fmShareDenyNone )
    );
    Zip->Add( FS.get(), ExtractFileName( InputFileName ) );

    Zip->Close();
}
//---------------------------------------------------------------------------

TDateTime FileTimeToDateTime( FILETIME const & FileTime )
{
    FILETIME LocalFileTime;
    SYSTEMTIME SystemTime;

    ::FileTimeToLocalFileTime( &FileTime, &LocalFileTime );
    ::FileTimeToSystemTime( &LocalFileTime, &SystemTime );
    return SystemTimeToDateTime( SystemTime );
}

//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
