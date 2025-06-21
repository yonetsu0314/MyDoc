#
#	revco2.pl	1994.02.08	R.Nishimura
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
#		b:>perl revco2.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	if (/^\S/) {
		if( $rev ne '' ) {
			@fname = split(/\W/, $rcs2);
			if($ENV{'RCS'} ne '') {
				print `co -r$rev $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co -r$rev p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
		$rev = '';
		$rcs = '';
	} else {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
			$rcs2 = $1;
		} elsif (/\$Log: (\S+) */) {
			$rcs = $1;
		}
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
}
close(FILE);
#
#	end of file
#
