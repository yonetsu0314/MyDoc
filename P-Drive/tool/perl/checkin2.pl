#
#	checkin2.pl	2002.03.25	R.Nishimura
#	�x�[�X�Fcheckin.pl	1994.06.15	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ���t�@�C������
#	�`�F�b�N�C���Ώۃt�@�C���i�������݉\�����j�݂̂̃`�F�b�N�C��
#	�R�}���h�s�� -r -u �I�v�V�����t���� ��������B
#		     ~~~~~~~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkin2.pl *.* > a.bat		�o�b�`�t�@�C���쐬
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
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
		print "ci -r$rel -u $fname $fname\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $fname\n";
			print "co $fname $fname\n";
		}
	} else {
		@rel = split(/\./, $rev);
		$rel[$#rel]++;
		$rel = join('.', @rel );
		print "ci -h -r$rel -u $fname $rcs < log.txt\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $rcs\n";
			print "co $fname $rcs\n";
		}
	}
}
#
#	end of file
#
