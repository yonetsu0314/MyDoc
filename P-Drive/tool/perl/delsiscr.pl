# delsiscr.pl						94.9.1 m. fukutome
# SI ファイルから SISCR 構造体を削除する

# perl delsiscr.pl odir ifiles
#   odir   : 出力ディレクトリ
#   ifiles : 入力ファイル（複数指定可）

$odir = shift(@ARGV);

foreach $ifile (@ARGV) {
	$iname = $ifile;
	$iname =~ s/^.+://;
	$iname =~ s/^.+\\//;
	$ofile = "$odir\\$iname";
	open(IFP, "<$ifile") || die "$ifile がオープンできません。\n";
	open(OFP, ">$ofile") || die "$ofile がオープンできません。\n";
	print "$ifile => $ofile\n";
	$siscr_f = 0;
	while (<IFP>) {
		/^SISCR[ \t]+\w*[ \t]*\w+_scr[ \t]+=[ \t]+{[ \t]*$/ && last;
		print OFP;
	}
	close(IFP);
	close(OFP);
}
