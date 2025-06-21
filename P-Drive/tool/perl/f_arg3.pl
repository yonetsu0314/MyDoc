#
#	f_arg.pl	1994.03.24	R.Nishimura
#
# ���
#	/AL /Fc /Od /Gs /G2 �I�v�V�����ŃR���p�C�����Đ������ꂽ
#	COD�t�@�C����ǂݍ���ŁA�e�֐��̈����o�C�g�T�C�Y�𒲂ׂ�
#
# �g�p���@
#	b:>perl f_arg.pl *.cod > f_arg
#
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	while(<FILE>) {
		if(/\s+call\s+FAR\s+PTR\s+(\w+)\s+.*/) {
			@work = @splist;
			$exst = 0;
			foreach $wk ( @work ) {
				if($wk eq $1) {
					$exst = 1;
				}
			}
			if($exst == 0) {
				push(@splist, $1);
				printf "\n$1";
				$_ = <FILE>;
				$func = $1;
				if(/\s+add\s+sp\,(\d+)\D+/) {
					print ",$1";
					$arg{$func} = $1;
				} else {
					$arg{$func} = 0;
				}
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