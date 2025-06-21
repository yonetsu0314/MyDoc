#
#	cvcallf.pl	1994.03.10	R.Nishimura
#
# 解説
#	CCW-RZ(-DZ) RCU プログラムの 間接呼出を直接呼出に変更する
#
# 使用方法
#	b:>perl cvcallf.pl *.*
#
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	s/pfusrprg\s*=\s*(\w+)\s*;/$1\(\);/g;
	print;
}
close(FILE);
#
