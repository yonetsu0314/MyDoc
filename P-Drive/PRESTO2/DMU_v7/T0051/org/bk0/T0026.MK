#*======================================================================
#* File name	: t0026.mk
#* Original 	: p:/presto/dmu_v7/bootload/rcs/r6452.mk 3.1
#*----------------------------------------------------------------------
#* Function 	:
#*		メイクファイル
#*----------------------------------------------------------------------
#*$Header: p:/presto/dmu_v7/bootload/rcs/t0026.mk 1.2 1970/01/01 00:00:00 kagatume Exp $
#*$Log: t0026.mk $
#*リビジョン 1.2  1970/01/01  00:00:00  kagatume
#*2004/05/28 14:30
#*biosのファイル名にサフィフックスを付加したことに伴ってアペンドファイル名
#*をt0017C.motに変更する。
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
TARGET = t0051

#*==========================================================================
#*		Debug Option Definition (0:No Debug, 1:Debug)
#*--------------------------------------------------------------------------
DEBUG	= $(DEBUGMODE)

#*==========================================================================
#*		Directory Definition
#*--------------------------------------------------------------------------
#	ID ファイル格納ディレクトリ
RCS	=P:\PRESTO\DMU_v7\bootload
BIOSDIR	=p:\PRESTO2\DMU_v7\bios

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
INC5 = $(USNET_BASE)\include

AFLAGS	= -cp=sh2 -deb -list
ifeq ($(DEBUG),0)
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INC1),$(INC2),$(INC3),$(INC4),$(INC5) -DEF=far,NDEBUG
else
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INC1),$(INC2),$(INC3),$(INC4),$(INC5) -DEF=far
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
MAKEFILE = t0026.mk
OUTPUT	 = mot
APEND	 = p:\presto\mkinf\MOTAPPND
COPY	 = copy
DEL	 = del

#*==========================================================================
#*		Library File
#*--------------------------------------------------------------------------
LNKLIB	:= $(MY_WORK_BASE)\shlib_v7\NORTI\USERLIB\n4ush2b.lib
#LNKLIB	:= $(LNKLIB), _dscibt3.LIB
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\DRIVE\SH7615\_dscicw.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\drive\fifo\dfifcsh2b.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bicush2b.LIB
#LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bpio.LIB
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\netsrc\usnet.lib
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\drvsrc\usdrv.lib
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\drvsrc\sh7615\uscpu.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\drive\sh7615\_dether.LIB
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\misc\mfssh2b.LIB

LNKLIB2 := $(MY_WORK_BASE)\shlib_v7\misc\dmu5562.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\ROM\romsh2b.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\cnv\cnvsh2b.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\CUSTOM\ctmsh2b.lib
LNKLIB2 := $(LNKLIB2), $(NORTI_BASE)\lib\sh\shc7\n4esh2.lib
LNKLIB2 := $(LNKLIB2), $(SH_COMPILER_BASE)\lib\sh2blib.lib
LNKLIB2 := $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\drive\_fs.LIB
LNKLIB2 := $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\drive\_cf.LIB

LNKLIB3 := $(MY_WORK_BASE)\shlib_v7\biosif\brtcsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\drtc.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\bsrmsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\dsrmsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\be2rsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\de2rsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\de2sr.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\bcomsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\dcomsh2b.LIB

#*==========================================================================
#*		Object Module
#*--------------------------------------------------------------------------
OBJS0 := t0026.obj sect.obj init.obj initsk.obj supapl.obj cmfhdl.obj  cortsk.obj
OBJS0 := $(OBJS0) stat_eth.obj remmon.obj rsinout.obj
OBJS1 := bootcmd1.obj bootcmd2.obj bootcmd3.obj bootsubs.obj bootmisc.obj download.obj
OBJS1 := $(OBJS1) fif001.obj fifinout.obj ledset.obj

#*==========================================================================
#*		Compile & Assemble
#*--------------------------------------------------------------------------
all : $(TARGET).abs

sect.obj : sect.asm $(MAKEFILE)
	$(AS) sect.asm $(AFLAGS) -o=sect.obj

t0026.obj : t0026.c $(MAKEFILE)
	$(CC) $(CFLAGS) t0026.c >> x

init.obj : init.c $(MAKEFILE)
	$(CC) $(CFLAGS) init.c >> x
#	$(CC) $(CCAOPT) $(CFLAGS) init.c >> x
#	$(AS) init.src $(AFLAGS) -o=init.obj

initsk.obj : initsk.c $(MAKEFILE)
	$(CC) $(CFLAGS) initsk.c >> x

cmfhdl.obj : cmfhdl.c $(MAKEFILE)
	$(CC) $(CFLAGS) cmfhdl.c >> x

supapl.obj : supapl.c $(MAKEFILE)
	$(CC) $(CFLAGS) supapl.c >> x

cortsk.obj : cortsk.c $(MAKEFILE)
	$(CC) $(CFLAGS) cortsk.c >> x

fif001.obj : fif001.c $(MAKEFILE)
	$(CC) $(CFLAGS) fif001.c >> x

fifinout.obj : fifinout.c $(MAKEFILE)
	$(CC) $(CFLAGS) fifinout.c >> x

stat_eth.obj : stat_eth.c $(MAKEFILE)
	$(CC) $(CFLAGS) stat_eth.c >> x

bootcmd1.obj : bootcmd1.c $(MAKEFILE)
	$(CC) $(CFLAGS) bootcmd1.c >> x

bootcmd2.obj : bootcmd2.c $(MAKEFILE)
	$(CC) $(CFLAGS) bootcmd2.c >> x

bootcmd3.obj : bootcmd3.c $(MAKEFILE)
	$(CC) $(CFLAGS) bootcmd3.c >> x

bootsubs.obj : bootsubs.c $(MAKEFILE)
	$(CC) $(CFLAGS) bootsubs.c >> x

bootmisc.obj : bootmisc.c $(MAKEFILE)
	$(CC) $(CFLAGS) bootmisc.c >> x

download.obj : download.c $(MAKEFILE)
	$(CC) $(CFLAGS) download.c >> x

remmon.obj : remmon.c $(MAKEFILE)
	$(CC) $(CFLAGS) remmon.c >> x

ledset.obj : ledset.c $(MAKEFILE)
	$(CC) $(CFLAGS) ledset.c >> x

rsinout.obj : rsinout.c $(MAKEFILE)
	$(CC) $(CFLAGS) rsinout.c >> x


$(TARGET).abs : $(OBJS0) $(OBJS1) $(MAKEFILE) $(TARGET).sub
	$(LD) $(LDFLAGS)
	$(APEND) $(BIOSDIR)\t0018.mot $(TARGET).mot temp.mot
	COPY	temp.mot $(TARGET).mot
	DEL	temp.mot

#*==========================================================================
#*		Make Object
#*--------------------------------------------------------------------------
$(TARGET).sub : $(MAKEFILE)
	echo INPUT   = $(OBJS0) 			 > $(TARGET).sub
	echo INPUT   = $(OBJS1) 			>> $(TARGET).sub
	echo LIB     = $(LNKLIB)			>> $(TARGET).sub
	echo LIB     = $(LNKLIB2) 			>> $(TARGET).sub
	echo LIB	 = $(LNKLIB3) 			>> $(TARGET).sub
	echo ROM     = D = R			>> $(TARGET).sub
ifeq ($(DEBUG),0)
	echo START   = P,C,D/00200000		>> $(TARGET).sub
	echo START   = R,B,STACK/06001000	>> $(TARGET).sub
	echo FORM    = STYPE 			>> $(TARGET).sub
else
	echo START   = P,C,D,R,B,STACK/06200000	>> $(TARGET).sub
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
#	echo START P,C,D	(00200000)	>> $(TARGET).sub
#	echo START R,B,STACK(06001000)	>> $(TARGET).sub
#ＤＲＡＭ上で実行用
#	echo START P,C,D,R,B,STACK(06010000) >> $(TARGET).sub
