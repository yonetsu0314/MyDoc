#-----------------------------------------------------------------------#
# File name	: wcu5561.mk	/ Romable liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/misc/wcu5561/rcs/wcu5561.mk 1.3 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: wcu5561.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* FLASH用関数をBIOSから呼ぶ
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* bootmisc.c追加
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* 初期リビジョン
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
CPUTYPE = 2
#CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=wcush3b
else
TARGET		=wcush3l
endif
else
CTYPE	= sh2
TARGET		=wcu5561
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=wcuid

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
OFILES := wcuid.obj fpgaio.obj bootmisc.obj flash.obj

#
#
all : clean $(TARGET).lib
#all : $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj	: $(ID).c
	$(CC) $(ID).c			>  $(ERRFILE)
fpgaio.obj	: fpgaio.c fpgaio.h
	$(CC) fpgaio.c			>> $(ERRFILE)
bootmisc.obj	: bootmisc.c
	$(CC) bootmisc.c			>> $(ERRFILE)
flash.obj	: flash.c
	$(CC) flash.c			>> $(ERRFILE)

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

