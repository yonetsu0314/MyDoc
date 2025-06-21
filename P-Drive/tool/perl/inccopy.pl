#
#	inccopy.pl	1994.12.23	R.Nishimura
#
# 解説
#	指定されたファイルを、カレントディレクトリに存在しない
#	ファイル名にしてコピーする。
#	コピーされるファイル名のうち、拡張子のみユニーク(.1～.999)なものとする。#
# 使用方法
#	b:>perl inccopy.pl datafile
#
#
@rel = split(/\./, $ARGV[0]);
if($#rel == 0){	$a = 1;}
else	      {	$a = $#rel;}
$rel[$a] = $num;
do {
	$rel[$a]++;
	$rel = join('.', @rel);
} while -e $rel;
print `copy $ARGV[0] $rel`;
