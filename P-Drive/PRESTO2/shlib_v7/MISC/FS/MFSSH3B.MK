#-----------------------------------------------------------------------#
# File name	: mfssh3b.mk	/ Romable liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/misc/fs/rcs/mfssh3b.mk 1.3 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: mfssh3b.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* FSをWCUでも使用する(FlashX,chip_erasex)
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* 2003/11/11 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* ファイル・システムとFLASH書込み
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 20-OCT-02 : 						#
#			      / Initial codeing				#
#-----------------------------------------------------------------------#
#
#	<<< デバッグ情報の定義 >>>
#
DEBUG 	= 0
#DEBUG	= 1
#
#	<<< エンディアンの定義 >>>
#
#ENDIAN	= 0
ENDIAN	= 1
#
#	<<< CPUの定義 >>>
#
#CPUTYPE = 2
CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=mfssh3b
else
TARGET		=mfssh3l
endif
else
CTYPE	= sh2
TARGET		=mfssh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=mfsid

#===========================================================
#
ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SHC_INC)
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#===========================================================
#
#	<<< ライブラリーの作成 >>>
#
OFILES := mfsid.obj fsys.obj date.obj biosfile.obj dos_util.obj lowsrc.obj flashf.obj

#
#
#all : clean $(TARGET).lib
all : $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj	: $(ID).c
	$(CC) $(ID).c				>  $(ERRFILE)
fsys.obj	: fsys.c
	$(CC) fsys.c			>> $(ERRFILE)
date.obj	: date.c
	$(CC) date.c			>> $(ERRFILE)
biosfile.obj	: biosfile.c
	$(CC) biosfile.c			>> $(ERRFILE)
dos_util.obj	: dos_util.c
	$(CC) dos_util.c			>> $(ERRFILE)
lowsrc.obj	: lowsrc.c
	$(CC) lowsrc.c			>> $(ERRFILE)
flashf.obj	: flashf.c
	$(CC) flashf.c			>> $(ERRFILE)

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
#	ＭＡＫＥコマンドの定義						#
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

