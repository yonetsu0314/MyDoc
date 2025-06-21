#*==========================================================================
#*	Filename	: de2sr_.MK
#* Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/de2sr.mk 1.1
#*--------------------------------------------------------------------------
#*	Function	: Makefile
#*--------------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/de2sr_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: de2sr_.mk $
#* ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  chimura
#* 2003/11/12 compiler ver7 vup
#* 
#* ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
#* LOG MESSAGE
#* 
#*--------------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		TELEPHONE (077) 553-4141
#*--------------------------------------------------------------------------
TARGET	=de2sr_

MAKEFILE =de2sr_.mk

INCLIST := .
INCLIST := $(INCLIST),$(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CCOPT	= -cpu=sh2 -op=0 -me -debug -comment=nest
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

OFILES	=	de2srid.obj\
			e2ssini.obj\
			e2sopen.obj\
			e2sclos.obj\
			e2swr.obj\
			e2srd.obj\
			e2sseek.obj\
			e2scmd.obj\
			e2siowr.obj

all : clean $(TARGET).lib

de2srid.obj : de2srid.c
	$(CC) de2srid.c > x
e2ssini.obj : e2ssini.c
	$(CC) e2ssini.c >> x
e2sopen.obj : e2sopen.c
	$(CC) e2sopen.c >> x
e2sclos.obj : e2sclos.c
	$(CC) e2sclos.c >> x
e2swr.obj : e2swr.c
	$(CC) e2swr.c >> x
e2srd.obj : e2srd.c
	$(CC) e2srd.c >> x
e2sseek.obj : e2sseek.c
	$(CC) e2sseek.c >> x
e2scmd.obj : e2scmd.c
	$(CC) e2scmd.c >> x
e2siowr.obj : e2siowr.c
	$(CC) e2siowr.c >> x

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
	if exist *.lbp del *.lbp
	if exist *.lst del *.lst
	if exist *.sub del *.sub
	if exist *.lib del *.lib

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
