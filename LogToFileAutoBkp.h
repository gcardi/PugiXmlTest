//---------------------------------------------------------------------------

#ifndef LogToFileAutoBkpH
#define LogToFileAutoBkpH

#include "LogToFile.h"

//---------------------------------------------------------------------------
namespace SvcApp {
//---------------------------------------------------------------------------
namespace Log {
//---------------------------------------------------------------------------

template<typename T>
class LogToFileWithAutoBackup : public LogToFile<T>
{
public:
    explicit LogToFileWithAutoBackup( String FileName )
      : LogToFile<T>( FileName ) {}

    template<typename F>
    LogToFileWithAutoBackup( F Filter, String FileName )
      : LogToFile<T>( Filter, FileName ) {}
protected:
    virtual void DoAppend( const T& Item ) override;
    virtual bool DoCheckForCompression( TDateTime FileDT, TDateTime NewItemDT ) const = 0;
    virtual String DoGetArchiveFileName( TDateTime Value ) const = 0;
private:
};
//---------------------------------------------------------------------------

extern TDateTime FileTimeToDateTime( FILETIME const & FileTime );
extern void ZipStream( TStream& InputStream, String InputFileName,
                       String OutputArchiveFileName );
extern void ZipFile( String InputFileName, String OutputArchiveFileName );

//---------------------------------------------------------------------------

template<typename T>
void LogToFileWithAutoBackup<T>::DoAppend( const T& Item )
{
    if ( HANDLE h = LogToFile<T>::GetHandle() ) {
        BY_HANDLE_FILE_INFORMATION fi = { 0 };

        if ( GetFileInformationByHandle( h, &fi ) ) {
            TDateTime const LastWriteTime = FileTimeToDateTime( fi.ftLastWriteTime );
            //TDateTime const NowTime = LogToFile<T>::GetServerTime();
            TDateTime const NowTime = Now();

            if ( DoCheckForCompression( LastWriteTime, NowTime ) ) {
                String const FileName = LogToFile<T>::GetFileName();
                String const OutputArchiveFileName =
                    ChangeFilePath(
                        DoGetArchiveFileName( NowTime ),
                        ExtractFilePath( FileName )
                    );
                if ( TStream* const Stream = LogToFile<T>::GetStream() ) {
                    try {
                        LogToFile<T>::Close();
                        try {
                            if ( FileExists( OutputArchiveFileName ) ) {
                                String const ArchBakFileName =
                                    ChangeFileExt(
                                        OutputArchiveFileName, _T( ".bak" )
                                    );
                                if ( FileExists( ArchBakFileName ) ) {
                                    DeleteFile( ArchBakFileName );
                                }
                                RenameFile(
                                    OutputArchiveFileName, ArchBakFileName
                                );
                            }
                            ZipFile( FileName, OutputArchiveFileName );
                            String const BakFileName =
                                ChangeFileExt( FileName, _T( ".bak" ) );
                            if ( FileExists( BakFileName ) ) {
                                DeleteFile( BakFileName );
                            }
                            RenameFile( FileName, BakFileName );
                            DeleteFile( FileName );
                        }
                        __finally {
                            LogToFile<T>::Open( FileName );
                        }
                    }
                    catch ( ... ) {
                    }
                }
            }
        }
    }
    LogToFile<T>::DoAppend( Item );
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
}; // End of namespace Log
//---------------------------------------------------------------------------
}; // End of namespace SvcApp
//---------------------------------------------------------------------------
#endif
