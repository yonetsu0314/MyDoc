#-----------------------------------------------------------------------#
# File name	: libutl2_.mk
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fssvr60/utility2/RCS/libutl2_.mk 1.2 1970/01/01 00:00:00 kagatume Exp $
#*$Log: libutl2_.mk $
#*リビジョン 1.2  1970/01/01  00:00:00  kagatume
#*2005/02/17 10:30
#*cnvfsys.cの追加
#*
#*リビジョン 1.1  1970/01/01  00:00:00  kagatume
#*初期リビジョン
#*
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
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
else
CTYPE	= sh2
endif

TARGET		=libutl2_

#===========================================================
#
ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(INC_BASE)\DRIVE\FSSVR60
INCLIST := $(INCLIST),$(SHC_INC)
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#-------------------------------------------------------#
# Directory path
#-------------------------------------------------------#
PRJ_HOME	= ..

LIBDIR		= $(PRJ_HOME)\lib

#===========================================================
#
#	<<< ライブラリーの作成 >>>
#
OFILES := fs_date.obj biosfile.obj dos_util.obj shellsrc.obj\
		powfail.obj apimisc.obj cnvfsys.obj

#
#
all : clean $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

fs_date.obj	: fs_date.c
	$(CC) fs_date.c			>> $(ERRFILE)
biosfile.obj	: biosfile.c
	$(CC) biosfile.c			>> $(ERRFILE)
dos_util.obj	: dos_util.c
	$(CC) dos_util.c			>> $(ERRFILE)
shellsrc.obj	: shellsrc.c
	$(CC) shellsrc.c			>> $(ERRFILE)
powfail.obj	: powfail.c
	$(CC) powfail.c			>> $(ERRFILE)
apimisc.obj	: apimisc.c
	$(CC) apimisc.c			>> $(ERRFILE)
cnvfsys.obj	: cnvfsys.c
	$(CC) cnvfsys.c			>> $(ERRFILE)

$(TARGET).lib : $(OFILES) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(OPTLNK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib $(LIBDIR)\*.*

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

clean:
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

