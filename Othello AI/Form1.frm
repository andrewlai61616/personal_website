VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Othello"
   ClientHeight    =   5220
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   5400
   FillStyle       =   0  '實心
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5220
   ScaleWidth      =   5400
   StartUpPosition =   3  '系統預設值
   Begin VB.Timer FRes 
      Interval        =   1
      Left            =   3480
      Top             =   2760
   End
   Begin VB.Timer ct 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   960
      Top             =   2400
   End
   Begin VB.CommandButton RB 
      Caption         =   "Random Position"
      Height          =   375
      Left            =   0
      TabIndex        =   4
      Top             =   0
      Visible         =   0   'False
      Width           =   1455
   End
   Begin VB.Timer LastT 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   1800
      Top             =   1680
   End
   Begin VB.OptionButton HardO 
      Caption         =   "Hard"
      Height          =   255
      Left            =   4680
      TabIndex        =   3
      Top             =   240
      Width           =   735
   End
   Begin VB.OptionButton EasyO 
      Caption         =   "Easy"
      Height          =   255
      Left            =   3960
      TabIndex        =   2
      Top             =   240
      Value           =   -1  'True
      Width           =   735
   End
   Begin VB.Timer PCT 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   4080
      Top             =   1080
   End
   Begin VB.CommandButton PCFirst 
      Caption         =   "PC First"
      Height          =   615
      Left            =   2040
      TabIndex        =   1
      Top             =   120
      Width           =   1815
   End
   Begin VB.CommandButton HumanFirst 
      Caption         =   "You First"
      Height          =   615
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1815
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim Moves As Integer, B(1 To 8, 1 To 8) As Integer, C1 As Integer, C2 As Integer, Rad As Single, HF As Integer
Dim FS As Integer, D1 As Integer, D2 As Integer, FS2 As Integer, E1 As Integer, E2 As Integer, PcOrHuman As Integer
Dim EasyOrHard As Integer, CA() As Integer, G1 As Integer, G2 As Integer, F1 As Integer, F2 As Integer

'c,d,e,g,f
'd,e,f

Dim ThisIsTesting As Integer
Dim EatA() As String, ForEat As Integer
Dim Saved As Integer, BU(1 To 8, 1 To 8) As Integer
Dim Checks1 As Integer, Checks2 As Integer, Last As Integer
Private Sub Backup()
If Saved = 0 Then
Saved = 1
For F1 = 1 To 8
For F2 = 1 To 8
BU(F1, F2) = B(F1, F2)
Next
Next
Else
Saved = 0
FS = 0
For F1 = 1 To 8
For F2 = 1 To 8
B(F1, F2) = BU(F1, F2)
Next
Next
End If

Dim m, a1, b1
m = ""
For a1 = 1 To 8
For b1 = 1 To 8
m = m & BU(b1, a1) & vbTab
Next
m = m & vbCrLf
Next
MsgBox m

End Sub


Private Sub CheckSide(GG As Integer)
GG = 0
For F1 = 1 To 8 Step 7
For F2 = 2 To 7
If B(F1, F2) = HF Then GG = GG + 1
If B(F2, F1) = HF Then GG = GG + 1
Next
Next
End Sub

Private Sub Try_to_catch_L_corners()
CheckMovable 3, 3
CheckMovable 3, 6
CheckMovable 6, 3
CheckMovable 6, 6
End Sub

Private Sub Try_to_catch_the_corners()
CheckMovable 1, 1
CheckMovable 1, 8
CheckMovable 8, 1
CheckMovable 8, 8
End Sub

Private Sub Try_to_eat_a_piece_on_one_side()
For a1 = 1 To 8 Step 7
For b1 = 2 To 7
CheckMovable a1, b1
If FS = 1 Then Exit Sub
Next
Next
For b1 = 1 To 8 Step 7
For a1 = 2 To 7
CheckMovable a1, b1
If FS = 1 Then Exit Sub
Next
Next


End Sub


Private Sub ct_Timer()
ct.Enabled = False
CheckHumanMovable
End Sub

Private Sub EasyO_Click()
EasyOrHard = 1
End Sub


Private Sub FRes_Timer()
Me.Width = Me.Height
End Sub

Private Sub HardO_Click()
EasyOrHard = 2
End Sub

Private Sub HumanFirst_Click()
RB.Visible = True
HF = 1
EasyO.Visible = False
HardO.Visible = False
HumanFirst.Visible = False
PCFirst.Visible = False
Reset
End Sub

Private Sub LastT_Timer()
peopiece = 0
pcpiece = 0
For a1 = 1 To 8
For a2 = 1 To 8
If B(a1, a2) = HF Then peopiece = peopiece + 1
If B(a1, a2) = 3 - HF Then pcpiece = pcpiece + 1
Next
Next
If peopiece > pcpiece Then
MsgBox "You win!!!" & vbCrLf & "You >> " & peopiece & "  v.s  PC >> " & pcpiece
ElseIf peopiece = pcpiece Then
MsgBox "You tied with the PC!!!" & vbCrLf & "You >> " & peopiece & "  v.s  PC >> " & pcpiece
Else
MsgBox "PC wins!!!" & vbCrLf & "You >> " & peopiece & "  v.s  PC >> " & pcpiece
End If
LastT.Enabled = False
If MsgBox("Play again?", vbQuestion + vbYesNo, "Play again") = vbYes Then
Me.Cls
Reset
Else
Unload Me
End If
End Sub

Private Sub PCFirst_Click()
HF = 2
EasyO.Visible = False
HardO.Visible = False
HumanFirst.Visible = False
PCFirst.Visible = False
PcOrHuman = 2
Reset
RB.Visible = True
PCT.Enabled = True
End Sub
Private Sub Reset()
Last = 0
Checks1 = 0
Moves = 0
For C1 = 1 To 8
For C2 = 1 To 8
B(C1, C2) = 0
Next
Next
B(4, 4) = 1
B(5, 5) = 1
B(4, 5) = 2
B(5, 4) = 2
DrawA
End Sub
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If HF <> 0 Then
If X > Rad And X < Rad * 9 And Y > Rad And Y < Rad * 9 Then
If HF = 1 Then
Me.FillColor = vbBlack
PcOrHuman = 1
Else
Me.FillColor = vbWhite
PcOrHuman = 2
End If
C1 = (X - Rad * 0.5) / Rad
C2 = (Y - Rad * 0.5) / Rad
FS = 0
CheckMovable C1, C2
If FS = 1 Then PCT.Enabled = True
End If
End If
End Sub
Private Sub Form_Resize()
If PCFirst.Visible = False Then
Me.Cls
Rad = Me.ScaleWidth / 10
DrawA
End If
End Sub
Private Sub Form_Load()
ThisIsTesting = 0
EasyOrHard = 1
PcOrHuman = 0
HF = 0
Rad = Me.ScaleWidth / 10
End Sub
Private Sub DrawA()
For C1 = 1 To 9
Me.Line (Rad * C1, Rad)-(Rad * C1, Rad * 9)
Me.Line (Rad, Rad * C1)-(Rad * 9, Rad * C1)
Next
For C2 = 1 To 8
For C1 = 1 To 8
If B(C1, C2) = 1 Then
Me.FillColor = vbBlack
Me.Circle (Rad * C1 + Rad / 2, Rad * C2 + Rad / 2), Me.ScaleWidth / 22
ElseIf B(C1, C2) = 2 Then
Me.FillColor = vbWhite
Me.Circle (Rad * C1 + Rad / 2, Rad * C2 + Rad / 2), Me.ScaleWidth / 22
End If
Next
Next
End Sub
Private Sub CheckMovable(Q1, Q2)
If FS = 0 Then
If B(Q1, Q2) = 0 Then
''''''''''First Section
If Q1 <> 1 And Q2 <> 1 Then
If B(Q1 - 1, Q2 - 1) = 3 - PcOrHuman Then
D1 = Q1 - 1
D2 = Q2 - 1
FSA -1, -1, 0, 0
End If
End If

If Q1 <> 1 And Q2 <> 8 Then
If B(Q1 - 1, Q2 + 1) = 3 - PcOrHuman Then
D1 = Q1 - 1
D2 = Q2 + 1
FSA -1, 1, 0, 9
End If
End If

If Q1 <> 1 Then
If B(Q1 - 1, Q2) = 3 - PcOrHuman Then
D1 = Q1 - 1
D2 = Q2
FSA -1, 0, 0, 0
End If
End If

If Q2 <> 8 Then
If B(Q1, Q2 + 1) = 3 - PcOrHuman Then
D1 = Q1
D2 = Q2 + 1
FSA 0, 1, 0, 9
End If
End If

If Q2 <> 1 Then
If B(Q1, Q2 - 1) = 3 - PcOrHuman Then
D1 = Q1
D2 = Q2 - 1
FSA 0, -1, 0, 0
End If
End If

If Q1 <> 8 And Q2 <> 8 Then
If B(Q1 + 1, Q2 + 1) = 3 - PcOrHuman Then
D1 = Q1 + 1
D2 = Q2 + 1
FSA 1, 1, 9, 9
End If
End If

If Q1 <> 8 Then
If B(Q1 + 1, Q2) = 3 - PcOrHuman Then
D1 = Q1 + 1
D2 = Q2
FSA 1, 0, 9, 0
End If
End If

If Q2 <> 1 And Q1 <> 8 Then
If B(Q1 + 1, Q2 - 1) = 3 - PcOrHuman Then
D1 = Q1 + 1
D2 = Q2 - 1
FSA 1, -1, 9, 0
End If
End If

If FS = 1 Then
B(Q1, Q2) = PcOrHuman
DrawA
End If
End If
End If
End Sub
Private Sub PcThink()
FS = 0
PcOrHuman = 3 - HF
If EasyOrHard = 1 Then
RandomMove
Else
'''''''''''''''''''''''Try to catch the corners
Try_to_catch_the_corners
'''''''''''''''''''''''If you can eat a piece on a side (Try never taking 2,2 2,7 7,2 7,7)
Try_to_eat_a_piece_on_one_side
'''''''''''''''''''''''Try catch sides (But if there is a piece beside don't take) (Never taking 2,2 2,7 7,2 7,7)

'''''''''''''''''''''''3,3 3,6 6,3 6,6
Try_to_catch_L_corners
'''''''''''''''''''''''Eat the maximum number of pieces (Never let human take side or corner)(Don't go out 4*4)

''''test
If FS = 0 Then RandomMove
End If
FS = 0
CheckHumanMovable
End Sub
Private Sub FSA(AA, AB, AC, AD)
E1 = D1
E2 = D2
FS2 = 0
Do Until D2 = AD Or D1 = AC Or FS2 = 1

If B(D1, D2) <> 0 Then
''''''''''''''''''''''''''''''''''''''''''''''''
'If ThisIsTesting = 1 Then
'ForEat = ForEat + 1
'EatA(ForEat) = D1 & D2
'End If
''''''''''''''''''''''''''''''''''''''''''''''''
If B(D1, D2) = PcOrHuman Then
'MsgBox ThisIsTesting
If ThisIsTesting = 1 Then
Select Case HF - PcOrHuman
Case 0
Checks2 = 0
Case Else
Checks1 = 0
End Select
Else
FS2 = 1
FS = 1
End If
End If
Else
''''''''''''''''''''''''''''''''''''''''''''''''
'ReDim EatA(1 To 14) As String
''''''''''''''''''''''''''''''''''''''''''''''''
Exit Do
End If

D1 = D1 + AA
D2 = D2 + AB
Loop

If FS2 = 1 Then
Do Until B(E1, E2) = PcOrHuman
B(E1, E2) = PcOrHuman
E1 = E1 + AA
E2 = E2 + AB
Loop
End If
End Sub

Private Sub PCT_Timer()
PCT.Enabled = False
PcThink
End Sub
Private Sub ScrambleAnArray(Array1() As Integer, Last As Integer)
For E1 = 1 To Last
Randomize
E2 = Fix(Rnd() * Last + 1)
D1 = Array1(E1)
Array1(E1) = Array1(E2)
Array1(E2) = D1
Next
End Sub
Private Sub CheckHumanMovable()
If Last = 0 Then

Checks2 = 1
Checks1 = 1

ThisIsTesting = 1
PcOrHuman = HF
For abc1 = 1 To 8 Step 1
For abc2 = 1 To 8 Step 1
'MsgBox ThisIsTesting & vbTab & Checks1
Call CheckMovable(abc2, abc1)
If Checks2 = 0 Then Exit For
'MsgBox a1 & vbTab & a2 & vbTab & ThisIsTesting & vbTab & Checks2
Next
Next

PcOrHuman = 3 - HF
For abd1 = 1 To 8 Step 1
For abd2 = 1 To 8 Step 1
'MsgBox abd1 & vbTab & abd2 & ThisIsTesting
Call CheckMovable(abd2, abd1)
If Checks1 = 0 Then Exit For
'MsgBox a1 & vbTab & a2 & vbTab & ThisIsTesting & vbTab & Checks1
Next
Next

ThisIsTesting = 0

If Checks2 = 1 And Checks1 = 1 Then
Last = 1
LastT.Enabled = True
ElseIf Checks2 = 1 And Checks1 = 0 Then
PCT.Enabled = True
End If
End If
End Sub
Private Sub RandomMove()
ReDim CA(1 To 64) As Integer
For D2 = 8 To 71
CA(D2 - 7) = D2
Next
ScrambleAnArray CA(), 64
For G1 = 1 To 64
C1 = CA(G1) Mod 8 + 1
C2 = Fix(CA(G1) / 8)
CheckMovable C1, C2
If FS = 1 Then Exit For
Next
End Sub

Private Sub RB_Click()
PcOrHuman = HF
ReDim CA(1 To 64) As Integer
For D2 = 8 To 71
CA(D2 - 7) = D2
Next
ScrambleAnArray CA(), 64
For G1 = 1 To 64
C1 = CA(G1) Mod 8 + 1
C2 = Fix(CA(G1) / 8)
CheckMovable C1, C2
If FS = 1 Then Exit For
Next
PCT.Enabled = True
End Sub
