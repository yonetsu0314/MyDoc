#
#	f_auto.pl	1994.03.24	R.Nishimura
#
# 解説
#	/AL /Fc /Od /Gs /G2 オプションでコンパイルして生成された
#	CODファイルを読み込んで、各関数の動的変数バイトサイズを調べる
#
# 使用方法
#	b:>perl f_auto.pl *.cod > f_auto
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/^(\w+)\s+PROC\s+.*/) {
			print "\n$1,$fname";
		}
		if(/\s+enter\s+(\d+)\D+/) {
			print ",$1";
		}
	}
	close(FILE);
}
print "\n";
#
#	end of file
#
