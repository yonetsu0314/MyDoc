#################################################################
# Renesas C compiler version 7
# NORTi4 Network sample program makefile for Hitachi ULSI MS7751SE01
# Big Endian format.
# make -f net7751bhr.mak
#
# net7751bhr.abs  for H-UDI
# net7751bhr.mot  for ROM
#
#################################################################

### Target Option ###

CPU     = SH4               # CPU type Defination
ENDIAN  = BIG_ENDIAN        # Byte Order used by Memory System
BOARD   = MS7751            # Board Specific Definition
INTSEC  = 10                # OS Interval Timer value in Milliseconds.

### Path ###

INC1 = ..
INC2 = ..\..\..\..\INC
INC3 = ..\..\..\..\NETSMP\INC
SRC1 = ..
SRC2 = ..\..\..\..\SRC
SRC3 = ..\..\..\..\NETSMP\SRC
LIB  = ..\..\..\..\LIB\SH\SHC7

### Flags ###

AFLAGS   = -cp=sh4 -deb -en=b
CFLAGS   = -cp=sh4 -deb -op=1 -en=b -nologo -i=$(INC1), $(INC2), $(INC3)
LDFLAGS  = -su=net7751bhr.sub

### Common Definitions for all files ###

COMMONDEF = MSEC=$(INTSEC), $(ENDIAN)

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk

### Files ###

TARGET = net7751bhr.abs

MAKEFILE = net7751bhr.mak

OBJS = vec7751h.obj initsh.obj net7751.obj n4i7751.obj AM79C973.obj mc1543.obj \
       mc1543int.obj mrshpc01v2.obj nonecfg.obj nos1543ch2.obj nos1543ch3.obj \
       nos7751.obj pci7751.obj \
       nonelan.obj nosio.obj \
       noncons.obj nondhcp.obj nondump.obj nonechc.obj nonecho.obj nonedns.obj \
       nonfile.obj nonftpc.obj nonftpd.obj nonping.obj nonramd.obj nonshel.obj \
       nontelc.obj nonteld.obj nontftp.obj

LIBS  = $(LIB)\n4dsh4b.lib $(LIB)\n4esh4b.lib

### Make Option ###

ALL: $(TARGET)

CLEAN:
    -del *.obj
    -del *.abs
    -del *.mot

### Executable Program Files ###

$(TARGET) : $(OBJS) $(LIBS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC1)\nonecfg.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU) $(SRC1)\nonecfg.c

### Boot and Main Program ###

net7751.obj: $(SRC1)\net7751.c $(MAKEFILE)
#   $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0  $(SRC1)\net7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0, DHCP  $(SRC1)\net7751.c
#   $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0, DHCP, DUMP  $(SRC1)\net7751.c

vec7751h.obj: $(SRC1)\vec7751h.asm
    $(AS) $(SRC1)\vec7751h.asm -o=vec7751h.obj $(AFLAGS) -def=_INIT_STACK="H'8c400000"

initsh.obj: $(SRC1)\initsh.c
    $(CC) $(CFLAGS) $(SRC1)\initsh.c

### Timer and Serial Driver ###

n4i7751.obj: $(SRC1)\n4i7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), CLK=27000000, CH=0, INTVEC=0x8c000000, REBOOT  $(SRC1)\n4i7751.c

nos7751.obj: $(SRC1)\nos7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), CLK=27000000, CH=0, CH1, V4  $(SRC1)\nos7751.c

nosio.obj:   $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), CHS=3, V4  $(SRC2)\nosio.c

### PCI and Ethernet driver ###

pci7751.obj:    $(SRC1)\pci7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), V4, $(BOARD)  $(SRC1)\pci7751.c

AM79C973.obj:    $(SRC1)\AM79C973.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), V4, AUTO_NEG, $(BOARD) $(SRC1)\AM79C973.c

### Super IO driver ###

mc1543.obj: $(SRC1)\mc1543.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU)  $(SRC1)\mc1543.c

mc1543int.obj:    $(SRC1)\mc1543int.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), CH2, CH3, LANC $(SRC1)\mc1543int.c

### Serial Controller UART1 & UART3 driver ################

nos1543ch2.obj:  $(SRC1)\nos1543.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), V4, CH=1, CH2 -ob=nos1543ch2.obj $(SRC1)\nos1543.c

nos1543ch3.obj:  $(SRC1)\nos1543.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), V4, CH=2, CH3 -ob=nos1543ch3.obj $(SRC1)\nos1543.c

### PCMCIA Controller Driver ################

mrshpc01v2.obj: $(SRC1)\mrshpc01v2.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), V4  $(SRC1)\mrshpc01v2.c

### LAN functions ###

nondump.obj:    $(SRC3)\nondump.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), CH=1 $(SRC3)\nondump.c

nonelan.obj:    $(SRC2)\nonelan.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), DUMP $(SRC2)\nonelan.c

### Sample Applications ###

nontftp.obj : $(SRC3)\nontftp.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nontftp.c

nonecho.obj : $(SRC3)\nonecho.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonecho.c

nonechc.obj : $(SRC3)\nonechc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonechc.c

nonteld.obj : $(SRC3)\nonteld.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonteld.c

nonshel.obj : $(SRC3)\nonshel.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonshel.c

nonftpd.obj : $(SRC3)\nonftpd.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonftpd.c

nonfile.obj : $(SRC3)\nonfile.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonfile.c

noncons.obj : $(SRC3)\noncons.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\noncons.c

nonramd.obj : $(SRC3)\nonramd.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonramd.c

nonping.obj : $(SRC3)\nonping.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonping.c

nontelc.obj : $(SRC3)\nontelc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nontelc.c

nonftpc.obj : $(SRC3)\nonftpc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC3)\nonftpc.c

nondhcp.obj : $(SRC3)\nondhcp.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF)  $(SRC3)\nondhcp.c

nonedns.obj : $(SRC3)\nonedns.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF)  $(SRC3)\nonedns.c

#################################################################
# end
