# NORTi Version4 (Renesas C compiler)
#  makefile for Hitachi ULSI MS7727SE01
#               Little Endian format
#
# smp7727lhr.abs  ... Absolute format for debugger
# smp7727lhr.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
SRC1 = ..
SRC2 = ..\..\..\..\SRC


### Flags ###

AFLAGS   = -cp=sh3 -deb -en=l
CFLAGS   = -nologo -cp=sh3 -deb -en=l -i=$(INC1)
LDFLAGS  = -su=smp7727lhr.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7727lhr.mak

### Files ###

OBJS   = vec7727h.obj initsh.obj smp7727.obj n4i7727.obj nosio.obj nos7727.obj nos16550.obj

TARGET = smp7727lhr.abs

### Make Option ###

ALL: $(TARGET)

CLEAN:
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7727h.obj:  $(SRC1)\vec7727h.asm
    $(AS) $(SRC1)\vec7727h.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7727h.obj

initsh.obj:     $(SRC1)\initsh.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7727.obj:   $(SRC1)\smp7727.c
    $(CC) $(CFLAGS) -op=0 $(SRC1)\smp7727.c

n4i7727.obj:   $(SRC1)\n4i7727.c
    $(CC) $(CFLAGS) -op=1 -def=CH=0,INTVEC=0x8c000000,REBOOT $(SRC1)\n4i7727.c

nosio.obj:      $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -op=1 -def=SH3,V4,CHS=2 $(SRC2)\nosio.c

nos7727.obj:   $(SRC1)\nos7727.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CH2,CH=0 $(SRC1)\nos7727.c

nos16550.obj:   $(SRC1)\nos16550.c
    $(CC) $(CFLAGS) -op=1 -def=SH3,V4,CH0,CH=1,CLKTYPE=1,MS7727 $(SRC1)\nos16550.c

$(TARGET): $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
