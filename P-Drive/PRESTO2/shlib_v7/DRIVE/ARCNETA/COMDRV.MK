#-----------------------------------------------------------------------#
# File name	: comdrv.mk	/ arcnet driver liblary			#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comdrv.mk 1.1 1970/01/01 00:00:00 chimura Exp $
#*$Log: comdrv.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 1993, ISHIDA  CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	ARC-NET関連ドライバーのライブラリーを作成する。			#
#-----------------------------------------------------------------------#
#	<<< ファイルネーム定義 >>>

TARGET		=dcom
ID		=dcomaid

COMSYINI	=comsini
COMOPEN		=comopn
COMCLOS		=comcls
COMCTL		=comctl
COMRX		=comrx
COMTX		=comtx
COMMAP		=commap
COMIORX		=comiorx

COM0INT		=com0int
COM1INT		=com1int
COM2INT		=com2int
COM3INT		=com3int

MAKEFILE =comdrv.mk

#	<<< ヘッダー･ファイル定義 >>>

COMCTXT		=comctxt
COMRXH		=comrx
#===========================================================
#
#	<<< コマンド定義 >>>

#	<<< デバッグ情報の定義 >>>
#
DEBUG =
#DEBUG = 1

ifdef	DEBUG
CCOPT	= -cpu=sh3 -op=0 -en=big -me -debug -comment=nest -def=DEBUG
else
CCOPT	= -cpu=sh3 -op=1 -en=big -me -nodebug -comment=nest -l -def=NDEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
LIB	=lbr

#	<<< ライブラリーの作成 >>>

OFILES	=$(ID).obj $(COMSYINI).obj $(COMOPEN).obj $(COMCLOS).obj $(COMTX).obj\
	 $(COMRX).obj $(COMMAP).obj $(COMIORX).obj $(COM0INT).obj\
	 $(COM1INT).obj $(COM2INT).obj $(COM3INT).obj $(COMCTL).obj

all : clean $(TARGET).lib
#
#	<<< オブジェクトファイル生成 >>>
#
$(ID).obj : $(ID).c
	$(CC) $(ID).c			>  x
$(COMSYINI).obj	: $(COMSYINI).c
	$(CC) $(COMSYINI).c		>> x
$(COMOPEN).obj	: $(COMOPEN).c
	$(CC) $(COMOPEN).c		>> x
$(COMCLOS).obj	: $(COMCLOS).c
	$(CC) $(COMCLOS).c		>>x
$(COMRX).obj	: $(COMRX).c
	$(CC) $(COMRX).c		>> x
$(COMTX).obj	: $(COMTX).c
	$(CC) $(COMTX).c		>> x
$(COMMAP).obj	: $(COMMAP).c
	$(CC) $(COMMAP).c		>> x
$(COMIORX).obj	: $(COMIORX).c $(COMRXH).h
	$(CC) $(COMIORX).c		>> x
$(COM0INT).obj	: $(COM0INT).c $(COMRXH).h
	$(CC) $(COM0INT).c		>> x
$(COM1INT).obj	: $(COM1INT).c $(COMRXH).h
	$(CC) $(COM1INT).c		>> x
$(COM2INT).obj	: $(COM2INT).c $(COMRXH).h
	$(CC) $(COM2INT).c		>> x
$(COM3INT).obj	: $(COM3INT).c $(COMRXH).h
	$(CC) $(COM3INT).c		>> x
$(COMCTL).obj	: $(COMCTL).c
	$(CC) $(COMCTL).c		>> x

$(TARGET).lib : $(OFILES) $(TARGET).sub
	del $(TARGET).lib
	del $(TARGET).lct
	$(LIB)  -subcommand=$(TARGET).sub
	copy $(TARGET).lib ..\*.*
	copy $(TARGET).lct ..\*.*

$(TARGET).sub : $(MAKEFILE)
	echo	CREATE $(TARGET).lib	>  $(TARGET).sub
	echo	ADD $(OFILES)		>> $(TARGET).sub
	echo	LIST $(TARGET)(S)	>> $(TARGET).sub
	echo	EXIT			>> $(TARGET).sub

#-----------------------------------------------------------------------#
#			ＭＡＫＥコマンドの定義										#
#-----------------------------------------------------------------------#
DRV 	=p:

checkin :
	perl $(DRV)\tool\perl\checkin.pl *.mk	>  $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.h	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.c	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.asm	>> $(TARGET).bat

clean:
	if exist *.obj del *.obj
	if exist *.abs del *.abs
	if exist *.mot del *.mot
	if exist *.map del *.map
	if exist *.lis del *.lis
	if exist *.sub del *.sub

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
