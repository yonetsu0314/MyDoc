#*======================================================================
#* File name    : nzcsh2b_.mk
#* Original	: 
#*----------------------------------------------------------------------
#* Function 	:
#*	NZコマンド&テキストに関するライブラリーを作成する。		
#*	SH2 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/nzcmd/src/RCS/nzcsh2b_.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: nzcsh2b_.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  kawamura
#* 2003/05/30  15:10
#* *.lctファイルのコピー条件を修正
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*Header: p:/presto/shlib/nzcmd/src/RCS/nzcmd.mk 1.2 1970/01/01 00:00:00
#*Log: nzcmd.mk
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
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
TARGET		=nzcsh3b_
else
TARGET		=nzcsh3l_
endif
else
CTYPE	= sh2
TARGET		=nzcsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=nzcmdid
#
NZI001		=nzi001
NZI002		=nzi002
NZI003		=nzi003
NZI004		=nzi004
NZI005		=nzi005
NZI006		=nzi006
NZI007		=nzi007
NZI008		=nzi008
NZI009		=nzi009
NZI010		=nzi010
NZI011		=nzi011
NZI012		=nzi012
NZI029		=nzi029
NZI030		=nzi030
NZI051		=nzi051
NZI052		=nzi052
NZR009		=nzr009
NZR010		=nzr010
NZR015		=nzr015
NZR016		=nzr016
NZR017		=nzr017
NZR018		=nzr018
NZR019		=nzr019
NZR020		=nzr020
NZR025		=nzr025
NZR026		=nzr026
NZR051		=nzr051
NZR052		=nzr052
NZR053		=nzr053
NZR054		=nzr054
NZR055		=nzr055
NZR056		=nzr056

#===========================================================
#
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
OFILE0  =$(ID).obj
OFILE1	=$(NZI001).obj $(NZI002).obj $(NZI003).obj $(NZI004).obj
OFILE2	=$(NZI005).obj $(NZI006).obj $(NZI007).obj $(NZI008).obj
OFILE3	=$(NZI009).obj $(NZI010).obj $(NZI011).obj $(NZI012).obj
OFILE4	=$(NZI029).obj $(NZI030).obj $(NZI051).obj $(NZI052).obj
OFILE5	=$(NZR009).obj $(NZR010).obj $(NZR015).obj $(NZR016).obj
OFILE6	=$(NZR017).obj $(NZR018).obj $(NZR019).obj $(NZR020).obj
OFILE7	=$(NZR025).obj $(NZR026).obj $(NZR051).obj $(NZR052).obj
OFILE8	=$(NZR053).obj $(NZR054).obj $(NZR055).obj $(NZR056).obj
OFILES	=$(OFILE0) $(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5)\
	 $(OFILE6) $(OFILE7) $(OFILE8)
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
$(NZI001).obj   : $(NZI001).c
	$(CC) $(NZI001).c			>>  $(ERRFILE)
$(NZI002).obj   : $(NZI002).c
	$(CC) $(NZI002).c			>>  $(ERRFILE)
$(NZI003).obj   : $(NZI003).c
	$(CC) $(NZI003).c			>>  $(ERRFILE)
$(NZI004).obj   : $(NZI004).c
	$(CC) $(NZI004).c			>>  $(ERRFILE)
$(NZI005).obj   : $(NZI005).c
	$(CC) $(NZI005).c			>>  $(ERRFILE)
$(NZI006).obj   : $(NZI006).c
	$(CC) $(NZI006).c			>>  $(ERRFILE)
$(NZI007).obj   : $(NZI007).c
	$(CC) $(NZI007).c			>>  $(ERRFILE)
$(NZI008).obj   : $(NZI008).c
	$(CC) $(NZI008).c			>>  $(ERRFILE)
$(NZI009).obj   : $(NZI009).c
	$(CC) $(NZI009).c			>>  $(ERRFILE)
$(NZI010).obj   : $(NZI010).c
	$(CC) $(NZI010).c			>>  $(ERRFILE)
$(NZI011).obj   : $(NZI011).c
	$(CC) $(NZI011).c			>>  $(ERRFILE)
$(NZI012).obj   : $(NZI012).c
	$(CC) $(NZI012).c			>>  $(ERRFILE)
$(NZI029).obj   : $(NZI029).c
	$(CC) $(NZI029).c			>>  $(ERRFILE)
$(NZI030).obj   : $(NZI030).c
	$(CC) $(NZI030).c			>>  $(ERRFILE)
$(NZI051).obj   : $(NZI051).c
	$(CC) $(NZI051).c			>>  $(ERRFILE)
$(NZI052).obj   : $(NZI052).c
	$(CC) $(NZI052).c			>>  $(ERRFILE)
$(NZR009).obj   : $(NZR009).c
	$(CC) $(NZR009).c			>>  $(ERRFILE)
$(NZR010).obj   : $(NZR010).c
	$(CC) $(NZR010).c			>>  $(ERRFILE)
$(NZR015).obj   : $(NZR015).c
	$(CC) $(NZR015).c			>>  $(ERRFILE)
$(NZR016).obj   : $(NZR016).c
	$(CC) $(NZR016).c			>>  $(ERRFILE)
$(NZR017).obj   : $(NZR017).c
	$(CC) $(NZR017).c			>>  $(ERRFILE)
$(NZR018).obj   : $(NZR018).c
	$(CC) $(NZR018).c			>>  $(ERRFILE)
$(NZR019).obj   : $(NZR019).c
	$(CC) $(NZR019).c			>>  $(ERRFILE)
$(NZR020).obj   : $(NZR020).c
	$(CC) $(NZR020).c			>>  $(ERRFILE)
$(NZR025).obj   : $(NZR025).c
	$(CC) $(NZR025).c			>>  $(ERRFILE)
$(NZR026).obj   : $(NZR026).c
	$(CC) $(NZR026).c			>>  $(ERRFILE)
$(NZR051).obj   : $(NZR051).c
	$(CC) $(NZR051).c			>>  $(ERRFILE)
$(NZR052).obj   : $(NZR052).c
	$(CC) $(NZR052).c			>>  $(ERRFILE)
$(NZR053).obj   : $(NZR053).c
	$(CC) $(NZR053).c			>>  $(ERRFILE)
$(NZR054).obj   : $(NZR054).c
	$(CC) $(NZR054).c			>>  $(ERRFILE)
$(NZR055).obj   : $(NZR055).c
	$(CC) $(NZR055).c			>>  $(ERRFILE)
$(NZR056).obj   : $(NZR056).c
	$(CC) $(NZR056).c			>>  $(ERRFILE)

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

