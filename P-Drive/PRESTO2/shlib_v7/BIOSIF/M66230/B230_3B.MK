#-----------------------------------------------------------------------#
# File name	: b230_3b.mk	/ m66230int liblary							#
#		  SH3 ビッグエンディアン用										#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/biosif/m66230/RCS/b230_3b.mk 1.1 1970/01/01 00:00:00 chimura Exp $
#*$Log: b230_3b.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.							#
#		959-1 SHIMOMAGARI RITTO-CITY									#
#		SHIGA JAPAN														#
#		(077) 553-4141													#
#-----------------------------------------------------------------------#
# Function																#
#	低レベル水準のM66230関連I/Fライブラリーを作成する。					#
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
TARGET		=b230_3b
else
TARGET		=b230_3l
endif
else
CTYPE	= sh2
TARGET		=b230_2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID			=b66230id
#
SCUINIT		=scuinit
SCUGST		=scugst
SCUSST		=scusst
SCUPUTC		=scuputc
SCUGETC		=scugetc

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
OFILE2=$(SCUINIT).obj $(SCUGST).obj $(SCUSST).obj $(SCUPUTC).obj $(SCUGETC).obj
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
	$(CC) $(ID).c						>  $(ERRFILE)
$(SCUINIT).obj : $(SCUINIT).c
	$(CC) $(SCUINIT).c					>>  $(ERRFILE)
$(SCUGST).obj : $(SCUGST).c
	$(CC) $(SCUGST).c					>>  $(ERRFILE)
$(SCUSST).obj : $(SCUSST).c
	$(CC) $(SCUSST).c					>>  $(ERRFILE)
$(SCUPUTC).obj : $(SCUPUTC).c
	$(CC) $(SCUPUTC).c					>>  $(ERRFILE)
$(SCUGETC).obj : $(SCUGETC).c
	$(CC) $(SCUGETC).c					>>  $(ERRFILE)

$(TARGET).lib : $(OFILES) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(OPTLNK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib ..\*.*

$(TARGET).sub :
	echo	FORM	= library			>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib		>>$(TARGET).sub
	echo	INPUT	= $(OFILES)			>>$(TARGET).sub
	echo	LIST	= $(TARGET)			>>$(TARGET).sub
	echo	EXIT						>>$(TARGET).sub

#-----------------------------------------------------------------------#
#	ＭＡＫＥコマンドの定義												#
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

