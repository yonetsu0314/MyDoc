#
#	checkin7.pl	2017.04.26	R.Nishimura
#	ベース：checkin2.pl	2002.03.25	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにあるファイルから
#	チェックイン対象ファイル（書き込み可能属性）のみのチェックイン
#	コマンド行を -r -u オプション付きで 生成する。
#		     ~~~~~~~~~~~~~~~~~~~~~~
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkin7.pl *.* > a.bat		バッチファイル作成
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
		print "ci -u $fname\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $fname\n";
			print "co $fname\n";
		}
	} else {
		print "rcs -l$rev $rcs\n";
		@rel = split(/\./, $rev);
		$rel[$#rel]++;
		$rel = join('.', @rel );
		print "ci -u$rel $rcs < log.txt\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $rcs\n";
			print "co $rcs\n";
		}
	}
}
#
#	end of file
#
