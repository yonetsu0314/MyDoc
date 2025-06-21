#!perl
#	revnewco.pl	1995.10.24	M.Fukutome
#	orginal :revco.pl2	1994.07.15 R.Nishimura/1995.10.20 K.Nakaoka
#
# ���
#	�V�����݂�RCS ident �o�̓t�@�C����ǂݍ���� 
#	$Header$,$Log$ �̓W�J���ʂ��Q�Ƃ��A�\���t�@�C�����̃��r�W�����ԍ���
#	�����Ɋւ��čŐV�̃��r�W������S�ă��[�h�I�����[�Ń`�F�b�N�A�E�g����B
#	                                  ~~~~~~~~~~~~~~~~
#	MKS-RCS���g�p���邱�Ƃ�O��Ƃ��Ă���B
#
# �g�p���@
#	1.�`�F�b�N�A�E�g��̐ݒ�Ǝ��s
#		b:>cd \work
#		b:>perl revnewco.pl r0003_1.2
# ��������
# M.Fukutome 1997.05.13
#   $Header$ �W�J���݂̂��g�p����B���������� Freeware �ł� RCS �ɂ����
#   ����ꂽ�\���t�@�C���͎g�p�ł��Ȃ��B
#
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if ($rev ne '') {
		$rev =~ s/\.[0-9]+$//;
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		if( index($h_rcs, "p:/") >= $[ ) {
			print "\nco.exe -r$rev -O $h_rcs";
		} elsif( index($h_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -r$rev -O $h_rcs";
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
	}
}
close(FILE);
__EOF__
