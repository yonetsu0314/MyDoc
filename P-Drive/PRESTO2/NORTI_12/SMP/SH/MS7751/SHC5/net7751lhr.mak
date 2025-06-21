#################################################################
# Renesas C compiler version5
# NORTi4 Network sample program makefile for Hitachi ULSI MS7751SE01
# Little Endian format.
# make -f net7751lhr.mak
#
# net7751lhr.abs  for H-UDI
#
#################################################################

### Target Option ###

CPU     = SH4               # CPU type Defination
ENDIAN  = LITTLE_ENDIAN     # Byte Order used by Memory System
BOARD   = MS7751            # Board Specific Definition
INTSEC  = 10                # OS Interval Timer value in Milliseconds.

### Path ###

INC1 = ..\..\..\..\INC
INC2 = ..\..\..\..\NETSMP\INC
INC3 = ..
SRC1 = ..\..\..\..\SRC
SRC2 = ..\..\..\..\NETSMP\SRC
SRC3 = ..
LIB = ..\..\..\..\LIB\SH\SHC5

### Command ###

AS       = asmsh
AFLAGS   = -cp=sh4 -deb -en=l
CC       = shc
CFLAGS   = -cp=sh4 -deb -op=0 -en=l -i=$(INC3), $(INC1), $(INC2)
LD       = lnk
LDFLAGS  = /su=net7751lhr.sub
MAKEFILE = net7751lhr.mak
TARGET = net7751lhr.abs

### Common Definitions for all files ###

COMMONDEF = MSEC=$(INTSEC), $(ENDIAN)

### Executable Program Files ###

OBJS = vec7751h.obj initsh.obj net7751.obj n4i7751.obj nosio.obj nos7751.obj     \
       pci7751.obj AM79C973.obj mc1543.obj nonelan.obj nondump.obj serialch2.obj \
       serialch3.obj mrshpc01v2.obj nontftp.obj nonecho.obj nonechc.obj          \
       nonteld.obj nonshel.obj nonftpd.obj nonfile.obj noncons.obj nonramd.obj   \
       nonping.obj nontelc.obj nonftpc.obj nondhcp.obj nonedns.obj nonecfg.obj   \
       mc1543int.obj

ALL: $(TARGET)

CLEAN:
    del *.obj
    del net7751lhr.abs
    del net7751lhr.mot
    del net7751lhr.map

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC3)\nonecfg.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU) $(SRC3)\nonecfg.c

### Boot and Main Program ###

net7751.obj: $(SRC3)\net7751.c $(MAKEFILE)
#   $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0 $(SRC3)\net7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0, DHCP $(SRC3)\net7751.c
#   $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU), $(BOARD), CH=0, DHCP, DUMP $(SRC3)\net7751.c

vec7751h.obj: $(SRC3)\vec7751h.asm
    $(AS) $(SRC3)\vec7751h.asm -o=vec7751h.obj $(AFLAGS) -def=_INIT_STACK="H'8c400000"

initsh.obj: $(SRC3)\initsh.c
    $(CC) $(CFLAGS) $(SRC3)\initsh.c

### Timer and Serial Driver ###

n4i7751.obj: $(SRC3)\n4i7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),CLK=27000000,CH=0,INTVEC=0x8c000000,REBOOT $(SRC3)\n4i7751.c

nos7751.obj: $(SRC3)\nos7751.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), $(CPU),CLK=27000000,CH=0,CH1,V4 $(SRC3)\nos7751.c

nosio.obj:   $(SRC1)\nosio.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU),CHS=3,V4 $(SRC1)\nosio.c

### PCI and Ethernet driver ###

pci7751.obj:    $(SRC3)\pci7751.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), V4, $(BOARD) $(SRC3)\pci7751.c

AM79C973.obj:    $(SRC3)\AM79C973.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), V4, AUTO_NEG, $(BOARD) $(SRC3)\AM79C973.c

### Super IO driver ###

mc1543.obj: $(SRC3)\mc1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU) $(SRC3)\mc1543.c

mc1543int.obj:    $(SRC3)\mc1543int.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF), CH2, CH3, LANC $(SRC3)\mc1543int.c

### Serial Controller UART1 & UART3 driver ################

serialch2.obj:  $(SRC3)\nos1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), V4, CH=1, CH2 -ob=serialch2.obj $(SRC3)\nos1543.c

serialch3.obj:  $(SRC3)\nos1543.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), V4, CH=2, CH3 -ob=serialch3.obj $(SRC3)\nos1543.c

### PCMCIA Controller Driver ################

mrshpc01v2.obj: $(SRC3)\mrshpc01v2.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), V4 $(SRC3)\mrshpc01v2.c

### LAN functions ###

nondump.obj:    $(SRC2)\nondump.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), CH=1 $(SRC2)\nondump.c

nonelan.obj:    $(SRC1)\nonelan.c
    $(CC) $(CFLAGS)  -def=$(COMMONDEF), $(CPU), DUMP $(SRC1)\nonelan.c

### Sample Applications ###

nontftp.obj : $(SRC2)\nontftp.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nontftp.c

nonecho.obj : $(SRC2)\nonecho.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonecho.c

nonechc.obj : $(SRC2)\nonechc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonechc.c

nonteld.obj : $(SRC2)\nonteld.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonteld.c

nonshel.obj : $(SRC2)\nonshel.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonshel.c

nonftpd.obj : $(SRC2)\nonftpd.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonftpd.c

nonfile.obj : $(SRC2)\nonfile.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonfile.c

noncons.obj : $(SRC2)\noncons.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\noncons.c

nonramd.obj : $(SRC2)\nonramd.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonramd.c

nonping.obj : $(SRC2)\nonping.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonping.c

nontelc.obj : $(SRC2)\nontelc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nontelc.c

nonftpc.obj : $(SRC2)\nonftpc.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nonftpc.c

nondhcp.obj : $(SRC2)\nondhcp.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF) $(SRC2)\nondhcp.c

nonedns.obj : $(SRC2)\nonedns.c
    $(CC) $(CFLAGS) -def=$(COMMONDEF)  $(SRC2)\nonedns.c

#################################################################

$(TARGET) : $(OBJS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -ENTRY=_RESET

#################################################################
# end
