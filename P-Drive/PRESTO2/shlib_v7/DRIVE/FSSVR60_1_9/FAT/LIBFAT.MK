#-----------------------------------------------------------------------#
# File name	: libfat.mk
#-----------------------------------------------------------------------#
#*$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/rcs/libfat.mk 1.4 1970/01/01 00:00:00 kagatume Exp $
#*$Log: libfat.mk $
#*リビジョン 1.4  1970/01/01  00:00:00  kagatume
#*2006/03/22 19:00
#*ライブラリ名を変更する
#*
#*リビジョン 1.3  1970/01/01  00:00:00  kagatume
#*2006/03/22 16:00
#*FSFATID.Cの追加
#*
#*リビジョン 1.2  1970/01/01  00:00:00  kagatume
#*2005/02/16 21:00
#*Saver60 Ver.01.04 up
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
#----------------------------------------------------------#
# Configuration 
#-------------------------------------------------------#
CPU           = SH2
SHC_VER       = 7
OPTIMIZE      = 1
LITTLE_ENDIAN = NO
DEBUG         = NO

ifeq ($(LITTLE_ENDIAN),YES)
ENDIAN	= l
else
ENDIAN	= b
endif

ifeq ($(DEBUG),YES)
DEBUG_INFO = -deb
else
DEBUG_INFO = -nodeb
endif

#-------------------------------------------------------#
# Directory path
#-------------------------------------------------------#
PRJ_HOME	= ..

LIBDIR		= $(PRJ_HOME)\lib
FATDIR		= $(PRJ_HOME)\FAT
APIDIR		= $(FATDIR)\API
COMDIR		= $(FATDIR)\COMMON
OSDIR		= $(FATDIR)\OS\ITRON
FATINCDIR	= $(INC_BASE)\DRIVE\FSSVR60

#-------------------------------------------------------#
#   Headers
#-------------------------------------------------------#
INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(FATINCDIR)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

HEADERS	=	$(FATINCDIR)\fsbase.h	\
			$(FATINCDIR)\fsdef.h	\
			$(FATINCDIR)\fsdrvdef.h	\
			$(FATINCDIR)\fsmacro.h	\
			$(FATINCDIR)\fsstruct.h	\
			$(FATINCDIR)\fstype.h	\
			$(FATINCDIR)\fsuser.h	\
			$(FATINCDIR)\powfail.h

#-------------------------------------------------------#
#   Compaile, Asemble
#-------------------------------------------------------#
CCFLAGS	= -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -i=$(INCLIST)
CAFLAGS = -cp=$(CPU) -op=$(OPTIMIZE) $(DEBUG_INFO) -en=$(ENDIAN) -nosp -code=asmcode -i=$(INCLIST)
ASFLAGS = -cp=$(CPU) -en=$(ENDIAN) $(DEBUG_INFO) -i=$(INCLIST)

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
MKCFG_FILE	= $(FATDIR)\libfat.mk

#-------------------------------------------------------#
# Make Command
#-------------------------------------------------------#
CC			= shc
AS			= asmsh

ifeq ($(SHC_VER),5)
LB      	= lbr
LK			= lnk
else
LB      	= optlnk
LK			= optlnk
endif

#-------------------------------------------------------#
#   Kernel library file
#-------------------------------------------------------#
TARGET	= libfata

#-------------------------------------------------------#
#   Object file
#-------------------------------------------------------#
OBJS =		fsfatid.obj		\
			fsclose.obj		\
			findclse.obj	\
			findfrst.obj	\
			findnext.obj	\
			fsformat.obj	\
			fsinfo.obj		\
			fsinit.obj		\
			fsmkdir.obj		\
			fsmount.obj		\
			fsopen.obj		\
			fsread.obj		\
			fsremove.obj	\
			fsrename.obj	\
			fsrmdir.obj		\
			fsseek.obj		\
			fssetatr.obj	\
			fsstat.obj		\
			fstell.obj		\
			unmount.obj		\
			fswrite.obj		\
			appddata.obj	\
			chkpath.obj		\
			compare.obj		\
			ctrlfat.obj		\
			copysect.obj	\
			deldata.obj		\
			deldent.obj		\
			dellnent.obj	\
			sjis_uni.obj	\
			uni_sjis.obj	\
			expndent.obj	\
			checksum.obj	\
			gdirclst.obj	\
			gempclst.obj	\
			gfatsect.obj	\
			gfatval.obj		\
			gsectnum.obj	\
			gshtname.obj	\
			inident.obj		\
			inifhdl.obj		\
			inisrhdl.obj	\
			linkclst.obj	\
			fmemcopy.obj	\
			fmemset.obj		\
			fsram.obj		\
			rdatasct.obj	\
			rdfatsct.obj	\
			fatinfo.obj		\
			sempclst.obj	\
			srchent.obj		\
			sbgedent.obj	\
			setdent.obj		\
			sfatsect.obj	\
			sdatasct.obj	\
			sfatval.obj		\
			setfinfo.obj	\
			slfnent.obj		\
			setpoint.obj	\
			skpsrpos.obj	\
			to_captl.obj	\
			spltpath.obj	\
			submkdir.obj	\
			subopen.obj		\
			subwrite.obj	\
			updata.obj		\
			wrdata.obj		\
			wenddata.obj	\
			oscresem.obj	\
			osgetime.obj	\
			osram.obj		\
			ossigsem.obj	\
			oswaisem.obj

#-------------------------------------------------------#
#   Create FAT library file
#-------------------------------------------------------#
all : clean $(TARGET).lib

fsfatid.obj :	fsfatid.c $(HEADERS)
		$(CC) $(CCFLAGS) fsfatid.c							> x

fsclose.obj :	$(APIDIR)\fsclose.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsclose.c				>> x
	
findclse.obj :	$(APIDIR)\findclse.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\findclse.c				>> x
	
findfrst.obj :	$(APIDIR)\findfrst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\findfrst.c				>> x

findnext.obj:	$(APIDIR)\findnext.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\findnext.c				>> x

fsformat.obj:	$(APIDIR)\fsformat.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsformat.c				>> x

fsinfo.obj:	$(APIDIR)\fsinfo.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsinfo.c					>> x

fsinit.obj:	$(APIDIR)\fsinit.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsinit.c					>> x

fsmkdir.obj:	$(APIDIR)\fsmkdir.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsmkdir.c				>> x

fsmount.obj:	$(APIDIR)\fsmount.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsmount.c				>> x

fsopen.obj:	$(APIDIR)\fsopen.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsopen.c					>> x

fsread.obj:	$(APIDIR)\fsread.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsread.c					>> x

fsremove.obj:	$(APIDIR)\fsremove.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsremove.c				>> x

fsrename.obj:	$(APIDIR)\fsrename.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsrename.c				>> x

fsrmdir.obj:	$(APIDIR)\fsrmdir.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsrmdir.c				>> x

fsseek.obj:	$(APIDIR)\fsseek.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsseek.c					>> x

fssetatr.obj:	$(APIDIR)\fssetatr.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fssetatr.c				>> x

fsstat.obj:	$(APIDIR)\fsstat.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fsstat.c					>> x

fstell.obj:	$(APIDIR)\fstell.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fstell.c					>> x

unmount.obj:	$(APIDIR)\unmount.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\unmount.c				>> x

fswrite.obj:	$(APIDIR)\fswrite.c $(HEADERS)
		$(CC) $(CCFLAGS) $(APIDIR)\fswrite.c				>> x

appddata.obj:	$(COMDIR)\appddata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\appddata.c				>> x

chkpath.obj:	$(COMDIR)\chkpath.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\chkpath.c				>> x

compare.obj:	$(COMDIR)\compare.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\compare.c				>> x

ctrlfat.obj:	$(COMDIR)\ctrlfat.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\ctrlfat.c				>> x

copysect.obj:	$(COMDIR)\copysect.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\copysect.c				>> x

deldata.obj:	$(COMDIR)\deldata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\deldata.c				>> x

deldent.obj:	$(COMDIR)\deldent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\deldent.c				>> x

dellnent.obj:	$(COMDIR)\dellnent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\dellnent.c				>> x

sjis_uni.obj:	$(COMDIR)\sjis_uni.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sjis_uni.c				>> x

uni_sjis.obj:	$(COMDIR)\uni_sjis.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\uni_sjis.c				>> x

expndent.obj:	$(COMDIR)\expndent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\expndent.c				>> x

checksum.obj:	$(COMDIR)\checksum.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\checksum.c				>> x

gdirclst.obj:	$(COMDIR)\gdirclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gdirclst.c				>> x

gempclst.obj:	$(COMDIR)\gempclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gempclst.c				>> x

gfatsect.obj:	$(COMDIR)\gfatsect.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gfatsect.c				>> x

gfatval.obj:	$(COMDIR)\gfatval.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gfatval.c				>> x

gsectnum.obj:	$(COMDIR)\gsectnum.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gsectnum.c				>> x

gshtname.obj:	$(COMDIR)\gshtname.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\gshtname.c				>> x

inident.obj:	$(COMDIR)\inident.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\inident.c				>> x

inifhdl.obj:	$(COMDIR)\inifhdl.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\inifhdl.c				>> x

inisrhdl.obj:	$(COMDIR)\inisrhdl.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\inisrhdl.c				>> x

linkclst.obj:	$(COMDIR)\linkclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\linkclst.c				>> x

fmemcopy.obj:	$(COMDIR)\fmemcopy.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\fmemcopy.c				>> x

fmemset.obj:	$(COMDIR)\fmemset.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\fmemset.c				>> x

fsram.obj:	$(COMDIR)\fsram.c $(HEADERS) $(FATINCDIR)\drvconf.h
		$(CC) $(CCFLAGS) $(COMDIR)\fsram.c					>> x

rdatasct.obj:	$(COMDIR)\rdatasct.c $(HEADERS) $(FATINCDIR)\drvconf.h
		$(CC) $(CCFLAGS) $(COMDIR)\rdatasct.c				>> x

rdfatsct.obj:	$(COMDIR)\rdfatsct.c $(HEADERS) $(FATINCDIR)\drvconf.h
		$(CC) $(CCFLAGS) $(COMDIR)\rdfatsct.c				>> x

fatinfo.obj:	$(COMDIR)\fatinfo.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\fatinfo.c				>> x

sempclst.obj:	$(COMDIR)\sempclst.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sempclst.c				>> x

srchent.obj:	$(COMDIR)\srchent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\srchent.c				>> x

sbgedent.obj:	$(COMDIR)\sbgedent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sbgedent.c				>> x

setdent.obj:	$(COMDIR)\setdent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\setdent.c				>> x

sfatsect.obj:	$(COMDIR)\sfatsect.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sfatsect.c				>> x

sdatasct.obj:	$(COMDIR)\sdatasct.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sdatasct.c				>> x

sfatval.obj:	$(COMDIR)\sfatval.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\sfatval.c				>> x

setfinfo.obj:	$(COMDIR)\setfinfo.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\setfinfo.c				>> x

slfnent.obj:	$(COMDIR)\slfnent.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\slfnent.c				>> x

setpoint.obj:	$(COMDIR)\setpoint.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\setpoint.c				>> x

skpsrpos.obj:	$(COMDIR)\skpsrpos.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\skpsrpos.c				>> x

to_captl.obj:	$(COMDIR)\to_captl.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\to_captl.c				>> x

spltpath.obj:	$(COMDIR)\spltpath.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\spltpath.c				>> x

submkdir.obj:	$(COMDIR)\submkdir.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\submkdir.c				>> x

subopen.obj:	$(COMDIR)\subopen.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\subopen.c				>> x

subwrite.obj:	$(COMDIR)\subwrite.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\subwrite.c				>> x

updata.obj:	$(COMDIR)\updata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\updata.c					>> x

wrdata.obj:	$(COMDIR)\wrdata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\wrdata.c					>> x

wenddata.obj:	$(COMDIR)\wenddata.c $(HEADERS)
		$(CC) $(CCFLAGS) $(COMDIR)\wenddata.c				>> x

oscresem.obj:	$(OSDIR)\oscresem.c $(HEADERS) $(OSDIR)\os.h
		$(CC) $(CCFLAGS) $(OSDIR)\oscresem.c				>> x

osgetime.obj:	$(OSDIR)\osgetime.c $(HEADERS) $(OSDIR)\os.h
		$(CC) $(CCFLAGS) $(OSDIR)\osgetime.c				>> x

osram.obj:	$(OSDIR)\osram.c $(HEADERS) $(OSDIR)\os.h
		$(CC) $(CCFLAGS) $(OSDIR)\osram.c					>> x

ossigsem.obj:	$(OSDIR)\ossigsem.c $(HEADERS) $(OSDIR)\os.h
		$(CC) $(CCFLAGS) $(OSDIR)\ossigsem.c				>> x

oswaisem.obj:	$(OSDIR)\oswaisem.c $(HEADERS) $(OSDIR)\os.h
		$(CC) $(CCFLAGS) $(OSDIR)\oswaisem.c				>> x

$(TARGET).lib : $(OBJS) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(LK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib $(LIBDIR)\*.*

$(TARGET).sub :
	echo	FORM	= library		>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib >>$(TARGET).sub
	echo	INPUT	= $(OBJS)		>>$(TARGET).sub
	echo	LIST	= $(TARGET)		>>$(TARGET).sub
	echo	EXIT					>>$(TARGET).sub

#-----------------------------------------------------------------------#
#	ＭＡＫＥコマンドの定義
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

