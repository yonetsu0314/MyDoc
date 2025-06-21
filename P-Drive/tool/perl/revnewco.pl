#!perl
#	revnewco.pl	1995.10.24	M.Fukutome
#	orginal :revco.pl2	1994.07.15 R.Nishimura/1995.10.20 K.Nakaoka
#
# 解説
#	新旧混在のRCS ident 出力ファイルを読み込んで 
#	$Header$,$Log$ の展開結果を参照し、構成ファイル中のリビジョン番号の
#	末尾に関して最新のリビジョンを全てリードオンリーでチェックアウトする。
#	                                  ~~~~~~~~~~~~~~~~
#	MKS-RCSを使用することを前提としている。
#
# 使用方法
#	1.チェックアウト先の設定と実行
#		b:>cd \work
#		b:>perl revnewco.pl r0003_1.2
# 改訂履歴
# M.Fukutome 1997.05.13
#   $Header$ 展開情報のみを使用する。したがって Freeware 版の RCS によって
#   得られた構成ファイルは使用できない。
#
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if ($rev ne '') {
		$rev =~ s/\.[0-9]+$//;
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		if( index($h_rcs, "p:/") >= $[ ) {
			print "\nco.exe -r$rev -O $h_rcs";
		} elsif( index($h_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -r$rev -O $h_rcs";
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
	}
}
close(FILE);
__EOF__
