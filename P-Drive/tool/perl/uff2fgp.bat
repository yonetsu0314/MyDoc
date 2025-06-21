@echo off
rem					1995.12/04 NAKAMURA Yo.
if "%1"==""	goto ERR
if "%2"==""	goto ERR
rem	if "%3"==""	goto ERR
	perl p:\tool\perl\uff2fgp.pl < %1 > %2
goto end
:ERR
rem	echo "uff2fgp: useage ---- uff2fgp OPTIONS IN-FILE OUT-FILE"
	echo "uff2fgp: useage ---- uff2fgp IN-FILE OUT-FILE"
:END
