#
#	allkcode.pl	1995.01.17	R.Nishimura
#
# 解説
#	
# 使用方法
#	b:>perl allkcode.pl *.*
#
foreach $fname (@ARGV) {
	print `kcode $fname allkcode.txt /I:NEC /O:DEC`;
	print `copy allkcode.txt $fname`;
}
#
#	end of file
#
