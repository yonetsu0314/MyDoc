# g_pre.pl							98.9.28 Hiroe Sasamoto

# original: 
# バックアップデータエリアを計算する為のソースリストを作成する。

# perl g_pre.pl ifiles > odir
#   ifiles : 入力ファイル（pre_rw.c）
#   odir   : 出力ディレクトリ

$id = 0;
$lst = "char *b_name[] = {\n";

print "static\tint dummy_w;\n\n";
print "static\tunsigned char sum;\n\n";
while (<>) {
	if ($id == 0) {
		if (/^static/) {
			$id = 1;
		} elsif (/^\#/) {
			print;
		}
	} else {
		if ($id > 1) {
			print;
			s/sizeof\((.+)\)/"$1"/;
			$lst .= $_;			
			if (/\};/) {
				last;
			}
		} elsif (/static.+setup_size\[\]/) {
			$id = 2;
			print "\n$_";
		}
	}
}

print "\n$lst\n";
print	"int main()\n{\n".
	"\tlong\tt_tmp = 0, total = 0;\n\tint\ti;\n\n" .
	"\tfor(i = 0; i < sizeof(setup_size)/sizeof(setup_size\[0\]); ++i) {\n" .
	"\t\tprintf(\"%-32s = %5d\\n\", b_name[i], setup_size[i]);\n" .
	"\t\tt_tmp += (setup_size[i] + sizeof(sum));\n\t}\n";

print "\tprintf(\"======================\\n\");\n";
print "\tt_tmp = (t_tmp / 256 + 1) * 256;\n";
print "\tprintf(\"setup  : %5ld\\n\", t_tmp);\n";
print "\ttotal += t_tmp;\n\n";
print "\tt_tmp = (long)(sizeof(PRE) + sizeof(sum)) * (long)PRESET_MAX;\n";
print "\tt_tmp = ((t_tmp + (128 - 1)) / 128 * 128);\n";
print "\tprintf(\"preset : %5ld\\n\", t_tmp);\n";
print "\ttotal += t_tmp;\n\n";
print "\tt_tmp = sizeof(TOTAL_T) * TOTAL_MAX;\n";
print "\tt_tmp = ((t_tmp + (128 - 1)) / 128 * 128);\n";
print "\tprintf(\"total  : %5ld\\n\", t_tmp);\n";
print "\ttotal += t_tmp;\n\n";
print "#ifdef\t__HOST__\n";
print "\tt_tmp = (sizeof(ERROR_T) + sizeof(sum)) * ERROR_MAX;\n";
print "\tt_tmp = ((t_tmp + (128 - 1)) / 128 * 128);\n";
print "\tprintf(\"err text: %5ld\\n\", t_tmp);\n";
print "\ttotal += t_tmp;\n";
print "#endif\n\n";
print "\tprintf(\"======================\\n\");\n";
print "\ttotal += 2048;\n";
print "\t/*メモリーをどのように使うかを覚える為の領域*/\n";
print "\t/*または、キーを管理する情報(256byte) 2000byte ≒ 2048byte*/\n";
print "\tprintf(\"memory  : %5ld\\n\", total);\n\treturn 0;}\n";
