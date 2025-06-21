#
#	checkin2.pl	2002.03.25	R.Nishimura
#	ベース：checkin.pl	1994.06.15	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにあるファイルから
#	チェックイン対象ファイル（書き込み可能属性）のみのチェックイン
#	コマンド行を -r -u オプション付きで 生成する。
#		     ~~~~~~~~~~~~~~~~~~~~~~
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkin2.pl *.* > a.bat		バッチファイル作成
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
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
		print "ci -r$rel -u $fname $fname\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $fname\n";
			print "co $fname $fname\n";
		}
	} else {
		@rel = split(/\./, $rev);
		$rel[$#rel]++;
		$rel = join('.', @rel );
		print "ci -h -r$rel -u $fname $rcs < log.txt\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $rcs\n";
			print "co $fname $rcs\n";
		}
	}
}
#
#	end of file
#
