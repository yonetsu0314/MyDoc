#-----------------------------------------------------------------------#
# File name	: rtcsh3.mk	/ rtcint liblary			#
#		  SH3 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/biosif/rtc/RCS/brtcsh3_.mk 1.1 1970/01/01 00:00:00 chimura Exp $
#*$Log: brtcsh3_.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* LOG MESSAGE
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	低レベル水準のRTC関連I/Fライブラリーを作成する。		#
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
TARGET		=brtcsh3b_
else
TARGET		=brtcsh3l_
endif
else
CTYPE	= sh2
TARGET		=brtcsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=brtcid
#
RTCINIT	=brtcini
RTCSDT	=brtcsdt
RTCGDT	=brtcgdt
RTCSTM	=brtcstm
RTCGTM	=brtcgtm

#===========================================================
#	<<< コマンド定義 >>>

ifdef	DEBUG
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
OFILE2=$(RTCINIT).obj $(RTCSDT).obj $(RTCGDT).obj $(RTCSTM).obj $(RTCGTM).obj
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
$(RTCINIT).obj : $(RTCINIT).c
	$(CC) $(RTCINIT).c			>>  $(ERRFILE)
$(RTCSDT).obj : $(RTCSDT).c
	$(CC) $(RTCSDT).c			>>  $(ERRFILE)
$(RTCGDT).obj : $(RTCGDT).c
	$(CC) $(RTCGDT).c			>>  $(ERRFILE)
$(RTCSTM).obj : $(RTCSTM).c
	$(CC) $(RTCSTM).c			>>  $(ERRFILE)
$(RTCGTM).obj : $(RTCGTM).c
	$(CC) $(RTCGTM).c			>>  $(ERRFILE)

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
	if exist *.lct del *.lct
	if exist *.lst del *.lst
	if exist *.sub del *.sub
	if exist *.lib del *.lib
	if exist *.opt del *.opt
	if exist dwfinf Rd /S /Q dwfinf

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id

