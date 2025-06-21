#
#	nrvnwco6.pl	1999.11.10	R.Nishimura
#	Original:revco.pl2	1994.07.15	R.Nishimura
#
# ���
#   $Header$ �W�J���݂̂��g�p����B���������� Freeware �ł� RCS �ɂ����
#   ����ꂽ�\���t�@�C���͎g�p�ł��Ȃ��B
#	$Header$�̓W�J���ʂ��Q�Ƃ��A�\���t�@�C�����̃��r�W�����ԍ���
#	�����Ɋւ��čŐV�̃��r�W������S�ă��[�h�I�����[�Ń`�F�b�N�A�E�g����B
#	                                  ~~~~~~~~~~~~~~~~
#	���̃X�N���v�g�ł́AMKS-RCS 6.2 ���g�p���邱�Ƃ�O��Ƃ��Ă���B
#                              ~~~~~
#
#	�\���Ǘ��t�@�C�����ȉ��̂悤�ɂȂ��Ă���ꍇ
#		els_eng/test.si:
#			$Header: p:/mks6test/rcs/test.si 1.2 2000/11/09 20:32:10 ryoji Exp $
#			$Log: test.si $
#
#	MKS-RCS 5.3 �p�� rvnwco.bat(p:/tool/perl/rvnwco.pl2) �ł́A
#
#			> co -O p:/mks6test/rcs/test.si
#				p:/mks6test/rcs/test/si ---> test.si
#
#	MKS-RCS 6.2 �p�� nrvnwco6.bat(p:/tool/perl/nrvnwco6.pl) �ł́A
#
#			> co -g -O p:/mks6test/rcs/test.si els_eng/test.si
#				p:/mks6test/rcs/test/si ---> els_eng/test.si
#
# �g�p���@
#	1.�`�F�b�N�A�E�g��̐ݒ�Ǝ��s
#		b:>cd \work
#		b:>perl nrvnwco6.pl r0003-1.2
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
		$outf = $wf;
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if( $rev ne '' ) {
		$rev =~ s/\.[0-9]+$//;
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		if( index($h_rcs, "p:/") >= $[ ) {
			print "\nco.exe -r$rev -g -O $h_rcs $l_rcs";
		} elsif( index($h_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -r$rev -g -O $h_rcs $l_rcs";
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
	}
}
close(FILE);
#
#	end of file
#
