#
#	cvcallf.pl	1994.03.10	R.Nishimura
#
# ���
#	CCW-RZ(-DZ) RCU �v���O������ �Ԑڌďo�𒼐ڌďo�ɕύX����
#
# �g�p���@
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