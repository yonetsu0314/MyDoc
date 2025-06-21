#
#	newco6.pl	1999.11.10	R.Nishimura
#	Original:newco.pl2	1994.07.15	R.Nishimura
#
# ���
#	�V�����݂�RCS ident �o�̓t�@�C����ǂݍ���� 
#	$Header$,$Log$ �̓W�J���ʂ��Q�Ƃ��A
#	�\���t�@�C����S�� ���[�h�I�����[�� �`�F�b�N�A�E�g����B
#		           ~~~~~~~~~~~~~~~~
#	���̃X�N���v�g�ł́AMKS-RCS 6.2 ���g�p���邱�Ƃ�O��Ƃ��Ă���B
#                              ~~~~~
#
# �g�p���@
#	1.�`�F�b�N�A�E�g��̐ݒ�Ǝ��s
#		b:>cd \work
#		b:>perl newco6.pl r0003-1.2
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
	if (/\$log: (\S+) */) {
		$l_rcs = $1;
	};
	if(( $rev ne '') && ( $l_rcs ne '')) {
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		@l_rcs_ = split(/\./, $l_rcs);
		if('v' eq chop($l_rcs_[$#l_rcs_])) {
			$l_rcs_[$#l_rcs_] = $f_typ[$#f_typ];
			$l_rcs = join('.', @l_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $l_rcs );
			}
		}
		@h_rcs_ = split(/\./, $h_rcs);
		if('v' eq chop($h_rcs_[$#h_rcs_])) {
			$h_rcs_[$#h_rcs_] = $f_typ[$#f_typ];
			$h_rcs = join('.', @h_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $h_rcs );
			}
		}
		if( index($l_rcs, "p:/") >= $[ ) {
			print "\nco.exe -g -O $l_rcs $outf";
		} elsif( index($l_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -g -O $l_rcs $outf";
		} elsif( index($h_rcs, "p:/") >= $[ ) {
			print "\nco.exe -g -O $h_rcs $l_rcs";
		} elsif( index($h_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -g -O $h_rcs $l_rcs";
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
		$l_rcs = '';
	}
}
close(FILE);
#
#	end of file
#
