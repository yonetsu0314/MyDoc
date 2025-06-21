#
#	dftcut.pl		1996.04.17	R.Nishimura
#
# ‰ðà
#	‚b‚b‚v¬Œ^‚q‚b‚tƒ\ƒtƒg‚Ì‚c‚e‚sƒtƒ@ƒCƒ‹‚©‚ç•¶Žš—ñ•”•ª‚ð’Šo‚·‚é
#
# Žg—p•û–@
#	b:>perl dftcut.pl arc.txt
#
#
$cut = 0;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/^FORMAT/) {
			$cut = 1;
			print "-------------------------------------------\n";
			print "$fname\n";
			print "-------------------------------------------\n";
			print;
		} elsif (/^SCROLL/) {
			$cut = 1;
			print;
		} elsif ($cut == 1) {
			if(/^\./) {
				$cut = 0;
				print;
			} else {
				chop;
				if(/(.*)\/\//) {
					if($1 =~ /\S+/) {
						print "$1\n";
					}
				} else {
					if(/\S+/) {
						print "$_\n";
					}
				}
			}
		}
	}
	close(FILE);
}
#
#	end of file
#
