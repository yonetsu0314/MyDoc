#
#	lockco.pl	1993.12.09	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ���t�@�C������
#	�`�F�b�N�C���Ώۃt�@�C���i�������݉\�����j�݂̂̃`�F�b�N�C��
#	�R�}���h�s�� -r -u -m �I�v�V�����t���� ��������B
#		     ~~~~~~~~~~~~~~~~~~~~~~~~~
# �g�p���@
#	b:>perl lockco.pl abc.c
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
#	next unless (-w $fname);
	$rev = '';
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if (/\$Header: (\S+) *(\S+)/) {
			$rev = $2;
		}
		if (/\$Log: (\S+) */) {
			$rcs = $1;
		}
	}
	close(FILE);
	if($rev ne '') {
		print `co -r$rev -l $fname $rcs`;
	}
}
#
#	end of file
#
