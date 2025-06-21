#*======================================================================
#* File name    : cnvsh3b.mk
#* Original	: 
#*----------------------------------------------------------------------
#* Function	:
#*	RX116 -> NORTi コンバートに関するライブラリーを作成する。	
#*	SH3 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/cnv/src/RCS/cnvsh3b.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: cnvsh3b.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/07/22 18:15 誤記修正
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*Header: p:/presto/shlib/cnv/src/RCS/cnv.mk 1.1 1970/01/01 00:00:00
#*Log: cnv.mk
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#
#	<<< デバッグ情報の定義 >>>
#
DEBUG 	= 0
#DEBUG	= 1
#
#	<<< エンディアンの定義 >>>
#
#ENDIAN	= 0
ENDIAN	= 1
#
#	<<< CPUの定義 >>>
#
#CPUTYPE = 2
CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=cnvsh3b
else
TARGET		=cnvsh3l
endif
else
CTYPE	= sh2
TARGET		=cnvsh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=cnvid
#
CANWUP		=canwup
CHGPRI		=chgpri
CREFLG		=creflg
CREMBX		=crembx
CRESEM		=cresem
CRETSK		=cretsk
CYCWUP		=cycwup
DEFEXT		=defext
DELFLG		=delflg
DELMBX		=delmbx
DELTSK		=deltsk
EXDTSK		=exdtsk
ERRCODE		=errcode
FLGADR		=flgadr
GETBLK		=getblk
HLTTSK		=hlttsk
IRETWUP		=iretwup
MBXADR		=mbxadr
MPLADR		=mpladr
RCVMSG		=rcvmsg
RELBLK		=relblk
RSMTSK		=rsmtsk
SEMADR		=semadr
SETFLG		=setflg
SIGSEM		=sigsem
SLPTSK		=slptsk
SNDMSG		=sndmsg
STATSK		=statsk
SUSTSK		=sustsk
TCBADR		=tcbadr
WAIFLG		=waiflg
WAISEM		=waisem
WAITSK		=waitsk
WUPTSK		=wuptsk

#===========================================================
#
ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
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
OFILE0=$(ID).obj
OFILE1=$(CANWUP).obj $(CHGPRI).obj $(CREFLG).obj $(CREMBX).obj $(CRESEM).obj
OFILE2=$(CRETSK).obj $(CYCWUP).obj $(DEFEXT).obj $(DELFLG).obj $(DELMBX).obj
OFILE3=$(DELTSK).obj $(EXDTSK).obj $(ERRCODE).obj $(FLGADR).obj $(GETBLK).obj $(HLTTSK).obj
OFILE4=$(IRETWUP).obj $(MBXADR).obj $(MPLADR).obj $(RCVMSG).obj $(RELBLK).obj
OFILE5=$(RSMTSK).obj $(SEMADR).obj $(SETFLG).obj $(SIGSEM).obj $(SLPTSK).obj
OFILE6=$(SNDMSG).obj $(STATSK).obj $(SUSTSK).obj $(TCBADR).obj $(WAIFLG).obj
OFILE7=$(WAISEM).obj $(WAITSK).obj $(WUPTSK).obj
OFILES=$(OFILE0) $(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5) $(OFILE6) $(OFILE7)
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
$(CANWUP).obj : $(CANWUP).c
	$(CC) $(CANWUP).c			>>  $(ERRFILE)
$(CHGPRI).obj : $(CHGPRI).c
	$(CC) $(CHGPRI).c			>>  $(ERRFILE)
$(CREFLG).obj : $(CREFLG).c
	$(CC) $(CREFLG).c			>>  $(ERRFILE)
$(CREMBX).obj : $(CREMBX).c
	$(CC) $(CREMBX).c			>>  $(ERRFILE)
$(CRESEM).obj : $(CRESEM).c
	$(CC) $(CRESEM).c			>>  $(ERRFILE)
$(CRETSK).obj : $(CRETSK).c
	$(CC) $(CRETSK).c			>>  $(ERRFILE)
$(CYCWUP).obj : $(CYCWUP).c
	$(CC) $(CYCWUP).c			>>  $(ERRFILE)
$(DEFEXT).obj : $(DEFEXT).c
	$(CC) $(DEFEXT).c			>>  $(ERRFILE)
$(DELFLG).obj : $(DELFLG).c
	$(CC) $(DELFLG).c			>>  $(ERRFILE)
$(DELMBX).obj : $(DELMBX).c
	$(CC) $(DELMBX).c			>>  $(ERRFILE)
$(DELTSK).obj : $(DELTSK).c
	$(CC) $(DELTSK).c			>>  $(ERRFILE)
$(EXDTSK).obj : $(EXDTSK).c
	$(CC) $(EXDTSK).c			>>  $(ERRFILE)
$(ERRCODE).obj : $(ERRCODE).c
	$(CC) $(ERRCODE).c			>>  $(ERRFILE)
$(FLGADR).obj : $(FLGADR).c
	$(CC) $(FLGADR).c			>>  $(ERRFILE)
$(GETBLK).obj : $(GETBLK).c
	$(CC) $(GETBLK).c			>>  $(ERRFILE)
$(HLTTSK).obj : $(HLTTSK).c
	$(CC) $(HLTTSK).c			>>  $(ERRFILE)
$(IRETWUP).obj : $(IRETWUP).c
	$(CC) $(IRETWUP).c			>>  $(ERRFILE)
$(MBXADR).obj : $(MBXADR).c
	$(CC) $(MBXADR).c			>>  $(ERRFILE)
$(MPLADR).obj : $(MPLADR).c
	$(CC) $(MPLADR).c			>>  $(ERRFILE)
$(RCVMSG).obj : $(RCVMSG).c
	$(CC) $(RCVMSG).c			>>  $(ERRFILE)
$(RELBLK).obj : $(RELBLK).c
	$(CC) $(RELBLK).c			>>  $(ERRFILE)
$(RSMTSK).obj : $(RSMTSK).c
	$(CC) $(RSMTSK).c			>>  $(ERRFILE)
$(SEMADR).obj : $(SEMADR).c
	$(CC) $(SEMADR).c			>>  $(ERRFILE)
$(SETFLG).obj : $(SETFLG).c
	$(CC) $(SETFLG).c			>>  $(ERRFILE)
$(SIGSEM).obj : $(SIGSEM).c
	$(CC) $(SIGSEM).c			>>  $(ERRFILE)
$(SLPTSK).obj : $(SLPTSK).c
	$(CC) $(SLPTSK).c			>>  $(ERRFILE)
$(SNDMSG).obj : $(SNDMSG).c
	$(CC) $(SNDMSG).c			>>  $(ERRFILE)
$(STATSK).obj : $(STATSK).c
	$(CC) $(STATSK).c			>>  $(ERRFILE)
$(SUSTSK).obj : $(SUSTSK).c
	$(CC) $(SUSTSK).c			>>  $(ERRFILE)
$(TCBADR).obj : $(TCBADR).c
	$(CC) $(TCBADR).c			>>  $(ERRFILE)
$(WAIFLG).obj : $(WAIFLG).c
	$(CC) $(WAIFLG).c			>>  $(ERRFILE)
$(WAISEM).obj : $(WAISEM).c
	$(CC) $(WAISEM).c			>>  $(ERRFILE)
$(WAITSK).obj : $(WAITSK).c
	$(CC) $(WAITSK).c			>>  $(ERRFILE)
$(WUPTSK).obj : $(WUPTSK).c
	$(CC) $(WUPTSK).c			>>  $(ERRFILE)

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

