#-----------------------------------------------------------------------#
# File name	: libram.mk
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fssvr60/ramdisk/RCS/libram.mk 1.1 1970/01/01 00:00:00 kagatume Exp $
#*$Log: libram.mk $
#*リビジョン 1.1  1970/01/01  00:00:00  kagatume
#*初期リビジョン
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
OPTIMIZE      = 1
LITTLE_ENDIAN = NO
DEBUG         = NO

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
RAMDIR		= $(PRJ_HOME)\RAMDISK
FATINCDIR	= $(INC_BASE)\DRIVE\FSSVR60

#-------------------------------------------------------#
#   Headers
#-------------------------------------------------------#
INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(FATINCDIR)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

HEADERS = 	$(RAMDIR)\ram_def.h		\
			$(RAMDIR)\ram_hw.h		\
			$(RAMDIR)\ram_prot.h	\
			$(RAMDIR)\ram_srct.h

#-------------------------------------------------------#
#   Compaile, Asemble
#-------------------------------------------------------#
CCFLAGS	= -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -i=$(INCLIST)
CAFLAGS = -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -code=asmcode -i=$(INCLIST)
ASFLAGS = -cp=$(CPU) -en=$(ENDIAN) $(DEBUG_INFO) -i=$(INCLIST)

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
MKCFG_FILE	= libram.mk

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
TARGET	= libram

#-------------------------------------------------------#
#   Object file
#-------------------------------------------------------#
OBJS =	ram_main.obj	\
		ram_io.obj		\
		ram_os.obj		\
		ram_sub.obj		\
		ram_ram.obj

#-------------------------------------------------------#
#   Create FAT library file
#-------------------------------------------------------#
#all : $(TARGET).lib
all : clean $(TARGET).lib

ram_main.obj: ram_main.c $(HEADERS)
		$(CC) $(CCFLAGS) ram_main.c					> x

ram_io.obj: ram_io.c $(HEADERS)
		$(CC) $(CCFLAGS) ram_io.c					>> x

ram_os.obj: ram_os.c $(HEADERS)
		$(CC) $(CCFLAGS) ram_os.c					>> x

ram_sub.obj: ram_sub.c $(HEADERS)
		$(CC) $(CCFLAGS) ram_sub.c					>> x

ram_ram.obj: ram_ram.c $(HEADERS)
		$(CC) $(CCFLAGS) ram_ram.c					>> x

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
#	ＭＡＫＥコマンドの定義
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

