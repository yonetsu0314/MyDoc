#
#	mkmfpp.pl	1994.03.10	R.Nishimura
#
# 解説
#	mkmf への入力ファイルに変換する。
#
# 使用方法
#	b:>perl mkmfpp.pl *.*
#
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/A-Z/a-z/;
	s/(#include.+)/$1 \/\*\*\//g;
	print;
}
close(FILE);
#
