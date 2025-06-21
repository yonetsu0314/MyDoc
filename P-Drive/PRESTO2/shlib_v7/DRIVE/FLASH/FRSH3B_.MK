#-----------------------------------------------------------------------#
# File name	: frsh3b_.mk	/ flash rom driver liblary		#
#		  SH3 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/flash/rcs/frsh3b_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: frsh3b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/flash/RCS/frdrv.mk 1.1 1970/01/01
#*Log: frdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	FLASH ROM関連ドライバーのライブラリーを作成する。		#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 16-MAR-93 : 株本 隆司					#
#			      / Initial codeing				#
#-----------------------------------------------------------------------#
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
TARGET		=dfrsh3b_
else
TARGET		=dfrsh3l_
endif
else
CTYPE	= sh2
TARGET		=dfrsh2b_
endif
#
#	<<< ファイルネーム定義 >>>

MAKEFILE	=frsh3b_.mk

ID		=dfrid

FRSINIT		=frsini
FROPEN		=fropen
FRCLOSE		=frclos
FRWRITE		=frwrite
FRREAD		=frread
FRSEEK		=frseek
FRWRTSK		=frwrtsk

#	<<< ヘッダー･ファイル定義 >>>

CONTEXT		=frctxt
#===========================================================

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
#
OFILES	=$(ID).obj $(FRSINIT).obj $(FROPEN).obj $(FRCLOSE).obj\
	 $(FRWRITE).obj $(FRREAD).obj $(FRSEEK).obj $(FRWRTSK).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c				> x
$(FRSINIT).obj : $(FRSINIT).c $(CONTEXT).h
	$(CC) $(FRSINIT).c			>> x
$(FROPEN).obj : $(FROPEN).c
	$(CC) $(FROPEN).c			>> x
$(FRCLOSE).obj : $(FRCLOSE).c
	$(CC) $(FRCLOSE).c			>> x
$(FRWRITE).obj : $(FRWRITE).c
	$(CC) $(FRWRITE).c			>> x
$(FRREAD).obj : $(FRREAD).c $(CONTEXT).h
	$(CC) $(FRREAD).c			>> x
$(FRSEEK).obj : $(FRSEEK).c
	$(CC) $(FRSEEK).c			>> x
$(FRWRTSK).obj : $(FRWRTSK).c $(CONTEXT).h
	$(CC) $(FRWRTSK).c			>> x

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
