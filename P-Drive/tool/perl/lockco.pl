#
#	lockco.pl	1993.12.09	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにあるファイルから
#	チェックイン対象ファイル（書き込み可能属性）のみのチェックイン
#	コマンド行を -r -u -m オプション付きで 生成する。
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# 使用方法
#	b:>perl lockco.pl abc.c
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
#	next unless (-w $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
		}
		if (/\$Log: (\S+) */) {
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev ne '') {
		print `co -r$rev -l $fname $rcs`;
	}
}
#
#	end of file
#
