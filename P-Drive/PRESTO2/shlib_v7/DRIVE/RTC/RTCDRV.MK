#----------------------------------------------------------------------*
# File name	: rtcdrv.mk											*
#----------------------------------------------------------------------*
#$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcdrv.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#$Log: rtcdrv.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/11/12 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* リアルタイム・クロック
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* リアルタイム・クロック
#* 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.			*
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN	*
#		PHONE (077) 553-4141					*
#----------------------------------------------------------------------*/
#
TARGET		=drtc
ID		=drtcid

RTCSINI	=rtcsini
RTCSDT	=rtcsdt
RTCGDT	=rtcgdt
RTCSTM	=rtcstm
RTCGTM	=rtcgtm

RTCCTL		=rtcctl

INCLIST := .
INCLIST := $(INCLIST),$(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CCOPT	= -cpu=sh2 -op=1 -nodebug -comment=nest
CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

HFILES	=$(RTCCTL).h

OFILES	=$(ID).obj $(RTCSINI).obj $(RTCSDT).obj $(RTCGDT).obj\
	 $(RTCSTM).obj $(RTCGTM).obj

all : clean $(TARGET).lib

$(ID).obj : $(ID).c
	$(CC) $(ID).c			> x
$(RTCSINI).obj : $(RTCSINI).c
	$(CC) $(RTCSINI).c		>>x
$(RTCSDT).obj : $(RTCSDT).c
	$(CC) $(RTCSDT).c		>>x
$(RTCGDT).obj : $(RTCGDT).c
	$(CC) $(RTCGDT).c		>>x
$(RTCSTM).obj : $(RTCSTM).c
	$(CC) $(RTCSTM).c		>>x
$(RTCGTM).obj : $(RTCGTM).c
	$(CC) $(RTCGTM).c		>>x

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
#			ＭＡＫＥコマンドの定義				#
#-----------------------------------------------------------------------#
DRV 	=p:

checkin :
	perl $(DRV)\tool\perl\checkin.pl *.mk	>  $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.h	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.c	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.asm	>> $(TARGET).bat

clean:
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


