REM ���l�T�XC/C++ �R���p�C���p�b�P�[�W 5.1F�p NORTi���C�u�����쐬�o�b�`�t�@�C��

@ECHO OFF
REM NORTi �C���X�g�[���p�X�̊m�F
IF EXIST C:\NORTi\Inc\n4rsh.h GOTO NPATHOK
:NPATHNG
@echo NORTi �C���X�g�[���p�X���m�F���Ă��������B
pause
GOTO END
:NPATHOK
cd C:\NORTi\LIB\SH\SHC5

REM ���l�T�XC �C���X�g�[���p�X�̊m�F
set HEW_DIR=C:\Hew
set SHC_VAR=5_1_0
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
:PATHNG
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
SET HEW_DIR=
set SHC_TMP=C:\TEMP
set HLNK_TMP=C:\TEMP

if exist %SHC_TMP% GOTO MDEND
mkdir %SHC_TMP%
:MDEND
@ECHO ON
GOTO SH3

:SH1
make -f n4dsh1.mak
make -f n4esh1.mak
make -f n4fsh1.mak
make -f n4nsh1.mak

:SH2
make -f n4dsh2.mak
make -f n4esh2.mak
make -f n4fsh2.mak
make -f n4nsh2.mak

:SH2E
make -f n4esh2e.mak
make -f n4fsh2e.mak

:SH3
make -f n4dsh3b.mak
make -f n4dsh3l.mak
make -f n4esh3b.mak
make -f n4esh3l.mak
make -f n4fsh3b.mak
make -f n4fsh3l.mak
make -f n4nsh3b.mak
make -f n4nsh3l.mak
GOTO END

:SH4
make -f n4dsh4b.mak
make -f n4dsh4l.mak
make -f n4esh4b.mak
make -f n4esh4db.mak
make -f n4esh4dl.mak
make -f n4esh4dxb.mak
make -f n4esh4dxl.mak
make -f n4esh4l.mak
make -f n4esh4xb.mak
make -f n4esh4xl.mak
make -f n4fsh4b.mak
make -f n4fsh4db.mak
make -f n4fsh4dl.mak
make -f n4fsh4dxb.mak
make -f n4fsh4dxl.mak
make -f n4fsh4l.mak
make -f n4fsh4xb.mak
make -f n4fsh4xl.mak
make -f n4nsh4b.mak
make -f n4nsh4l.mak

:END
