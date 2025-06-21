#-----------------------------------------------------------------------#
# File name	: comdrv.mk	/ arcnet driver liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comsh2b_.mk 1.4 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: comsh2b_.mk $
#* リビジョン 1.4  1970/01/01  00:00:00  sakaguti
#* 2004/07/21 15:30:00
#* 自動認識機能付きARCNETﾗｲﾌﾞﾗﾘdcomash2b_.libをメイクする。
#* TARGETをdcomash2b_に変更。
#* 
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* CPUSH2定義追加
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/drive/arcnet/RCS/comdrv.mk 1.1 1970/01/01
#*Log: comdrv.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 1993, ISHIDA  CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	ARC-NET関連ドライバーのライブラリーを作成する。			#
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
TARGET		=dcomash3b_
else
TARGET		=dcomash3l_
endif
else
CTYPE	= sh2
TARGET		=dcomash2b_
endif
#
#	<<< ファイルネーム定義 >>>

MAKEFILE 	=comsh2b_.mk

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

#	<<< ヘッダー･ファイル定義 >>>

COMCTXT		=comctxt
COMRXH		=comrx
#===========================================================
#
#	<<< コマンド定義 >>>

ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -def=DEBUG,CPUSH2
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG,CPUSH2
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

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
