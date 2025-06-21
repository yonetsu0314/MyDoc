#----------------------------------------------------------------------*
# File name	: compiler.mak                                              *
#----------------------------------------------------------------------*
#$Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/config/sh7615/hitachi/rcs/compiler.mak 1.2 1970/01/01 00:00:00 sakaguti Exp $
#$Log: compiler.mak $
# リビジョン 1.2  1970/01/01  00:00:00  sakaguti
# ver7 vup
# 
# リビジョン 1.1  1970/01/01  00:00:00  masahiro
# 初期リビジョン
# 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.                           *
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       *
#		PHONE (077) 553-4141                                           *
#----------------------------------------------------------------------*/
#
#  +++=>>> * %n  Edition: %v  Last changed: %f  By: %w *
# * COMPILER.MAK  Edition: 2  Last changed: 18-Sep-97,8:30:32  By: NICK *
#  +++=>>> '%l'
#
# Hitachi C Unix makefile for Hitachi SH no multitasker
# Copyright (C) 1993, 1995, 1997 United States Software Corporation
#


ENDIAN=BIG              #Motorola byte order BIG, Intel LITTLE
O=obj                      #object extension
L=lib                      #library extension
E=abs                      #executable extension
RAM=0                      # 0 - execute in ROM
CACHE=0                    # 1 - enable cache
NTRACE=0            #trace level, 0=none, 9=maximum  //saka
#NTRACE=1            #trace level, 0=none, 9=maximum
#
#  Define the file types.  This is used for removing
#  files when "make clean" is called from $(USROOTDIR).
#
%set FILETYPES=$O $L $E err def sub lst map bak lnk tmp out mot lis


#==============================================================
#
# user options 
#

# # # # # # # # # # 
#
#  Select platform by including appropriate support
#  makefile. 
#
# # # # # # # # # # 
# 
# Select target SH processor.  3 only.
#
TARGET = 3              #Target SH3 processor

#
#  Point to hitachi compiler
#
PTH = L:\SH2\HEW\TOOLS\HITACHI\SH\7_1_1
# PTH = p:\presto\hew\tools\hitachi\sh\7_1_1       #where the compiler is

# ======================================================
#
# path and tool names
#
LPTH = $(PTH)\lib       # Where the C runtime library is
CC = $(PTH)\bin\shc     # The compiler
AS = $(PTH)\bin\asmsh   # The assembler
LNK = $(PTH)\bin\lnk    # The linker
#LIBR = $(PTH)\bin\lbr    # The linker
LIBR = $(PTH)\bin\optlnk    # The linker
CNVS = $(PTH)\bin\cnvs   # The object converter

#
# Debug flag
#
DBG = -debug
#DBG = -nodebug

#
# Secifiy library to link with.  See compiler documentation
# for more information.
#   
%if $(ENDIAN) == LITTLE
 SHLIB =                 # BIG only with SH2....
%else
 SHLIB = shcnpic.lib     # SH2
%endif

#
#  Compile flags.  Need to create a subcommand file
#  because of the lenght of options.
#

CFLAGS_FILE = $(BLDPTH)\cflags.sub
%echo -optimize=1 > $(CFLAGS_FILE)
#%echo -optimize=0 > $(CFLAGS_FILE)
%echo -cpu=sh2 >> $(CFLAGS_FILE)
%echo -show=source,object >> $(CFLAGS_FILE)
%echo -endian=$(ENDIAN) >> $(CFLAGS_FILE)
%echo $(DBG) >> $(CFLAGS_FILE)
%echo -n -include=( >> $(CFLAGS_FILE)
FIRST = .
%foreach INCS $(INCPTHS)
 %ifndef FIRST
   %echo -n , >> $(CFLAGS_FILE)
 %endif
 %undef FIRST
 %echo -n $(INCS) >> $(CFLAGS_FILE)
%end
%echo ) >> $(CFLAGS_FILE)
%echo -def=$(ENDIAN),NTRACE=$(NTRACE) >> $(CFLAGS_FILE)
%echo -listfile >> $(CFLAGS_FILE)
####%echo -code = asmcode >> $(CFLAGS_FILE)

CFLAGS =  -sub=$(CFLAGS_FILE)
        

#
# Assembler flags
#
AFLAGS=-cpu=sh2 -endian=$(ENDIAN) -list -assigna=RAM=$(RAM) -assigna=CACHE=$(CACHE)

#
# Flag to add a library to a library
#
LIB_ADD_FLAG = /ADD=
     
#
#  Point to the startup file:  This may need to be changed
#  depending on target processor or a new start up file may
#  need to be written. See the makefile.sh# file.
#  SH1 and SH2 support use the same start file.  
#  There is no SH3 start file.  Code start was performed by 
#  the eval boards code monitor.  Once you have written the 
#  start code for you SH3 board, enter the file name in makefile.sh3.
#
START_UP = $(DRVPTH)\$(CPU)\$(COMPILER)\start.obj

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Setup environment variables used by SHC
#
#   %setenv SHC_LIB=$(PTH)\BIN
#   %setenv SHC_INC=$(PTH)\BIN
#   %setenv SHC_TMP=C:\USNET
#   %setenv DOS16M= @1M-31M

#===========================================================
#
# default rules 
#
.SUFFIXES : .c .s .obj .lst .abs .hex .run

.c.obj:
        $(CC) $< $(CFLAGS)
##        $(AS) $*.src $(AFLAGS)

.s.obj:
        $(AS) $< $(AFLAGS)

.obj.abs:
    %set SUB_FILE = $(BLDPTH)\usnet.sub
    %echo input $(START_UP) > $(SUB_FILE)
    %echo input $*.$(O) >> $(SUB_FILE)
    %foreach OBJ $(OBJDEPENDS)
      %echo input $(OBJ).$(O) >> $(SUB_FILE)
    %end
    %foreach LIBRARY $(STDLIBS)
      %echo library $(LIBPTH)\$(LIBRARY).$(L) >> $(SUB_FILE)
    %end
    %foreach LIBRARY $(OPTLIBS)
      %echo library $(LIBPTH)\$(LIBRARY).$(L) >> $(SUB_FILE)
    %end
    %echo library $(LPTH)\$(SHLIB) >> $(SUB_FILE)
    %echo output $* >> $(SUB_FILE)
    %echo print $*.map >> $(SUB_FILE)
%if $(RAM) == 1
    %echo start Entry,S(26000000),P,C,D,B,Vector(06000800) >> $(SUB_FILE)
%else   # rom
    %echo ROM (D,R) >> $(SUB_FILE)
    %echo start S,P,C,D(00002000),R,B,STACK(06000000),Vector(1000E000),STK(1001E000) >> $(SUB_FILE)
%endif  # if ram
    %echo entry __warm_start >> $(SUB_FILE)
    %echo form a >> $(SUB_FILE)
    %echo udfcheck >> $(SUB_FILE)
    %echo debug >> $(SUB_FILE)
    %echo exit >> $(SUB_FILE)
	$(LNK) -SUBCOMMAND=$(SUB_FILE)
    $(CNVS) $* $*

