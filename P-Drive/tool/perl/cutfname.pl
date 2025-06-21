#
#	cutfname.pl		1995.12.14	R.Nishimura
#
# 解説
#	VAXのdir/outで出力された結果から、ファイル名のみを抽出する。
#
# 使用方法
#	b:>perl cutfname.pl vaxfile.lst > delfile.lst
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/(.+);/) {
			print "$1\n";
		}
	}
	close(FILE);
}
#
#	end of file
#
