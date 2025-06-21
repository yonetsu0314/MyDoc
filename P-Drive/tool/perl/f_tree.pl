#
#	f_tree.pl	1994.03.24	R.Nishimura
#
# ���
#	/AL /Fc /Od /Gs /G2 �I�v�V�����ŃR���p�C�����Đ������ꂽ
#	COD�t�@�C����ǂݍ���ŁA�e�֐����Ăяo���Ă���֐����𒲂ׂ�
#
# �g�p���@
#	b:>perl f_tree.pl *.cod | sort > f_tree
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/^(\w+)\s+PROC\s+.*/) {
			print "\n$1";
			@splist = ();
		}
		if(/\s+call\s+FAR\s+PTR\s+(\w+)\s+.*/) {
			@work = @splist;
			$exst = 0;
			foreach $wk (@work) {
				if($wk eq $1) {
					$exst = 1;
				}
			}
			if($exst == 0) {
				push(@splist, $1);
				printf ",$1";
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