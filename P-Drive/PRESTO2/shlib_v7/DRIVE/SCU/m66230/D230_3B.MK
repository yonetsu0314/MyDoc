#-----------------------------------------------------------------------#
# File name	: m66230.mk	/ scu driver liblary							#
#		  SH3 ビッグエンディアン用										#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/d230_3b.mk 1.1 1970/01/01 00:00:00 chimura Exp $
#*$Log: d230_3b.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 1993, ISHIDA  CO., LTD.							#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							#
#		SHIGA JAPAN														#
#		(0775) 53-4141													#
#-----------------------------------------------------------------------#
# Function																#
#	SH3内臓SCU関連ドライバーのライブラリーを作成する。					#
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
TARGET		=d230_3b
else
TARGET		=d230_3l
endif
else
CTYPE	= sh2
TARGET		=d230_2b
endif
#
#	<<< ファイルネーム定義 >>>
MAKEFILE	=d230_3b.mk

ID		=d66230id

SCUSYINI	=scusini
SCUOPEN		=scuopn
SCUCLOS		=scucls
SCURX		=scurx
SCUTX		=scutx

SCUINT		=scuint

SCUIOTX		=sciotx

#	<<< ヘッダー･ファイル定義 >>>

SCU			=scua
SCUCTL		=scuctl
SCUCTXT		=scuctxt
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

#===========================================================
#	<<< ライブラリーの作成 >>>

OFILES	=$(ID).obj $(SCUSYINI).obj $(SCUOPEN).obj $(SCUCLOS).obj $(SCURX).obj\
	  $(SCUTX).obj $(SCUIOTX).obj $(SCUINT).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c							> x
$(SCUSYINI).obj : $(SCUSYINI).c
	$(CC) $(SCUSYINI).c						>>x
$(SCUOPEN).obj : $(SCUOPEN).c
	$(CC) $(SCUOPEN).c						>>x
$(SCUCLOS).obj : $(SCUCLOS).c
	$(CC) $(SCUCLOS).c						>>x
$(SCURX).obj : $(SCURX).c
	$(CC) $(SCURX).c						>>x
$(SCUTX).obj : $(SCUTX).c
	$(CC) $(SCUTX).c						>>x
$(SCUIOTX).obj : $(SCUIOTX).c
	$(CC) $(SCUIOTX).c						>>x
$(SCUINT).obj : $(SCUINT).c
	$(CC) $(SCUINT).c						>>x

$(TARGET).lib : $(OFILES) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(OPTLNK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib ..\*.*

$(TARGET).sub :
	echo	FORM	= library				>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib			>>$(TARGET).sub
	echo	INPUT	= $(OFILES)				>>$(TARGET).sub
	echo	LIST	= $(TARGET)				>>$(TARGET).sub
	echo	EXIT							>>$(TARGET).sub

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
