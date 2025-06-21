#
#	checkin3.pl	1993.12.09	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ���t�@�C������
#	�`�F�b�N�C���Ώۃt�@�C���i�������݉\�����j�݂̂̃`�F�b�N�C��
#	�R�}���h�s�� -r -u -m �I�v�V�����t���� ��������B
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>set RCS=p:\wcu\main\rz_wcu\rcs\	�Ǘ��t�@�C���ۊǏꏊ�w��
#	b:>cd \work				���[�N�t�@�C���W�J�ꏊ�w��
#	b:>perl checkin3.pl *.* > a.bat		�o�b�`�t�@�C���쐬
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
	@fname = split(/\./, $fname);
	if( $fname[1] eq '' ) {
		$fname[1] = 'v';
	} elsif ( length($fname[1]) == 3 ) {
		chop( $fname[1] );
		$fname[1] .= 'v';
	} else {
		$fname[1] .= 'v';
	}
	$fname2 = $ENV{'RCS'}.$fname[0].'.'.$fname[1];
	$2 = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		print STDERR $1,' ',$2,"\n" if /\$Header: (\S+) *(\S+)/;
	}
	close(FILE);
	if($2 eq '') {
		$rel = "1.1";
	} else {
		@rel = split(/\./, $2);
		$rel[$#rel]++;
		$rel = join('.', @rel );
	}
	print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $fname2\n";
	if($fname[1] eq "mkv") {
		print "rcs -c\"#* \" $fname2\n";
		print "co $fname $fname2\n";
	}
}
#
#	end of file
#
