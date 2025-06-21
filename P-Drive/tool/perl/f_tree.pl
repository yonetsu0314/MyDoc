#
#	f_tree.pl	1994.03.24	R.Nishimura
#
# 解説
#	/AL /Fc /Od /Gs /G2 オプションでコンパイルして生成された
#	CODファイルを読み込んで、各関数が呼び出している関数名を調べる
#
# 使用方法
#	b:>perl f_tree.pl *.cod | sort > f_tree
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/^(\w+)\s+PROC\s+.*/) {
			print "\n$1";
			@splist = ();
		}
		if(/\s+call\s+FAR\s+PTR\s+(\w+)\s+.*/) {
			@work = @splist;
			$exst = 0;
			foreach $wk (@work) {
				if($wk eq $1) {
					$exst = 1;
				}
			}
			if($exst == 0) {
				push(@splist, $1);
				printf ",$1";
			}
		}
	}
	close(FILE);
}
print "\n";
#
#	end of file
#
