#
#	mkmfcv.pl	1994.03.10	R.Nishimura
#
# ���
#	mkmf �o�̓t�@�C����ϊ�����B
#
# �g�p���@
#	b:>perl mkmfcv.pl makefile
#
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/A-Z/a-z/;
	s/\/usr\/include\/\S+//g;
	s/\.o/\.obj/;
	print;
}
close(FILE);
#
