//---------------------------------------------------------------------------

#ifndef FormInfoH
#define FormInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmInfo : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlMain;
    TImage *Image;
    TTimer *Timer;
    TLabel *Label;
    TLabel *LabelVersion;
    void __fastcall ImageClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall TimerTimer(TObject *Sender);
private:	// User declarations
    bool allowClose_;
    int autoCloseTime_;

    void __fastcall SetAutoCloseTime( int Val );
    String __fastcall GetText( void );
    void __fastcall SetText( String Val );
    TTextLayout __fastcall GetTextLayout( void );
    void __fastcall SetTextLayout( TTextLayout Val );
    TAlignment __fastcall GetTextAlignment( void );
    void __fastcall SetTextAlignment( TAlignment Val );
    String __fastcall GetVersionText( void );
    void __fastcall SetVersionText( String Val );
public:		// User declarations
    __fastcall TfrmInfo(TComponent* Owner);
    __property bool AllowClose = { read = allowClose_, write = allowClose_ };
    __property int AutoCloseTime = { read = autoCloseTime_, write = SetAutoCloseTime };
    __property String Text = { read = GetText, write = SetText };
    __property TTextLayout TextLayout = { read = GetTextLayout, write = SetTextLayout };
    __property TAlignment TextAlignment = { read = GetTextAlignment, write = SetTextAlignment };
    __property String VersionText = { read = GetVersionText, write = SetVersionText };
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmInfo *frmInfo;
//---------------------------------------------------------------------------
#endif
