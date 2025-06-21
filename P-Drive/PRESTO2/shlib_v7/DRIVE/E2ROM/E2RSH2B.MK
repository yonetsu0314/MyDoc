#-----------------------------------------------------------------------#
# File name	: e2rsh2b.mk	/ e2rom driver liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2rsh2b.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: e2rsh2b.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/e2rom/RCS/e2rdrv.mk 1.1 1970/01/01
#*Log: e2rdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	E2ROM関連ドライバーのライブラリーを作成する。			#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 16-MAR-93 : 株本 隆司					#
#			      / Initial codeing				#
#-----------------------------------------------------------------------#
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
TARGET		=de2rsh3b
else
TARGET		=de2rsh3l
endif
else
CTYPE	= sh2
TARGET		=de2rsh2b
endif
#
#	<<< ファイルネーム定義 >>>

MAKEFILE 	=e2rsh2b.mk

ID		=de2rid

E2SINIT		=e2sini
E2OPEN		=e2open
E2CLOSE		=e2clos
E2WRITE		=e2write
E2READ		=e2read
E2SEEK		=e2seek
E2WRTSK		=e2wrtsk

#	<<< ヘッダー･ファイル定義 >>>

CONTEXT		=e2ctxt
#===========================================================
#	<<< デバッグ情報の定義 >>>

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
OFILES	=$(ID).obj $(E2SINIT).obj $(E2OPEN).obj $(E2CLOSE).obj\
	 $(E2WRITE).obj $(E2READ).obj $(E2SEEK).obj $(E2WRTSK).obj

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>

$(ID).obj : $(ID).c
	$(CC) $(ID).c				> x
$(E2SINIT).obj : $(E2SINIT).c $(CONTEXT).h
	$(CC) $(E2SINIT).c			>> x
$(E2OPEN).obj : $(E2OPEN).c
	$(CC) $(E2OPEN).c			>> x
$(E2CLOSE).obj : $(E2CLOSE).c
	$(CC) $(E2CLOSE).c			>> x
$(E2WRITE).obj : $(E2WRITE).c
	$(CC) $(E2WRITE).c			>> x
$(E2READ).obj : $(E2READ).c $(CONTEXT).h
	$(CC) $(E2READ).c			>> x
$(E2SEEK).obj : $(E2SEEK).c
	$(CC) $(E2SEEK).c			>> x
$(E2WRTSK).obj : $(E2WRTSK).c $(CONTEXT).h
	$(CC) $(E2WRTSK).c			>> x

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
