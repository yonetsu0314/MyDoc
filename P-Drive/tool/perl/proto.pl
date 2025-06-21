#	PROTO.PL	1993.6.3	M.Fukutome
#	C �v���O�����\�[�X�t�@�C����ǂݎ��A
#	�֐��v���g�^�C�v�錾�t�@�C�����쐬����


for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
	print "\n/*\t$fname\t*/\n";	# �ǂݍ��݃t�@�C����

	$n = 0; $find = 0; $rem = 0;

	while (<F>) {
		chop;
# �R�����g�̍폜
		if ($rem != 0) {
			$ecom = index($_, '*/');
			next if ($ecom < 0);
			substr($_, 0, $ecom+2) = '';
			$rem = 0;
		}
		while (($bcom = index($_, '/*')) >= 0) {
			$ecom = index($_, '*/');
			if ($ecom < 0) {
				substr($_, $bcom) = '';
				$rem = 1;
			} else {
				substr($_, $bcom, $ecom-$bcom+2) = '';
			}
		}
		next if (/^[ \t]*$/);	# �󔒍s�͓ǂݔ�΂��B
###############################################################################
#	�֐����A�����A�錾�q���̓ǂݍ���
		if (/^[a-zA-Z_]/) {
			$line[$n++] = $_;
			$find = 1;
		} elsif ($find > 0 && /^[ \t]*[a-zA-Z_\*\)\.]+/) {
			$line[$n++] = $_;
		} elsif ($find && /^{/) {
#$deb = 1;
# �֐��Ɋւ�����̓ǂݍ��݂̏I���^�v���g�^�C�v�̐���
			$_ = join(' ', @line);
#print "$deb::::$_\n"; ++$deb;
	# ";"/"}" �܂ł��폜
			s/.*;|.*\}//g;
#print "$deb::::$_\n"; ++$deb;
	# �s���^�s���̋󔒂��폜
			s/^[ \t]+//;
			s/[ \t]+$//;
#print "$deb::::$_\n"; ++$deb;
	# �^�u�ƃX�y�[�X�̘A�����P�̃X�y�[�X�ɕϊ�
			s/[ \t]+/ /g;
#print "$deb::::$_\n"; ++$deb;
	# '(' ���̋󔒂̍폜
			s/ \(/\(/;
#print "$deb::::$_\n"; ++$deb;
	# '(' �E�̋󔒂̍폜
			s/\( /\(/g;
#print "$deb::::$_\n"; ++$deb;
	# ')' �����̋󔒂̍폜
			s/[ ]*\)[ ]*/\)/g;
#print "$deb::::$_\n"; ++$deb;
	# ',' ���̋󔒂��폜��, �E�ɂP�̋󔒂�}��
			s/[ ]*,[ ]*/, /g;
#print "$deb::::$_\n"; ++$deb;
	# '()' �̏ꍇ�@void ��ǉ�
			s/\([ ]*\)/\(void\)/g;
#print "$deb::::$_\n"; ++$deb;
	# �֐��������Ȃ��ꍇ�́Avoid ��ǉ�
			s/^([^ ]+\()/void $1/;
#print "$deb::::$_\n"; ++$deb;

			print "$_;\n" if (length($_) > 3);

			$find = 0;
			undef @line;
			$n = 0;
		} else {
			$find = 0;
			undef @line;
			$n = 0;
		}
###############################################################################
	}
	close(F);
}
