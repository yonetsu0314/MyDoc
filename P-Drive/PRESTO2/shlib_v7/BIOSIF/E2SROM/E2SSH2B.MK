#-----------------------------------------------------------------------#
# File name	: e2ssh2b.mk	/ e2rom int liblary			#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/biosif/e2srom/RCS/e2ssh2b.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: e2ssh2b.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/11/13 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* シリアルＲＯＭＩ／Ｆ
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	低レベル水準のE2ROM関連I/Fライブラリーを作成する。		#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 17-DEC-92 : 株本 隆司					#
#			      / Initial codeing				#
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
TARGET		=be2ssh3b
else
TARGET		=be2ssh3l
endif
else
CTYPE	= sh2
TARGET		=be2ssh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=be2srid
#
E2SRCE		=e2srce
E2SRSK		=e2srsk
E2SRDI		=e2srdi
E2SRDO		=e2srdo

#===========================================================
#	<<< デバッグ情報の定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -def=DEBUG,CPUSH2
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG,CPUSH2
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
OFILE2=$(E2SRCE).obj $(E2SRSK).obj $(E2SRDI).obj $(E2SRDO).obj
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
$(E2SRCE).obj : $(E2SRCE).c
	$(CC) $(E2SRCE).c			>>  $(ERRFILE)
$(E2SRSK).obj : $(E2SRSK).c
	$(CC) $(E2SRSK).c			>>  $(ERRFILE)
$(E2SRDI).obj : $(E2SRDI).c
	$(CC) $(E2SRDI).c			>>  $(ERRFILE)
$(E2SRDO).obj : $(E2SRDO).c
	$(CC) $(E2SRDO).c			>>  $(ERRFILE)

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

