# txsplit.pl
# C ソースファイルを分割する。

#   perl txsplit.pl filename
#   filename : 分割するファイル名
#   ファイル中の #pragma の記述されている行でファイルを分割する。出力ファイル
#   名は入力ファイルと #pragma 行に記述されている名前で指定する。
#       出力ファイル名の生成規則
#         入力ファイル名                   xxx_yyy.zzz
#         pragma 行で指定されている名前    aaa
#         とすると、出力ファイル名は       aaa_yyy.HH
#       pragam 行
#         #pragma xxxxx "name"
#   注意点
#   1) 最初の pragma 行までは出力されません。
#   2) #define, #pragma 以外のプリプロセッサ行は出力されません。
#   3) コメントは考慮されていません。

$suf = $ARGV[0];
$suf =~ s/\..*$//;
$suf =~ /(_\w*)$/;
$suf = $1;

$o_f = 0;
while (<>) {
	if (/^#(\w+)/) {
		if ($1 eq 'pragma') {
			/"(\w+)"/;
			$o_f == 1 && close(OFP);
			$oname = "$1$suf.hh";
			open(OFP, ">$oname")
				|| die "Can't open file ($oname)\n";
			$o_f = 1;
			print OFP;
			next;
		} elsif ($1 ne "define") {
			next;
		}
	}
	print OFP if $o_f == 1;
}

$o_f == 1 && close(OFP);
