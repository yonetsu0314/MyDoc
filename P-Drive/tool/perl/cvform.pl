# cvform.pl						m.fukutome 1994.10.12
# 画面定義ファイルを読み込み画面定義ヘッダを生成する（LCW 用）

# perl cvform.pl xxxxx.dft

#==============================================================================
# 仕様変数の一覧と定義

$define_func	=	0;	# 定義選択

# 定義中フラグ
$keydef_f	=	0;	# キー定義
$comment_f	=	0;	# コメント定義中

# 多重定義チェック用カウンタ
$bname_n	=	0;	# ベースネーム定義
$format_n	=	0;	# フォーマット定義
$scroll_n	=	0;	# スクロール定義
$keydef_n	=	0;	# キー定義
#$comment_n	=	0;	# コメント定義
#$Write_n	=	0;	# 直接出力

undef $format_name;		# フォーマット構造体名
undef $scroll_name;		# スクロール構造体名
undef $keydef_name;		# キー定義構造体名

%key_def = (	# キーシンボルリスト
	"KEY_UP",	1,
	"KEY_ENTER",	1,
	"KEY_0",	1,
	"KEY_1",	1,
	"KEY_2",	1,
	"KEY_3",	1,
	"KEY_4",	1,
	"KEY_5",	1,
	"KEY_6",	1,
	"KEY_7",	1,
	"KEY_8",	1,
	"KEY_9",	1,
	"KEY_DP",	1,
	"KEY_CL",	1,
	"KEY_START",	1,
	"KEY_STOP",	1,
	"KEY_ZERO",	1,
	"KEY_SET",	1,
	"KEY_MON",	1,
	"KEY_SEL",	1,
	"KEY_END",	1,
	"KEY_TOTAL",	1,
);

$MaxLine	= 8;		#最大表示可能行数

#          dummy  F1 F2 F3 F4
@Line_ht = (0,     1, 2, 4, 8);
@chr_wd  = (0,     1, 1, 2, 4);

undef $bname;			# 変数名の基部（デフォルトはファイル名の基部）
$iname	=	$ARGV[0];	# 入力ファイル名
$oname	=	$ARGV[1];	# 出力ファイル名
$tname	=	"cvform.tmp";	# テンポラリファイル名
undef $tmp_dir;			# テンポラリディレクトリ

$iext	= "dft";		# デフォルト入力ファイル拡張子
$oext	= "dh";			# デフォルト出力ファイル拡張子
#==============================================================================
# 前処理

# テンポラリファイル名の作成。環境変数 TMP または TEMP で定義されている
# ディレクトリを使用

if (defined $ENV{'TMP'}) {
	$tmp_dir = $ENV{'TMP'};
} elsif (defined $ENV{'TEMP'}) {
	$tmp_dir = $ENV{'TEMP'};
}

if (defined $tmp_dir) {
	$len = length($tmp_dir);
	if (substr($tmp_dir, $len - 1, 1) ne '\\') {
		$tmp_dir .= '\\';
	}
	$tname = $tmp_dir . $tname;
}

# 入力ファイル名の生成

if ($iname !~ m/\..+$/) {
	$iname .= ".$iext";
}

# 変数名基部の生成

$bname = $iname;
$bname =~ s/.*://g;
$bname =~ s/.*\\//g;
$bname =~ s/\..*$//g;

# 出力ファイル名の生成

if (defined $oname) {
	$len = length($oname);
	$c = substr($oname, $len - 1, 1);
	if ($c eq ':' || $c eq '\\') {
		$oname .= "$bname.$oext";
	}
} else {
	$oname = "$bname.$oext";
}

print "$iname\n";
# print "oname = $oname\n";

#==============================================================================
open(IFILE,"<$iname") || die "入力ファイル（$iname）がオープンできません。";
open(OFILE,">$oname") || die "出力ファイル（$oname）がオープンできません。";

#==============================================================================

&put_header;

while(<IFILE>) {
	if ($define_func != 0) {
		&format_func  if $define_func == 1;
		&scroll_func  if $define_func == 2;
		&keydef_func  if $define_func == 3;
		&comment_func if $define_func == 4;
		&write_func   if $define_func == 5;
	} else {
		if (/^BASENAME[ \t=]/) {
			if (++$bname_n > 1) {
				print "Line $.:多重定義エラー\n";
			} else {
				&basename_func;
			}
		}
		if (/^FORMAT$/) {
			$define_func = 1;
			&clear_frm_var;
			if (++$format_n > 1) {
				print "Line $.:多重定義エラー\n";
			}
		}
		if (/^SCROLL$/) {
			$define_func = 2;
			&clear_frm_var;
			if (++$scroll_n > 1) {
				print "Line $.:多重定義エラー\n";
			}
			if ($scroll_line_enable_flag < 1) {
				print "Line $.:" .
				     "スクロール定義は無効です。\n";
			}
		}
		if (/^KEYDEF$/) {
			$define_func = 3;
			if (++$keydef_n > 1) {
				print "Line $.:多重定義エラー\n";
			}
		}
		if (/^COMMENT$/) {
			$define_func = 4;
#			if (++$comment_n > 1) {
#				print "Line $.:多重定義エラー\n";
#			}
		}
		if (/^WRITE$/) {
			$define_func = 5;
#			if (++$Write_n > 1) {
#				print "Line $.:多重定義エラー\n";
#			}
		}
		if (/^\.$/) {
#			$define_func = 0;
		}
	}
}
#==============================================================================
# 定義終了までにファイルの終わりに到達した.
if ($define_func != 0) {
	print "定義が未完了です。\n";
}
#==============================================================================
# 統合データの構造体を生成

#if (defined($keydef_name) || defined($csrdef_name)) {
	print OFILE "struct keyform_t far ${bname}_keyform = {\n";
	if (defined($keydef_name)) {
		print OFILE "\tdim($keydef_name), &${keydef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
	if (defined($csrdef_name)) {
		print OFILE "\tdim($csrdef_name), &${csrdef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
	print OFILE "};\n\n";
#}

print OFILE "struct dfmt_t far ${bname}_dfmt = {\n";
	if (defined($fixfrmdef_name)) {
		print OFILE "\tdim($fixfrmdef_name), &${fixfrmdef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
	if (defined($varfdef_name)) {
		print OFILE "\tdim($varfdef_name), &${varfdef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
	if (defined($scradef_name)) {
		print OFILE "\tdim($scradef_name), &${scradef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
	if (defined($scrddef_name)) {
		print OFILE "\tdim($scrddef_name), &${scrddef_name}[0],\n";
	} else {
		print OFILE "\t0, NULL,\n";
	}
print OFILE "};\n\n";

print OFILE "struct dkf_t far ${bname}_dkf = {\n";
	print OFILE "\t&${bname}_dfmt, &${bname}_keyform,\n";
	print OFILE "\tNULL, 0, 0, 0, 0, 0, 0,\n";
print OFILE "};\n\n";

&put_sep_line;

close(IFILE);
close(OFILE);

#==============================================================================
# ベースネーム定義部の解析とコード生成

sub basename_func {
	if ($format_n > 0 || $scroll_n > 0 || $keydef_n > 0) {
		print "Line $.:使用場所が不適当です。";
		print "他の定義に先行して定義すること\n";
	} else {
		s/^BASENAME[ \t=]+//;
		s/#.*$//;
		split;
		$bname = $_[0];
	}
}

#==============================================================================
# フォーマット定義部の解析とコード生成

sub format_func {
	if (/^\./) {
		++$line_n;	# 行数に補正する。
		$lpos = 0;
		for ($i = 0; $i < $line_n; ++$i) {
			$tmp = $line_hgt_lst[$i];
			$line_hgt_lst[$i] = $lpos;
			$lpos += $tmp;
		}
		if ($lpos > $MaxLine) {
			print "Line $.:表示行数を超えています。\n";
		}
		&put_format;
		&put_var;
		&put_scroll_area;
		&put_cur;
		$define_func = 0;
		&put_sep_line;
	} else {
		&format_parse;
	}
}

sub clear_frm_var {
# 使用変数の初期化
	undef @var_lst;
	undef @var_atr;
	undef @cur_lst;
	$var_cnt_p = 0;
	$var_cnt_s = 0;
	$fn_cnt_p = 0;
	$fn_cnt_s = 0;
	$cur_cnt_p = 0;
	$cur_cnt_s = 0;

	$line_n = -1;
	undef @func_list;
	undef @form_txt;
	undef @line_frm_lst;
	undef @line_hgt_lst;
	undef @func_list;
	undef @scroll_line_inf;

	undef @var_list;
	$v_list_n  = 0;
	undef @var_l_lst;
	undef @var_i_lst;
	undef @cur_fn_lst;
	$cur_fn_cnt = 0;
	$cur_fn_flg = 0;
	$cur_var_flg = 0;

	$line_form = 2;
}
#==============================================================================
# スクロール定義部の解析とコード生成

sub scroll_func {
	if (/^\./) {
		++$line_n;	# 行数に補正する。
		$lpos = 0;
		for ($i = 0; $i < $line_n; ++$i) {
			$tmp = $line_hgt_lst[$i];
			$line_hgt_lst[$i] = $lpos;
			$lpos += $tmp;
		}
		&put_scroll;
		$define_func = 0;
		&put_sep_line;
	} else {
		&format_parse;
	}
}

sub format_parse {
	chop;
	if (m%\t[ \t]*//%) {
		$form = $`;
		$attrib = $';
	} else {
		$form = $_;
		$attrib = "";
	}
	if (length($form) > 0 || length($attrib) == 0) {
		++$line_n;
		&form_parse;
	}
	&attrib_parse;
}

#=============================================================================
# フォーマットエリアの解析

sub form_parse {
	$form =~ s/[ \t]+$//;
	$form = &get_esc_chr($form);
	$val_n = 0;
# 変数位置の解析
	$form_t = $form;
	$cur_x = 0;
	while (($var_p = index($form_t, '@')) >= $[) {
		$form_t =~ s/^[^@]*@([#<>]*)//;
		$c = 's';
		$adj = '';
		$frac_l =0;
		$var_l = 1;
		if (($var_l += length($1)) > 1) {
			if (substr($1,0,1) eq '#') {
				$c = 'f';
				if ($form_t =~ s/^\.(#*)//) {
					$frac_l = length($1);
				}
			} elsif (substr($1,0,1) eq '<') {
				$adj = '-';
			}
		}
		$cur_x += $var_p;
		if ($frac_l > 0) {
			$var_l += $frac_l + 1;
			$var_atr[$var_cnt_p]
				= "${cur_x}x${line_n}y%$var_l.$frac_l$c";
		} else {
			$var_frm = "%$adj$var_l";
			$var_frm .= ".$var_l" if $c eq "s";
			$var_atr[$var_cnt_p]
				= "${cur_x}x${line_n}y$var_frm$c";
		}
		# 変数定義を空白に置換する
		substr($form, $cur_x, $var_l) = ' ' x $var_l;
		$cur_x += $var_l;
		++$var_cnt_p;
	}
# カーソル位置の解析
	$form_t = $form;
	$cur_x = 0;
	while (($var_p = index($form_t, '_')) >= $[) {
		$form_t =~ s/^[^_]*_//;
		$c = '^';
		$cur_x += $var_p;
		substr($form, $cur_x, 1) = ' ';
		$cur_lst[$cur_cnt_p++] = "${cur_x}x${line_n}y";
		++$cur_x;
	}
	$form = &put_esc_chr($form);
	$form =~ s/[ ]*$//;
	$form =~ s/\\/\\\\/g;
	$form =~ s/\"/\\\"/g;
	$form_txt[$line_n] = $form;
}

# ESC 文字の処理
sub put_esc_chr {
	if (defined(@esc_chr)) {
		$_[0] =~ s/\t/shift(esc_chr)/eg;
	}
	return $_[0];
}

sub get_esc_chr {
	undef @esc_chr;
	while ($_[0] =~ s/\\(.)/\t/) {
		push(esc_chr, $1);
	}
	return $_[0];
}

#=============================================================================

# アトリビュートエリアの解析
sub attrib_parse {
	$attrib =~ s/^[ \t]+//;
	$attrib =~ s/[ \t]*#.*$//;
	$attrib =~ s/,/ /g;
	undef @atr;
	@atr = split(/[ \t]+/,$attrib);
	$n = @atr;

	$scroll_f = 0;
	for ($i = 0; $i < $n; ++$i) {
		$it = $atr[$i];
		if ($it =~ /^\[SCROLL\]$/) {
			if ($scroll_f != 1) {
				++$line_n;
				$scroll_f = 1;
			}
		} elsif ($it =~ /^\[F([1-4])\]$/) {	# 行フォントの取得
			$line_form = $1;
		} elsif ($it =~ /^(\w+)(.*)$/) {
			$var_l_lst[$line_n] = $v_list_n;
			$var_list[$v_list_n++] = $it;
		} elsif ($it =~ /^\((.+)\)$/) {
			$it = $1;
			if ($it =~ /^\w+/) {
				$func_list[$line_n] = $&;
				++$cur_fn_flg;
			}
			if ($it =~ /\$([0-9]*)$/) {
				if (length($1) == 0) {
					$it .= $v_list_n - 1;
				}
				++$cur_var_flg;
				$var_i_lst[$line_n] = 1;
			}
			$cur_fn_lst[$cur_fn_cnt++] = $it;
		} else {
			print "Line $.: [$it] attrib parse error.\n";
		}
	}
	$line_frm_lst[$line_n] = $line_form;
	$line_hgt_lst[$line_n] = $Line_ht[$line_form];
	$scroll_line_inf[$line_n] = $scroll_f;
	$scroll_line_enable_flag |= $scroll_f;
}

#=============================================================================
# 解析結果を構造体として出力する

sub put_func_list {
	if (defined @func_list) {
		$d_line = "";
		for ($i = 0; $i < $#func_list+1; ++$i) {
			if (defined $func_list[$i]) {
				$fn_nm = "$func_list[$i]()";
				if (length($d_line) == 0) {
					$d_line = "extern int  $fn_nm";
				} else {
					if (length($d_line)
						+ length($fn_nm) < 75) {
						$d_line .= ", $fn_nm";
					} else {
						print OFILE "$d_line;\n"; 
						$d_line = "extern int  $fn_nm";
					}
				} 
			}
		}
		if (length($d_line) > 0) {
			print OFILE "$d_line;\n";
		}
		print OFILE "\n";
	}
}

sub put_format {
	&put_func_list;
	&clear_tmp;
	for ($i = 0; $i < $line_n; ++$i) {
		next if !defined $form_txt[$i];
		$form = $form_txt[$i];
		$form =~ s/^[ ]*//;
		$l = length($&);
		if (length($form) > 0) {
			&append_tmp("\t{$l, $line_hgt_lst[$i], " .
				    "F$line_frm_lst[$i], \"$form\",},\n");
			$format_write_flg = 1;
		}
	}
	if (defined($format_write_flg)) {
		$fixfrmdef_name = "${bname}_fxfmt";
		print OFILE "struct fixedform_t far ${fixfrmdef_name}[] = {\n";
		&copy_tmp;
		print OFILE "};\n\n";
	}
}

sub put_scroll {
	&put_func_list;
	$scrddef_name = "${bname}_sd";
	print OFILE "struct scroll_data_t far ${scrddef_name}[] = {\n";
	$j = 0; $k = 0;
	for ($i = 0; $i < $line_n; ++$i) {
		if (defined($var_l_lst[$i])) {
			$j = $var_l_lst[$i];
			&get_var_atr($var_list[$j], $var_atr[$j], $i);
		} else {
			&get_var_atr("", "", $i);
		}
		$form = $form_txt[$i];
		$var_i_flg = 0;
		$var_i_flg = 1 if defined($var_i_lst[$i]);
		print OFILE "\t{$c_x, \"$form\", $var_i_flg,\n\t    ";
		$var_atr[$j] =~ /^(\d+)x(\d+)y(%\d+\.?\d*)([a-z])/;
		if ($2 == $i && $2 ne "") {
			print OFILE "$v_x, \"$v_fm\", $v_typ, ";
			print OFILE "&" if $v_typ ne 'STR';
			print OFILE "$var_name,";
			++$j;
		} else {
			print OFILE "0, NULL, 0, NULL,";
		}
		if (defined $func_list[$i]) {
			print OFILE " $func_list[$i],";
		} else {

			print OFILE " NULL,";
		}
		print OFILE "},\n";
		$var_atr[$k] =~ /^(\d+)x(\d+)y(%\d+\.?\d*)([a-z])/;

	}
	print OFILE "};\n\n";
}

sub put_scroll_area {
	if ($scroll_line_enable_flag > 0) {
		$scradef_name = "${bname}_scroll";
		print OFILE "struct scroll_area_t far ${scradef_name}[] = {\n";
		for ($i = 0; $i < $line_n; ++$i) {
			next if $scroll_line_inf[$i] != 1;
			print OFILE "\t{$line_hgt_lst[$i], F$line_frm_lst[$i],},\n";
		}
		print OFILE "};\n\n";
	}
}

sub put_var {
	if (defined @var_list) {
		$varfdef_name = "${bname}_varfmt";
		print OFILE
			"struct variableform_t far ${varfdef_name}[] = {\n";
		for ($i = 0; $i <= $#var_list; ++$i) {
			next if !defined $var_list[$i];
			&get_var_atr($var_list[$i],  $var_atr[$i], $i);
			print OFILE "\t{$v_x, $line_hgt_lst[$v_y], $v_fnt, ";
			print OFILE "&" if $v_typ ne 'STR';
			print OFILE "$var_name, $v_typ, \"$v_fm\",},\n";
		}
		print OFILE "};\n\n";
	}
}

sub get_var_atr {
#print	"n$_[2]: $_[0]\n";
#print	"a$_[2]: $_[1]\n";
	$_[0] =~ /^[\w.\[\]]+/;
	$var_name = $&;
	$_[0] =~ /:(.*)$/;
	undef $v_atr;
	$v_atr = $1;
	undef $v_fnt;
	if ($v_atr =~ s/F[0-9]$//) {
		$v_fnt = $&;
	}
	$_[1] =~ /^(\d+)x(\d+)y(%-?\d+\.?\d*)([a-z])/;
	$v_x = $1; $v_y = $2; $v_fm = $3; $v_t = $4;

	$v_fnt = "F$line_frm_lst[$v_y]" unless defined($v_fnt);
	if (length($v_atr) > 0) {
		$v_t = 'd' if $v_atr eq 'I';
		$v_t = 'f' if $v_atr eq 'F';
		$v_t = 'ld' if $v_atr eq 'L';
		$v_t = 's' if $v_atr eq 'S';
	}
	$v_fm .= $v_t;
	$v_typ = "INT" if $v_t eq 'd';
	$v_typ = "STR" if $v_t eq 's';
	$v_typ = "FLT" if $v_t eq 'f';
	$v_typ = "LNG" if $v_t eq 'ld';
	$c_x = 0;
	if (defined @cur_lst) {
		for ($iii = 0; $iii <= $#cur_lst; ++$iii) {
			$cur_lst[$iii] =~ /^(\d+)x(\d+)y/;
			if ($_[2] == $2) {
				$c_x = $1;
				last;
			}
		}
	}
}

sub put_cur {
	if (defined @cur_fn_lst) {
		$csrdef_name = "${bname}_kcft";
		print OFILE "struct csr_func_t far ${csrdef_name}[] = {\n";
		for ($i = 0; $i < $cur_cnt_p; ++$i) {
			$cur_lst[$i] =~ /^(\d+)x(\d+)y/;
			print OFILE "\t{$1, $line_hgt_lst[$2], ";
			if ($cur_fn_lst[$i] =~ /^\w+/) {
				print OFILE "$&, ";
			} else {
				print OFILE "NULL, ";
			}
			if ($cur_fn_lst[$i] =~ /\$([0-9]+)$/) {
				print OFILE "&${varfdef_name}[$1],},\n";
			} else {
				print OFILE "NULL,},\n";
			}
		}
		print OFILE "};\n\n";
	}
}

#==============================================================================
# キー定義部の解析とコード生成

sub keydef_func {
	if ($keydef_f == 0) {
		$keydef_f = 1;
		&open_tmp;
		$keydef_name = "${bname}_keyfunc";
		print TFILE "\nstruct key_func_t far ${keydef_name}[] = {\n";
	}
	if (/^\.$/) {
		$keydef_f = 0;
		print TFILE "};\n\n";
		&close_tmp;
		&copy_tmp;
		$define_func = 0;
		&put_sep_line;
	} else {
		s/,/ /g;
		s/^[ \t]+//;
		split;
		if ($key_def{$_[0]} != 1) {
			print "Line $.:$_[0] は無効なシンボルです\n";
		} else {
			print TFILE "\t{$_[0],\t$_[1],},\n";
			$_[1] ne "NULL" &&
				print OFILE "extern int $_[1]();\n";
		}
	}
}

#==============================================================================
# コメント定義部の解析とコード生成

sub comment_func {
	if ($comment_f == 0) {
		$comment_f = 1;
		print OFILE "/*\n";
	}
	if (/^\.$/) {
		$comment_f = 0;
		print OFILE "*/\n";
		$define_func = 0;
		&put_sep_line;
	} else {
		print OFILE "// $_";
	}
}

#==============================================================================
# 直接出力

sub write_func {
	if (/^\.$/) {
		$define_func = 0;
		&put_sep_line;
	} else {
		print OFILE "$_";
	}
}

sub put_sep_line {
	print OFILE "/*======================================" .
	            "=====================================*/\n\n";
}

#==============================================================================
# テンポラリファイル操作サブルーチン

sub clear_tmp {		# テンポラリファイルの消去
	unlink $tname;
}

sub open_tmp {
	open(TFILE,">$tname") || die 
		"テンポラリファイル（$tname）がオープンできません。";
}

sub close_tmp {
	close TFILE;
}

sub append_tmp {
	open(TFILE,">>$tname") || die 
		"テンポラリファイル（$tname）がオープンできません。";
	print TFILE @_;
	close TFILE;
}

sub copy_tmp {
	open(TFILE,"<$tname") || die 
		"テンポラリファイル（$tname）がオープンできません。";
	while(<TFILE>) {
		print OFILE;
	}
	close TFILE;
}

#==============================================================================
#  ヘッダの出力

sub put_header {
format OPENING =
/*===========================================================================*/
/*  SOURCE FILE: @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<            */
                 $iname
/*===========================================================================*/
.

	$ofp = select(OFILE);
	$~   = OPENING;
	write;
	select($ofp);
	print OFILE "\n#include\t\"dklib.h\"\n\n";

}

