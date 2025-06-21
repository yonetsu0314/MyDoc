#
#	chkci.pl	1993.11.01	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにすべてのソースコードファイルを集め
#	下記のコマンドを実行することによって
#	チェックイン対象ファイル（書き込み可能属性）を表示する
#
#		b:>perl chkci.pl *.*
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
	print	$fname,"\n";
}
#
#	end of file
#
