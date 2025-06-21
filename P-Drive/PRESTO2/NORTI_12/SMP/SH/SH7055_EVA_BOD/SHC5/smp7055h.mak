# NORTi Version 4 (Renesas C compiler)
#  makefile for Computex SH7055 EVA BOD
#
# smp7055h.abs  ... Absolute format for debugger
# smp7055h.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS   = -cp=sh2e -deb
CFLAGS   = -cp=sh2e -deb -extra=a=2000 -i=$(INC1),$(INC2)
LDFLAGS  = /su=smp7055h.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = lnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7055h.mak

### Files ###

OBJS  = vec7055h.obj initsh.obj smp7055.obj n4i7055.obj nosio.obj nos7055.obj

TARGET = smp7055h.abs

### Make Option ###

ALL : $(TARGET)

CLEAN :
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7055h.obj :  $(SRC1)\vec7055h.asm
    $(AS) $(SRC1)\vec7055h.asm $(AFLAGS) -def=_INIT_STACK="H'004FF000" -o=vec7055h.obj

initsh.obj :    $(SRC1)\initsh.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7055.obj :   $(SRC1)\smp7055.c
    $(CC) $(CFLAGS) -op=0 $(SRC1)\smp7055.c

n4i7055.obj :   $(SRC1)\n4i7055.c
    $(CC) $(CFLAGS) -op=1 -def=CH=6 $(SRC1)\n4i7055.c

nosio.obj :     $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CHS=1 $(SRC2)\nosio.c

nos7055.obj :   $(SRC1)\nos7055.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CH=0,CH1 $(SRC1)\nos7055.c

$(TARGET) : $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -ENTRY=_RESET
    $(CNV) $(TARGET) smp7055h.mot

# end
