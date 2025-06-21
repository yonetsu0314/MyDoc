@echo off
rem					1995.11/21 NAKAMURA Yo.
if "%1"==""	goto ERR
if "%2"==""	goto ERR
if "%3"==""	goto ERR
	perl p:\tool\perl\to7d.pl %1 < %2 > %3
goto end
:ERR
	echo "to7d: useage ---- to7d OPTIONS IN-FILE OUT-FILE"
:END
