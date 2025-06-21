# NORTi Version4 (Renesas C compiler)
#  makefile for Computex SH3 EVA BOD (SH7729)
#               Big Endian format
#
# smp7729bh.abs  ... Absolute format for debugger
# smp7729bh.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS   = -cp=sh3 -deb
CFLAGS   = -cp=sh3 -deb -i=$(INC1),$(INC2)
LDFLAGS  = /su=smp7729bh.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = lnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7729bh.mak

### Files ###

OBJS   = vec7729h.obj initsh.obj smp7729.obj n4i7729.obj nosio.obj nos7729.obj

TARGET = smp7729bh.abs

### Make Option ###

ALL: $(TARGET)

CLEAN:
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7729h.obj:  $(SRC1)\vec7729h.asm
    $(AS) $(SRC1)\vec7729h.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7729h.obj

initsh.obj:     $(SRC1)\initsh.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7729.obj:   $(SRC1)\smp7729.c
    $(CC) $(CFLAGS) -op=0 $(SRC1)\smp7729.c

n4i7729.obj:   $(SRC1)\n4i7729.c
    $(CC) $(CFLAGS) -op=1 -def=CH=0,INTVEC=0x80000000 $(SRC1)\n4i7729.c

nosio.obj:      $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -op=1 -def=SH3,V4,CHS=1 $(SRC2)\nosio.c

nos7729.obj:   $(SRC1)\nos7729.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CH0,CH=0 $(SRC1)\nos7729.c

$(TARGET): $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -ENTRY=_RESET
    $(CNV) $(TARGET) smp7729bh.mot

# end
