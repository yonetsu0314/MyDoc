# Renesas C
# NORTi Version4
# Network sample program makefile for Computex SH7615 EVA BOD+
#
# net7615hr.abs ... Absolute format for debugger
# net7615hr.mot ... S-format
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
CFLAGS  = -cp=sh2 -op=1 -deb -nologo -i=$(INC1),$(INC2),$(INC3)
LDFLAGS = -su=net7615hr.sub

### Command ###

AS       = asmsh
CC       = shc
LD       = optlnk
MAKE     = make
MAKEFILE = net7615hr.mak

### Files ###

OBJS = net7615.obj nonecfg.obj vec7615h.obj init7615.obj n4i7615.obj \
       nosio.obj nos7615.obj nonethw.obj dp83843.obj sh7615e.obj nonelan.obj \
       nondump.obj nontftp.obj nonecho.obj nonechc.obj nonteld.obj nonshel.obj \
       nonftpd.obj nonfile.obj noncons.obj nonramd.obj nonping.obj nontelc.obj \
       nonftpc.obj nondhcp.obj nonedns.obj

LIBS  = $(LIB)\n4dsh2.lib $(LIB)\n4esh2.lib

TARGET = net7615hr.abs

### Target Option ###

ENDIAN  = BIG_ENDIAN
BOARD   = SH7615EV

### Make Option ###

all : $(TARGET)

clean :
    del *.obj
    del *.abs
    del *.mot

### Kernel & TCP/IP Configuration ###

nonecfg.obj : $(SRC1)\nonecfg.c
    $(CC) $(CFLAGS) $(SRC1)\nonecfg.c

### Boot and Main Program ###

vec7615h.obj : $(SRC1)\vec7615h.asm $(MAKEFILE)
    $(AS) $(SRC1)\vec7615h.asm $(AFLAGS) -o=vec7615h.obj

net7615.obj : $(SRC1)\net7615.c $(MAKEFILE)
#   $(CC) $(CFLAGS) -def=$(ENDIAN),$(BOARD) $(SRC1)\net7615.c
    $(CC) $(CFLAGS) -def=$(BOARD),DHCP,$(ENDIAN) $(SRC1)\net7615.c
#   $(CC) $(CFLAGS) -def=$(BOARD),DHCP,$(ENDIAN),DUMP $(SRC1)\net7615.c

init7615.obj : $(SRC1)\init7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) $(SRC1)\init7615.c

### Timer and Serial Driver ###

n4i7615.obj : $(SRC1)\n4i7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CH=3,CLK=15000000,INTVEC=0x6000000,REBOOT $(SRC1)\n4i7615.c

nos7615.obj : $(SRC1)\nos7615.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CLK=15000000,V4,CH1,CH=0 $(SRC1)\nos7615.c

nosio.obj : $(SRC2)\nosio.c $(MAKEFILE)
    $(CC) $(CFLAGS) -def=CHS=1,V4 $(SRC2)\nosio.c

### Ethernet Driver ###

nonethw.obj : $(SRC1)\nonethw.c $(SRC1)\nonethw.h $(MAKEFILE)
    $(CC) $(CFLAGS) -def=$(ENDIAN) $(SRC1)\nonethw.c

dp83843.obj : $(SRC1)\dp83843.c $(SRC1)\dp83843.h $(MAKEFILE)
    $(CC) $(CFLAGS) -c=a -def=PHYAD=0x01,$(BOARD) $(SRC1)\dp83843.c
    $(AS) dp83843.src $(AFLAGS)
    del dp83843.src
#   $(CC) $(CFLAGS) -c=a -def=PHYAD=0x01,LOOP_BACK,$(BOARD) $(SRC1)\dp83843.c

sh7615e.obj : $(SRC1)\sh7615e.c $(SRC1)\sh7615e.h $(MAKEFILE)
    $(CC) $(CFLAGS) -def=AUTONEGO $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=AUTONEGO,FULL $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=SPEED=10 $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=SPEED=10,FULL $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=SPEED=100 $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=SPEED=100,FULL $(SRC1)\sh7615e.c
#   $(CC) $(CFLAGS) -def=LOOP_BACK $(SRC1)\sh7615e.c

### LAN Function ###

nondump.obj : $(SRC3)\nondump.c
    $(CC) $(CFLAGS) -def=CH=1 $(SRC3)\nondump.c

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
    $(CC) $(CFLAGS) -def=$(ENDIAN) $(SRC3)\nondhcp.c

nonedns.obj : $(SRC3)\nonedns.c
    $(CC) $(CFLAGS) -def=$(ENDIAN) $(SRC3)\nonedns.c

### Executable Program Files ###

$(TARGET) : $(OBJS) $(LIBS) $(MAKEFILE)
    $(LD) $(LDFLAGS) -FORM=ABSOLUTE -ENTRY=_RESET -DEBUG -LIST -SHOW=SYMBOL
    $(LD) $(LDFLAGS) -FORM=STYPE

# end
