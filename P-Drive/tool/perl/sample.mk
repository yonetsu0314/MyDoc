#
#  チェックインの準備
#　(例) make -S -fr0003.mk checkin"
#
checkin :
 $(DRV)\tool\perl\perl $(DRV)\tool\perl\checkin.pl *.mk	 >  $(TARGET).ba
 $(DRV)\tool\perl\perl $(DRV)\tool\perl\checkin.pl *.h	 >> $(TARGET).ba
 $(DRV)\tool\perl\perl $(DRV)\tool\perl\checkin.pl *.c	 >> $(TARGET).bat
 $(DRV)\tool\perl\perl $(DRV)\tool\perl\checkin.pl *.asm >> $(TARGET).ba
#
#  リリース時の手続き
#  (例) make -fr0003.mk -DVER=a -DREL=r0003_1.1 release
#
RCS	= p:\wcu\main\rz_wcu
#
release :
!if $d(REL)
	ident	*.mk	>  $(RCS)\$(REL)
	ident	*.h	>> $(RCS)\$(REL)
	ident	*.c	>> $(RCS)\$(REL)
	ident	*.asm	>> $(RCS)\$(REL)
	attr	+r	   $(RCS)\$(REL)
	copy $(TARGET)$(OUTPUT) $(RCS)\$(TARGET)$(VER)$(OUTPUT)
!else
	ident	*.mk	>  release.id
	ident	*.h	>> release.id
	ident	*.c	>> release.id
	ident	*.asm	>> release.id
!endif
#
