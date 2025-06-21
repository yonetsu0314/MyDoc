DRV 	=p:

checkin :
	perl $(DRV)\tool\perl\checkin.pl *.h	>  xxx.bat
	perl $(DRV)\tool\perl\checkin.pl *.c	>> xxx.bat
	perl $(DRV)\tool\perl\checkin.pl *.mk	>> xxx.bat
	perl $(DRV)\tool\perl\checkin.pl *.asm	>> xxx.bat
	perl $(DRV)\tool\perl\checkin.pl *.mac	>> xxx.bat
	perl $(DRV)\tool\perl\checkin.pl *.mak	>> xxx.bat

release :
	if exist *.h	ident *.h	>  release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.mk	ident *.mk	>> release.id
	if exist *.asm	ident *.asm	>> release.id
	if exist *.mac	ident *.mac	>> release.id
	if exist *.mak	ident *.mak	>> release.id

