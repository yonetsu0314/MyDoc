#
#	allkcode.pl	1995.01.17	R.Nishimura
#
# ���
#	
# �g�p���@
#	b:>perl allkcode.pl *.*
#
foreach $fname (@ARGV) {
	print `kcode $fname allkcode.txt /I:DEC /O:NEC`;
	print `copy allkcode.txt $fname`;
}
#
#	end of file
#
