#*======================================================================
#* File name    : dzcsh2b_.mk
#* Original	: 
#*----------------------------------------------------------------------
#* Function	:
#*	DZコマンド&テキストに関するライブラリーを作成する。
#*	SH2 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/dzcmd/src/RCS/dzcsh2b_.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: dzcsh2b_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  kawamura
#* 2003/05/30  13:56
#* *.lctファイルのコピー条件を修正
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*Header: p:/presto/shlib/dzcmd/src/RCS/dzcmd.mk 1.2 1970/01/01 00:00:00
#*Log: dzcmd.mk
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#	<<< デバッグ情報の定義 >>>
#
#DEBUG 	= 0
DEBUG	= 1
#
#	<<< エンディアンの定義 >>>
#
#ENDIAN	= 0
ENDIAN	= 1
#
#	<<< CPUの定義 >>>
#
CPUTYPE = 2
#CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=dzcsh3b_
else
TARGET		=dzcsh3l_
endif
else
CTYPE	= sh2
TARGET		=dzcsh2b_
endif

#
#	<<< ファイルネーム定義 >>>
#
ID		=dzcmdid
#
DZR009		=dzr009
DZR010		=dzr010
DZR203		=dzr203
DZR204		=dzr204
DZR205		=dzr205
DZR206		=dzr206
DZR209		=dzr209
DZR210		=dzr210
DZR211		=dzr211
DZR212		=dzr212
DZI203		=dzi203
DZI204		=dzi204
DZI205		=dzi205
DZI206		=dzi206

#===========================================================

ifeq ($(DEBUG),0)
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SHC_INC)
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#===========================================================
#
#	<<< ライブラリーの作成 >>>
#
OFILE1=$(ID).obj
OFILE2=$(DZR009).obj $(DZR010).obj $(DZR203).obj $(DZR204).obj $(DZR205).obj
OFILE3=$(DZR206).obj $(DZR209).obj $(DZR210).obj $(DZR211).obj $(DZR212).obj
OFILE4=$(DZI203).obj $(DZI204).obj $(DZI205).obj $(DZI206).obj 
OFILES=$(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4)
#
#
#
all : clean $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj : $(ID).c
	$(CC) $(ID).c				>  $(ERRFILE)
$(DZR009).obj : $(DZR009).c
	$(CC) $(DZR009).c			>>  $(ERRFILE)
$(DZR010).obj : $(DZR010).c
	$(CC) $(DZR010).c			>>  $(ERRFILE)
$(DZR203).obj : $(DZR203).c
	$(CC) $(DZR203).c			>>  $(ERRFILE)
$(DZR204).obj : $(DZR204).c
	$(CC) $(DZR204).c			>>  $(ERRFILE)
$(DZR205).obj : $(DZR205).c
	$(CC) $(DZR205).c			>>  $(ERRFILE)
$(DZR206).obj : $(DZR206).c
	$(CC) $(DZR206).c			>>  $(ERRFILE)
$(DZR209).obj : $(DZR209).c
	$(CC) $(DZR209).c			>>  $(ERRFILE)
$(DZR210).obj : $(DZR210).c
	$(CC) $(DZR210).c			>>  $(ERRFILE)
$(DZR211).obj : $(DZR211).c
	$(CC) $(DZR211).c			>>  $(ERRFILE)
$(DZR212).obj : $(DZR212).c
	$(CC) $(DZR212).c			>>  $(ERRFILE)
$(DZI203).obj : $(DZI203).c
	$(CC) $(DZI203).c			>>  $(ERRFILE)
$(DZI204).obj : $(DZI204).c
	$(CC) $(DZI204).c			>>  $(ERRFILE)
$(DZI205).obj : $(DZI205).c
	$(CC) $(DZI205).c			>>  $(ERRFILE)
$(DZI206).obj : $(DZI206).c
	$(CC) $(DZI206).c			>>  $(ERRFILE)

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
#	ＭＡＫＥコマンドの定義						#
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

