#
#	arctext.pl		1995.11.08	R.Nishimura
#
# 解説
#	ＡＲＣＳＣＯＰＥでＨＥＸ形式での印字イメージを出力したファイルを
#	Ｗｉｎｄｏｗｓ版のＭｉｆｅｓなどで＾Ｍを削除したファイルを読み込んで
#	テキスト部分を抽出する。
#
# 使用方法
#	b:>perl arctext.pl arc.txt
#
#
$first = 1;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/^\d/) {
			if($first) {
				print "\n";
				$first = 0;
			}
			@str = split(/\s/);
			print $str[$#str];
		} else {
			$first = 1;
		}
	}
	close(FILE);
}
#
#	end of file
#
