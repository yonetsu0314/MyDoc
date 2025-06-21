#
#	txtcut.pl		1996.04.17	R.Nishimura
#
# ���
#	�b�b�v���^�q�b�t�\�t�g�̂s�w�s�t�@�C�����當���񕔕��𒊏o����
#
# �g�p���@
#	b:>perl txtcut.pl arc.txt
#
#
$cut = 0;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	print "-------------------------------------------\n";
	print "$fname\n";
	print "-------------------------------------------\n";
	while(<FILE>) {
		if (/^STRING[^S]/) {
			$cut = 0;
			print;
		} elsif (/^STRINGS/) {
			$cut = 1;
			print;
		} elsif ($cut == 1) {
			print;
			if(/^ENDS/) {
				$cut = 0;
			}
		}
	}
	close(FILE);
}
#
#	end of file
#
