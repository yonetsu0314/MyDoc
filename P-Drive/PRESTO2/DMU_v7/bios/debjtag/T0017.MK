#*======================================================================
#* File name	: t0017.mk
#* Original 	: p:/dacs_i/mcu3/bootload/rcs/l9020.mk 1.14
#*----------------------------------------------------------------------
#* Function 	:
#*	CCW-R:DMU:ブートローダ用BIOSメイクファイル
#*----------------------------------------------------------------------
#*$Header: p:/presto2/dmu_v7/bios/src/rcs/t0017.mk 3.3 1970/01/01 00:00:00 kagatume Exp $
#*$Log: t0017.mk $
#*リビジョン 3.3  1970/01/01  00:00:00  kagatume
#*2005/03/29 19:30
#*presto2ライブラリへアップする。
#*
#*リビジョン 3.2  1970/01/01  00:00:00  kagatume
#*2004/05/11 20:20
#*setup.cをアセンブルするように変更する。
#*
#*リビジョン 3.1  1970/01/01  00:00:00  kagatume
#*2004/04/07 10:00
#*release時にmotファイルをPドライブにアップするように変更する。
#*
#*リビジョン 1.1  1970/01/01  00:00:00  kagatume
#*初期リビジョン
#*
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY
#*		SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#*==========================================================================
#*		Target filename Definition
#*--------------------------------------------------------------------------
TARGET = t0017

#*==========================================================================
#*		Debug Option Definition (0:No Debug, 1:Debug)
#*--------------------------------------------------------------------------
DEBUG	= $(DEBUGMODE)
DEBOPT	= $(DEBUGSW)

#*==========================================================================
#*		Directory Definition
#*--------------------------------------------------------------------------
#	ID ファイル格納ディレクトリ
RCS =P:\PRESTO2\DMU_v7\bios

#*==========================================================================
#*		メイクファイル内部変数定義
#*--------------------------------------------------------------------------
ifdef REV
RCS_DIR	:=$(RCS)\$(TARGET)\id
CHMOD	=attrib +r
RELEASE	:=$(RCS_DIR)\$(TARGET)-$(REV)
else
RELEASE	=release.id
endif

#*==========================================================================
#*		Option Definition
#*--------------------------------------------------------------------------
INC1 = .
INC2 = $(MY_WORK_BASE)\shinc
INC3 = $(SH_COMPILER_BASE)\include
INC4 = $(NORTI_BASE)\inc

AFLAGS	= -cp=sh2 -deb -list
ifeq ($(DEBUG),0)
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INC1),$(INC2),$(INC3),$(INC4) -DEF=far,APL_SECT=0x00010000,INTVEC=0x06000000
else
ifeq ($(DEBUG),1)
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INC1),$(INC2),$(INC3),$(INC4) -DEF=far,APL_SECT=0x06010000,INTVEC=0x06000000
else
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INC1),$(INC2),$(INC3),$(INC4) -DEF=far,APL_SECT=0x06200000,INTVEC=0x06000000
endif
endif
LDFLAGS = -su=$(TARGET).sub
CCAOPT	= -code=asmcode

#*==========================================================================
#*		Command Definition
#*--------------------------------------------------------------------------
AS	 = asmsh
CC	 = shc
CCA	 = shc $(CCAOPT)
LD	 = optlnk
CNV 	 = optcnvs
MAKE	 = make
MAKEFILE = $(TARGET).mk
OUTPUT	 = mot
COPY	 = copy
DEL	 = del

#*==========================================================================
#*		Library File
#*--------------------------------------------------------------------------
LNKLIB := $(SH_COMPILER_BASE)\lib\sh2blib.lib
LNKLIB := $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\misc\dmu5562.lib

#*==========================================================================
#*		Object Module
#*--------------------------------------------------------------------------
OBJS0 := t0017.obj sect.obj regster.obj setup.obj vecter.obj icuio.obj fromio.obj cfio.obj
OBJS1 := e2romio.obj sramio.obj rtcio.obj portio.obj tcuio.obj comio.obj fpgaio.obj
OBJS2 := e2sromio.obj scuio.obj

#*==========================================================================
#*		Compile & Assemble
#*--------------------------------------------------------------------------
all : $(TARGET).abs

sect.obj : sect.asm $(MAKEFILE)
	$(AS) sect.asm $(AFLAGS) -o=sect.obj

t0017.obj : t0017.c $(MAKEFILE)
	$(CC) $(CFLAGS) t0017.c 						> x

regster.obj : regster.c $(MAKEFILE)
	$(CC) $(CFLAGS) regster.c 						>> x

setup.obj : setup.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) setup.c 				>> x
	$(AS) setup.src $(AFLAGS)
#	$(AS) setup.src $(AFLAGS) -o=setup.obj
#	$(CC) $(CFLAGS) setup.c >> x

vecter.obj : vecter.c $(MAKEFILE)
	$(CC) $(CFLAGS) vecter.c 						>> x

tcuio.obj : tcuio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) tcuio.c				>> x
	$(AS) tcuio.src $(AFLAGS)	

icuio.obj : icuio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) icuio.c				>> x
	$(AS) icuio.src $(AFLAGS)	

e2romio.obj : e2romio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) e2romio.c				>> x
	$(AS) e2romio.src $(AFLAGS)	

sramio.obj : sramio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) sramio.c				>> x
	$(AS) sramio.src $(AFLAGS)	

rtcio.obj : rtcio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) rtcio.c				>> x
	$(AS) rtcio.src $(AFLAGS)	

portio.obj : portio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) portio.c				>> x
	$(AS) portio.src $(AFLAGS)	

comio.obj : comio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) comio.c				>> x
	$(AS) comio.src $(AFLAGS)	

fpgaio.obj : fpgaio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) fpgaio.c				>> x
	$(AS) fpgaio.src $(AFLAGS)	

fromio.obj : fromio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) fromio.c				>> x
	$(AS) fromio.src $(AFLAGS)	

cfio.obj : cfio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) cfio.c				>> x
	$(AS) cfio.src $(AFLAGS)	

e2sromio.obj : e2sromio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) e2sromio.c			>> x
	$(AS) e2sromio.src $(AFLAGS)	

scuio.obj : scuio.c $(MAKEFILE)
	$(CC) $(CCAOPT) $(CFLAGS) scuio.c				>> x
	$(AS) scuio.src $(AFLAGS)	

$(TARGET).abs : $(OBJS0) $(OBJS1) $(OBJS2) $(MAKEFILE) $(TARGET).sub
	$(LD) $(LDFLAGS)

#*==========================================================================
#*		Make Object
#*--------------------------------------------------------------------------
$(TARGET).sub : $(MAKEFILE)
	echo INPUT   = $(OBJS0) 		 > $(TARGET).sub
	echo INPUT   = $(OBJS1) 		>> $(TARGET).sub
	echo INPUT   = $(OBJS2) 		>> $(TARGET).sub
	echo LIB     = $(LNKLIB)			>> $(TARGET).sub
	echo ROM     = D = R			>> $(TARGET).sub
ifeq ($(DEBUG),0)
	echo START   = CVECT/00000000		>> $(TARGET).sub
	echo START   = P,C,D/00001000		>> $(TARGET).sub
	echo START   = R,B,STACK/06f80000	>> $(TARGET).sub
	echo FORM    = STYPE 			>> $(TARGET).sub
else
	echo START   = CVECT,P,C,D/06001000	>> $(TARGET).sub
	echo START   = R,B,STACK/06f80000	>> $(TARGET).sub
	echo FORM    = ABSOLUTE 		>> $(TARGET).sub
endif
	echo OUTPUT  = $(TARGET)			>> $(TARGET).sub
	echo LIST    = $(TARGET)			>> $(TARGET).sub
	echo SHOW    = SYMBOL			>> $(TARGET).sub
	echo EXIT				>> $(TARGET).sub

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
	if exist *.obj DEL *.obj
	if exist *.abs DEL *.abs
	if exist *.mot DEL *.mot
	if exist *.map DEL *.map
	if exist *.lis DEL *.lis
	if exist *.sub DEL *.sub

release :
	@echo File Release...
ifdef REV
	COPY	$(TARGET).$(OUTPUT) $(RCS)\$(TARGET)$(VER).$(OUTPUT)
endif
	if exist *.mk	ident	*.mk	>  $(RELEASE)
	if exist *.h	ident	*.h	>> $(RELEASE)
	if exist *.c	ident	*.c	>> $(RELEASE)
	if exist *.asm	ident	*.asm	>> $(RELEASE)
	$(CHMOD) $(RELEASE) > nul


#アドレス0x200000にアプリをロード出来るブートローダの位置
#	echo START CVECT	(00000000)	>> $(TARGET).sub
#	echo START P,C,D	(00001000)	>> $(TARGET).sub
#	echo START R,B,STACK(06f80000)	>> $(TARGET).sub
#ＤＲＡＭ上で実行用
#	echo START CVECT,P,C,D,R,B,STACK(06000800) >> $(TARGET).sub
