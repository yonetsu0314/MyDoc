#-----------------------------------------------------------------------#
# File name	: vdpsh3b_.mk	/ virtual dp-sram driver liblary	#
#		  SH3 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpsh3b_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: vdpsh3b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/vdpm/RCS/vdpmdrv.mk 1.1 1970/01/01 00:00:00
#*Log: vdpmdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1993, ISHIDA  CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	Virtual DP-SRAM関連ドライバーのライブラリーを作成する。		#
#-----------------------------------------------------------------------#
#
#	<<< デバッグ情報の定義 >>>
#
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
TARGET		=dvdpsh3b_
else
TARGET		=dvdpsh3l_
endif
else
CTYPE	= sh2
TARGET		=dvdpsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
MAKEFILE	=vdpsh3b_.mk

ID		=dvdpmid

DPMSYINI	=vdpmsini
DPMEMRD		=vdpmrd
DPMEMWR		=vdpmwr

#	<<< ヘッダー･ファイル定義 >>>

DPMCTL		=vdpmctl
DPMCTXT		=vdpmctxt
#===========================================================
#	<<< コマンド定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#	<<< ライブラリーの作成 >>>

OFILES	=$(ID).obj $(DPMSYINI).obj $(DPMEMWR).obj $(DPMEMRD).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c			> x
$(DPMSYINI).obj : $(DPMSYINI).c
	$(CC) $(DPMSYINI).c		>> x
$(DPMEMRD).obj	: $(DPMEMRD).c
	$(CC) $(DPMEMRD).c		>> x
$(DPMEMWR).obj	: $(DPMEMWR).c
	$(CC) $(DPMEMWR).c		>> x

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

clean :
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
