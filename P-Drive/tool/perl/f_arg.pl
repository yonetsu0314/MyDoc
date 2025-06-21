#
#	f_arg.pl	1994.03.24	R.Nishimura
#
# 解説
#	/AL /Fc /Od /Gs /G2 オプションでコンパイルして生成された
#	CODファイルを読み込んで、各関数の引数バイトサイズを調べる
#
# 使用方法
#	b:>perl f_arg.pl *.cod > f_arg
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/\s+call\s+FAR\s+PTR\s+(\w+)\s+.*/) {
			$_ = <FILE>;
			$func = $1;
			if(/\s+add\s+sp\,(\d+)\D+/) {
				@work = @splist;
				$exst = 0;
				foreach $wk (@work) {
					if($wk eq $func) {
						$exst = 1;
					}
				}
				if($exst == 0) {
					push(@splist, $func);
					printf "\n$func,$1";
				}
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