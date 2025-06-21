#
#	newco2.pl	1994.04.14	R.Nishimura
#
# 解説
#	RCSの ident 出力ファイルを読み込んで 
#	$Log$ の展開結果を参照し、
#	構成ファイルの最新を全て リードオンリーで チェックアウトする。
#		      ~~~~~~     ~~~~~~~~~~~~~~~~
#	newco.pl との違いは、ｃｏ時のオプションが設定できる機能が追加されている
#	例えば、ステータスがＲｅｌである最新レビジョンをチェックアウト
#	したい場合には以下のように使用すれば良い。
#
#	b:>newco r0036-1.14 -sRel
#
#	注意：オプションは、最後に記述して下さい
#
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
#		b:>perl newco2.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/A-Z/a-z/;
	if (/\$log: (\S+) */) {
		if( index($1,"p:/") >= $[ ) {
			print `co $ARGV[1] $1`;
		} elsif( index($1,"p:\\") >= $[ ) {
			print `co $ARGV[1] $1`;
		} else {
			@fname = split(/\W/, $1);
			if($ENV{'RCS'} ne '') {
				print `co $ARGV[1] $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co $ARGV[1] p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
	}
}
close(FILE);
#
#	end of file
#
