#
#	nrvnwco6.pl	1999.11.10	R.Nishimura
#	Original:revco.pl2	1994.07.15	R.Nishimura
#
# 解説
#   $Header$ 展開情報のみを使用する。したがって Freeware 版の RCS によって
#   得られた構成ファイルは使用できない。
#	$Header$の展開結果を参照し、構成ファイル中のリビジョン番号の
#	末尾に関して最新のリビジョンを全てリードオンリーでチェックアウトする。
#	                                  ~~~~~~~~~~~~~~~~
#	このスクリプトでは、MKS-RCS 6.2 を使用することを前提としている。
#                              ~~~~~
#
#	構成管理ファイルが以下のようになっている場合
#		els_eng/test.si:
#			$Header: p:/mks6test/rcs/test.si 1.2 2000/11/09 20:32:10 ryoji Exp $
#			$Log: test.si $
#
#	MKS-RCS 5.3 用の rvnwco.bat(p:/tool/perl/rvnwco.pl2) では、
#
#			> co -O p:/mks6test/rcs/test.si
#				p:/mks6test/rcs/test/si ---> test.si
#
#	MKS-RCS 6.2 用の nrvnwco6.bat(p:/tool/perl/nrvnwco6.pl) では、
#
#			> co -g -O p:/mks6test/rcs/test.si els_eng/test.si
#				p:/mks6test/rcs/test/si ---> els_eng/test.si
#
# 使用方法
#	1.チェックアウト先の設定と実行
#		b:>cd \work
#		b:>perl nrvnwco6.pl r0003-1.2
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
		$outf = $wf;
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if( $rev ne '' ) {
		$rev =~ s/\.[0-9]+$//;
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		if( index($h_rcs, "p:/") >= $[ ) {
			print "\nco.exe -r$rev -g -O $h_rcs $l_rcs";
		} elsif( index($h_rcs, "p:\\") >= $[ ) {
			print "\nco.exe -r$rev -g -O $h_rcs $l_rcs";
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
	}
}
close(FILE);
#
#	end of file
#
