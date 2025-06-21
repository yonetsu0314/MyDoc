#
#	cod_sym.pl		1998.01.07	R.Nishimura
#
# 解説
#	*.CODファイルを読み込んで外部シンボルを抽出する。
#
#
# 使用方法
#	b:>perl cod_sym.pl *.cod
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if(/^COMM\s+NEAR\s+(\w+):/) {
			write;
		} elsif(/^PUBLIC\s+(\w+)\s/) {
			write;
		} elsif(/^\s+PUBLIC\s+(\w+)\s/) {
			write;
		}
	}
	close(FILE);
}
format	STDOUT =
#123456789012345678901234567890123456789012
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<<<<<<<
$1,$fname
.
#
#	end of file
#
