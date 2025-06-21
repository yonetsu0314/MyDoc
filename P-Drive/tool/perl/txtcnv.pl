#
#	txtcnv.pl		1996.04.22	R.Nishimura
#
# ‰ðà
#	‚c‚`‚b‚r|‚v‚Ì‚s‚w‚sƒtƒ@ƒCƒ‹‚©‚ç•¶Žš—ñ•”•ª‚ð’Šo‚·‚é
#
# Žg—p•û–@
#	b:>perl txtcnv.pl arc.txt
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		chop;
		if (/^;/) {
			print "\n$_";
		} elsif (/\s+'/) {
			print "\n$_";
		} elsif (/\s+\[/) {
			print;
		} else {
			print "\n$_";
		}
	}
	close(FILE);
}
#
#	end of file
#
