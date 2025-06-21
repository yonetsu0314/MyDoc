#=============================================================================
# rcu32si.pl					1998.12.21  R.Nishimura
#
# original: rcu32si.r1			1998.10.19 r.nishimura
#
# SIファイルを RCU32開発環境用に編集する
# SISCR 構造体を削除する
#
# perl rcu32si.pl odir ifiles
#   odir   : 出力ディレクトリ
#   ifiles : 入力ファイル（複数指定可）
#=============================================================================
# 入力ファイルとなる SIファイルとしては、
#・「スーパーで～ぐ」から生成されたファイル
#・上記ファイルを CVコマンドで変換したファイル
#・上記ファイルをさらに edit_si.pl3で変換したファイル
#
# int(unsigned int)をshort(unsigned short)に変更する場合には、
# ライブラリの suprdeg2.c も修正する必要がある。
# INT2(0x1000)での参照を(int *)から(short *)に変更する
# BYTE2(0x2000)での参照を(word *)から(unsigned short *)に変更する
#
# この変更を行う理由としては、intとshortが同じサイズであるという前提で
# プログラミングされている場合がある為である。
# この問題を一時的に回避しているプログラムがあるので、変更する場合には
# この回避処理も元に戻す必要がある。
#=============================================================================

$odir = shift(@ARGV);

foreach $ifile (@ARGV) {
	$ofile = $ifile;
	$ofile =~ s/^.+[:\\]//;
	$ofile = "$odir\\$ofile";
	open(IFP, "<$ifile") || die "$ifile がオープンできません。\n";
	open(OFP, ">$ofile") || die "$ofile がオープンできません。\n";
	print "$ifile => $ofile\n";
	undef $ifile;
	undef $ofile;
	while (<IFP>) {
		if (/^struct\s+\S+_val\s+{\s*$/) {
			print OFP;
			$loop = 0;
			$offset[$loop] = 0;
			while (<IFP>) {
#rn				s/^(\s+)int(\s+\S+)/$1short$2/;
#rn				s/^(\s+)word(\s+\S+)/$1unsigned short$2/;
#rn				s/^(\s+unsigned\s+)int(\s+\S+)/$1short$2/;
				print OFP;
				if (/^\};/) {
					last;
				} elsif ((/^\s+int\s+\S+/) 	||
						 (/^\s+unsigned\s+int\s+\S+/) ||
						 (/^\s+long\s+\S+/) 	||
						 (/^\s+unsigned\s+long\s+\S+/) ||
						 (/^\s+word\s+\S+/)) {
					if(int($offset[$loop] % 4) != 0) {
						$offset[$loop] = (int($offset[$loop] / 4) + 1) * 4;
					}
					$offset[$loop+1] = $offset[$loop] + 4;
					$loop++;
				} elsif ((/^\s+short\s+\S+/) ||
						 (/^\s+unsigned\s+short\s+\S+/)) {
					if(int($offset[$loop] % 2) != 0) {
						$offset[$loop] = (int($offset[$loop] / 2) + 1) * 2;
					}
					$offset[$loop+1] = $offset[$loop] + 2;
					$loop++;
				} elsif (/^\s+double\s+\S+/) {
					if(int($offset[$loop] % 4) != 0) {
						$offset[$loop] = (int($offset[$loop] / 4) + 1) * 4;
					}
					$offset[$loop+1] = $offset[$loop] + 8;
					$loop++;
				} elsif (/^\s+char\s+\S+\s*\[(\d+)\+(\d+)\]\w*\s*/) {
					$offset[$loop+1] = $offset[$loop] + $1 + $2;
					$loop++;
				} elsif (/^\s+char\s+\S+\s*\[(\d+)\]\w*\s*/) {
					$offset[$loop+1] = $offset[$loop] + $1;
					$loop++;
				}
			}
			$loop_max = $loop;
		} elsif ((/^static\s+SICEL\s+\S+_cel\[\]\s+=\s+{\s*$/) 			||
		         (/^static\s+SICEL\s+far\s+\S+_cel\[\]\s+=\s+{\s*$/) 	||
		         (/^SICEL\s+\S+_cel\[\]\s+=\s+{\s*$/)					||
		         (/^SICEL\s+far\s+\S+_cel\[\]\s+=\s+{\s*$/)) {
			print OFP;
			$loop = 0;
			while (<IFP>) {
				if (/^\};/) {
					print OFP;
					last;
				} elsif (/^(\s+{\s+)(\S+)(,.+)/) {
					print OFP "$1$offset[$loop]$3\n";
					$loop++;
				}
			}
			if( $loop_max != $loop ) {
				print OFP "!!!ERROR!!!";
				print "!!!ERROR!!!";
			}
		} else {
			/^static\s+SISCR\s+\w*\s*\w+_scr\s+=\s+{\s*$/ && last;
			/^SISCR\s+\w*\s*\w+_scr\s+=\s+{\s*$/ && last;
			s!^(#include\s+)"(.+)"!$1<rculib/$2>!;
			print OFP;
		}
	}
	close(IFP);
	close(OFP);
}
