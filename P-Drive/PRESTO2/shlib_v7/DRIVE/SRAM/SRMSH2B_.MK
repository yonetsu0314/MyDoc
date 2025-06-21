#-----------------------------------------------------------------------#
# File name	: srmsh2b_.mk	/ sram driver liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/sram/RCS/srmsh2b_.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: srmsh2b_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* dsrmsh2b.libにデバッグ情報を付加する
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/sram/RCS/sramdrv.mk 1.1 1970/01/01 00:00:00
#*Log: sramdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	SRAM関連ドライバーのライブラリーを作成する。			#
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
TARGET		=dsrmsh3b_
else
TARGET		=dsrmsh3l_
endif
else
CTYPE	= sh2
TARGET		=dsrmsh2b_
endif

#	<<< ファイルネーム定義 >>>
MAKEFILE	=srmsh2b_.mk

ID		=dsramid

SRAMSINI	=sramsini
SRAMOPN		=sramopn
SRAMCLS		=sramcls
SRAMSK		=sramsk
SRAMRD		=sramrd
SRAMWR		=sramwr

#	<<< ヘッダー･ファイル定義 >>>

SRAMCTL		=sramctl
SRAMCTXT	=sramctxt
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

OFILES	=$(ID).obj $(SRAMSINI).obj $(SRAMOPN).obj $(SRAMCLS).obj\
	 $(SRAMWR).obj $(SRAMRD).obj $(SRAMSK).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c			> x
$(SRAMSINI).obj : $(SRAMSINI).c
	$(CC) $(SRAMSINI).c		>>x
$(SRAMOPN).obj : $(SRAMOPN).c
	$(CC) $(SRAMOPN).c		>>x
$(SRAMCLS).obj : $(SRAMCLS).c
	$(CC) $(SRAMCLS).c		>>x
$(SRAMRD).obj	: $(SRAMRD).c
	$(CC) $(SRAMRD).c		>>x
$(SRAMWR).obj	: $(SRAMWR).c
	$(CC) $(SRAMWR).c		>>x
$(SRAMSK).obj	: $(SRAMSK).c
	$(CC) $(SRAMSK).c		>>x

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
