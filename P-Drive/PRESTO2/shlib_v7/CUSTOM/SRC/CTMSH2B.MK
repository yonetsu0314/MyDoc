#-----------------------------------------------------------------------#
# File name	: ctmsh2b.mk	/ Custom liblary			#
#		  SH2 ビッグエンディアン用				#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/custom/src/RCS/ctmsh2b.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: ctmsh2b.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#*Header: p:/presto/shlib/custom/src/RCS/custom.mak 1.1 1970/01/01 00:00:00
#*Log: custom.mak
#-----------------------------------------------------------------------#
#		(c) Copyright 2002, ISHIDA CO., LTD.			#
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		#
#		(077) 553-4141						#
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
TARGET		=ctmsh3b
else
TARGET		=ctmsh3l
endif
else
CTYPE	= sh2
TARGET		=ctmsh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=customid
#
ABP2BBP		=abp2bbp
BBP2ABP		=bbp2abp
CHKBCC		=chkbcc
CHRCMP		=chrcmp
CKSUMALP	=cksmalp
CKSUMCPL	=cksmcpl
FTOA		=ftoa
HTOI		=htoi
ITOH		=itoh
SETARGV		=setarg
SWAP		=swap
VFBITRD		=vfbitrd
VFBITWR		=vfbitwr
VFGETC		=vfgetc
VFPUTC		=vfputc
VFGETI		=vfgeti
VFPUTI		=vfputi
VFMCPY		=vfmcpy
VFBKCPY		=vfbkcpy
INTCTRL		=intctrl
#
#===========================================================
#
#	<<< コマンド定義 >>>
#
# segment name
SEG	=custom


INC1 = .
INC2 = $(INC_BASE)
INC3 = $(SH_COMPILER_BASE)\include

# target library name
#
ifeq ($(DEBUG),1)
CFLAGS	=-i=$(INC1),$(INC2),$(INC3) -debug -op=0 
else
CFLAGS	=-i=$(INC1),$(INC2),$(INC3) -nodebug -op=1
endif
#

CCOPT	=-cpu=$(CTYPE) -comment=nest -en=$(ETYPE) -me
CC	=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk
#===========================================================
#
#	<<< ライブラリーの作成 >>>
#
OFILE1	=$(CHKBCC).obj $(CHRCMP).obj $(CKSUMALP).obj $(CKSUMCPL).obj
OFILE2	=$(FTOA).obj $(HTOI).obj $(ITOH).obj $(ABP2BBP).obj $(BBP2ABP).obj
OFILE3	=$(SETARGV).obj $(ID).obj
OFILE4	=$(VFBITRD).obj $(VFBITWR).obj $(VFGETC).obj $(VFPUTC).obj
OFILE5	=$(VFGETI).obj $(VFPUTI).obj $(VFMCPY).obj $(VFBKCPY).obj
OFILE6	=$(INTCTRL).obj
#
OFILES	=$(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5) $(OFILE6)
#
all : clean $(TARGET).lib

$(ID).obj   : $(ID).c
	$(CC) $(ID).c
$(ABP2BBP).obj   : $(ABP2BBP).c
	$(CC) $(ABP2BBP).c
$(BBP2ABP).obj   : $(BBP2ABP).c
	$(CC) $(BBP2ABP).c
$(CHKBCC).obj    : $(CHKBCC).c
	$(CC) $(CHKBCC).c
$(CHRCMP).obj    : $(CHRCMP).c
	$(CC) $(CHRCMP).c
$(CKSUMALP).obj  : $(CKSUMALP).c
	$(CC) $(CKSUMALP).c
$(CKSUMCPL).obj  : $(CKSUMCPL).c
	$(CC) $(CKSUMCPL).c
$(FTOA).obj      : $(FTOA).c
	$(CC) $(FTOA).c
$(HTOI).obj      : $(HTOI).c
	$(CC) $(HTOI).c
$(ITOH).obj      : $(ITOH).c
	$(CC) $(ITOH).c
$(SETARGV).obj   : $(SETARGV).c
	$(CC) $(SETARGV).c
$(VFBITRD).obj	 : $(VFBITRD).c
	$(CC) $(VFBITRD).c
$(VFBITWR).obj   : $(VFBITWR).c
	$(CC) $(VFBITWR).c
$(VFBKCPY).obj    : $(VFBKCPY).c
	$(CC) $(VFBKCPY).c
$(VFGETC).obj    : $(VFGETC).c
	$(CC) $(VFGETC).c
$(VFPUTC).obj    : $(VFPUTC).c
	$(CC) $(VFPUTC).c
$(VFGETI).obj    : $(VFGETI).c
	$(CC) $(VFGETI).c
$(VFPUTI).obj    : $(VFPUTI).c
	$(CC) $(VFPUTI).c
$(VFMCPY).obj    : $(VFMCPY).c
	$(CC) $(VFMCPY).c
$(INTCTRL).obj    : $(INTCTRL).c
	$(CC) $(INTCTRL).c

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
