#
#	allco.pl	1993.10.19	R.Nishimura
#
# 解説
#	RCSのident出力ファイル(rxxxxx.id)を読み込んで
#	$Header$の展開結果を参照し、
#	構成しているファイルを全て リードオンリーで チェックアウトします。
#				   ~~~~~~~~~~~~~~~~
#		b:>perl allco.pl p:..\rxxxxx.id
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	print `co -r$2 $1` if /\$Header: (\S+) *(\S+)/;
}
#
#	end of file
#
