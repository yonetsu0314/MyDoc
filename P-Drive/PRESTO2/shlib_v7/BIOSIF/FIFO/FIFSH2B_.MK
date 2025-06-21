#-----------------------------------------------------------------------#
# File name	: fifsh2b_.mk	/ init liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/biosif/fifo/rcs/fifsh2b_.mk 1.3 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: fifsh2b_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* DMUにint_mask,pint_maskを追加
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* fifinit,fiffullを追加
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* 04/12/07 初期レビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	section及びregisterの初期化ライブラリーを作成する。		#
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
TARGET		=bfifsh3b_
else
TARGET		=bfifsh3l_
endif
else
CTYPE	= sh2
TARGET		=bfifsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=bfifid
#
FIFINP		=fifinp
FIFOUTP		=fifoutp
FIFSTTS		=fifstts
FIFMASK		=fifmask
FIFINIT		=fifinit
FIFFULL		=fiffull
DMUMASK		=dmumask
DMUPMSK		=dmupmsk
#

#===========================================================
#	<<< コマンド定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG,CPUSH2
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
OFILE1=$(ID).obj $(FIFINIT).obj $(FIFFULL).obj $(DMUMASK).obj $(DMUPMSK).obj
OFILE2=$(FIFINP).obj $(FIFOUTP).obj $(FIFSTTS).obj $(FIFMASK).obj
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
$(FIFINP).obj : $(FIFINP).c
	$(CC) $(FIFINP).c			>>  $(ERRFILE)
$(FIFOUTP).obj : $(FIFOUTP).c
	$(CC) $(FIFOUTP).c			>>  $(ERRFILE)
$(FIFSTTS).obj : $(FIFSTTS).c
	$(CC) $(FIFSTTS).c			>>  $(ERRFILE)
$(FIFMASK).obj : $(FIFMASK).c
	$(CC) $(FIFMASK).c			>>  $(ERRFILE)
$(FIFINIT).obj : $(FIFINIT).c
	$(CC) $(FIFINIT).c			>>  $(ERRFILE)
$(FIFFULL).obj : $(FIFFULL).c
	$(CC) $(FIFFULL).c			>>  $(ERRFILE)
$(DMUMASK).obj : $(DMUMASK).c
	$(CC) $(DMUMASK).c			>>  $(ERRFILE)
$(DMUPMSK).obj : $(DMUPMSK).c
	$(CC) $(DMUPMSK).c			>>  $(ERRFILE)

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

