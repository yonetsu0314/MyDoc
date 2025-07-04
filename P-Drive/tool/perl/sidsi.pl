#
#	looksi.pl	1994.03.28	R.Nishimura
#
# 解説
#
# 使用方法
#	b:>perl looksi.pl abc.si
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
#	next unless (-w $fname);
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
		$rcs =~ tr/A-Z/a-z/;
		$rcs =~ s/\.siv/\.sid/;
		$rcs =~ s/rcs/rev_100/;
		if(-e $rcs) {
			print "sid $rcs";
		} else {
			$rcs =~ s/rev_100/rev_1/;
			if(-e $rcs) {
				print "sid $rcs";
			} else {
				$rcs =~ s/rev_1/rev_2/;
				if(-e $rcs) {
					print "sid $rcs";
				} else {
					$rcs =~ s/rev_2/rev_101/;
					if(-e $rcs) {
						print "sid $rcs";
					} else {
						print "Cannot open $rcs";
					}
				}
			}
		}
	}
}
#
#	end of file
#
