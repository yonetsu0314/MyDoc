# mksiall.pl						1994.8.24 m.fukutome
#
#=============================================================================
# 多国語対応のための SI ファイルを作成する
#
#   perl mksiall.pl filename
#   filename : 統合する SI ファイルのリストと処理ディレクトリを記述したファイル
#	フォーマット
#	  SIDIR = dirname	#ターゲットディレクトリ名
#	  L*DIR = dirname	#ソースディレクトリ名 (*:1-4)
#	  mksi:
#	  file1.si:		#処理ファイル名
#	  file2.si:
#	    |
#	  fileN.si:
#	  mksiend:
#	いずれの行も行の先頭から記述する必要があります。このフォーマットは
#	メイクファイルの記述方法と互換性がありますので、生成に必要な情報を
#	メイクファイルに記述することが可能です。ただしメイクファイル中の
#	マクロやルールは処理できませんので注意してください。
#
#   各ファイルには次の操作が行われます。
#   1) 各ディレクトリの SI ファイルから ELS_STR 定義部を抽出し、統合した配列
#      として新たな定義を作成する。
#   2) ELS_FRAME 定義部は ELSDRAW.H の構造体定義と内容が合わなくなるため
#      ELS_FRAMES と構造体定義名を付け替える。
#   3) SISCR 定義部は削除される。
#   4) ELS_FRAMES 定義を有効にするため mltling.h をインクルードする。
#   
#   ベースファイルは L1DIR にあるファイルとなります。他のディレクトリのファイル
#   からは、ELS_STR 定義の内容のみが使用されます。
#=============================================================================
# 履歴:
#   1995.11.02 m.fukutome
#   エラー発生時にファイルのクローズが行われない場合がある不具合を修正
#=============================================================================

undef	@fname;
undef	@sdir;
undef	$tgdir;
$error_detect = 0;

while (<>) {
	s/#.*$//;
	/^mksi:/ && last;
	if (/^SIDIR[ \t]*=/) {
		s/=/ /;
		split;
		$tgdir = $_[1];
	}
	if (/^L([1-4])DIR[ \t]*=/) {
		$i = $1 - 1;
		s/=/ /;
		split;
		$sdir[$i] = $_[1];
	}
}

defined $tgdir || die "SIDIR の定義がありません。";
defined @sdir  || die "L*DIR の定義がありません。";

$dmax = $#sdir + 1;

while (<>) {
	/^([a-z_0-9]{1,8}\.si):/i && &mksisub($1);
	/^mksiend:/ && last;
}

$error_detect != 0 && die;

sub mksisub {
	$fname = shift(@_);
	print "$fname:\n";
	for ($i = 0; $i < $dmax; ++$i) {
		$iname[$i] = "$sdir[$i]\\$fname";
	}
	if (!open(FP0, "<$iname[0]")) {
		print "ファイル（$iname[0]）がオープンできません。\n";
		$error_detect = 1;
		return;
	}

	$oname = "$tgdir\\$fname";
	if (!open(OFP, ">$oname")) {
		print "ファイル（$oname）がオープンできません。\n";
		$error_detect = 1;
		close(FP0);
		return;
	}

	while (<FP0>) {
		if (/^static[ \t]+ELS_STR[ \t]+far[ \t](\w+_str)\[\] = \{/) {
			&elsstr_func($1) == 0 && return;
		} elsif (/^#include[ \t]+\"si\.h\"/i) {
			print OFP;
			print OFP "#include \"mltling.h\"\n";
		} else {
			/^SISCR.+_scr = \{/ && last;
			s/^(static ELS_FRAME)( \w+_frame = {)/$1S$2/;
			print OFP;
		}
	}
	close(FP0);
	close(OFP);
}

sub elsstr_func {
	$func_name = shift(@_);
	s/$func_name/$&0/;
	print OFP;
	while (<FP0>) {
		print OFP;
		/^};/ && last;
	}
	print OFP "\n";
	for ($i = 1; $i < $dmax; ++$i) {
		if (!open(FP, "<$iname[$i]")) {
			print "ファイル（$iname[$i]）がオープンできません。\n";
			$error_detect = 1;
			return 0;
		}
		while (<FP>) {
			/^static[ \t]+ELS_STR[ \t]+far[ \t]\w+_str\[\]/
				&& last;
		}
		s/$func_name/$&$i/;
		print OFP;
		while (<FP>) {
			print OFP;
			/^};/ && last;
		}
		print OFP "\n";
		close(FP);
	}
	print OFP "static ELS_STR * far $func_name[] = {\n";
	for ($i = 0; $i < $dmax; ++$i) {
		print OFP "\t&$func_name${i}[0],\n";
	}
	print OFP "};\n\n";
}

#
#	END OF FILE
#
