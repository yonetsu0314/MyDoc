#
#	romlst11.pl	2000.12.20	Hiroe Konishi
#
# ���
#	= �� - �� | ���g���č쐬���ꂽ�\�t�g�ԍ��ꗗ�\��html�t�@�C���ɕϊ�����B
#	�������A�\�t�g�ԍ��ꗗ�\�́A
#	���̃��[��������Ă��鎖�B
#	�P�D�t�@�C���S�̂̃^�C�g���́A�s�t�Ŋ��邱�ƁB
#	�Q�D�t�@�C�����̃^�C�g���́A�y�z�Ŋ��邱�ƁB
#	    �^�C�g���̉p����[]�Ŋ���A�y�z���ɓ���邱�ƁB
#	�R�D�\�́A2�ȏ�� = �� 1�� | �ň͂ނ��ƁB
#	    �y ���� �z 2�ȏ�� = ��\���̎d�؂�Ɏg���Ă͍s���Ȃ��B
#	�S�D�\���̎d�؂�́A2�ȏ�� - ���g�����ƁB
#	    �y ���� �z 2�ȏ�� - ��\�̎d�؂�ȊO�Ɏg���Ă͍s���Ȃ��B
#
# �g�p���@
#	perl romlst111.pl m_rcu.txt m_rcu.htm
#	romlst12.pl �Ő��������ڎ��t�@�C����\�̏��߂ɂ��ă����ԍ����X�g�𐶐�����B

$id = 0;
$tid = 0;
$ofile = shift(@ARGV);
$tfile = "test.tmp";
open(OFP, ">$ofile") || die "Can't open file ($ofile)\n";

while(<>){
	s/\s{2,}//g;
	s/[�@]//g;
	s/,/+/g;
	s/</(/g;
	s/>/)/g;
	if ($id == 1) {
		s/^[|]/<td WIDTH=/;
		s![|]$!></td>!;
		s![|]!></td><td WIDTH=!g;
		
		s!(SOFT.\s*No)!\"76\"><center>$1</center!;
		s!(�@\s*��)!\"200\"><center>$1</center!;
		s!(MODEL)!\"200\"><center>$1</center!;
		s!(Ether)!\"47\"><center>$1</center!;
		s!(�d\s*�l)!\"363\"><center>$1</center!;
		s!(LANG)!\"76\"><center>$1</center!;
		s!(ASIA)!\"76\"><center>$1</center!;
		s!(��)!\"76\"><center>$1</center!;
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
	s!��ί��hspec(\d+)!<a href=\"file:///p|/bbs/hopper.htm\#hspec$1\">��ί��hspec$1</a>!g;

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
	} elsif (/�s(.*)�t/) {
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
	} elsif (/�y(.*)�z/) {
		print OFP "<hr WIDTH=\"100%\"><h3><a name=\"std${tid}\"></a>�y$1�z</h3>\n";
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
