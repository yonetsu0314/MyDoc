#----------------------------------------------------------------------*
# File name	: cf_.mk                                                    *
#----------------------------------------------------------------------*
#$Header: p:/presto2/shlib_v7/drive/cf/RCS/cf_.mk 1.3 1970/01/01 00:00:00 sakaguti Exp $
#$Log: cf_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* dcfid.c追加
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/11/12 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* LOG MESSAGE
#* 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.                           *
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       *
#		PHONE (077) 553-4141                                           *
#----------------------------------------------------------------------*/
#

TARGET	=_cf_

MAKEFILE =cf_.mk

INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc
INC5 = P:\PRESTO\shlib_v7\DRIVE\USNETS\USNETSH7\INCLUDE


CFLAGS	= -i=$(INC1),$(INC2),$(INC3),$(INC4),$(INC5)

#CCOPT	= -cpu=sh2 -op=1 -comment=nest
CCOPT	= -cpu=sh2 -op=0 -debug -me -comment=nest
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

OFILES	=	atadrv.obj\
			pcmcard.obj\
			pcmsock.obj\
			pcmtpl.obj\
			dcfid.obj\

all : clean $(TARGET).lib

atadrv.obj : atadrv.c
	$(CC) atadrv.c >> x
pcmcard.obj : pcmcard.c
	$(CC) pcmcard.c >> x
pcmsock.obj : pcmsock.c
	$(CC) pcmsock.c >> x
pcmtpl.obj : pcmtpl.c
	$(CC) pcmtpl.c >> x
dcfid.obj : dcfid.c
	$(CC) dcfid.c >> x


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

clean:
	if exist *.obj del *.obj
	if exist *.abs del *.abs
	if exist *.mot del *.mot
	if exist *.map del *.map
	if exist *.lis del *.lis
	if exist *.sub del *.sub

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
