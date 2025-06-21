#
#	genmstbl.pl		1995.11.08	R.Nishimura
#
# 解説
#	DACS文字列定義ソースファイルを読み込んで文字列テーブルファイルを
#	作成するものである。
#	cutmess.pl で抽出したものと比較する場合には、
#		print "'--------------------------------'\n";
#	この行をコメントにする。
#
# 使用方法
#	b:>perl genmstbl.pl jpn*.src
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/(.+):.*'(.+)'.*/) {
			$mes = "|$2|";
			$com = $1;
			write;
		} elsif (/.*'(.+)'>,(\d\d\d\S+)/) {
			print "\"|--------------------------------|\"\n";
			$mes = "|$1|";
			$com = $2;
			write;
		} elsif (/.*'(.+)'>,(\S+)/) {
			$mes = "|$1|";
			$com = $2;
			write;
		} elsif (/'(.+)'/) {
			$mes = "|$1|";
			$com = "";
			write;
		}
	}
	close(FILE);
}
#
format	STDOUT =
#123456789012345678901234567890123456789012
"@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<","@<<<<<<<<<<<<<<<<<<<<<<<<<"
$mes,$com
.
#
#	end of file
#
