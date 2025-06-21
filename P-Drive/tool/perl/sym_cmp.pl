#
#	sym_cmp.pl	1998.01.06	R.Nishimura
#
# 解説
#	文字列ファイルを比較して、同じ文字列を表示する
#
# 使用方法
#	b:>perl sym_cmp.pl list1.sym list2.sym
#
#
@splist = ();
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	if(/(\S+)\s+/) {
#		print "\n$1";
		push(@splist, $1);
	}
}
close(FILE);
#
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
while(<FILE>) {
	if(/(\S+)\s+/) {
		@work = @splist;
		$exst = 0;
		foreach $wk (@work) {
			if($wk eq $1) {
				$exst = 1;
			}
		}
		if($exst == 1) {
			print ;
		}
	}
}
close(FILE);
#
