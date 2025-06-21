#*======================================================================
#* File name    : r4308
#* Original	: 
#*----------------------------------------------------------------------
#* Function 	:
#*	メイクファイル
#*----------------------------------------------------------------------
#*$Header: p:/mhic/main/ccw/rcs/r4308.mk 1.3 1970/01/01 00:00:00 ryoji Exp $
#*$Log: r4308.mk $
#*----------------------------------------------------------------------
#*		(c) Copyright 1994-1998, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
#*		SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#
#	<<< ファイルネーム定義 >>>
#
#	 Target file name

PROG	=t4308

#*======================================================================
#
#		<<< デバッグ情報の定義 >>>
#
#*======================================================================
#	 0:MOT, 1:ABS
#
ifndef	DEBUG
DEBUG	=0
endif
#*======================================================================
#
#		<<< ディレクトリー定義 >>>
#
#*======================================================================
ifndef	DMU_BASE
DMU_BASE	=P:\PRESTO2
endif

ifndef	DMUDIR
DMUDIR		=\DMU
endif

#	Usnetディレクトリ
ifndef	USNET_BASE
USNET_BASE	=$(DMU_BASE)\shlib_v7\drive\usnets\usnetsh7
endif

#	Perlスクリプトディレクトリ
ifndef	PERL5DIR
PERL5DIR	=$(DMU_BASE)$(DMUDIR)\perl
endif

#	Perlスクリプトディレクトリ
ifndef	PERL5DIR
PERL5DIR	=$(DMU_BASE)$(DMUDIR)\perl
endif

ifndef	PERL5
PERL5	=p:\tool\perl\ver5win3\Perl
endif

#	ID ファイル格納ディレクトリ
RCS =.\

#*======================================================================
#
#		<<<メイクファイル内部変数定義>>>
#
#=======================================================================#

NEWCOLST=newcosrc
REVCOLST=revcosrc
RELLST	=relsrc

ifdef REV
RCS_DIR	:=$(RCS)\$(PROG)
CHMOD	=attrib +r
RELEASE	:=$(RCS_DIR)\$(PROG)-$(REV)
else
RELEASE	=release.id
endif

#=======================================================================#
#
#		<<<コマンド設定>>>
#
#=======================================================================#
#	デバッグモードの指定（0:MOT, 1:ABS）
ifeq ($(DEBUG),0)
CCOPT	= -nodebug -op=1
DEFINE	= far,near,NDEBUG
else
CCOPT 	= -debug -op=0
DEFINE	= far,near,DEBUG
endif

INCLIST := $(DMU_BASE)$(DMUDIR)\include\bank
INCLIST := $(INCLIST),$(DMU_BASE)$(DMUDIR)\INCLUDE
INCLIST := $(INCLIST),$(DMU_BASE)\SHINC
INCLIST := $(INCLIST),$(DMU_BASE)\norti_12\inc
INCLIST := $(INCLIST),$(USNET_BASE)\include

# 以下は暫定処置
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),.
INCLIST := $(INCLIST),.\plcif

ASM		=	asmsh
CC		=	shc
CCOPT	:=	$(CCOPT) -cp=sh2 -mes -i=$(INCLIST) -def=$(DEFINE)
LINK	=	optlnk
CNV		=	optcnvs
LDFLAGS	=	-su=$(PROG).sub
MKOPT	=	echo
OUTPUT	=	mot
MAKEFILE=	$(PROG).mk
PLCDIR	=	plcif
PLCDIR2	=	plcif2
APEND	= p:\presto2\mkinf\MOTAPPND

#=======================================================================#
#		ライブラリー･ファイル定義の定義				#
#=======================================================================#
# CCW 機種固有のLibrary
LIB1	:= $(DMU_BASE)\SHLIB_V7\RZCMD\rzcsh2b.lib
# MiddleWare
LIB2	:= $(USNET_BASE)\NETSVR60\usnet.lib
LIB2	:= $(LIB2), $(USNET_BASE)\drvsrc\usdrv.lib
LIB2	:= $(LIB2), $(USNET_BASE)\drvsrc\sh7615\uscpu.lib
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\sh7615\_dether.lib
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\_cf.lib
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\FSSVR60_1_9\LIB\libram.lib#ファイルシステム用SRAMドライバ
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\FSSVR60_1_9\LIB\libata.lib#ファイルシステム用CFドライバ
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\FSSVR60_1_9\LIB\libutl.lib#スキャンディスク
LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\FSSVR60_1_9\LIB\libutl2b.lib#電断対策機能（イシダ製）、ファイルシステムI/F、シェルI/F関連
#LIB2	:= $(LIB2), $(DMU_BASE)\SHLIB_V7\DRIVE\_fs.lib

# Driever
LIB3	:= $(DMU_BASE)\SHLIB_V7\DRIVE\SCU\SH7615\_dscic_.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\CUSTOM\ctmsh2b.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\drtc.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\dcomash2b.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\dtmrsh2b.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\dsrmsh2b.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\de2sr.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\fifo\dfifcsh2b.lib
LIB3	:= $(LIB3), $(DMU_BASE)\SHLIB_V7\DRIVE\dppish2b_.lib

#BIOS-IF / BIOS / MISC
LIB4	:= $(DMU_BASE)\SHLIB_V7\BIOSIF\brtcsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bicush2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bcomsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bsrmsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bfifsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bcfsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bfrsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\be2ssh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\binish2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bscush2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\BIOSIF\bppish2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\CNV\cnvsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\MISC\mnetsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\MISC\marcsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\DRIVE\FSSVR60_1_9\LIB\libfata.lib
#LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\MISC\mfssh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\ROM\romsh2b.LIB
LIB4	:= $(LIB4), $(DMU_BASE)\SHLIB_V7\MISC\dmu5562.lib

#SH-C Compiler / NORT-i
LIB5	:= $(DMU_BASE)\SHLIB_V7\NORTI\USERLIB\n4ush2b.lib
#LIB5	:= $(LIB5), $(DMU_BASE)\NORTI\LIB\SH\DMU\n4esh2.lib
LIB5	:= $(LIB5), $(DMU_BASE)\NORTI_12\LIB\SH\SHC7\n4esh2.lib
LIB5	:= $(LIB5), $(SH_COMPILER_BASE)\lib\sh2blib.lib


#=======================================================================#
#		オブジェクトリストの定義				#
#=======================================================================#
#asm module
OBJS0	:=$(OBJS0) aplinf.obj

#obj module
OBJS0	:=$(OBJS0) shcstart.obj
OBJS0	:=$(OBJS0) cmfhdl.obj
OBJS0	:=$(OBJS0) hir001.obj
OBJS0	:=$(OBJS0) hsr001.obj
OBJS0	:=$(OBJS0) icr001.obj
OBJS0	:=$(OBJS0) initsk.obj
OBJS0	:=$(OBJS0) rcr001.obj
OBJS0	:=$(OBJS0) $(PROG).obj
OBJS0	:=$(OBJS0) supapl.obj
OBJS0	:=$(OBJS0) swi001.obj
OBJS0	:=$(OBJS0) ax0rtsk.obj
OBJS0	:=$(OBJS0) ax1rtsk.obj
OBJS0	:=$(OBJS0) ax2rtsk.obj
OBJS0	:=$(OBJS0) ax3rtsk.obj
OBJS0	:=$(OBJS0) ax4rtsk.obj
OBJS0	:=$(OBJS0) ax5rtsk.obj
OBJS0	:=$(OBJS0) cor0tsk.obj
OBJS0	:=$(OBJS0) cor1tsk.obj
OBJS0	:=$(OBJS0) sumfnc.obj
OBJS0	:=$(OBJS0) fnctbl.obj
OBJS0	:=$(OBJS0) pcom001.obj
OBJS0	:=$(OBJS0) pcom002.obj
OBJS0	:=$(OBJS0) pcom003.obj
OBJS0	:=$(OBJS0) mbx003.obj
OBJS0	:=$(OBJS0) adrs.obj
OBJS0	:=$(OBJS0) axwupc.obj
OBJS0	:=$(OBJS0) decimal.obj
OBJS0	:=$(OBJS0) romno.obj
OBJS0	:=$(OBJS0) rpktchg.obj
OBJS0	:=$(OBJS0) led001.obj

OBJS_DMU	:=$(OBJS0)

#=======================================================================#
#		ターゲット生成（ターゲット依存リスト）			#
#=======================================================================#
$(PROG).abs : $(OBJS_DMU) $(OBJS_PLCIF) $(MAKEFILE) $(PROG).sub
	$(LINK) $(LDFLAGS)
ifeq ($(DEBUG),0)
	$(APEND) p:\PRESTO2\DMU_v7\bios\t0018.mot $(PROG).mot temp.mot
	COPY	temp.mot $(PROG).mot
	DEL	temp.mot
	$(PERL5) $(DMU_BASE)\mkinf\mkhed.pl $(PROG).c > $(PROG).hed
else
	dw22ctx.exe -ex48 $(PROG).abs
endif

#=======================================================================#
#		コンパイルリスト					#
#=======================================================================#
aplinf.obj  : aplinf.asm
	$(ASM) aplinf.asm $(ASMOPT)
shcstart.obj  : shcstart.c
	$(CC) $(CCOPT) shcstart.c
$(PROG).obj : $(PROG).c
	$(CC) $(CCOPT) $(PROG).c
cmfhdl.obj  : cmfhdl.c
	$(CC) $(CCOPT) cmfhdl.c
hir001.obj  : hir001.c
	$(CC) $(CCOPT) hir001.c
hsr001.obj  : hsr001.c
	$(CC) $(CCOPT) hsr001.c
icr001.obj  : icr001.c
	$(CC) $(CCOPT) icr001.c
initsk.obj  : initsk.c
	$(CC) $(CCOPT) initsk.c
rcr001.obj  : rcr001.c
	$(CC) $(CCOPT) rcr001.c
supapl.obj  : supapl.c
	$(CC) $(CCOPT) supapl.c
swi001.obj  : swi001.c
	$(CC) $(CCOPT) swi001.c
ax0rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax0rtsk.c
ax1rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax1rtsk.c
ax2rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax2rtsk.c
ax3rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax3rtsk.c
ax4rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax4rtsk.c
ax5rtsk.obj  : ax0rtsk.c
	$(CC) $(CCOPT) ax5rtsk.c
cor0tsk.obj  : cor0tsk.c
	$(CC) $(CCOPT) cor0tsk.c
cor1tsk.obj  : cor1tsk.c
	$(CC) $(CCOPT) cor1tsk.c
sumfnc.obj  : sumfnc.c
	$(CC) $(CCOPT) sumfnc.c
fnctbl.obj  : fnctbl.c
	$(CC) $(CCOPT) fnctbl.c
pcom001.obj  : pcom001.c
	$(CC) $(CCOPT) pcom001.c
pcom002.obj  : pcom002.c
	$(CC) $(CCOPT) pcom002.c
pcom003.obj  : pcom002.c
	$(CC) $(CCOPT) pcom003.c
mbx003.obj  : mbx003.c
	$(CC) $(CCOPT) mbx003.c
adrs.obj  : adrs.c
	$(CC) $(CCOPT) adrs.c
axwupc.obj  : axwupc.c
	$(CC) $(CCOPT) axwupc.c
decimal.obj  : decimal.c
	$(CC) $(CCOPT) decimal.c
romno.obj  : romno.c
	$(CC) $(CCOPT) romno.c
rpktchg.obj  : rpktchg.c
	$(CC) $(CCOPT) rpktchg.c
led001.obj  : led001.c
	$(CC) $(CCOPT) led001.c

#=======================================================================#
#		ソースファイル依存リスト				#
#=======================================================================#
$(PROG).abs:	$(MAKEFILE)

#-----------------------------------------------------------------------#
#			ロケート情報ファイルの生成			#
#-----------------------------------------------------------------------#
$(PROG).sub : $(MAKEFILE)
	$(MKOPT) INPUT   = $(OBJS0)			> $(PROG).sub
	$(MKOPT) LIB     = $(LIB1)			>> $(PROG).sub
	$(MKOPT) LIB     = $(LIB2)			>> $(PROG).sub
	$(MKOPT) LIB     = $(LIB3)			>> $(PROG).sub
	$(MKOPT) LIB     = $(LIB4)			>> $(PROG).sub
	$(MKOPT) LIB     = $(LIB5)			>> $(PROG).sub
	$(MKOPT) ROM     = D = R			>> $(PROG).sub
ifeq ($(DEBUG),1)
	$(MKOPT) START   = P,C,D,R,B,STACK/06200000	>> $(PROG).sub
	$(MKOPT) FORM    = ABSOLUTE			>> $(PROG).sub
else
	$(MKOPT) START   = CVECT/00200000		>> $(PROG).sub
	$(MKOPT) START   = P,C,D/00210000		>> $(PROG).sub
	$(MKOPT) START   = R,B,STACK/6001000		>> $(PROG).sub
	$(MKOPT) FORM    = STYPE			>> $(PROG).sub
endif
	$(MKOPT) OUTPUT  = $(PROG)			>> $(PROG).sub
	$(MKOPT) LIST    = $(PROG)			>> $(PROG).sub
	$(MKOPT) SHOW    = SYMBOL			>> $(PROG).sub
	$(MKOPT) EXIT					>> $(PROG).sub

#=======================================================================#
#			コマンド定義					#
#=======================================================================#

# 中間ファイル／ターゲットファイルの削除
clean:
	if exist *.obj		del *.obj
	if exist $(PROG).mot	del $(PROG).mot
	if exist $(PROG).abs	del $(PROG).abs
	if exist $(PROG).sub	del $(PROG).sub
	if exist $(PROG).map	del $(PROG).map

#	リリース時の手続き
#	(例) make -fr00xx.mk release
#

release :	$(RELLST)
	@echo File Release...
ifdef REV
	copy	$(PROG).$(OUTPUT) $(RCS)\$(PROG)$(VER).$(OUTPUT)
	copy	$(PROG).hed $(RCS)\$(PROG)$(VER).hed
endif

relsrc:
	if exist *.mk		ident	*.mk	>  $(RELEASE)
	if exist *.h		ident	*.h	>> $(RELEASE)
	if exist *.c		ident	*.c	>> $(RELEASE)
	if exist *.asm		ident	*.asm	>> $(RELEASE)
	$(CHMOD) $(RELEASE) > nul
#

# ID ファイルを元にソースファイルをチェックアウトする
# リビジョン指定
#	(例) make -S -frxxxx.mk -DREV=1.1 revco
revco:	$(REVCOLST)

# 最新リビジョン
#	(例) make -S -frxxxx.mk -DREV=1.1 newco
newco:	$(NEWCOLST)

# サブコマンド
newcosrc:	$(RCS)\$(PROG)-$(REV)
	p:\tool\perl\newco.bat $(RCS)\$(PROG)-$(REV)
revcosrc:	$(RCS)\$(PROG)-$(REV)
	p:\tool\perl\revco.bat $(RCS)\$(PROG)-$(REV)

#=======================================================================#
#			END OF FILE													#
#=======================================================================#
