#
#	genmstbl.pl		1995.11.08	R.Nishimura
#
# ���
#	DACS�������`�\�[�X�t�@�C����ǂݍ���ŕ�����e�[�u���t�@�C����
#	�쐬������̂ł���B
#	cutmess.pl �Œ��o�������̂Ɣ�r����ꍇ�ɂ́A
#		print "'--------------------------------'\n";
#	���̍s���R�����g�ɂ���B
#
# �g�p���@
#	b:>perl genmstbl.pl jpn*.src
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/(.+):.*'(.+)'.*/) {
			$mes = "|$2|";
			$com = $1;
			write;
		} elsif (/.*'(.+)'>,(\d\d\d\S+)/) {
			print "\"|--------------------------------|\"\n";
			$mes = "|$1|";
			$com = $2;
			write;
		} elsif (/.*'(.+)'>,(\S+)/) {
			$mes = "|$1|";
			$com = $2;
			write;
		} elsif (/'(.+)'/) {
			$mes = "|$1|";
			$com = "";
			write;
		}
	}
	close(FILE);
}
#
format	STDOUT =
#123456789012345678901234567890123456789012
"@<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<","@<<<<<<<<<<<<<<<<<<<<<<<<<"
$mes,$com
.
#
#	end of file
#
