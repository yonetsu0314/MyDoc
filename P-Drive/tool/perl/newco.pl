#
#	newco.pl	1993.12.18	R.Nishimura
#
# 解説
#	RCSの ident 出力ファイルを読み込んで 
#	$Log$ の展開結果を参照し、
#	構成ファイルの最新を全て リードオンリーで チェックアウトする。
#		      ~~~~~~     ~~~~~~~~~~~~~~~~
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
#		b:>perl newco.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/A-Z/a-z/;
	if (/\$log: (\S+) */) {
		if( index($1,"p:/") >= $[ ) {
			print `co $1`;
		} elsif( index($1,"p:\\") >= $[ ) {
			print `co $1`;
		} else {
			@fname = split(/\W/, $1);
			if($ENV{'RCS'} ne '') {
				print `co $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
	}
}
close(FILE);
#
#	end of file
#
