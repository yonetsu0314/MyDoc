# mksiall.pl						1994.8.24 m.fukutome
#
# ������Ή��̂��߂� SI �t�@�C�����쐬����
#
#   perl mksiall.pl filename
#   filename : �������� SI �t�@�C���̃��X�g�Ə����f�B���N�g�����L�q�����t�@�C��
#	�t�H�[�}�b�g
#	  SIDIR = dirname	#�^�[�Q�b�g�f�B���N�g����
#	  L*DIR = dirname	#�\�[�X�f�B���N�g���� (*:1-4)
#	  mksi:
#	  file1.si:		#�����t�@�C����
#	  file2.si:
#	    |
#	  fileN.si:
#	  mksiend:
#	������̍s���s�̐擪����L�q����K�v������܂��B���̃t�H�[�}�b�g��
#	���C�N�t�@�C���̋L�q���@�ƌ݊���������܂��̂ŁA�����ɕK�v�ȏ���
#	���C�N�t�@�C���ɋL�q���邱�Ƃ��\�ł��B���������C�N�t�@�C������
#	�}�N���⃋�[���͏����ł��܂���̂Œ��ӂ��Ă��������B
#
#   �e�t�@�C���ɂ͎��̑��삪�s���܂��B
#   1) �e�f�B���N�g���� SI �t�@�C������ ELS_STR ��`���𒊏o���A���������z��
#      �Ƃ��ĐV���Ȓ�`���쐬����B
#   2) ELS_FRAME ��`���� ELSDRAW.H �̍\���̒�`�Ɠ��e������Ȃ��Ȃ邽��
#      ELS_FRAMES �ƍ\���̒�`����t���ւ���B
#   3) SISCR ��`���͍폜�����B
#   4) ELS_FRAMES ��`��L���ɂ��邽�� mltling.h ���C���N���[�h����B
#   
#   �x�[�X�t�@�C���� L1DIR �ɂ���t�@�C���ƂȂ�܂��B���̃f�B���N�g���̃t�@�C��
#   ����́AELS_STR ��`�̓��e�݂̂��g�p����܂��B

undef	@fname;
undef	@sdir;
undef	$tgdir;

while (<>) {
	s/#.*$//;
	/^mksi:/ && last;
	if (/^SIDIR[ \t]*=/) {
		s/=/ /;
		split;
		$tgdir = $_[1];
	}
	if (/^L([1-4])DIR[ \t]*=/) {
		$i = $1 - 1;
		s/=/ /;
		split;
		$sdir[$i] = $_[1];
	}
}

defined $tgdir || die "SIDIR �̒�`������܂���B";
defined @sdir  || die "L*DIR �̒�`������܂���B";

$dmax = $#sdir + 1;

while (<>) {
	/^([a-z_0-9]{1,8}\.si):/i && &mksisub($1);
	/^mksiend:/ && last;
}

sub mksisub {
	$fname = shift(@_);
	print "$fname:\n";
	for ($i = 0; $i < $dmax; ++$i) {
		$iname[$i] = "$sdir[$i]\\$fname";
	}
	$oname = "$tgdir\\$fname";
	if (!open(OFILE, ">$oname")) {
		warn "�t�@�C���i$oname�j���I�[�v���ł��܂���B\n";
		return;
	}
	$old_fp = select(OFILE);

	if (!open(FP0, "<$iname[0]")) {
		warn "�t�@�C���i$iname[0]�j���I�[�v���ł��܂���B\n";
		return;
	}
	while (<FP0>) {
		if (/^static[ \t]+ELS_STR[ \t]+far[ \t](\w+_str)\[\] = \{/) {
			&elsstr_func($1) == 0 && return;
		} elsif (/^#include[ \t]+\"si\.h\"/i) {
			print;
			print "#include \"mltling.h\"\n";
		} else {
			/^(SISCR)( .+_scr = {)/ && last;
			s/^(static ELS_FRAME)( .+_frame = {)/$1S$2/;
			print;
		}
	}
	close(FP0);
	select($old_fp);
	close(OFILE);
}

sub elsstr_func {
	$func_name = shift(@_);
	s/$func_name/$&0/;
	print;
	while (<FP0>) {
		print;
		/^};/ && last;
	}
	print "\n";
	for ($i = 1; $i < $dmax; ++$i) {
		if (!open(FP, "<$iname[$i]")) {
			warn "�t�@�C���i$iname[$i]�j���I�[�v���ł��܂���B\n";
			return 0;
		}
		while (<FP>) {
			/^static[ \t]+ELS_STR[ \t]+far[ \t]\w+_str\[\]/
				&& last;
		}
		s/$func_name/$&$i/;
		print;
		while (<FP>) {
			print;
			/^};/ && last;
		}
		print "\n";
		close(FP);
	}
	print "static ELS_STR * far $func_name[] = {\n";
	for ($i = 0; $i < $dmax; ++$i) {
		print "\t&$func_name${i}[0],\n";
	}
	print "};\n\n";
}

#
#	END OF FILE
#
