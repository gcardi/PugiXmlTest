//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>

#include <JPeg.hpp>

#include "FormInfo.h"

using std::auto_ptr;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmInfo *frmInfo;
//---------------------------------------------------------------------------

__fastcall TfrmInfo::TfrmInfo(TComponent* Owner)
    : TForm(Owner), allowClose_( false ), autoCloseTime_( 0 )
{
    DoubleBuffered = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::ImageClick(TObject */*Sender*/)
{
    if ( allowClose_ && ( !Timer->Enabled || FormState.Contains( fsModal ) ) ) {
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::SetAutoCloseTime( int Val )
{
    if ( Val != autoCloseTime_ ) {
        if ( Showing && Val ) {
            Timer->Enabled = false;
            Timer->Interval = Val;
            Timer->Enabled = true;
        }
        autoCloseTime_ = Val;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::FormShow(TObject */*Sender*/)
{
    if ( autoCloseTime_ ) {
        Timer->Enabled = false;
        Timer->Interval = autoCloseTime_;
        Timer->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::FormKeyPress(TObject */*Sender*/, char &Key)
{
    if ( Key == VK_ESCAPE ) {
        Click();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::TimerTimer(TObject */*Sender*/)
{
    Close();
}
//---------------------------------------------------------------------------

String __fastcall TfrmInfo::GetText( void )
{
    return Label->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::SetText( String Val )
{
    Label->Caption = Val;
    Label->Refresh();
}
//---------------------------------------------------------------------------

TTextLayout __fastcall TfrmInfo::GetTextLayout( void )
{
    return Label->Layout;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::SetTextLayout( TTextLayout Val )
{
    Label->Layout = Val;
    Label->Refresh();
}
//---------------------------------------------------------------------------

TAlignment __fastcall TfrmInfo::GetTextAlignment( void )
{
    return Label->Alignment;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::SetTextAlignment( TAlignment Val )
{
    Label->Alignment = Val;
    Label->Refresh();
}
//---------------------------------------------------------------------------

String __fastcall TfrmInfo::GetVersionText( void )
{
    return LabelVersion->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInfo::SetVersionText( String Val )
{
    LabelVersion->Caption = Val;
}
//---------------------------------------------------------------------------


