#	PROTO.PL	1993.6.3	M.Fukutome
#	C プログラムソースファイルを読み取り、
#	関数プロトタイプ宣言ファイルを作成する


for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
	print "\n/*\t$fname\t*/\n";	# 読み込みファイル名

	$n = 0; $find = 0; $rem = 0;

	while (<F>) {
		chop;
# コメントの削除
		if ($rem != 0) {
			$ecom = index($_, '*/');
			next if ($ecom < 0);
			substr($_, 0, $ecom+2) = '';
			$rem = 0;
		}
		while (($bcom = index($_, '/*')) >= 0) {
			$ecom = index($_, '*/');
			if ($ecom < 0) {
				substr($_, $bcom) = '';
				$rem = 1;
			} else {
				substr($_, $bcom, $ecom-$bcom+2) = '';
			}
		}
		next if (/^[ \t]*$/);	# 空白行は読み飛ばす。
###############################################################################
#	関数名、引数、宣言子等の読み込み
		if (/^[a-zA-Z_]/) {
			$line[$n++] = $_;
			$find = 1;
		} elsif ($find > 0 && /^[ \t]*[a-zA-Z_\*\)\.]+/) {
			$line[$n++] = $_;
		} elsif ($find && /^{/) {
#$deb = 1;
# 関数に関する情報の読み込みの終了／プロトタイプの生成
			$_ = join(' ', @line);
#print "$deb::::$_\n"; ++$deb;
	# ";"/"}" までを削除
			s/.*;|.*\}//g;
#print "$deb::::$_\n"; ++$deb;
	# 行頭／行末の空白を削除
			s/^[ \t]+//;
			s/[ \t]+$//;
#print "$deb::::$_\n"; ++$deb;
	# タブとスペースの連続を１個のスペースに変換
			s/[ \t]+/ /g;
#print "$deb::::$_\n"; ++$deb;
	# '(' 左の空白の削除
			s/ \(/\(/;
#print "$deb::::$_\n"; ++$deb;
	# '(' 右の空白の削除
			s/\( /\(/g;
#print "$deb::::$_\n"; ++$deb;
	# ')' 両側の空白の削除
			s/[ ]*\)[ ]*/\)/g;
#print "$deb::::$_\n"; ++$deb;
	# ',' 左の空白を削除し, 右に１個の空白を挿入
			s/[ ]*,[ ]*/, /g;
#print "$deb::::$_\n"; ++$deb;
	# '()' の場合　void を追加
			s/\([ ]*\)/\(void\)/g;
#print "$deb::::$_\n"; ++$deb;
	# 関数名しかない場合は、void を追加
			s/^([^ ]+\()/void $1/;
#print "$deb::::$_\n"; ++$deb;

			print "$_;\n" if (length($_) > 3);

			$find = 0;
			undef @line;
			$n = 0;
		} else {
			$find = 0;
			undef @line;
			$n = 0;
		}
###############################################################################
	}
	close(F);
}
