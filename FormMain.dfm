object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'frmMain'
  ClientHeight = 469
  ClientWidth = 737
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  DesignSize = (
    737
    469)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 32
    Width = 42
    Height = 13
    Caption = 'Percorso'
  end
  object BitBtn1: TBitBtn
    Left = 688
    Top = 46
    Width = 25
    Height = 25
    Action = actFileSelectFolder
    Anchors = [akTop, akRight]
    Caption = '...'
    TabOrder = 0
  end
  object comboboxSourceFolder: TComboBox
    Left = 24
    Top = 48
    Width = 653
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object pnlLog: TPanel
    Left = 0
    Top = 152
    Width = 737
    Height = 298
    Align = alBottom
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvNone
    Caption = 'pnlLog'
    ShowCaption = False
    TabOrder = 2
    object pnlLogViewer: TPanel
      Left = 0
      Top = 19
      Width = 737
      Height = 279
      Align = alClient
      BevelOuter = bvNone
      Caption = 'pnlLog'
      ShowCaption = False
      TabOrder = 0
      ExplicitHeight = 362
      object ndrawgridLog: TNDrawGrid
        Left = 0
        Top = 0
        Width = 737
        Height = 279
        Align = alClient
        ColCount = 6
        Color = clWindow
        DefaultRowHeight = 18
        FixedColor = clBtnFace
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goThumbTracking]
        RowCount = 2
        TabOrder = 0
        OnFixedCellGetText = ndrawgridLogFixedCellGetText
        OnNormalCellGetText = ndrawgridLogNormalCellGetText
        CellAutoHintEnabled = True
        OnDrawCell = ndrawgridLogDrawCell
        ExplicitHeight = 362
        ColWidths = (
          20
          142
          40
          40
          42
          1024)
        RowHeights = (
          18
          18)
      end
    end
    object pnlLogCtrls: TPanel
      Left = 0
      Top = 0
      Width = 737
      Height = 19
      Align = alTop
      Caption = 'Log'
      TabOrder = 1
      object SpeedButton1: TSpeedButton
        Left = 92
        Top = 0
        Width = 41
        Height = 16
        Cursor = crHandPoint
        Action = actLogClear
        Flat = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        StyleElements = []
      end
      object CheckBox1: TCheckBox
        Left = 8
        Top = -1
        Width = 65
        Height = 17
        Action = actLogUpdateActive
        State = cbChecked
        TabOrder = 0
      end
    end
  end
  object ActionMainMenuBar1: TActionMainMenuBar
    Left = 0
    Top = 0
    Width = 737
    Height = 25
    UseSystemFont = False
    ActionManager = ActionManager1
    Caption = 'ActionMainMenuBar1'
    Color = clMenuBar
    ColorMap.DisabledFontColor = 7171437
    ColorMap.HighlightColor = clWhite
    ColorMap.BtnSelectedFont = clBlack
    ColorMap.UnusedColor = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Spacing = 0
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 450
    Width = 737
    Height = 19
    Panels = <
      item
        Width = 100
      end
      item
        Width = 50
      end>
    ExplicitLeft = -32
    ExplicitTop = 461
  end
  object Button1: TButton
    Left = 24
    Top = 88
    Width = 75
    Height = 25
    Action = actProcessFolderStart
    TabOrder = 5
  end
  object ActionManager1: TActionManager
    ActionBars = <
      item
        Items = <
          item
            Items = <
              item
                Action = actFileSelectFolder
                Caption = '&Seleziona cartella...'
              end
              item
                Action = actFileExit
                ImageIndex = 43
              end>
            Caption = '&File'
          end
          item
            Items = <
              item
                Action = actProcessFolderStart
                Caption = '&Avvia'
              end>
            Caption = '&Process'
          end
          item
            Items = <
              item
                Action = actLogUpdateActive
                Caption = '&Update'
              end
              item
                Action = actLogClear
                Caption = '&Clear'
              end>
            Caption = '&Log'
          end>
        ActionBar = ActionMainMenuBar1
      end>
    Left = 584
    Top = 72
    StyleName = 'Platform Default'
    object actFileSelectFolder: TAction
      Category = 'File'
      Caption = 'Seleziona cartella...'
      Hint = 'Seleziona la cartella con i file da analizzare'
      OnExecute = actFileSelectFolderExecute
      OnUpdate = actFileSelectFolderUpdate
    end
    object actProcessFolderStart: TAction
      Category = 'Process'
      Caption = 'Avvia'
      OnExecute = actProcessFolderStartExecute
      OnUpdate = actProcessFolderStartUpdate
    end
    object actLogUpdateActive: TAction
      Category = 'Log'
      AutoCheck = True
      Caption = 'Update'
      Checked = True
      Hint = 'Update log active|Enable/Disable log update'
      OnExecute = actLogUpdateActiveExecute
      OnUpdate = actLogUpdateActiveUpdate
    end
    object actLogClear: TAction
      Category = 'Log'
      Caption = 'Clear'
      OnExecute = actLogClearExecute
      OnUpdate = actLogClearUpdate
    end
    object actFileExit: TFileExit
      Category = 'File'
      Caption = 'E&xit'
      Hint = 'Esce|Chiude l'#39'applicazione'
      ImageIndex = 43
    end
  end
  object fileopndlgSelectFolder: TFileOpenDialog
    ClientGuid = '{41596BB0-95E9-4786-897E-3A02C23B9692}'
    FavoriteLinks = <>
    FileTypes = <>
    OkButtonLabel = 'Seleziona'
    Options = [fdoPickFolders, fdoPathMustExist]
    Title = 'Seleziona cartella da processare'
    Left = 584
    Top = 136
  end
  object imglistLog: TImageList
    ColorDepth = cd32Bit
    DrawingStyle = dsTransparent
    Height = 18
    Width = 18
    Left = 584
    Top = 200
    Bitmap = {
      494C010104000900040012001200FFFFFFFF2110FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000480000002400000001002000000000008028
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000060606072F2F2F3D3F3F3F5F40404061212121280000
      0000000000000000000000000000000000000000000000000000000000002A2A
      2A343E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E
      3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C3E3E3E5C2A2A2A340000
      0000000000000000000000000000000000000000000000000000060606072F2F
      2F3D3F3F3F5F4040406121212128000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000050505099565656F2A4A4A4F1CBCBCBFDC9C9C9FD929292F44E4E
      4EFF4E4E4E900000000000000000000000000000000000000000000000002929
      29334E6060F652B6B6FD49B6B6FD3FB6B6FD36B6B6FD2DB6B6FD25B6B6FD23B6
      B6FD25B6B6FD2DB6B6FD36B6B6FD3FB6B6FD49B6B6FD4D6060F6292929330000
      000000000000000000000000000000000000000000004D4D4F964D4D56F27373
      A3EF8989CDFB8686CEFB6A6A98F24E4E4FFF4B4B4E8E00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00005B5B5BD2D2D2D2F7FFFFFFFFFDFDFDFFF4F4F4FFEBEBEBFFECECECFFFCFC
      FCFFAAAAAAF6656565B800000000000000000000000000000000000000000000
      0000505757BC54DCDCFF46FFFFFF36FFFFFF26FFFFFF16FFFFFF016B6BFF006A
      6AFF06FFFFFF16FFFFFF26FFFFFF36FFFFFF47DCDCFF505757BC000000000000
      000000000000000000000000000000000000505059D07676D2F7F9F9FFFFA4A4
      FFFF8686FFFF7D7DFFFF9B9BFFFFEDEDFFFF2626C4F5555567B7000000000000
      0000000000000000000000000000000000002C2C2C383C3C3C55000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00003B3B3B542C2C2C3800000000000000000000000000000000000000006868
      68B8E1E1E1FFFFFFFFFFFFFFFFFFF9F9F9FFF48F8FFFEC8989FFD2D2D2FFCCCC
      CCFFFFFFFFFFCFCFCFFF595959D1000000000000000000000000000000000000
      00001F1F1F254F6B6BE946FFFFFF36FFFFFF26FFFFFF16FFFFFF014B4BFF004A
      4AFF0AFFFFFF16FFFFFF26FFFFFF36FFFFFF4C6B6BE91F1F1F25000000000000
      00000000000000000000000000005D5D67B69292E1FFBEBEFFFF5151FFFF4444
      FFFF5353FFFF4B4BFFFF2424FFFF1919FFFFFFFFFFFF5A5AE0FF4A4A59CF0000
      00000000000000000000000000000000000000000000535353A4555555BA0000
      000000000000414143672F2F72F12F2F72F14141436700000000000000005555
      55BA535353A400000000000000000000000000000000000000004F4F4F91C5C5
      C5F7FFFFFFFFFFFFFFFFFFFFFFFFF9F9F9FFFE0C0CFFFD1111FFD5D5D5FFD2D2
      D2FFD5D5D5FFF3F3F3FFC5C5C5F7505050980000000000000000000000000000
      0000000000004E5454A74BCBCBFD3AFFFFFF2EFFFFFF22FFFFFF10C3C3FF10C5
      C5FF16FFFFFF22FFFFFF2EFFFFFF3FCBCBFD4E5454A700000000000000000000
      000000000000000000004C4C4E8E6A6AC4F5FFFFFFFF5D5DFFFF5050FFFF4949
      FFFFF8F8FFFFF8F8FFFF2A2AFFFF1919FFFF0E0EFFFF9B9BFFFF4242D1F74C4C
      4F96000000000000000000000000000000000000000000000000535353C20A0A
      0A0B4A4A58A2000046FF00006FFF00006FFF000046FF4A4A58A20A0A0A0B5353
      53C20000000000000000000000000000000000000000000000004F4F4FFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCFFFF0D0DFFFE1212FFDFDFDFFFDFDF
      DFFFDFDFDFFFEAEAEAFFFFFFFFFF555555F30505050600000000000000000000
      000000000000151515184E6363E846FFFFFF36FFFFFF2EFFFFFF094848FF0A4D
      4DFF26FFFFFF2EFFFFFF36FFFFFF4D6363E81515151800000000000000000000
      000000000000000000004E4E4FFFF4F4FFFF6969FFFF5D5DFFFF5050FFFF4747
      FFFF9C9CFFFF9797FFFF2727FFFF1F1FFFFF1616FFFF0F0FFFFFF4F4FFFF4949
      56F20505050600000000000000000000000000000000000000002D2D2D3A4B4B
      4BD10303B1FF0000B9FF0000C8FF0000C8FF0000B9FF0303B1FF4B4B4BD12D2D
      2D3A000000000000000000000000000000000000000021212128999999F3FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0D0DFFFE1313FFECECECFFECEC
      ECFFECECECFFF3F3F3FFFDFDFDFFA3A3A3F02E2E2E3C00000000000000000000
      000000000000000000004C4F4F904EB9B9FA46FFFFFF3AFFFFFF0C3F3FFF0D43
      43FF36FFFFFF3AFFFFFF4AB9B9FA4C4F4F900000000000000000000000000000
      000000000000212121287F7F98F2C2C2FFFF6C6CFFFF6060FFFF5757FFFF4A4A
      FFFF8C8CFFFF8585FFFF2F2FFFFF2424FFFF1F1FFFFF1919FFFF9090FFFF6767
      A1EF2E2E2E3C0000000000000000000000000000000000000000000000004747
      5EC10000D4FF00003AFF000059FF000059FF00003AFF0000D4FF47475EC10000
      000000000000000000000000000000000000000000003E3E3E5CCFCFCFFDFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0E0EFFFF1313FFF9F9F9FFF9F9
      F9FFF9F9F9FFFCFCFCFFFFFFFFFFCFCFCFFD3E3E3E5D00000000000000000000
      000000000000000000000D0D0D0E4F5E5EE652FCFCFF4AFFFFFF0D3535FF0E39
      39FF46FFFFFF4AFCFCFF4E5E5EE60D0D0D0E0000000000000000000000000000
      0000000000003E3E3E5CA3A3CCFBB0B0FFFF7373FFFF6666FFFF5A5AFFFF5050
      FFFFDADAFFFFD5D5FFFF3434FFFF2F2FFFFF2727FFFF2424FFFF7878FFFF7C7C
      CCFB3E3E3E5D0000000000000000000000004949497700000000000000004141
      69CC00008DFF0000A0FF0000C1FF0000C1FF0000A0FF00008DFF414169CC0000
      0000000000004949497700000000000000000000000040404061D1D1D1FDFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0E0EFFFF1414FFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFD1D1D1FD3F3F3F6000000000000000000000
      00000000000000000000000000004749497956A8A8F65AFFFFFF0D2C2CFF0E2F
      2FFF56FFFFFF53A9A9F647494979000000000000000000000000000000000000
      00000000000040404061A7A7CEFBB3B3FFFF7676FFFF6C6CFFFF6060FFFF5757
      FFFFE2E2FFFFDEDEFFFF3C3CFFFF3636FFFF3131FFFF2F2FFFFF7D7DFFFF8383
      CDFA3F3F3F600000000000000000000000002D2D2D3A535353BD010101024747
      4F84000053FF000099FF000185FF000185FF000099FF000053FF47474F840101
      0102535353BD2D2D2D3A0000000000000000000000002A2A2A349D9D9DF2FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7373FFFF7676FFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF9B9B9BF02B2B2B3700000000000000000000
      000000000000000000000000000006060607515B5BE268F7F7FF1F4F4FFF1F51
      51FF65F7F7FF505B5BE206060607000000000000000000000000000000000000
      0000000000002A2A2A3483839BF0CBCBFFFF7C7CFFFF7373FFFF6969FFFF5D5D
      FFFFEAEAFFFFE7E7FFFF4444FFFF3F3FFFFF3C3CFFFF3939FFFFAFAFFFFF6666
      99EE2B2B2B3700000000000000000000000000000000222222294D4D4DDD4949
      49E80A0A90FF000696FF0F2A49FF0E2948FF000693FF0A0A90FF494949E84D4D
      4DDD22222229000000000000000000000000000000000909090A595959EEFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB0B0FFFFB0B0FFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF545454F50303030400000000000000000000
      000000000000000000000000000000000000404141625D9797F276FFFFFF76FF
      FFFF5D9797F24041416200000000000000000000000000000000000000000000
      0000000000000909090A525258EDF2F2FFFF8888FFFF7979FFFF7070FFFF6666
      FFFFF2F2FFFFEFEFFFFF5050FFFF4A4AFFFF4747FFFF5252FFFFD7D7FFFF4B4B
      54F5030303040000000000000000000000000000000000000000000000000000
      000042494EF8010608FF818181FF818181FF010608FF434B50F7000000000000
      00000000000000000000000000000000000000000000000000004F4F4F93CDCD
      CDF8FFFFFFFFFFFFFFFFFFFFFFFFFFF9F9FFFF0202FFFF0101FFFFF9F9FFFFFF
      FFFFFFFFFFFFFFFFFFFFB6B6B6F8474747710000000000000000000000000000
      00000000000000000000000000000000000001010102545A5ADB99F0F0FF99F0
      F0FF545A5ADB0101010200000000000000000000000000000000000000000000
      000000000000000000004D4D4E918D8DCBF7DDDDFFFF8484FFFF7979FFFF7070
      FFFFF8F8FFFFF6F6FFFF5A5AFFFF5757FFFF5050FFFFFFFFFFFF5959B6F84444
      466E000000000000000000000000000000000000000000000000000000000000
      0000505050AE49808FFA2E3031FE2E3031FF4A818FF8505051AB000000000000
      0000000000000000000000000000000000000000000000000000000000005B5B
      5BCDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCBCFFFFBBBBFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF5E5E5EFC000000010000000000000000000000000000
      000000000000000000000000000000000000000000003737374B7B8686EE7B86
      86EE3737374B0000000000000000000000000000000000000000000000000000
      000000000000000000000000000053535ACCAAAAFFFFFFFFFFFF8D8DFFFF7979
      FFFF9999FFFF9393FFFF6666FFFF6565FFFFD0D0FFFF7C7CFFFF5C5C5DFC0000
      0001000000000000000000000000000000000000000000000000000000004141
      41622A2A2A342323232A4848487748484877212121272A2A2A34414141620000
      0000000000000000000000000000000000000000000000000000000000000000
      00005E5E5EFCB6B6B6F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFCDCDCDF85B5B5BCE00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000595959CF5959
      59CF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000005C5C5DFC7B7BB6F8E7E7FFFFCDCD
      FFFFB2B2FFFFACACFFFFC1C1FFFFEDEDFFFF7272CDF851515ACC000000000000
      0000000000000000000000000000000000000000000000000000000000002D2D
      2D3A0000000000000000000000000000000000000000000000002D2D2D3A0000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000147474770535353F69B9B9BF0CDCDCDFDD1D1D1FD9D9D9DF15959
      59EE4F4F4F910000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000002B2B2B372B2B
      2B37000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000014545466E505053F47D7D
      9AEFA0A0CAFBA1A1CDFB7D7D9CF14F4F59EE4D4D4F9200000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000030303042B2B2B373D3D3D5A3F3F3F60292929330909
      090A000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000040404052B2B
      2B373D3D3D5B3F3F3F60292929330909090A0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000048000000240000000100010000000000B00100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FFFFFFFFFFFFFFFFFF000000FC1FE0001FC1FFFFFF000000
      F807E0001F807FFFFF000000F003F0003F003F3FF3000000E001F0003E001F98
      67000000C000F8007C000FC00F000000C00078007C0007C00F00000080007C00
      F80007E01F00000080007C00F80007601B00000080007E01F800070003000000
      80007E01F80007800700000080007F03F80007F03F000000C000FF03FC000FF0
      3F000000E000FF87FE000FE01F000000F003FFCFFF003FEFDF000000F007FFCF
      FF007FFFFF000000FC0FFFFFFFC0FFFFFF000000FFFFFFFFFFFFFFFFFF000000
      00000000000000000000000000000000000000000000}
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 20
    OnTimer = Timer1Timer
    Left = 600
    Top = 275
  end
  object Timer2: TTimer
    Interval = 100
    OnTimer = Timer2Timer
    Left = 608
    Top = 339
  end
end