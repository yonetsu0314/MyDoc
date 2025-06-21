#
#	bnkhexcv.pl	1994.08.18	R.Nishimura
#
# 解説
#	bnkhex 出力ファイルを変換する。
#
# 使用方法
#	b:>perl bnkhexcv.pl hexfile
#
#
@exaddr = ("0000FC","0400F8","0800F4","0C00F0","1000EC","1400E8","1800E4","1C00E0","2000DC","2400D8","2800D4","2C00D0","3000CC","3400C8","3800C4","3C00C0","4000BC");
$a = 0;
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	if(/:\S\S800000/) {
		print ":02000002@exaddr[$a]\n";
		$a=$a+1;
	}
	print;
}
close(FILE);
#
