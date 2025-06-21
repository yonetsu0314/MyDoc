#*======================================================================
#* File name	: dmubtwr.mk
#* Original 	: p:/dacs_i/mcu3/bootload/rcs/l9020.mk 1.14
#*----------------------------------------------------------------------
#* Function 	:
#*		メイクファイル
#*----------------------------------------------------------------------
#*$Header: p:/presto2/dmu_v7/bootload/rcs/dmubtwr.mk 5.1 1970/01/01 00:00:00 kagatume Exp $
#*$Log: dmubtwr.mk $
#*リビジョン 5.1  1970/01/01  00:00:00  kagatume
#*2005/6/27 13:30
#*電断対応ファイルシステム搭載
#*
#*リビジョン 4.1  1970/01/01  00:00:00  kagatume
#*2004/05/26 14:30
#*ブートローダのプログラム番号をt0026に変更したことに対応する
#*
#*リビジョン 3.1  1970/01/01  00:00:00  kagatume
#*2004/04/07 10:00
#*・コンパイル時にbiosを自動アペンドするように変更する。
#*・release時にmotファイルをPドライブにアップするように変更する。
#*
#*リビジョン 2.2  1970/01/01  00:00:00  kagatume
#*2004/03/13 13:30
#*・前リビジョンの変更を元に戻す
#*・trash.cをコンパイルしないようにする
#*
#*リビジョン 2.1  1970/01/01  00:00:00  kagatume
#*2004/03/09 10:00
#*init.cをアセンブルするように変更
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
TARGET = DMUBTWR

#*==========================================================================
#*		Directory Definition
#*--------------------------------------------------------------------------
#	ID ファイル格納ディレクトリ
RCS	=P:\PRESTO2\DMU_v7\bootload
BIOSDIR	=P:\PRESTO2\DMU_v7\bios

#*==========================================================================
#*		メイクファイル内部変数定義
#*--------------------------------------------------------------------------
#ifdef REV
#RCS_DIR	:=$(RCS)\$(TARGET)\id
#CHMOD	=attrib +r
#RELEASE	:=$(RCS_DIR)\$(TARGET)-$(REV)
#else
RELEASE	=release.id
#endif

#*==========================================================================
#*		Option Definition
#*--------------------------------------------------------------------------
INCLIST := .
INCLIST := $(INCLIST),$(MY_WORK_BASE)\shinc
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc
INCLIST := $(INCLIST),$(USNET_BASE)\include

AFLAGS	= -cp=sh2 -deb -list -DEF=BTWRITER="BTWR"
CFLAGS	= -cp=sh2 -op=1 -me -deb -comment=nest -i=$(INCLIST) -DEF=far,NDEBUG,BTWRITER
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
MAKEFILE = dmubtwr.mk
OUTPUT	 = mot
APEND	 = p:\presto2\mkinf\MOTAPPND
COPY	 = copy
DEL	 = del

#*==========================================================================
#*		Library File
#*--------------------------------------------------------------------------
LNKLIB	:= $(MY_WORK_BASE)\shlib_v7\NORTI\USERLIB\n4ush2b.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\DRIVE\SCU\sh7615\_dscic.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bscush2b.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\drive\fifo\dfifcsh2b.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bfifsh2b.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bicush2b.LIB
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\biosif\bppish2b.lib
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\netsvr60\usnet.lib
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\drvsrc\usdrv.lib
LNKLIB	:= $(LNKLIB), $(USNET_BASE)\drvsrc\sh7615\uscpu.lib
LNKLIB	:= $(LNKLIB), $(MY_WORK_BASE)\shlib_v7\drive\sh7615\_dether.LIB

LNKLIB2 := $(MY_WORK_BASE)\shlib_v7\misc\dmu5562.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\ROM\romsh2b.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\cnv\cnvsh2b.LIB
LNKLIB2	:= $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\CUSTOM\ctmsh2b.lib
LNKLIB2 := $(LNKLIB2), $(NORTI_BASE)\lib\sh\shc7\n4esh2.lib
LNKLIB2 := $(LNKLIB2), $(SH_COMPILER_BASE)\lib\sh2blib.lib
LNKLIB2 := $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\drive\_cf.LIB
LNKLIB2 := $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\biosif\bcfsh2b.LIB
LNKLIB2 := $(LNKLIB2), $(MY_WORK_BASE)\shlib_v7\biosif\bfrsh2b.LIB

LNKLIB3 := $(MY_WORK_BASE)\shlib_v7\biosif\brtcsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\drtc.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\bsrmsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\dsrmsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\be2rsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\de2rsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\de2sr.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\be2ssh2b.lib
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\biosif\bcomsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\shlib_v7\drive\dcomsh2b.LIB
LNKLIB3	:= $(LNKLIB3), $(MY_WORK_BASE)\SHLIB_V7\BIOSIF\binish2b.lib

LNKLIB4	:= $(MY_WORK_BASE)\shlib_v7\drive\fssvr60\lib\libfat.lib
LNKLIB4	:= $(LNKLIB4), $(MY_WORK_BASE)\shlib_v7\drive\fssvr60\lib\libram.lib
LNKLIB4	:= $(LNKLIB4), $(MY_WORK_BASE)\shlib_v7\drive\fssvr60\lib\libata.lib
LNKLIB4	:= $(LNKLIB4), $(MY_WORK_BASE)\shlib_v7\drive\fssvr60\lib\libutl.lib
LNKLIB4	:= $(LNKLIB4), $(MY_WORK_BASE)\shlib_v7\drive\fssvr60\lib\libutl2.lib

#*==========================================================================
#*		Object Module
#*--------------------------------------------------------------------------
OBJS0 := t0026.obj sect.obj init.obj initsk.obj supapl.obj cmfhdl.obj  cortsk.obj
OBJS0 := $(OBJS0) stat_eth.obj remmon.obj rsinout.obj
OBJS1 := bootcmd1.obj bootcmd2.obj bootcmd3.obj bootsubs.obj bootmisc.obj download.obj
OBJS1 := $(OBJS1) fif001.obj fifinout.obj ledset.obj flashf.obj

#*==========================================================================
#*		Compile & Assemble
#*--------------------------------------------------------------------------
all : $(TARGET).abs

sect.obj : sect.asm $(MAKEFILE)
	$(AS) sect.asm $(AFLAGS) -o=sect.obj > x

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

flashf.obj : flashf.c $(MAKEFILE)
	$(CC) $(CFLAGS) flashf.c >> x

$(TARGET).abs : $(OBJS0) $(OBJS1) $(MAKEFILE) $(TARGET).sub
	$(LD) $(LDFLAGS)
	$(APEND) $(BIOSDIR)\t0017btw.mot $(TARGET).mot temp.mot
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
	echo LIB     = $(LNKLIB4) 			>> $(TARGET).sub
	echo ROM     = D = R			>> $(TARGET).sub
	echo START   = VECT/00200000		>> $(TARGET).sub
	echo START   = P,C,D/00210000		>> $(TARGET).sub
	echo START   = R,B,STACK/06e00000	>> $(TARGET).sub
	echo FORM    = STYPE 			>> $(TARGET).sub
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
#	@echo File Release...
#ifdef REV
	COPY	$(TARGET).$(OUTPUT) $(RCS)\$(TARGET).$(OUTPUT)
#endif
	if exist *.mk	ident	*.mk	>  $(RELEASE)
	if exist *.h	ident	*.h	>> $(RELEASE)
	if exist *.c	ident	*.c	>> $(RELEASE)
	if exist *.asm	ident	*.asm	>> $(RELEASE)
#	$(CHMOD) $(RELEASE) > nul

#======================================================================#
#====                   ***** END OF FILE *****                    ====#
#======================================================================#
