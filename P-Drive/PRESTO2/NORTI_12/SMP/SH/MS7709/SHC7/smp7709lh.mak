# NORTi Version4 (Hitachi C compiler)
#  makefile for Hitachi ULSI MS7709SE01
#               Little Endian format
#
# smp7709lh.abs  ... Absolute format for debugger
# smp7709lh.mot  ... S-format for ROM
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS   = -nologo -cp=sh3 -deb -en=l
CFLAGS   = -nologo -cp=sh3 -deb -en=l -i=$(INC1),$(INC2)
LDFLAGS  = -su=smp7709lh.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = smp7709lh.mak

### Files ###

OBJS  = vec7709h.obj initsh.obj smp7709.obj n4i7709.obj nosio.obj nos7709.obj nos16550.obj

TARGET = smp7709lh.abs

### Make Option ###

ALL: $(TARGET)

CLEAN:
	del *.obj
	del *.abs
	del *.mot
	del *.map

### Executable Program Files ###

vec7709h.obj:   $(SRC1)\vec7709h.asm
	$(AS) $(SRC1)\vec7709h.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7709h.obj

initsh.obj:     $(SRC1)\initsh.c
	$(CC) $(CFLAGS) -op=1 $(SRC1)\initsh.c

smp7709.obj:    $(SRC1)\smp7709.c
	$(CC) $(CFLAGS) -op=0 $(SRC1)\smp7709.c

n4i7709.obj:    $(SRC1)\n4i7709.c
	$(CC) $(CFLAGS) -op=1 -def=CH=0,INTVEC=0x80000000 $(SRC1)\n4i7709.c

nosio.obj:      $(SRC2)\nosio.c
	$(CC) $(CFLAGS) -op=1 -def=SH3,V4,CHS=2 $(SRC2)\nosio.c

nos7709.obj:    $(SRC1)\nos7709.c
	$(CC) $(CFLAGS) -op=1 -def=V4,CH2,CH=0 $(SRC1)\nos7709.c

nos16550.obj:   $(SRC1)\nos16550.c
	$(CC) $(CFLAGS) -op=1 -def=SH3,V4,CH0,CH=1,MS7709 $(SRC1)\nos16550.c

$(TARGET): $(OBJS) $(MAKEFILE)
	$(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
	$(LD) $(LDFLAGS) -FORM=STYPE

# end
