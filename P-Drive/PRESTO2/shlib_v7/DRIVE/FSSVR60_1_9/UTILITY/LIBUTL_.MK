#-----------------------------------------------------------------------#
# File name	: libutl_.mk
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/RCS/libutl_.mk 1.1 1970/01/01 00:00:00 kagatume Exp $
#*$Log: libutl_.mk $
#*ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  kagatume
#*‰ŠúƒŠƒrƒWƒ‡ƒ“
#*
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
#----------------------------------------------------------#
# Configuration 
#-------------------------------------------------------#
CPU           = SH2
SHC_VER       = 7
OPTIMIZE      = 0
LITTLE_ENDIAN = NO
DEBUG         = YES

ifeq ($(LITTLE_ENDIAN),YES)
ENDIAN	= l
else
ENDIAN	= b
endif

ifeq ($(DEBUG),YES)
DEBUG_INFO = -deb
else
DEBUG_INFO = -nodeb
endif

#-------------------------------------------------------#
# Directory path
#-------------------------------------------------------#
PRJ_HOME	= ..

LIBDIR		= $(PRJ_HOME)\lib
UTLDIR		= $(PRJ_HOME)\UTILITY
UTLAPIDIR	= $(UTLDIR)\API
UTLCOMDIR	= $(UTLDIR)\COMMON
UTLOSDIR	= $(UTLDIR)\OS\ITRON
FATINCDIR	= $(INC_BASE)\DRIVE\FSSVR60

#-------------------------------------------------------#
#   Headers
#-------------------------------------------------------#
INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(FATINCDIR)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

HEADERS	=	$(FATINCDIR)\fsuser.h		\
			$(FATINCDIR)\fsdef.h		\
			$(FATINCDIR)\fsdrvdef.h	\
			$(FATINCDIR)\fsstruct.h	\
			$(FATINCDIR)\fstype.h		\
			$(FATINCDIR)\fsmacro.h	\
			$(FATINCDIR)\utility.h	\
			$(FATINCDIR)\powfail.h

#-------------------------------------------------------#
#   Compaile, Asemble
#-------------------------------------------------------#
CCFLAGS	= -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -i=$(INCLIST)
CAFLAGS = -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -code=asmcode -i=$(INCLIST)
ASFLAGS = -cp=$(CPU) -en=$(ENDIAN) $(DEBUG_INFO) -i=$(INCLIST)

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
MKCFG_FILE	= $(FATDIR)\libutl.mk

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
CC			= shc
AS			= asmsh

ifeq ($(SHC_VER),5)
LB      	= lbr
LK			= lnk
else
LB      	= optlnk
LK			= optlnk
endif

#-------------------------------------------------------#
#   Kernel library file
#-------------------------------------------------------#
TARGET	= libutl_

#-------------------------------------------------------#
#   Object file
#-------------------------------------------------------#
OBJS =		scandisk.obj			\
			badclst.obj			\
			dlerrlfn.obj	\
			frbitchk.obj			\
			frbitoff.obj			\
			frchaoff.obj	\
			gclstnum.obj			\
			gdirname.obj			\
			gfatrtry.obj	\
			gfsize.obj			\
			glstsctn.obj		\
			gtpclstn.obj		\
			makentry.obj			\
			scrpfile.obj		\
			pickclst.obj			\
			rdscrtry.obj		\
			errlog.obj			\
			lnerrlog.obj		\
			scanentr.obj			\
			scnentrt.obj	\
			scnfdata.obj		\
			scanclst.obj			\
			sfatrtry.obj	\
			sfsize.obj			\
			stpclstn.obj		\
			wrscrtry.obj		\
			osrtwait.obj

#-------------------------------------------------------#
#   Create FAT library file
#-------------------------------------------------------#
#all : $(TARGET).lib
all : clean $(TARGET).lib

scandisk.obj :	$(UTLAPIDIR)\scandisk.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLAPIDIR)\scandisk.c				> x
	
badclst.obj :	$(UTLCOMDIR)\badclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\badclst.c				>> x
	
dlerrlfn.obj :	$(UTLCOMDIR)\dlerrlfn.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\dlerrlfn.c		>> x

frbitchk.obj:	$(UTLCOMDIR)\frbitchk.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\frbitchk.c				>> x

frbitoff.obj:	$(UTLCOMDIR)\frbitoff.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\frbitoff.c				>> x

frchaoff.obj:	$(UTLCOMDIR)\frchaoff.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\frchaoff.c		>> x

gclstnum.obj:	$(UTLCOMDIR)\gclstnum.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\gclstnum.c				>> x

gdirname.obj:	$(UTLCOMDIR)\gdirname.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\gdirname.c				>> x

gfatrtry.obj:	$(UTLCOMDIR)\gfatrtry.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\gfatrtry.c		>> x

gfsize.obj:	$(UTLCOMDIR)\gfsize.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\gfsize.c			>> x

glstsctn.obj:	$(UTLCOMDIR)\glstsctn.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\glstsctn.c			>> x

gtpclstn.obj:	$(UTLCOMDIR)\gtpclstn.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\gtpclstn.c			>> x

makentry.obj:	$(UTLCOMDIR)\makentry.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\makentry.c				>> x

scrpfile.obj:	$(UTLCOMDIR)\scrpfile.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\scrpfile.c			>> x

pickclst.obj:	$(UTLCOMDIR)\pickclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\pickclst.c				>> x

rdscrtry.obj:	$(UTLCOMDIR)\rdscrtry.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\rdscrtry.c			>> x

errlog.obj:	$(UTLCOMDIR)\errlog.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\errlog.c				>> x

lnerrlog.obj:	$(UTLCOMDIR)\lnerrlog.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\lnerrlog.c			>> x

scanentr.obj:	$(UTLCOMDIR)\scanentr.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\scanentr.c				>> x

scnentrt.obj:	$(UTLCOMDIR)\scnentrt.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\scnentrt.c		>> x

scnfdata.obj:	$(UTLCOMDIR)\scnfdata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\scnfdata.c			>> x

scanclst.obj:	$(UTLCOMDIR)\scanclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\scanclst.c				>> x

sfatrtry.obj:	$(UTLCOMDIR)\sfatrtry.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\sfatrtry.c		>> x

sfsize.obj:	$(UTLCOMDIR)\sfsize.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\sfsize.c				>> x

stpclstn.obj:	$(UTLCOMDIR)\stpclstn.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\stpclstn.c			>> x

wrscrtry.obj:	$(UTLCOMDIR)\wrscrtry.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLCOMDIR)\wrscrtry.c			>> x

osrtwait.obj:	$(UTLOSDIR)\osrtwait.c $(HEADERS)
		$(CC) $(CCFLAGS) $(UTLOSDIR)\osrtwait.c				>> x

$(TARGET).lib : $(OBJS) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(LK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib $(LIBDIR)\*.*

$(TARGET).sub :
	echo	FORM	= library		>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib >>$(TARGET).sub
	echo	INPUT	= $(OBJS)		>>$(TARGET).sub
	echo	LIST	= $(TARGET)		>>$(TARGET).sub
	echo	EXIT					>>$(TARGET).sub

#-----------------------------------------------------------------------#
#	‚l‚`‚j‚dƒRƒ}ƒ“ƒh‚Ì’è‹`
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

