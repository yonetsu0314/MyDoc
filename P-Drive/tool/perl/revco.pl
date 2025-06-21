#
#	revco.pl	1993.12.18	R.Nishimura
#
# 解説
#	RCSの ident 出力ファイルを読み込んで 
#	$Log$ の展開結果を参照し、
#	構成ファイルを全て リードオンリーで チェックアウトする。
#		           ~~~~~~~~~~~~~~~~
# 使用方法
#	1.RCSファイルの有る場所を指定
#	  環境変数の設定
#		b:>set RCS=p:\wcu\main\rz_wcu\rcs\
#	  またはカレントディレクトリーの設定
#		b:>p:
#		p:>cd \wcu\main\rz_wcu\rcs
#		p:>b:
#	2.チェックアウト先の設定と実行
#		b:>cd \work
#		b:>perl revco.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		if( index($rcs, "p:/") >= $[ ) {
			print `co -r$rev $rcs`;
		} elsif( index($rcs, "p:\\") >= $[ ) {
			print `co -r$rev $rcs`;
		} else {
			@fname = split(/\W/, $rcs);
			if($ENV{'RCS'} ne '') {
				print `co -r$rev $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co -r$rev p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
#	end of file
#
