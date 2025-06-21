# cleanup.pl						1995.1.9
# カレントディレクトリから不要なソースファイルを削除する。
# .lnk ファイルに指定されているオブジェクトを生成可能な .c, .asm ファイル
# 以外の .c, .asm ファイルを削除する。
# .si ファイルは .c ファイルを検索し、定義されているファイル以外を削除する。
# ただし検索時には #include のみをキーとしているため、#if, #ifdef 等には
# 対応できていない。
# 使用方法：perl p:\tool\perl\cleanup.pl xxxx.lnk els_yyy
#	xxxx.lnk : リンクファイル実行前に make により作成しておくこと
#	els_xxx  : .si ファイルのあるディレクトリ

&conout("cleanup.pl  m.fukutome 1995/01/09\n\n");

if (defined($ARGV[1])) {
     $si_dir = $ARGV[1];
     pop ARGV;
}

if (! -e $ARGV[0]) {
     die "$ARGV[0] が見つかりません。";
}

# 削除しないファイルのリストを作成する。
while (<>) {
     chop;
     s/\+//;
     y/A-Z/a-z/;
     split;
     next if /[:\\.]/;
     $valid_file{$_[0]} = 1;
}

open(CONOUT, ">CON");
&conout(
     "ファイル削除の際に確認をするかどうかを指定します。\n" .
     "ここで N または n とタイプすると確認をせずにファイルを削除します。\n\n" .
     "ファイル削除時の確認をしますか？ (y/n) [y]\033[2D"
);
open(CONIN, "<CON");
$c = getc(CONIN);
close(CONIN);
if ($c eq 'n' || $c eq 'N') {
     $no_confirm = 1;
}
&conout("\n");

&del_file("c");
&del_file("asm");

&conout("\n.si ファイル削除の準備中です。\n\n");
while (($key, $value) = each %valid_file) {
     push(cfile, "$key.c");
}

# .si ファイルの削除処理
undef %valid_file;

if (defined($si_dir)) {
     $si_dir =~ y/A-Z/a-z/;
     $dir = "$si_dir\\";
}

while($ifile = pop(cfile)) {
     if (-e $ifile) {
          open(IFP, $ifile);
          while(<IFP>) {
               if (/^[ \t]*\#include[ \t]/) {
                    if (/[\"<](.+)\.[Ss][Ii][\">]/) {
                         $name = $1;
                         $name =~ y/A-Z/a-z/;
                         $valid_file{"$name"} = 1;
                    }
               }
          }
          close(IFP);
     }
}

&del_file(si);

# 指定した拡張子のファイルの削除処理
sub del_file {		# 引数は拡張子
     $ext = $_[0];
     system "dir $dir*.$ext > ${ext}_lst.tmp";
     open(DEL_LST, "${ext}_lst.tmp");
     while (<DEL_LST>) {
          if (/^[\w\d]/) {
               y/A-Z/a-z/;
               split;
               $_[0] =~ /([^\\:]+)$/;
               if ($valid_file{$1} != 1) {
                    &confirm_del("$dir$_[0].$ext");
               }
          }
     }
     close(DEL_LST);
}

# ファイル削除の確認を求める。デフォルトは削除しない。

sub confirm_del {	# 引数はファイル名
     if ($no_confirm != 1) {
          &conout("$_[0] を削除します。よろしいですか？(y/n) [n]\033[2D");
          open(CONIN, "<CON");
          $c = getc(CONIN);
          close(CONIN);
     } else {
          $c = 'y';
     }
     if ($c eq 'y' || $c eq 'Y') {
          chmod 0777, $_[0];
          unlink $_[0];
          &conout("$_[0] を削除しました。\n");
     }
}

sub conout {
     open(CONOUT, ">CON");
     print CONOUT $_[0];
     open(CONOUT, ">CON");
}
