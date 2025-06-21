#
#	cod_sym.pl		1998.01.07	R.Nishimura
#
# ���
#	*.COD�t�@�C����ǂݍ���ŊO���V���{���𒊏o����B
#
#
# �g�p���@
#	b:>perl cod_sym.pl *.cod
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if(/^COMM\s+NEAR\s+(\w+):/) {
			write;
		} elsif(/^PUBLIC\s+(\w+)\s/) {
			write;
		} elsif(/^\s+PUBLIC\s+(\w+)\s/) {
			write;
		}
	}
	close(FILE);
}
format	STDOUT =
#123456789012345678901234567890123456789012
@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<<<<<<<
$1,$fname
.
#
#	end of file
#
