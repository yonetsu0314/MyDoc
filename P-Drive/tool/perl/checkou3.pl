#
#	checkou3.pl	1993.12.10	R.Nishimura
#
# 解説
#	RCSの ident 出力ファイルを読み込んで $Header$ の展開結果を参照し、
#	環境変数 RCS で示されたディレクトリから
#	構成ファイルを全て リードオンリーで チェックアウトする。
#		           ~~~~~~~~~~~~~~~~
# 使用方法
#	b:>set RCS=p:\wcu\main\rz_wcu\rcs\	管理ファイル保管場所指定
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl checkou3.pl r0003_12		ＩＤファイル名指定(拡張子不要)
#
$id = $ENV{'RCS'}."..\\".$ARGV[0].".id";
open(FILE, $id) || die "Cannot open $id: $!" ;
while(<FILE>) {
	if (/\$Header: (\S+) *(\S+)/) {
		@fname = split(/\W/, $1);
		print `co -r$2 $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
	};
};
close(FILE);
#
#	end of file
#
