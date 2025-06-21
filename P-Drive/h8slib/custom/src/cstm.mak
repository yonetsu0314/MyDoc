#-----------------------------------------------------------------------#
# File name	: custm.mk		/ Custom liblary							#
#-----------------------------------------------------------------------#
#$Header: p:/h8slib/custom/src/RCS/cstm.mak 1.1 1970/01/01 00:00:00 yonetsu Exp $
#$Log: cstm.mak $
# リビジョン 1.1  1970/01/01  00:00:00  yonetsu
# 初期リビジョン
# 
#-----------------------------------------------------------------------#
#				(c) Copyright 2000, ISHIDA  CO., LTD.					#
#				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					#
#				SHIGA JAPAN												#
#				(077) 553-4141											#
#-----------------------------------------------------------------------#

PROG	=cstm				# Liblary name

INC1	=p:\h8sinc			# reference dir


!if $(DEBUG) == 0			# No debug option
CFLAG	= /def=NDEBUG
!endif
!if $(DEBUG) == 1			# debug option
CFLAG	= /deb
AFLAG	= /deb
DB		= _
!endif
!if $(DEBUG) > 2
!error Invalid DEBUG option selected 
!endif

!if	$(CPU) == 0
LIB_NAME	=stk
CPU_TYPE	=300stk
!endif
!if $(CPU) == 1
LIB_NAME	=reg
CPU_TYPE	=300reg
!endif
!if $(CPU) == 2
LIB_NAME	=hn
CPU_TYPE	=300hn
!endif
!if $(CPU) == 3
LIB_NAME	=ha
CPU_TYPE	=300ha
!endif
!if $(CPU) == 4
LIB_NAME	=26n
CPU_TYPE	=2600n
!endif
!if $(CPU) == 5
LIB_NAME	=26a
CPU_TYPE	=2600a
!endif
!if $(CPU) == 6
LIB_NAME	=20n
CPU_TYPE	=2000n
!endif
!if $(CPU) == 7
LIB_NAME	=20a
CPU_TYPE	=2000a
!endif


CC		=ch38
CFLAG	=$(CFLAG) /cpu=$(CPU_TYPE) /i=$(INC1) /ee /sj /sp /nol
ASM		=asm38
AFLAG	=$(AFLAG) /cpu=$(CPU_TYPE)
LIBR	=lbr
LFLAG	=/subcommand=$(PROG).sub

LRF		= ECHO > NUL

#============================================================================
TARGET	=$(PROG)$(LIB_NAME)$(DB).lib

#============================================================================
OBJS	=	custmid.obj\
			abp2bbp.obj\
			bbp2abp.obj\
			chkbcc.obj\
			chrcmp.obj\
			cksmalp.obj\
			cksmcpl.obj\
			ftoa.obj\
			htoi.obj\
			itoh.obj\
			setarg.obj

#============================================================================
$(TARGET):	$(PROG).sub $(OBJS)
	if exist $(TARGET) del $(TARGET)
	$(LIBR) $(LFLAG)
	copy $(TARGET) ..\$(TARGET)

.c.obj:
	$(CC) $(CFLAG) $*

$(PROG).sub:	$(PROG).mak
	echo create	$(TARGET)						>  $(PROG).SUB
	perl -p -e 's/\s//g;s/.obj/\n/g;' 			> $(PROG).TMP <<!
	$(OBJS)
!
	perl -p -e "s/^(.*)$/add $1/;" $(PROG).TMP	>> $(PROG).SUB
	echo list	$(PROG)(S)						>> $(PROG).SUB
	echo exit									>> $(PROG).SUB

#============================================================================
clean:
	if exist *.obj del *.obj
	if exist *.sub del *.sub
	if exist *.lib del *.lib

all: clean $(TARGET)
#============================================================================
#	構成管理ファイルの作成
#----------------------------------------------------------------------------
release:
!if $d(REV)
	ident	$(PROG).mak	>	$(PROG)-$(REV)
	ident	*.mk		>>	$(PROG)-$(REV)
	ident	*.c			>>	$(PROG)-$(REV)
	attrib	+R				$(PROG)-$(REV)
!endif
#==========================================================================
