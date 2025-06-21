#
#	setlabel.pl	1995.10.03	T.Ohtani
#
#	original : checkin.pl	1994.06.15	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ���t�@�C������
#
# �g�p���@
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkin.pl *.* > a.bat		�o�b�`�t�@�C���쐬
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	@fname = split(/\./, $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev eq '') {
		$rel = "1.1";
		print "ci -r$rel -nLABEL $fname\n";
	} else {
		@rel = split(/\./, $rev);
		$#rel--;
		$rel = join('.', @rel );
		print "rcs -nLABEL:$rel $rcs\n";
	}
}
#
#	end of file
#
