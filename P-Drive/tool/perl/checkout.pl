#
#	checkout.pl	1993.12.13	R.Nishimura
#
# ���
#	���ϐ� RCS �Ŏ����ꂽ�f�B���N�g���̐e�f�B���N�g���ɂ���
#	RCS�� ident �o�̓t�@�C����ǂݍ���� 
#	$Header$ �� $Log �̓W�J���ʂ��Q�Ƃ��A
#	�\���t�@�C����S�� ���[�h�I�����[�� �`�F�b�N�A�E�g����B
#		           ~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>set RCS=p:\wcu\main\rz_wcu\rcs\	�Ǘ��t�@�C���ۊǏꏊ�w��
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkout.pl r0003_1.2		�h�c�t�@�C�����w��
#
$id = $ENV{'RCS'}."..\\".$ARGV[0];
open(FILE, $id) || die "Cannot open $id: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	$rev = $2 if (/\$Header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$Log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		print `co -r$rev $rcs`;
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
#	end of file
#
