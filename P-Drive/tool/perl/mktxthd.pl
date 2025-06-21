# mktxthd.pl						1994.9.1 m.fukutome
#
# 多国語対応のためのテキストヘッダファイルを作成する
#
#   perl mktxthd.pl ofile suf1 suf2 [suf3 ...]
#	ofile : 出力ファイル名
#	sufN  : 入力ファイル名修飾子
#
#   入力ファイル名 ifileN は ofile と sufN から自動生成される。生成規則は
#   ofile を xxx_yyy.zzz とすると、ifileN は xxx_sufN.zzz となる。
#   ifile1 がベースファイルになり、このファイルにある定義は全ての入力ファイル
#   で出現順序および定義内容の数の対応がとれている必要があります。ifile1 に
#   無い定義は無視されます。また各 ifile の 最初の #pragma 行までは処理の対象
#   になりません。
#   入力ファイル中の有効な定義は次の通りです。
#	1) enum
#	2) STRING
#	3) STRINGS
#	4) #define
#   これらの定義が次のような定義に変換されて出力されます。
#	コメントは全て削除されます。
#	1) enum 定義は ifile1 の内容がそのまま出力されます。
#	2) STRING(x, y) は
#		STRING(x)
#		DEFM(y)			#ifile1
#		DEFM(y)			#ifile2
#		  |			   |
#		DEFM(y)			#ifileN
#		ENDS
#		#define	x x[lang]
#	   と変換されます。
#	3) STRINGS(x)
#	   DEFM(y1)
#	     |
#	   DEFM(yN)
#	   ENDS		は
#		STRINGS(x,N)
#		DEFBLK			#ifile1
#		DEFM(y1)
#		  |
#		DEFM(y1)
#		ENDBLK
#		DEFBLK			#ifile2
#		DEFM(y1)
#		  |
#		DEFM(y1)
#		ENDBLK
#		  …
#		ENDS
#		#define	x x[lang]
#	   と変換されます。
#	4) #define x y  は
#		STRING(x)
#		DEFM(y)			#ifile1
#		DEFM(y)			#ifile2
#		  |			   |
#		DEFM(y)			#ifileN
#		ENDS
#		#define	x x[lang]
#	   と変換されます。
#
#============================================================================
($dmax = @ARGV) < 2 && die "入力ファイル名が指定されていません。\n";
$dmax > 5 && die "入力ファイルが多すぎます。\n";
$dmax < 1 && die "出力ファイル名が指定されていません。\n";

undef $jpn_f;
$oname = $ARGV[0];
$bnm = $oname;
$bnm =~ s/\.(.+)$//;
$ext = $1;
$bnm =~ s/_.*//;

for ($i = 1; $i < $dmax; ++$i) {
	$iname[$i-1] = "${bnm}_$ARGV[$i].$ext";
	-s $iname[$i-1] ||
	die "ファイル（$iname[$i-1]）が存在しないか、またはサイズが 0 です。\n";
	$iname[$i-1] =~ /_jpn\./i && ($jpn_f = 1);
}
--$dmax;

@ifpp = ("BFP", "IFP1", "IFP2", "IFP3", "IFP4");

#============================================================================

# テンポラリファイル名の作成。環境変数 TMP または TEMP で定義されている
# ディレクトリを使用

$tname	=	"mktxthd.tmp";	# テンポラリファイル名

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

#============================================================================

&setup_ifp;

open(OFP,">$oname") || die "ファイル（$oname）がオープンできません。\n";

$old_fp = select(OFP);

print "/* Original Files:\n";
for ($i = 0; $i < $dmax; ++$i) {
	print "$header[$i]";
}
print "*/\n\n";

#============================================================================
# 日本語用ファイルが含まれている場合、シンボル USE_JPN を定義する
# （仮名キー処理を有効にするため）

print "#define\tUSE_JPN\n" if defined $jpn_f;
print "#define\tLANG_MAX\t($dmax)\n\n";

#============================================================================

while (<BFP>) {
	&delete_rem(0);
	if (/^enum/) {
		&enum_func;
	} elsif (/^STRING\(/) {
		&string_func;
	} elsif (/^STRINGS\(/) {
		&strings_func;
	} elsif (/^#define[ \t]/) {
		&define_func;
	}
}

print "/*======================== EOF ===============================*/\n";

&cleanup_ifp;
select($old_fp);
close(OFP);
close(BFP);

#============================================================================
sub enum_func {			# enum 定義の処理
	print;
	while (<BFP>) {
		&delete_rem(0);
		print "\t$_" if /[a-z0-9_]/i;
		/};/ && last;
	}
	print "};\n\n";
}

sub string_func {		# STRING 定義の処理
	s/^[ \t]+//;
	s/[ \t]+$//;
	/STRING\([ \t]*([a-zA-Z0-9_]+)[ \t]*,[ \t]*(.+)\)$/;
	$str_nm = $1;
	$str_def = $2;
	print "STRING($str_nm)\n";
	print "\tDEFM($str_def)\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/STRING\([ \t]*$str_nm[ \t]*,[ \t]*(.+)\)$/) {
				print "\tDEFM($1)\n";
				$find_flg = 1;
				last;
			}
		}
		&close_ifp;
		$find_flg == 1 ||
			die "$iname[$i] に $str_nm がありません。\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

sub strings_func {		# STRINGS 定義の処理
	s/^[ \t]+//;
	s/[ \t]+$//;
	/STRINGS\([ \t]*([a-zA-Z0-9_]+)[ \t]*\)$/;
	$str_nm = $1;
	&clear_tmp;
	&open_tmp;
	$defm_cnt = 0;
	while (<BFP>) {
		&delete_rem(0);
		/^ENDS$/ && last;
		if (/^DEFM/) {
			print TFP "\t$_";
			++$defm_cnt;
		}
	}
	&close_tmp;
	print "STRINGS($str_nm,$defm_cnt)\n";
	print "    DEFBLK\n";
	&copy_tmp;
	print "    ENDBLK\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/STRINGS\([ \t]*$str_nm[ \t]*\)$/) {
				$find_flg = 1;
				last;
			}
		}
		$find_flg == 1 ||
			die "$iname[$i] に $str_nm がありません。\n";
		print "    DEFBLK\n";
		$defm_n = 0;
		while (<$ifp>) {
			&delete_rem($i);
			/^ENDS$/ && last;
			if (/^DEFM/) {
				print "\t$_";
				++$defm_n;
			}
		}
		&close_ifp;
		print "    ENDBLK\n";
		$defm_cnt != $defm_n &&
			die "$iname[$i] の $str_nm の定義数が異なります。\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

sub define_func {		# #define 定義の処理
	s/^[ \t]*#define[ \t]+//;
	s/[ \t]+$//;
	/^([a-zA-Z0-9_]+)[ \t]+(.+)$/;
	$str_nm = $1;
	$str_def = $2;
	print "STRING($str_nm)\n";
	print "\tDEFM($str_def)\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/#define[ \t]*$str_nm[ \t]*(.+)$/) {
				print "\tDEFM($1)\n";
				$find_flg = 1;
				last;
			}
		}
		&close_ifp;
		$find_flg == 1 ||
			die "$iname[$i] に $str_nm がありません。\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

#==============================================================================
# コメント削除
sub delete_rem {
	$rem_n = shift(@_);
	$rem = $_;
	if ($rem_f[$rem_n] != 0) {
		if (($end = index($_,"*/")) >= $[) {
			$_ = substr($_,$end+2);
			$rem_f[$rem_n] = 0;
		} else {
			s/^.*$//;
			return;
		}
	}
	while (($beg = index($_,"/*")) >= $[) {
		if (($end = index($_,"*/")) >= $[) {
			$_ = substr($_,0,$beg).substr($_,$end+2);
		} else {
			$rem_f[$rem_n] = 1;
			$_ = substr($_,0,$beg);
		}
	}
	s/^[ \t]+//;
	s/[ \t]+$//;
}

#==============================================================================
# 入力ファイル操作サブルーチン

sub setup_ifp {
	for ($ii = 0; $ii < $dmax; ++$ii) {
		open($ifpp[$ii], "<$iname[$ii]") ||
			die "ファイル（$iname[$ii]）がオープンできません。\n";
		$rem_f[$ii] = 0;
		$header[$ii] = "$iname[$ii]\n";
	}
	for ($ii = 0; $ii < $dmax; ++$ii) {
		$ifp = $ifpp[$ii];
		while (<$ifp>) {
			&delete_rem($ii);
			if ($rem =~ /\$Header.*\$/) {
				$rem =~ s/^.*\$Header:?[ \t]*//;
				$rem =~ s/[ \t]*Exp[ \t]*\$.*$//;
				$header[$ii] = $rem;
			}
			/#pragma/ && last;
		}
		print "$header[$ii]";
	}
}

sub open_ifp {
	$ii = shift(@_);
#	open($ifp, "<$iname[$ii]") ||
#		die "ファイル（$iname[$ii]）がオープンできません。\n";
	$ifp = $ifpp[$ii];
}

sub close_ifp {
}

sub cleanup_ifp {
	for ($ii = 1; $ii < $dmax; ++$ii) {
		close($ifpp[$ii]);
	}
}

#==============================================================================
# テンポラリファイル操作サブルーチン

sub clear_tmp {		# テンポラリファイルの消去
	unlink $tname;
}

sub open_tmp {
	open(TFP,">$tname") || die 
		"テンポラリファイル（$tname）がオープンできません。";
}

sub close_tmp {
	close TFP;
}

sub copy_tmp {
	open(TFP,"<$tname") || die 
		"テンポラリファイル（$tname）がオープンできません。";
	while(<TFP>) {
		print;
	}
	close TFP;
}

#==============================================================================
