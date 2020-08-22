//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <NDrawGrid.h>
#include <Vcl.Grids.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <System.SysUtils.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ComCtrls.hpp>

#include <memory>
#include <utility>
#include <mutex>
#include <atomic>
#include <vector>
#include <future>

#include "LogInMemoryNotify.h"
#include "GridMapper.h"
#include "LogBase.h"
#include "LogAsyncChain.h"

#include <anafestica/PersistFormVCL.h>
#include <anafestica/CfgRegistrySingleton.h>

//---------------------------------------------------------------------------

using TConfigRegistryForm =
    Anafestica::TPersistFormVCL<Anafestica::TConfigRegistrySingleton>;

class TfrmMain : public TConfigRegistryForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtn1;
    TActionManager *ActionManager1;
    TAction *actFileSelectFolder;
    TAction *actProcessFolderStart;
    TFileOpenDialog *fileopndlgSelectFolder;
    TComboBox *comboboxSourceFolder;
    TPanel *pnlLog;
    TPanel *pnlLogViewer;
    TNDrawGrid *ndrawgridLog;
    TPanel *pnlLogCtrls;
    TSpeedButton *SpeedButton1;
    TCheckBox *CheckBox1;
    TAction *actLogUpdateActive;
    TAction *actLogClear;
    TImageList *imglistLog;
    TLabel *Label1;
    TActionMainMenuBar *ActionMainMenuBar1;
    TFileExit *actFileExit;
    TTimer *Timer1;
    TTimer *Timer2;
    TStatusBar *StatusBar1;
    TButton *Button1;
    void __fastcall actFileSelectFolderExecute(TObject *Sender);
    void __fastcall actFileSelectFolderUpdate(TObject *Sender);
    void __fastcall actProcessFolderStartExecute(TObject *Sender);
    void __fastcall actProcessFolderStartUpdate(TObject *Sender);
    void __fastcall actLogUpdateActiveExecute(TObject *Sender);
    void __fastcall actLogUpdateActiveUpdate(TObject *Sender);
    void __fastcall actLogClearExecute(TObject *Sender);
    void __fastcall actLogClearUpdate(TObject *Sender);
    void __fastcall ndrawgridLogDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
    void __fastcall ndrawgridLogFixedCellGetText(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value);
    void __fastcall ndrawgridLogNormalCellGetText(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
private:	// User declarations
    using LogItemType = SvcApp::Log::LogItem;
    using LogObjType = SvcApp::Log::LogBase<LogItemType>;
    using LogParamsType = SvcApp::Log::LogParams;
    using LogInMemType = SvcApp::Log::LogInMemoryNotify<LogItemType>;
    using LogAppendCallbackType = LogObjType::AppendCallbackType;

    using TaskCont = std::vector<std::future<void>>;

    std::mutex mtxLogInMem_;
    unsigned guiThId_ { TThread::CurrentThread->ThreadID };
    std::atomic<long> logUpdateActive_ { true };
    std::unique_ptr<LogInMemType> logInMem_ {
        std::make_unique<LogInMemType>(
            //[this]( auto& Sender, auto Reason ) {
            //    LogNotifyChanges( Sender, Reason );
            //}
            &LogNotifyChanges
          , 500000
          , 500000
        )
    };
    TaskCont tasks_;
    std::atomic<TaskCont::size_type> runningTasks_ {};

    static String GetModuleFileName();
    void SetupCaption();
    void RestoreProperties();
    void SaveProperties() const;
    String GetSourceFolder() const;
    void SetSourceFolder( String Val );
    static void AddToMRUList( TStrings& List, String Val, int MaxItems = 20 );
    TStrings& GetSourceFolderMRUList() const;
    void ProcessFolder( String Path );
    void UpdateLogViewer( bool Force = false );
    static int SeverityToImageIndex( LogInMemType::ItemType::SeverityType Severity );
    bool GetLogUpdateActive() const;
    void SetLogUpdateActive( bool Val );
    int GetLogViewerRowCount() const;
    void SetLogViewerRowCount( int Val );
    void SetLogViewerSelectedRow( int Val );
    static std::unique_ptr<LogObjType> CreateLogSubsystem( LogAppendCallbackType OnLogAppendItem );
    void LogNotifyChanges( LogInMemType& Sender, LogInMemType::ReasonType Reason );
    void KickLogTimer();
    static std::tuple<double,double> EvaluateMTSpinDelay( size_t SampleCount, size_t SpinCount );

    template<typename M, typename S, typename... A>
    void LogMsg( M Message, S const & Severity, A... Args ) {
        logInMem_->Append(
            SvcApp::Log::LogItem(
                LOG_ENT_APPLICATION, Message,
                SvcApp::Log::LogParams( std::forward<A...>( Args )... ),
                Now(), Severity
            )
        );
    }
protected:
    virtual void __fastcall WndProc( Winapi::Messages::TMessage &Message ) override;
public:		// User declarations
    __fastcall TfrmMain( TComponent* Owner ) override;
    __fastcall TfrmMain( TComponent* Owner, StoreOpts StoreOptions,
                         Anafestica::TConfigNode* const RootNode = nullptr );
    __fastcall ~TfrmMain();

    __property String SourceFolder = {
        read = GetSourceFolder, write = SetSourceFolder
    };
    __property TStrings& SourceFolderMRUList = { read = GetSourceFolderMRUList };
    __property bool LogUpdateActive = {
        read = GetLogUpdateActive, write = SetLogUpdateActive
    };
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
