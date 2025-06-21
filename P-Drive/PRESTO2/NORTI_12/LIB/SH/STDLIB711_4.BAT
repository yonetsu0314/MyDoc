REM 日立C 7.1.01用 標準ライブラリ(SH-4用)作成バッチファイル

@ECHO OFF
REM Root of Hew installed path.
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1

set SHC_DIR=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%
set SHC_LIB=%SHC_DIR%\bin
set SHC_INC=%SHC_DIR%\include
set PATH=%SHC_LIB%;%HEW_DIR%;%PATH%
set SHC_TMP=C:\TEMP
set HLNK_TMP=C:\TEMP
set HEW_DIR=
IF EXIST %SHC_LIB%\shc.exe GOTO PATHOK
@echo %SHC_LIB%\shc.exeが見つかりませんHewインストールパスを確認してください。
pause
GOTO END
:PATHOK
if exist %SHC_TMP% GOTO MDEND
mkdir %SHC_TMP%
:MDEND
IF EXIST %SHC_DIR%\LIB GOTO LIBOK
mkdir %SHC_DIR%\LIB
:LIBOK
cd %SHC_DIR%\LIB
GOTO SH4

:SH1
%SHC_LIB%\lbgsh.exe -head=ALL -output=shclib   -cp=sh1  -pic=0

:SH2
%SHC_LIB%\lbgsh.exe -head=ALL -output=shcnpic  -cp=sh2  -pic=0
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc2enp  -cp=sh2e -pic=0

:SH3
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc3npb  -cp=sh3
%SHC_LIB%\lbgsh.exe -head=ALL -output=shc3npl  -cp=sh3  -en=l

:SH4
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nbmzn -cp=sh4  -en=b -pic=0 -denormalization=off -round=nearest
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nlmzn -cp=sh4  -en=l -pic=0 -denormalization=off -round=nearest
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nbfzn -cp=sh4  -en=b -pic=0 -denormalization=off -round=nearest -fpu=single
%SHC_LIB%\lbgsh.exe -head=ALL -output=sh4nlfzn -cp=sh4  -en=l -pic=0 -denormalization=off -round=nearest -fpu=single

:END
