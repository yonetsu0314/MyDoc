==============================================================================

                   �͂��߂ɂ��ǂ݂�������(Readme.txt)
                              2002�N3��25��

                Copyright (c) MiSPO Co., Ltd. 1991-2002
                       http://www.mispo.co.jp/

==============================================================================

���̃t�@�C���ł̓R���s���[�e�b�N�X��ROMICE64�œ�����SolutionEngine(MS7709SE01)
���T���v���v���O�������f�o�b�N���邽�߂ɕK�v�Ȑݒ�ɕt���Đ������܂��B

�� SolutionEngine�̃f�B�b�v�X�C�b�`�ݒ�
�N���Ɋւ�镔���̐ݒ�����L�Ɏ����܂��B

SW3
����������������������������������������
����������������������������������������ON
����������������������  ����  ����  ����
����  ����  ����  ����������������������
����������������������������������������OFF
����������������������������������������
�@�@���@�@���@�@���@�@���@�@���@�@�� SW3-6 Endian ON=Big OFF=Little(�o�׎�)
�@�@���@�@���@�@���@�@���@�@�������� SW3-5 OFF �Œ�
�@�@���@�@���@�@���@�@�������������� SW3-4 OFF �Œ�
�@�@���@�@���@�@�������������������� SW3-3 ON  �Œ�
�@�@���@�@�������������������������� SW3-2 CPUMODE ON=MODE1(�o�׎�) OFF=MODE3
�@�@�������������������������������� SW3-1 CPUMODE ON=MODE1(�o�׎�) OFF=MODE3

SW4
����������������������������
����������������������������ON
����������������  ����  ����
����  ����  ����������������
����������������������������OFF
����������������������������
�@�@���@�@���@�@���@�@�� SW4-4�@OFF �Œ�
�@�@���@�@���@�@�������� SW4-3�@OFF �Œ�
�@�@���@�@�������������� SW4-2�@ON  �Œ�
�@�@�������������������� SW4-1�@ON=EPROM BOOT(�o�׎�) OFF=Flash BOOT


�� SolutionEngine��ROMICE64�̐ڑ�
�ȉ��̒ʂ�ڑ����ĉ������B

ROM�\�P�b�gM14 = ROMICE64 42PIN�v���[�u ROM1 & ROM2
ROM�\�P�b�gM13 = ROMICE64 42PIN�v���[�u ROM3 & ROM4
TP8 = NMI �@(YEL)��
TP7 = RESET (GRN)�� ROMICE64 CCN
TP6 = GND �@(BLK)��

�� ���[�U�E�J�X�^�}�C�Y�E�v���O�����ɕt����
SolutionEngine�̏����ݒ��ROMICE64�ōs���ׂɃ��[�U�E�J�X�^�}�C�Y�E�v���O������
�K�v�ɂȂ܂��B
�f�B�b�v�X�C�b�`SW3-6�̐ݒ�ɍ��������[�U�E�J�X�^�}�C�Y�E�v���O������CSIDE��
[�n�[�h�E�G�A�̐ݒ�]��[���j�^]��ʂɗL�郆�[�U�E�J�X�^�}�C�Y�E�v���O�����̎w��
�ɋL�����ĉ������B

WishB.UCM �@�r�b�N�G���f�B�A���p���[�U�E�J�X�^�}�C�Y�E�v���O����
WishL.UCM �@���g���G���f�B�A���p���[�U�E�J�X�^�}�C�Y�E�v���O����

�J�X�^�}�C�Y�������ŏC�����K�v�ȏꍇ���L�t�@�C�����g�p���ĕύX���Ă��������B

Wish.SRC�@�@�@��L�t�@�C���̃\�[�X�t�@�C��(�r�b�N�E���g���G���f�B�A������)
MkUcm.PIF�@   MS-DOS�A�v���P�[�V����MkUcm.BAT�ւ̃V���[�g�J�b�g
MkUcm2000.lnk MS-DOS�A�v���P�[�V����MkUcm.BAT�ւ̃V���[�g�J�b�g(Win2000�p)
MkUcm.BAT �@�@WishB.UCM, WishL.UCM�쐬�p�o�b�`�t�@�C��

����1�@HEW�y��CSIDE���f�t�H���g�f���N�g���ɃC���X�g�[�����Ă��Ȃ��ꍇBAT��PATH
��ҏW���ĉ������B
����2�@�p�ӂ���PIF�ȊO�����g�p�̏ꍇ���ϐ����K�v�Ȃ��ߊ��ϐ��̏����T�C�Y��
4096�ɂ��Ă��g�p�������B
����3�@�p�ӂ���PIF��Windows2000�ł͓����܂���BMkUcm2000.lnk�����g�p�������B

�� ROMICE64�̃n�[�h�E�G�A�̐ݒ�
�N���p��CSIDE�p�v���W�F�N�g�E�t�@�C����p�Ӓv���܂����B

CsideBH.CPF �@ROM�N���E�r�b�N�G���f�B�A���p�v���W�F�N�g�E�t�@�C��
CsideBHR.CPF�@RAM�N���E�r�b�N�G���f�B�A���p�v���W�F�N�g�E�t�@�C��
CsideLH.CPF �@ROM�N���E���g���G���f�B�A���p�v���W�F�N�g�E�t�@�C��
CsideLHR.CPF�@RAM�N���E���g���G���f�B�A���p�v���W�F�N�g�E�t�@�C��

RAM�N���̏ꍇ�ȉ��̎��ɂ����Ӊ������B
�N�����Ƀ\�[�X�t�@�C�����\������܂���B�\�[�X��t�@�C���őI�����ĉ������B
PC�̒l��AC000000�Ƃ�RAM�̈悩����s���ĉ������B

CSIDE�̃o�[�W������PATH�̈Ⴂ�ŋN�����Ȃ��ꍇ�͉��L�̐ݒ���Q�l�ɂ��ĉ������B

[CPU����]
�f�o�b�N���[�h [ROMICE]
CPU�̎�� [SH7709]
VBR�̐ݒ� [A0000000]�@RAM�N���̏ꍇ��[AC000000]
���̑��f�t�H���g

[ROM]
�f�[�^�E�o�X [32Bit]
�^�C�v(bit) [4M]
ROM1-4 [�`�F�b�N]
�A�h���X���N���b�N
..[ROM1-4]
�@�@�G���f�B�A�� [���g�����̓r�b�N] SW3-6�̐ݒ�ɍ����������g�p���ĉ������B
�@�@���̑��f�t�H���g

[���j�^]
�R�[�h�J�n�A�h���X [A01FF000]
�f�[�^�J�n�A�h���X [ACFFF000]
���[�U�E�J�X�^�}�C�Y�E�v���O�������g�p [�`�F�b�N]
���g���G���f�B�A���̂Ƃ�
  ���[�U�E�J�X�^�}�C�Y�E�v���O�����̎w�� [c:\NORTi\SMP\SH\MS7709\ROMICE64\WishL.UCM]
�r�b�N�G���f�B�A���̂Ƃ�
  ���[�U�E�J�X�^�}�C�Y�E�v���O�����̎w�� [c:\NORTi\SMP\SH\MS7709\ROMICE64\WishB.UCM]
(PATH�̓f�t�H���g�C���X�g�[���̏ꍇ�ł��BSW3-6�̐ݒ�ɍ����������g�p���ĉ������B)

[ROMICE64�̐M���ݒ�]
NMI�o�̓^�C�v [�I�[�v���E�R���N�^]
���̑��f�t�H���g


�X�V����

02/03/25
	Win2000�p�̏C��
	SHC7, SHC6, SHC5 �Ή�
	Readme.txt�@�@�C��
	MkBig.PIF, MkLittle.PIF�@MkUcm.PIF�ɓ���
	MkUcm.PIF�@�V�K�쐬
	MkBig.BAT�@�C��

Readme.txt�@�@02/03/13�@�V�K�쐬
WishB.UCM �@�@02/03/13�@�V�K�쐬
WishL.UCM �@�@02/03/13�@�V�K�쐬
Wish.SRC�@�@�@02/03/13�@�V�K�쐬
MkBig.PIF �@�@02/03/13�@�V�K�쐬
MkLittle.PIF�@02/03/13�@�V�K�쐬
MkBig.BAT �@�@02/03/13�@�V�K�쐬
MkLittle.BAT�@02/03/13�@�V�K�쐬
CsideB.CPF�@�@02/03/13�@�V�K�쐬
CsideL.CPF�@�@02/03/13�@�V�K�쐬
