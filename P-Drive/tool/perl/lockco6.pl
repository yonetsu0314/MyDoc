#
#	lockco6.pl	1999.11.10	R.Nishimura
#	Original: lockco.pl2	1994.06.15	R.Nishimura
#
# ‰ðà
#
# Žg—p•û–@
#	b:>perl lockco6.pl abc.c
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev ne '') {
		print "\nco.exe -g -r$rev -l -O $rcs $fname";
	}
}
#
#	end of file
#
