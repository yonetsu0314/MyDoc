#
#	cutmess.pl		1995.11.08	R.Nishimura
#
# ���
#	DACS�������`�\�[�X�t�@�C����ǂݍ���ŕ������`�s�𒊏o����B
#	����́A������e�[�u���t�@�C�����쐬���� genmstbl.pl �Œ��o����
#	���Ȃ����Ƃ��m�F����ׂ̂��̂ł���B
#
#
# �g�p���@
#	b:>perl cutmess.pl jpn*.src
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/'.+'/) {
			print;
		}
	}
	close(FILE);
}
#
#	end of file
#
