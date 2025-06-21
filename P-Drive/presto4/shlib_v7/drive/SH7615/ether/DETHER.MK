#----------------------------------------------------------------------*
# File name	: dether.mk                                                *
#----------------------------------------------------------------------*
#* Origina	: p:/dacs_i/shdsplib/drive/sh7615/ether/RCS/dether.mk 1.1
#$Header: p:/presto2/shlib_v7/drive/sh7615/ether/rcs/dether.mk 1.4 1970/01/01 00:00:00 sakaguti Exp $
#$Log: dether.mk $
#* リビジョン 1.4  1970/01/01  00:00:00  sakaguti
#* dethid.c追加
#* 
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/11/12 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* LOG MESSAGE
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

TARGET	=_dether

MAKEFILE =dether.mk

INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc
INC5 = P:\PRESTO\shlib_v7\DRIVE\USNETS\USNETSH7\INCLUDE


CFLAGS	= -i=$(INC1),$(INC2),$(INC3),$(INC4),$(INC5)

CCOPT	= -cpu=sh2 -op=1 -comment=nest
#CCOPT	= -cpu=sh2 -op=0 -debug -comment=nest
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

OFILES	=	eth_init.obj\
			lan_init.obj\
			dethid.obj\

all : clean $(TARGET).lib

dethid.obj : dethid.c
	$(CC) dethid.c
eth_init.obj : eth_init.c
	$(CC) eth_init.c
lan_init.obj : lan_init.c
	$(CC) lan_init.c


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
	if exist *.lbp del *.lbp
	if exist *.lst del *.lst
	if exist *.sub del *.sub
	if exist *.lib del *.lib

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
