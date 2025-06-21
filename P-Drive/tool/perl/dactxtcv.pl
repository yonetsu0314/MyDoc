#
#	dactxtcv.pl	1996.08.05	R.Nishimura
#
#  機能：
#	ＤＡＣＳ－Ｗの文字列ブックからｔｘｔ形式で出力された
#	ファイルを変換する
#
for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
	print	"; \$Header\$\n";
	while (<F>) {
		while(/\"\|/) {
			s/\"\|/\'/g;
		};
		while(/\|\"/) {
			s/\|\"/\'/g;
		};
		while(/\"\"/) {
			s/\"\"/\"/g;
		};
		while(/\|/) {
			s/\|/\'/g;
		};
		print;
	}
	close(F);
}
