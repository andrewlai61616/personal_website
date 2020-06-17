VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frm 
   BorderStyle     =   1  '單線固定
   Caption         =   "解數獨器"
   ClientHeight    =   2670
   ClientLeft      =   7800
   ClientTop       =   4800
   ClientWidth     =   3570
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2670
   ScaleWidth      =   3570
   Begin VB.Timer SBT 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1680
      Top             =   1560
   End
   Begin VB.Timer Testt 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1320
      Top             =   1080
   End
   Begin VB.Timer TT 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1920
      Top             =   1080
   End
   Begin VB.CommandButton Loadb 
      Caption         =   "打開文件"
      Height          =   615
      Left            =   2520
      TabIndex        =   11
      Top             =   1080
      Width           =   975
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   1560
      Top             =   360
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   8
      Left            =   0
      MaxLength       =   9
      TabIndex        =   9
      ToolTipText     =   "第九欄位"
      Top             =   1920
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   7
      Left            =   0
      MaxLength       =   9
      TabIndex        =   8
      ToolTipText     =   "第八欄位"
      Top             =   1680
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   6
      Left            =   0
      MaxLength       =   9
      TabIndex        =   7
      ToolTipText     =   "第七欄位"
      Top             =   1440
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   5
      Left            =   0
      MaxLength       =   9
      TabIndex        =   6
      ToolTipText     =   "第六欄位"
      Top             =   1200
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   4
      Left            =   0
      MaxLength       =   9
      TabIndex        =   5
      ToolTipText     =   "第五欄位"
      Top             =   960
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   3
      Left            =   0
      MaxLength       =   9
      TabIndex        =   4
      ToolTipText     =   "第四欄位"
      Top             =   720
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   2
      Left            =   0
      MaxLength       =   9
      TabIndex        =   3
      ToolTipText     =   "第三欄位"
      Top             =   480
      Width           =   1095
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   1
      Left            =   0
      MaxLength       =   9
      TabIndex        =   2
      ToolTipText     =   "第二欄位"
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton SB 
      Caption         =   "Start"
      Height          =   495
      Left            =   2520
      TabIndex        =   10
      Top             =   480
      Width           =   975
   End
   Begin VB.TextBox T 
      Height          =   270
      Index           =   0
      Left            =   0
      MaxLength       =   9
      TabIndex        =   1
      ToolTipText     =   "第一欄位"
      Top             =   0
      Width           =   1095
   End
   Begin VB.TextBox label2 
      Height          =   1695
      Left            =   1200
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   14
      Text            =   "frm.frx":0000
      Top             =   360
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "若想看示範，在第一欄位輸入 ""test"" + 1~9"
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   2400
      Width           =   3375
   End
   Begin VB.Label RC 
      Caption         =   "重複計算 : 0次了"
      Height          =   255
      Left            =   1200
      TabIndex        =   13
      Top             =   2160
      Visible         =   0   'False
      Width           =   1575
   End
   Begin VB.Label lagl 
      Caption         =   "狀態 : 預備狀態請寫入字串"
      Height          =   255
      Left            =   1200
      TabIndex        =   12
      Top             =   120
      Width           =   2175
   End
   Begin VB.Label Label1 
      Caption         =   "(0代表空位)"
      Height          =   255
      Left            =   2520
      TabIndex        =   0
      Top             =   1800
      Width           =   975
   End
End
Attribute VB_Name = "frm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim A(1 To 9, 1 To 9) As String, C(1 To 9, 1 To 9, 1 To 9) As Integer, Started As Integer
Dim D(1 To 9, 1 To 9, 1 To 9) As Integer, Done As Integer, Testing As Integer, Repeat As Integer
Private Sub ErF()
MsgBox "輸入格式錯誤!!!", vbExclamation, "錯誤格式!"
End Sub

Private Sub Form_Load()
Started = 0
Repeat = 0
End Sub
Function ReadTextFileContents(filename As String) As String
    Dim fnum As Integer, isOpen As Boolean
    On Error GoTo Error_Handler
    fnum = FreeFile()
    Open filename For Input As #fnum
    isOpen = True
    ReadTextFileContents = Input(LOF(fnum), fnum)
Error_Handler:
    If isOpen Then Close #fnum
    If Err Then Err.Raise Err.Number, , Err.Description
End Function

Private Sub Loadb_Click()
If Loadb.Caption = "打開文件" Then
    On Error Resume Next
    CommonDialog1.DialogTitle = "打開文件"
    CommonDialog1.Filter = "所有支持的格式" + "(*.txt)|" + "*.txt"
    CommonDialog1.ShowOpen
inputstring = ReadTextFileContents(CommonDialog1.filename)
If Len(inputstring) <> 81 Then Exit Sub
For a1 = 1 To 9
T(a1 - 1) = ""
For a2 = 1 To 9
T(a1 - 1).Text = T(a1 - 1).Text & Mid(inputstring, (a1 - 1) * 9 + a2, 1)
Next
Next
Loadb.Caption = "重新"
Else
Started = 0
For a1 = 0 To 8
T(a1).Text = ""
Next
Loadb.Caption = "打開文件"
Repeat = 0
End If
End Sub
Private Sub SB_Click()
If Started = 0 Then
RC.Visible = True
Started = 1
For a0 = 0 To 8
If Len(T(a0).Text) <> 9 Then
ErF
T(a0).Text = ""
Exit Sub
Else
If IsNumeric(T(a0).Text) = False Then
ErF
T(a0).Text = ""
Exit Sub
Else
If Fix(T(a0).Text) <> T(a0).Text Then
ErF
T(a0).Text = ""
Exit Sub
End If
End If
End If
Next
For a0 = 0 To 8
For a1 = 1 To 9
A(a1, a0 + 1) = Mid(T(a0).Text, a1, 1)
Next
Next
For a0 = 1 To 9
For a3 = 1 To 9
a2 = 0
a4 = 0
a5 = 0
For a1 = 1 To 9
If A(3 * ((a0 - 1) Mod 3) + ((a1 - 1) Mod 3) + 1, ((a0 - 1) \ 3) * 3 + ((a1 - 1) \ 3) + 1) = a3 Then a5 = a5 + 1
If A(a0, a1) = a3 Then a2 = a2 + 1
If A(a1, a0) = a3 Then a4 = a4 + 1
Next
If a2 > 1 Or a4 > 1 Or a5 > 1 Then
ErF
Exit Sub
End If
Next
Next
For a0 = 1 To 9
For a1 = 1 To 9
If A(a0, a1) = 0 Then
For a2 = 1 To 9
C(a0, a1, a2) = 1
Next
End If
Next
Next
Testing = 0
If Testing = 0 Then
lagl.Caption = "狀態 : Lag (計算) 中...ㄏㄏ"
TT.Enabled = True
End If
Else
TT.Enabled = True
End If
End Sub
Private Sub FT()
m = ""
For a2 = 1 To 9
For a1 = 1 To 9
n = ""
For a3 = 1 To 9
If C(a1, a2, a3) = 1 Then n = n & a3
Next
m = m & n & ","  ''''
Next
m = m & vbCrLf
Next
'L.Caption = m

m = ""
For a1 = 1 To 9
For a0 = 1 To 9
m = m & A(a0, a1)
Next
m = m & vbCrLf
Next
label2.Text = m

End Sub
Private Sub EnterRealCountingSection()
Done = 0
abcde = 0
Do Until abcde = 1
For a1 = 1 To 9
For a2 = 1 To 9
For a3 = 1 To 9
D(a1, a2, a3) = C(a1, a2, a3)
Next
Next
Next
For a1 = 1 To 9
For a2 = 1 To 9
If A(a1, a2) = 0 Then
MakeChart a1, a2
End If
Next
Next
Check1
Check2
Check3
FT
abcde = 1
Done = 1
For a1 = 1 To 9
For a2 = 1 To 9
If A(a1, a2) = 0 Then
abcde = 0
Done = 0
End If
Next
Next
If abcde = 0 Then
abcde = 1
For a1 = 1 To 9
For a2 = 1 To 9
For a3 = 1 To 9
If D(a1, a2, a3) <> C(a1, a2, a3) Then abcde = 0
Next
Next
Next
If abcde = 1 Then Testing = Testing + 1
End If
Loop
If Done = 1 Then lagl.Caption = "狀態 : 完成!!!ㄎㄎ帥吧"
If Testing = 2 Then SBT.Enabled = True
If Testing = 1 And Done = 0 Then Testt.Enabled = True
End Sub

Private Sub SBT_Timer()
Started = 0
SB_Click
SBT.Enabled = False
End Sub

Private Sub Testt_Timer()
Repeat = Repeat + 1
RC.Caption = "重複計算 : " & Repeat & " 次了"
a1 = Fix(Rnd() * 9 + 1)
a2 = Fix(Rnd() * 9 + 1)
Do Until A(a1, a2) = 0
a1 = Fix(Rnd() * 9 + 1)
a2 = Fix(Rnd() * 9 + 1)
Loop
Randomize
b = Fix(Rnd() * 9 + 1)
Do Until C(a1, a2, b) = 1
b = Fix(Rnd() * 9 + 1)
Loop
A(a1, a2) = b
TT.Enabled = True
Testt.Enabled = False
End Sub
Private Sub Check3()
For a0 = 1 To 9
For a1 = 1 To 9
If A(a1, a0) <> 0 Then
For a2 = 1 To 9
C(a1, a0, a2) = 0
Next
End If
Next
Next
End Sub
Private Sub Check2()
For a0 = 1 To 9
c1 = 3 * ((a0 - 1) Mod 3)
c2 = ((a0 - 1) \ 3) * 3
For a3 = 1 To 9

needcheck = 1
For a1 = 1 To 9
If A(c1 + ((a1 - 1) Mod 3) + 1, c2 + ((a1 - 1) \ 3) + 1) = a3 Then needcheck = 0
Next

If needcheck = 1 Then

Sum = 0
For a1 = 1 To 9
If C(c1 + ((a1 - 1) Mod 3) + 1, c2 + ((a1 - 1) \ 3) + 1, a3) = 1 Then Sum = Sum + 1
Next

Select Case Sum
Case 2, 3

For a2 = 1 To 7 Step 3
s1 = 0
For a1 = a2 To a2 + 2
If C(c1 + ((a1 - 1) Mod 3) + 1, c2 + ((a1 - 1) \ 3) + 1, a3) = 1 Then s1 = s1 + 1
Next
If s1 = Sum Then
s2 = c2 + ((a2 - 1) \ 3) + 1
For s3 = 1 To 9
If s3 <= c1 Or s3 > c1 + 3 Then C(s3, s2, a3) = 0
Next
End If
Next

End Select

End If

Next
Next
End Sub
Private Sub Check1()
For a0 = 1 To 9

For a3 = 1 To 9

needcheck = 1
For a1 = 1 To 9
If A(3 * ((a0 - 1) Mod 3) + ((a1 - 1) Mod 3) + 1, ((a0 - 1) \ 3) * 3 + ((a1 - 1) \ 3) + 1) = a3 Then needcheck = 0
Next

If needcheck = 1 Then
a5 = 0
For a1 = 1 To 9
If C(3 * ((a0 - 1) Mod 3) + ((a1 - 1) Mod 3) + 1, ((a0 - 1) \ 3) * 3 + ((a1 - 1) \ 3) + 1, a3) = 1 Then
a5 = a5 + 1
a2 = CStr(3 * ((a0 - 1) Mod 3) + ((a1 - 1) Mod 3) + 1) & CStr(((a0 - 1) \ 3) * 3 + ((a1 - 1) \ 3) + 1)
End If
Next

If a5 = 1 Then
A(Left(a2, 1), Right(a2, 1)) = a3
For ab0 = 1 To 9
C(Left(a2, 1), Right(a2, 1), ab0) = 0
Next
End If


End If

Next

Next
End Sub

Private Sub MakeChart(b1, b2)
For ab1 = 1 To 9
If C(b1, b2, ab1) = 1 Then

A(b1, b2) = ab1
For a0 = 1 To 9
For a3 = 1 To 9
a2 = 0
a4 = 0
a5 = 0
For a1 = 1 To 9
If A(3 * ((a0 - 1) Mod 3) + ((a1 - 1) Mod 3) + 1, ((a0 - 1) \ 3) * 3 + ((a1 - 1) \ 3) + 1) = a3 Then a5 = a5 + 1
If A(a0, a1) = a3 Then a2 = a2 + 1
If A(a1, a0) = a3 Then a4 = a4 + 1
Next
If a2 > 1 Or a4 > 1 Or a5 > 1 Then
C(b1, b2, ab1) = 0
A(b1, b2) = 0
Exit For
End If
Next
If a2 > 1 Or a4 > 1 Or a5 > 1 Then Exit For
Next

If A(b1, b2) <> 0 Then
a1 = 0
For a0 = 1 To 9
If a0 = ab1 Then
If a0 = 9 Then
Exit For
Else
a0 = a0 + 1
End If
End If
If C(b1, b2, a0) >= 1 Then a1 = 1
Next
If a1 = 1 Then
A(b1, b2) = 0
Else
For a0 = 1 To 9
C(b1, b2, a0) = 0
Next
End If
End If

If A(b1, b2) <> 0 Then Exit For
End If
Next
End Sub
Private Sub T_Change(Index As Integer)
Started = 0
Repeat = 0
Select Case T(0).Text
Case "test1"
T(0).Text = "007002000"
T(1).Text = "580007000"
T(2).Text = "040800607"
T(3).Text = "692000000"
T(4).Text = "000030000"
T(5).Text = "000000291"
T(6).Text = "703004050"
T(7).Text = "000700089"
T(8).Text = "000600400"
Case "test2"
T(0).Text = "361004000"
T(1).Text = "000300006"
T(2).Text = "070000009"
T(3).Text = "002010050"
T(4).Text = "009000600"
T(5).Text = "050020800"
T(6).Text = "600000010"
T(7).Text = "800007000"
T(8).Text = "000900564"
Case "test3"
T(0).Text = "200080006"
T(1).Text = "050000040"
T(2).Text = "004609300"
T(3).Text = "900706003"
T(4).Text = "000205000"
T(5).Text = "060000010"
T(6).Text = "500908004"
T(7).Text = "008030600"
T(8).Text = "370000082"
Case "test4"
T(0).Text = "108000600"
T(1).Text = "050200004"
T(2).Text = "007305000"
T(3).Text = "009800406"
T(4).Text = "080030090"
T(5).Text = "502009700"
T(6).Text = "000403800"
T(7).Text = "900008060"
T(8).Text = "001000305"
Case "test5"
T(0).Text = "000000006"
T(1).Text = "080060430"
T(2).Text = "006809050"
T(3).Text = "007010900"
T(4).Text = "090507010"
T(5).Text = "001090500"
T(6).Text = "070406300"
T(7).Text = "019030060"
T(8).Text = "200000000"
Case "test6"
T(0).Text = "004000700"
T(1).Text = "080105020"
T(2).Text = "100090003"
T(3).Text = "030609040"
T(4).Text = "900000007"
T(5).Text = "070201060"
T(6).Text = "700040008"
T(7).Text = "040507010"
T(8).Text = "000900200"
Case "test7"
T(0).Text = "470000065"
T(1).Text = "100436007"
T(2).Text = "000000000"
T(3).Text = "090010020"
T(4).Text = "010302050"
T(5).Text = "050090070"
T(6).Text = "000000000"
T(7).Text = "300275001"
T(8).Text = "760000098"
Case "test8"
T(0).Text = "902000506"
T(1).Text = "040501030"
T(2).Text = "010906020"
T(3).Text = "007080600"
T(4).Text = "001000900"
T(5).Text = "000407000"
T(6).Text = "000108000"
T(7).Text = "080040090"
T(8).Text = "005090200"
Case "test9"
T(0).Text = "700102005"
T(1).Text = "010090040"
T(2).Text = "008000700"
T(3).Text = "006708400"
T(4).Text = "070000050"
T(5).Text = "001209300"
T(6).Text = "009000200"
T(7).Text = "060070030"
T(8).Text = "500903004"
End Select
End Sub

Private Sub TT_Timer()
EnterRealCountingSection
TT.Enabled = False
End Sub
