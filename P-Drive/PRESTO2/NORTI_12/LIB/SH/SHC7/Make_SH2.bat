REM ���l�T�XC 7.1.03 7.1.02�p NORTi���C�u�����쐬�o�b�`�t�@�C��

@ECHO OFF
REM NORTi �C���X�g�[���p�X�̊m�F
IF EXIST C:\NORTi\Inc\n4rsh.h GOTO NPATHOK
:NPATHNG
@echo NORTi �C���X�g�[���p�X���m�F���Ă��������B
pause
GOTO END
:NPATHOK
cd C:\NORTi\LIB\SH\SHC7

REM ���l�T�XC �C���X�g�[���p�X�̊m�F�B
set HEW_DIR=C:\Hew3
set SHC_VAR=7_1_2
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_2
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
@echo Hew�C���X�g�[���p�X���m�F���Ă��������B
pause
GOTO END
:PATHOK

REM �ǉ����s�p�X�̐ݒ��(���v�J�X�^�}�C�Y)START
REM SET path=c:\Progra~1\bin;%PATH%
REM SET path=%USERPROFILE%\My Documents\Bin;%PATH%
REM �ǉ����s�p�X�̐ݒ�(���v�J�X�^�}�C�Y)END

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
@ECHO ON
GOTO SH2

:SH1
make -fn4dsh1.mak
make -fn4esh1.mak
make -fn4fsh1.mak
make -fn4nsh1.mak

:SH2
make -fn4dsh2.mak
make -fn4esh2.mak
make -fn4fsh2.mak
make -fn4nsh2.mak

:SH2E
make -fn4esh2e.mak
make -fn4fsh2e.mak
GOTO END

:SH3
make -fn4dsh3b.mak
make -fn4dsh3l.mak
make -fn4esh3b.mak
make -fn4esh3l.mak
make -fn4fsh3b.mak
make -fn4fsh3l.mak
make -fn4nsh3b.mak
make -fn4nsh3l.mak

:SH4
make -fn4dsh4b.mak
make -fn4dsh4l.mak
make -fn4esh4b.mak
make -fn4esh4db.mak
make -fn4esh4dl.mak
make -fn4esh4dxb.mak
make -fn4esh4dxl.mak
make -fn4esh4l.mak
make -fn4esh4xb.mak
make -fn4esh4xl.mak
make -fn4fsh4b.mak
make -fn4fsh4db.mak
make -fn4fsh4dl.mak
make -fn4fsh4dxb.mak
make -fn4fsh4dxl.mak
make -fn4fsh4l.mak
make -fn4fsh4xb.mak
make -fn4fsh4xl.mak
make -fn4nsh4b.mak
make -fn4nsh4l.mak

:END
