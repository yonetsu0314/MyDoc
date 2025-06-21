#!/usr/local/bin/perl
#					1995.12/04 NAKAMURA Yo.
# NAME:
#	UFF2FGP.pl - ユニバーサルファイルのフォーマットを変換する。
# SYNOPSIS:
#	uff2fgp.pl [options] < in-file > out-file
# OPTIONS:
#	-t	入力データが 伝達関数 のデータであることを示す。
#
print OK;

$PI = atan2(1,1)*4;

$Options = shift(@ARGV);
if( $Options eq "-" ) {
	$CH{1}=1;
} elsif( $Options =~ /^-/ ) {
  for( $i=1; $i<length($Options); $i++ ){
	if( ($c=substr($Options,$i,1)) =~ /1/ ){
		$CH{1} = 1;
	} else {
		print STDERR "uff2fgp.pl: unknown: ",$c,"\n";
		print STDERR "Usage: uff2fgp.pl [[-Options] Value1 Value2 ...]\n";
		print STDERR "\n";
		print STDERR "Options:\n";
		print STDERR "        t     TrabsferFunction\n";
		print STDERR "\n";
		exit;
	}
  }
}

#####
# 1. -1
$Line=<>;
# 2. 58:計測データ
$Line=<>;
print STDERR $Line;
# 3. Comments
$Line=<>;
print STDERR $Line;
$Line=<>;
print STDERR $Line;
# 5. Date
$Line=<>;
print STDERR $Line;
# 6. EXITATIONRESPONSE
$Line=<>;
print STDERR $Line;
# 7. NONE
$Line=<>;
# 8. [1/2/3/4/6],-,-,0,NONE,[1-500],[+-123456],NONE,[1-500],[+-123456]
$Line=<>;
print STDERR $Line;
# 9. [1/2/4/5/6],[1-801],1,>0,>0,-
$Line=<>;
print STDERR $Line;
chop $Line;
$Line =~ s/\t/ /g;
$Line =~ s/      / /g;
$Line =~ s/     / /g;
$Line =~ s/    / /g;
$Line =~ s/   / /g;
$Line =~ s/  / /g;
$Line =~ s/^ //;
($YDType,$DNum,$XMode,$XStart,$XStep) = split(/ /,$Line);
printf "%7d\n", $DNum;
# 10. 18,-,-,-,NONE,NONE
$Line=<>;
print STDERR $Line;
# 11. [0/8/11/12/13/15],-,-,-,NONE,NONE
$Line=<>;
print STDERR $Line;
# 12. [0/8/11/12/13/15],-,-,-,NONE,NONE
$Line=<>;
print STDERR $Line;
# 13. -,-
$Line=<>;
#####
#####
# 14. DATA
$M=0;
while($Line=<>){
	chop $Line;
	$Line =~ s/\t/ /g;
	$Line =~ s/      / /g;
	$Line =~ s/     / /g;
	$Line =~ s/    / /g;
	$Line =~ s/   / /g;
	$Line =~ s/  / /g;
	$Line =~ s/^ //;
	
	($R{1},$I{1},$R{2},$I{2},$R{3},$I{3}) = split(/ /,$Line);

 	if( $M < $DNum ){
	    $G = sqrt($R{1}*$R{1}+$I{1}*$I{1});
	    $G = 20./log(10.)*log($G);
	    $P = 180./$PI*atan2($R{1},$I{1});
	    printf "%10.3f%10.3f %10.3f\n", $XStart+$M*$XStep,$G,$P;
	    $M++;
	}
 	if( $M < $DNum ){
	    $G = sqrt($R{2}*$R{2}+$I{2}*$I{2});
	    $G = 20./log(10.)*log($G);
	    $P = 180./$PI*atan2($R{2},$I{2});
	    printf "%10.3f%10.3f %10.3f\n", $XStart+$M*$XStep,$G,$P;
	    $M++;
	}
 	if( $M < $DNum ){
	    $G = sqrt($R{3}*$R{3}+$I{3}*$I{3});
	    $G = 20./log(10.)*log($G);
	    $P = 180./$PI*atan2($R{3},$I{3});
	    printf "%10.3f%10.3f %10.3f\n", $XStart+$M*$XStep,$G,$P;
	    $M++;
	}

	if ( ($M%50) == 0 ){	print STDERR ".";	}
	if ( ($M%4000) == 0 ){	print STDERR "\n";	}

}
print STDERR "\n";
#####