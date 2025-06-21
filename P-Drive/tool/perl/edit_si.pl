# edit_si.pl						95.11.07 m.fukutome
# SI ファイルを編集する
#
# perl edit_si.pl odir ifiles
#   odir   : 出力ディレクトリ
#   ifiles : 入力ファイル（複数指定可）
#=============================================================================
#1.SISCR 構造体を削除する
#2.ELS_STR 構造体中の文字列を static char far 定義として構造体の外で定義する
#=============================================================================
# 履歴:
#   1995.11.07 m.fukutome
#   delsiscr.pl (94/09/01, 95/11/04) を元に作成
#   1996.09.30 m.fukutome
#   ELS_STR 構造体中のコメントを無視するように変更
#=============================================================================

$tname = "elstmp.tmp";
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
		if (/^static ELS_STR /) {
			&mk_els_str;
		} else {
			/^SISCR[ \t]+\w*[ \t]*\w+_scr[ \t]+=[ \t]+{[ \t]*$/ && last;
			s/^(static ELS_FRAME)( \w+_frame = {)/$1 far$2/;
			print OFP;
		}
	}
	close(IFP);
	close(OFP);
}

sub mk_els_str_sub {
	while (<$Handle>) {
		s%/\*.*\*/%%g;
		s/\s+$//;
		if (length > 0) {
			if (/NULL$/) {
				print TFP "$_\n";
				next;
			}
			if (/^\};/) {
				print TFP "$_\n";
				last;
			}
			/,(\".+\"),$/;
			print OFP "static char far ${t}_$j\[\] = $1;\n";
			print TFP "$`,${t}_$j,\n";
			++$j;
		}
	}
	print OFP "\n";
	print TFP "\n";
}

sub mk_els_str {
	open(TFP, ">$tname") || die "$tname がオープンできません。\n";
	print TFP;
	/([\w\d]+_str[\d]*)\[/;
	$t = $1;
	$j = 0;
	$Handle = "IFP";
	&mk_els_str_sub;
	close(TFP);
	open(TFP, "<$tname") || die "$tname がオープンできません。\n";
	while (<TFP>) {
		print OFP;
	}
	close(TFP);
}

__END__
