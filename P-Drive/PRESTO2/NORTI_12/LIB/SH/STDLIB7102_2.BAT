REM ルネサスC 7.1.02, 7.1.01用 標準ライブラリ(SH-2用)作成バッチファイル

@ECHO OFF
REM ルネサスC インストールパスの確認。
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
:PATHNG
@echo SHC 7.1.02/7.1.01 Hewインストールパスを確認してください。
pause
GOTO END
:PATHOK

set SHC_DIR=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%
IF EXIST %SHC_DIR%\LIB GOTO LIBOK
mkdir %SHC_DIR%\LIB
:LIBOK
cd %SHC_DIR%\LIB
set SHC_DIR=
set SHC_LIB=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin
set SHC_INC=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\include
set SHC_VAR=
set PATH=%SHC_LIB%;%HEW_DIR%;%PATH%
set HEW_DIR=
set SHC_TMP=C:\TEMP
set HLNK_TMP=C:\TEMP

if exist %SHC_TMP% GOTO MDEND
mkdir %SHC_TMP%
:MDEND
GOTO SH2

:SH1
%SHC_LIB%\lbgsh.exe -head=ALL -output=shclib   -cp=sh1  -pic=0

:SH2
%SHC_LIB%\lbgsh.exe -head=ALL -output=shcnpic  -cp=sh2  -pic=0
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc2enp  -cp=sh2e -pic=0
GOTO END

:SH3
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc3npb  -cp=sh3
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc3npl  -cp=sh3  -en=l

:SH4
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nbmzn -cp=sh4  -en=b -pic=0 -denormalization=off -round=nearest
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nlmzn -cp=sh4  -en=l -pic=0 -denormalization=off -round=nearest
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nbfzn -cp=sh4  -en=b -pic=0 -denormalization=off -round=nearest -fpu=single
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nlfzn -cp=sh4  -en=l -pic=0 -denormalization=off -round=nearest -fpu=single

:END
