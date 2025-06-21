#-----------------------------------------------------------------------#
# File name	: tmrsh3b_.mk	/ software timer driver liblary		#
#		  SH3 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/timer/RCS/tmrsh3b_.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: tmrsh3b_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/06/06 16:30
#* 誤記修正
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/timer/RCS/tmrdrv.mk 1.1 1970/01/01 00:00:00
#*Log: tmrdrv.mk
#-----------------------------------------------------------------------#
#*		(c) Copyright 2002, ISHIDA CO., LTD.			#
#*		959-1 SHIMOMAGARI RITTO-CITY				#
#*		SHIGA JAPAN						#
#*		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	Software Timer関連ドライバーのライブラリーを作成する。		#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 16-MAR-02 : 						#
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
TARGET		=dtmrsh3b_
else
TARGET		=dtmrsh3l_
endif
else
CTYPE	= sh2
TARGET		=dtmrsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
#
ID		=dtmrid
#
TMR10EVN	=tm10ev
TMR5EVN		=tm5ev
TMRINT		=tmrint
TMRSINIT	=tmrsini
TMRTSK		=tmrtsk
TMRDEC		=tmrdec

#===========================================================
#	<<< コマンド定義 >>>

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
OFILE1=$(ID).obj
OFILE2=$(TMR10EVN).obj $(TMR5EVN).obj $(TMRINT).obj $(TMRSINIT).obj $(TMRTSK).obj $(TMRDEC).obj
OFILES=$(OFILE1) $(OFILE2)
#
#
#
all : clean $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj : $(ID).c
	$(CC) $(ID).c				>  $(ERRFILE)
$(TMR10EVN).obj : $(TMR10EVN).c
	$(CC) $(TMR10EVN).c			>>  $(ERRFILE)
$(TMR5EVN).obj : $(TMR5EVN).c
	$(CC) $(TMR5EVN).c			>>  $(ERRFILE)
$(TMRINT).obj : $(TMRINT).c
	$(CC) $(TMRINT).c			>>  $(ERRFILE)
$(TMRSINIT).obj : $(TMRSINIT).c
	$(CC) $(TMRSINIT).c			>>  $(ERRFILE)
$(TMRTSK).obj : $(TMRTSK).c
	$(CC) $(TMRTSK).c			>>  $(ERRFILE)
$(TMRDEC).obj : $(TMRDEC).c
	$(CC) $(TMRDEC).c			>>  $(ERRFILE)

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

