#-----------------------------------------------------------------------#
# File name	: ndadcdrv.mk	/ digital proc. adc driver liblary	#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/ndadcdrv.mk 1.4 1970/01/01 00:00:00 chimura Exp $
#*$Log: ndadcdrv.mk $
#* リビジョン 1.4  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.3  1970/01/01  00:00:00  m-yamana
#* 2003/07/03  16:00
#* ・メークファイルに柔軟性を持たせた。
#*   (プリメークファイル DADC5554.MK および DADC5561.MK を作成し、これらから ndadcdrv.mk を呼ぶようにした。)
#* 
#*   DOSで >gnumake -S -fdadc5554.mk とすれば、P5554用ライブラリ dadc5554.lib が作成され、
#*         >gnumake -S -fdadc5561.mk とすれば、P5561用ライブラリ dadc5561.lib が作成される。
#* 
#*   DADC5554.MK または DADC5561.MK を編集することで、エンディアン、デバッグモードの変更が行える。
#* 
#*   なお、リリースは >gnumake -S -fdadc5554.mk release というように実行すれば release.id ができる。
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  m-yamana
#* 2003/06/30  15:50
#* ・基板(P5561/P5554)に対応させた。
#*   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
#*   定義しないことでP5554(村田氏作成)に対応させた。
#*   
#* ・エンディアン(ビッグ/リトル)に対応させた。
#*   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
#*   定義しないことでリトルエンディアンに対応させた。
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	ADC(P-5422)関連ドライバーのライブラリーを作成する。		#
#-----------------------------------------------------------------------#
#	<<< ファイルネーム定義 >>>
ID		=dndadcid

DADSYINI	=dadsini
DADBOOT0	=dadbot0
DADBOOT1	=dadbot1
DADBOOT2	=dadbot2
DADBOOT3	=dadbot3
DADFZERO	=dadfzt
DADSPAN		=dadspn
DADAFV		=dadafv
DADWDRD		=dadwhd
DADDFRD		=daddfd
DADMRD0		=dadmr0
DADMWR0		=dadmw0
DADMON0		=dadmon0
DADMON1		=dadmon1
DADCSCK		=dadsck
DADCRES		=dadres
DADMZERO	=dadmzt
DADMRD1		=dadmr1
DADMWR1		=dadmw1
DADMON2		=dadmon2
DADMON3		=dadmon3
DADDIAG		=daddiag
DAD0INT		=dad0int
DAD1INT		=dad1int

DADREAD		=dadcrd
DADWRITE	=dadcwr
DADITSK		=daditsk
DADINV		=dadinv

MAKEFILE =ndadcdrv.mk

#	<<< ヘッダー･ファイル定義 >>>

DADCTL		=dadctl
DADCTXT		=dadctxt
#===========================================================
#	<<< デバッグ情報の定義 >>>
# 
# ENDIAN_BIG	1:Big		1以外:Little
# P5561		1:P5561		1以外:P5554
# DEBUG		1:Debug		1以外:No Debug
# 

ifeq ($(ENDIAN_BIG),1)
OPT_ENDIAN=big
DEF_ENDIAN=MAS_BIG_ENDIAN
else
OPT_ENDIAN=little
DEF_ENDIAN=MAS_LITTLE_ENDIAN
endif

ifeq ($(P5561),1)
PNAME		=dadc5561
DEF_PNAM	=MAS_P5561
else
PNAME		=dadc5554
DEF_PNAM	=MAS_P5554
endif

ifeq ($(DEBUG),1)
TARGET		=$(PNAME)_
OPT_OP=0
OPT_DEBUG=-debug
DEF_DEBUG=DEBUG
else
TARGET		=$(PNAME)
OPT_OP=1
OPT_DEBUG=-nodebug
DEF_DEBUG=NDEBUG
endif

CCOPT	= -cpu=sh3 -op=$(OPT_OP) -en=$(OPT_ENDIAN) -me $(OPT_DEBUG) -comment=nest -l -def=$(DEF_DEBUG),$(DEF_PNAM),$(DEF_ENDIAN)

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#	<<< ライブラリーの作成 >>>
#
OFILES	=$(ID).obj $(DADSYINI).obj $(DADBOOT0).obj $(DADBOOT1).obj $(DADFZERO).obj\
	 $(DADSPAN).obj $(DADAFV).obj $(DADWDRD).obj $(DADDFRD).obj\
	 $(DADMRD0).obj $(DADMWR0).obj $(DADMON0).obj $(DADMON1).obj\
	 $(DADCRES).obj $(DADBOOT2).obj $(DADBOOT3).obj $(DADCSCK).obj\
	 $(DADMZERO).obj $(DADMRD1).obj $(DADMWR1).obj $(DADMON2).obj\
	 $(DADMON3).obj $(DADDIAG).obj $(DAD0INT).obj $(DAD1INT).obj\
	 $(DADITSK).obj $(DADREAD).obj $(DADWRITE).obj $(DADINV).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>
#
$(ID).obj : $(ID).c
	$(CC) $(ID).c				> x
$(DADSYINI).obj : $(DADSYINI).c $(DADCTXT).h
	$(CC) $(DADSYINI).c			>> x
$(DADBOOT0).obj : $(DADBOOT0).c
	$(CC) $(DADBOOT0).c			>> x
$(DADBOOT1).obj : $(DADBOOT1).c
	$(CC) $(DADBOOT1).c			>> x
$(DADFZERO).obj : $(DADFZERO).c
	$(CC) $(DADFZERO).c			>> x
$(DADSPAN).obj : $(DADSPAN).c
	$(CC) $(DADSPAN).c			>> x
$(DADAFV).obj : $(DADAFV).c
	$(CC) $(DADAFV).c			>> x
$(DADWDRD).obj : $(DADWDRD).c
	$(CC) $(DADWDRD).c			>> x
$(DADDFRD).obj : $(DADDFRD).c
	$(CC) $(DADDFRD).c			>> x
$(DADMRD0).obj : $(DADMRD0).c
	$(CC) $(DADMRD0).c			>> x
$(DADMWR0).obj : $(DADMWR0).c
	$(CC) $(DADMWR0).c			>> x
$(DADMON0).obj : $(DADMON0).c
	$(CC) $(DADMON0).c			>> x
$(DADMON1).obj : $(DADMON1).c
	$(CC) $(DADMON1).c			>> x
$(DADCRES).obj : $(DADCRES).c
	$(CC) $(DADCRES).c			>> x
$(DADBOOT2).obj : $(DADBOOT2).c
	$(CC) $(DADBOOT2).c			>> x
$(DADBOOT3).obj : $(DADBOOT3).c
	$(CC) $(DADBOOT3).c			>> x
$(DADCSCK).obj : $(DADCSCK).c
	$(CC) $(DADCSCK).c			>> x
$(DADMZERO).obj : $(DADMZERO).c
	$(CC) $(DADMZERO).c			>> x
$(DADMRD1).obj : $(DADMRD1).c
	$(CC) $(DADMRD1).c			>> x
$(DADMWR1).obj : $(DADMWR1).c
	$(CC) $(DADMWR1).c			>> x
$(DADMON2).obj : $(DADMON2).c
	$(CC) $(DADMON2).c			>> x
$(DADMON3).obj : $(DADMON3).c
	$(CC) $(DADMON3).c			>> x
$(DADDIAG).obj : $(DADDIAG).c
	$(CC) $(DADDIAG).c			>> x
$(DAD0INT).obj : $(DAD0INT).c $(DADCTXT).h
	$(CC) $(DAD0INT).c			>> x
$(DAD1INT).obj : $(DAD1INT).c $(DADCTXT).h
	$(CC) $(DAD1INT).c			>> x
$(DADITSK).obj : $(DADITSK).c $(DADCTXT).h
	$(CC) $(DADITSK).c			>> x
$(DADREAD).obj : $(DADREAD).c $(DADCTXT).h
	$(CC) $(DADREAD).c			>> x
$(DADWRITE).obj : $(DADWRITE).c $(DADCTXT).h
	$(CC) $(DADWRITE).c			>> x
$(DADINV).obj : $(DADINV).c
	$(CC) $(DADINV).c			>> x

$(TARGET).lib : $(OFILES) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(OPTLNK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib ..\*.*

$(TARGET).sub :
	echo	FORM	= library		>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib		>>$(TARGET).sub
	echo	INPUT	= $(OFILES)		>>$(TARGET).sub
	echo	LIST	= $(TARGET)		>>$(TARGET).sub
	echo	EXIT				>>$(TARGET).sub

#-----------------------------------------------------------------------#
#			ＭＡＫＥコマンドの定義										#
#-----------------------------------------------------------------------#
DRV 	=p:

checkin :
	perl $(DRV)\tool\perl\checkin.pl *.mk	>  $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.h	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.c	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.asm	>> $(TARGET).bat

clean :
	if exist *.obj del *.obj
	if exist *.lbp del *.lbp
	if exist *.lst del *.lst
	if exist *.sub del *.sub
	if exist *.lib del *.lib

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
