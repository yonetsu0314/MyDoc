# dft2cnv.pl				01.11.21 h.konishi
# dftファイルを翻訳を依頼するシートに編集する
#
# perl dft2cnv.pl ofile ifiles
#   ofile  : 出力ファイル
#   ifiles : 入力ファイル（複数指定可）
#=============================================================================
# 入力ファイルから、FORMAT～.までとSCROLL～.までを出力ファイルに出力する。
#=============================================================================
# 履歴:
# 2001.11.21 h.konishi		作成
#=============================================================================

$ofile = shift(@ARGV);
$LABEL = 0;

open(OFP, ">$ofile") || die "$ofile がオープンできません。\n";
foreach $ifile (@ARGV) {
	open(IFP, "<$ifile") || die "$ifile がオープンできません。\n";
	print "$ifile => $ofile\n";
	while (<IFP>) {
		s/\t//g;
		if (/^\$Log:\s*(\w+).dft\s*\$/) {
			print OFP "$1.dft,\"\"12345678901234567890123456789012\"\"\n";
		}
		if ($LABEL == 0) {
			if (/^FORMAT/) {
				$LABEL = 1;
			}
			if (/^SCROLL/) {
				$LABEL = 2;
			}
		} elsif ($LABEL == 1) {
			if (/^\./) {
				$LABEL = 0;
			} else {
				print OFP "F,$_";
			}
		} elsif ($LABEL == 2) {
			if (/^\./) {
				$LABEL = 0;
			} elsif(m%(.*)//(.+)%) {
				print OFP "S,$1,\"$2\"\n";
			} else {
				print OFP "S,$_";
			}
		}
	}
	close(IFP);
}
close(OFP);

__END__
