#
#	cvcallf2.pl	1993.12.09	R.Nishimura
#
# ���
#	
# �g�p���@
#	b:>perl cvcallf2.pl *.c
#
foreach $fname (@ARGV) {
	print "perl p:\\tool\\perl\\cvcallf.pl $fname > tmp\n";
	print "copy tmp $fname\n";
}
#
#	end of file
#
