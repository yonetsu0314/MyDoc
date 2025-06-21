#
#	arctext2.pl		1995.11.08	R.Nishimura
#
# 解説
#	ａｒｃｔｅｘｔ．ｐｌで抽出した結果から特定のテキスト（部分）を抽出する
#
# 使用方法
#	b:>perl arctext2.pl arc.txt
#
#
$l = 0;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/^PZﾊ....WCU0RCU0\\0000.{5}(.{84})/) {
			if($l==0) {
				print "\n---------------";
				$l = 1;
			} else {
				$l = 0;
			}
			$n = index($1,"08",0);
			if($n >= 0) {
				$m = ($n + 2) / 6;
				$cnt[$m]++;
				print "\n";
				for($i=1; $i<=14; $i++) {
					if($i == $m) {
						print '*';
					} else {
						print '0';
					}
				}
				print ":$m";
			} else {
				print "\n00000000000000";
			}
		}
	}
	print "\n";
	for($i=1; $i<=14; $i++) {
		print "$cnt[$i] ";
	}
	close(FILE);
}
#
#	end of file
#
