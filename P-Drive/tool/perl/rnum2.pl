#
#	rnum2.pl	1994.09.02	R.Nishimura
#
# ���
#	r_number �t�@�C����ǂݍ���ňꗗ�\���쐬����B
#
# �g�p���@
#	b:>perl rnum2.pl r_number
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
		$name =~ /(\w+)$/;
		$na = $1; 
		write;
	} else {
		$name = $_;
	}
}
close(FILE);
#
#
format	STDOUT_TOP = 
-------------------------------------------------------------------------------
No.    machine             unit          ver    name      spec
-------------------------------------------------------------------------------
.
format	STDOUT =
#r0000  ccw-rz-216b-2m/30-wp  rcu-main(eng)   1.00   spec
@<<<< @<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<@##.## @<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<
$no,$ma,$un,$ve,$na,$sp
.
#
#	end of file
#
