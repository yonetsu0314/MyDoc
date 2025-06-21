#
#	ulockci.pl	1994.03.16	R.Nishimura
#
# ‰ðà
#
# Žg—p•û–@
#	b:>perl ulockci.pl abc.c
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
		}
		if (/\$Log: (\S+) */) {
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev ne '') {
		@rel = split(/\./, $rev);
		$rel[$#rel]++;
		$rel = join('.', @rel );
		print `ci -r$rel -u $fname $rcs`;
	}
}
#
#	end of file
#
