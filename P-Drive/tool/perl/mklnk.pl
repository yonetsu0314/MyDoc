# NKLNK.PL						m.fukutome 1994.5.9
# Rxxxx.H を読み込みリンクファイル（Rxxxx.LNK）を生成する

# perl mklnk.pl rXXXX.mk

while(<>) {
	s/#.*$//;		# コメントを削除
	if (/\$/) {		# マクロを展開する
		while (/\$\(([A-Za-z_0-9]+)\)/) {
			$np = $mlist{$1};
			$op = "\\\$\\\($1\\\)";
			$_ =~ s/$op/$np/g;
		}
	}
	if (/=/) {		# マクロ定義を読み込む
		chop;
		s/[ \t]+$//;
		/^[ \t]*([A-Za-z_0-9]+)[ \t]*=[ \t]*(.*)$/;
		if ($1 ne 'OBJ') {
			delete $mlist{$1};
			$mlist{$1} = $2;
		}
	}
	if (/echo.*>/) {	# リンクファイルを作成する
		s/[ \t]*echo[ \t]+//;
		s/[ \t]+$//;
		s/>>/>/;
		s/>[ \t]+/>/;
		s/[ \t]+>/>/;
		/^([^>]+)>(.+)*$/;
		if (!$lnk_flag) {
			open(LNK, ">$2");
			$lnk_flag = 1;
		}
		print LNK "$1\n";
#		print "$1\n";
	}
}
if (!$lnk_flag) {
	close(LNK);
}
