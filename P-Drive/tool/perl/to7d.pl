#!/usr/local/bin/perl
#					1995.11/21 NAKAMURA Yo.
#NAME:
#	to7d.pl - 任意フォーマットのデータを７桁整数に変換する。
#SYNOPSIS:
#	to7d.pl [options] < in-file > out-file
#OPTIONS:
#	-N	入力データが ヘッドNo.N のデータであることを示す。
#		ヘッド数は、１からｏまでの ２４ヘッド。
#
$AddZERO = 1;

$Options = shift(@ARGV);
if( $Options eq "-" ) {
	$CH{1}=1;
	$CH{2}=2;
	$CH{3}=3;
	$CH{4}=4;
	$CH{5}=5;
	$CH{6}=6;
	$CH{7}=7;
	$CH{8}=8;
	$CH{9}=9;
	$CH{10}=10;
	$CH{11}=11;
	$CH{12}=12;
	$CH{13}=13;
	$CH{14}=14;
	$CH{15}=15;
	$CH{16}=16;
	$CH{17}=17;
	$CH{18}=18;
	$CH{19}=19;
	$CH{20}=20;
	$CH{21}=21;
	$CH{22}=22;
	$CH{23}=23;
	$CH{24}=24;
} elsif( $Options =~ /^-/ ) {
  for( $i=1; $i<length($Options); $i++ ){
	if( ($c=substr($Options,$i,1)) =~ /1/ ){
		$CH{1} = 1;
	} elsif( ($c=substr($Options,$i,1)) =~ /2/ ){
		$CH{2} = 2;
	} elsif( ($c=substr($Options,$i,1)) =~ /3/ ){
		$CH{3} = 3;
	} elsif( ($c=substr($Options,$i,1)) =~ /4/ ){
		$CH{4} = 4;
	} elsif( ($c=substr($Options,$i,1)) =~ /5/ ){
		$CH{5} = 5;
	} elsif( ($c=substr($Options,$i,1)) =~ /6/ ){
		$CH{6} = 6;
	} elsif( ($c=substr($Options,$i,1)) =~ /7/ ){
		$CH{7} = 7;
	} elsif( ($c=substr($Options,$i,1)) =~ /8/ ){
		$CH{8} = 8;
	} elsif( ($c=substr($Options,$i,1)) =~ /9/ ){
		$CH{9} = 9;
	} elsif( ($c=substr($Options,$i,1)) =~ /a/ ){
		$CH{10} = 10;
	} elsif( ($c=substr($Options,$i,1)) =~ /b/ ){
		$CH{11} = 11;
	} elsif( ($c=substr($Options,$i,1)) =~ /c/ ){
		$CH{12} = 12;
	} elsif( ($c=substr($Options,$i,1)) =~ /d/ ){
		$CH{13} = 13;
	} elsif( ($c=substr($Options,$i,1)) =~ /e/ ){
		$CH{14} = 14;
	} elsif( ($c=substr($Options,$i,1)) =~ /f/ ){
		$CH{15} = 15;
	} elsif( ($c=substr($Options,$i,1)) =~ /g/ ){
		$CH{16} = 16;
	} elsif( ($c=substr($Options,$i,1)) =~ /h/ ){
		$CH{17} = 17;
	} elsif( ($c=substr($Options,$i,1)) =~ /i/ ){
		$CH{18} = 18;
	} elsif( ($c=substr($Options,$i,1)) =~ /j/ ){
		$CH{19} = 19;
	} elsif( ($c=substr($Options,$i,1)) =~ /k/ ){
		$CH{20} = 20;
	} elsif( ($c=substr($Options,$i,1)) =~ /l/ ){
		$CH{21} = 21;
	} elsif( ($c=substr($Options,$i,1)) =~ /m/ ){
		$CH{22} = 22;
	} elsif( ($c=substr($Options,$i,1)) =~ /n/ ){
		$CH{23} = 23;
	} elsif( ($c=substr($Options,$i,1)) =~ /o/ ){
		$CH{24} = 24;
	} elsif( ($c=substr($Options,$i,1)) =~ /u/ ){
		$AddZERO = 0;
	} else {
		print STDERR "to7d.pl: unknown: ",$c,"\n";
		print STDERR "Usage: to7d.pl [[-Options] Value1 Value2 ...]\n";
		print STDERR "\n";
		print STDERR "Options:\n";
		print STDERR "        1     exist HEAD DATA No.1\n";
		print STDERR "        2     exist HEAD DATA No.2\n";
		print STDERR "          .\n";
		print STDERR "          .\n";
		print STDERR "        a     exist HEAD DATA No.10\n";
		print STDERR "          .\n";
		print STDERR "        o     exist HEAD DATA No.24\n";
		print STDERR "\n";
		print STDERR "        u     not append  value 0\n";
		print STDERR "\n";
		exit;
	}
  }
}

$Line=<>;
chop $Line;
$Line =~ s/\t/ /g;
$Line =~ s/      / /g;
$Line =~ s/     / /g;
$Line =~ s/    / /g;
$Line =~ s/   / /g;
$Line =~ s/  / /g;
$Line =~ s/^ //;

($Time) = split(/ /,$Line);
printf "%5d\n", $Time;

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
	
	($Time,$H{1},$H{2},$H{3},$H{4},$H{5},$H{6},$H{7},$H{8},$H{9},$H{10},$H{11},$H{12},$H{13},$H{14},$H{15},$H{16},$H{17},$H{18},$H{19},$H{20},$H{21},$H{22},$H{23},$H{24}) = split(/ /,$Line);

	printf "%6d", $Time;
	for($j=1,$i=1;$i<=24;$i++){
		if ( $CH{$i} != 0 ){		printf "%7d", $H{$j}; $j++; 	}
		elsif($AddZERO ==1 ) {	printf "%7d", 0;	}
	}
#	printf "%7d%7d%7d%7d%7d%7d%7d%7d", $C{9},$C{10},$C{11},$C{12},$C{13},$C{14},$C{15},$C{16};
	printf "\n";

	$M++;
	if ( ($M%50) == 0 ){	print STDERR ".";	}
	if ( ($M%4000) == 0 ){	print STDERR "\n";	}

}
print STDERR "\n";
