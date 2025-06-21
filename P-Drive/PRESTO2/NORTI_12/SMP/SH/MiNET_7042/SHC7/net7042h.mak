# Renesas C
# NORTi Version4
# Network sample program for makefile MiNET-7042.
#
# net7042h.abs  for ROMICE
# net7042h.mot  for ROM
#

### Path ###

INC1 = ..\..\..\..\INC
INC2 = ..\..\..\..\NETSMP\INC
INC3 = ..
SRC1 = ..
SRC2 = ..\..\..\..\SRC
SRC3 = ..\..\..\..\NETSMP\SRC
LIB  = ..\..\..\..\LIB\SH\SHC7

### Flags ###

AFLAGS  = -cp=sh2 -deb
CFLAGS  = -cp=sh2 -op=1 -deb -i=$(INC1),$(INC2),$(INC3) -nologo
LDFLAGS = -su=net7042h.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
CNV      = cnvs
MAKE     = make
MAKEFILE = net7042h.mak

### Files ###

OBJS  = vec7042h.obj init7040.obj net7042.obj n4i7040.obj nosio.obj  \
        nos7040.obj nos70401.obj mi42rtc.obj mi42fls.obj mi42eep.obj \
        mi42tes.obj nonethw.obj mb86964.obj nonelan.obj nondump.obj  \
        nontftp.obj nonecho.obj nonechc.obj nonteld.obj nonshel.obj  \
        nonftpd.obj nonfile.obj noncons.obj nonramd.obj nonping.obj  \
        nontelc.obj nonftpc.obj nondhcp.obj nonedns.obj nonecfg.obj

LIBS  = $(LIB)\n4dsh2.lib $(LIB)\n4esh2.lib

TARGET = net7042h.abs

### Make Option ###

ALL : $(TARGET)

CLEAN :
    del *.obj
    del *.abs
    del *.mot
    del *.map

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC1)\nonecfg.c
    $(CC) $(CFLAGS) $(SRC1)\nonecfg.c

### Boot and Main Program ###

vec7042h.obj : $(SRC1)\vec7042h.asm
    $(AS) $(SRC1)\vec7042h.asm $(AFLAGS) -def=_INIT_STACK="H'0047FE00" -o=vec7042h.obj

init7040.obj : $(SRC1)\init7040.c
    $(CC) $(CFLAGS) $(SRC1)\init7040.c

net7042.obj : $(SRC1)\net7042.c
#   $(CC) $(CFLAGS) -def=MiNET7042  $(SRC1)\net7042.c
    $(CC) $(CFLAGS) -def=MiNET7042, DHCP $(SRC1)\net7042.c
#   $(CC) $(CFLAGS) -def=MiNET7042, DHCP, DUMP $(SRC1)\net7042.c

### Timer and Serial Driver ###

n4i7040.obj : $(SRC1)\n4i7040.c
    $(CC) $(CFLAGS) -def=CLK=28636360,CH=0 $(SRC1)\n4i7040.c

nosio.obj : $(SRC2)\nosio.c
    $(CC) $(CFLAGS) -def=V4,CHS=2 $(SRC2)\nosio.c

nos7040.obj : $(SRC1)\nos7040.c
    $(CC) $(CFLAGS) -def=V4,CH=0,CLK=28636360 $(SRC1)\nos7040.c
    $(CC) $(CFLAGS) -def=V4,CH=1,CLK=28636360 -ob=nos70401.obj $(SRC1)\nos7040.c

### Device drivers for MiNET-7042 ###

mi42rtc.obj : $(SRC1)\mi42rtc.c
    $(CC) $(CFLAGS) $(SRC1)\mi42rtc.c

mi42fls.obj : $(SRC1)\mi42fls.c
    $(CC) $(CFLAGS) $(SRC1)\mi42fls.c

mi42eep.obj : $(SRC1)\mi42eep.c
    $(CC) $(CFLAGS) $(SRC1)\mi42eep.c

mi42tes.obj : $(SRC1)\mi42tes.c
    $(CC) $(CFLAGS) $(SRC1)\mi42tes.c

### Ethernet Driver ###

nonethw.obj : $(SRC1)\nonethw.c
    $(CC) $(CFLAGS) $(SRC1)\nonethw.c

mb86964.obj : $(SRC1)\mb86964.c
    $(CC) $(CFLAGS) $(SRC1)\mb86964.c

### LAN function ###

nondump.obj : $(SRC3)\nondump.c
    $(CC) $(CFLAGS) $(SRC3)\nondump.c

nonelan.obj : $(SRC2)\nonelan.c
    $(CC) $(CFLAGS) -def=DUMP $(SRC2)\nonelan.c

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
    $(CC) $(CFLAGS) $(SRC3)\nondhcp.c

nonedns.obj : $(SRC3)\nonedns.c
    $(CC) $(CFLAGS) $(SRC3)\nonedns.c

### Executable Program Files ###

$(TARGET) : $(OBJS) $(LIBS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
