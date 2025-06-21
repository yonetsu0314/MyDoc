#
#	romlst11.pl	2000.12.20	Hiroe Konishi
#
# 解説
#	= と - と | を使って作成されたソフト番号一覧表をhtmlファイルに変換する。
#	ただし、ソフト番号一覧表は、
#	次のルールが守られている事。
#	１．ファイル全体のタイトルは、《》で括ること。
#	２．ファイル内のタイトルは、【】で括ること。
#	    タイトルの英文は[]で括り、【】内に入れること。
#	３．表は、2つ以上の = と 1つの | で囲むこと。
#	    【 注意 】 2つ以上の = を表中の仕切りに使っては行けない。
#	４．表中の仕切りは、2つ以上の - を使うこと。
#	    【 注意 】 2つ以上の - を表の仕切り以外に使っては行けない。
#
# 使用方法
#	perl romlst111.pl m_rcu.txt m_rcu.htm
#	romlst12.pl で生成される目次ファイルを表の初めにつけてロム番号リストを生成する。

$id = 0;
$tid = 0;
$ofile = shift(@ARGV);
$tfile = "test.tmp";
open(OFP, ">$ofile") || die "Can't open file ($ofile)\n";

while(<>){
	s/\s{2,}//g;
	s/[　]//g;
	s/,/+/g;
	s/</(/g;
	s/>/)/g;
	if ($id == 1) {
		s/^[|]/<td WIDTH=/;
		s![|]$!></td>!;
		s![|]!></td><td WIDTH=!g;
		
		s!(SOFT.\s*No)!\"76\"><center>$1</center!;
		s!(機\s*種)!\"200\"><center>$1</center!;
		s!(MODEL)!\"200\"><center>$1</center!;
		s!(Ether)!\"47\"><center>$1</center!;
		s!(仕\s*様)!\"363\"><center>$1</center!;
		s!(LANG)!\"76\"><center>$1</center!;
		s!(ASIA)!\"76\"><center>$1</center!;
		s!(旧)!\"76\"><center>$1</center!;
		s!(IES)!\"76\"><center>$1</center!;
		s!(IEN)!\"76\"><center>$1</center!;
		s!(IEE)!\"76\"><center>$1</center!;
		s!(VER)!\"47\"><center>$1</center!;
		s!(\w*\s*SUM\s*[\w\(\)]*)!\"47\"><center>$1</center!;
		s!(SPECIFICATIONS)!\"363\"><center>$1</center!;
		
	} else {
		s/^[|]/<td>/;
		s![|]$!</td>!;
		s![|]!</td><td>!g;
	}

#	s!R(\d)(\d+)(\w*)!<a href=\"file:///o|/card.dir/r$1.dir/r$1$2.txt\">${NGS}R$1$2$3${NGE}</a>!g;
#	s/r1.dir/r.dir/g;
#	s/r0.dir/r.dir/g;

	s!xS(\d)(\d+)(\w*)!<a href=\"file:///o|/card/s$1/s$1$2.txt\"><font color="#ff0000"><del>s$1$2$3</del></font></a>!g;
	s!\*S(\d)(\d+)(\w*)!<a href=\"file:///o|/card/s$1/s$1$2.txt\"><font color="#ff0000">s$1$2$3</font></a>!g;
	s!S(\d)(\d+)(\w*)!<a href=\"file:///o|/card/s$1/s$1$2.txt\">S$1$2$3</a>!g;

	s!xR(\d)(\d+)(\w*)!<a href=\"file:///o|/card/r$1/r$1$2.txt\"><font color="#ff0000"><del>r$1$2$3</del></font></a>!g;
	s!\*R(\d)(\d+)(\w*)!<a href=\"file:///o|/card/r$1/r$1$2.txt\"><font color="#ff0000">r$1$2$3</font></a>!g;
	s!R(\d)(\d+)(\w*)!<a href=\"file:///o|/card/r$1/r$1$2.txt\">R$1$2$3</a>!g;

	s!/r1/!/r/!g;
	s!/r0/!/r/!g;

	s!xT(\d)(\d+)(\w*)!<a href=\"file:///o|/card/t$1/t$1$2.txt\"><font color="#ff0000"><del>t$1$2$3</del></font></a>!g;
	s!\*T(\d)(\d+)(\w*)!<a href=\"file:///o|/card/t$1/t$1$2.txt\"><font color="#ff0000">t$1$2$3</font></a>!g;
	s!T(\d)(\d+)(\w*)!<a href=\"file:///o|/card/t$1/t$1$2.txt\">T$1$2$3</a>!g;

	s!/t1/!/t/!g;
	s!/t0/!/t/!g;

	s!xW(\d)(\d+)(\w*)!<a href=\"file:///o|/card/w$1/w$1$2.txt\"><font color="#ff0000"><del>w$1$2$3</del></font></a>!g;
	s!\*W(\d)(\d+)(\w*)!<a href=\"file:///o|/card/w$1/w$1$2.txt\"><font color="#ff0000">w$1$2$3</font></a>!g;
	s!W(\d)(\d+)(\w*)!<a href=\"file:///o|/card/w$1/w$1$2.txt\">W$1$2$3</a>!g;

	s!/w1/!/w/!g;
	s!/w0/!/w/!g;
	s!特ﾎｯﾊﾟhspec(\d+)!<a href=\"file:///p|/bbs/hopper.htm\#hspec$1\">特ﾎｯﾊﾟhspec$1</a>!g;

	if (/[=]{2,}/) {
		if ($id == 0) {
			print OFP "<table BORDER COLS=5 WIDTH=\"950\" >\n";
			$id++;
		} else {
			if ($id == 1) {
				$id++;
			} else {
				print OFP "</table>\n";
				$id = 0;
			}
		}
	} elsif (/[-]{2,}/) {
	} elsif (/《(.*)》/) {
		print OFP "<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n";
		print OFP "<html>\n";
		print OFP "<head>\n";
		print OFP "   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\">\n";
		print OFP "   <meta name=\"GENERATOR\" content=\"WZ Editor4.0\">\n";
		print OFP "   <title>$1</title>\n";
		print OFP "</head>\n";
		print OFP "<body>\n";
		open(TFP, "<$tfile") || die "Can't open file ($tfile)\n";
		while (<TFP>) {
			print OFP;
		}
		close(TFP);
	} elsif (/【(.*)】/) {
		print OFP "<hr WIDTH=\"100%\"><h3><a name=\"std${tid}\"></a>【$1】</h3>\n";
		$tid++;
#	} elsif (/\$Header:/) {
#		print OFP "<! $_ >\n";
	} elsif ($_) {
		print OFP "<tr>$_</tr>\n";
	} else {
		print OFP "\n";
	}
}

print OFP "</body>\n";
print OFP "</html>\n";
close(OFP);
