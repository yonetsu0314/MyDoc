$cname = "[_a-zA-Z][_a-zA-Z0-9]+";

$ofile = "txt.tmp";
open(OFP, ">$ofile") || die "Can't open file ($ofile)\n";

while (<>) {
	if(/($cname)\[\d*\],\" (.*)\"/) {
		if ($var eq $1) {
			print OFP "\tDEFM(\"$2\")\n";
		} else {
			if ($var) {
				print "STRINGS($var)\n";
				close(OFP);
				open(OFP, "<$ofile") || die "Can't open file ($ofile)\n";
				while (<OFP>) {
					print $_;
				}
				close(OFP);
				print "ENDS\n";
			}
			$var = $1;
			open(OFP, ">$ofile") || die "Can't open file ($ofile)\n";
			print OFP "\tDEFM(\"$2\")\n";
		}
#	} elsif(/([_A-Z0-9]+\d*),\" (.*)\"/) {
	} elsif(/($cname)#,\" (.*)\"/) {
		if ($var) {
			print "STRINGS($var)\n";
			close(OFP);
			open(OFP, "<$ofile") || die "Can't open file ($ofile)\n";
			while (<OFP>) {
				print;
			}
			print "ENDS\n";
			close(OFP);
			undef	$var;
		}
		print "#define\t\t$1\t\"$2\"\n";
	} elsif(/($cname),\" (.*)\"/) {
		if ($var) {
			print "STRINGS($var)\n";
			close(OFP);
			open(OFP, "<$ofile") || die "Can't open file ($ofile)\n";
			while (<OFP>) {
				print;
			}
			print "ENDS\n";
			close(OFP);
			undef	$var;
		}
		print "STRING($1, \"$2\")\n";
	}
}

if ($var) {
	print "STRINGS($var)\n";
	while (<OFP>) {
		print;
	}
	print "ENDS\n";
}
close(OFP);
