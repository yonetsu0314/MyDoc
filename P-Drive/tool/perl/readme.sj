This is jperl, version 4.036 + 1.4 Oct 31 1993

$RCSfile: perl.c,v $$Revision: 4.0.1.8 $$Date: 1993/02/05 19:39:30 $
Patch level: 36

Copyright (c) 1989, 1990, 1991, Larry Wall
MS-DOS port Copyright (c) 1989, 1990, Diomidis Spinellis

Perl may be copied only under the terms of either the Artistic License or the
GNU General Public License, which may be found in the Perl 4.0 source kit.

------------------------------------------------------

Larry Wall���ɂ��perl(Practical Extraction and Report Language)��
���{��Ή���(MS-DOS�ł̎��s�`���ƁA�\�[�X��patch)�ł��B

jperl14.lzh(MS-DOS�Ŏ��s�`��)�ɂ͈ȉ��̃t�@�C�����܂܂�܂��B
	readme.sj     - ���̃t�@�C��
	jperl.exe     - ���{��Ή�perl��MS-DOS�p���s�`��
	jperl         - �V MS-DOS�pGO32�ł̎��s�`��
	perlglob.exe  - �t�@�C�����u��(glob)���s���Ƃ��g�p�����
	cpppp.sed     - -P�I�v�V�������Ɏg�p�����sed�X�N���v�g
	cpp.exe       - �V C PreProcessor
	jperl14.dif   - �\�[�X��context diff

patch�ɂ�unix�p�̂��̂�MS-DOS�p�̂��̂��S�Ċ܂܂�Ă��܂��B
		patch -p -N -d PERLDIR < jperl14.dif
���邢��perl�̃\�[�X�̂���f�B���N�g����
		patch -p -N < jperl14.dif
�Ƃ��Ă��������B

MSDOS�p�̎��s�`�����ăR���p�C������ɂ́Amsdos�f�B���N�g�����ɂ���t�@
�C����perl�f�B���N�g���ɃR�s�[���Aconfig_h.ms(MS-C�p)�܂���
config_h.dj(djgcc�p)�̂����ꂩ��config.h��rename�܂���copy���Ďg�p����
���������B

MS-DOS�Ŏ��s�`����Microsoft Visual C++ 1.00���g�p���č쐬���Atest
script�̖w��(MS-DOS��ł͓����������Ȃ����̂������A�t�@�C������echo��
�d�l�̈Ⴂ�����C����������)���p�X���܂����B

���Ή������ɂ���

�{�h�L�������g�ł͂ЂƂ��炰�Ɂg�����h���邢�́g���{��h�ƌ����\���Ă�
�܂����Ajperl�������镶����͋N���I�v�V�����ɂ����

	-Lsjis		���{��Shift-JIS
	-Leuc		���{��EUC
	-Ltca		������(��p)TCA
	-Lkseuc		�؍���EUC(KS C5601-1987)
	-Llatin		1�o�C�g�����̂�

��5�ʂ�ɐ؂�ւ��܂��B-L�I�v�V�������ȗ������Ƃ��̕�����̓R���p�C
�����Ɍ��肳��܂��BMSDOS�p�̎��s�`���ł͓��{��Shift JIS�ƂȂ��Ă��܂��B


�������Ή��͈̔�

�E������
	Shift-JIS�̂Ƃ������񒆂̊�����2�o�C�g�ڂ̓��^�L�����N�^��
	�Փ˂��܂���B
	token�Ɋ������g�����Ƃ͂ł��܂���B
	������̑召�֌W�́AC���C�u�����֐�memcmp�Ɉˑ����܂��B
	���C�u�����֐���memcmp��signed char�Ŕ�r���Ă���ꍇ�A
	"A" gt "�`"("A"�̂ق����傫��)�ƂȂ��Ă��܂��܂��B
	���̂悤�ȂƂ��́Aconfig.h��#undef HAS_MEMCMP���Ă��������B
	Microsoft C�ł�unsigned char�ł����ANEWS-OS�Ȃǂł�
	signed char�ł����B

�E���K�\��
	.        ��1�o�C�g/2�o�C�g�����ꂩ�́g1�����h��
	[aA���`] �̓A���t�@�x�b�g�̃G�[��
	[��-��]  �͂Ђ炪�Ȃ�
	����+    �́g�����h,�g�������h,�g�����������������h���ƃ}�b�`���܂�

�Etr, y
	tr/�`-�y/A-Z/; �̂悤��2�o�C�g�������g1�����h�Ƃ��Ĉ����܂��B
	�܂��A�I�v�V�����Ƃ���'B'��ǉ����Ă��܂��B
	���̃I�v�V�������t����ꂽ�ꍇ�ASEARCHLIST, REPLACEMENT,
	�y�ђu���̑ΏۂƂȂ镶����̊������ӎ����܂���B
	�ȉ��̗��EBCDIC�R�[�h�̐�����ASCII�ɕϊ����܂��B
		y/\360-\371/\060-\071/B;

�Eformat
	format���ꂽ�����񂪊������E�Ő؂�Ă��܂��悤�ȏꍇ�A
	' '��padding���đ����܂��B
	���l�ɁA"..."�ŏI��ꍇ"... "���t�����܂��B

�Echop
	chop �͖����̕����������ł���Ƃ��A���̊���1�������͂����܂��B
	�Ԃ������͊���1����(2�o�C�g��)�ɂȂ�܂��B
	�����A�����̕�����������1�o�C�g�ڂƂȂ��Ă���悤�ȏꍇ�A
	����1�o�C�g���͂����܂��B
	perl.man��-l�I�v�V�����̍��Ɏ�����Ă���80�J�����Ő؂葵������
	�����ɑΉ����邽�߂ɂ͈ȉ��̂悤�ɂ��邱�ƂɂȂ�܂��B
		jperl -lpe 'substr($_, 81)="";chop;'

�Esplit
	split(//) �͊������ӎ����܂��B����ɂ���Đ��������z���
	�o�C�g�P�ʂł͂Ȃ������P�ʂŕ������ꂽ���̂ƂȂ�܂��B
	�ȉ��̗�́A������́g�������h��Ԃ��܂��B

		sub jlength {split(//, pop(@_)) + 0;}

	�o�C�g�P�ʂł̕������K�v�ȏꍇ��unpack��substr���g���Ă��������B

�Esubstr
	substr�͊������ӎ�*���܂���*�B
	�������ӎ�����$off�����ڂ���$len�����؂���T�u���[�`����
	�ȉ��̂悤�ɏ������Ƃ��ł��܂��B
		sub jsubstr {
			local($str, $off, $len) = @_;
			local(@s);
			@s = split(//,$str);
			$len = $#s unless $len;
			return join("", @s[$off..($off+$len-1)]);
		}

�Eindex, rindex
	��������������ӎ����������������Ȃ��܂����A
	�Ԃ��l�̓o�C�g�ʒu�ł��B�����ʒu��Ԃ��悤�ȃT�u���[�`����
	�ȉ��̂悤�ɏ������Ƃ��ł��܂��B
		sub jindex {
			local($str, $subs, $pos) = @_;
			local($i);
			return $i if (($i = index($str, $subs, $pos)) < 0);
			return &jlength(substr($str, $[, $i-$[)) + $[;
		}

�E-B, -T
	�t�@�C���e�X�g���Z�q�̂����A-T, -B�ł́A2�o�C�g�������\������
	char��odd character�Ƃ݂Ȃ��܂���B

�Ereverse
	�X�J���[�R���e�L�X�g�ł�reverse�͊������ӎ�*���܂���*�B
	�������ӎ�����reverse����ɂ�

		sub jreverse {join('', reverse(split(//, pop(@_))));}

	�̂悤�ɂ��Ă��������B


��MS-DOS��
	�R�}���h���C���̓V���O���N�H�[�g(')�ł�����܂����A
	| �� > �A�����󔒓���command.com�ɉ��߂���Ă��܂�
	�����̓_�u���N�H�[�g(")�Ŋ���悤�ɂ��Ă��������B
	�܂��Afile.[ch]��manual.{man,doc,txt}�̂悤��
	csh����globbing���\�ł�(perlglob.exe�����l)�B
	GO32�łł͂��̋@�\�̓T�|�[�g����Ă��܂���B

	-P�I�v�V�������g���ꍇ�ɂ́APATH�̒ʂ��Ă���Ƃ����
	sed��cpp���K�v�ł��Bjperl�́Aperl�X�N���v�g��ǂޑO��
	sed -f cpppp.sed scriptfile | cpp -DMSDOS -I/usr/local/lib/perl
	�����s���܂��Bcpppp.sed��sed���ǂݍ��߂�ꏊ�ɒu���Ă��������B
	�Y�t��cpp.exe�́A�G���E�G�X�E�A�C �W���p��(��)�����LSI-C���H�ł�
	�܂܂�Ă�����̂ł��B

	globbing (while (<*.c>) {...}�̂悤��) ���g�p����ꍇ�ɂ�
	perlglob.exe ��PATH�̒ʂ��Ă���Ƃ���ɒu���Ă����Ă��������B
	csh�̂悤��[..]�����{,}���g���܂��B�����jperl.exe��
	�R�}���h���C����ł����l�ł��B
	�\�[�X��msdos/glob.c�ƂȂ��Ă���Amake�����glob.exe��
	�ł��܂����Aperlglob.exe�͂����rename�������̂ł��B

	system()��``�ł� system("/bin/ls > /tmp/tmp$$"); �̂悤��
	path�̋�؂��'/'���g���Ă����������삵�܂����A
	command.com�̓����R�}���h�����s���悤�Ƃ���ꍇ
	switch character�ɂ���Ă͐���������ł��܂���B
	�܂� 2>&1 �̂悤�Ƀt�@�C���n���h���̐؂�ւ����T�|�[�g�����
	���܂��B
		$msg = `cmd -invalid_option 2>&1`;
	�́A(���Ԃ�)cmd�̃G���[���b�Z�[�W��$msg�֑�����܂��B
	GO32�łł͂��̋@�\�̓T�|�[�g����Ă��܂���B

	crypt()��NDBM���T�|�[�g����Ă��܂��B
	crypt()�͐X����Y(kmori)���̍�i�ŁAunix�̂���ƌ݊���������܂��B
	NDBM���C�u������Ozan Yigit����sdbm��MS-DOS�ɈڐA�������̂ł��B

���\�[�X
	�ȉ��̃t�@�C���������Ă��܂��B

		config_h.dj      djgcc�p��config.h
		makefile.dj      djgcc�p��Makefile
		config_h.ms      MSC�p��config.h
		makefile.ms      MSC�p��Makefile
		kanji.[ch]       ���������p
		msdos/djdos.c    djgcc�p�̕⏕�t�@�C��
		msdos/rand.c     random���Ăяo���C���^�[�t�F�[�X
		msdos/random.c   �����W�F�l���[�^
		msdos/xargs.[ch] ���C���h�J�[�h�W�J
		msdos/cpppp.sed  -P�I�v�V�����g�p����sed���g���X�N���v�g
		msdos/crypt.c    crypt()�B�X����Y(kmori)���̍�i
		msdos/ndbm.[ch]  NDBM���C�u�����BOzan Yigit����sdbm��
		                 MS-DOS�ɈڐA��������
		msdos/xsystem.c  �l��Ԃ�system()

���o�O
	format��"^"�t�B�[���h�́A�֑����s���܂���B
	$: �Ɋ������Z�b�g���Ă���������܂��B

	$] �̓I���W�i���Ɠ����l��Ԃ��܂��B

	MS-DOS�ł�80[123]?87���g���܂���B
	�I�v�V����(-FPa)��ύX���čăR���p�C������K�v���L��܂��B

	jperl�́AJunker's Pathologically Eclectic Rubbish Lister�̗�
	�ł͂���܂���B

----_--__---_-_-_-__--_-__-__---_-_----_--_-_---_---_-----
�c��serow�ǒm
pcs19614 (ascii-net pcs)
serow (mix)
