#
#	delfile.pl	1995.12.14	R.Nishimura
#
# ���
#	�폜�t�@�C�����X�g�ɏ]���āA�t�@�C���̏������s���B
#
# �g�p���@
#	b:>perl delfile.pl delfile.lst
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		chop;
		if( -s ) {
			print "$_\n";
			chmod 0777, $_;
			unlink( $_ );
		}
	}
	close(FILE);
}
#
#	end of file
#
