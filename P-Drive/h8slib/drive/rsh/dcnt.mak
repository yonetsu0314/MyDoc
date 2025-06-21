#-----------------------------------------------------------------------#
# File name	: dcnt.mak		/ counter driver liblary					#
#-----------------------------------------------------------------------#
#*$Header: p:/h8slib/drive/rsh/RCS/dcnt.mak 1.2 1970/01/01 00:00:00 yonetsu Exp $
#*$Log: dcnt.mak $
# リビジョン 1.2  1970/01/01  00:00:00  yonetsu
# release に *.mk を追加
# 
# リビジョン 1.1  1970/01/01  00:00:00  yonetsu
# 初期リビジョン
# 
#-----------------------------------------------------------------------#
#				(c) Copyright 2000, ISHIDA  CO., LTD.					#
#				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					#
#				SHIGA JAPAN												#
#				(077) 553-4141											#
#-----------------------------------------------------------------------#
# Function																#
#		カウンター関連ドライバーのライブラリーを作成する。				#
#-----------------------------------------------------------------------#

PROG	=dcnt				# Liblary name

INC1	=p:\h8sinc			# refernce dir

!if $(DEBUG) == 0			# No debug option
CFLAG	= /def=NDEBUG
!endif
!if $(DEBUG) == 1			# debug option
CFLAG	= /deb
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
AFLAG	=/cpu=$(CPU_TYPE)
LIBR	=lbr
LFLAG	=/subcommand=$(PROG).sub
LRF		= ECHO > NUL

#============================================================================
TARGET	=$(PROG)$(LIB_NAME)$(DB).lib
#============================================================================

.c.obj:
	$(CC) $(CFLAG) $*

#============================================================================
OBJS	=	dcntid.obj\
			cntsini.obj\
			cntget.obj\
			cntena.obj\
			cntdis.obj\
			cntint.obj

#============================================================================
$(TARGET): $(PROG).sub $(OBJS)
	if exist $(TARGET) del $(TARGET)
	$(LIBR) $(LFLAG)
	copy $(TARGET) ..\$(TARGET)

$(PROG).sub: $(PROG).mak
	echo create	$(TARGET)						>  $(PROG).SUB
	perl -p -e 's/\s//g;s/.obj/\n/g;'			> $(PROG).TMP <<!
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
	ident	*.h			>>	$(PROG)-$(REV)
	attrib	+R			$(PROG)-$(REV)
!endif
#============================================================================
