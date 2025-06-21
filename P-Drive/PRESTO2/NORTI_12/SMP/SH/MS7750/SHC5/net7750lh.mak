# Renesas C
# NORTi Version4
# Network sample program makefile for Hitachi ULSI MS7750SE.
# Little Endian format
#
# net7750lh.abs  ... Absolute format for debugger
# net7750lh.mot  ... S-format for ROM
#

### Path ###

INC1 = ..\..\..\..\INC
INC2 = ..\..\..\..\NETSMP\INC
INC3 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC
SRC3 = ..\..\..\..\NETSMP\SRC
LIB  = ..\..\..\..\LIB\SH\SHC5

### Flags ###

AFLAGS   = -cp=sh4 -deb -en=l
CFLAGS   = -cp=sh4 -deb -en=l -op=1 -i=$(INC1),$(INC2),$(INC3) -fp=s
LDFLAGS  = /su=net7750lh.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = lnk
CNV      = cnvs
MAKE     = make
MAKEFILE = net7750lh.mak

### Files ###

OBJS = net7750.obj vec7750h.obj initsh.obj n4i7750.obj nosio.obj   \
       nos7750.obj nonethw.obj dp8390.obj nonelan.obj nondump.obj  \
       nontftp.obj nonecho.obj nonechc.obj nonteld.obj nonshel.obj \
       nonftpd.obj nonfile.obj noncons.obj nonramd.obj nonping.obj \
       nontelc.obj nonftpc.obj nondhcp.obj nonedns.obj nos16550.obj\
       nonecfg.obj

LIBS  = $(LIB)\n4dsh4l.lib $(LIB)\n4esh4dl.lib

TARGET = net7750lh.abs

### Target Option ###

CPU     = SH4
ENDIAN  = LITTLE_ENDIAN
BOARD   = MS7750

### Make Option ###

ALL : $(TARGET)

CLEAN :
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Boot and Main Program ###

vec7750h.obj : $(SRC1)\vec7750h.asm
    $(AS) $(SRC1)\vec7750h.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7750h.obj

net7750.obj : $(SRC1)\net7750.c $(MAKEFILE)
#   $(CC) $(CFLAGS) -def=$(CPU),$(ENDIAN),$(BOARD) $(SRC1)\net7750.c
    $(CC) $(CFLAGS) -def=$(CPU),$(BOARD),DHCP,$(ENDIAN) $(SRC1)\net7750.c
#   $(CC) $(CFLAGS) -def=$(CPU),$(BOARD),DHCP,$(ENDIAN),DUMP $(SRC1)\net7750.c

initsh.obj : $(SRC1)\initsh.c
    $(CC) $(CFLAGS) $(SRC1)\initsh.c

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC1)\nonecfg.c
    $(CC) $(CFLAGS) -def=$(CPU) $(SRC1)\nonecfg.c

### Timer and Serial Driver ###

n4i7750.obj : $(SRC1)\n4i7750.c
    $(CC) $(CFLAGS) -def=CH=0,INTVEC=0x80000000 $(SRC1)\n4i7750.c

nos7750.obj : $(SRC1)\nos7750.c
    $(CC) $(CFLAGS) -def=V4,CH1,CH=0 $(SRC1)\nos7750.c

nos16550.obj : $(SRC1)\nos16550.c
    $(CC) $(CFLAGS) -def=$(CPU),V4,CH0,CH=1,CLKTYPE=1,$(BOARD) $(SRC1)\nos16550.c

nosio.obj : $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -def=$(CPU),V4,CHS=2 $(SRC2)\nosio.c

### Ethernet Driver ###

nonethw.obj : $(SRC1)\nonethw.c $(SRC1)\nonethw.h
    $(CC) $(CFLAGS) -def=$(CPU),$(ENDIAN) $(SRC1)\nonethw.c

dp8390.obj : $(SRC1)\dp8390.c $(SRC1)\dp8390.h $(SRC1)\nonethw.h
    $(CC) $(CFLAGS) -def=$(CPU),$(ENDIAN),$(BOARD) $(SRC1)\dp8390.c

### LAN Function ###

nondump.obj : $(SRC3)\nondump.c
    $(CC) $(CFLAGS) -def=$(CPU),CH=1 $(SRC3)\nondump.c

nonelan.obj : $(SRC2)\nonelan.c
    $(CC) $(CFLAGS) -def=$(CPU),DUMP $(SRC2)\nonelan.c

### Sample Application ###

nontftp.obj : $(SRC3)\nontftp.c
    $(CC) $(CFLAGS) $(SRC3)\nontftp.c

nonecho.obj : $(SRC3)\nonecho.c
    $(CC) $(CFLAGS) $(SRC3)\nonecho.c

nonechc.obj : $(SRC3)\nonechc.c
    $(CC) $(CFLAGS) $(SRC3)\nonechc.c

nonteld.obj : $(SRC3)\nonteld.c
    $(CC) $(CFLAGS) $(SRC3)\nonteld.c

nonshel.obj : $(SRC3)\nonshel.c
    $(CC) $(CFLAGS) $(SRC3)\nonshel.c

nonftpd.obj : $(SRC3)\nonftpd.c
    $(CC) $(CFLAGS) $(SRC3)\nonftpd.c

nonfile.obj : $(SRC3)\nonfile.c
    $(CC) $(CFLAGS) $(SRC3)\nonfile.c

noncons.obj : $(SRC3)\noncons.c
    $(CC) $(CFLAGS) $(SRC3)\noncons.c

nonramd.obj : $(SRC3)\nonramd.c
    $(CC) $(CFLAGS) $(SRC3)\nonramd.c

nonping.obj : $(SRC3)\nonping.c
    $(CC) $(CFLAGS) $(SRC3)\nonping.c

nontelc.obj : $(SRC3)\nontelc.c
    $(CC) $(CFLAGS) $(SRC3)\nontelc.c

nonftpc.obj : $(SRC3)\nonftpc.c
    $(CC) $(CFLAGS) $(SRC3)\nonftpc.c

nondhcp.obj : $(SRC3)\nondhcp.c
    $(CC) $(CFLAGS) -def=$(ENDIAN) $(SRC3)\nondhcp.c

nonedns.obj : $(SRC3)\nonedns.c
    $(CC) $(CFLAGS) -def=$(ENDIAN) $(SRC3)\nonedns.c

### Executable Program Files ###

$(TARGET) : $(OBJS) $(LIBS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -ENTRY=_RESET
    $(CNV) $(TARGET) net7750lh.mot

# end
