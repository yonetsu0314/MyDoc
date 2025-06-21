REM ルネサスC/C++ コンパイラパッケージ 7.1.02, 7.1.01用 DOS窓環境作成バッチファイル

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

REM make.exe等へのパスが通っていない場合は追加する必要が有ります。
REM 追加実行パスの設定(※要カスタマイズ)START
GOTO MYPATH1
IF NOT EXIST c:\pf\bin\dw22ctx.exe GOTO MYPATH1
SET path=c:\pf\bin;%PATH%
:MYPATH1
REM 追加実行パスの設定(※要カスタマイズ)END

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
DOSKEY
:END
