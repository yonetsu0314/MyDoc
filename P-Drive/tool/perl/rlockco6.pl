#
#	ulockco6.pl	1999.11.10	R.Nishimura
#	Original:ulockco.pl2	1994.06.15	R.Nishimura
#
# ‰ðà
#
# Žg—p•û–@
#	b:>perl ulockco6.pl abc.c
#
$fname=@ARGV[0];
$rev1=@ARGV[1];
	-e $fname || die "Cannot open $fname: $!";
#	next unless (-w $fname);
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
		print "\nco.exe -g -O -r$rev1 $rcs $fname";
	}
#
#	end of file
#
