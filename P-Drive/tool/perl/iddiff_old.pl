#
#	iddiff.pl2	1994.07.18	R.Nishimura
#
# 解説
#	２つの ident 出力ファイルを読み込んで比較する。
#
# 使用方法
#	b:>perl iddiff.pl2 r0003-1.2 r0003-1.3
#
#print "\t\t",$ARGV[0],"\t\t",$ARGV[1],"\n";
#
$c = 0;
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
$rev = '';
$l_rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	tr/\\/\//;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if (/\$log: (\S+) */) {
		$l_rcs = $1;
	};
	if(( $rev ne '') && ( $l_rcs ne '')) {
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		@l_rcs_ = split(/\./, $l_rcs);
		if('v' eq chop($l_rcs_[$#l_rcs_])) {
			$l_rcs_[$#l_rcs_] = $f_typ[$#f_typ];
			$l_rcs = join('.', @l_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $l_rcs );
			}
		}
		@h_rcs_ = split(/\./, $h_rcs);
		if('v' eq chop($h_rcs_[$#h_rcs_])) {
			$h_rcs_[$#h_rcs_] = $f_typ[$#f_typ];
			$h_rcs = join('.', @h_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $h_rcs );
			}
		}
		if( index($l_rcs, "p:/") >= $[ ) {
			$rev{$l_rcs} = $rev;
		} elsif( index($h_rcs, "p:/") >= $[ ) {
			$rev{$h_rcs} = $rev;
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
		$l_rcs = '';
	}
}
close(FILE);
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
$rev = '';
$l_rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	tr/\\/\//;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if (/\$log: (\S+) */) {
		$l_rcs = $1;
	};
	if(( $rev ne '') && ( $l_rcs ne '')) {
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		@l_rcs_ = split(/\./, $l_rcs);
		if('v' eq chop($l_rcs_[$#l_rcs_])) {
			$l_rcs_[$#l_rcs_] = $f_typ[$#f_typ];
			$l_rcs = join('.', @l_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $l_rcs );
			}
		}
		@h_rcs_ = split(/\./, $h_rcs);
		if('v' eq chop($h_rcs_[$#h_rcs_])) {
			$h_rcs_[$#h_rcs_] = $f_typ[$#f_typ];
			$h_rcs = join('.', @h_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $h_rcs );
			}
		}
		if( index($l_rcs, "p:/") >= $[ ) {
			if( length($rev{$l_rcs}) < 3 ) {
				$c = $c + 1;
				$orev = "-.-";
				$nrev = $rev;
				$rcs = $l_rcs;
				write;
			} elsif( $rev{$l_rcs} ne $rev ) {
				$c = $c + 1;
				$orev = $rev{$l_rcs};
				$nrev = $rev;
				$rcs = $l_rcs;
				write;
			}
		} elsif( index($h_rcs, "p:/") >= $[ ) {
			if( length($rev{$h_rcs}) < 3 ) {
				$c = $c + 1;
				$orev = "-.-";
				$nrev = $rev;
				$rcs = $h_rcs;
				write;
			} elsif( $rev{$h_rcs} ne $rev ) {
				$c = $c + 1;
				$orev = $rev{$h_rcs};
				$nrev = $rev;
				$rcs = $h_rcs;
				write;
			}
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
		$l_rcs = '';
	}
}
close(FILE);
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
$rev = '';
$rcs = '';
while(<FILE>) {
	tr/A-Z/a-z/;
	tr/\\/\//;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if (/\$log: (\S+) */) {
		$l_rcs = $1;
	};
	if(( $rev ne '') && ( $l_rcs ne '')) {
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		@l_rcs_ = split(/\./, $l_rcs);
		if('v' eq chop($l_rcs_[$#l_rcs_])) {
			$l_rcs_[$#l_rcs_] = $f_typ[$#f_typ];
			$l_rcs = join('.', @l_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $l_rcs );
			}
		}
		@h_rcs_ = split(/\./, $h_rcs);
		if('v' eq chop($h_rcs_[$#h_rcs_])) {
			$h_rcs_[$#h_rcs_] = $f_typ[$#f_typ];
			$h_rcs = join('.', @h_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $h_rcs );
			}
		}
		if( index($l_rcs, "p:/") >= $[ ) {
			$rev2{$l_rcs} = $rev;
		} elsif( index($h_rcs, "p:/") >= $[ ) {
			$rev2{$h_rcs} = $rev;
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
		$l_rcs = '';
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
	tr/\\/\//;
	if (/^(\S+)/) {
		$f_typ = $1;
		$wf = $f_typ;
		chop($wf);
	};
	if (/\$header: (\S+) *(\S+)/) {
		$h_rcs = $1;
		$rev = $2;
	};
	if (/\$log: (\S+) */) {
		$l_rcs = $1;
	};
	if(( $rev ne '') && ( $l_rcs ne '')) {
		@f_typ  = split(/\./, $f_typ);
		if($f_typ[$#f_typ] eq $f_typ) {
			$f_typ[$#f_typ] = ':';
		}
		chop( $f_typ[$#f_typ] );
		@l_rcs_ = split(/\./, $l_rcs);
		if('v' eq chop($l_rcs_[$#l_rcs_])) {
			$l_rcs_[$#l_rcs_] = $f_typ[$#f_typ];
			$l_rcs = join('.', @l_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $l_rcs );
			}
		}
		@h_rcs_ = split(/\./, $h_rcs);
		if('v' eq chop($h_rcs_[$#h_rcs_])) {
			$h_rcs_[$#h_rcs_] = $f_typ[$#f_typ];
			$h_rcs = join('.', @h_rcs_);
			if($f_typ[$#f_typ] eq '') {
				chop( $h_rcs );
			}
		}
		if( index($l_rcs, "p:/") >= $[ ) {
			if( length($rev2{$l_rcs}) < 3 ) {
				$c = $c + 1;
				$orev = $rev;
				$rcs = $l_rcs;
				write;
			}
		} elsif( index($h_rcs, "p:/") >= $[ ) {
			if( length($rev2{$h_rcs}) < 3 ) {
				$c = $c + 1;
				$orev = $rev;
				$rcs = $h_rcs;
				write;
			}
		} else {
			print "\nNot Full Path ! [ $l_rcs ]";
			close(FILE);
			exit;
		}
		$rev = '';
		$l_rcs = '';
	}
}
close(FILE);
#
format	STDOUT_TOP = 
-------------------------------------------------------------------------------
No.  filename                                     @<<<<<<<<<<<<< @<<<<<<<<<<<<<
                                                  $ARGV[0],      $ARGV[1]
-------------------------------------------------------------------------------
.
format	STDOUT = 
@### @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<< @<<<<<<<<<<<<<
$c,  $rcs,                                        $orev,      $nrev
.
#
#	end of file
#
