# Renesas C
# NORTi Version4
# Sample program makefile for MiNET-7042.
#
# RAM address 400000H
#
# smp7042h.abs  ... Absolute format for debugger
# smp7042h.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7042h.mak

### Flags ###

AFLAGS  = -cp=sh2
CFLAGS  = -cp=sh2 -i=$(INC1),$(INC2) -nologo
LDFLAGS = -su=smp7042h.sub

### Files ###

OBJS   = vec7042h.obj init7040.obj smp7042.obj n4i7040.obj nosio.obj nos7040.obj nos70401.obj
TARGET = smp7042h.abs

### Make Option ###

ALL : $(TARGET)

CLEAN:
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7042h.obj:   $(SRC1)\vec7042h.asm
    $(AS) $(SRC1)\vec7042h.asm -def=_INIT_STACK="H'0047FE00" -o=vec7042h.obj

init7040.obj:   $(SRC1)\init7040.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\init7040.c

smp7042.obj:    $(SRC1)\smp7042.c
    $(CC) $(CFLAGS) -deb $(SRC1)\smp7042.c

n4i7040.obj:    $(SRC1)\n4i7040.c
    $(CC) $(CFLAGS) -deb -op=1 -def=CH=0,CLK=28636360 $(SRC1)\n4i7040.c

nosio.obj:      $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -deb -def=V4,CHS=2 $(SRC2)\nosio.c

nos7040.obj:    $(SRC1)\nos7040.c
    $(CC) $(CFLAGS) -deb -def=V4,CH=0,CLK=28636360 $(SRC1)\nos7040.c

nos70401.obj:   $(SRC1)\nos7040.c
    $(CC) $(CFLAGS) -deb -def=V4,CH=1,CLK=28636360 -ob=nos70401.obj $(SRC1)\nos7040.c

$(TARGET): $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
