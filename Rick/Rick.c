void PlayRick();


int main() {
  PlayRick();
  return 0;
}

asm void PlayRick() {
  asm {
        jmp START
RICK1R  EQU     $E00
RICK2R  EQU     $108A
RICK1L  EQU     $1314
RICK2L  EQU     $15B6
BADGUY  EQU     $185C
BULLET  EQU     $1A9C
RICKNL  EQU     $1C6E
RICKNR  EQU     $1AB7
BLOCK   FCB     0
XAXIS   FCB     $7F
YAXIS   FCB     $75
MEMAR   FDB     $6240
TEMPA   FDB     $4778
XCOUNT  FDB     $4F65
BUBBLE  FDB     $5056
OFFSET  FDB     $5960
SCENE   FCB     $4A
XGOOD   FCB     $42
YGOOD   FCB     $49
GDTM1   FDB     $6A00
GDTM2   FDB     0
DFLAG   FCB     0
POINTS  RMB     6
TEMRES  FDB     4
FLAG    FCB     0
FLAG2   FCB     0                  0 means Rick not falling, 1 means falling
FLAG3   FCB     0                  Number of jump iters left to fall
COSFLG  FCB     0
PRCOL   FCB     0
PRCOUT  FCB     0
PRCNT   FCB     0
UP1     FCC     /1Up/
        FCB     $3a
        FCB     0
FIR     RMB     12
BADMEN  RMB     8
STACK   RMB     600

*******************************************************************************
* Printing routine
*******************************************************************************
PRINT   PSHS    U
        LDA     $FFA7
        STA     BLOCK
        LDA     #63
        STA     $FFA7
        LDB     YAXIS
        CLRA
        CMPB    #8
        BLO     PSET1
        SUBB    #8
        CMPB    #8
        BLO     PSET2
        SUBB    #8
        INCA
PSET2   INCA
PSET1   TST     SCENE
        BEQ     PR000
        ADDA    #6
PR000   STA     $FFA3
        LDA     #8
        MUL
        LDA     #128
        MUL
        ADDA    #$60
        TFR     D,X
        LDB     XAXIS
        LDA     #4
        MUL
        ABX
        LDY     MEMAR
PR001   LDA     ,Y+
        TSTA
        BEQ     PREND
        SUBA    #32
        LDB     #8
        MUL
        ADDD    #$F09D
        TFR     D,U
        LDA     #8
        STA     PRCNT
PR002   LDA     ,U+
        BSR     PRTROT
        LEAX    124,X
        DEC     PRCNT
        BNE     PR002
        LEAX    -1020,X
        BRA     PR001
PRTROT  LDB     #4
        STB     PRCOUT
LDCL    LDB     PRCOL
        TSTA
        BPL     PRROT1
        ANDB    #15
PRROT1  ROLA
        BPL     PRROT2
        ANDB    #240
PRROT2  STB     ,X+
        ROLA
        DEC     PRCOUT
        BNE     LDCL
        RTS
PREND   LDA     BLOCK
        STA     $FFA7
        PULS    U
        RTS

*******************************************************************************
* Drawing routiine
*******************************************************************************
PUT     LDD     OFFSET
        SUBD    #2
        SUBD    MEMAR
        STD     R1+2               * Modify code
        STD     END+2              * Modify code
        STD     ADDY+2             * Modify code
        LDD     XAXIS
        ADDA    #2
        ANDA    #127
        LDX     MEMAR
        STA     TEMPA+1
        CLR     XCOUNT+1
        TST     SCENE
        BNE     PNXT
        CLRA
        BRA     PNXT1

PNXT    LDA     #6
PNXT1   STA     $FFAB
        INCA
        STA     $FFAC
        INCA
        STA     $FFAD
        LDA     $FF91
        ORA     #1
        STA     $FF91
        LDA     #128
        MUL
        ADDD    TEMPA
        ADDD    #$6000
        TFR     D,Y
CHEK1   LDD     ,X++
        INC     XCOUNT+1
        INC     XCOUNT+1
        CMPA    #$FF
        BEQ     ADDY
STA     STD     BUBBLE
        LDD     ,Y
R1
        STD     800,X              * Code get modified
        LDA     BUBBLE
        TFR     A,B
        ANDA    #15
        TSTA
        BEQ     FLY00
        LDB     ,Y
        ANDB    #240
        STB     ,Y
        ORA     ,Y
        STA     ,Y
FLY00   LDB     BUBBLE
        ANDB    #240
        TSTB
        BEQ     BLAR
        LDA     ,Y
        ANDA    #15
        STA     ,Y
        ORB     ,Y
        STB     ,Y
BLAR    LEAY    1,Y
CHEK03  LDA     BUBBLE+1
        ANDA    #15
        TSTA
        BEQ     FLY01
        LDB     ,Y
        ANDB    #240
        STB     ,Y
        ORA     ,Y
        STA     ,Y
FLY01   LDB     BUBBLE+1
        ANDB    #240
        TSTB
        BEQ     BLAR1
        LDA     ,Y
        ANDA    #15
        STA     ,Y
        ORB     ,Y
        STB     ,Y
BLAR1   LEAY    1,Y
        BRA     CHEK1
ADDY
        STD     800,X              * Code gets modified
        LDD     ,X++
        CMPA    #$FF
        BEQ     END
        LEAY    128,Y
        EXG     Y,D
        SUBD    XCOUNT
        CLR     XCOUNT+1
        EXG     Y,D
        BRA     STA
END
        STD     800,X              * Code gets modified

ENDD    LDA     $FF91
        ANDA    #$FE
        STA     $FF91
        RTS

*******************************************************************************
* Screen decompression routine
*******************************************************************************
DECOMP  LDX     #$6000
        LDA     #3
        STA     DFLAG
        PSHS    U
        LDA     #6
        LDB     SCENE
        MUL
        STB     $FFA3
        LDY     #$E00+6144
DSUB    LDA     ,Y+
        CMPA    #$FF
        BEQ     DDSTR
        STA     ,X+
        CMPX    #$8000
        BLO     DSUB
        DEC     DFLAG
        BEQ     DSTOP
        INC     $FFA3
        LDX     #$6000
        BRA     DSUB
DDSTR   LDD     ,Y++
        TFR     D,U
        LDA     ,Y+
DSTR0   LEAU    -1,U
        STA     ,X+
        CMPU    #0
        BEQ     DSUB
        CMPX    #$8000
        BLO     DSTR0
        DEC     DFLAG
        BEQ     DSTOP
        LDX     #$6000
        INC     $FFA3
        BRA     DSTR0
DSTOP   PULS    U
        RTS

*******************************************************************************
* Clear the area that was previously drawn on
*******************************************************************************
CLEAR   LDD     XAXIS
        ADDA    #2
        ANDA    #127
        LDX     MEMAR
        STA     TEMPA+1
        CLR     XCOUNT+1
        TST     SCENE
        BNE     CNXT
        CLRA
        BRA     CNXT1
CNXT    LDA     #6
CNXT1   STA     $FFAB
        INCA
        STA     $FFAC
        INCA
        STA     $FFAD
        LDA     $FF91
        ORA     #1
        STA     $FF91
        LDA     #128
        MUL
        ADDD    TEMPA
        ADDD    #$6000
        TFR     D,Y
CCEK1   LDD     ,X++
        INC     XCOUNT+1
        INC     XCOUNT+1
        CMPA    #$FF
        BEQ     ADD
STA2    STA     ,Y+
CCEK03  STB     ,Y+
        BRA     CCEK1
ADD     LDD     ,X++
        CMPA    #$FF
        LBEQ    ENDD
        LEAY    128,Y
        EXG     Y,D
        SUBD    XCOUNT
        CLR     XCOUNT+1
        EXG     Y,D
        BRA     STA2

*******************************************************************************
* Draw the score
*******************************************************************************
SCORE   LDX     #POINTS+3
        LDY     #TEMRES+3
        LDA     ,-X
        ADCA    #$10
        DAA
        STA     ,-Y
        LDA     ,-X
        ADCA    #0
        DAA
        STA     ,-Y
        LDA     ,-X
        ADCA    #0
        DAA
        STA     ,-Y
        LDA     TEMRES
        STA     POINTS
        PSHS    A
        ANDA    #240
        RORA
        RORA
        RORA
        RORA
        CMPA    #10
        BLO     SLOOP3
        CLRA
SLOOP3  TFR     A,B
        PULS    A
        ANDA    #15
        EXG     A,B
        ADDD    #$3030
        STD     1024
        LDA     TEMRES+1
        STA     POINTS+1
        PSHS    A
        ANDA    #240
        RORA
        RORA
        RORA
        RORA
        ANDA    #15
        TFR     A,B
        PULS    A
        ANDA    #15
        EXG     A,B
        ADDD    #$3030
        STD     1026
        LDA     TEMRES+2
        STA     POINTS+2
        PSHS    A
        ANDA    #240
        RORA
        RORA
        RORA
        RORA
        TFR     A,B
        PULS    A
        ANDA    #15
        EXG     A,B
        ADDD    #$3030
        STD     1028
        CLR     1030
        LDD     #$0600
        STD     XAXIS
        LDD     #1024
        STD     MEMAR
        LDA     #$AA
        STA     PRCOL
        JSR     CHNGSC
        JSR     PRINT
        JSR     CHNGSC
        JSR     PRINT
        RTS
CHNGSC  COM     SCENE
        TST     SCENE
        BEQ     CSC000
        LDA     #3
        LDB     #8
        BRA     CSC001
CSC000  LDA     #4
        LDB     #9
CSC001  STA     $FFA6
        STA     $FFAE
        STB     $FFA7
        STB     $FFAF
        RTS

*******************************************************************************
* Interrupt handler
*******************************************************************************
INTH    LDA     $FF92        
        RTI

*******************************************************************************
* Keyboard routine
*******************************************************************************
KEYIN   PSHS    U
        JSR     [$A000]
        PULS    U
        RTS

*******************************************************************************
* Joystick routine
*******************************************************************************
JOYIN   PSHS    U
        JSR     [$A00A]
        PULS    U
        RTS

*******************************************************************************
* Program starts here
*******************************************************************************
START   CLR     $71                Not sure what this does

*******************************************************************************
* Disable interrupts, setup interrupt for VSYNC
*******************************************************************************
        ORCC    #$50
        LDS     #STACK+500
        LDA     #$34
        LDX     #$FF00
        STA     1,X
        STA     3,X
        STA     $21,X
        STA     $23,X
        LDA     ,X
        LDA     2,X
        LDA     $20,X
        LDA     $22,X
        LDA     #$EE
        STA     $90,X
        LDA     #8
        STA     $92,X
        LDA     $92,X
        LDA     $93,X
        LDA     #$7E
        LDX     #INTH
        STA     $10C
        STX     $10D
        ANDCC   #$EF

*******************************************************************************
* Set palette
*******************************************************************************
PALETTE_SETUP
        LDX     #XAXIS
        LDY     #$FFB0
PALSET  LDA     ,X
        CLR     ,X+
        STA     ,Y+
        CMPY    #$FFBF
        BLO     PALSET

*******************************************************************************
* Zero out area for storing locations of bad guys
*******************************************************************************
        LDX     #BADMEN
CLRBD   CLR     ,X+
        CMPX    #BADMEN+8
        BLO     CLRBD

*******************************************************************************
* Copy executive MMU registers to task registers
*******************************************************************************
        LDX     #$FFA0
SLD     LDA     ,X
        STA     8,X
        LEAX    1,X
        CMPX    #$FFA8
        BLE     SLD

*******************************************************************************
* Initialize variables
*******************************************************************************
        CLR     FLAG2              By default, Rick is not falling
        CLR     FLAG3              Start jumping at 0
        LDA     #$50
        TFR     A,DP
        LDD     #0
        STD     POINTS
        STD     POINTS+2
        STD     POINTS+4
        STD     FIR
        STD     FIR+2
        STD     FIR+4
        STD     FIR+6
        STD     FIR+8
        STD     FIR+10
        STD     POINTS
        STD     POINTS+2
        STD     POINTS+4
        STD     XAXIS
        LDD     #135
        STD     XGOOD
        STD     GDTM1
        STD     GDTM2

*******************************************************************************
* Decompress the background picture
*******************************************************************************
        CLR     SCENE              Decompress on screen 0
        LBSR    DECOMP
        INC     SCENE              Decompress on screen 1
        LBSR    DECOMP

*******************************************************************************
* Print Score
*******************************************************************************
        CLR     SCENE              Print on screen 0
        LBSR    CHNGSC
        LDD     #UP1
        STD     MEMAR
        LDA     #$AA
        STA     PRCOL
        LBSR    PRINT
        LBSR    SCORE
        
*******************************************************************************
* This may be initializing the buffers we use to store sprite backing data
*******************************************************************************
        LBSR    CHNGSC
        LDD     #$FFFF
        STD     $C100
        STD     $C102
        STD     $C104
        LBSR    CHNGSC
        LDD     #$FFFF
        STD     $C100
        STD     $C102
        STD     $C104

*******************************************************************************
* This is the start of the main game loop
*******************************************************************************
GO      LBSR    KEYIN              Pause if enter is pressed
        CMPA    #13
        BNE     GCONT
        LBSR    PAUSE

GCONT   LBSR    JOYIN              Read the joystick
        LDA     $FF00
        ANDA    #2
        BNE     FSKIP
        LBSR    FIRE               Start the fire routine if button is pressed

FSKIP
        LBSR    BULPUT             Draw the bullets
        LBSR    GENBAD             Generate new bad guys if needed
        LBSR    PUTBAD             Draw the bad guys

RICKMOTION
        TST     FLAG2              If zero, Rick is not falling
        BEQ     JUMP
        LDA     YGOOD
        ADDA    #8                 Rick is falling
        STA     YGOOD              Update Ricks Y position
        DEC     FLAG3              Update how far back Rick has gone
        BNE     JSKP               Skip the jumping routine
        CLR     FLAG2              Note that Rick is not jumping

JUMP    LDA     $FF00              Is jump button pressed?
        ANDA    #8
        BEQ     JGO                Yes, start jump routine
        TST     FLAG3              No, should Rick fall?
        BEQ     JSKP               No, already on ground, skip jump
        INC     FLAG2              Indicate that Rick should fall
        BRA     JSKP               Skip the jumping routine

JGO     LDA     YGOOD              Start jumping
        CLR     FLAG2              Note that Rick is not falling
        LDB     FLAG3
        CMPB    #8                 Is Rick at the top?
        BLO     JNXT               No, so start routine to move up
        INC     FLAG2              Yes, note we are falling
        BRA     JSKP

JNXT    SUBA    #8                 Move  up
        STA     YGOOD
        INC     FLAG3              Note we have one more iter to fall

JSKP    LDA     $15C               Load the joystick
        CMPA    #22
        BGE     CHECKRIGHT
        BSR     MOVL               Move left
        BRA     NXT

CHECKRIGHT
        CMPA    #44
        BLE     DONTMOVE        
        BSR     MOVR               Move right?
        BRA     NXT

DONTMOVE
        LBSR    MOVN               Do not move

NXT     LBSR    SHOW_SCREEN
        LBSR    CHNGSC
        LDD     $C000
        STD     XAXIS
        LDD     #$C100
        STD     MEMAR
        LBSR    CLEAR
        LBSR    CLRBAD
        LBSR    FXBLL
        LBRA    GO

MOVL    INC     COSFLG
        LDD     XGOOD
        TSTA
        BLE     MVL
        SUBA    #3
MVL     STD     XGOOD
        STD     XAXIS
        STD     $C000
        LDD     #$C100
        STD     OFFSET
        TST     SCENE
        BNE     MOVL0
        LDD     #RICK1L
        BRA     MOVL1
MOVL0   LDD     #RICK2L
MOVL1   STD     MEMAR
        LBSR    PUT
        RTS

MOVR    CLR     COSFLG
        LDD     XGOOD
        CMPA    #112
        BHI     MVR
        ADDA    #3
        STD     XGOOD
MVR     STD     XAXIS
        STD     $C000
        LDD     #$C100
        STD     OFFSET
        TST     SCENE
        BNE     MOVR0
        LDD     #RICK1R
        BRA     MOVL1
MOVR0   LDD     #RICK2R
        BRA     MOVL1

MOVN    TST     COSFLG
        BEQ     MOVN0
        LDA     $15D
        CMPA    #40
        BLO     MOVNU
        TST     FLAG3
        BNE     MOVNU
        LDD     #RICKNR
        BRA     MOVN1
MOVNU   LDD     #RICK1L
        BRA     MOVN1
MOVN0   LDA     $15D
        CMPA    #40
        BLO     MOVN2
        TST     FLAG3
        BNE     MOVN2
        LDD     #RICKNL
        BRA     MOVN1
MOVN2   LDD     #RICK1R
MOVN1   STD     MEMAR
        LDD     #$C100
        STD     OFFSET
        LDD     XGOOD
        STD     $C000
        STD     XAXIS
        LBSR    PUT
        RTS

*******************************************************************************
* Pause routine
*******************************************************************************
PAUSE   LBSR    KEYIN
        CMPA    #13
        BNE     PAUSE
        RTS

FIRE    LDX     #FIR
FSUB    LDA     ,X
        TSTA
        BEQ     FYES
        LEAX    4,X
        CMPX    #FIR+12
        BHS     FEND
        BRA     FSUB
FYES    LDA     COSFLG
        INC     ,X+
        STA     ,X+
        LDD     XGOOD
        ADDB    #22
        STD     ,X
        LEAX    -FIR,X
        TFR     X,D
        LDA     #8
        MUL
        TFR     D,X
        LBSR    CHNGSC
        LDD     #$FFFF
        STD     $C400,X
        STD     $C402,X
        LBSR    CHNGSC
FEND    RTS

*******************************************************************************
* Routine for drawing the bullets
*******************************************************************************
BULPUT  LDX     #FIR
BLLSUB  LDA     ,X+
        CMPA    #137
        BEQ     BLLPLS
        TSTA
        BNE     BLLYES
BLLPLS  LEAX    3,X
        CMPX    #FIR+12
        BLO     BLLSUB
        RTS
BLLYES  PSHS    X
        LDB     ,X+
        TSTB
        BEQ     BLLR
        LDD     ,X
        CMPA    #5
        BLE     BRSET
        SUBA    #5
BLLSB   STD     XAXIS
        STD     ,X
        LEAX    -FIR,X
        STD     $C002,X
        TFR     X,D
        LDA     #8
        MUL
        ADDD    #$C400
        STD     OFFSET
        LDD     #BULLET
        STD     MEMAR
        LBSR    PUT
        PULS    X
        BRA     BLLPLS
BRSET   LEAX    -2,X
        LDA     #137
        STA     ,X
        PULS    X
        BRA     BLLPLS
BLLR    LDD     ,X
        CMPA    #120
        BHS     BRSET
        ADDA    #5
        BRA     BLLSB
FXBLL   LDX     #FIR+12
FXSUB   LEAX    -4,X
        LDA     ,X++
        CMPA    #137
        BEQ     FXCLR
        TSTA
        BEQ     FXBL2
        BRA     FXNXT
FXCLR   LEAX    -2,X
        CLR     ,X
        LEAX    2,X
        BRA     FXNXT
FXBL2   LEAX    -2,X
        CMPX    #FIR
        BHI     FXSUB
        RTS
FXNXT   PSHS    X
        LEAX    -FIR,X
        LDD     $C002,X
        STD     XAXIS
        TFR     X,D
        LDA     #8
        MUL
        ADDD    #$C400
        STD     MEMAR
        LBSR    CLEAR
        PULS    X
        BRA     FXBL2

*******************************************************************************
* Routine for drawing the bad guys
*******************************************************************************
PUTBAD  LDX     #BADMEN
PDLOAD  LDA     ,X+
        BNE     FBDLOC
BDPLUS  LEAX    3,X
        CMPX    #BADMEN+8
        BLO     PDLOAD
        RTS
FBDLOC  PSHS    X
        LDD     ,X++
        TST     ,X
        BEQ     PTBDRT
        SUBA    #4
        BRA     PBDNXT
PTBDRT  ADDA    #4
PBDNXT  STD     XAXIS
        STD     -2,X
        LEAX    -BADMEN,X
        STD     $C020,X
        TFR     X,D
        LDA     #150
        MUL
        TFR     D,X
        LEAX    $C600,X
        STX     OFFSET
        LDY     #BADGUY
        STY     MEMAR
        LBSR    PUT
        PULS    X
        BRA     BDPLUS

*******************************************************************************
* Generate new bad guys if they are dead?
*******************************************************************************
GENBAD  LDX     #BADMEN
GENLD   LDA     ,X
        BEQ     BDMARK
        LEAX    4,X
        CMPX    #BADMEN+8
        BLO     GENLD
        RTS
BDMARK  INC     ,X+
        TST     SCENE
        BNE     BDLFT
        LDD     #$208B
        STD     ,X++
        CLR     ,X
        BRA     BDCARE
BDLFT   LDD     #$8B
        STD     ,X++
        LDA     #1
        STA     ,X
BDCARE  LEAX    -BADMEN,X
        TFR     X,D
        LDA     #150
        MUL
        ADDD    #$C600
        TFR     D,X
        PSHS    X
        JSR     CHNGSC
        PULS    X
        LDD     #$FFFF
        STD     ,X++
        STD     ,X++
        JSR     CHNGSC
        RTS

CLRBAD  LDX     #BADMEN+8
BDTST   LEAX    -4,X
        LDA     ,X
        TSTA
        BNE     BDCYES
BCLRSB  CMPX    #BADMEN
        BHI     BDTST
        RTS
BDCYES  PSHS    X
        LEAX    3,X
        LEAX    -BADMEN,X
        LDD     $C020,X
        STD     XAXIS
        TFR     X,D
        LDA     #150
        MUL
        TFR     D,X
        LEAX    $C600,X
        STX     MEMAR
        LBSR    CLEAR
        PULS    X
        BRA     BCLRSB

*******************************************************************************
* Routine for showing the screen
*******************************************************************************
SHOW_SCREEN
        CWAI    #$EF
        LDA     $FF90
        ANDA    #127
        ORA     #0x60
        STA     $FF90
        LDA     #128
        STA     $FF98
        LDA     #26
        STA     $FF99
        CLR     $FF9E
        LDA     SCENE
        TSTA
        BNE     Screen1
Screen0 
        CLRA
        BRA     Finish
Screen1 LDA     #24
Finish  STA     $FF9D
        RTS

	}
}

