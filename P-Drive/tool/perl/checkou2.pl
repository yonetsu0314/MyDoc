#
#	checkou2.pl	1993.12.10	R.Nishimura
#
# ���
#	RCS�� ident �o�̓t�@�C����ǂݍ���� $Header$ �̓W�J���ʂ��Q�Ƃ��A
#	���ϐ� RCS �Ŏ����ꂽ�f�B���N�g������
#	�\���t�@�C����S�� ���[�h�I�����[�� �`�F�b�N�A�E�g����B
#		           ~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>set RCS=p:\wcu\main\rz_wcu\rcs\	�Ǘ��t�@�C���ۊǏꏊ�w��
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkou2.pl r0003_12		�h�c�t�@�C�����w��(�g���q�s�v)
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	if (/\$Header: (\S+) *(\S+)/) {
		@fname = split(/\W/, $1);
		print `co -r$2 $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
	};
};
close(FILE);
#
#	end of file
#
