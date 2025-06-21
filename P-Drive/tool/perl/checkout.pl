#
#	checkout.pl	1993.12.13	R.Nishimura
#
# 解説
#	環境変数 RCS で示されたディレクトリの親ディレクトリにある
#	RCSの ident 出力ファイルを読み込んで 
#	$Header$ と $Log の展開結果を参照し、
#	構成ファイルを全て リードオンリーで チェックアウトする。
#		           ~~~~~~~~~~~~~~~~
# 使用方法
#	b:>set RCS=p:\wcu\main\rz_wcu\rcs\	管理ファイル保管場所指定
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkout.pl r0003_1.2		ＩＤファイル名指定
#
$id = $ENV{'RCS'}."..\\".$ARGV[0];
open(FILE, $id) || die "Cannot open $id: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	$rev = $2 if (/\$Header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$Log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		print `co -r$rev $rcs`;
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
#	end of file
#
