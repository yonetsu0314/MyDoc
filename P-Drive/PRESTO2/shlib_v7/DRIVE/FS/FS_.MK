#----------------------------------------------------------------------*
# File name	: cf_.mk                                                    *
#* Original	: p:/dacs_i/shdsplib/drive/fs/RCS/fs.mk 1.1
#----------------------------------------------------------------------*
#$Header: p:/presto2/shlib_v7/drive/fs/rcs/fs_.mk 1.3 1970/01/01 00:00:00 sakaguti Exp $
#$Log: fs_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* dfsid.c追加
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/11/12 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* ファイル・システム
#* 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.                           *
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       *
#		PHONE (077) 553-4141                                           *
#----------------------------------------------------------------------*/
#

TARGET	=_fs_

MAKEFILE =fs_.mk

INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc
INC5 = P:\PRESTO\shlib_v7\DRIVE\USNETS\USNETSH7\INCLUDE


CFLAGS	= -i=$(INC1),$(INC2),$(INC3),$(INC4),$(INC5)

#CCOPT	= -cpu=sh2 -op=1 -comment=nest
CCOPT	= -cpu=sh2 -op=0 -debug -comment=nest
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

OFILES	=	fs_ram.obj\
			fs_ata.obj\
			dfsid.obj\

all : clean $(TARGET).lib

fs_ram.obj : fs_ram.c
	$(CC) fs_ram.c >> x
fs_ata.obj : fs_ata.c
	$(CC) fs_ata.c >> x
dfsid.obj : dfsid.c
	$(CC) dfsid.c >> x


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
