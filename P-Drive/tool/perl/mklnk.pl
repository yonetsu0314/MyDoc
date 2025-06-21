# NKLNK.PL						m.fukutome 1994.5.9
# Rxxxx.H ��ǂݍ��݃����N�t�@�C���iRxxxx.LNK�j�𐶐�����

# perl mklnk.pl rXXXX.mk

while(<>) {
	s/#.*$//;		# �R�����g���폜
	if (/\$/) {		# �}�N����W�J����
		while (/\$\(([A-Za-z_0-9]+)\)/) {
			$np = $mlist{$1};
			$op = "\\\$\\\($1\\\)";
			$_ =~ s/$op/$np/g;
		}
	}
	if (/=/) {		# �}�N����`��ǂݍ���
		chop;
		s/[ \t]+$//;
		/^[ \t]*([A-Za-z_0-9]+)[ \t]*=[ \t]*(.*)$/;
		if ($1 ne 'OBJ') {
			delete $mlist{$1};
			$mlist{$1} = $2;
		}
	}
	if (/echo.*>/) {	# �����N�t�@�C�����쐬����
		s/[ \t]*echo[ \t]+//;
		s/[ \t]+$//;
		s/>>/>/;
		s/>[ \t]+/>/;
		s/[ \t]+>/>/;
		/^([^>]+)>(.+)*$/;
		if (!$lnk_flag) {
			open(LNK, ">$2");
			$lnk_flag = 1;
		}
		print LNK "$1\n";
#		print "$1\n";
	}
}
if (!$lnk_flag) {
	close(LNK);
}
