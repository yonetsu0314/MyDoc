#
#	cutfname.pl		1995.12.14	R.Nishimura
#
# ���
#	VAX��dir/out�ŏo�͂��ꂽ���ʂ���A�t�@�C�����݂̂𒊏o����B
#
# �g�p���@
#	b:>perl cutfname.pl vaxfile.lst > delfile.lst
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/(.+);/) {
			print "$1\n";
		}
	}
	close(FILE);
}
#
#	end of file
#
