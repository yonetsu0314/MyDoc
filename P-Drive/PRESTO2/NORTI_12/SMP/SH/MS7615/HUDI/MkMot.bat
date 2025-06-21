REM 環境に合わせて下記PATHを設定して下さい。

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
@ECHO asmsh.exe can't find.
PAUSE
GOTO END

:HVDO
set SHC_LIB=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\BIN
set SHC_INC=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\include
set PATH=%SHC_LIB%;%PATH%
set SHC_TMP=C:\TEMP

:以下ucmファイル作成コマンド
asmsh FMR7615h.asm -CPU=SH2 -o=FMR7615h.obj
IF EXIST %SHC_LIB%\Cnvs.exe GOTO HVCNVS
optlnk -su=hoptlnk.sub -NOOP -FORM=STYPE -OU=FMR7615.mot
GOTO DOMON
:HVCNVS
lnk -su=hlnk.sub
cnvs wish FMR7615h.abs .\FMR7615.mot
:DOMON

del FMR7615h.obj
del FMR7615h.abs

:END
PAUSE
