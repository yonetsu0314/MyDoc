#################################################################
# Renesas C compiler version 7
# NORTi4 OS sample program makefile for Hitachi ULSI MS7751SE01
#  for Big Endian Format.
# make -f smp7751bhr.mak
#
# smp7751bhr.abs  for H-UDI
# smp7751bhr.mot  for ROM
#
#################################################################

### Target Option ###

CPU     = SH4               # CPU type Defination
ENDIAN  = BIG_ENDIAN        # Byte Order used by Memory System
BOARD   = MS7751            # Board Specific Definition
INTSEC  = 10                # OS Interval Timer value in Milliseconds.

### Path ###

INC2 = ..\..\..\..\INC
INC1 = ..
SRC2 = ..\..\..\..\SRC
SRC1 = ..
LIB = ..\..\..\..\LIB\SH\SHC7

### Command ###

AS       = asmsh
AFLAGS   = -cp=sh4 -deb -en=b
CC       = shc
CFLAGS   = -cp=sh4 -deb -en=b -nologo -i=$(INC1), $(INC2)
LD       = optlnk
LDFLAGS  = -su=smp7751bhr.sub
MAKEFILE = smp7751bhr.mak
TARGET = smp7751bhr.abs

### Common Definitions for all files ###

COMMONDEF = MSEC=$(INTSEC), $(ENDIAN)

### Executable Program Files ###


OBJS = vec7751h.obj initsh.obj smp7751.obj n4i7751.obj nos7751.obj nosio.obj \
        pci7751.obj mc1543int.obj mc1543.obj nos1543ch2.obj nos1543ch3.obj     \
        mrshpc01v2.obj

ALL: $(TARGET)

CLEAN:
    del *.obj
    del smp7751bhr.abs
    del smp7751bhr.ctx
    del smp7751bhr.mot
    del smp7751bhr.map

### Boot and Main Program ###

smp7751.obj: $(SRC1)\smp7751.c smp7751bhr.mak
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),$(BOARD) $(SRC1)\smp7751.c

vec7751h.obj: $(SRC1)\vec7751h.asm
    $(AS) $(SRC1)\vec7751h.asm -o=vec7751h.obj $(AFLAGS) -def=_INIT_STACK="H'8c400000"

initsh.obj: $(SRC1)\initsh.c
    $(CC) $(CFLAGS) $(SRC1)\initsh.c

### Timer and Serial Driver ###

n4i7751.obj: $(SRC1)\n4i7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),CLK=27000000,CH=0,INTVEC=0x8c000000, REBOOT $(SRC1)\n4i7751.c

nos7751.obj: $(SRC1)\nos7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),CLK=27000000,CH=0,CH1,V4 $(SRC1)\nos7751.c

nosio.obj:   $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),CHS=3,V4 $(SRC2)\nosio.c

### PCI and Ethernet driver ###

pci7751.obj:    $(SRC1)\pci7751.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), V4 $(SRC1)\pci7751.c

### Super IO driver #######################################

mc1543.obj: $(SRC1)\mc1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU) $(SRC1)\mc1543.c

mc1543int.obj:    $(SRC1)\mc1543int.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), CH2, CH3 $(SRC1)\mc1543int.c

### Serial Controller UART1 & UART3 driver ################

nos1543ch2.obj:  $(SRC1)\nos1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), V4, CH=1, CH2 -ob=nos1543ch2.obj $(SRC1)\nos1543.c

nos1543ch3.obj:  $(SRC1)\nos1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), V4, CH=2, CH3 -ob=nos1543ch3.obj $(SRC1)\nos1543.c

### PCMCIA Controller Driver ################

mrshpc01v2.obj: $(SRC1)\mrshpc01v2.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), V4 $(SRC1)\mrshpc01v2.c


$(TARGET) : $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

#################################################################
# end
