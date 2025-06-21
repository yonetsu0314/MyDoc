#
#	mkmfpp2.pl	1993.12.09	R.Nishimura
#
# ‰ðà
#	
# Žg—p•û–@
#	b:>perl mkmfpp2.pl *.c
#
foreach $fname (@ARGV) {
	print "perl mkmfpp.pl $fname > tmp\n";
	print "copy tmp $fname\n";
}
#
#	end of file
#
