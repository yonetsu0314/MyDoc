#
#	revco.pl	1993.12.18	R.Nishimura
#
# ���
#	RCS�� ident �o�̓t�@�C����ǂݍ���� 
#	$Log$ �̓W�J���ʂ��Q�Ƃ��A
#	�\���t�@�C����S�� ���[�h�I�����[�� �`�F�b�N�A�E�g����B
#		           ~~~~~~~~~~~~~~~~
# �g�p���@
#	1.RCS�t�@�C���̗L��ꏊ���w��
#	  ���ϐ��̐ݒ�
#		b:>set RCS=p:\wcu\main\rz_wcu\rcs\
#	  �܂��̓J�����g�f�B���N�g���[�̐ݒ�
#		b:>p:
#		p:>cd \wcu\main\rz_wcu\rcs
#		p:>b:
#	2.�`�F�b�N�A�E�g��̐ݒ�Ǝ��s
#		b:>cd \work
#		b:>perl revco.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		if( index($rcs, "p:/") >= $[ ) {
			print `co -r$rev $rcs`;
		} elsif( index($rcs, "p:\\") >= $[ ) {
			print `co -r$rev $rcs`;
		} else {
			@fname = split(/\W/, $rcs);
			if($ENV{'RCS'} ne '') {
				print `co -r$rev $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co -r$rev p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
#	end of file
#
