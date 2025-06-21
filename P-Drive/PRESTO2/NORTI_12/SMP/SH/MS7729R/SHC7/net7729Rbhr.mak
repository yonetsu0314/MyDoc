# Renesas C
# NORTi Version4
# Network for sample program makefile Hitachi ULSI MS7729RSE.
# Big Endian format
#
# net7729Rbhr.abs  ... Absolute format for debugger
# net7729Rbhr.mot  ... S-format for ROM
#

### Path ###

INC1 = ..\..\..\..\INC
INC2 = ..\..\..\..\NETSMP\INC
SRC1 = ..
SRC2 = ..\..\..\..\SRC
SRC3 = ..\..\..\..\NETSMP\SRC
LIB  = ..\..\..\..\LIB\SH\SHC7

### Flags ###

AFLAGS   = -cp=sh3 -deb
CFLAGS   = -nologo -cp=sh3 -deb -op=1 -i=$(INC1),$(INC2)
LDFLAGS  = -su=net7729Rbhr.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = net7729Rbhr.mak

### Files ###

OBJS  = vec7729Rh.obj net7729R.obj initsh.obj n4i7729R.obj nosio.obj \
        nos7729R.obj nonethw.obj dp8390.obj nonelan.obj nondump.obj \
        nontftp.obj nonecho.obj nonechc.obj nonteld.obj nonshel.obj \
        nonftpd.obj nonfile.obj noncons.obj nonramd.obj nonping.obj \
        nontelc.obj nonftpc.obj nondhcp.obj nonedns.obj nos16550.obj \
        nonecfg.obj

LIBS  = $(LIB)\n4dsh3b.lib $(LIB)\n4esh3b.lib

TARGET = net7729Rbhr.abs

### Target Option ###

CPU = SH3
ENDIAN = BIG_ENDIAN
BOARD  = MS7729R

### Make Option ###

ALL : $(TARGET)

CLEAN :
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC1)\nonecfg.c
    $(CC) $(CFLAGS) -def=$(CPU) $(SRC1)\nonecfg.c

### Boot and Main Program ###

vec7729Rh.obj : $(SRC1)\vec7729Rh.asm
    $(AS) $(SRC1)\vec7729Rh.asm $(AFLAGS) -def=_INIT_STACK="H'8C080000" -o=vec7729Rh.obj

net7729R.obj : $(SRC1)\net7729R.c
#   $(CC) $(CFLAGS) -def=$(CPU),$(BOARD) $(SRC1)\net7729R.c
    $(CC) $(CFLAGS) -def=$(CPU),$(BOARD),DHCP $(SRC1)\net7729R.c
#   $(CC) $(CFLAGS) -def=$(CPU),$(BOARD),DHCP,DUMP $(SRC1)\net7729R.c

initsh.obj : $(SRC1)\initsh.c
    $(CC) $(CFLAGS) $(SRC1)\initsh.c

### Timer and Serial Driver ###

n4i7729R.obj : $(SRC1)\n4i7729R.c
    $(CC) $(CFLAGS) -def=CH=0,INTVEC=0x8c000000,REBOOT $(SRC1)\n4i7729R.c

nosio.obj : $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -def=$(CPU),V4,CHS=2 $(SRC2)\nosio.c

nos7729R.obj : $(SRC1)\nos7729R.c
    $(CC) $(CFLAGS) -def=V4,CH2,CH=0 $(SRC1)\nos7729R.c

nos16550.obj : $(SRC1)\nos16550.c
    $(CC) $(CFLAGS) -def=$(CPU),V4,CH0,CH=1,CLKTYPE=1,$(BOARD) $(SRC1)\nos16550.c

### Ethernet Driver ###

nonethw.obj : $(SRC1)\nonethw.c
    $(CC) $(CFLAGS) -def=$(CPU),$(ENDIAN),$(BOARD) $(SRC1)\nonethw.c

dp8390.obj : $(SRC1)\dp8390.c
    $(CC) $(CFLAGS) -def=$(CPU),$(ENDIAN),$(BOARD) $(SRC1)\dp8390.c

nondump.obj : $(SRC3)\nondump.c
    $(CC) $(CFLAGS) -def=$(CPU) $(SRC3)\nondump.c

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
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
