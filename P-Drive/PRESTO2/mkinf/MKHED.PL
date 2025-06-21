#*$Header: p:/presto/mkinf/rcs/mkhed.pl 1.1 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: mkhed.pl $
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* プログラム番号とバージョン番号を取り出す
#* 
#
#	mkhed.pl	2003.04.28	K.Sakaguchi
#
# 解説
#	プログラム情報ファイルから
#	プログラム番号とバージョン番号を取り出す
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# 使用方法
#	b:>cd \work				ワークファイル展開場所指定
#	b:>perl mkhed.pl l9020.c > l9020.hed	バッチファイル作成
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
	}
	close(FILE);
	if($rev eq '') {
		$rel = "1.1";
		print "$fname $rel\n";
	} else {
		@rel = split(/\./, $rev);
		$rel = join('.', @rel );
		print "$fname[0] $rel\n";
	}
#
#	end of file
#
