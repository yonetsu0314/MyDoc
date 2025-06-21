#
#	iddiff.pl	1994.02.0	R.Nishimura
#
# 解説
#	２つの ident 出力ファイルを読み込んで比較する。
#
# 使用方法
#	b:>perl iddiff.pl r0003_1.2 r0003_1.3
#
#print "\t\t",$ARGV[0],"\t\t",$ARGV[1],"\n";
#
$c = 0;
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		@fname = split(/\W/, $rcs);
		$f = $fname[$#fname-1].".".$fname[$#fname];
		$rev{$f} = $rev;
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		@fname = split(/\W/, $rcs);
		$f = $fname[$#fname-1].".".$fname[$#fname];
		if( length($rev{$f}) < 3 ) {
			$c = $c + 1;
			$orev = "-.-";
			$nrev = $rev;
			write;
		} elsif( $rev{$f} ne $rev ) {
			$c = $c + 1;
			$orev = $rev{$f};
			$nrev = $rev;
			write;
		}
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		@fname = split(/\W/, $rcs);
		$f = $fname[$#fname-1].".".$fname[$#fname];
		$rev2{$f} = $rev;
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$rcs = '';
$nrev = "-.-";
while(<FILE>) {
	tr/A-Z/a-z/;
	$rev = $2 if (/\$header: (\S+) *(\S+)/);
	$rcs = $1 if (/\$log: (\S+) */);
	if(( $rev ne '') && ( $rcs ne '')) {
		@fname = split(/\W/, $rcs);
		$f = $fname[$#fname-1].".".$fname[$#fname];
		if( length($rev2{$f}) < 3 ) {
			$c = $c + 1;
			$orev = $rev;
			write;
		}
		$rev = '';
		$rcs = '';
	}
}
close(FILE);
#
format	STDOUT_TOP = 
-------------------------------------------------
No.     filename    @<<<<<<<<<<<<< @<<<<<<<<<<<<<
                    $ARGV[0],      $ARGV[1]
-------------------------------------------------
.
format	STDOUT = 
@###    @<<<<<<<<<< @<<<<<<<<<<<<< @<<<<<<<<<<<<<
$c,     $f,         $orev,      $nrev
.
#
#	end of file
#
