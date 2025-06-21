REM 環境に合わせて下記PATHを設定して下さい。

SET CSIDE_MON=C:\Program Files\CSIDE\ETC\ROMICE64 SH3H\MKMON.EXE

IF EXIST "%CSIDE_MON%" GOTO HV710
@ECHO "%CSIDE_MON%" can't find.
PAUSE
GOTO END

:HV711
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\asmsh.exe GOTO HVDO

:HV710
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_0
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\asmsh.exe GOTO HVDO

:HV6
set HEW_DIR=C:\Hew
set SHC_VAR=6_0_a
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\asmsh.exe GOTO HVDO

:HV5
set HEW_DIR=C:\Hew
set SHC_VAR=5_1_0
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\asmsh.exe GOTO HVDO

:HVNONE
@ECHO HEW PATH can't find.
PAUSE
GOTO END

:HVDO
set SHC_LIB=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\BIN
set SHC_INC=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\include
set PATH=%SHC_LIB%;%PATH%
set SHC_TMP=C:\TEMP

:以下ucmファイル作成コマンド
asmsh wish.src -CPU=SH3 -ENDIAN=LITTLE
PAUSE
IF EXIST %SHC_LIB%\Cnvs.exe GOTO HVCNVSL
optlnk wish -FORM=Stype
GOTO DOMONL
:HVCNVSL
lnk wish
cnvs wish
:DOMONL
"%CSIDE_MON%" wish.mot wishL.ucm

asmsh wish.src -CPU=SH3 -ENDIAN=BIG
IF EXIST %SHC_LIB%\Cnvs.exe GOTO HVCNVSB
optlnk wish -FORM=Stype
GOTO DOMONB
:HVCNVSB
lnk wish
cnvs wish
:DOMONB
"%CSIDE_MON%" wish.mot wishB.ucm

del wish.obj
del wish.abs
del wish.mot

:END
PAUSE
