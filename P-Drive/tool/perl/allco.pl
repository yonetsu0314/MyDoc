#
#	allco.pl	1993.10.19	R.Nishimura
#
# ���
#	RCS��ident�o�̓t�@�C��(rxxxxx.id)��ǂݍ����
#	$Header$�̓W�J���ʂ��Q�Ƃ��A
#	�\�����Ă���t�@�C����S�� ���[�h�I�����[�� �`�F�b�N�A�E�g���܂��B
#				   ~~~~~~~~~~~~~~~~
#		b:>perl allco.pl p:..\rxxxxx.id
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	print `co -r$2 $1` if /\$Header: (\S+) *(\S+)/;
}
#
#	end of file
#
