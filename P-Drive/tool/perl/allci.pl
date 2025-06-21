#
#	allci.pl	1993.11.01	R.Nishimura
#
# 解説
#	カレント作業ディレクトリにすべてのソースコードファイルを集め
#	下記のコマンドを実行することによって
#	チェックイン対象ファイル（書き込み可能属性）のみのチェックイン
#	コマンド行を -r -u -m オプション付きで 生成する。
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
#		b:>perl allci.pl *.* > p:..\rxxxxx.bat
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
	@fname = split(/\./, $fname);
	if( $fname[1] eq '' ) {
		$fname[1] = 'v';
	} elsif ( length($fname[1]) == 3 ) {
		chop( $fname[1] );
		$fname[1] .= 'v';
	} else {
		$fname[1] .= 'v';
	}
	$fname2 = "p:".$fname[0].'.'.$fname[1];
	$2 = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		print STDERR $1,' ',$2,"\n" if /\$Header: (\S+) *(\S+)/;
	}
	close(FILE);
	if($2 eq '') {
		$rel = "1.1";
	} else {
		@rel = split(/\./, $2);
		$rel[$#rel]++;
		$rel = join('.', @rel );
	}
	print "ci -r$rel -u -m\"LOG MESSAGE\" $fname $fname2\n";
	if($fname[1] eq "mkv") {
		print "rcs -c\"#* \" $fname2\n";
		print "co $fname $fname2\n";
	}
}
#
#	end of file
#
