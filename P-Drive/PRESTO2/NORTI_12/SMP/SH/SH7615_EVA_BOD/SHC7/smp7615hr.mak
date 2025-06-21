# Renesas C
# NORTi Version4
# Sample program makefile for Computex SH7615 EVA BOD+
#
# smp7615hr.abs ... Absolute format for debugger
# smp7615hr.mot ... S-format
#

INC1 = ..\..\..\..\INC
INC2 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC

### Flags ###

AFLAGS  = -cp=sh2 -deb
CFLAGS  = -cp=sh2 -op=1 -deb -nologo -i=$(INC1),$(INC2)
LDFLAGS = -su=smp7615hr.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
MAKE     = make
MAKEFILE = smp7615hr.mak

### Files ###

OBJS   = vec7615h.obj init7615.obj smp7615.obj n4i7615.obj nosio.obj nos7615.obj
TARGET = smp7615hr.abs

### Target Option ###

ENDIAN  = BIG_ENDIAN
BOARD   = SH7615EV

### Make Option ###

all : $(TARGET)

clean :
    del *.obj
    del *.abs
    del *.mot

vec7615h.obj : $(SRC1)\vec7615h.asm $(MAKEFILE)
    $(AS) $(SRC1)\vec7615h.asm $(AFLAGS) -o=vec7615h.obj

init7615.obj : $(SRC1)\init7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) $(SRC1)\init7615.c

smp7615.obj : $(SRC1)\smp7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) $(SRC1)\smp7615.c

n4i7615.obj : $(SRC1)\n4i7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CH=3,CLK=15000000,INTVEC=0x6000000,REBOOT $(SRC1)\n4i7615.c

nosio.obj : $(SRC2)\nosio.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=V4,CHS=1 $(SRC2)\nosio.c

nos7615.obj : $(SRC1)\nos7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CLK=15000000,V4,CH1,CH=0 $(SRC1)\nos7615.c

### Executable Program Files ###

$(TARGET) : $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
