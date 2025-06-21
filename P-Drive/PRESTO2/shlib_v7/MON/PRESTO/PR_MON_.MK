#-----------------------------------------------------------------------#
# File name	: pr_mon_.mk	/ pdadc moniter liblary			#
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/mon/presto/RCS/pr_mon_.mk 1.1 1970/01/01 00:00:00 chimura Exp $
#*$Log: pr_mon_.mk $
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------#
#		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	#
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			#
#		SHIGA JAPAN						#
#		(0775) 53-4141						#
#-----------------------------------------------------------------------#
# Function								#
#	ADC(P-5561)のモニター用ライブラリーを作成する。			#
#-----------------------------------------------------------------------#
#
#
#===========================================================
DEBUG	= 1
#
#P5554	= P5554=0
P5561	= P5561=0
#
#	<<< ファイルネーム定義 >>>
#
#
TARGET		=pr_mon_
MON		=mon
#
STUPMON		=supmon
RCVTSK		=rcvtsk
MORTXT		=mor002
MRT001		=mrt001
MRT002		=mrt002
MRT003		=mrt003
MRT004		=mrt004
MRT005		=mrt005
MRT006		=mrt006
MRT007		=mrt007
MRT008		=mrt008
MRT010		=mrt010
MRT011		=mrt011
MRT012		=mrt012
MRT013		=mrt013
MRT014		=mrt014
MRT015		=mrt015
MRT016		=mrt016
MRT017		=mrt017
MRT018		=mrt018
MRT100		=mrt100
MOSTXT		=mos002
MST001		=mst001
MST002		=mst002
MST003		=mst003
MST004		=mst004
MST005		=mst005
MST006		=mst006
MST007		=mst007
MST010		=mst010
MST011		=mst011
MST012		=mst012
MEVF01		=mevf01
SMPLTSK		=smpltsk
AFVTSK		=afvtsk
STBTSK		=stbtsk

MAKEFILE = pr_mon.mk

C	=.c
SRC	=.asm
OBJ	=.obj

#===========================================================
#	<<< デバッグ情報の定義 >>>
include	option2.mac
#
ifdef	P5554
PNUM	=$(P5554)
else
PNUM	=$(P5561)
endif
#
ifeq ($(DEBUG),0)
ADBG	= -nodebug
CDBG	= -nodebug -op=1
LDBG	= nodebug
DEFINE_ = $(MOPT),NDEBUG,$(PNUM)
else
ADBG	= -debug
CDBG	= -debug -op=0
LDBG	= debug
DEFINE_ = $(MOPT),DEBUG,$(PNUM)
endif

CCOPT  = $(CDBG) -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -def=$(DEFINE_)

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc
#INCLIST := $(INCLIST),d:\home\chimura\newwcu\r6308
#INCLIST := $(INCLIST),d:\home\chimura\newwcu\r6510\wcu

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#	<<< ライブラリーの作成 >>>
OFILES	=$(STUPMON)$(OBJ) $(RCVTSK)$(OBJ) $(MORTXT)$(OBJ) $(MRT001)$(OBJ)\
	 $(MRT002)$(OBJ) $(MRT003)$(OBJ) $(MRT004)$(OBJ) $(MRT005)$(OBJ)\
	 $(MRT006)$(OBJ) $(MRT007)$(OBJ) $(MRT008)$(OBJ) $(MRT010)$(OBJ)\
	 $(MRT011)$(OBJ) $(MRT012)$(OBJ) $(MRT013)$(OBJ) $(MRT014)$(OBJ)\
	 $(MRT015)$(OBJ) $(MRT016)$(OBJ) $(MRT017)$(OBJ) $(MRT018)$(OBJ)\
	 $(MRT100)$(OBJ) $(MOSTXT)$(OBJ) $(MST001)$(OBJ) $(MST002)$(OBJ)\
	 $(MST003)$(OBJ) $(MST004)$(OBJ) $(MST005)$(OBJ) $(MST006)$(OBJ)\
	 $(MST007)$(OBJ) $(MST010)$(OBJ) $(MST011)$(OBJ) $(MST012)$(OBJ)\
	 $(MEVF01)$(OBJ) $(AFVTSK)$(OBJ)

all : clean $(TARGET).lib

#	<<< オブジェクトファイル生成 >>>
#
$(STUPMON)$(OBJ)  : $(STUPMON)$(C)
	$(CC) $(STUPMON)$(C)
$(RCVTSK)$(OBJ)	  : $(RCVTSK)$(C)
	$(CC) $(RCVTSK)$(C)
$(MORTXT)$(OBJ)	  : $(MORTXT)$(C)
	$(CC) $(MORTXT)$(C)
$(MRT001)$(OBJ)	  : $(MRT001)$(C)
	$(CC) $(MRT001)$(C)
$(MRT002)$(OBJ)	  : $(MRT002)$(C)
	$(CC) $(MRT002)$(C)
$(MRT003)$(OBJ)	  : $(MRT003)$(C)
	$(CC) $(MRT003)$(C)
$(MRT004)$(OBJ)	  : $(MRT004)$(C)
	$(CC) $(MRT004)$(C)
$(MRT005)$(OBJ)	  : $(MRT005)$(C)
	$(CC) $(MRT005)$(C)
$(MRT006)$(OBJ)	  : $(MRT006)$(C)
	$(CC) $(MRT006)$(C)
$(MRT007)$(OBJ)	  : $(MRT007)$(C)
	$(CC) $(MRT007)$(C)
$(MRT008)$(OBJ)	  : $(MRT008)$(C)
	$(CC) $(MRT008)$(C)
$(MRT010)$(OBJ)	  : $(MRT010)$(C)
	$(CC) $(MRT010)$(C)
$(MRT011)$(OBJ)	  : $(MRT011)$(C)
	$(CC) $(MRT011)$(C)
$(MRT012)$(OBJ)	  : $(MRT012)$(C)
	$(CC) $(MRT012)$(C)
$(MRT013)$(OBJ)	  : $(MRT013)$(C)
	$(CC) $(MRT013)$(C)
$(MRT014)$(OBJ)	  : $(MRT014)$(C)
	$(CC) $(MRT014)$(C)
$(MRT015)$(OBJ)	  : $(MRT015)$(C)
	$(CC) $(MRT015)$(C)
$(MRT016)$(OBJ)	  : $(MRT016)$(C)
	$(CC) $(MRT016)$(C)
$(MRT017)$(OBJ)	  : $(MRT017)$(C)
	$(CC) $(MRT017)$(C)
$(MRT018)$(OBJ)	  : $(MRT018)$(C)
	$(CC) $(MRT018)$(C)
$(MRT100)$(OBJ)	  : $(MRT100)$(C)
	$(CC) $(MRT100)$(C)
$(MOSTXT)$(OBJ)	  : $(MOSTXT)$(C)
	$(CC) $(MOSTXT)$(C)
$(MST001)$(OBJ)	  : $(MST001)$(C)
	$(CC) $(MST001)$(C)
$(MST002)$(OBJ)	  : $(MST002)$(C)
	$(CC) $(MST002)$(C)
$(MST003)$(OBJ)	  : $(MST003)$(C)
	$(CC) $(MST003)$(C)
$(MST004)$(OBJ)	  : $(MST004)$(C)
	$(CC) $(MST004)$(C)
$(MST005)$(OBJ)	  : $(MST005)$(C)
	$(CC) $(MST005)$(C)
$(MST006)$(OBJ)	  : $(MST006)$(C)
	$(CC) $(MST006)$(C)
$(MST007)$(OBJ)	  : $(MST007)$(C)
	$(CC) $(MST007)$(C)
$(MST010)$(OBJ)	  : $(MST010)$(C)
	$(CC) $(MST010)$(C)
$(MST011)$(OBJ)	  : $(MST011)$(C)
	$(CC) $(MST011)$(C)
$(MST012)$(OBJ)	  : $(MST012)$(C)
	$(CC) $(MST012)$(C)
$(MEVF01)$(OBJ)	  : $(MEVF01)$(C)
	$(CC) $(MEVF01)$(C)
$(AFVTSK)$(OBJ)	  : $(AFVTSK)$(C)
	$(CC) $(AFVTSK)$(C)

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
	if exist *.mac	ident *.mac	>> release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
