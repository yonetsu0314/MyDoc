#
#	rcscv2.pl	1994.07.04	R.Nishimura
#
# ‰ðà
#	‚l‚j‚r”Å‚q‚b‚r‚ÌRCSƒtƒ@ƒCƒ‹‚Å branches s‚Ìƒ\[ƒgs‚¤B
#
# Žg—p•û–@
#	b:>perl rcscv2.pl dsd001.c > dsd001.new
#	b:>fc dsd001.c dsd001.new
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/branches\s+([^;]+)/) {
			@bra2 = '';
			@bra3 = '';
			@bra = split(' ',$1);
			while($f = shift(@bra)) {
				@f = split('\.',$f);
				if($f[2] > 99) {
					unshift(@bra2,$f);
				} else {
					unshift(@bra3,$f);
				}
			}
			@bra2 = sort(@bra2);
			@bra3 = sort(@bra3);
			@bra4 = join(' ',@bra2);
			@bra5 = join(' ',@bra3);
			print "branches@bra5@bra4;\n";
		} else {
			print;
		}
	}
	close(FILE);
}
#
#	end of file
#
