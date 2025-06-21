#
#	setlabel.pl	1995.10.03	T.Ohtani
#
#	original : checkin.pl	1994.06.15	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにあるファイルから
#
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkin.pl *.* > a.bat		バッチファイル作成
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	@fname = split(/\./, $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev eq '') {
		$rel = "1.1";
		print "ci -r$rel -nLABEL $fname\n";
	} else {
		@rel = split(/\./, $rev);
		$#rel--;
		$rel = join('.', @rel );
		print "rcs -nLABEL:$rel $rcs\n";
	}
}
#
#	end of file
#
