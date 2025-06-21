This is jperl, version 4.036 + 1.4 Oct 31 1993

$RCSfile: perl.c,v $$Revision: 4.0.1.8 $$Date: 1993/02/05 19:39:30 $
Patch level: 36

Copyright (c) 1989, 1990, 1991, Larry Wall
MS-DOS port Copyright (c) 1989, 1990, Diomidis Spinellis

Perl may be copied only under the terms of either the Artistic License or the
GNU General Public License, which may be found in the Perl 4.0 source kit.

------------------------------------------------------

Larry Wall氏によるperl(Practical Extraction and Report Language)の
日本語対応版(MS-DOS版の実行形式と、ソースのpatch)です。

jperl14.lzh(MS-DOS版実行形式)には以下のファイルが含まれます。
	readme.sj     - このファイル
	jperl.exe     - 日本語対応perlのMS-DOS用実行形式
	jperl         - 〃 MS-DOS用GO32版の実行形式
	perlglob.exe  - ファイル名置換(glob)を行うとき使用される
	cpppp.sed     - -Pオプション時に使用されるsedスクリプト
	cpp.exe       - 〃 C PreProcessor
	jperl14.dif   - ソースのcontext diff

patchにはunix用のものもMS-DOS用のものも全て含まれています。
		patch -p -N -d PERLDIR < jperl14.dif
あるいはperlのソースのあるディレクトリで
		patch -p -N < jperl14.dif
としてください。

MSDOS用の実行形式を再コンパイルするには、msdosディレクトリ下にあるファ
イルをperlディレクトリにコピーし、config_h.ms(MS-C用)または
config_h.dj(djgcc用)のいずれかをconfig.hにrenameまたはcopyして使用して
ください。

MS-DOS版実行形式はMicrosoft Visual C++ 1.00を使用して作成し、test
scriptの殆ど(MS-DOS上では動きそうもないものを除き、ファイル名やechoの
仕様の違い等を修正したもの)をパスしました。

●対応文字について

本ドキュメントではひとからげに“漢字”あるいは“日本語”と言い表してい
ますが、jperlが扱える文字種は起動オプションによって

	-Lsjis		日本語Shift-JIS
	-Leuc		日本語EUC
	-Ltca		中国語(台湾)TCA
	-Lkseuc		韓国語EUC(KS C5601-1987)
	-Llatin		1バイト文字のみ

の5通りに切り替わります。-Lオプションを省略したときの文字種はコンパイ
ル時に決定されます。MSDOS用の実行形式では日本語Shift JISとなっています。


●漢字対応の範囲

・文字列等
	Shift-JISのとき文字列中の漢字の2バイト目はメタキャラクタと
	衝突しません。
	tokenに漢字を使うことはできません。
	文字列の大小関係は、Cライブラリ関数memcmpに依存します。
	ライブラリ関数のmemcmpがsigned charで比較している場合、
	"A" gt "Ａ"("A"のほうが大きい)となってしまいます。
	このようなときは、config.hで#undef HAS_MEMCMPしてください。
	Microsoft Cではunsigned charですが、NEWS-OSなどでは
	signed charでした。

・正規表現
	.        は1バイト/2バイトいずれかの“1文字”と
	[aAａＡ] はアルファベットのエーと
	[ぁ-ん]  はひらがなと
	うあ+    は“うあ”,“うああ”,“うあああああああ”等とマッチします

・tr, y
	tr/Ａ-Ｚ/A-Z/; のように2バイト文字も“1文字”として扱います。
	また、オプションとして'B'を追加しています。
	このオプションが付けられた場合、SEARCHLIST, REPLACEMENT,
	及び置換の対象となる文字列の漢字を意識しません。
	以下の例はEBCDICコードの数字をASCIIに変換します。
		y/\360-\371/\060-\071/B;

・format
	formatされた文字列が漢字境界で切れてしまうような場合、
	' 'をpaddingして揃えます。
	同様に、"..."で終る場合"... "が付けられます。

・chop
	chop は末尾の文字が漢字であるとき、その漢字1文字をはぎ取ります。
	返す文字は漢字1文字(2バイト長)になります。
	もし、末尾の文字が漢字の1バイト目となっているような場合、
	その1バイトをはぎ取ります。
	perl.manの-lオプションの項に示されている80カラムで切り揃える例は
	漢字に対応するためには以下のようにすることになります。
		jperl -lpe 'substr($_, 81)="";chop;'

・split
	split(//) は漢字を意識します。これによって生成される配列は
	バイト単位ではなく文字単位で分割されたものとなります。
	以下の例は、文字列の“文字数”を返します。

		sub jlength {split(//, pop(@_)) + 0;}

	バイト単位での分割が必要な場合はunpackかsubstrを使ってください。

・substr
	substrは漢字を意識*しません*。
	漢字を意識して$off文字目から$len文字切り取るサブルーチンは
	以下のように書くことができます。
		sub jsubstr {
			local($str, $off, $len) = @_;
			local(@s);
			@s = split(//,$str);
			$len = $#s unless $len;
			return join("", @s[$off..($off+$len-1)]);
		}

・index, rindex
	いずれも漢字を意識した検索をおこないますが、
	返す値はバイト位置です。文字位置を返すようなサブルーチンは
	以下のように書くことができます。
		sub jindex {
			local($str, $subs, $pos) = @_;
			local($i);
			return $i if (($i = index($str, $subs, $pos)) < 0);
			return &jlength(substr($str, $[, $i-$[)) + $[;
		}

・-B, -T
	ファイルテスト演算子のうち、-T, -Bでは、2バイト文字を構成する
	charをodd characterとみなしません。

・reverse
	スカラーコンテキストでのreverseは漢字を意識*しません*。
	漢字を意識してreverseするには

		sub jreverse {join('', reverse(split(//, pop(@_))));}

	のようにしてください。


●MS-DOS版
	コマンドラインはシングルクォート(')でも括れますが、
	| や > 、漢字空白等のcommand.comに解釈されてしまう
	文字はダブルクォート(")で括るようにしてください。
	また、file.[ch]やmanual.{man,doc,txt}のような
	csh風のglobbingが可能です(perlglob.exeも同様)。
	GO32版ではこの機能はサポートされていません。

	-Pオプションを使う場合には、PATHの通っているところに
	sedとcppが必要です。jperlは、perlスクリプトを読む前に
	sed -f cpppp.sed scriptfile | cpp -DMSDOS -I/usr/local/lib/perl
	を実行します。cpppp.sedをsedが読み込める場所に置いてください。
	添付のcpp.exeは、エル・エス・アイ ジャパン(株)さんのLSI-C試食版に
	含まれているものです。

	globbing (while (<*.c>) {...}のような) を使用する場合には
	perlglob.exe をPATHの通っているところに置いておいてください。
	cshのような[..]および{,}が使えます。これはjperl.exeの
	コマンドライン上でも同様です。
	ソースはmsdos/glob.cとなっており、makeするとglob.exeが
	できますが、perlglob.exeはこれをrenameしたものです。

	system()や``では system("/bin/ls > /tmp/tmp$$"); のように
	pathの句切りに'/'を使っても正しく動作しますが、
	command.comの内部コマンドを実行しようとする場合
	switch characterによっては正しく動作できません。
	また 2>&1 のようにファイルハンドルの切り替えもサポートされて
	います。
		$msg = `cmd -invalid_option 2>&1`;
	は、(たぶん)cmdのエラーメッセージを$msgへ代入します。
	GO32版ではこの機能はサポートされていません。

	crypt()とNDBMがサポートされています。
	crypt()は森公一郎(kmori)氏の作品で、unixのそれと互換性があります。
	NDBMライブラリはOzan Yigit氏のsdbmをMS-DOSに移植したものです。

●ソース
	以下のファイルが増えています。

		config_h.dj      djgcc用のconfig.h
		makefile.dj      djgcc用のMakefile
		config_h.ms      MSC用のconfig.h
		makefile.ms      MSC用のMakefile
		kanji.[ch]       漢字処理用
		msdos/djdos.c    djgcc用の補助ファイル
		msdos/rand.c     randomを呼び出すインターフェース
		msdos/random.c   乱数ジェネレータ
		msdos/xargs.[ch] ワイルドカード展開
		msdos/cpppp.sed  -Pオプション使用時にsedが使うスクリプト
		msdos/crypt.c    crypt()。森公一郎(kmori)氏の作品
		msdos/ndbm.[ch]  NDBMライブラリ。Ozan Yigit氏のsdbmを
		                 MS-DOSに移植したもの
		msdos/xsystem.c  値を返すsystem()

●バグ
	formatの"^"フィールドは、禁則を行いません。
	$: に漢字をセットしても無視されます。

	$] はオリジナルと同じ値を返します。

	MS-DOS版は80[123]?87を使えません。
	オプション(-FPa)を変更して再コンパイルする必要が有ります。

	jperlは、Junker's Pathologically Eclectic Rubbish Listerの略
	ではありません。

----_--__---_-_-_-__--_-__-__---_-_----_--_-_---_---_-----
田中serow良知
pcs19614 (ascii-net pcs)
serow (mix)
