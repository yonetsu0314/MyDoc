#
#	branch.pl	1995.09.27	T.Ohtani
#
#	original : checkin.pl	1994.06.15	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ���t�@�C������
#	�`�F�b�N�C���Ώۃt�@�C���i�������݉\�����j�݂̂̃`�F�b�N�C��
#	�R�}���h�s�� -r -u -m �I�v�V�����t���� ��������B
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkin.pl *.* > a.bat		�o�b�`�t�@�C���쐬
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	if($ENV{'ALL'} eq '') {
		next unless (-w $fname);
	}
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
		print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $fname\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $fname\n";
			print "co $fname $fname\n";
		}
	} else {
		@rel = split(/\./, $rev);
		if(($ENV{'HOLD'} ne '') && ($ENV{'BRANCH'} ne '')) {
			$rel[$#rel]++;
		}
		$rel = join('.', @rel );
		if($ENV{'BRANCH'} ne '') {
			$rel = "$rel.$ENV{'BRANCH'}";
		}
		print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $rcs\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $rcs\n";
			print "co $fname $rcs\n";
		}
	}
}
#
#	end of file
#
