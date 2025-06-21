# NORTi Version4 (Renesas C compiler)
#  makefile for Hitachi ULSI MS7729RSE01
#               Little Endian format
#
# smp7729Rlh.abs  ... Absolute format for debugger
# smp7729Rlh.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS   = -cp=sh3 -deb -en=l
CFLAGS   = -nologo -cp=sh3 -deb -en=l -i=$(INC1)
LDFLAGS  = -su=smp7729Rlh.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7729Rlh.mak

### Files ###

OBJS   = vec7729Rh.obj initsh.obj smp7729R.obj n4i7729R.obj nosio.obj nos7729R.obj nos16550.obj

TARGET = smp7729Rlh.abs

### Make Option ###

ALL: $(TARGET)

CLEAN:
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Executable Program Files ###

vec7729Rh.obj:  $(SRC1)\vec7729Rh.asm
    $(AS) $(SRC1)\vec7729Rh.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7729Rh.obj

initsh.obj:     $(SRC1)\initsh.c
    $(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7729R.obj:   $(SRC1)\smp7729R.c
    $(CC) $(CFLAGS) -op=0 $(SRC1)\smp7729R.c

n4i7729R.obj:   $(SRC1)\n4i7729R.c
    $(CC) $(CFLAGS) -op=1 -def=CH=0,INTVEC=0x80000000 $(SRC1)\n4i7729R.c

nosio.obj:      $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -op=1 -def=SH3,V4,CHS=2 $(SRC2)\nosio.c

nos7729R.obj:   $(SRC1)\nos7729R.c
    $(CC) $(CFLAGS) -op=1 -def=V4,CH2,CH=0 $(SRC1)\nos7729R.c

nos16550.obj:   $(SRC1)\nos16550.c
    $(CC) $(CFLAGS) -op=1 -def=SH3,V4,CH0,CH=1,CLKTYPE=1,MS7729R $(SRC1)\nos16550.c

$(TARGET): $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
