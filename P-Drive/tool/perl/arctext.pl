#
#	arctext.pl		1995.11.08	R.Nishimura
#
# ‰ðà
#	‚`‚q‚b‚r‚b‚n‚o‚d‚Å‚g‚d‚wŒ`Ž®‚Å‚ÌˆóŽšƒCƒ[ƒW‚ðo—Í‚µ‚½ƒtƒ@ƒCƒ‹‚ð
#	‚v‚‰‚Ž‚„‚‚—‚“”Å‚Ì‚l‚‰‚†‚…‚“‚È‚Ç‚ÅO‚l‚ðíœ‚µ‚½ƒtƒ@ƒCƒ‹‚ð“Ç‚Ýž‚ñ‚Å
#	ƒeƒLƒXƒg•”•ª‚ð’Šo‚·‚éB
#
# Žg—p•û–@
#	b:>perl arctext.pl arc.txt
#
#
$first = 1;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/^\d/) {
			if($first) {
				print "\n";
				$first = 0;
			}
			@str = split(/\s/);
			print $str[$#str];
		} else {
			$first = 1;
		}
	}
	close(FILE);
}
#
#	end of file
#
