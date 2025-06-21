#
#	txtcut.pl		1996.04.17	R.Nishimura
#
# ‰ðà
#	‚b‚b‚v¬Œ^‚q‚b‚tƒ\ƒtƒg‚Ì‚s‚w‚sƒtƒ@ƒCƒ‹‚©‚ç•¶Žš—ñ•”•ª‚ð’Šo‚·‚é
#
# Žg—p•û–@
#	b:>perl txtcut.pl arc.txt
#
#
$cut = 0;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	print "-------------------------------------------\n";
	print "$fname\n";
	print "-------------------------------------------\n";
	while(<FILE>) {
		if (/^STRING[^S]/) {
			$cut = 0;
			print;
		} elsif (/^STRINGS/) {
			$cut = 1;
			print;
		} elsif ($cut == 1) {
			print;
			if(/^ENDS/) {
				$cut = 0;
			}
		}
	}
	close(FILE);
}
#
#	end of file
#
