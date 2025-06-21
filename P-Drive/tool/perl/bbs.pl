while(<>){
	s/\s+//g;
	s/,/+/g;
	s/[|]/,/g;
	if (/[=]{2,}/) {
		print "\n";
	} elsif (/[-]{2,}/) {
	} else {
		print "$_\n";
	}
}
