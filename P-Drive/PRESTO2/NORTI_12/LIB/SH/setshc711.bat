REM ����C/C++ �R���p�C���p�b�P�[�W 7.1.01�p DOS�����쐬�o�b�`�t�@�C��

@ECHO OFF
REM Root of Hew installed path.
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1

REM make.exe���ւ̃p�X���ʂ��Ă��Ȃ��ꍇ�͒ǉ�����K�v���L��܂��B
REM �ǉ����s�p�X�̐ݒ�(���v�J�X�^�}�C�Y)START
GOTO MYPATH1
IF NOT EXIST c:\pf\bin\dw22ctx.exe GOTO MYPATH1
SET path=c:\pf\bin;%PATH%
:MYPATH1
REM �ǉ����s�p�X�̐ݒ�(���v�J�X�^�}�C�Y)END

set SHC_DIR=%HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%
set SHC_LIB=%SHC_DIR%\bin
set SHC_INC=%SHC_DIR%\include
set PATH=%SHC_LIB%;%HEW_DIR%;%PATH%
set SHC_TMP=C:\TEMP
set HLNK_TMP=C:\TEMP
set HEW_DIR=
set SHC_DIR=

IF EXIST %SHC_LIB%\shc.exe GOTO PATHOK
@echo %SHC_LIB%\shc.exe��������܂���Hew�C���X�g�[���p�X���m�F���Ă��������B
pause
GOTO END
:PATHOK
if exist %SHC_TMP% GOTO MDEND
mkdir %SHC_TMP%
:MDEND
DOSKEY
:END
