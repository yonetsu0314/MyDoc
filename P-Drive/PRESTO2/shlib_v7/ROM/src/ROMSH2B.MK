#-----------------------------------------------------------------------#
# File name	: romsh2b.mk	/ Romable liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/rom/src/RCS/romsh2b.mk 1.4 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: romsh2b.mk $
#* リビジョン 1.4  1970/01/01  00:00:00  sakaguti
#* makeの前にcleanする
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
#*Header: p:/presto/shlib/rom/src/RCS/rom.mk 1.2 1970/01/01 00:00:00
#*Log: rom.mk
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY				#
#		SHIGA JAPAN						#
#		(077) 553-4141						#
#-----------------------------------------------------------------------#
# Modification history							#
#	Version |    Date   |  Name			       		#
#	v1.00	: 20-OCT-02 : 						#
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
TARGET		=romsh3b
else
TARGET		=romsh3l
endif
else
CTYPE	= sh2
TARGET		=romsh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=romid
#
ABORT		=abort
ASSERT		=assert
CLOSE		=close
#CTYPE		=cdata
CTIME		=ctime
RTC		=rtc
DMM		=dmm
FORMAT		=format
GETCH		=getch
GETCHAR		=getchar
GETS		=gets
JFORMAT		=jformat
KBHIT		=kbhit
OPEN		=open
PRINTF		=printf
PUTCH		=putch
PUTCHAR		=putchar
PUTS		=puts
READ		=read
ROMEXIT		=raext
SCAN		=scan
SEEK		=seek
SPRINTF		=sprintf
SSCANF		=sscanf
WRITE		=write
REDIRECT	=redirect
#
#	ASM Source file
#
#CHKSTK		=chkstk
#SIGNAL		=signal
#TIME		=time
#

#===========================================================
#
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
OFILE1=$(ID).obj $(ABORT).obj $(ASSERT).obj $(CLOSE).obj
#OFILE2=$(CTYPE).obj $(CTIME).obj $(RTC).obj $(DMM).obj $(FORMAT).obj
OFILE2=$(CTIME).obj $(RTC).obj $(DMM).obj
OFILE3=$(GETCH).obj $(GETCHAR).obj $(GETS).obj
OFILE4=$(KBHIT).obj $(OPEN).obj $(PRINTF).obj $(PUTCH).obj $(PUTCHAR).obj $(PUTS).obj
OFILE5=$(READ).obj $(SEEK).obj $(SPRINTF).obj $(SSCANF).obj
#OFILE6=$(WRITE).obj $(REDIRECT).obj $(CHKSTK).obj $(SIGNAL).obj $(TIME).obj
OFILE6=$(WRITE).obj $(REDIRECT).obj
SUBF1=$(FORMAT).obj $(JFORMAT).obj $(SCAN).obj $(ROMEXIT).obj
#
OFILES1=$(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5) $(OFILE6)
OFILES2=$(SUBF1)
OFILES=$(OFILES1) $(OFILES2)

#
#
all : clean $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj	: $(ID).c
	$(CC) $(ID).c				>  $(ERRFILE)
$(ABORT).obj	: $(ABORT).c
	$(CC) $(ABORT).c			>> $(ERRFILE)
$(ASSERT).obj	: $(ASSERT).c
	$(CC) $(ASSERT).c			>> $(ERRFILE)
$(CLOSE).obj	: $(CLOSE).c
	$(CC) $(CLOSE).c			>> $(ERRFILE)
#$(CTYPE).obj	: $(CTYPE).c
#	$(CC) $(CTYPE).c			>> $(ERRFILE)
$(CTIME).obj	: $(CTIME).c
	$(CC) $(CTIME).c			>> $(ERRFILE)
$(RTC).obj	: $(RTC).c
	$(CC) $(RTC).c				>> $(ERRFILE)
$(DMM).obj	: $(DMM).c
	$(CC) $(DMM).c				>> $(ERRFILE)
$(FORMAT).obj	: $(FORMAT).c
	$(CC) $(FORMAT).c			>> $(ERRFILE)
$(GETCH).obj	: $(GETCH).c
	$(CC) $(GETCH).c			>> $(ERRFILE)
$(GETCHAR).obj	: $(GETCHAR).c
	$(CC) $(GETCHAR).c			>> $(ERRFILE)
$(GETS).obj	: $(GETS).c
	$(CC) $(GETS).c				>> $(ERRFILE)
$(JFORMAT).obj	: $(JFORMAT).c
	$(CC) $(JFORMAT).c			>> $(ERRFILE)
$(KBHIT).obj	: $(KBHIT).c
	$(CC) $(KBHIT).c			>> $(ERRFILE)
$(OPEN).obj	: $(OPEN).c
	$(CC) $(OPEN).c				>> $(ERRFILE)
$(PRINTF).obj	: $(PRINTF).c
	$(CC) $(PRINTF).c			>> $(ERRFILE)
$(PUTCH).obj	: $(PUTCH).c
	$(CC) $(PUTCH).c			>> $(ERRFILE)
$(PUTCHAR).obj	: $(PUTCHAR).c
	$(CC) $(PUTCHAR).c			>> $(ERRFILE)
$(PUTS).obj	: $(PUTS).c
	$(CC) $(PUTS).c				>> $(ERRFILE)
$(READ).obj	: $(READ).c
	$(CC) $(READ).c				>> $(ERRFILE)
$(ROMEXIT).obj	: $(ROMEXIT).c
	$(CC) $(ROMEXIT).c			>> $(ERRFILE)
$(SCAN).obj	: $(SCAN).c
	$(CC) $(SCAN).c				>> $(ERRFILE)
$(SEEK).obj	: $(SEEK).c
	$(CC) $(SEEK).c				>> $(ERRFILE)
$(SPRINTF).obj	: $(SPRINTF).c
	$(CC) $(SPRINTF).c			>> $(ERRFILE)
$(SSCANF).obj	: $(SSCANF).c
	$(CC) $(SSCANF).c			>> $(ERRFILE)
$(WRITE).obj	: $(WRITE).c
	$(CC) $(WRITE).c			>> $(ERRFILE)
$(REDIRECT).obj	: $(REDIRECT).c
	$(CC) $(REDIRECT).c			>> $(ERRFILE)
#
#$(CHKSTK).obj	: $(CHKSTK).asm
#	$(ASM) $(CHKSTK).asm			>> $(ERRFILE)
#$(SIGNAL).obj	: $(SIGNAL).asm
#	$(ASM) $(SIGNAL).asm			>> $(ERRFILE)
#$(TIME).obj	: $(TIME).asm
#	$(ASM) $(TIME).asm			>> $(ERRFILE)

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

