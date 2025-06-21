#
#	f_auto.pl	1994.03.24	R.Nishimura
#
# ���
#	/AL /Fc /Od /Gs /G2 �I�v�V�����ŃR���p�C�����Đ������ꂽ
#	COD�t�@�C����ǂݍ���ŁA�e�֐��̓��I�ϐ��o�C�g�T�C�Y�𒲂ׂ�
#
# �g�p���@
#	b:>perl f_auto.pl *.cod > f_auto
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/^(\w+)\s+PROC\s+.*/) {
			print "\n$1,$fname";
		}
		if(/\s+enter\s+(\d+)\D+/) {
			print ",$1";
		}
	}
	close(FILE);
}
print "\n";
#
#	end of file
#
