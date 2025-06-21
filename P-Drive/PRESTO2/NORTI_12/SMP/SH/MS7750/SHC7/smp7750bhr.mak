# NORTi Version4 (Renesas C compiler)
#  makefile for Hitachi ULSI MS7750SE01
#               Big Endian format
#
# smp7750bhr.abs  ... Absolute format for debugger
# smp7750bhr.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS   = -cp=sh4 -deb
CFLAGS   = -cp=sh4 -deb -nologo -fp=s -i=$(INC1),$(INC2)
LDFLAGS  = -su=smp7750bhr.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7750bhr.mak

### Files ###

OBJS  = vec7750h.obj initsh.obj smp7750.obj n4i7750.obj nosio.obj nos7750.obj nos16550.obj

TARGET = smp7750bhr.abs

### Make Option ###

ALL: $(TARGET)

CLEAN:
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7750h.obj:   $(SRC1)\vec7750h.asm
    $(AS) $(SRC1)\vec7750h.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7750h.obj

initsh.obj:     $(SRC1)\initsh.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7750.obj:    $(SRC1)\smp7750.c
    $(CC) $(CFLAGS) -op=0 $(SRC1)\smp7750.c

n4i7750.obj:    $(SRC1)\n4i7750.c
    $(CC) $(CFLAGS) -op=1 -def=CH=0,INTVEC=0x8c000000,REBOOT $(SRC1)\n4i7750.c

nosio.obj:      $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -op=1 -def=SH4,V4,CHS=2 $(SRC2)\nosio.c

nos7750.obj:    $(SRC1)\nos7750.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CH1,CH=0 $(SRC1)\nos7750.c

nos16550.obj:   $(SRC1)\nos16550.c
    $(CC) $(CFLAGS) -op=1 -def=SH4,V4,CH0,CH=1,CLKTYPE=1,MS7750 $(SRC1)\nos16550.c

$(TARGET): $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
