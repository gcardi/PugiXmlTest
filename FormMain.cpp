//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <type_traits>
#include <array>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <random>

#include <System.IOUtils.hpp>

#include <anafestica/FileVersionInfo.h>

#include "Cursor.h"
#include "AppStrIds.h"
#include "Globals.h"
#include "pugixmlw.hpp"
#include "StopWatch.h"
#include "WaitableTimer.h"
#include "FormInfo.h"
#include "FormMain.h"

using Anafestica::TFileVersionInfo;

using std::array;
using std::max;
using std::unique_ptr;
using std::make_unique;
using std::lock_guard;
using std::mutex;

using Severity = SvcApp::Log::DefSeverityType;
using SvcApp::LogMessage;
using SvcApp::LogDbgMessage;
using SvcApp::LogInfoMessage;

#define  WM_REFRESH_LOG_GRID     ( WM_USER + 100 )

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NDrawGrid"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TfrmMain( Owner, StoreOpts::All, nullptr )
{
}
//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain( TComponent* Owner, StoreOpts StoreOptions,
                           Anafestica::TConfigNode* const RootNode )
    : TConfigRegistryForm( Owner, StoreOptions, RootNode )
{
#if !defined( _DEBUG ) || defined( SPLASH_SCREEN )
    TWaitableTimer WaitableTimerSplash( false );

    WaitableTimerSplash.SetPeriod( static_cast<__int64>( 2000 ) * -10000I64, 0 );

    unique_ptr<TfrmInfo> Info( new TfrmInfo( Vcl::Forms::Application ) );
    Info->VersionText = TFileVersionInfo{ GetModuleFileName() }.ProductVersion;
    Info->Visible = true;
    Info->Text = LoadStr( SPLASH_SCRN_INIT_TEXT );
#endif

    SetupCaption();
    RestoreProperties();
    SvcApp::LogObj =
        CreateLogSubsystem(
            [this]( auto const & Item, bool Flush ){
                if ( logInMem_ ) {
                    lock_guard<mutex> Guard( mtxLogInMem_ );
                    logInMem_->Append( Item, Flush );
                }
            }
        );
    LogInfoMessage( LOG_MSG_START_APP );

    LogDbgMessage( _D( "Evaluate Spin Delay" ) );

    spinCountPerMicroSec_ = EstimateSpinCountPerMicroSec();

    LogDbgMessage(
        Format(
            _D( "Estimated spin count per µs: %.2f" ),
            ARRAYOFCONST(( static_cast<long double>( spinCountPerMicroSec_ ) ))
        )
    );


    LogDbgMessage( Format( _T( "Start test 100 us" ) ) );
    StopWatch SW;
    DelayMicroSec( 100 );
    auto Stop = SW.GetElapsedMicroseconds().QuadPart;
    LogDbgMessage( Format( _T( "Test: %u" ), ARRAYOFCONST(( Stop )) ) );

    LogDbgMessage( Format( _T( "Start test 1 ms" ) ) );
    SW.Start();
    DelayMicroSec( 1000 );
    Stop = SW.GetElapsedMicroseconds().QuadPart;
    LogDbgMessage( Format( _T( "Test: %u" ), ARRAYOFCONST(( Stop )) ) );

    LogDbgMessage( Format( _T( "Start test 10 ms" ) ) );
    SW.Start();
    DelayMicroSec( 10000 );
    Stop = SW.GetElapsedMicroseconds().QuadPart;
    LogDbgMessage( Format( _T( "Test: %u" ), ARRAYOFCONST(( Stop )) ) );

    LogDbgMessage( Format( _T( "Start test 100 ms" ) ) );
    SW.Start();
    DelayMicroSec( 100000 );
    Stop = SW.GetElapsedMicroseconds().QuadPart;
    LogDbgMessage( Format( _T( "Test: %u" ), ARRAYOFCONST(( Stop )) ) );

    LogDbgMessage( Format( _T( "Start test 1 s" ) ) );
    SW.Start();
    DelayMicroSec( 1000000 );
    Stop = SW.GetElapsedMicroseconds().QuadPart;
    LogDbgMessage( Format( _T( "Test: %u" ), ARRAYOFCONST(( Stop )) ) );

#if !defined( _DEBUG ) || defined( SPLASH_SCREEN )
    WaitableTimerSplash.WaitFor( INFINITE );
#endif

//    actHelpAbout->Enabled = true;

}
//---------------------------------------------------------------------------

__fastcall TfrmMain::~TfrmMain()
{
    LogInfoMessage( LOG_MSG_END_APP );
    try {

for( auto& t : tasks_ ) {
    t.wait();
}

        SaveProperties();
        SvcApp::LogObj.reset();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

String TfrmMain::GetModuleFileName()
{
    return GetModuleName( reinterpret_cast<unsigned>( HInstance ) );
}
//---------------------------------------------------------------------------

void TfrmMain::SetupCaption()
{
    Caption =
        Format(
            _T( "%s, Ver %s" ),
            ARRAYOFCONST( (
                Application->Title,
                TFileVersionInfo{ GetModuleFileName() }.ProductVersion
            ) )
        );
}
//---------------------------------------------------------------------------

void TfrmMain::RestoreProperties()
{
    RESTORE_LOCAL_PROPERTY( SourceFolder );
    //RESTORE_LOCAL_PROPERTY( SourceFolderMRUList );
    GetConfigNode().GetItem( _D( "SourceFolderMRUList" ), SourceFolderMRUList );
    RESTORE_LOCAL_PROPERTY( DataExtractionTime );
    RESTORE_LOCAL_PROPERTY( DBMSStoreTime );
}
//---------------------------------------------------------------------------

void TfrmMain::SaveProperties() const
{
    SAVE_LOCAL_PROPERTY( SourceFolder );
    SAVE_LOCAL_PROPERTY( SourceFolderMRUList );
    SAVE_LOCAL_PROPERTY( DataExtractionTime );
    SAVE_LOCAL_PROPERTY( DBMSStoreTime );
}
//---------------------------------------------------------------------------

String TfrmMain::GetSourceFolder() const
{
    return comboboxSourceFolder->Text;
}
//---------------------------------------------------------------------------

void TfrmMain::SetSourceFolder( String Val )
{
    comboboxSourceFolder->Text = Val;
}
//---------------------------------------------------------------------------

void TfrmMain::AddToMRUList( TStrings& List, String Val, int MaxItems )
{
    struct UpdateManager {
        TStrings& sl_;
        UpdateManager( TStrings& Strs ) : sl_{ Strs } { Strs.BeginUpdate(); }
        ~UpdateManager() { try { sl_.EndUpdate(); } catch ( ... ) {} }
        UpdateManager( UpdateManager const & ) = delete;
        UpdateManager& operator=( UpdateManager const & ) = delete;
    } UpdateMngr{ List };

    auto Idx = List.IndexOf( Val );
    if ( Idx >= 0 ) {
        List.Delete( Idx );
    }
    else {
        while ( List.Count >= MaxItems ) {
            List.Delete( List.Count - 1 );
        }
    }
    List.Insert( 0, Val );
}
//---------------------------------------------------------------------------

TStrings& TfrmMain::GetSourceFolderMRUList() const
{
    return *comboboxSourceFolder->Items;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actFileSelectFolderExecute(TObject *Sender)
{
    fileopndlgSelectFolder->FileName = SourceFolder;
    if ( fileopndlgSelectFolder->Execute() ) {
        SourceFolder = fileopndlgSelectFolder->FileName;
        AddToMRUList( GetSourceFolderMRUList(), SourceFolder );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actFileSelectFolderUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actProcessFolderStartExecute(TObject *Sender)
{
    if ( TDirectory::Exists( SourceFolder ) ) {
        AddToMRUList( GetSourceFolderMRUList(), SourceFolder );
        ProcessFolder( SourceFolder );
    }
    else {
        throw Exception(
            _D( "la cartella \'%s\' non esiste" ),
            ARRAYOFCONST(( SourceFolder ))
        );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actProcessFolderStartUpdate(TObject *Sender)
{
    TAction& Action = static_cast<TAction&>( *Sender );
    Action.Enabled = !runningTasks_ &&
                     IsDataExtractionTimeValid() &&
                     IsDBMSStoreTimeValid();
}
//---------------------------------------------------------------------------

bool TfrmMain::GetLogUpdateActive() const
{
    return logUpdateActive_;
}
//---------------------------------------------------------------------------

void TfrmMain::SetLogUpdateActive( bool Val )
{
    logUpdateActive_ = Val;
    actLogUpdateActive->Checked = Val;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actLogUpdateActiveExecute(TObject *Sender)
{
    TAction& Action = static_cast<TAction&>( *Sender );
    logUpdateActive_ = Action.Checked;
    UpdateLogViewer();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actLogUpdateActiveUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actLogClearExecute(TObject *Sender)
{
    if ( logInMem_ ) {
        logInMem_->Clear();
        UpdateLogViewer( true );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actLogClearUpdate(TObject *Sender)
{
    TAction& Action = static_cast<TAction&>( *Sender );
    Action.Enabled = logInMem_ ? logInMem_->GetItemCount() : false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Timer1Timer(TObject *Sender)
{
    UpdateLogViewer();
    if ( Timer1->Tag ) {
        Timer1->Tag = false;
    }
    else {
        Timer1->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void TfrmMain::KickLogTimer()
{
    if ( Timer1->Enabled ) {
        Timer1->Tag = true;
    }
    else {
        Timer1->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::WndProc( Winapi::Messages::TMessage &Message )
{
    switch ( Message.Msg ) {
        case WM_REFRESH_LOG_GRID:
            KickLogTimer();
            break;
        default:
            TConfigRegistryForm::WndProc( Message );
            break;
    }
}
//---------------------------------------------------------------------------

void TfrmMain::LogNotifyChanges( LogInMemType& Sender,
                                 LogInMemType::ReasonType Reason )
{
    if ( logUpdateActive_  ) {
        if ( guiThId_ == TThread::CurrentThread->ThreadID ) {
            KickLogTimer();
            //UpdateLogViewer();
        }
        else {
            ::PostMessage(
                Handle, WM_REFRESH_LOG_GRID,
                static_cast<WPARAM>( Reason ), 0
            );
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ndrawgridLogDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
    using LogItemType = LogInMemType::ItemType;

    TNDrawGrid& Grid = static_cast<TNDrawGrid&>( *Sender );

    if ( ARow && !ACol && logInMem_ ) {
        try {
            size_t const Idx = RowNum2Idx( Grid, ARow );

            if ( Idx < logInMem_->GetItemCount() ) {
                LogItemType const & Item = logInMem_->GetItem( Idx );

                int const ImageIndex = SeverityToImageIndex( Item.GetSeverity() );

                if ( ImageIndex >= 0 && ImageIndex < imglistLog->Count ) {
                    int const MissingHalfGapWidth =
                        ( Grid.ColWidths[ACol] - Rect.Width() ) / 2;
                    TRect AdjustedRect(
                        Rect.Left - MissingHalfGapWidth + 1,
                        Rect.Top,
                        Rect.Right + MissingHalfGapWidth - 1,
                        Rect.Bottom
                    );
                    int const ImageLeft =
                        ( AdjustedRect.Left + AdjustedRect.Right - imglistLog->Width ) / 2;
                    int const ImageTop =
                        ( AdjustedRect.Top + AdjustedRect.Bottom - imglistLog->Height ) / 2;
                    imglistLog->Draw( Grid.Canvas, ImageLeft, ImageTop, ImageIndex );
                }
            }
        }
        catch ( std::out_of_range ) {
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ndrawgridLogFixedCellGetText(TObject *Sender, int ACol,
          int ARow, UnicodeString &Value)
{
    using DescriptionCont = array<String,6>;

    static DescriptionCont const Description = {
        LoadStr( LOG_GRID_HDR_SEVERITY_ICN ),
        LoadStr( LOG_GRID_HDR_DATETIME ),
        LoadStr( LOG_GRID_HDR_SEVERITY ),
        LoadStr( LOG_GRID_HDR_SOURCE ),
        LoadStr( LOG_GRID_HDR_CODE ),
        LoadStr( LOG_GRID_HDR_MESSAGE ),
    };

    Value = ACol >= 0 && ACol < static_cast<int>( Description.size() ) ?
        Description[ACol] : DescriptionCont::value_type();
}
//---------------------------------------------------------------------------

int TfrmMain::SeverityToImageIndex( LogInMemType::ItemType::SeverityType Severity )
{
    switch ( Severity ) {
        case LogInMemType::ItemType::SeverityType::Emergency:     return 2;
        case LogInMemType::ItemType::SeverityType::Alert:         return 2;
        case LogInMemType::ItemType::SeverityType::Critical:      return 2;
        case LogInMemType::ItemType::SeverityType::Error:         return 2;
        case LogInMemType::ItemType::SeverityType::Warning:       return 1;
        case LogInMemType::ItemType::SeverityType::Notice:        return 0;
        //case LogInMemType::ItemType::SeverityType::Informational: return 0;
        case LogInMemType::ItemType::SeverityType::Debug:         return 3;
        default:                                                  return -1;
    }
}
//---------------------------------------------------------------------------

int TfrmMain::GetLogViewerRowCount() const
{
    return ndrawgridLog->Tag; // <- tiene il vero valore di RowCount
}
//---------------------------------------------------------------------------

void TfrmMain::SetLogViewerRowCount( int Val )
{
    int const NewRowCount = max( 1, Val ) + ndrawgridLog->FixedRows;
    if ( ndrawgridLog->RowCount == NewRowCount ) {
        ndrawgridLog->Invalidate();
    }
    ndrawgridLog->RowCount = NewRowCount;
    ndrawgridLog->Tag = Val;  // <- tiene il vero valore di RowCount
}
//---------------------------------------------------------------------------

void TfrmMain::SetLogViewerSelectedRow( int Val )
{
    ndrawgridLog->Row = Val + ndrawgridLog->FixedRows;
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateLogViewer( bool Force )
{
    if ( ( LogUpdateActive || Force ) && logInMem_ ) {
        int const NewRowCount = logInMem_->GetItemCount();
        if ( GetLogViewerRowCount() != NewRowCount ) {
            SetLogViewerRowCount( NewRowCount );
            SetLogViewerSelectedRow( max( NewRowCount - 1, 0 ) );
        }
    }
}
//---------------------------------------------------------------------------

unique_ptr<TfrmMain::LogObjType> TfrmMain::CreateLogSubsystem( LogAppendCallbackType OnLogAppendItem )
{
    auto ALogObj =
        make_unique<SvcApp::Log::LogAsyncChain<LogObjType::ItemType>>();

    ALogObj->SetAppendCallback( OnLogAppendItem );

    /*
    try {
        if ( ConfigObj->GetLog().GetFile().GetEnabled() ) {
            try {
                ALogObj->AppendLogObj( CreateLogToFileSubsystem() );
            }
            catch ( Exception const & E ) {
                // Probabilmente il file bloccato, o ci sono problemi di permessi
                LogMessageTo(
                    *ALogObj, LOG_MSG_SVC_LOG_TO_FILE_SUBSYS_CREATE_ERR,
                    LogItemType::SeverityType::Warning
                );
            }
        }
    }
    catch ( Exception const & E ) {
        LogMessageTo( *ALogObj, LOG_MSG_FAILURE, LogItemType::SeverityType::Error, E.Message );
    }
    catch ( ... ) {
        LogMessageTo( *ALogObj, LOG_MSG_UNKNOWN_FAILURE, LogItemType::SeverityType::Error );
    }
    */

    return ALogObj;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ndrawgridLogNormalCellGetText(TObject *Sender, int ACol,
          int ARow, UnicodeString &Value)
{
    using ItemType = LogInMemType::ItemType;

    if ( logInMem_ ) {
        TNDrawGrid& Grid = static_cast<TNDrawGrid&>( *Sender );
        size_t const Idx = RowNum2Idx( Grid, ARow );
        if ( Idx < logInMem_->GetItemCount() ) {
            ItemType Item = logInMem_->GetItem( Idx );
            switch ( ACol ) {
                case 1:              // DATETIME
                    Value = FormatDateTime(
                        LoadStr( LOG_DATE_TIME_FORMAT ),
                        Item.GetDateTime()
                    );
                    break;
                case 2:              // SEVERITY
                    Value = LoadStr(
                        LOG_SEVERITY_BASE_SHORT +
                        static_cast<int>( Item.GetSeverity() )
                    );
                    break;
                case 3:              // SOURCE
                    Value = LoadStr( Item.GetEntity() );
                    break;
                case 4:              // CODE
                    Value = Item.GetMessage();
                    break;
                case 5:              // MESSAGE
                    Value = GetMessageText( Item );
                    break;
                default:
                    break;
            }
        }
    }
}
//---------------------------------------------------------------------------

template<typename T>
static String GetFileSizeText( T const & Path )
{
    auto const Size = file_size( Path );
    if ( Size < 10240 ) {
        return Format( _D( "%u Bytes" ), Size );
    }
    else if ( Size < 0x100000LL ) {
        return Format( _D( "%u KiB" ), Size / 0x400LL );
    }
    else if ( Size < 0x40000000LL ) {
        return Format( _D( "%u MiB" ), Size / 0x100000LL );
    }
    else if ( Size < 0x10000000000LL ) {
        return Format( _D( "%u GiB" ), Size / 0x40000000LL );
    }
    else {
        return _D( "> 1TiB" );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Timer2Timer(TObject *Sender)
{
    auto const RunningTaskCount = runningTasks_.load();
    StatusBar1->Panels->Items[0]->Text =
        RunningTaskCount ?
            Format( _D( "Running (%u)" ), runningTasks_.load() )
        :
            String( _D( "Idle" ) );
}
//---------------------------------------------------------------------------

std::tuple<double,double> TfrmMain::EvaluateMTSpinDelay( size_t SampleCount,
                                                         size_t SpinCount )
{
    std::vector<double> Samples;
    Samples.reserve( SampleCount );

#if 1
    {
        using SpinTaskCont = std::vector<std::future<double>>;

        SpinTaskCont SampleTasks;
        SampleTasks.reserve( SampleCount );
        for ( size_t i {} ; i < SampleCount ; ++i ) {
            SampleTasks.push_back(
                std::async(
                    std::launch::async,
                    [SpinCount]() -> double {
                        StopWatch SW;
                        size_t volatile z = SpinCount;
                        while ( z-- ) {}
                        return SW.GetElapsedMicroseconds().QuadPart;
                    }
                )
            );
        }
        std::transform(
            std::begin( SampleTasks ), std::end( SampleTasks ),
            std::back_inserter( Samples ),
            []( auto& Val ){ return Val.get(); }
        );
    }
#else
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{5,2};

    for ( size_t i {} ; i < SampleCount ; ++i ) {
        Samples.push_back( d( gen ) );
    }

#endif

    auto const Sum =
        std::accumulate(
            std::begin( Samples ), std::end( Samples ), 0.0
        );

    auto const Mu = Sum / SampleCount;

    auto const Sigma =
        sqrt(
            std::accumulate(
                std::begin( Samples ), std::end( Samples ), 0.0,
                [Mu]( auto Lhs, auto Rhs ) {
                    auto const Diff = Rhs - Mu;
                    return Lhs + ( Rhs - Mu ) * ( Rhs - Mu );
                }
            ) / SampleCount
        );

    return std::make_tuple( Mu, Sigma );
}
//---------------------------------------------------------------------------

std::tuple<double,double> TfrmMain::MeasureMTSpinDelay( size_t SampleCount,
                                                        double Delay,
                                                        double SpinCountPerMicroSec )
{
    std::vector<double> Samples;
    Samples.reserve( SampleCount );

    {
        using SpinTaskCont = std::vector<std::future<double>>;

        SpinTaskCont SampleTasks;
        SampleTasks.reserve( SampleCount );
        for ( size_t i {} ; i < SampleCount ; ++i ) {
            SampleTasks.push_back(
                std::async(
                    std::launch::async,
                    [Delay,SpinCountPerMicroSec]() -> double {
                        StopWatch SW;
                        DelayMicroSec( Delay, SpinCountPerMicroSec );
                        return SW.GetElapsedMicroseconds().QuadPart;
                    }
                )
            );
        }
        std::transform(
            std::begin( SampleTasks ), std::end( SampleTasks ),
            std::back_inserter( Samples ),
            []( auto& Val ){ return Val.get(); }
        );
    }

    auto const Sum =
        std::accumulate(
            std::begin( Samples ), std::end( Samples ), 0.0
        );

    auto const Mu = Sum / SampleCount;

    auto const Sigma =
        sqrt(
            std::accumulate(
                std::begin( Samples ), std::end( Samples ), 0.0,
                [Mu]( auto Lhs, auto Rhs ) {
                    auto const Diff = Rhs - Mu;
                    return Lhs + ( Rhs - Mu ) * ( Rhs - Mu );
                }
            ) / SampleCount
        );

    return std::make_tuple( Mu, Sigma );
}
//---------------------------------------------------------------------------

double TfrmMain::EstimateSpinCountPerMicroSec()
{
    static constexpr size_t SampleCount = 2000;
    static constexpr size_t SpinCount = 2500;
    auto [Mu, Sigma] = EvaluateMTSpinDelay( SampleCount, SpinCount );

    LogDbgMessage(
        Format(
            _D( "La media per %u misurazioni di %u spin è %.1f µs, sigma: %.1f" ),
            ARRAYOFCONST(( SampleCount, SpinCount, Mu, Sigma ))
        )
    );

    return static_cast<double>( SpinCount ) / Mu; // * 0.99;
}
//---------------------------------------------------------------------------

String TfrmMain::GetDataExtractionTime() const
{
    return edtDataExtractionTime->Text;
}
//---------------------------------------------------------------------------

void TfrmMain::SetDataExtractionTime( String Val )
{
    edtDataExtractionTime->Text = Val;
}
//---------------------------------------------------------------------------

String TfrmMain::GetDBMSStoreTime() const
{
    return edtDBMSStoreTime->Text;
}
//---------------------------------------------------------------------------

void TfrmMain::SetDBMSStoreTime( String Val )
{
    edtDBMSStoreTime->Text = Val;
}
//---------------------------------------------------------------------------

int TfrmMain::GetDataExtractionTimeVal() const
{
    return GetDataExtractionTime().ToIntDef( -1 );
}
//---------------------------------------------------------------------------

int TfrmMain::GetDBMSStoreTimeVal() const
{
    return GetDBMSStoreTime().ToIntDef( -1 );
}
//---------------------------------------------------------------------------

bool TfrmMain::IsDataExtractionTimeValid() const
{
    // 2-3000 µs
    auto const Val = GetDataExtractionTimeVal();
    return Val >= 2 && Val <= 3000;
}
//---------------------------------------------------------------------------

bool TfrmMain::IsDBMSStoreTimeValid() const
{
    // 20-20000 µs
    auto const Val = GetDBMSStoreTimeVal();
    return Val >= 20 && Val <= 20000;
}
//---------------------------------------------------------------------------

void TfrmMain::ProcessFolder( String Path )
{
    TCursorManager CursorMngr;
    LogDbgMessage(
        Format(
            _D( "Collezione nomi file per il percorso \'%s\'" ),
            Path
        )
    );

    namespace fs = std::filesystem;

    std::vector<std::filesystem::directory_entry> Files;

    for( auto const & p : fs::directory_iterator( SourceFolder.c_str() ) ) {
        if ( SameText( p.path().extension().c_str(), _D( ".xml" ) ) ) {
            Files.push_back( p );
        }
    }

    auto const PTime = GetDataExtractionTimeVal();
    auto const STime = GetDBMSStoreTimeVal();

    TaskCont Tasks;
    Tasks.reserve( Files.size() );
    runningTasks_ = Files.size();

    LogDbgMessage( Format( _D( "Inizio processing %d file XML" ), Files.size() ) );

    for ( auto File : Files ) {
        Tasks.push_back(
            std::async(
                std::launch::async,
                [this,PTime,STime]( auto const & FileEntry ) {
                    StopWatch SW;
                    pugiw::xml_document doc;
                    auto result = doc.load_file( FileEntry.path().c_str() );
                    auto const ParsingTime =
                        SW.GetElapsedMicroseconds().QuadPart;

                    SW.Start();
                    DelayMicroSec( PTime );
                    auto const ProcessingTime =
                        SW.GetElapsedMicroseconds().QuadPart;

                    SW.Start();
                    DelayMicroSec( STime );
                    auto const StoreTime =
                        SW.GetElapsedMicroseconds().QuadPart;

                    LogDbgMessage(
                        Format(
                            _D( "File \'%s\' (%s), %s parsing time %u µs, processing time %u µs" ),
                            String( FileEntry.path().filename().c_str() ),
                            GetFileSizeText( FileEntry.path() ),
                            result.description(),
                            ParsingTime,
                            ProcessingTime
                        )
                    );

                    if ( --runningTasks_ == 0 ) {
                        LogDbgMessage( _D( "End processing" ) );
                    }
                },
                File
            )
        );
    }
    tasks_ = std::move( Tasks );
}
//---------------------------------------------------------------------------


