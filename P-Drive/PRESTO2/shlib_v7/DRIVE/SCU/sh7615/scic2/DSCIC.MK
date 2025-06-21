#*==========================================================================
#*	Filename	: dscic.MK
#* Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/dsci.mk 1.1
#*--------------------------------------------------------------------------
#*	Function	: Makefile
#*--------------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/drive/scu/sh7615/scic2/RCS/dscic.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: dscic.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2008/02/15
#* ライブラリー名を dscic2.lib に変更する。
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*--------------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		TELEPHONE (077) 553-4141
#*--------------------------------------------------------------------------
TARGET	=_dscic2

MAKEFILE =dscic.mk

INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc

CFLAGS	= -i=$(INC1),$(INC2),$(INC3),$(INC4)

CCOPT	= -cpu=sh2 -op=1 -comment=nest
#CCOPT	= -cpu=sh2 -op=0 -debug -comment=nest
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

OFILES	=	dsciid.obj\
			scisini.obj\
			sciopn.obj\
			scicls.obj\
			scirx.obj\
			scitx.obj\
			sciotx.obj\
			sciint.obj

all : clean $(TARGET).lib

dsciid.obj : dsciid.c
	$(CC) dsciid.c > x
scisini.obj : scisini.c
	$(CC) scisini.c >> x
sciopn.obj : sciopn.c
	$(CC) sciopn.c >> x
scicls.obj : scicls.c
	$(CC) scicls.c >> x
scirx.obj : scirx.c
	$(CC) scirx.c >> x
scitx.obj : scitx.c
	$(CC) scitx.c >> x
sciotx.obj : sciotx.c
	$(CC) sciotx.c >> x
sciint.obj : sciint.c
	$(CC) sciint.c >> x

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
