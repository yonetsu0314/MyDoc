#
#	chg_id.pl	1993.07.12	R.Nishimura
#
#  �@�\�F
#	�啶�����������ɕϊ�����A
#
#�@���[���P�F	[A-Z] ---> [a-z]
#
#�@���ӁF
#	 �R�����g����\����������ϊ������B
#
for $fname (@ARGV) {
	unless (open(F, $fname)) {
		warn "$fname: $!\n";
		next;
	}
#	print "\n/*\t$fname\t*/\n";	# �ǂݍ��݃t�@�C����
	while (<F>) {
		while(/[A-Z]/) {
			s/([A-Z])/\l$1/g;
		};
		print;
	}
	close(F);
}
