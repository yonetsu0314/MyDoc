#-----------------------------------------------------------------------#
# File name	: libata_.mk
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/libata_.mk 1.1 1970/01/01 00:00:00 kagatume Exp $
#*$Log: libata_.mk $
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
PCATADIR	= $(PRJ_HOME)\PCATA
FATINCDIR	= $(INC_BASE)\DRIVE\FSSVR60

#-------------------------------------------------------#
#   Headers
#-------------------------------------------------------#
INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(FATINCDIR)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

HEADERS = 	$(PCATADIR)\ata_def.h	\
			$(PCATADIR)\ata_prot.h	\
			$(PCATADIR)\ata_srct.h

#-------------------------------------------------------#
#   Compaile, Asemble
#-------------------------------------------------------#
CCFLAGS	= -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -i=$(INCLIST)
CAFLAGS = -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -code=asmcode -i=$(INCLIST)
ASFLAGS = -cp=$(CPU) -en=$(ENDIAN) $(DEBUG_INFO) -i=$(INCLIST)

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
MKCFG_FILE	= libata.mk

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
TARGET	= libata_

#-------------------------------------------------------#
#   Object file
#-------------------------------------------------------#
OBJS =	ata_main.obj		\
		ata_io.obj		\
		ata_os.obj	\
		ata_sub.obj		\
		ata_ram.obj		\
		ata_cmd.obj

#-------------------------------------------------------#
#   Create FAT library file
#-------------------------------------------------------#
#all : $(TARGET).lib
all : clean $(TARGET).lib

ata_main.obj: ata_main.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_main.c				> x

ata_io.obj: ata_io.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_io.c					>> x

ata_os.obj: ata_os.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_os.c				>> x

ata_sub.obj: ata_sub.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_sub.c				>> x

ata_ram.obj: ata_ram.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_ram.c				>> x

ata_cmd.obj: ata_cmd.c $(HEADERS)
		$(CC) $(CCFLAGS) ata_cmd.c					>> x

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

