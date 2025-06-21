#
#	delfile.pl	1995.12.14	R.Nishimura
#
# 解説
#	削除ファイルリストに従って、ファイルの消去を行う。
#
# 使用方法
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