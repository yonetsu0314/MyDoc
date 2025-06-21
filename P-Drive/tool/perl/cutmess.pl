#
#	cutmess.pl		1995.11.08	R.Nishimura
#
# 解説
#	DACS文字列定義ソースファイルを読み込んで文字列定義行を抽出する。
#	これは、文字列テーブルファイルを作成する genmstbl.pl で抽出抜け
#	がないことを確認する為のものである。
#
#
# 使用方法
#	b:>perl cutmess.pl jpn*.src
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/'.+'/) {
			print;
		}
	}
	close(FILE);
}
#
#	end of file
#
