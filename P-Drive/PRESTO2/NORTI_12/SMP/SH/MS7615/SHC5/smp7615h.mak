# Renesas C
# NORTi Version4
# Sample program makefile for MS7615SE01
#
# smp7615h.abs  ... Absolute format for debugger
# smp7615h.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS  = -cp=sh2 -deb
CFLAGS  = -cp=sh2 -op=1 -deb -i=$(INC1),$(INC2)
LDFLAGS = -su=smp7615h.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = lnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7615h.mak

### Files ###

OBJS   = vec7615h.obj init7615.obj smp7615.obj n4i7615.obj nosio.obj nos7615.obj nos16550.obj
TARGET = smp7615h.abs

### Target Option ###

ENDIAN  = BIG_ENDIAN
BOARD   = MS7615

### Make Option ###

all : $(TARGET)

vec7615h.obj : $(SRC1)\vec7615h.asm $(MAKEFILE)
    $(AS) $(SRC1)\\vec7615h.asm $(AFLAGS) -o=vec7615h.obj

init7615.obj : $(SRC1)\init7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) $(SRC1)\init7615.c

smp7615.obj : $(SRC1)\smp7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) $(SRC1)\smp7615.c

n4i7615.obj : $(SRC1)\n4i7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CH=3,CLK=15000000,INTVEC=0x6000000 $(SRC1)\n4i7615.c

nosio.obj : $(SRC2)\nosio.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=V4,CHS=2 $(SRC2)\nosio.c

nos7615.obj : $(SRC1)\nos7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=V4,CH0,CH=0,CLK=15000000 $(SRC1)\nos7615.c

nos16550.obj : $(SRC1)\nos16550.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=V4,CH0,CH=1,CLKTYPE=1,$(BOARD) $(SRC1)\nos16550.c

### Executable Program Files ###

$(TARGET) : $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -ENTRY=_RESET
    $(CNV) $(TARGET) smp7615h.mot

# end
