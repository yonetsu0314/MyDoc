#*==========================================================================
#*	Filename	: MOTCNV.MK
#*--------------------------------------------------------------------------
#*$Header: p:/tool/motcnv/RCS/MOTCNV.MK 1.1 1997-02-15 03:56:27+09 minaguchi Exp $
#*$Log: MOTCNV.MK $
#*Revision 1.1  1997-02-15 03:56:27+09  minaguchi
#*‰ŠúƒŠƒrƒWƒ‡ƒ“
#*
#*--------------------------------------------------------------------------
#*		(c)	Copyright 1996-1997, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
#*		TELEPHONE (0775)53-4141
#*--------------------------------------------------------------------------
#*
LINK=link
CC=cl /c /W4
#
MAIN=motcnv
SUB1=cnv
#
$(MAIN).exe : $(MAIN).obj $(SUB1).obj
	$(LINK) $(MAIN)+$(SUB1) , $(MAIN).exe;

$(MAIN).obj : $(MAIN).c
	$(CC) $(MAIN).c
$(SUB1).obj : $(SUB1).c
	$(CC) $(SUB1).c
#*==========================================================================
release:
!if $d(REV)
	ident	*.mk	>	$(MAIN)-$(REV)
	ident	*.c		>>	$(MAIN)-$(REV)
	ident	*.h		>>	$(MAIN)-$(REV)
	attrib +R $(MAIN)-$(REV)
!else
	@echo off
!endif
#*==========================================================================
