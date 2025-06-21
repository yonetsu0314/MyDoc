#
#	mktblinf.pl			2004.03.05	A.Ishino
#	Original: mkhed.pl	2003.04.28	K.Sakaguchi
#
# 解説
#	プログラム情報ファイルから
#	プログラム番号、バージョン番号、IF定義表情報を取り出す
#
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl mktblinf.pl l9020.c > l9020.hed	バッチファイル作成
#
	$fname = $ARGV[0];
	@fname = split(/\./, $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
			$rcs = $1;
		}
		if (/iftbl_sum = (\d+);/){
			$tblsum = $1;
		}
		if (/iftbl_ver = (\d+);/){
			$tblver = $1;
		}
		if (/iftbl_rev = (\d+);/){
			$tblrev = $1;
		}
	}
	close(FILE);
	if($rev eq '') {
		$rel = "1.1";
		print "$fname-$rel, ";
	} else {
		@rel = split(/\./, $rev);
		$rel = join('.', @rel );
		print "$fname[0]-$rel, ";
	}

	print "$tblver, $tblrev, $tblsum";
	print "\n";
	
#
#	end of file
#
