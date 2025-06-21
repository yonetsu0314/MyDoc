#
#	t_stk.pl	1994.03.28	R.Nishimura
#
# ���
#	/AL /Fc /Od /Gs /G2 �I�v�V�����ŃR���p�C�����Đ������ꂽ
#	COD�t�@�C����ǂݍ���ŁA�^�X�N�̐ݒ�X�^�b�N�T�C�Y�𒲂ׂ�
#
# �g�p���@
#	b:>perl t_stk.pl *.cod > t_stk
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/\s+mov\s+WORD\s+PTR\s+\[bp\-10\]\,\s*SEG\s+(\w+)\W+/) {
			$tsk = $1;
		}
		if(/\s+tsk_pkt\.stk_size\s+/) {
			$_ = <FILE>;
			$_ = <FILE>;
			if(/\s+mov\s+WORD\s+PTR\s+\[bp\-8\]\,(\d+)\D+/) {
				print "$tsk,$1\n";
				$stk{$tsk} = $1;
			}
		}
	}
	close(FILE);
}
print "\n";
#
#	end of file
#
