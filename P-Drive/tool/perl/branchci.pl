#
#	branch.pl	1995.09.27	T.Ohtani
#
#	original : checkin.pl	1994.06.15	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにあるファイルから
#	チェックイン対象ファイル（書き込み可能属性）のみのチェックイン
#	コマンド行を -r -u -m オプション付きで 生成する。
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkin.pl *.* > a.bat		バッチファイル作成
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	if($ENV{'ALL'} eq '') {
		next unless (-w $fname);
	}
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
		print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $fname\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $fname\n";
			print "co $fname $fname\n";
		}
	} else {
		@rel = split(/\./, $rev);
		if(($ENV{'HOLD'} ne '') && ($ENV{'BRANCH'} ne '')) {
			$rel[$#rel]++;
		}
		$rel = join('.', @rel );
		if($ENV{'BRANCH'} ne '') {
			$rel = "$rel.$ENV{'BRANCH'}";
		}
		print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $rcs\n";
		if($fname[1] eq "mk") {
			print "rcs -c\"#* \" $rcs\n";
			print "co $fname $rcs\n";
		}
	}
}
#
#	end of file
#
