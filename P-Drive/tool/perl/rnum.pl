#
#	rnum.pl		1994.03.09	R.Nishimura
#
# 解説
#	r_number ファイルを読み込んで一覧表を作成する。
#
# 使用方法
#	b:>perl rnum.pl r_number
#
#
$c = 0;
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/a-z/A-Z/;
	if (/^# ([^:]+):([^:]+):([^:]+):([^:]+):(.*)/) {
		$no = $1;
		$ma = $2;
		$un = $3;
		$ve = $4;
		$sp = $5;
		$ve =~ s/[^0-9]+//;
		write;
	}
}
close(FILE);
#
#
format	STDOUT_TOP = 
-------------------------------------------------------------------------------
No.    machine             unit          ver    spec
-------------------------------------------------------------------------------
.
format	STDOUT =
#r0000  ccw-rz-216b-2m/30-wp  rcu-main(eng)   1.00   spec
@<<<< @<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<@##.## @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
$no,$ma,$un,$ve,$sp
.
#
#	end of file
#
