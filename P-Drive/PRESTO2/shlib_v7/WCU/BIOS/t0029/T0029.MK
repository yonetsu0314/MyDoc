#-----------------------------------------------------------------------*
# File name	: t0029.mk						*
#-----------------------------------------------------------------------*
#$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/t0029.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#$Log: t0029.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2005/02/14 fromio.c m66230io.c を追加
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  kawamura
#* 2004/07/29  11:45
#* 出来上がったファイルを１つ上のディレクトリにコピーする。
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#-----------------------------------------------------------------------*
#		(C) Copyright 1994, ISHIDA CO., LTD.			*
#		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
#		PHONE (0775) 53-4141					*
#-----------------------------------------------------------------------*/
#*==========================================================================
#*		Target filename Definition
#*--------------------------------------------------------------------------
TARGET	= T0029
#*==========================================================================
#*		Debug Option Definition (0:No Debug, 2:Debug)
#*--------------------------------------------------------------------------
DEBUG	= 0
#*--------------------------------------------------------------------------
#*		Board Option Definition
#*--------------------------------------------------------------------------
BOARD	= 
#*==========================================================================
#*		Directory Definition
#*--------------------------------------------------------------------------

#*==========================================================================
#*		Configuration by Debug Option
#*--------------------------------------------------------------------------
ifeq ($(DEBUG),0)
ADBG	= -nodebug
CDBG	= -nodebug -op=1
CDBG1	= -nodebug -op=0
LDBG	= nodebug
else
ADBG	= -debug
CDBG	= -debug -op=0
LDBG	= debug
endif
#*==========================================================================
#*		Option Definition
#*--------------------------------------------------------------------------
INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SH_COMPILER_BASE)\include
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

ASMOPT = $(ADBG) -cpu=sh3 -lis
ifeq ($(DEBUG),2)
CCOPT  = $(CDBG) -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -i=$(INCLIST) -def=APL_SECT=0xc010000
CCAOPT  = $(CDBG) -code=asmcode -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -i=$(INCLIST) -def=APL_SECT=0xc010000
else
CCOPT  = $(CDBG) -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -i=$(INCLIST) -def=APL_SECT=0x00010000
#CCOPT  = $(CDBG) -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -def=APL_SECT=0x12010000
CCAOPT  = $(CDBG) -code=asmcode -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -i=$(INCLIST) -def=APL_SECT=0x00010000
CCAOPT1  = $(CDBG1) -code=asmcode -cpu=sh3 -sp -mess -comm=nonest -sh=e -l -i=$(INCLIST) -def=APL_SECT=0x00010000
endif
#*==========================================================================
#*		Command Definition
#*--------------------------------------------------------------------------
ASM  = asmsh
CC   = shc
LNK  = optlnk
CNVS = optlnk

#*==========================================================================
#*		Library File
#*--------------------------------------------------------------------------
LIB = $(SH_COMPILER_BASE)\lib\sh3blib.lib
#*==========================================================================
#*		Object Module
#*--------------------------------------------------------------------------
OBJS =		reset.obj\
		trap.obj\
		inthd.obj\
		sect.obj\
		vecter.obj\
		setup.obj\
		regster.obj\
		fpga.obj\
		t0029.obj\
		comio.obj\
		dpmemio.obj\
		e2romio.obj\
		fromio.obj\
		icuio.obj\
		ppiio.obj\
		rtcio.obj\
		scuio.obj\
		sramio.obj\
		tcuio.obj\
		fpgaio.obj\
		m66230io.obj\

#*==========================================================================
#*		Compile & Assemble
#*--------------------------------------------------------------------------
all: $(TARGET).abs

reset.obj  : reset.asm
	$(ASM) reset.asm $(ASMOPT)
trap.obj  : trap.asm
	$(ASM) trap.asm $(ASMOPT)
inthd.obj  : inthd.asm
	$(ASM) inthd.asm $(ASMOPT)
sect.obj  : sect.asm
	$(ASM) sect.asm $(ASMOPT)
vecter.obj   : vecter.c vecter.h
	$(CC) $(CCOPT) vecter.c
setup.obj : setup.c
	$(CC) $(CCOPT) setup.c
regster.obj : regster.c
	$(CC) $(CCOPT) regster.c
fpga.obj : fpga.c
	$(CC) $(CCOPT) fpga.c
t0029.obj : t0029.c
	$(CC) $(CCOPT) t0029.c
comio.obj : comio.c
	$(CC) $(CCAOPT) comio.c
	$(ASM) comio.src $(ASMOPT)
dpmemio.obj : dpmemio.c
	$(CC) $(CCAOPT) dpmemio.c
	$(ASM) dpmemio.src $(ASMOPT)
e2romio.obj : e2romio.c
	$(CC) $(CCAOPT) e2romio.c
	$(ASM) e2romio.src $(ASMOPT)
fromio.obj : fromio.c
	$(CC) $(CCAOPT) fromio.c
	$(ASM) fromio.src $(ASMOPT)
icuio.obj : icuio.c
	$(CC) $(CCAOPT) icuio.c
	$(ASM) icuio.src $(ASMOPT)
ppiio.obj : ppiio.c
	$(CC) $(CCAOPT) ppiio.c
	$(ASM) ppiio.src $(ASMOPT)
rtcio.obj : rtcio.c
	$(CC) $(CCAOPT) rtcio.c
	$(ASM) rtcio.src $(ASMOPT)
scuio.obj : scuio.c
	$(CC) $(CCAOPT) scuio.c
	$(ASM) scuio.src $(ASMOPT)
sramio.obj : sramio.c
	$(CC) $(CCAOPT) sramio.c
	$(ASM) sramio.src $(ASMOPT)
tcuio.obj : tcuio.c
	$(CC) $(CCAOPT) tcuio.c
	$(ASM) tcuio.src $(ASMOPT)
fpgaio.obj : fpgaio.c
	$(CC) $(CCAOPT) fpgaio.c
	$(ASM) fpgaio.src $(ASMOPT)
m66230io.obj : m66230io.c
	$(CC) $(CCAOPT1) m66230io.c
	$(ASM) m66230io.src $(ASMOPT)

$(TARGET).abs : $(OBJS) $(TARGET).mk $(TARGET).sub
	$(LNK) -subcommand=$(TARGET).sub
#	$(CNVS) $(TARGET).abs $(TARGET).mot
	copy $(TARGET).mot ..\*.*
#*==========================================================================
#*		Make Object
#*--------------------------------------------------------------------------
$(TARGET).sub : $(TARGET).mk
	echo input  = $(OBJS)				>	$(TARGET).sub
	echo output = $(TARGET)				>>	$(TARGET).sub
	echo list   = $(TARGET)				>>	$(TARGET).sub
	echo show   = symbol				>>	$(TARGET).sub
	echo lib    = $(LIB)				>>	$(TARGET).sub
	echo rom    = D = R				>>	$(TARGET).sub
ifeq ($(DEBUG),2)
	echo form   = stype				>>	$(TARGET).sub
#	echo form   = absolute				>>	$(TARGET).sub
	echo start  = RES/00C000000			>>	$(TARGET).sub
	echo start  = TRP/00C000100			>>	$(TARGET).sub
	echo start  = INT/00C000600			>>	$(TARGET).sub
	echo start  = P,C,D/00C001000			>>	$(TARGET).sub
	echo start  = R,B,BSTK/00C004000		>>	$(TARGET).sub
else
	echo form   = stype				>>	$(TARGET).sub
	echo start  = RES/000000000			>>	$(TARGET).sub
	echo start  = TRP/000000100			>>	$(TARGET).sub
	echo start  = INT/000000600			>>	$(TARGET).sub
	echo start  = P,C,D/000001000			>>	$(TARGET).sub
#	echo start  = RES/012000000			>>	$(TARGET).sub
#	echo start  = TRP/012000100			>>	$(TARGET).sub
#	echo start  = INT/012000600			>>	$(TARGET).sub
#	echo start  = P,C,D/012001000			>>	$(TARGET).sub
	echo start  = R,B,BSTK/00C000000		>>	$(TARGET).sub
endif
#	echo define __STACK(0FFFFFFFC)			>>	$(TARGET).sub
#	echo $(LDBG)					>>	$(TARGET).sub
	echo exit					>>	$(TARGET).sub
#*==========================================================================
#*		ファイル削除
#*--------------------------------------------------------------------------
clean:
	if exist $(TARGET).mot del $(TARGET).mot
	if exist $(TARGET).abs del $(TARGET).abs
	if exist $(TARGET).map del $(TARGET).map
	if exist $(TARGET).sub del $(TARGET).sub
	if exist *.obj         del *.obj
	if exist *.lst         del *.lst
	if exist *.lis         del *.lis
	if exist *.src         del *.src
#*==========================================================================
#*		構成管理ファイルの作成
#*--------------------------------------------------------------------------
release:
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id
#*==========================================================================
