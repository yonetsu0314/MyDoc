REM ���l�T�XC/C++ �R���p�C���p�b�P�[�W 7.1.02, 7.1.01�p DOS�����쐬�o�b�`�t�@�C��

@ECHO OFF
REM ���l�T�XC �C���X�g�[���p�X�̊m�F�B
set HEW_DIR=C:\Hew2
set SHC_VAR=7_1_1
IF EXIST %HEW_DIR%\Tools\Hitachi\Sh\%SHC_VAR%\bin\shc.exe GOTO PATHOK
:PATHNG
@echo SHC 7.1.02/7.1.01 Hew�C���X�g�[���p�X���m�F���Ă��������B
pause
GOTO END
:PATHOK

REM make.exe���ւ̃p�X���ʂ��Ă��Ȃ��ꍇ�͒ǉ�����K�v���L��܂��B
REM �ǉ����s�p�X�̐ݒ�(���v�J�X�^�}�C�Y)START
GOTO MYPATH1
IF NOT EXIST c:\pf\bin\dw22ctx.exe GOTO MYPATH1
SET path=c:\pf\bin;%PATH%
:MYPATH1
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
DOSKEY
:END
