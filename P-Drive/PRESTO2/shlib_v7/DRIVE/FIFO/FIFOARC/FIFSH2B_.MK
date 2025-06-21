#-----------------------------------------------------------------------#
# File name	: fifdrv.mk	/ fifo driver liblary			#
#* Original	: p:/presto/shlib/drive/scu/RCS/scudrv.mk 1.1 
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fifo/fifoarc/rcs/fifsh2b_.mk 1.1 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: fifsh2b_.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* 初期レビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 1993, ISHIDA  CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	FIFO関連ドライバーのライブラリーを作成する。			#
#-----------------------------------------------------------------------#
#	<<< デバッグ情報の定義 >>>
#DEBUG 	= 0
DEBUG	= 1
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
TARGET		=dfifsh3b_
else
TARGET		=dfifsh3l_
endif
else
CTYPE	= sh2
TARGET		=dfifsh2b_
endif
#
#	<<< ファイルネーム定義 >>>

MAKEFILE =fifsh2b_.mk

ID		=dfifid
#
FIFSYINI	=fifsini
FIFOPEN		=fifopn
FIFCLOS		=fifcls
FIFRX		=fifrx
FIFTX		=fiftx

FIFINT		=fifint

FIFIOTX		=fifiotx


#	<<< ヘッダー･ファイル定義 >>>

#FIF		=fifa
FIFCTL		=fifctl
FIFCTXT		=fifctxt
#===========================================================
#
#	<<< コマンド定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=big -me -debug -comment=nest -l -def=DEBUG,CPUSH2
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=big -me -nodebug -comment=nest -l -def=NDEBUG,CPUSH2
endif

INCLIST := .
INCLIST := $(INCLIST),$(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#===========================================================
#	<<< ライブラリーの作成 >>>

OFILES	=$(ID).obj $(FIFSYINI).obj $(FIFOPEN).obj $(FIFCLOS).obj $(FIFRX).obj\
	  $(FIFTX).obj $(FIFIOTX).obj $(FIFINT).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c			> x
$(FIFSYINI).obj : $(FIFSYINI).c
	$(CC) $(FIFSYINI).c		>>x
$(FIFOPEN).obj : $(FIFOPEN).c
	$(CC) $(FIFOPEN).c		>>x
$(FIFCLOS).obj : $(FIFCLOS).c
	$(CC) $(FIFCLOS).c		>>x
$(FIFRX).obj : $(FIFRX).c
	$(CC) $(FIFRX).c		>>x
$(FIFTX).obj : $(FIFTX).c
	$(CC) $(FIFTX).c		>>x
$(FIFIOTX).obj : $(FIFIOTX).c
	$(CC) $(FIFIOTX).c		>>x
$(FIFINT).obj : $(FIFINT).c
	$(CC) $(FIFINT).c		>>x

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
