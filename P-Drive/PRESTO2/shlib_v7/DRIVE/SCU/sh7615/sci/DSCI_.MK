#*==========================================================================
#*	Filename	: dsci_.MK
#* Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/dsci.mk 1.1
#*--------------------------------------------------------------------------
#*	Function	: Makefile
#*--------------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/drive/scu/sh7615/sci/RCS/dsci_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: dsci_.mk $
#* ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  chimura
#* 2004/09/01 compiler ver7 vup
#* 
#* ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
#* LOG MESSAGE
#* 
#*--------------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		TELEPHONE (077) 553-4141
#*--------------------------------------------------------------------------
TARGET	=_dsci_

MAKEFILE =dsci_.mk

INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc

CFLAGS	= -i=$(INC1),$(INC2),$(INC3),$(INC4)

#CCOPT	= -cpu=sh2 -op=1 -comment=nest
CCOPT	= -cpu=sh2 -op=0 -debug -comment=nest
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
#			‚l‚`‚j‚dƒRƒ}ƒ“ƒh‚Ì’è‹`										#
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
