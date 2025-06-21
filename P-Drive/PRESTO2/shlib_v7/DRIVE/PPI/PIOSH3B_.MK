#-----------------------------------------------------------------------#
# File name	: piosh3b_.mk	/ pio driver liblary			#
#		  SH3 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/ppi/RCS/piosh3b_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: piosh3b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/ppi/RCS/piodrv.mk 1.1 1970/01/01 00:00:00
#*Log: piodrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	PIO関連ドライバーのライブラリーを作成する。			#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 16-MAR-93 : 株本 隆司					#
#			      / Initial codeing				#
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
TARGET		=dppish3b_
else
TARGET		=dppish3l_
endif
else
CTYPE	= sh2
TARGET		=dppish2b_
endif
#
#	<<< ファイルネーム定義 >>>
MAKEFILE	=piosh3b_.mk

ID		=dppiid

PIOSINI		=piosini
CENTRO		=centro

SIG0INT		=sig0int
SIG1INT		=sig1int
SIG2INT		=sig2int
SIG3INT		=sig3int
SIG4INT		=sig4int
SIG5INT		=sig5int

#	<<< ヘッダー･ファイル定義 >>>

PIOCTL		=pioctl
PIOCTXT		=pioctxt

#===========================================================
#	<<< コマンド定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -def=DEBUG
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
OFILES	=$(ID).obj $(PIOSINI).obj $(CENTRO).obj $(SIG0INT).obj $(SIG1INT).obj\
	 $(SIG2INT).obj $(SIG3INT).obj $(SIG4INT).obj $(SIG5INT).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c			>  x
$(PIOSINI).obj	: $(PIOSINI).c
	$(CC) $(PIOSINI).c		>> x
$(CENTRO).obj	: $(CENTRO).c
	$(CC) $(CENTRO).c		>> x
$(SIG0INT).obj	: $(SIG0INT).c
	$(CC) $(SIG0INT).c		>> x
$(SIG1INT).obj	: $(SIG1INT).c
	$(CC) $(SIG1INT).c		>> x
$(SIG2INT).obj	: $(SIG2INT).c
	$(CC) $(SIG2INT).c		>> x
$(SIG3INT).obj	: $(SIG3INT).c
	$(CC) $(SIG3INT).c		>> x
$(SIG4INT).obj	: $(SIG4INT).c
	$(CC) $(SIG4INT).c		>> x
$(SIG5INT).obj	: $(SIG5INT).c
	$(CC) $(SIG5INT).c		>>x

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
