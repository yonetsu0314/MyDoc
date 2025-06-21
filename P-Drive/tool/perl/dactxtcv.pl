#
#	dactxtcv.pl	1996.08.05	R.Nishimura
#
#  �@�\�F
#	�c�`�b�r�|�v�̕�����u�b�N���炔�����`���ŏo�͂��ꂽ
#	�t�@�C����ϊ�����
#
for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
	print	"; \$Header\$\n";
	while (<F>) {
		while(/\"\|/) {
			s/\"\|/\'/g;
		};
		while(/\|\"/) {
			s/\|\"/\'/g;
		};
		while(/\"\"/) {
			s/\"\"/\"/g;
		};
		while(/\|/) {
			s/\|/\'/g;
		};
		print;
	}
	close(F);
}
