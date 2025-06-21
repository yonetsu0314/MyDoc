#
#	chg_id.pl	1993.07.12	R.Nishimura
#
#  機能：
#	大文字を小文字に変換する、
#
#　ルール１：	[A-Z] ---> [a-z]
#
#　注意：
#	 コメント内や表示文字列も変換される。
#
for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
#	print "\n/*\t$fname\t*/\n";	# 読み込みファイル名
	while (<F>) {
		while(/[A-Z]/) {
			s/([A-Z])/\l$1/g;
		};
		print;
	}
	close(F);
}
