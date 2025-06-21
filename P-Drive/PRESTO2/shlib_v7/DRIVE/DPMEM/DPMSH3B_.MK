#-----------------------------------------------------------------------#
# File name	: dpmsh3b_.mk	/ dp-sram driver liblary		#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmsh3b_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: dpmsh3b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/dpmem/RCS/dpmdrv.mk 1.1 1970/01/01
#*Log: dpmdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	DP-SRAM関連ドライバーのライブラリーを作成する。			#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 11-JUL-93 : 株本 隆司					#
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
TARGET		=ddpmsh3b_
else
TARGET		=ddpmsh3l_
endif
else
CTYPE	= sh2
TARGET		=ddpmsh2b_
endif
#
#	<<< ファイルネーム定義 >>>

MAKEFILE	=dpmsh3b_.mk

ID		=ddpmid

DPMSYINI	=dpmsini
DPMEMOPN	=dpmopn
DPMEMCLS	=dpmcls
DPMEMRD		=dpmrd
DPMEMWR		=dpmwr
DPM0INT		=dpm0int
DPM1INT		=dpm1int

#	<<< ヘッダー･ファイル定義 >>>

DPMCTL		=dpmctl
DPMCTXT		=dpmctxt
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

OFILES	=$(ID).obj $(DPMSYINI).obj $(DPMEMOPN).obj $(DPMEMCLS).obj\
	 $(DPMEMWR).obj $(DPMEMRD).obj $(DPM0INT).obj $(DPM1INT).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c			>  x
$(DPMSYINI).obj : $(DPMSYINI).c
	$(CC) $(DPMSYINI).c		>> x
$(DPMEMOPN).obj : $(DPMEMOPN).c
	$(CC) $(DPMEMOPN).c		>> x
$(DPMEMCLS).obj : $(DPMEMCLS).c
	$(CC) $(DPMEMCLS).c		>> x
$(DPMEMRD).obj	: $(DPMEMRD).c
	$(CC) $(DPMEMRD).c		>> x
$(DPMEMWR).obj	: $(DPMEMWR).c
	$(CC) $(DPMEMWR).c		>> x
$(DPM0INT).obj	: $(DPM0INT).c
	$(CC) $(DPM0INT).c		>> x
$(DPM1INT).obj	: $(DPM1INT).c
	$(CC) $(DPM1INT).c		>> x

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
#			ＭＡＫＥコマンドの定義				#
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
