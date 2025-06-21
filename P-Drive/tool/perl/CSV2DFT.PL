# csv2dft.pl				01.11.21 h.konishi
# 翻訳ファイル(csvファイル)から dft ファイルを編集する
#
# perl csv2dft.pl ifiles
#   ifiles : 入力ファイル
#=============================================================================
# 入力ファイルからFORMAT～.とSCROLL～.抽出する。
#=============================================================================
# 履歴:
# 2001.11.21 h.konishi		作成
#=============================================================================
$ifile = shift(@ARGV);
$LABEL = 0;
$id = 0;
$id1 = 0;

open(IFP, "<$ifile") || die "$ifile がオープンできません。\n";
while (<IFP>) {
	if (/^(\w+).dft,/) {
		if ($id == 1) {
			if ($id1 != 0) {
				print OFP ".\n";
			}
			close(OFP);
		}
		$ofile = "$1.dft";
		open(OFP, ">$ofile") || die "$ofile がオープンできません。\n";
		print "$ifile => $ofile\n";
		$id = 1;
		$id1 = 0;
	} elsif (/^F,(.+),/ || /^F,(.+)/) {
		if ($id1 == 0) {
			print OFP "FORMAT\n$1\n";
			$id1 = 1;
		} elsif ($id1 == 1) {
			print OFP "\t$1\n";
		}
	} elsif (/^S,(.+),"(.+)"/ || /^S,(.+),(\(.+\))/) {
		if ($id1 == 1) {
			print OFP ".\nSCROLL\n$1\t//$2\n";
			$id1 = 2;
		} elsif ($id1 == 2) {
			print OFP "$1\t//$2\n";
		}
	} elsif (/^S,(.+),/) {
		if ($id1 == 1) {
			print OFP ".\nSCROLL\n$1\n";
			$id1 = 2;
		} elsif ($id1 == 2) {
			print OFP "$1\n";
		}
	}
}
if ($id == 1) {
	close(OFP);
}
close(IFP);

__END__
