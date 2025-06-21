#
# NORTi4
# Network Library makefile for Renesas C (with Debug Option)
#

### PATH ###

INC = ..\..\..\INC
SRC = ..\..\..\SRC

### FILES ###

LIBFILE  = n4dsh2.lib
SRCFILE  = $(SRC)\nonet.c $(SRC)\nonetip.c $(SRC)\nonicmp.c $(SRC)\nonearp.c \
           $(SRC)\noneudp.c $(SRC)\nontcp1.c $(SRC)\nontcp2.c $(SRC)\nonelan.c \
           $(SRC)\nonitod.c $(SRC)\nonslib.c $(SRC)\nonigmp.c $(SRC)\noneipf.c
INCFILE  = $(INC)\nonet.h $(INC)\nonetc.h $(INC)\nonets.h
MAKEFILE = n4dsh2.mak
CMDFILE  = n4dsh2.sub

### COMMAND ###

CC       = shc
CFLAGS   = -cp=sh2 -deb -op=0 -i=$(INC) -nologo -gb=u
LD       = optlnk
LDFLAGS  = -subcommand=$(CMDFILE)

$(LIBFILE) : $(SRCFILE) $(INCFILE) $(MAKEFILE) $(CMDFILE)
    $(CC) $(CFLAGS) $(SRC)\nonet.c
    $(CC) $(CFLAGS) $(SRC)\nonetip.c
    $(CC) $(CFLAGS) $(SRC)\nonicmp.c
    $(CC) $(CFLAGS) $(SRC)\nonearp.c
    $(CC) $(CFLAGS) $(SRC)\noneudp.c
    $(CC) $(CFLAGS) $(SRC)\nontcp1.c
    $(CC) $(CFLAGS) $(SRC)\nontcp2.c
    $(CC) $(CFLAGS) $(SRC)\nonelan.c
    $(CC) $(CFLAGS) $(SRC)\nonitod.c
    $(CC) $(CFLAGS) $(SRC)\nonslib.c
    $(CC) $(CFLAGS) $(SRC)\nonigmp.c
    $(CC) $(CFLAGS) $(SRC)\noneipf.c
    $(LD) $(LDFLAGS)
    del *.obj

# end
