REM 日立C/C++ コンパイラパッケージ 7.1.01用 DOS窓環境作成バッチファイル

@ECHO OFF
REM Root of Hew installed path.
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1

REM make.exe等へのパスが通っていない場合は追加する必要が有ります。
REM 追加実行パスの設定(※要カスタマイズ)START
GOTO MYPATH1
IF NOT EXIST c:\pf\bin\dw22ctx.exe GOTO MYPATH1
SET path=c:\pf\bin;%PATH%
:MYPATH1
REM 追加実行パスの設定(※要カスタマイズ)END

set SHC_DIR=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%
set SHC_LIB=%SHC_DIR%\bin
set SHC_INC=%SHC_DIR%\include
set PATH=%SHC_LIB%;%HEW_DIR%;%PATH%
set SHC_TMP=C:\TEMP
set HLNK_TMP=C:\TEMP
set HEW_DIR=
set SHC_DIR=

IF EXIST %SHC_LIB%\shc.exe GOTO PATHOK
@echo %SHC_LIB%\shc.exeが見つかりませんHewインストールパスを確認してください。
pause
GOTO END
:PATHOK
if exist %SHC_TMP% GOTO MDEND
mkdir %SHC_TMP%
:MDEND
DOSKEY
:END
